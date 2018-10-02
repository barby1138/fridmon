/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/mib_next.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

/****************************************************************************
 *
 *  *** Restricted Rights Legend ***
 *
 *  The programs and information contained herein are licensed only
 *  pursuant to a license agreement that contains use, reverse
 *  engineering, disclosure, and other restrictions; accordingly, it
 *  is "Unpublished--all rights reserved under the applicable
 *  copyright laws".
 *
 *  Use duplication, or disclosure by the Government is subject to
 *  restrictions as set forth in subparagraph (c)(1)(ii) of the Rights
 *  in Technical Data and Computer Licensed Programs clause of DFARS
 *  52.227 7013.
 *
 *  Copyright 1988-1997 Epilogue Technology Corporation.
 *  All rights reserved.
 *
 *  *** Government Use ***
 *
 *  The Licensed Programs and their documentation were developed at
 *  private expense and no part of them is in the public domain.
 *
 *  The Licensed Programs are "Restricted Computer Software" as that
 *  term is defined in Clause 52.227-19 of the Federal Acquisition
 *  Regulations (FAR) and are "Commercial Computer Software" as that
 *  term is defined in Subpart 227.401 of the Department of Defense
 *  Federal Acquisition Regulation Supplement (DFARS).
 *
 *  (i) If the licensed Programs are supplied to the Department of
 *      Defense (DoD), the Licensed Programs are classified as
 *      "Commercial Computer Software" and the Government is acquiring
 *      only "restricted rights" in the Licensed Programs and their
 *      documentation as that term is defined in Clause 52.227
 *      7013(c)(1) of the DFARS, and
 *
 *  (ii) If the Licensed Programs are supplied to any unit or agency
 *      of the United States Government other than DoD, the
 *      Government's rights in the Licensed Programs and their
 *      documentation will be as defined in Clause 52.227-19(c)(2) of
 *      the FAR.
 ****************************************************************************/

/*
 * $Log: mib_next.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:52  sar
 * Updated revision to 8.0
 *
 * Revision 1.2  1997/10/22 03:10:22  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 1.1  1997/10/16 00:35:06  sar
 * Result of the break up of the mibutils.c file.  These were broken
 * out to allow for better granularity and smaller size for customers
 * that weren't using all of the functions in mibutils.c
 *
 */

/* moved the routines for find next object here from mibutils.c (where
   the revision log lives) to allow for better granularity of code. */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>
#include <objectid.h>
#include <view.h>

static int
  leaf_check(SNMP_PKT_T *pktp,
             VB_T       *vbp,
             MIBLEAF_T  *lp,
             OBJ_ID_T   *robj,
             int         rcount,
             int         check_flag)
{

/* An inactive leaf is the same as the leaf not existing, if the inactive
   flag is set return an error indicating no leaf found */
if (lp->node_type & LEAF_INACTIVE)
    return(VIEW_EXCLUDED);

/* We also skip objects that don't allow read access */
if (((lp->access_type) & READ_ACCESS) == 0)
    return(VIEW_EXCLUDED);


/* If we have v1 installed and don't have use_v2_types installed
   then we may have objects that have types that shouldn't be used
   in a v1 setting.  We need to test for that case (v1 && !v2_types)
   and skip any objects that have v2 types if this is a v1 packet */
#if (INSTALL_ENVOY_SNMP_VERSION_1 && !(INSTALL_ENVOY_SNMP_USE_V2_TYPES))
    if ((pktp->snmp_version == SNMP_VERSION_1) &&
        (lp->expected_tag == VT_COUNTER64))
        return(VIEW_EXCLUDED);
#endif

#if (INSTALL_ENVOY_MAIN_AGENT)
if (check_flag) {
    /* Check if the variable is within the given view of the MIB.
       The customer chooses which view mechanism to use at compile
       time. */
#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
    return(SNMP_View_Check(pktp->view_index, robj->component_list,
                           rcount, VIEW_CHECK_IND));
#else /* (INSTALL_ENVOY_SNMP_RFC1445_VIEWS) */
    if ((lp->view_mask & pktp->mib_view) == 0)
        return (VIEW_EXCLUDED);
#endif /* (INSTALL_ENVOY_SNMP_RFC1445_VIEWS) */
    }
#endif /* INSTALL_ENVOY_MAIN_AGENT */

return(VIEW_INCLUDED);
}

#if INSTALL_ENVOY_AGENTX_MASTER
#define AX_HELP_ERROR -1
#define AX_HELP_FOUND  0
#define AX_HELP_SKIP   1
#define AX_HELP_USE    2
#define AX_HELP_LM     3

/* find_next_helper_ax is used when find_next_helper encounters
   an interior leaf node.  It determines if there are any useable
   leaf nodes under the interior leaf or the instance for use with
   the parent node.
   return codes: -1 error, 0 leaf found, 1 skip this branch,
                  2 use leaf, 3 needs last match */

static int
  find_next_helper_ax(MIBNODE_T  *np,
                      int         tcount,
                      OIDC_T     *tlist,
                      OBJ_ID_T   *robj,
                      int         rcount,
                      VB_T       *vbp,
                      SNMP_PKT_T *pktp,
                      int         check_flag)
{
int eret;
MIBLEAF_T *lp;

if (rcount > robj->num_components) {
    OIDC_T *nlist;
    nlist = (OIDC_T *)SNMP_memory_alloc((robj->num_components + 4) *
      sizeof(OIDC_T));
    if (nlist == 0)
        return(AX_HELP_ERROR);
    MEMCPY(nlist, robj->component_list, robj->num_components * sizeof(OIDC_T));
    SNMP_memory_free(robj->component_list);
    robj->component_list = nlist;
    robj->num_components = robj->num_components + 4;
    }

/* We have a leaf node, if it is in our view we set up the info we
   want to return.  Finish building the object by attaching any
   remaining target information (this will be instance info).
   We don't worry about running out of space in the result buffer
   as the only way we have instance information is if we didn't
   modify the target oid and we allocated enough space to hold
   the entire target oid.  */

if ((np->node_type & NODE_TYPE) == LEAF_NODE) {
    /* Cast the structure so it's easier to work with */
    lp = (MIBLEAF_T *)np;

    switch(leaf_check(pktp, vbp, lp, robj, rcount, check_flag)) {
        case VIEW_EXCLUDED:
            return(AX_HELP_SKIP);
        case VIEW_INDETERMINATE:
            vbp->vb_flags |= VFLAG_NEXT_VCREQ;
            break;
        }

    if (tcount) {
        MEMCPY(robj->component_list + rcount, tlist, tcount * sizeof(OIDC_T));
        vbp->vb_ml.ml_remaining_objid.component_list = robj->component_list +
                                                        rcount;
        }
    else {
        vbp->vb_ml.ml_remaining_objid.component_list = 0;
        }

    vbp->vb_ml.ml_remaining_objid.num_components = tcount;
    robj->num_components = rcount + tcount;

    vbp->vb_ml.ml_node = np;
    vbp->vb_ml.ml_flags = ML_IS_LEAF;
    return(AX_HELP_LM);
    }

else { /* inner node */
    MIBARC_T *ap = np->arcs;
    OIDC_T    node_id, *temp_list;
    int       temp_count, vc = 0;

    if (np->leaf) {
        vc = leaf_check(pktp, vbp, np->leaf, robj, rcount, check_flag);
        if (vc == VIEW_EXCLUDED)
            return(AX_HELP_SKIP);
        }

    if (tcount > 0) {
        for(; ap->nodep && (ap->id < *tlist); ap++)
            ;

        node_id    = *tlist;
        temp_count = tcount - 1;
        temp_list  = tlist + 1;
        }
    else {
        node_id    = 0;
        temp_count = 0;
        temp_list  = 0;
        /* set the include flag so that we will get this item
           if its both an agentx and instance object */
        vbp->ax_flags |= ENVOY_AX_FLAGS_INCLUDE;
        }

    if (ap->nodep && (node_id == ap->id)) {
        eret = 0;
        for(; ap->nodep && (ap->id == node_id); ap++) {
            robj->component_list[rcount] = node_id;

            eret = find_next_helper_ax((MIBNODE_T *)ap->nodep, temp_count,
                                       temp_list, robj, rcount + 1,
                                       vbp, pktp, check_flag);
            if ((eret == AX_HELP_ERROR) || (eret == AX_HELP_FOUND))
                return(eret);
            if (eret == AX_HELP_LM) {
                vbp->vb_ml.ml_last_match = node_id;
                return(AX_HELP_FOUND);
                }
            if (eret == AX_HELP_USE) {
                if (tcount || node_id ||
                    vbp->vb_ml.ml_remaining_objid.num_components)
                    vbp->vb_ml.ml_remaining_objid.num_components++;
                vbp->vb_ml.ml_remaining_objid.component_list--;
                break;
                }

            node_id++;
            if (node_id == 0)
                return(AX_HELP_SKIP);

            /* set the include flag so that we will get this item
               if its both an agentx and instance object */
            vbp->ax_flags |= ENVOY_AX_FLAGS_INCLUDE;
            temp_count = 0;
            temp_list  = 0;
            }

        if (eret == 0) {
            vbp->vb_ml.ml_remaining_objid.num_components = 1;
            vbp->vb_ml.ml_remaining_objid.component_list =
              robj->component_list + rcount;
            robj->num_components = rcount + 1;
            }
        }
    else {
        if (tcount) {
            MEMCPY(robj->component_list + rcount, tlist,
                   tcount * sizeof(OIDC_T));
            }
        vbp->vb_ml.ml_remaining_objid.num_components = tcount;

        /* The following line must be done even if we have no
           remaining objects to allow the routine to determine
           remaining objects by backing up on the list
           (comp_list--) */
        vbp->vb_ml.ml_remaining_objid.component_list =
          robj->component_list + rcount;
        robj->num_components = rcount + tcount;
        }
  
    if (np->leaf) {
        if (vc == VIEW_INDETERMINATE)
            vbp->vb_flags |= VFLAG_NEXT_VCREQ;
        
        vbp->vb_ml.ml_leaf  = np->leaf;
        vbp->vb_ml.ml_flags = ML_IS_LEAF;
        return(AX_HELP_LM);
        }
    else {
        return(AX_HELP_USE);
        }
    }
}
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

/* return codes: -1 error, 0 leaf found, 1 no leaf found */

static int
  find_next_helper(MIBNODE_T    *np,
                   int           tcount,
                   OIDC_T       *tlist,
                   OBJ_ID_T     *robj,
                   int           rcount,
                   VB_T         *vbp,
                   SNMP_PKT_T   *pktp,
                   int           check_flag)
{
int eret;
MIBLEAF_T *lp;

if (rcount + MAX_OID_COUNT > robj->num_components) {
    OIDC_T *nlist;
    nlist = (OIDC_T *)SNMP_memory_alloc((robj->num_components + 4) *
      sizeof(OIDC_T));
    if (nlist == 0)
        return(-1);
    MEMCPY(nlist, robj->component_list, robj->num_components * sizeof(OIDC_T));
    SNMP_memory_free(robj->component_list);
    robj->component_list = nlist;
    robj->num_components = robj->num_components + 4;
    }
    
if ((np->node_type & NODE_TYPE)  == INNER_NODE) {
    MIBARC_T *ap = np->arcs;

    /* if agentx is installed we see if we have an interior leaf.
       if we do we send it to an agentx specific routine to
       figure out if the leaf is within our view and if there
       are any other leaves in that leave's subtree */

#if INSTALL_ENVOY_AGENTX_MASTER
    if (np->leaf) {
        eret = find_next_helper_ax(np, tcount, tlist, robj, rcount,
                                   vbp, pktp, check_flag);
        if (eret == 3)
            return(0);
        else
            return(eret);
        }
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

    /* if we have a target object id then we need to skip any
       arcs that come "before" the target.  If one matches the
       target we need to check if it has a "next" at a lower level
       If there isn't a target or for the arcs "after" the target
       we need to check if any of those provide a usable next */
       
    if (tcount > 0) {
        for(; ap->nodep && (ap->id < *tlist); ap++)
            ;

        if (ap->nodep && (ap->id == *tlist)) {
            vbp->vb_ml.ml_last_match = ap->id;
            robj->component_list[rcount] = ap->id;
            eret = find_next_helper((MIBNODE_T *)ap->nodep, tcount - 1,
                                    tlist + 1, robj, rcount + 1,
                                    vbp, pktp, check_flag);
            if (eret <= 0)
                return (eret);
            ap++;
            }
        }

    /* If we have no target object id at this level, then   */
    /* we need to scan the available arcs, from the lowest  */
    /* to the highest, to see if any of them can provide    */
    /* a usable "next".                                     */

#if INSTALL_ENVOY_AGENTX_MASTER
    /* set the include flag so that we will get this item
       if its both an agentx and instance object */
    vbp->ax_flags |= ENVOY_AX_FLAGS_INCLUDE;
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

    for(; ap->nodep; ap++) {
        robj->component_list[rcount] = ap->id;
        vbp->vb_ml.ml_last_match = ap->id;
        eret = find_next_helper((MIBNODE_T *)(ap->nodep), 0, 0, robj,
                                rcount + 1, vbp, pktp, check_flag);
        if (eret <= 0)
            return (eret);
        }
    return(1);
    }

else { /* node_type != INNER_NODE, i.e. it must be of type LEAF_NODE */
    /* We have a leaf node, if it is in our view we set up the info we
       want to return.  Finish building the object by attaching any
       remaining target information (this will be instance info).
       We don't worry about running out of space in the result buffer
       as the only way we have instance information is if we didn't
       modify the target oid and we allocated enough space to hold
       the entire target oid.  */

    /* Cast the structure so it's easier to work with */
    lp = (MIBLEAF_T *)np;

    switch(leaf_check(pktp, vbp, lp, robj, rcount, check_flag)) {
        case VIEW_EXCLUDED:
            return(1);
        case VIEW_INDETERMINATE:
            vbp->vb_flags |= VFLAG_NEXT_VCREQ;
            break;
        }

    if (tcount) {
        MEMCPY(robj->component_list + rcount, tlist, tcount * sizeof(OIDC_T));
        vbp->vb_ml.ml_remaining_objid.component_list = robj->component_list +
                                                        rcount;
        }
    else {
        vbp->vb_ml.ml_remaining_objid.component_list = 0;
        }

    vbp->vb_ml.ml_remaining_objid.num_components = tcount;
    robj->num_components = rcount + tcount;
        
    vbp->vb_ml.ml_node = np;
    vbp->vb_ml.ml_flags = ML_IS_LEAF;
    return(0);
    }
/*NOTREACHED*/
}

#if INSTALL_ENVOY_AGENTX_MASTER
/****************************************************************************
NAME:  find_bound_helper

PURPOSE: recursivly descend the tree looking for any bounds on
         the current object.

PARAMETERS: MIBNODE_T * node to process
            bits32_t    id of session for the original leaf
            OBJ_ID_T  * return object
            int         how many levels down we are
            int         number of target subids left
            OIDC_T    * pointer to list of unused target subids

RETURNS: int -1 - error, 0 no bound found, 1 bound found
****************************************************************************/
#define AX_BOUND_HELP_ERROR   -1
#define AX_BOUND_HELP_NOBOUND  0
#define AX_BOUND_HELP_BOUND    1

int
  find_bound_helper(MIBNODE_T *mibnode,
                    bits32_t   session_id,
                    OBJ_ID_T  *robj,
                    int        num_comp,
                    int        tcount,
                    OIDC_T    *tlist)
{
MIBARC_T  *ap;
MIBLEAF_T *mibleaf;
OIDC_T    *clist;

/* Try to find a leaf */
if ((mibnode->node_type & NODE_TYPE) == LEAF_NODE)
    mibleaf = (MIBLEAF_T *)mibnode;
else 
    mibleaf = mibnode->leaf;

/* If we have a leaf that isn't for the current session
   we build the return object id, which will be filled in
   by the calling routine(s).  then return indicating a bound.

   If we aren't bounded and this is a true leaf node we indicate
   no bound to our caller otherwise we fall through and test any
   children for bounding */
   
if (mibleaf) {
    if (mibleaf->session_id != session_id) {
        if (num_comp == 0)
            return(AX_BOUND_HELP_BOUND);
        clist = (OIDC_T *)SNMP_memory_alloc(num_comp * sizeof(OIDC_T));
        if (clist == 0)
            return(-1);
        robj->component_list = clist;
        robj->num_components = num_comp;
        return(AX_BOUND_HELP_BOUND);
        }
    if (mibleaf == (MIBLEAF_T *)mibnode)
        return(AX_BOUND_HELP_NOBOUND);
    }

/* If we have a target list skip all of the arcs before the desired node.
   If the desired nodes exists check it for a bounding node, if we have
   one add the current subid to the return object id */
ap = mibnode->arcs;
if (tcount) {
    for(; ap->nodep && (ap->id < *tlist); ap++)
        ;
    if (ap->nodep && (*tlist == ap->id)) {
        switch(find_bound_helper((MIBNODE_T *)ap->nodep, session_id, robj,
                                 num_comp + 1, tcount - 1, tlist + 1)) {
            case AX_BOUND_HELP_ERROR:
                return(AX_BOUND_HELP_ERROR);
            case AX_BOUND_HELP_BOUND:
                robj->component_list[num_comp] = ap->id;
                return(AX_BOUND_HELP_BOUND);
            }
        ap++;
        }
    }

/* The target list didn't provide a boundary, try stepping through
   the rest of the arcs */
for(; ap->nodep; ap++) {
    switch(find_bound_helper((MIBNODE_T *)ap->nodep, session_id, robj,
                             num_comp + 1, 0, 0)) {
        case AX_BOUND_HELP_ERROR:
            return(AX_BOUND_HELP_ERROR);
        case AX_BOUND_HELP_BOUND:
            robj->component_list[num_comp] = ap->id;
            return(AX_BOUND_HELP_BOUND);
        }
    }

/* no boundary was found */
return(AX_BOUND_HELP_NOBOUND);
}

/****************************************************************************
NAME:  find_next_bound

PURPOSE: Find the boundary object id for the given vbp.  The vbp
         must have the mib leaf information correctly set, probably
         via find_next_object.  The boundary id will be inserted
         into the vb, if no boundary is found we put nothing into
         the vb.

PARAMETERS: MIBNODE_T * root node to use
            VB_T      * vb under discussion 

RETURNS: int 0 on success
****************************************************************************/
int
  find_next_bound(MIBNODE_T *mibroot,
                  VB_T      *vbp)
{
MIBNODE_T *mibnode;
OBJ_ID_T   objid;
OIDC_T    *clist;
int        num_comp;

num_comp = vbp->vb_obj_id.num_components -
           vbp->vb_ml.ml_remaining_objid.num_components;
objid.num_components = num_comp;
objid.component_list = vbp->vb_obj_id.component_list;

 (void) Find_Node_From_Root(mibroot, &objid, &mibnode);

if ((mibnode->node_type & NODE_TYPE) == INNER_NODE) {
    switch(find_bound_helper(mibnode, vbp->vb_ml.ml_leaf->session_id,
                             &vbp->ax_search_end, num_comp,
                             vbp->vb_ml.ml_remaining_objid.num_components,
                             vbp->vb_ml.ml_remaining_objid.component_list)) {
        case AX_BOUND_HELP_ERROR:
            return(-1);
        case AX_BOUND_HELP_BOUND:
            if (num_comp)
              MEMCPY(vbp->ax_search_end.component_list, objid.component_list,
                     num_comp * sizeof(OIDC_T));
            return(0);
        }
    }

if (num_comp == 0)
    return(0);

if ((clist = (OIDC_T *)SNMP_memory_alloc(num_comp * sizeof(OIDC_T))) == 0)
    return(-1);

MEMCPY(clist, objid.component_list, num_comp * sizeof(OIDC_T));
while((++clist[num_comp - 1]) == 0) {
    num_comp--;
    if (num_comp == 0) {
        SNMP_memory_free(clist);
        return(0);
        }
    }

vbp->ax_search_end.component_list = clist;
vbp->ax_search_end.num_components = num_comp;
return(0);
}
#endif /* #if INSTALL_ENVOY_AGENTX_MASTER */

/****************************************************************************
NAME:  find_next_object

PURPOSE:  Given an object identifier, find the "next" SNMP object using
          SNMP's notion of lexicographic ordering of object identifiers.
          The "next" object may be a leaf node of the MIB tree or an object
          within a tabular structure.  In the latter case, located object
          id will pass through the leaf node representing the table.

          We install an object id and some mib leaf info into the given
          var bind.  The returned object id, unless null, will be a
          valid id for the next routine named in the leaf node for the
          object.

PARAMETERS:
        VB_T       *    The vb structure to store the results, we put the
                        result object id, and some mib leaf info into it.
        OBJ_ID_T   *    The target oid to start with.                   
        SNMP_PKT_T *    The received packet
        int             Do the view check (1) or not (0)

RETURNS:  int           > 0 for success,
                        0 if nothing found,
                        -1 for internal failure.
                        On success, the object identifier referenced by
                        the  vbp will contain the resulting object id.
Note that the for the bulk of find_next_object and find_next helper we
use the num_components field in the result objid as the amount of space
we have allocated in the componet_list field.  Rcount is the actual number
of components.
****************************************************************************/
int
  find_next_object(VB_T       *vbp,
                   OBJ_ID_T   *targetp,
                   SNMP_PKT_T *pktp,
                   int         check_flag)
{
OBJ_ID_T        *resultp;
MIBNODE_T       *np;
int              eret;

/* Set result to point to the objid in the vbp (where we want the result) */
resultp = &(vbp->vb_obj_id);

resultp->num_components = max(targetp->num_components, 16 + MAX_OID_COUNT);
resultp->component_list =
        (OIDC_T *)SNMP_memory_alloc(resultp->num_components * sizeof(OIDC_T));
if (resultp->component_list == 0)
    return(-1);

if (pktp->mib_root)
    np = pktp->mib_root;
else 
    np = &(ENVOY_MIB_ROOT_NODE);

eret = find_next_helper(np, targetp->num_components, targetp->component_list,
                        resultp, 0, vbp, pktp, check_flag);
if (eret) {
    Clean_Obj_ID(resultp);
    if (eret > 0)
        return(0);
    else
        return(-1);
    }

#if INSTALL_ENVOY_AGENTX_MASTER
/* if agentx is installed and this is an agentx leaf
   we need to find the boundary oid.
   We also need to update the reference counter */
if (vbp->vb_ml.ml_node->node_type & AGENTX_LEAF) {
    if (find_next_bound(np, vbp) || 
        (vbp->vb_ml.ml_leaf->ref_count == AX_MAX_REF_COUNT)) {
        Clean_Obj_ID(resultp);
        return(-1);
        }
    vbp->vb_ml.ml_leaf->ref_count++;
    }
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

return (resultp->num_components);
}


