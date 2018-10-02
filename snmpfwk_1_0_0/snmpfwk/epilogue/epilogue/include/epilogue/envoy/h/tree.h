/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/tree.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: tree.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:31  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:21  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:55  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/09/29  18:31:12  sar
 * Minor cleanup, adding other include files.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.1  1994/05/10  20:17:19  sar
 * Removed the #if that restricted it for use with v2 only
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 1.8  1993/06/02  23:05:55  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 1.7  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo> where it was correct.
 *
 * Revision 1.6  1993/04/30  22:58:26  sar
 * Added mechanism for minimal proxies and coarse grained locks.
 *
 * Revision 1.5  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 1.4  1993/04/17  00:58:50  sar
 * changed def of TREE_Add to allow the user to determine what node the leaf
 * was attatched to.
 *
 * Revision 1.3  1993/03/05  03:56:25  dab
 * fixed up types
 *
 * Revision 1.2  1993/02/24  00:59:50  sar
 * Removed the name objid from the partyleaf and put a copy of it in the
 * pkt.  Also Updated many of the SNMP_(Party, Context, ACL)_* files
 * to use a OIDC_T/length instead of a OBJ_ID_T so we don't have to
 * allocate an OBJ_ID_T when we don't have one.
 *
 * Revision 1.1  1993/02/17  20:55:28  sar
 * Initial revision
 *
 * Revision 3.0  92/09/09  sar
*/

#include <envoy/h/asn1.h>

#if (!defined(tree_inc))
#define tree_inc

/* Define a generic tree structure */
typedef struct TREENODE_S
        {
        INT_32_T                status;         /* flag and status word */
        OIDC_T                  name;           /* subid for this level */
        struct TREENODE_S *     parent;         /* ptr to one level up  */
        struct TREENODE_S *     sibling;        /* ptr to this level */
        PTR_T                   child;          /* ptr to one level down */
                                                /* or to the data block  */
        } TREENODE_T;
        
#define TREESTATIC  0x1         /* Node may not be deleted, ptrs may change */
#define TREETWIN    0x2         /* Next node contains a leaf pointer */
#define TREECHILD   0x4         /* Node contains a child pointer */
#define TREELEAF    0x8         /* Node contains a leaf pointer */

extern int      TREE_Add            __((TREENODE_T **root, OIDC_T *oidc,
                                        int oidclen, PTR_T leaf,
                                        TREENODE_T **retnode));
extern int      TREE_Delete         __((TREENODE_T **root, TREENODE_T *node));
extern TREENODE_T *TREE_Get         __((TREENODE_T *root, OIDC_T *oidc, 
                                        int oidclen, int *length));
extern TREENODE_T *TREE_GetPrev     __((TREENODE_T *root, OIDC_T *oidc,
                                        int oidclen));
extern TREENODE_T *TREE_GetNext     __((TREENODE_T *root, TREENODE_T *node));
extern int      TREE_Extract     __((TREENODE_T *node, PTR_T *leaf));
extern int      TREE_Install     __((TREENODE_T *node, PTR_T leaf));
extern int      TREE_Name        __((TREENODE_T *node, OIDC_T *oidc, int len));

#endif /* tree_inc */
