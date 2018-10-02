/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/ax_mth.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 *  Copyright 1994-1997 Epilogue Technology Corporation.
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
 * $Log: ax_mth.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:22  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/10/29 04:11:41  sar
 * Cleaned up some type mismatches to keep compilers happy
 *
 * Revision 7.1  1997/10/16 00:25:17  sar
 * Primarily this set of modifications is for the agentx protocol.
 * Most of the work was in the agentx.h and ax_mth.h files.  Most
 * other files (snmpdefs.h, vbdefs.h and mib.h) received updates
 * to their structures and snmp got some new lock macros.
 *
 * One other change of note was the modification of continue
 * processing.  Previously we called a specific continue routine
 * directly, now we store a pointer to the corret continue
 * function in the snmp packet structure and call that.  We include
 * macros for backwards compatibility.
 *
 */

#include <envoy/h/snmpdefs.h>
#include <envoy/h/vbdefs.h>
#include <envoy/h/tree.h>

extern  bits32_t  agentxDefaultTimeout;
extern sbits32_t  agentxMasterAgentXVer; 
extern  bits8_t  *agentxMasterTransports;

extern  bits32_t  agentxSATableLastChange;
extern  bits32_t  agentxRegisterDuplicate;
extern  bits32_t  agentxSANumber;

extern TREENODE_T *registration_root;

extern void ax_get_number_lock (OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);

extern void agentxDefaultTimeout_set  (OIDC_T, int, OIDC_T *,
                                       SNMP_PKT_T *, VB_T *);
extern void agentxDefaultTimeout_test (OIDC_T, int, OIDC_T *,
                                       SNMP_PKT_T *, VB_T *);

extern void ax_mth_sa_getproc  (OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);
extern void ax_mth_sa_nextproc (OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);
extern void ax_mth_sa_testproc (OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);
extern void ax_mth_sa_setproc  (OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);

extern void ax_mth_reg_getproc (OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);
extern void ax_mth_reg_nextproc(OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);
extern void ax_mth_reg_testproc(OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);
extern void ax_mth_reg_setproc (OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *);

