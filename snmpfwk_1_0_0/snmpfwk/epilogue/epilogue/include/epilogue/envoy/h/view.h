/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/view.h,v 1.1 2006/06/29 12:17:09 smasyutin Exp $ */

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
 *  Copyright 1995-1997 Epilogue Technology Corporation.
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
 * $Log: view.h,v $
 * Revision 1.1  2006/06/29 12:17:09  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:31  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:33  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:27  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 1.4  1995/11/14  22:42:26  sar
 * Added the prototype defs for the view method routines.
 *
 * Revision 1.3  1995/11/11  00:05:52  sar
 * removed use of rfc1445_view_routines
 *
 * Revision 1.2  1995/11/01  00:54:33  sar
 * removed casts of 0
 * changed the default row status to be nis instead of rsne
 *
 * Revision 1.1  1995/10/20  22:53:35  sar
 * Initial revision
 *
 *
 */

#if (!defined(view_inc))
#define view_inc

#if (!defined(snmpdefs_inc))
#include <envoy/h/snmpdefs.h>
#endif

#define STO_OTHER  1
#define STO_VOL    2
#define STO_NONVOL 3
#define STO_PERM   4

#define RS_RSNE    0
#define RS_ACTIVE  1
#define RS_NIS     2
#define RS_NREADY  3
#define RS_CAGO    4
#define RS_CAWAIT  5
#define RS_DESTROY 6

/* define the view structures */
#define VIEW_INCLUDED 1
#define VIEW_EXCLUDED 2
#define VIEW_INDETERMINATE 3
#define VIEW_MAXMASKLEN 16
#define VIEW_MAXINDEX  65535
#define VIEW_MAXINDEXL 65535L

/* options for SNMP_View_Check */
#define VIEW_CHECK_DET   0
#define VIEW_CHECK_IND   1

typedef struct VIEWLEAF_S
        {
        UINT_16_T       index;          /* index of the view table, this
                                           is the same as it's parent's view
                                           index, used for admin purposes */
        OBJ_ID_T        subtree;        /* the oid for this subtree */
        EBUFFER_T       mask;           /* wildcard string */
        int             type;           /* is this entry included or exc */
        int             status;         /* status of this entry */
        int             storage;        /* type of storage of this entry */
        struct VIEWLEAF_S *  lexinext;  /* list of leaves in lexi order */
        struct VIEWLEAF_S *  masknext;  /* list of leaves in mask order */
        } VIEWLEAF_T;

typedef struct VIEWINDEX_S
        {
        UINT_16_T               index;          /* index of the view table */
        struct VIEWINDEX_S *    next;           /* next index in the chain */
        VIEWLEAF_T  *   lexilist;       /* list of leaves in lexi order */
        VIEWLEAF_T  *   masklist;       /* list of leaves in mask order */
        UINT_32_T       quickmask;      /* mask for quick check */
        } VIEWINDEX_T;


#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
extern int         SNMP_View_Check         __((UINT_16_T, OIDC_T *,
                                               int, int));
#define SNMP_View_Get_Type(V) (V)->type
#define SNMP_View_Set_Type(V, W) (V)->type = (UINT_16_T)W
#define SNMP_View_Get_Mask(V) EBufferStart(&(V)->mask)
#define SNMP_View_Get_MaskLen(V) EBufferUsed(&(V)->mask)
#define SNMP_View_Set_StorageType(V, W) (V)->storage = (UINT_16_T)W
#define SNMP_View_Get_StorageType(V) (V)->storage
#define SNMP_View_Set_Status(V, W) (V)->status = (UINT_16_T)W
#define SNMP_View_Get_Status(V) (V)->status

#define SNMP_View_Set_Defaults(V) ((V)->index    = 0, \
                                   EBufferInitialize(&(V)->mask), \
                                   (V)->masknext = 0, \
                                   (V)->lexinext = 0, \
                                   (V)->type     = VIEW_INCLUDED, \
                                   (V)->storage  = STO_NONVOL, \
                                   (V)->status   = RS_NIS)


extern VIEWLEAF_T *SNMP_View_Create        __((OIDC_T *, int));
extern int         SNMP_View_Install       __((UINT_16_T, VIEWLEAF_T *)); 
extern int         SNMP_View_Name          __((VIEWLEAF_T *, UINT_16_T *,
                                               OIDC_T *, int));
extern int         SNMP_View_Deinstall     __((UINT_16_T, OIDC_T *, int));
extern void        SNMP_View_Delete        __((VIEWLEAF_T *));
extern VIEWLEAF_T *SNMP_View_Lookup        __((UINT_16_T, OIDC_T *, int));
extern VIEWLEAF_T *SNMP_View_Next          __((VIEWLEAF_T *));
extern int         SNMP_View_Set_Mask      __((VIEWLEAF_T *, OCTET_T *, int));

extern void        viewtable_test          __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));

extern void        viewtable_get           __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));

extern void        viewtable_next          __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));

extern void        viewtable_set           __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));

#endif /* (INSTALL_ENVOY_SNMP_RFC1445_VIEWS) */

extern VIEWINDEX_T *viewroot;

#endif /* view_inc */

