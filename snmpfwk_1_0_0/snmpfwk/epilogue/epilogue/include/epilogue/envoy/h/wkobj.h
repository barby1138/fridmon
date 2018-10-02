/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/wkobj.h,v 1.1 2006/06/29 12:17:09 smasyutin Exp $ */

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
 * $Log: wkobj.h,v $
 * Revision 1.1  2006/06/29 12:17:09  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:31  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:34  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:28  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:47  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 1.2  1993/05/26  21:30:55  sar
 * updated to work with the objid defined in the rfcs.
 *
 * Revision 1.1  1993/04/23  23:36:14  sar
 * Initial revision
 *
 */

#if (!defined(wkobj_inc))
#define wkobj_inc

#define SNMP_INTERNET           1, 3, 6, 1

#define SNMP_DIRECTORY          SNMP_INTERNET, 1
#define SNMP_MGMT               SNMP_INTERNET, 2
#define SNMP_MIB2               SNMP_MGMT, 1

#define SNMP_EXPERIMENTAL       SNMP_INTERNET, 3
#define SNMP_PRIVATE            SNMP_INTERNET, 4
#define SNMP_ENTERPRISES        SNMP_PRIVATE, 1

#define SNMP_SECURITY           SNMP_INTERNET, 5
#define SNMP_SNMPV2             SNMP_INTERNET, 6

/*#define SNMP_SNMPMAPPINGS     SNMP_SNMPV2, 1*/
/*#define SNMP_SNMPMODULES      SNMP_SNMPV2, 4*/

/*#define SNMP_SNMPDOMAINS      SNMP_SNMPMAPPINGS, 1*/
#define SNMP_SNMPDOMAINS        SNMP_SNMPV2, 1
#define SNMP_UDPDOMAIN          SNMP_SNMPDOMAINS,  1
#define SNMP_CLNSDOMAIN         SNMP_SNMPDOMAINS,  2
#define SNMP_CONSDOMAIN         SNMP_SNMPDOMAINS,  3
#define SNMP_DDPDOMAIN          SNMP_SNMPDOMAINS,  4
#define SNMP_IPXDOMAIN          SNMP_SNMPDOMAINS,  5

/*#define SNMP_SNMPPROXYS               SNMP_SNMPMAPPINGS, 2*/
#define SNMP_SNMPPROXYS         SNMP_SNMPV2, 2
#define SNMP_RFC1157PROXY       SNMP_SNMPPROXYS,   1
#define SNMP_RFC1157DOMAIN      SNMP_RFC1157PROXY, 1
#define SNMP_RFC1157NOAUTH      SNMP_RFC1157PROXY, 2

#define SNMP_SNMPMODULES        SNMP_SNMPV2, 3
#define SNMP_SNMPMIB            SNMP_SNMPMODULES, 1
#define SNMP_SNMPMIBOBJECTS     SNMP_SNMPMIB,     1

#define SNMP_SNMPTRAP           SNMP_SNMPMIBOBJECTS, 4
#define SNMP_SNMPTRAPOID        SNMP_SNMPTRAP,       1
#define SNMP_SNMPTRAPENTERPRISE SNMP_SNMPTRAP,       3

#define SNMP_SNMPTRAPS          SNMP_SNMPMIBOBJECTS, 5
#define SNMP_COLDSTART          SNMP_SNMPTRAPS,      1
#define SNMP_WARMSTART          SNMP_SNMPTRAPS,      2
#define SNMP_LINKDOWN           SNMP_SNMPTRAPS,      3
#define SNMP_LINKUP             SNMP_SNMPTRAPS,      4
#define SNMP_AUTHENTICATIONFAILURE   SNMP_SNMPTRAPS, 5
#define SNMP_EGPNEIGHBORLOSS    SNMP_SNMPTRAPS,      6

#define SNMP_PARTYMIB           SNMP_SNMPMODULES, 3
#define SNMP_PARTYADMIN         SNMP_PARTYMIB,    1

#define SNMP_PARTYPROTOCOLS     SNMP_PARTYADMIN,  1
#define SNMP_NOAUTH             SNMP_PARTYPROTOCOLS, 1
#define SNMP_NOPRIV             SNMP_PARTYPROTOCOLS, 2
#define SNMP_DESPRIV            SNMP_PARTYPROTOCOLS, 3
#define SNMP_MD5AUTH            SNMP_PARTYPROTOCOLS, 4

#define SNMP_TEMPORALDOMAINS    SNMP_PARTYADMIN, 2
#define SNMP_CURRENTTIME        SNMP_TEMPORALDOMAINS, 1
#define SNMP_RESTARTTIME        SNMP_TEMPORALDOMAINS, 2
#define SNMP_CACHETIME          SNMP_TEMPORALDOMAINS, 3

#define SNMP_INITIALPARTYID     SNMP_PARTYADMIN, 3
#define SNMP_INITIALCONTEXTID   SNMP_PARTYADMIN, 4

#endif /* wkobj_inc */
