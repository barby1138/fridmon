/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/env_init.c,v 1.1 2006/06/29 12:17:27 smasyutin Exp $ */

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
 * $Log: env_init.c,v $
 * Revision 1.1  2006/06/29 12:17:27  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:48  sar
 * Updated revision to 8.0
 *
 * Revision 7.1  1997/10/22 03:10:20  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 */

#include <envoy/h/asn1.h>
#include <envoy/h/snmp.h>

#if INSTALL_ENVOY_SNMP_LOCK

ENVOY_LOCK_T SNMP_CoarseLock;
ENVOY_LOCK_T SNMP_TimerLock;

#if INSTALL_ENVOY_AGENTX_MASTER
ENVOY_LOCK_T SNMP_infrastructure_lock;
#endif

#if INSTALL_ENVOY_SNMP_PROXY
ENVOY_LOCK_T SNMP_ProxyLock;
#endif

#if INSTALL_ENVOY_EPI_MASTER
ENVOY_LOCK_T SNMP_SubagentLock;
#endif

void
  envoy_init()
{

ENVOY_SNMP_INIT_LOCK(SNMP_CoarseLock);
ENVOY_SNMP_INIT_LOCK(SNMP_TimerLock);

#if INSTALL_ENVOY_AGENTX_MASTER
ENVOY_SNMP_INIT_LOCK(SNMP_infrastructure_lock);
#endif

#if INSTALL_ENVOY_SNMP_PROXY
ENVOY_SNMP_INIT_LOCK(SNMP_ProxyLock);
#endif

#if INSTALL_ENVOY_EPI_MASTER
ENVOY_SNMP_INIT_LOCK(SNMP_SubagentLock);
#endif

return;
}

#else

void
  envoy_init()
{
return;
}

#endif


