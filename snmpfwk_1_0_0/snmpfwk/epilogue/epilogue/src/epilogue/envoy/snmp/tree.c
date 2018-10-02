/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/tree.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 *  Copyright 1993-1997 Epilogue Technology Corporation.
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
 * $Log: tree.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:04  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/10/16 00:48:09  sar
 * In tree_add make sure we set the parent pointer for all cases
 *
 * Revision 7.2  1997/03/20 06:49:23  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:48:24  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/07/27  18:30:06  sar
 * Modified TREE_GetPrev.  Previously after finding a node whose next
 * sibling's name was greater than the current oid we would stop and
 * were supposed to descend the subtree starting at the first node.
 * Instead we would follow the sibling chain and then descend the last
 * siblings subtree.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.1  1994/05/10  20:16:03  sar
 * Remvoed the #if for version 2 as we can use trees with v1 (sub agent stuff)
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 1.12  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 1.11  1993/05/10  14:22:04  sar
 * Modfied Get and Get_Prev to return the current node when reaching the end
 * of the oid list.  (Previously we would do one extra child decesnt under some
 * conditions.
 *
 * Revision 1.10  1993/04/30  22:54:08  sar
 * Added mechanism for minimial proxies and coarse grained locks.  The
 * latter required the addition of routines to get the name of a leaf given
 * a pointer to the leaf.
 *
 * Revision 1.9  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 */

#include <asn1conf.h>
#include <tree.h>

/****************************************************************************
NAME: TREE_Add

PURPOSE: Add a node to the tree creating intermediate nodes as needed.
         The add fails if space cannot be allocated for some node or
         if a leaf already exists at the specified node.
PARAMETERS: 
        TREENODE_T **   Pointer to root to start the search from, if the root
                        is (TREENODE_T *)0 it will be filled in.
        OIDC_T     *    Id to add
        int             Length of oid to add
        PTR_T           Data block to link in.
        TREENODE_T *    Pointer to location to fill with the address of the
                        node that has the pointer to PTR_T

RETURNS: int            0 = success.
                        
****************************************************************************/

#if !defined(NO_PP)
int
  TREE_Add(TREENODE_T **root,
           OIDC_T      *oidc,
           int          oidclen,
           PTR_T        leaf,
           TREENODE_T **retnode)
#else /* NO_PP */
int
  TREE_Add(root, oidc, oidclen, leaf, retnode)
TREENODE_T **root;
OIDC_T      *oidc;
int          oidclen;
PTR_T        leaf;
TREENODE_T **retnode;
#endif /* NO_PP */
{
UINT_32_T status = 0;
TREENODE_T *node, *newnode, **prevnodeptr, *parent = 0;;

/* make sure there is something to the oid */
if ((oidclen == 0) || (oidc == 0))
    return(-1);

/* match the given oid against the tree until we can't match a subid
   or run out of components */
for(node = *root, prevnodeptr = root; (node != 0) && (node->name <= *oidc); ){ 
    if (node->name != *oidc) {
        prevnodeptr = &(node->sibling);
        node = node->sibling;
        continue;
        }

    if (oidclen == 1) {
        /* Out of subids so the leaf node goes on this level */
        /* if we have a leaf or twin return an error */
        if (node->status & (TREELEAF | TREETWIN))
            return(-1);

        /* If we don't have a child install the leaf here. */
        if (!(node->status & TREECHILD)) {
            node->status |= TREELEAF;
            node->child = leaf;
            *retnode = node;
            return(0);
            }

        /* We have a child so install the leaf after the child */
        newnode = (TREENODE_T *)SNMP_memory_alloc(sizeof(TREENODE_T));
        if (newnode == 0)
            return(-1);
        newnode->parent  = parent;
        newnode->name    = node->name;
        newnode->status  = TREELEAF;
        newnode->sibling = node->sibling;
        newnode->child   = leaf;
        *retnode = newnode;

        node->status  |= TREETWIN;
        node->sibling  = newnode;
        return(0);
        }
            
    /* More subids to go so this is a child level */
    /* If the current node is a leaf node we need to add a child node before 
       the leaf node and prepare for twining */
    if (node->status & TREELEAF) {
        status = TREETWIN;
        break;
        }

    /* Normal conditions, follow the child */
    oidc++;
    oidclen--;
    parent = node;
    prevnodeptr = (TREENODE_T **)&(node->child);
    node = (TREENODE_T *)node->child;
    }

/* build nodes for the part of the oid that wasn't in the tree */
/* create the node that gets linked into the current tree */
newnode = (TREENODE_T *)SNMP_memory_alloc(sizeof(TREENODE_T));
if (newnode == 0)
    return(-1);
newnode->parent  = parent;
newnode->name    = *oidc;
newnode->status  = status | TREECHILD;
newnode->sibling = node;
/* mostly set to be linked in we made need to update the status if this is
   the leaf pointer.  save it in case something fails */
node = newnode;

for(parent = node, oidclen--, oidc++; oidclen > 0; oidclen--, oidc++) {
    newnode = (TREENODE_T *)SNMP_memory_alloc(sizeof(TREENODE_T));
    if (newnode == 0) {
        /* free the chain */
        while(node->child != 0) {
            node = (TREENODE_T *)node->child;
            SNMP_memory_free(node->parent);
            }
        SNMP_memory_free(node);
        return(-1);
        }
    newnode->parent  = parent;
    newnode->name    = *oidc;
    newnode->status  = TREECHILD;
    newnode->sibling = 0;
    /* link the child to the parent and step the parent down */
    parent->child    = (PTR_T)newnode;
    parent           = newnode;
    }

/* declare the last structure a leaf structure and put the leaf pointer in */
newnode->child = leaf;
newnode->status = (newnode->status & ~TREECHILD) | TREELEAF;
*retnode = newnode;

/* link the chain into the tree */
*prevnodeptr = node;
return(0);
}

/****************************************************************************
NAME: TREE_Delete

PURPOSE: Delete a node and any ancestors that are needed only to get to it
         (ancestors that have no other descendents or leaves).
         The node must not have any children and any leaves must be handled
         by the calling routine (moved or freed).
PARAMETERS: 
        TREENODE_T **   The root of the tree
        TREENODE_T  *   The node to free.

RETURNS: int            0 = success.
                        
****************************************************************************/

#if !defined(NO_PP)
int
  TREE_Delete(TREENODE_T **root,
              TREENODE_T  *node)
#else /* NO_PP */
int
  TREE_Delete(root, node)
TREENODE_T **root;
TREENODE_T  *node;
#endif /* NO_PP */
{
TREENODE_T *parent;

/* make sure we have a node to work with and a tree to put delete it from */
if ((node == 0) || (*root == 0))
    return(-1);

/* if we have a twin unlink and free the node that points to the leaf */
/* we don't have any other work as we know we have another sibling */
if (node->status & TREETWIN) {
    parent = node->sibling;
    if (parent->status & TREESTATIC)
        return(-1);
    node->status &= ~TREETWIN;
    node->sibling = parent->sibling;
    SNMP_memory_free(parent);
    return(0);
    }

/* the node has children or is static */
if (node->status & (TREECHILD || TREESTATIC))
    return(-1);

/* walk up the parent chain until we have other siblings, run out of parents
   or run into a parent that isn't dynamic */
for(parent = node->parent; ;node = parent, parent = node->parent) {

    /* if we are at the top of the tree clean up the root pointers */
    if (parent == 0) {
        if (*root == node)
            *root = node->sibling;
        else {
            for(parent = *root;
                 parent->sibling != node;
                 parent = parent->sibling)
                 ;
            parent->sibling = node->sibling;
            }
        break;
        }

    /* does this node have siblings or is the parent static */
    if ((node->sibling != 0) ||
        (parent->child != (PTR_T) node) ||
        (parent->status & TREESTATIC)) {
        if (parent->child == (PTR_T) node)
            parent->child = (PTR_T)node->sibling;
        else {
            for(parent = (TREENODE_T *)parent->child;
                 parent->sibling != node;
                 parent = parent->sibling)
                ;
            parent->sibling = node->sibling;
            }
        break;
        }
  }

/* we have removed a chain from the tree now free the chain */
while(node->status & TREECHILD) {
    node = (TREENODE_T *)node->child;
    SNMP_memory_free(node->parent);
    }
SNMP_memory_free(node);
return(0);
}

/****************************************************************************
NAME: TREE_Get

PURPOSE: Search the tree starting from the given root for the named oid.
         The search terminates when the oid is consumed or when it runs out
         of tree.  The last node matched is returned and length is the
         number of subids matched.

PARAMETERS: 
        TREENODE_T *    Root to start the search from
        OIDC_T     *    Id to find
        int             Length of oid to find
        int        *    Returned length.

RETURNS: TREENODE_T *   Pointer to the last node matched, 
                        (TREENODE *)0 on failure
                        
****************************************************************************/

#if !defined(NO_PP)
TREENODE_T *
  TREE_Get(TREENODE_T *root,
           OIDC_T     *oidc,
           int         oidclen,
           int        *length)
#else /* NO_PP */
TREENODE_T *
  TREE_Get(root, oidc, oidclen, length)
TREENODE_T *root;
OIDC_T     *oidc;
int         oidclen;
int        *length;
#endif /* NO_PP */
{
int indx;
TREENODE_T *node;

/* make sure we have a tree to work with */
if ((node = root) == 0)
    return(0);

/* make sure we have an oid to work with */
if (((indx = oidclen) == 0) || (oidc == 0))
    return(0);

/* step through the oid */
while (1) {    
    /* search through this level for a match to the subid */
    for (;*oidc != node->name; node = node->sibling)
        /* about to run off the end of the list return what we have */
        if (node->sibling == 0) {
            if ((*length = oidclen - indx) == 0)
                return(0);
            else
                /* must have matched somewhere so return the parent */
                return(node->parent);
            }

    /* we have a match.  If we are out of subids or there are no 
       children break */
    indx--;
    oidc++;

    if ((indx == 0) || !(node->status & TREECHILD))
        break;

    node = (TREENODE_T *)node->child;
    }

*length = oidclen - indx;
return(node);
}

/****************************************************************************
NAME: TREE_GetPrev

PURPOSE: From the given oid and starting point find the node before the
         lexi-next node.

PARAMETERS: 
        TREENODE_T *    Root to start the search from
        OIDC_T     *    Id to find
        int             Length of oid to find

RETURNS: TREENODE_T *   The node to do TREE_GetNext on. 
                        (TREENODE_T *)0 if the desired node is before the tree.
                        
****************************************************************************/

#if !defined(NO_PP)
TREENODE_T *
  TREE_GetPrev(TREENODE_T *root,
               OIDC_T     *oidc,
               int         oidclen)
#else /* NO_PP */
TREENODE_T *
  TREE_GetPrev(root, oidc, oidclen)
TREENODE_T *root;
OIDC_T     *oidc;
int         oidclen;
#endif /* NO_PP */
{
TREENODE_T *node;

/* make sure we have a root */
if ((node = root) == 0)
    return(0);

/* make sure we have an oid to work with */
if ((oidclen == 0) || (oidc == 0))
    return(0);

/* if the oid is before the root return (TREENODE_T *)0 */
if (*oidc < node->name)
    return(0);

/* step through the oid */
while(1) {
    
    /* search through this level for a match to the subid */
    for (;*oidc != node->name; node = node->sibling)
        /* check for end of list or about to pass the node */
        if ((node->sibling == 0) || (node->sibling->name > *oidc)) {
            /* return the last node in the subtree rooted at node
               if there aren't any children we return node
               otherwise we go do one level and then start looping
               - going to the end of the sibling list and descending */
            if (!(node->status & TREECHILD))
                return(node);
            node = (TREENODE_T *)node->child;
            while(1) {
                if (node->sibling == 0) {
                    if (node->status & TREECHILD) {
                        node = (TREENODE_T *)node->child;
                        continue;
                        }
                    else
                        return(node);
                    }
                node = node->sibling;
                }
            }

    /* we have a match, prepare to descend the tree.  adjust the 
       counters then check that we have more subids, we have a 
       child and that the the next subid wouldn't be before the
       child (the subid isn't less than the childs name) */
    oidclen--;
    oidc++;
    if ((oidclen == 0) || 
        (!(node->status & TREECHILD)) ||
        (*oidc < ((TREENODE_T *)node->child)->name))
        break;
    node = (TREENODE_T *)node->child;
    }

return(node);
}

/****************************************************************************
NAME: TREE_GetNext

PURPOSE: Find the first leaf after the given node.  If the node is 0
         find the first leaf in the given tree.

PARAMETERS: 
        TREENODE_T *    Root to start the search from
        TREENODE_T *    Node to start from

RETURNS: TREENODE_T *   The first node that has a leaf.
                        (TREENODE_T *)0 if the desired node is after the tree.
                        
****************************************************************************/

#if !defined(NO_PP)
TREENODE_T *
  TREE_GetNext(TREENODE_T *root,
               TREENODE_T *node)
#else /* NO_PP */
TREENODE_T *
  TREE_GetNext(root, node)
TREENODE_T *root;
TREENODE_T *node;
#endif /* NO_PP */
{
TREENODE_T *lnode;

/* get lnode pointing to the starting point. */
if ((lnode = node) == 0) {
    if ((lnode = root) == 0)
        return(0);
    else {
        if (lnode->status & (TREELEAF | TREETWIN))
            return(lnode);
        }
    }

while(1) {
    if (lnode->status & TREECHILD)
        lnode = (TREENODE_T *)lnode->child;
    else {
        if (lnode->sibling != 0)
            lnode = lnode->sibling;
        else
            while (1) {
                if ((lnode = lnode->parent) == 0)
                    return(0);
                if (lnode->status & TREETWIN)
                    lnode = lnode->sibling;
                if (lnode->sibling != 0) {
                    lnode = lnode->sibling;
                    break;
                    }
                }
        }

    if (lnode->status & (TREELEAF | TREETWIN))
        return(lnode);
    }
}

/****************************************************************************
NAME: TREE_Extract

PURPOSE: Extract the leaf from the given node.

PARAMETERS: 
        TREENODE_T *    Node to extract leaf from
        PTR_T      *    Leaf we extracted

RETURNS: int            0 if we found a leaf, -1 if not.
                        
****************************************************************************/

#if !defined(NO_PP)
int
  TREE_Extract(TREENODE_T *node,
               PTR_T      *leaf)
#else /* NO_PP */
int
  TREE_Extract(node, leaf)
TREENODE_T *node;
PTR_T *leaf;
#endif /* NO_PP */
{
/* make sure we have a node */
if (node == 0)
    return(-1);

if (node->status & TREELEAF) {
    *leaf = node->child;
    return(0);
    }

if (node->status & TREETWIN) {
    *leaf = node->sibling->child;
    return(0);
    }

return(-1);
}

/****************************************************************************
NAME: TREE_Install

PURPOSE: Given a node and a leaf install the leaf into the node.
         The caller must deal with any current leaf.

PARAMETERS: 
        TREENODE_T *    Node to install leaf in
        PTR_T           Leaf to install

RETURNS: int            0 if we installed the leaf, -1 if not.
                        
****************************************************************************/

#if !defined(NO_PP)
int
  TREE_Install(TREENODE_T *node,
               PTR_T       leaf)
#else /* NO_PP */
int
  TREE_Install(node, leaf)
TREENODE_T *node;
PTR_T leaf;
#endif /* NO_PP */
{
/* make sure we have a node */
if (node == 0)
    return(-1);

/* if we have a leaf just stick the pointer in */
if (node->status & TREELEAF) {
    node->child = leaf;
    return(0);
    }

/* if we have a twin the next node had better be a leaf so stick 
   the pointer there */
if (node->status & TREETWIN) {
    node->sibling->child = leaf;
    return(0);
    }

/* if we have a child we have no place to put the pointer */
if (node->status & TREECHILD)
    return(-1);

/* we have a node that isn't tagged so make it a leaf node and put
   the pointer in */
node->child = leaf;
node->status |= TREELEAF;
return(0);
}

/****************************************************************************
NAME: TREE_Name

PURPOSE: Determine the name (objid) of the given node.

PARAMETERS: 
        TREENODE_T *    Node to find the name of.
        OIDC_T     *    Space for the returned name
        int             length of space for name

RETURNS: int            Number of subids in the name, if greater than
                        the input length the returned name is empty
                        0 on other failures
                        
****************************************************************************/

#if !defined(NO_PP)
int
  TREE_Name(TREENODE_T *node,
            OIDC_T     *oidc,
            int         len)
#else /* NO_PP */
int
  TREE_Name(node, oidc, len)
TREENODE_T *node;
OIDC_T     *oidc;
int         len;
#endif /* NO_PP */
{
int i, j;
TREENODE_T *anc;

/* make sure we have a node */
if (node == 0)
    return(0);

/* count the number of subids */
for(i = 1, anc = node; anc->parent != 0; i++, anc = anc->parent)
    ;

if (i > len)
    return(i);

/* save the number of subids for return */
j = i;

/* build the objid, going up the levels */
for(anc = node; i > 0; i--, anc = anc->parent)
    oidc[i-1] = anc->name;

return(j);
}

