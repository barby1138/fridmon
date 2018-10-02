/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/mib_fon.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: mib_fon.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:51  sar
 * Updated revision to 8.0
 *
 * Revision 1.2  1997/10/22 03:10:21  sar
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

/* moved find_object_node here from mibutils.c (where most of the
   revision log is) to allow for better granularity of code. */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>
#include <objectid.h>
#include <view.h>

/****************************************************************************
NAME:  find_object_node

PURPOSE:  Locate a node in the MIB tree corresponding to a given object id.
          The search terminates sucessfully when all the object identifer
          components have been consumed or a leaf node is encountered.
          (One of the status flags in the MIBLOC_T structure indicates
          whether the search terminated at an inner node or a leaf.)

          The search terminates unsucessfully (return code -1) if an
          object identifier component does not match those available at
          a given inner node.

PARAMETERS:
        OBJ_ID_T *      Object ID to be used
        SNMP_PKT_T *    The received packet
        int             Do the view check (1) or not (0)

RETURNS:  int           Zero if node located
                        vbp->vb_ml structure will be filled in with lots
                        of information about the found object.
                          -1 if nothing located (MIBLOC_T is not valid)
                          -2 if we found something but it was outside
                             of our view.
                          -3 if we had a more general error 
****************************************************************************/
int
  find_object_node(VB_T       * vbp,
                   SNMP_PKT_T * pktp,
                   int          check_flag)
{
OBJ_ID_T  *objp;
MIBLOC_T  *mlp;
MIBNODE_T *np;
MIBARC_T  *ap;
OIDC_T    *compp;         /* Current object id component of interest */
int        comp_num;      /* Index of current object id component    */

#if (INSTALL_ENVOY_AGENTX_MASTER)
MIBLEAF_T *ax_np = 0;
OIDC_T     ax_last_match = 0, *ax_compp = 0;
int        ax_comp_num = 0;
#endif

objp = &(vbp->vb_obj_id);
mlp = &(vbp->vb_ml);

/* The user can choose (at compile time) to use the old Epilogue
   view mask system, or to use the view scheme described in rfc1445.
   The user will be forced to choose the rfc1445 scheme if they choose
   to use the master agent code. */
/* We can do the check here for this scheme because we only need the
   object id, in the old v1 scheme we need the actual node to examine
   the view masks so we have to wait until after the tree lookup.  */

#if (INSTALL_ENVOY_MAIN_AGENT && INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
if (check_flag) {
    if (SNMP_View_Check(pktp->view_index, objp->component_list,
                        objp->num_components,
                        VIEW_CHECK_DET) == VIEW_EXCLUDED)
        return (-2);
    }
#endif /* (INSTALL_ENVOY_MAIN_AGENT && INSTALL_ENVOY_SNMP_RFC1445_VIEWS) */

/* find the proper root of the tree */
if (pktp->mib_root)
    np = pktp->mib_root;
else 
    np = &(ENVOY_MIB_ROOT_NODE);

/* if agentx is installed see if the root node has a leaf, if so
   save some information about it */
#if (INSTALL_ENVOY_AGENTX_MASTER)
if (np && ((np->node_type & NODE_TYPE) == INNER_NODE) && np->leaf) {
    ax_np         = np->leaf;
    ax_last_match = 0;
    ax_comp_num   = objp->num_components;
    ax_compp      = objp->component_list;
    }
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

for(comp_num = objp->num_components, compp = objp->component_list;
    ((np->node_type & NODE_TYPE) == INNER_NODE) && (comp_num > 0);
    comp_num--, compp++)
   {
   for(ap = np->arcs; (MIBNODE_T *)(ap->nodep) != 0; ap++) {
      if (ap->id == *compp) 
          goto found_arc;
      }
   /* If we fall through the arcs, then there is no terminating leaf,
      if agentx is installed we see if we have an interior leaf */
#if (INSTALL_ENVOY_AGENTX_MASTER)
   if (np->leaf) {
       /* adjust the leaf information */
       np = (MIBNODE_T *)np->leaf;
       break;
       }
   else if (ax_np) {
       /* adjust the leaf, comp num and last match information */
       np                 = (MIBNODE_T *)ax_np;
       mlp->ml_last_match = ax_last_match;
       comp_num           = ax_comp_num;
       compp              = ax_compp;
       break;
       }
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

   return (-1);

found_arc:

   /* if agentx is installed see if this inner node also has a leaf,
      if it does we save some information so we can get back to it
      later if necessary */
#if (INSTALL_ENVOY_AGENTX_MASTER)
   if (np->leaf) {
       ax_np         = np->leaf;
       ax_last_match = *compp;
       ax_comp_num   = comp_num - 1;
       ax_compp      = compp + 1;
       }
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

   /* Save the value of the matched arc so that later, when we call     */
   /* the procs for the various objects we can tell them the last arc   */
   /* matched.  This will allow the routines which access tabular       */
   /* objects to know which column (attribute) of the table is being    */
   /* used.                                                             */

   mlp->ml_last_match = *compp;
   np = (MIBNODE_T *)(ap->nodep);
   }

/* Here we have either run out of object identifiers or we have hit        */
/* a leaf node (in which case there may be zero or more identifers left.)  */

/* If agentx is installed we may need to adjust the node that we
   are using.  If the current node is an inner node we try to
   find a leaf, either from the current node or that we saved earlier.
   If the current node is an inactive leaf we check the saved information */

#if (INSTALL_ENVOY_AGENTX_MASTER)
if ((np->node_type & NODE_TYPE) == INNER_NODE) {
    if (np->leaf)
        np = (MIBNODE_T *)np->leaf;
    else if (ax_np) {
        /* adjust the leaf, comp num and last match information */
        np                 = (MIBNODE_T *)ax_np;
        mlp->ml_last_match = ax_last_match;
        comp_num           = ax_comp_num;
        compp              = ax_compp;
        }
    }

if (np->node_type & LEAF_INACTIVE) {
    /* if agentx is installed see if we have a saved leaf */
    if (ax_np) {
        /* adjust the leaf, comp num and last match information */
        np                 = (MIBNODE_T *)ax_np;
        mlp->ml_last_match = ax_last_match;
        comp_num           = ax_comp_num;
        compp              = ax_compp;
        }
    else 
        return (-1);
    }

#else
/* If agentx isn't installed inactive leaves are errors */
if (np->node_type & LEAF_INACTIVE)
    return (-1);

#endif /* INSTALL_ENVOY_AGENTX_MASTER */

if ((np->node_type & NODE_TYPE) == LEAF_NODE) {
    /* We now have the node and can start doing various checks 
       The first one is to determine if the type is acceptable.
       In the case that we have both v1 installed and aren't allowing
       v2_types we may have objects that have types that shouldn't be used
       in a v1 setting.  We need to test for that case (v1 && !v2_types)
       and skip any objects that have v2 types if this is a v1 packet */

#if (INSTALL_ENVOY_SNMP_VERSION_1 && !(INSTALL_ENVOY_SNMP_USE_V2_TYPES))
    if ((pktp->snmp_version == SNMP_VERSION_1) &&
        (((MIBLEAF_T *)np)->expected_tag == VT_COUNTER64))
        return(-1);
#endif

    /* Now we have the node we can do the old envoy view check.
       It will only be required if rfc1445 views aren't installed
       and the sub agent isn't installed.  (The sub agent doesn't do
       view checks and the rfc1445 view check must be installed if
       the master agent is also installed) */

#if (INSTALL_ENVOY_MAIN_AGENT && !(INSTALL_ENVOY_SNMP_RFC1445_VIEWS))
    if ((check_flag) && ((((MIBLEAF_T *)np)->view_mask & pktp->mib_view) == 0))
        return (-2);
#endif /* (INSTALL_ENVOY_MAIN_AGENT && !(INSTALL_ENVOY_SNMP_RFC1445_VIEWS)) */
    mlp->ml_flags = ML_IS_LEAF;
    }
else
    mlp->ml_flags = 0;

#if (INSTALL_ENVOY_AGENTX_MASTER)
/* If agentx is installed we need to reference count agentx leaves */
if (np->node_type & AGENTX_LEAF) {
    if (((MIBLEAF_T *)np)->ref_count == AX_MAX_REF_COUNT)
        return (-3);
    else 
        ((MIBLEAF_T *)np)->ref_count++;
    }
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

mlp->ml_remaining_objid.num_components = comp_num;
mlp->ml_remaining_objid.component_list = compp;
mlp->ml_node = np;

return(0);
}

