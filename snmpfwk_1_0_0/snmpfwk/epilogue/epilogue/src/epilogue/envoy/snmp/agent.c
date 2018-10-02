/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/agent.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 *  Copyright 1996-1997 Epilogue Technology Corporation.
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
 * $Log: agent.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:35  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:31  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 1.1  1996/01/10  16:34:47  sar
 * Initial revision
 *
 */

#include <asn1.h>
#include <snmp.h>
#include <snmpdefs.h>
#include <vbdefs.h>

#include <agent.h>

INT_32_T  usec_agentSize = SNMP_MAX_PACKET_SIZE;
INT_32_T  usec_offsetTime = 0;
UINT_32_T usec_agentBoots;
OCTET_T   usec_agentID[12];

/*AGENTLEAF_T agent1 = { 0, {0,0,0,0,0,0,0,0,0,0,0,0}, 0, 0}; */
AGENTLEAF_T *root_agent = 0;

AGENTLEAF_T *
  SNMP_Agent_Lookup(OCTET_T *agentid)
{
AGENTLEAF_T *agent;
for(agent = root_agent; agent; agent = agent->next)
    if (MEMCMP(agentid, agent->agentid, 12) == 0)
        return(agent);
return(0);
}

INT_16_T
  SNMP_Agent_Install(OCTET_T   *agentid,
                     UINT_32_T  agentboots,
                     INT_32_T   offsettime)
{
AGENTLEAF_T *agent;
if (SNMP_Agent_Lookup(agentid))
    return(1);
if ((agent = SNMP_memory_alloc(sizeof(AGENTLEAF_T))) == 0)
    return(-1);
MEMCPY(agent->agentid, agentid, 12);
agent->agentboots = agentboots;
agent->offsettime = offsettime;
agent->lasttime = 0;
agent->next = root_agent;
root_agent = agent;
return(0);
}

void
  get_agentTime(OIDC_T      lastmatch,
                int         compc,
                OIDC_T     *compl,
                SNMP_PKT_T *pktp,
                VB_T       *vbp)
{
/* Test the instance qualifier, we must have one subid
   with a value of 0 */
if ((compc != 1) || (*compl != 0))
    getproc_nosuchins(pktp, vbp);
else
    getproc_got_uint32(pktp, vbp, usec_offsetTime + SNMP_USECTIME, VT_GAUGE);
return;
}

void
  get_agentID(OIDC_T      lastmatch,
              int         compc,
              OIDC_T     *compl,
              SNMP_PKT_T *pktp,
              VB_T       *vbp)
{
/* Test the instance qualifier, we must have one subid
   with a value of 0 */
if ((compc != 1) || (*compl != 0))
    getproc_nosuchins(pktp, vbp);
else
    getproc_got_string(pktp, vbp, 12, usec_agentID, 0, VT_STRING);
return;
}
