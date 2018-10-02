/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/bug.h,v 1.1 2006/06/29 12:17:05 smasyutin Exp $ */

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
 * $Log: bug.h,v $
 * Revision 1.1  2006/06/29 12:17:05  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/28 15:00:12  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:11  alext
 * no message
 *
 * Revision 3.13  1998/02/25 04:41:40  sra
 * Update copyrights.
 *
 * Revision 3.12  1998/02/05 22:30:18  josh
 * Top-level include files have been moved into directories under common,
 * in order to avoid filename conflicts
 *
 * Revision 3.11  1997/05/30 16:58:59  lowell
 * spelling nit that's been bothering me for months
 *
 * Revision 3.10  1997/03/20 06:58:17  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 3.9  1997/03/19 04:11:35  sra
 * BUG() now common to all products, so move the associated .h files.
 *
 * Revision 3.8  1996/03/22 10:04:12  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 3.7  1996/03/14  20:16:39  sra
 * Get rid of default definition of BUG_OUT, I had it right the first
 * time....
 *
 * Revision 3.6  1996/03/11  20:07:33  sra
 * Add default definition of BUG_OUT to avoid preprocessor problems.
 *
 * Revision 3.5  1995/11/24  23:21:04  sra
 * Add BUG_OUT token to BUG() macros.
 *
 * Revision 3.4  1995/06/28  19:43:16  sra
 * Add cookie argument to BUG() macro.
 *
 * Revision 3.3  1995/06/27  06:20:23  sra
 * Add some debugging code to catch botched packet buffer allocs/frees.
 *
 * Revision 3.2  1995/06/26  21:50:52  sra
 * Soup up a few of the macros.  Change the include-protection macro name
 * to ATTACHE_BUG_H (this file never had anything to do with a certain
 * DOS network software package, but my fingers slipped and the old macro
 * name might have confused people...).
 *
 * Revision 3.1  1995/06/26  05:12:25  sra
 * Invert test in BUG_ASSERT().
 *
 * Revision 3.0  1995/05/10  22:36:18  sra
 * Release 3.0.
 *
 * Revision 2.2  1995/04/26  01:05:13  sra
 * Move bug definitions themselves to separate file bugdef.h.
 *
 * Revision 2.1  1995/04/10  23:40:30  sra
 * Initial revision.
 *
 */

#include <common/h/bug.h>
