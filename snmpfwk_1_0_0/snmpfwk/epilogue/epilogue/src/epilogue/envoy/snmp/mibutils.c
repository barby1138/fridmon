/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/mibutils.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: mibutils.c,v $
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
 * Revision 7.4  1997/10/22 03:10:22  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.3  1997/10/16 00:45:55  sar
 * Updated for agentx support, with most of the work in mibutils.c to deal
 * with interior leaves and the pdu processors to deal with agentx leaves,
 * especially their reference counts.
 *
 * Add a bug macro into the get_{read write}_lock calls to give the user
 * a chance if things start going wrong.
 *
 * Fixed a potential problem int the proxy code where we were trying to
 * stuff a 32 bit quantity into a pointer which may or may not work depending
 * on the platform.
 *
 * Revision 7.2  1997/03/20 06:49:00  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.2  1995/11/01  00:46:48  sar
 * Removed localio.h, buffer.h and decode.h
 *
 * Revision 6.1  1995/10/20  23:02:48  sar
 * changed party.h to view.h
 * removed nsapaddress, uinteger32 and bitstring types
 * removed no_pp stuff
 * rearranged the view code to so the rfc1445 view scheme for both v1 & v2
 * if INSTALL_ENVOY_SNMP_RFC1445_VIEWS is installed or the old envoy
 * scheme if it isn't
 *
 * Revision 6.0  1995/05/31  21:47:32  sra
 * Release 6.0.
 *
 * Revision 5.6  1995/04/28  23:12:28  sar
 * Dropped the static/dynamice flag in the call to nextproc_next_instance
 *
 * Revision 5.5  1995/03/22  18:39:10  sar
 * Added parans around the access_type check to make compilers happy.
 *
 * Revision 5.4  1995/03/21  00:25:01  sar
 * Updated to use the new async system, the biggest effect is in the next
 * routines where we no longer call the nexproc routine,  This has effects
 * on the way we deal with view checking and the use of the result object.
 *
 * Revision 5.3  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.2  1994/08/19  18:39:21  sar
 * Copy the entire target oid into the result field during find_next_obj
 * so that the sub agents can access the entire oid easily.  This requires
 * allocating enough space for the oid as well.
 *
 * Revision 5.1  1994/08/04  22:04:16  sar
 * Added multiple mib tree capability
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.6  1994/05/11  17:28:52  sar
 * Removed the #if master_agent from around the find, add, and remove node
 * functions.
 *
 * Revision 4.5  1994/05/10  20:15:07  sar
 * Added some #if's and added some clauses to compile only what we need
 * for the sub agent.
 *
 * Revision 4.4  1994/05/09  21:03:44  sar
 * Added #if around a return statment so it should only be included when it
 * will be used (to avoid unreachable code warnings).
 *
 * Revision 4.3  1994/04/29  21:56:01  sar
 * Mostly mods dealing with the view check stuff, added a flag to
 * find_object_node to determine if the view check should be done,
 * add ifdefs to handle the sub agent and v1 using v2 view schemes.
 * Modified the calling sequence to find_node.
 *
 * Revision 4.2  1994/02/01  23:40:34  sar
 * Made the handling of oids more dynamic in the find next routines.
 * Added the routines Find, Add and Remove_Node for use with dynamic mib
 * manipulation.
 *
 * Revision 4.1  1993/10/06  19:49:43  sar
 * moved the version 2 view check to be at the beginning of find_object_node.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.9  1993/06/17  23:33:44  sar
 * Modified view_check to be able to check the view before a next proc is
 * run (though it may need to be called again after the nextproc is run).
 *
 * Revision 3.8  1993/06/07  18:26:04  sar
 * Modified the find_next_objid (or whatever) and find_next_helper to
 * allocate the objids it uses dynamically, this allows them to grow
 * if neccesary and guarantees the method routines at least MAX_OID_COUNT
 * oids in the list handed to them.
 *
 * Revision 3.7  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.6  1993/05/22  01:04:31  sar
 * Cleaned up some unused variables
 *
 * Revision 3.5  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.4  1993/04/20  19:53:27  sar
 * I had removed the read-only check during sets (for v2) so I put it back
 * and cleaned up the code around it, also gave mibuitls another error return
 * so we can coose between no access and no such.
 *
 * Revision 3.3  1993/03/20  00:25:43  sar
 * Updated some types for version two party type stuff (indicies, local
 * storage, status, mtu and privileges).
 * Also removed some unused break statements.
 *
 * Revision 3.2  1993/03/17  23:02:47  sar
 * added the view structure and the routines to use and manipulate it.
 * in mibutils and setpdu started using views.
 *
 * Revision 3.1  1993/02/17  21:05:50  sar
 * Removed #define then and uses of it, general rearrangement.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.102  91/10/30  20:42:04  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.101  91/08/15  12:31:04  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:22  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:45  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:50
 * Release 2.00
 * 
 *    Rev 1.4   24 Sep 1989 22:06:38
 * Renamed mib_root to mib_root_node to support the MIB compiler.
 * 
 *    Rev 1.3   27 Apr 1989 15:55:58
 * Removed unused variables
 * 
 *    Rev 1.2   19 Sep 1988 17:26:34
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:16
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:47:02
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>
#include <objectid.h>
#include <view.h>

/*lint -e715    */
/*ARGSUSED*/
int
  std_next(OIDC_T       lastmatch,
           int          tcount,
           OIDC_T     * tlist,
           OIDC_T     * rlist,
           char       * cookie,
           SNMP_PKT_T * pktp)
{
if (tcount > 0) 
   return 0;
else {
    *rlist = 0;
    return 1;
    }
}

void
  std_next_async(OIDC_T       lastmatch,
                 int          tcount,
                 OIDC_T     * tlist,
                 SNMP_PKT_T * pktp,
                 VB_T       * vbp)
{
OIDC_T oidc = 0;
if (tcount > 0) 
    nextproc_no_next(pktp, vbp);
else {
    nextproc_next_instance(pktp, vbp, 1, &oidc);
    vbp->vb_ml.ml_leaf->getproc(lastmatch, 1, &oidc, pktp, vbp);
    }
}

/****************************************************************************
NAME:  branch_free

PURPOSE:  Free any allocated memory when cleaning a partially allocated
          branch of the mib tree.  Given two nodes, descend from the
          first one and free any allocated memory up to, but not including
          the second one.

PARAMETERS:
        MIBNODE_T *     Starting node
        MIBNODE_T *     Ending node

RETURNS:  void

****************************************************************************/
void
  branch_free(MIBNODE_T *startnode,
              MIBNODE_T *endnode)
{
MIBARC_T *ap;

if (startnode == endnode)
    return;

for(ap = startnode->arcs; (MIBNODE_T *)(ap->nodep) != 0; ap++)
    branch_free((MIBNODE_T *)(ap->nodep), endnode);

if ((startnode->node_type & ARC_DYNAMIC) != 0)
    SNMP_memory_free((char *)(startnode->arcs));

if ((startnode->node_type & NODE_DYNAMIC) != 0)
    SNMP_memory_free((char *)startnode);

return;
}

/****************************************************************************
NAME:  Find_Node_From_Root

PURPOSE:  Locate a node in a MIB tree corresponding to a given object id.
          If root is non-zero it is used as the base of the tree, 
          otherwise we use ENVOY_MIB_ROOT_NODE, which is defined in mib.h
          as ENVOY_DEFAULT_MIB_ROOT_NODE if the default is defined or
          mib_root_node if it isn't.
          The search terminates sucessfully when all the object identifer
          components have been consumed (the node is returned and the function
          returns a 0), a leaf node is encountered before all object
          identifier components are consumed (the node is returned and the
          function returns 1) or if an object identifier component does
          not match those at a given inner node (a node of 0 is returned and
          the function returns 2).

PARAMETERS:
        MIBNODE_T  *    Pointer to the base of the mib tree
        OBJ_ID_T   *    Object ID to be used
        MIBNODE_T **    Ponter to be filled in with the desired node,
                        0 if no node was found.

RETURNS: sbits32_t      0 if all subids were consumed
                        1 if a leaf node was found before all subids were
                          consumed
                        2 if no node was found. (the 0 node is returned)

****************************************************************************/
sbits32_t
  Find_Node_From_Root(MIBNODE_T  *root,
                      OBJ_ID_T   *objp,
                      MIBNODE_T **npp)
{
MIBNODE_T *np;
MIBARC_T  *ap;
OIDC_T    *compp;         /* Current object id component of interest */
int        comp_num;      /* Index of current object id component    */

/* find the proper root of the tree */
if (root)
    np = root;
else 
    np = &(ENVOY_MIB_ROOT_NODE);

for(comp_num = objp->num_components, compp = objp->component_list;
    ((np->node_type & NODE_TYPE) == INNER_NODE) && (comp_num > 0);
    comp_num--, compp++)
   {
   for(ap = np->arcs; (MIBNODE_T *)(ap->nodep) != 0; ap++) {
      if (ap->id == *compp)
          goto found_arc;
      }

   /* If we fall through the arcs, then we have failed in the search */
   *npp = 0;
   return (2);

found_arc:
   /* we found a matching arc so we step down to the next level of the tree */
   np = (MIBNODE_T *)(ap->nodep);
   }

/* We have a node, set up to return it to the caller, then check
   if we have consumed all the identifiers.  If we haven't we return
   1 otherwise return 0 */

*npp = np;
if (comp_num > 0)
    return(1);
else
    return(0);
}

/****************************************************************************
NAME:  Add_Node_From_Root

PURPOSE:  Add a node to the mib tree.
          If root is non-zero it is used as the base of the tree, 
          otherwise we use ENVOY_MIB_ROOT_NODE, which is defined in mib.h
          as ENVOY_DEFAULT_MIB_ROOT_NODE if the default is defined or
          mib_root_node if it isn't.
          Given a node and object id, attach the node to the mib tree at
          the point specified by the object id, if a replaceable node
          already exists at the given point it is unlinked and a pointer
          to it returned.  It is up to the calling routine to dispose of
          the old node correctly (perhaps by freeing the space).
          On errors any partialy allocated branches are freed, the node
          isn't modified.
          There must be an allocated mib_root_node which is not changable
          by this function.

PARAMETERS:
        MIBNODE_T *     Pointer to the base of the mib tree
        OBJ_ID_T  *     Object ID to be used
        MIBNODE_T *     Node to be added.
        MIBNODE_T **    Replaced node.

RETURNS:  int           0 for success
                        1 Error: General error (memory alloc failure etc).
                        2 Error: The object id points through a leaf
                        3 Error: A unreplaceable node already exists
                        4 Error: A replaceable node exists which doesn't
                                 match the type of the given node.
                        5 Error: invalid obj id.

****************************************************************************/
int
  Add_Node_From_Root(MIBNODE_T  *root,
                     OBJ_ID_T   *objp,
                     MIBNODE_T  *anode,
                     MIBNODE_T **rnode)
{
MIBNODE_T       *np, *newnode;
MIBARC_T        *ap = 0, *newarc, *savearc;
OIDC_T          *compp;         /* Current object id component of interest */
int             comp_num;       /* Index of current object id component    */
int             num_arcs;       /* number of arcs, for allocating more space */

/* Make sure the objid is acceptable */
if (objp->num_components < 1)
    return(5);

/* Set the replaced node to be 0 in case we don't have a node to return */
*rnode = 0;

/* find the proper root of the tree */
if (root)
    np = root;
else 
    np = &(ENVOY_MIB_ROOT_NODE);

/* find the last matching node in the given object id */
for(comp_num = objp->num_components, compp = objp->component_list;
    ((np->node_type & NODE_TYPE) == INNER_NODE) && (comp_num > 0);
    comp_num--, compp++, np = (MIBNODE_T *)(ap->nodep))
   {
   for(ap = np->arcs; (MIBNODE_T *)(ap->nodep) != 0; ap++) {
      if (ap->id == *compp)
         break;
      }

   /* If we are out of arcs we use the current node else we had a matching
      arc and we continue to the next level */
   if ((MIBNODE_T *)(ap->nodep) == 0)
       break;
   }

/* Take care of the simple cases first,  if we are out of sub ids
   we are at the point where we would add the new node, so we do
   some tests to make sure we don't destroy a leaf or overwrite
   something that can't be overwritten or overwrite a leaf with
   an inner node or vice versa.  If none of the tests fail we simply
   change the pointer in the arc list to point to the new node. */

if (comp_num == 0) {
    if ((np->node_type & NODE_TYPE) == LEAF_NODE) {
        if ((((MIBLEAF_T *)np)->node_type & REPLACEABLE) == 0)
            return(3);
        if ((anode->node_type & NODE_TYPE) != LEAF_NODE)
            return(4);
        *rnode = np;
        ap->nodep = (MIBARC_T *)anode;
        return(0);
        }
    else {
        if ((np->node_type & REPLACEABLE) == 0)
            return(3);
        if ((anode->node_type & NODE_TYPE) != INNER_NODE)
            return(4);
        *rnode = np;
        ap->nodep = (MIBARC_T *)anode;
        return(0);
        }
    }

/* If we are here we have more sub ids left.  We check that the last
   node we matched isn't a leaf node (we need an inner node to hook the
   subtree we are about to create onto) and then we start building the
   subtree. */

if ((np->node_type & NODE_TYPE) == LEAF_NODE)
    return(2);

/* We work from the leaf back to the current tree, allocate the arc space
   fill it in (including the terminator), allocate the next level node,
   connect the arcs to it and repeat */

for (newnode = anode; comp_num > 1; comp_num--) {
    newarc = (MIBARC_T *)SNMP_memory_alloc(sizeof(MIBARC_T) * 2);
    if (newarc == 0) {
        /* free any memory allocated for this branch */
        branch_free(newnode, anode);
        return(1);
        }

    newarc[0].id = compp[comp_num - 1];
    newarc[0].nodep = (MIBARC_T *)newnode;
    newarc[1].id = 0;
    newarc[1].nodep = 0;

    if ((newnode = (MIBNODE_T *)SNMP_memory_alloc(sizeof(MIBNODE_T))) == 0) {
        /* free any memory allocated for this branch */
        newnode = (MIBNODE_T *)(newarc->id);
        SNMP_memory_free(newarc);
        branch_free(newnode, anode);
        return(1);
        }

    MEMSET(newnode, 0, sizeof(MIBNODE_T));
    newnode->node_type = INNER_NODE | NODE_DYNAMIC | ARC_DYNAMIC |
                         SYSTEM_OWNED;
    newnode->arcs = newarc;
    }

/* num_arcs is the number of arcs in the current list */
for(num_arcs = 1, ap = np->arcs; (MIBNODE_T *)(ap->nodep) != 0; ap++)
    num_arcs++;

/* add one to num_arcs for the new arc we are adding */
newarc = (MIBARC_T *)SNMP_memory_alloc(sizeof(MIBARC_T) * (num_arcs + 1));
if (newarc == 0) {
    /* free the branch */
    branch_free(newnode, anode);
    return(1);
    }

/* copy all arcs less then the new arc, then add the new arc */
/* then copy all the arcs after the new arc, there will always be at least
   one, the terminator */
savearc = newarc;
for(ap = np->arcs;
    (ap->id < *compp) && ((MIBNODE_T *)(ap->nodep) != 0);
    ap++, newarc++, num_arcs--)
     MEMCPY(newarc, ap, sizeof(MIBARC_T));

newarc->id = *compp;
newarc->nodep = (MIBARC_T *)newnode;
newarc++;
MEMCPY(newarc, ap, sizeof(MIBARC_T) * num_arcs);

/* now clean up, if necessary free the old arc list then tag the node so that
   the new arc list will be freed when released */
ap = np->arcs;
np->arcs = savearc;
if (np->node_type & ARC_DYNAMIC)
    SNMP_memory_free(ap);
np->node_type |= ARC_DYNAMIC;

return 0;
}

/****************************************************************************
NAME:  Remove_Node_From_Root

PURPOSE:  Remove a node from the tree.  Also remove any branches that
          lead only to that node and are owned by the system, freeing
          the branches sections if required.
          If root is non-zero it is used as the base of the tree, 
          otherwise we use ENVOY_MIB_ROOT_NODE, which is defined in mib.h
          as ENVOY_DEFAULT_MIB_ROOT_NODE if the default is defined or
          mib_root_node if it isn't.

PARAMETERS:
        MIBNODE_T *     Pointer to the base of the mib tree
        OBJ_ID_T  *     Object ID to be used

RETURNS:  MIBNODE_T *   The mib node being removed, it is up to the caller
                        to free any allocated space associated with the 
                        node.  NULL means no node found, and nothing
                        removed.

****************************************************************************/
MIBNODE_T *
  Remove_Node_From_Root(MIBNODE_T *root,
                        OBJ_ID_T  *objp)
{
MIBNODE_T *np, *bnode;
MIBARC_T  *ap;
OIDC_T     barc, *compp;        /* Current object id component of interest */
int        comp_num;            /* Index of current object id component    */

if (objp->num_components < 1)
    return(0);

/* find the proper root of the tree */
if (root)
    np = root;
else 
    np = &(ENVOY_MIB_ROOT_NODE);
bnode = np;

/* find the last matching node in the given object id */
for(comp_num = objp->num_components, compp = objp->component_list,
    barc = *compp;
    ((np->node_type & NODE_TYPE) == INNER_NODE) && (comp_num > 0);
    comp_num--, compp++, np = (MIBNODE_T *)(ap->nodep))
   {
   /* If we have multiple branches descending from this node or this
      node isn't owned by the system we update the branch node pointer
      (which will control what gets removed along with the specified
      node). */
   ap = np->arcs;
   if (((np->node_type & SYSTEM_OWNED) == 0) || 
       ((MIBNODE_T *)(ap[1].nodep) != 0)) {
       bnode = np;
       barc = *compp;
       }

   /* step through the arcs (ap is already pointing to the beginning
      of the arcs) looking for the current subid.  If we hit a null
      node pointer we have run out of arcs and the node we were looking
      for doesn't exist so we return a 0 for an error */
   for(; ; ap++) {
      if (ap->id == *compp) 
         break;
      if ((MIBNODE_T *)(ap->nodep) == 0)
         return(0);
      }

#if INSTALL_ENVOY_AGENTX_MASTER
    /* if agentx is installed we want to set the branch pointer
       in front of inner nodes that also have direct pointers to leaves
       but that aren't the node to remove.  This will allow us to
       keep those leaves by moving them into the arc list in place
       of the node */
    if (((MIBNODE_T *)ap->nodep)->leaf && (comp_num != 1)) {
        bnode = np;
        barc = *compp;
        }
#endif
   }

/* np should point to the node we want to remove, bnode points to the last
   node before the branch that should be removed from the tree and
   barc is the id of the branch coming out of bnode's arc list that
   connects to np */

/* first some tests, we see if we used all of the subids and if the
   node is removeable */
if ((comp_num != 0) || ((np->node_type & REMOVABLE) == 0))
    return(0);

/* Now we unlink the branch from the tree */
/* Find the arc to remove then either copy all the arcs above it
   down by one arc structure or if agentx is installed we might
   need to move a leaf pointer from the first node in the branch
   to the arc list in its place.
   In either case save a pointer to the branch to be removed. */
for(ap = bnode->arcs; ap->id != barc; ap++)
    ;
bnode = (MIBNODE_T *)ap->nodep;

#if INSTALL_ENVOY_AGENTX_MASTER
if ((bnode != np) && (bnode->leaf)) {
    ap->nodep = (MIBARC_T *)bnode->leaf;
    branch_free(bnode, np);
    return(np);
    }
#endif

for(; ((MIBNODE_T *)(ap->nodep) != 0); ap++)
    MEMCPY(ap, (ap + 1), sizeof(MIBARC_T));

/* Free the branch */
branch_free(bnode, np);

/* return the node */
return(np);
}

#if INSTALL_ENVOY_AGENTX_MASTER
/****************************************************************************
NAME:  Find_Leaf_From_Root

PURPOSE: find the leaf for the node returned from find_node_from_root
         if there isn't set the mibleaf pointer to 0.
        

PARAMETERS: MIBNODE_T * the root node to use
            OBJ_ID_T  * the object id that names the leaf
            MIBLEAF_T ** return area for mibleaf pointer, 0 if no leaf

RETURNS: 0 if we consumed all subids
         1 if a leaf node was found before all subids were consumed
         2 if no node was found 
****************************************************************************/
sbits32_t
  Find_Leaf_From_Root(MIBNODE_T *mibroot,
                      OBJ_ID_T  *objid,
                      MIBLEAF_T **mibleaf)
{
MIBNODE_T *mnode;
sbits32_t err_ret;

err_ret = Find_Node_From_Root(mibroot, objid, &mnode);

if (mnode) {
    if ((mnode->node_type & NODE_TYPE) == INNER_NODE)
        *mibleaf = mnode->leaf;
    else
        *mibleaf = (MIBLEAF_T *)mnode;
    }
else
    *mibleaf = 0;

return(err_ret);
}

/****************************************************************************
NAME:  Add_Leaf_From_Root

PURPOSE: Attempt to add a leaf to the tree at the given position
         If the leaf already exists and is replaceable we replace it
         If the node already exists and is replaceable we add the leaf
         Otherwise we try and insert the leaf and any necessary branches

PARAMETERS: MIBNODE_T * root of the mibtree
            OBJ_ID_T  * name of the point where the leaf should go
            MIBLEAF_T * the leaf to add
            MIBLEAF_T ** the removed leaf (if any) 

RETURNS: int 0 failure
             1 success, added to tree
             2 success, not added to tree, this might occur if the leaf
             we are trying to replace isn't an agentx leaf.
****************************************************************************/
int
  AX_Add_Leaf_From_Root(MIBNODE_T  *mibroot,
                        OBJ_ID_T   *objp,
                        MIBLEAF_T  *add_leaf,
                        MIBLEAF_T **rem_leaf)
{
MIBNODE_T       *np, *newnode;
MIBARC_T        *ap = 0, *newarc, *savearc;
OIDC_T          *compp;         /* Current object id component of interest */
int             comp_num;       /* Index of current object id component    */
int             num_arcs;       /* number of arcs, for allocating more space */

/* make sure we have a root node */
if (mibroot == 0)
    return(0);
np = mibroot;

/* Make sure the objid is acceptable */
if (objp->num_components < 1)
    return(1);

/* Set the replaced leaf to be 0 in case we don't have a leaf to return */
*rem_leaf = 0;

/* find the last matching node in the given object id */
for(comp_num = objp->num_components, compp = objp->component_list;
    ((np->node_type & NODE_TYPE) == INNER_NODE) && (comp_num > 0);
    comp_num--, compp++, np = (MIBNODE_T *)(ap->nodep))
   {
   for(ap = np->arcs; ap->nodep != 0; ap++) {
      if (ap->id == *compp)
         break;
      }

   /* If we are out of arcs we use the current node else we had a matching
      arc and we continue to the next level */
   if (ap->nodep == 0)
       break;
   }

/* If the node is a non-agentx leaf we don't allow it to be replaced.
   This isn't classed as a failure */
if ((np->node_type & (NODE_TYPE | AGENTX_LEAF)) == LEAF_NODE)
    return(2);

if (comp_num == 0) {
    if ((np->node_type & NODE_TYPE) == LEAF_NODE) {
        if ((np->node_type & REPLACEABLE) == 0)
            return(0);
        *rem_leaf = (MIBLEAF_T *)np;
        ap->nodep = (MIBARC_T *)add_leaf;
        return(1);
        }

    if (np->leaf)  {
        if ((np->leaf->node_type & REPLACEABLE) == 0)
            return(0);
        if ((np->leaf->node_type & AGENTX_LEAF) == 0)
            return(2);
        *rem_leaf = np->leaf;
        np->leaf = add_leaf;
        return(1);
        }

    if ((np->node_type & REPLACEABLE) == 0) {
        np->leaf = add_leaf;
        return(1);
        }
    }

/* If we are here we have more sub ids left, start building the subtree.
   We work from the leaf back to the current tree, allocate the arc space
   fill it in (including the terminator), allocate the next level node,
   connect the arcs to it and repeat */

for (newnode = (MIBNODE_T *)add_leaf; comp_num > 1; comp_num--) {
    newarc = (MIBARC_T *)SNMP_memory_alloc(sizeof(MIBARC_T) * 2);
    if (newarc == 0) {
        /* free any memory allocated for this branch */
        branch_free(newnode, (MIBNODE_T *)add_leaf);
        return(0);
        }

    newarc[0].id = compp[comp_num - 1];
    newarc[0].nodep = (MIBARC_T *)newnode;
    newarc[1].id = 0;
    newarc[1].nodep = 0;

    if ((newnode = (MIBNODE_T *)SNMP_memory_alloc(sizeof(MIBNODE_T))) == 0) {
        /* free any memory allocated for this branch */
        newnode = (MIBNODE_T *)(newarc->id);
        SNMP_memory_free(newarc);
        branch_free(newnode, (MIBNODE_T *)add_leaf);
        return(0);
        }

    MEMSET(newnode, 0, sizeof(MIBNODE_T));
    newnode->node_type = INNER_NODE | NODE_DYNAMIC | ARC_DYNAMIC |
                         SYSTEM_OWNED;
    newnode->arcs = newarc;
    }

/* if we found a leaf node we attach the leaf to the start of the
   new branch and then attach the branch to the arc list in the same
   place the leaf was */
if ((np->node_type & NODE_TYPE) == LEAF_NODE) {
    newnode->leaf = (MIBLEAF_T *)np;
    ap->nodep = (MIBARC_T *)newnode;
    return(1);
    }

/* num_arcs is the number of arcs in the current list */
for(num_arcs = 1, ap = np->arcs; (MIBNODE_T *)(ap->nodep) != 0; ap++)
    num_arcs++;

/* add one to num_arcs for the new arc we are adding */
newarc = (MIBARC_T *)SNMP_memory_alloc(sizeof(MIBARC_T) * (num_arcs + 1));
if (newarc == 0) {
    /* free the branch */
    branch_free(newnode, (MIBNODE_T *)add_leaf);
    return(0);
    }

/* copy all arcs less then the new arc, then add the new arc 
   then copy all the arcs after the new arc, there will always
   be at least one, the terminator */
savearc = newarc;
for(ap = np->arcs;
    (ap->id < *compp) && ((MIBNODE_T *)(ap->nodep) != 0);
    ap++, newarc++, num_arcs--)
     MEMCPY(newarc, ap, sizeof(MIBARC_T));

newarc->id = *compp;
newarc->nodep = (MIBARC_T *)newnode;
newarc++;
MEMCPY(newarc, ap, sizeof(MIBARC_T) * num_arcs);

/* now clean up, if necessary free the old arc list then tag the node so that
   the new arc list will be freed when released */
ap = np->arcs;
np->arcs = savearc;
if (np->node_type & ARC_DYNAMIC)
    SNMP_memory_free(ap);
np->node_type |= ARC_DYNAMIC;

return(1);
}

/****************************************************************************
NAME:  Remove_Leaf_From_Root

PURPOSE: remove a leaf from the given mibtree removing any branches that
         are no longer needed

PARAMETERS: MIBNODE_T * the root of the mib tree
            OBJ_ID_T  * the name of the node to remove the leaf from

RETURNS: MIBLEAF_T * the leaf that was removed 0 if there wasn't a
                     a leaf to remove or the leaf wasn't an agentx leaf
****************************************************************************/
MIBLEAF_T *
  AX_Remove_Leaf_From_Root(MIBNODE_T  *mibroot,
                           OBJ_ID_T   *objid)
{
MIBNODE_T *mnode;
MIBLEAF_T *mleaf;

if ((mibroot == 0) || Find_Node_From_Root(mibroot, objid, &mnode))
    return(0);

if ((mnode->node_type & NODE_TYPE) == LEAF_NODE) {
    if (mnode->node_type & AGENTX_LEAF)
        return((MIBLEAF_T *)Remove_Node_From_Root(mibroot, objid));
    else
        return(0);
    }

mleaf = mnode->leaf;      
if ((mleaf == 0) ||
    ((mleaf->node_type & (REMOVABLE | AGENTX_LEAF)) !=
     (REMOVABLE | AGENTX_LEAF)))
    return(0);

mnode->leaf = 0;
if ((mnode->arcs == 0) || (mnode->arcs->nodep == 0)) {
    mnode = Remove_Node_From_Root(mibroot, objid);
    if (mnode && (mnode->node_type & NODE_DYNAMIC))
        SNMP_memory_free(mnode);
    }
return(mleaf);
}

#endif /* #if INSTALL_ENVOY_AGENTX_MASTER */

/*lint +e715    */
