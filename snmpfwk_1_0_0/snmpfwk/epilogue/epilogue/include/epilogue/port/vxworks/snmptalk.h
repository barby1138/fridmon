/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/port/vxworks/snmptalk.h,v 1.1 2006/06/29 12:17:15 smasyutin Exp $ */

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
 * $Log: snmptalk.h,v $
 * Revision 1.1  2006/06/29 12:17:15  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:18  denysg
 * no message
 *
 * Revision 1.1  1998/02/25 04:57:11  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:24  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.5  1997/05/17 23:08:54  sra
 * Clean up some SunOS/gcc warnings.
 *
 * Revision 0.4  1997/03/20 06:57:55  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.2  1996/10/30 22:10:02  sar
 * removed bsd_release
 *
 * Revision 0.1  1996/10/23  00:14:56  sar
 * *** empty log message ***
 *
 */


#include <stdlib.h>
#include <strings.h>

/* defines for nvutils for file manipulation */
#define SNMP_READONLY "r"
#define SNMP_READPLUS "r+"
#define SNMP_WRITEPLUS "w+"
#define SNMP_REMOVE_FILE(fname) unlink(fname)

/* SunOS doesn't have these routines so we provide them. */
/* #define NEEDS_STRTOUL */
