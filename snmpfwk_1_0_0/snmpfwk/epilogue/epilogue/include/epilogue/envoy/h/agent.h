/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/agent.h,v 1.1 2006/06/29 12:17:07 smasyutin Exp $ */

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
 * $Log: agent.h,v $
 * Revision 1.1  2006/06/29 12:17:07  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:09  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:19  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:47:57  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 1.1  1996/01/10  16:23:25  sar
 * Initial revision
 *
 */

#if (!defined(agent_inc))
#define agent_inc
#include <envoy/h/asn1.h>

#if (!defined(snmpdefs_inc))
#include <snmpdefs.h>
#endif
#if (!defined(vb_inc))
#include <vbdefs.h>
#endif

/*#define SNMP_USECTIME 0*/

typedef struct AGENTLEAF_S 
        {
        struct AGENTLEAF_S *next;
        OCTET_T   agentid[12];
        UINT_32_T agentboots;
        INT_32_T  offsettime;
        INT_32_T  lasttime;
        } AGENTLEAF_T;

extern AGENTLEAF_T * SNMP_Agent_Lookup __((OCTET_T *));
extern INT_16_T      SNMP_Agent_Install __((OCTET_T *, UINT_32_T, INT_32_T));

extern INT_32_T  usec_agentSize;
extern INT_32_T  usec_offsetTime;
extern UINT_32_T usec_agentBoots;
extern OCTET_T   usec_agentID[12];

extern void  get_agentID   __((OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *));
extern void  get_agentTime __((OIDC_T, int, OIDC_T *, SNMP_PKT_T *, VB_T *));

#endif /* (!defined(agent_inc)) */


