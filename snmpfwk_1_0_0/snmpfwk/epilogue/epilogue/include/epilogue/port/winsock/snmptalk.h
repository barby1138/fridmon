/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/port/winsock/snmptalk.h,v 1.1 2006/06/29 12:17:20 smasyutin Exp $ */

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
 *  Copyright 1998 Integrated Systems, Inc.
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
 * Configuration for snmptalk on SunOS.
 */

/*
 * $Log: snmptalk.h,v $
 * Revision 1.1  2006/06/29 12:17:20  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:32  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:42  alext
 * no message
 *
 * Revision 1.4  1998/02/25 04:57:13  sra
 * Update copyrights.
 *
 * Revision 1.3  1997/03/20 06:58:11  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 1.1  1996/10/30 17:32:19  mrf
 * Initial revision
 *
 * Revision 1.1  1996/07/02  19:55:38  mrf
 * Initial revision
 *
 * Revision 7.0  1996/03/18  20:25:29  sar
 * Updated rev to 7.0 and copyright ot 96
 *
 * Revision 6.0  1995/05/31  21:51:00  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  16:47:41  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  17:44:26  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 1.1  1993/06/09  19:33:45  dab
 * Initial revision
 *
 */


#include <stdlib.h>
#include <string.h>

/* defines for nvutils for file manipulation */
#define SNMP_READONLY "r"
#define SNMP_READPLUS "r+"
#define SNMP_WRITEPLUS "w+"
#define SNMP_REMOVE_FILE(fname) unlink(fname)

#define BSD_RELEASE 43

#define DESIRED_WINSOCK_VERSION 0x0101
#define MINIMUM_WINSOCK_VERSION 0x0001
#define SOCKET_TIMEOUT          20L     /* time to wait to for selects */
