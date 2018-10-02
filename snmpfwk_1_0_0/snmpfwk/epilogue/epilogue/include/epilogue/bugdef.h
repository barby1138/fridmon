/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/bugdef.h,v 1.1 2006/06/29 12:17:05 smasyutin Exp $ */

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
*****************************************************************************/

/*
 * $Log: bugdef.h,v $
 * Revision 1.1  2006/06/29 12:17:05  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/28 15:00:12  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:11  alext
 * no message
 *
 * Revision 3.28  1998/02/25 04:41:41  sra
 * Update copyrights.
 *
 * Revision 3.27  1998/02/05 22:30:18  josh
 * Top-level include files have been moved into directories under common,
 * in order to avoid filename conflicts
 *
 * Revision 3.26  1997/11/20 17:33:15  sra
 * Clear out a bunch of unused PAP and CHAP bug codes.
 *
 * Revision 3.25  1997/11/19 21:14:42  josh
 * Liaison bug definitions
 *
 * Revision 3.24  1997/11/18 03:18:24  sra
 * Brand new PAP and CHAP code for PPP.
 *
 * Revision 3.23  1997/10/28 00:51:48  sra
 * Convert PPP code to use BUG() macro.
 * Bang on CHAP and PAP code some more.
 * Create test driver for PPP under Snark/BSD.
 *
 * Revision 3.22  1997/10/15 19:49:36  sra
 * Preliminary cleanup of PPP PAP and CHAP modules.
 *
 * Revision 3.21  1997/08/15 22:14:57  lowell
 * PAP/CHAP polishing
 *
 * Revision 3.20  1997/08/14 16:04:33  lowell
 * boilerplate
 *
 * Revision 3.19  1997/07/29 00:18:41  lowell
 * PAP/CHAP messages for logging already in file
 *
 * Revision 3.18  1997/07/17 02:15:56  alan
 * New errors for a new Decorum
 *
 * Revision 3.17  1997/06/17 19:00:20  lowell
 * more extensive TFTP debugging
 *
 * Revision 3.16  1997/05/30 17:00:18  lowell
 * TFTP (got rid of old TFTP_DEBUG)
 *
 * Revision 3.15  1997/05/23 04:13:32  sra
 * Add BUG_IPADDR_TEST_UNKNOWN_CODE.
 *
 * Revision 3.14  1997/05/22 03:35:25  sra
 * Get rid of struct udp_hdr.  Change pkt_rcv_info into an array, convert
 * receive path to use it instead of passing pointer arguments in upcalls.
 *
 * Revision 3.13  1997/05/17 07:39:02  sra
 * Change IP protocol dispatch to use ip_listener table.
 *
 * Revision 3.12  1997/03/20 06:58:16  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 3.11  1997/03/19 04:11:35  sra
 * BUG() now common to all products, so move the associated .h files.
 *
 * Revision 3.10  1997/02/24 14:43:18  josh
 * new bug definitions for Courier RIP and Attache Plus TCP
 *
 * Revision 3.9  1996/10/28 18:19:01  lowell
 * Decorum messages
 * Looks like this needs to be freed of its ownership by Attache...
 *
 * Revision 3.8  1996/03/22  10:04:12  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 3.7  1996/03/14  17:51:55  lowell
 * DHCP packet dumper code
 *
 * Revision 3.6  1995/12/02  09:15:04  sra
 * Add a bunch of Courier stuff.
 *
 * Revision 3.5  1995/09/28  23:06:59  sra
 * Fix Courier identifiers and copyrights for release packaging.
 *
 * Revision 3.4  1995/06/30  23:24:10  sra
 * Back out hairy interface to RCI TRACE() to a simpler scheme.
 *
 * Revision 3.3  1995/06/30  22:44:00  sra
 * Add RCI trace bug codes.
 *
 * Revision 3.2  1995/06/30  16:10:40  lowell
 * first few ARP and DHCP bug definitions (more to come)
 *
 * Revision 3.1  1995/06/27  06:20:23  sra
 * Add some debugging code to catch botched packet buffer allocs/frees.
 *
 * Revision 3.0  1995/05/10  22:36:18  sra
 * Release 3.0.
 *
 * Revision 1.1  1995/04/25  21:39:43  sra
 * Initial revision
 *
 */

#include <common/h/bugdef.h>

