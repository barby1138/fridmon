/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/snmpvars.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: snmpvars.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:13  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:49:42  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:24:05  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/11/01  00:38:58  sar
 * Removed used of bitstrings and unsigned32s
 *
 * Revision 6.0  1995/05/31  21:49:27  sra
 * Release 6.0.
 *
 * Revision 5.2  1994/09/29  18:23:27  sar
 * Check the install option before using v2 types in a v1 setting.
 *
 * Revision 5.1  1994/07/19  19:29:32  sar
 * Added the defs for the variables underlying the test group.
 *
 * Revision 5.0  1994/05/16  17:21:18  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  17:53:56  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 3.0  1992/04/03  19:54:08  dab
 * Release 3.0
 *
 * Revision 1.1  92/03/31  16:43:47  dab
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:48
 * Release 2.00
 * 
 *    Rev 1.1   26 Aug 1989 16:50:34
 * Moved ipForwarding into the ip_stats structure.
 * 
 *    Rev 1.0   14 Nov 1988 10:27:16
 * Initial revision.
*/

#if (!defined(snmpvars_inc))
#define snmpvars_inc

#if (!defined(asn1_inc))
#include <asn1.h>
#endif
#include <ui64.h>

/**********************************************************************
 *
 * Define the various SNMP management variables used in this system.
 *
 * This file should closely match snmpvars.c
 *
 **********************************************************************/

#define SNMP_COMM_MAX   32

extern  int             if_number;              /* ifNumber             */
extern  unsigned long   system_clicks;          /* System time          */

extern  char            snmp_get_request_community[];
extern  char            snmp_set_request_community[];
extern  char            snmp_trap_community[];
extern  char            snmp_auth_traps;
extern  unsigned char   snmp_trap_receiver_ip[];
extern  OIDC_T          snmp_product_id[];
extern  int             snmp_product_id_count;
extern  OBJ_ID_T        snmp_system_object_id;
extern  unsigned char   snmp_local_ip_address[];
extern  char            snmp_system_description[];

typedef struct  IP_STATS_S
        {
        int             ip_Forwarding;
        int             ip_DefaultTTL;
        unsigned long   ip_InReceives;
        unsigned long   ip_InHdrErrors;
        unsigned long   ip_InAddrErrors;
        unsigned long   ip_ForwDatagrams;
        unsigned long   ip_InUnknownProtos;
        unsigned long   ip_InDiscards;
        unsigned long   ip_InDelivers;
        unsigned long   ip_OutRequests;
        unsigned long   ip_OutDiscards;
        unsigned long   ip_OutNoRoutes;
        unsigned long   ip_ReasmTimeout;
        unsigned long   ip_ReasmReqds;
        unsigned long   ip_ReasmOKs;
        unsigned long   ip_ReasmFails;
        unsigned long   ip_FragOKs;
        unsigned long   ip_FragFails;
        unsigned long   ip_FragCreates;
        } IP_STATS_T;

extern  IP_STATS_T      ip_stats;

typedef struct  ICMP_STATS_S
        {
        unsigned long   icmp_InMsgs;
        unsigned long   icmp_InErrors;
        unsigned long   icmp_InDestUnreachs;
        unsigned long   icmp_InTimeExcds;
        unsigned long   icmp_InParmProbs;
        unsigned long   icmp_InSrcQuenchs;
        unsigned long   icmp_InRedirects;
        unsigned long   icmp_InEchos;
        unsigned long   icmp_InEchoReps;
        unsigned long   icmp_InTimestamps;
        unsigned long   icmp_InTimestampReps;
        unsigned long   icmp_InAddrMasks;
        unsigned long   icmp_InAddrMaskReps;
        unsigned long   icmp_OutMsgs;
        unsigned long   icmp_OutErrors;
        unsigned long   icmp_OutDestUnreachs;
        unsigned long   icmp_OutTimeExcds;
        unsigned long   icmp_OutParmProbs;
        unsigned long   icmp_OutSrcQuenchs;
        unsigned long   icmp_OutRedirects;
        unsigned long   icmp_OutEchos;
        unsigned long   icmp_OutEchoReps;
        unsigned long   icmp_OutTimestamps;
        unsigned long   icmp_OutTimestampReps;
        unsigned long   icmp_OutAddrMasks;
        unsigned long   icmp_OutAddrMaskReps;
        } ICMP_STATS_T;

extern  ICMP_STATS_T    icmp_stats;

typedef struct  TCP_STATS_S
        {
        int             tcp_RtoAlg;
        int             tcp_RtoMin;
        int             tcp_RtoMax;
        int             tcp_MaxConn;
        unsigned long   tcp_ActiveOpens;
        unsigned long   tcp_PassiveOpens;
        unsigned long   tcp_AttemptFails;
        unsigned long   tcp_EstabResets;
        int             tcp_CurrEstab;
        unsigned long   tcp_InSegs;
        unsigned long   tcp_OutSegs;
        unsigned long   tcp_RetransSegs;
        } TCP_STATS_T;

extern  TCP_STATS_T     tcp_stats;

typedef struct  UDP_STATS_S
        {
        unsigned long   udp_InDatagrams;
        unsigned long   udp_NoPorts;
        unsigned long   udp_InErrors;
        unsigned long   udp_OutDatagrams;
        } UDP_STATS_T;

extern  UDP_STATS_T     udp_stats;

extern  long    test_int0;
extern  long    test_int1p;
extern  long    test_int2p;
extern  long    test_int3p;
extern  long    test_int4p;
extern  long    test_int1n;
extern  long    test_int2n;
extern  long    test_int3n;
extern  long    test_int4n;

extern  unsigned long   test_count0;
extern  unsigned long   test_count1;
extern  unsigned long   test_count2;
extern  unsigned long   test_count3;
extern  unsigned long   test_count4;
extern  unsigned long   test_count5;

extern  unsigned long   test_gauge0;
extern  unsigned long   test_gauge1;
extern  unsigned long   test_gauge2;
extern  unsigned long   test_gauge3;
extern  unsigned long   test_gauge4;
extern  unsigned long   test_gauge5;

extern  unsigned long   test_time0;
extern  unsigned long   test_time1;
extern  unsigned long   test_time2;
extern  unsigned long   test_time3;
extern  unsigned long   test_time4;
extern  unsigned long   test_time5;

/* We only include the following variables if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
extern  UINT_64_T test_bigc0;
extern  UINT_64_T test_bigc1;
extern  UINT_64_T test_bigc2;
extern  UINT_64_T test_bigc3;
extern  UINT_64_T test_bigc4;
extern  UINT_64_T test_bigc51;
extern  UINT_64_T test_bigc52;
extern  UINT_64_T test_bigc6;
extern  UINT_64_T test_bigc7;
extern  UINT_64_T test_bigc8;
extern  UINT_64_T test_bigc9;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */

extern  OCTET_T test_string0[];
extern  OCTET_T test_string1[];

extern  OCTET_T test_ipaddr0[];
extern  OCTET_T test_ipaddr1[];
extern  OCTET_T test_ipaddr2[];

#endif  /* snmpvars_inc */
