/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/snmpvars.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: snmpvars.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:13  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:49:41  sra
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
 * Remove the zeroing of the snmp_stats variable and added initial values
 * for the variables underlying the test group.
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
 * Revision 1.1  92/03/31  16:43:43  dab
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:14:34
 * Release 2.00
 * 
 *    Rev 1.1   26 Aug 1989 16:50:00
 * Moved ipForwarding into the ip_stats structure.
 * 
 *    Rev 1.0   10 Oct 1988 21:48:48
 * Initial revision.
*/

#include <asn1.h>
#include <snmp.h>
#include <snmpstat.h>
#include <ui64.h>

#include "snmpvars.h"


/**********************************************************************
 *
 * Define the various SNMP management variables used in this system.
 *
 * This file should closely match snmpvars.h
 *
 **********************************************************************/

int             max_tcp_conn = 0;       /* tcpMaxConn           */
int             if_number = 0;          /* ifNumber             */
unsigned long   system_clicks = 0;      /* System time          */

char            snmp_get_request_community[SNMP_COMM_MAX] =
                                "public";

char            snmp_set_request_community[SNMP_COMM_MAX] =
                                "private";

char            snmp_trap_community[SNMP_COMM_MAX] = "SNMP_trap";

char            snmp_auth_traps = 1;

unsigned char   snmp_trap_receiver_ip[4] = {128, 224, 10, 5};

OIDC_T  snmp_product_id[] = {
                           /* ISO */                    1,
                           /* ORG */                    3,
                           /* DOD */                    6,
                           /* INTERNET */               1,
                           /* PRIVATE */                4,
                           /* ENTERPRISES */            1,
                           /* EPILOGUE */               12,
                           /* XXX */                    1,
                           /* YYY */                    1};

int             snmp_product_id_count =
                                sizeof(snmp_product_id)/sizeof(OIDC_T);

OBJ_ID_T        snmp_system_object_id = {
                                sizeof(snmp_product_id)/sizeof(OIDC_T),
                                snmp_product_id};

unsigned char   snmp_local_ip_address[4] = {128, 224, 10, 5};

char            snmp_system_description[] =
                        "Generic SNMP";

IP_STATS_T      ip_stats =
        {
        /* ip_Forwarding        */      0,
        /* ip_DefaultTTL        */      0,
        /* ip_InReceives        */      0,
        /* ip_InHdrErrors       */      0,
        /* ip_InAddrErrors      */      0,
        /* ip_ForwDatagrams     */      0,
        /* ip_InUnknownProtos   */      0,
        /* ip_InDiscards        */      0,
        /* ip_InDelivers        */      0,
        /* ip_OutRequests       */      0,
        /* ip_OutDiscards       */      0,
        /* ip_OutNoRoutes       */      0,
        /* ip_ReasmTimeout      */      0,
        /* ip_ReasmReqds        */      0,
        /* ip_ReasmOKs          */      0,
        /* ip_ReasmFails        */      0,
        /* ip_FragOKs           */      0,
        /* ip_FragFails         */      0,
        /* ip_FragCreates       */      0
        };

ICMP_STATS_T    icmp_stats =
        {
        /* icmp_InMsgs                  */      0,
        /* icmp_InErrors                */      0,
        /* icmp_InDestUnreachs          */      0,
        /* icmp_InTimeExcds             */      0,
        /* icmp_InParmProbs             */      0,
        /* icmp_InSrcQuenchs            */      0,
        /* icmp_InRedirects             */      0,
        /* icmp_InEchos                 */      0,
        /* icmp_InEchoReps              */      0,
        /* icmp_InTimestamps            */      0,
        /* icmp_InTimestampReps         */      0,
        /* icmp_InAddrMasks             */      0,
        /* icmp_InAddrMaskReps          */      0,
        /* icmp_OutMsgs                 */      0,
        /* icmp_OutErrors               */      0,
        /* icmp_OutDestUnreachs         */      0,
        /* icmp_OutTimeExcds            */      0,
        /* icmp_OutParmProbs            */      0,
        /* icmp_OutSrcQuenchs           */      0,
        /* icmp_OutRedirects            */      0,
        /* icmp_OutEchos                */      0,
        /* icmp_OutEchoReps             */      0,
        /* icmp_OutTimestamps           */      0,
        /* icmp_OutTimestampReps        */      0,
        /* icmp_OutAddrMasks            */      0,
        /* icmp_OutAddrMaskReps         */      0
        };

TCP_STATS_T     tcp_stats =
        {
        /* tcp_RtoAlg           */      0,
        /* tcp_RtoMin           */      0,
        /* tcp_RtoMax           */      0,
        /* tcp_MaxConn          */      0,
        /* tcp_ActiveOpens      */      0,
        /* tcp_PassiveOpens     */      0,
        /* tcp_AttemptFails     */      0,
        /* tcp_EstabResets      */      0,
        /* tcp_CurrEstab        */      0,
        /* tcp_InSegs           */      0,
        /* tcp_OutSegs          */      0,
        /* tcp_RetransSegs      */      0
        };

UDP_STATS_T     udp_stats =
        {
        /* udp_InDatagrams      */      0,
        /* udp_NoPorts          */      0,
        /* udp_InErrors         */      0,
        /* udp_OutDatagrams     */      0
        };

#if (INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2)
SNMP_STATS_T    snmp_stats;
#endif

long    test_int0       =         0x0L;
long    test_int1p      =        0x45L;
long    test_int2p      =      0x3456L;
long    test_int3p      =    0x234567L;
long    test_int4p      =  0x12345678L;
long    test_int1n      =       -0x12L;
long    test_int2n      =     -0x789AL;
long    test_int3n      =   -0x6789ABL;
long    test_int4n      = -0x56789ABCL;

unsigned long   test_count0     =        0x0L;
unsigned long   test_count1     =       0x7FL;
unsigned long   test_count2     =     0x7FFFL;
unsigned long   test_count3     =   0x7FFFFFL;
unsigned long   test_count4     = 0x7FFFFFFFL;
unsigned long   test_count5     = 0xFFFFFFFFL;

unsigned long   test_gauge0     =        0x0L;
unsigned long   test_gauge1     =       0x56L;
unsigned long   test_gauge2     =     0x789AL;
unsigned long   test_gauge3     =   0x1BCDEFL;
unsigned long   test_gauge4     = 0x23456789L;
unsigned long   test_gauge5     = 0xABCDEF12L;

unsigned long   test_time0      =        0x0L;
unsigned long   test_time1      =       0x55L;
unsigned long   test_time2      =     0x7ABCL;
unsigned long   test_time3      =   0x345678L;
unsigned long   test_time4      = 0x6F123456L;
unsigned long   test_time5      = 0x81234567L;

/* We only include the following variables if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
UINT_64_T test_bigc0    = {       0x0L,        0x0L};
UINT_64_T test_bigc1    = {       0x0L,       0x12L};
UINT_64_T test_bigc2    = {       0x0L,     0x789AL};
UINT_64_T test_bigc3    = {       0x0L,   0x6789ABL};
UINT_64_T test_bigc4    = {       0x0L, 0x56789ABCL};
UINT_64_T test_bigc51   = {       0x0L, 0xF0000000L};
UINT_64_T test_bigc52   = {      0x45L, 0x67890ABCL};
UINT_64_T test_bigc6    = {    0x3456L, 0x7890ABCDL};
UINT_64_T test_bigc7    = {  0x234567L, 0x890ABCDEL};
UINT_64_T test_bigc8    = {0x12345678L, 0x90ABCDEFL};
UINT_64_T test_bigc9    = {0xFFFFFFFFL, 0xFFFFFFFFL};
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */

OCTET_T         test_string0[] = "";
OCTET_T         test_string1[] = "This is the time";

OCTET_T         test_ipaddr0[] = {0, 0, 0, 0};
OCTET_T         test_ipaddr1[] = {0xFF, 0xFF, 0xFF, 0xFF};
OCTET_T         test_ipaddr2[] = {128, 224, 10, 5};
