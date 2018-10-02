/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/auxfuncs.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 *  Copyright 1991-1997 Epilogue Technology Corporation.
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
 * $Log: auxfuncs.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:09  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:21  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:00  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.2  1996/01/05  19:09:34  sar
 * added group_by_getproc_and_instance
 *
 * Revision 6.1  1995/10/20  22:53:35  sar
 * removed bitstring & uinteger32 options
 *
 * Revision 6.0  1995/05/31  21:46:31  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/06  01:39:54  sar
 * moved the defs for method routines in auxi into auxmth.h
 *
 * Revision 5.3  1995/03/20  23:11:33  sar
 * Added declarations of the get_<type>_async functions
 * Updated scan_vb_forLeaf & scan_for_leaf_and_match to use vb pointers
 * instead of indexes.
 *
 * Revision 5.2  1994/10/03  21:20:27  sar
 * Added the defs for SNMP_Create_And_Send_Traps and
 * SNMP_Create_Encode_And_Send_Traps.
 *
 * Revision 5.1  1994/09/29  18:31:12  sar
 * Rearrange some items to deal with removing v2 types from v1 settings if
 * they aren't installed.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.3  1994/05/04  22:43:48  sar
 * added def for null_set_proc
 *
 * Revision 4.2  1994/04/15  21:38:04  sar
 * added the dynmaic flag to get_ui64.
 *
 * Revision 4.1  1994/03/08  18:00:01  dab
 * Added oidcmp2
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.9  1993/06/07  18:31:09  sar
 * Added the get and set functions for ui64s and the get function for
 * bit strings.
 *
 * Revision 3.8  1993/05/14  15:13:29  sar
 * removed scan_vb_and_reset as it was no longer being used.
 *
 * Revision 3.7  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo> where it was correct.
 *
 * Revision 3.6  1993/04/27  21:06:48  sar
 * Removed index_to_vbp it is now in buildpkt.h
 *
 * Revision 3.5  1993/04/17  00:58:20  sar
 * added the def for vb_extract_row_loose
 *
 * Revision 3.4  1993/03/05  03:56:25  dab
 * fixed up types
 *
 * Revision 3.3  1993/03/03  16:12:39  dab
 * Protect against multiple inclusion and remove __() definition.
 *
 * Revision 3.2  1993/03/02  19:04:38  sar
 * added definition of vb_scan_and_reset.
 *
 * Revision 3.1  1993/02/26  17:28:25  sar
 * added scan_for_leaf_and_match and scan_vb_for_leaf so they can be use
 * by outside routines.
 *
 * Revision 3.0  1992/04/03  19:52:27  dab
 * Release 3.0
 *
 * Revision 1.13  92/03/31  12:23:54  dab
 * Changed flags in the create_row structure again.
 * 
 * Revision 1.12  92/03/31  11:16:56  dab
 * Changed flags in create_row structure.
 * 
 * Revision 1.11  92/03/30  18:43:17  dab
 * Changed arguments to oid_to_ip().
 * 
 * Revision 1.10  92/03/26  10:47:08  dab
 * removed get_cookie().
 * 
 * Revision 1.9  92/02/27  16:47:39  dab
 * Changed the create_row structure to hold a MIBLEAF_T pointer instead
 * of a locator.
 * 
 * Revision 1.7  92/02/04  10:49:00  dab
 * Added oidcmp().
 * 
 * Revision 1.6  91/11/06  14:39:44  dab
 * Added vb_extract_row and the create_row structure.
 * 
 * Revision 1.5  91/11/04  15:25:56  dab
 * Renamed index parameter in function declarations so it doesn't
 * conflict with system routine index().
 * 
 * Revision 1.4  91/11/04  15:10:31  dab
 * Added oidorder().
 * 
 * Revision 1.3  91/10/31  13:08:41  dab
 * Changed args to ip_to_llist so llist is a signed long *.
 * Also added llist_cmp.
 * 
 * Revision 1.2  91/10/30  09:43:32  dab
 * New functions ip_to_rlist(), ip_to_llist(), and oid_to_ip().
 * 
 * Revision 1.1  91/10/24  10:59:34  dab
 * Initial revision
 * 
 */

#ifndef auxfuncs_inc
#define auxfuncs_inc

#if (!defined(buildpkt_inc))
#include <envoy/h/buildpkt.h>
#endif

#if (!defined(auxmth_inc))
#include <envoy/h/auxmth.h>
#endif

/* A structure used for looking up all the pieces of a row create of an
 * SNMP packet. */
struct create_row {
  MIBLEAF_T *leaf;
  OCTET_T flags;
};

#define ROW_FLAG_NEEDED 0x01
#define ROW_FLAG_CALL_SET 0x02

/* various other housekeeping functions */
extern int ip_to_llist __((UINT_32_T ip,
                           INT_32_T *llist));

extern int ip_to_rlist __((UINT_32_T  ip,
                           OIDC_T    *rlist));

extern int llist_cmp __((INT_32_T *list1,
                         INT_32_T *list2,
                         int       len));

extern int oid_to_ip __((int        tcount,
                         OIDC_T    *tlist,
                         UINT_32_T *addr));

extern int oidcmp __((int     compc_1,
                      OIDC_T *compl_1,
                      int     compc_2,
                      OIDC_T *compl_2));

extern int oidcmp2 __((int     compc_1,
                       OIDC_T *compl_1,
                       int     compc_2,
                       OIDC_T *compl_2));

extern int oidorder __((OIDC_T *idp1,
                        OIDC_T *idp2,
                        int      n));

extern int scan_vb_for_locator __((SNMP_PKT_T * pktp,
                                   int          vb_index,
                                   UINT_16_T    loc));

extern VB_T *vb_extract_row __((SNMP_PKT_T        *pkt,
                                int                start_indx,
                                int                compc,
                                OIDC_T            *compl,
                                int                row_len,
                                struct create_row *row));

extern VB_T *scan_for_leaf_and_match __((SNMP_PKT_T *pkt, 
                                         VB_T       *vbp,
                                         MIBLEAF_T  *leaf,
                                         int         compc,
                                         OIDC_T     *compl));

extern VB_T *scan_vb_for_leaf __((SNMP_PKT_T *pktp,
                                  VB_T       *vbp,
                                  MIBLEAF_T  *leaf));

extern VB_T *vb_extract_row_loose __((SNMP_PKT_T *pktp, 
                                      int         indx,
                                      MIBLEAF_T **leaves,
                                      int         compc,
                                      OIDC_T     *compl));

extern void group_by_getproc_and_instance __((SNMP_PKT_T *pktp,
                                              VB_T       *first_vbp,
                                              int         compc,
                                              OIDC_T     *compl));

#endif /* auxfuncs_inc */
