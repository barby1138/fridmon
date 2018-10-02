/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/snmpstat.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: snmpstat.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:28  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:19  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.3  1996/01/10  16:23:25  sar
 * USEC updates
 *
 * Revision 6.2  1995/11/14  22:42:26  sar
 * renamed proxydrops & silentdrops to match the mib definitions.
 *
 * Revision 6.1  1995/10/20  22:53:35  sar
 * moved some stats into and out of the ifdef blocks
 * removed the v2 classic stats
 * added some v2 community stats
 *
 * Revision 6.0  1995/05/31  21:46:53  sra
 * Release 6.0.
 *
 * Revision 5.1  1995/03/20  23:11:33  sar
 * Moved the deinition of the statistics macros here so there is one
 * central point for them.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.5  1993/06/02  23:05:55  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.4  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo> where it was correct.
 *
 * Revision 3.3  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 3.2  1993/02/18  20:43:26  sar
 * Added DestPartyMismatches and UnknownContexts to the v2 snmpstat group.
 *
 * Revision 3.1  1993/02/17  20:55:10  sar
 * Updated stats to deal with v1 and v2 options.
 *
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 1.1  91/10/30  20:45:25  dab
 * Initial revision
 * 
 */

#if (!defined(snmpstat_inc))
#define snmpstat_inc

#if (!defined(snmpdefs_inc))
#include <envoy/h/snmpdefs.h>
#endif

#if (!defined(snmp_inc))
#include <envoy/h/snmp.h>
#endif

#if (INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2 || \
     INSTALL_ENVOY_SNMP_VERSION_USEC)
typedef struct  SNMP_STATS_S
        {
        unsigned long   snmpInPkts;
        unsigned long   snmpInBadVersions;
        unsigned long   snmpInBadCommunityNames;
        unsigned long   snmpInBadCommunityUses;
        unsigned long   snmpInASNParseErrs;
        unsigned short  snmpEnableAuthTraps;
#if INSTALL_ENVOY_SNMP_GROUP_V1
        unsigned long   snmpOutPkts;
        unsigned long   snmpInBadTypes;
        unsigned long   snmpInTooBigs;
        unsigned long   snmpInNoSuchNames;
        unsigned long   snmpInBadValues;
        unsigned long   snmpInReadOnlys;
        unsigned long   snmpInGenErrs;
        unsigned long   snmpInTotalReqVars;
        unsigned long   snmpInTotalSetVars;
        unsigned long   snmpInGetRequests;
        unsigned long   snmpInGetNexts;
        unsigned long   snmpInSetRequests;
        unsigned long   snmpInGetResponses;
        unsigned long   snmpInTraps;
        unsigned long   snmpOutTooBigs;
        unsigned long   snmpOutNoSuchNames;
        unsigned long   snmpOutBadValues;
        unsigned long   snmpOutReadOnlys;
        unsigned long   snmpOutGenErrs;
        unsigned long   snmpOutGetRequests;
        unsigned long   snmpOutGetNexts;
        unsigned long   snmpOutSetRequests;
        unsigned long   snmpOutGetResponses;
        unsigned long   snmpOutTraps;
#endif /* INSTALL_ENVOY_SNMP_GROUP_V1 */
#if INSTALL_ENVOY_SNMP_GROUP_V2
        unsigned long   snmpSilentDrops;
        unsigned long   snmpProxyDrops;
#endif /* INSTALL_ENVOY_SNMP_GROUP_V2 */
#if INSTALL_ENVOY_SNMP_VERSION_USEC
        unsigned long   usecStatsUnsupportedQoS;
        unsigned long   usecStatsNotInWindows;
        unsigned long   usecStatsUnknownUserNames;
        unsigned long   usecStatsWrongDigestValues;
        unsigned long   usecStatsUnknownContexts;
        unsigned long   usecStatsBadParameters;
        unsigned long   usecStatsUnauthorizedOperations;
        unsigned long   usecStats;
#endif /* INSTALL_ENVOY_SNMP_VERSION_USEC */
        } SNMP_STATS_T;

extern  SNMP_STATS_T    snmp_stats;

#endif  /* INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2 */


/* Some macros to make the mainline code a little cleaner to read */
#if (INSTALL_ENVOY_SNMP_GROUP_V1)
#define SGRPv1_INC_COUNTER(x) inc_counter(x)
#define SGRPv1_ADD_COUNTER(x, y) add_counter(x, y)
#else
#define SGRPv1_INC_COUNTER(x)
#define SGRPv1_ADD_COUNTER(x, y)
#endif

#if (INSTALL_ENVOY_SNMP_GROUP_V2)
#define SGRPv2_INC_COUNTER(x) inc_counter(x)
#else
#define SGRPv2_INC_COUNTER(x)
#endif

#if (INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2)
#define SGRPv1v2_INC_COUNTER(x) inc_counter(x)
#else
#define SGRPv1v2_INC_COUNTER(x)
#endif

#if (INSTALL_ENVOY_SNMP_VERSION_USEC)
#define SGRP_USEC_INC_COUNTER(x) inc_counter(x)
#else
#define SGRP_USEC_INC_COUNTER(x)
#endif

#endif /* snmpstat_inc */
