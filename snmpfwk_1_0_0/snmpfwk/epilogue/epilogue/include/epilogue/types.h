/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/types.h,v 1.1 2006/06/29 12:17:05 smasyutin Exp $ */

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
 * $Log: types.h,v $
 * Revision 1.1  2006/06/29 12:17:05  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/28 15:00:12  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:11  alext
 * no message
 *
 * Revision 1.11  1998/02/25 04:41:43  sra
 * Update copyrights.
 *
 * Revision 1.10  1998/02/05 22:30:20  josh
 * Top-level include files have been moved into directories under common,
 * in order to avoid filename conflicts
 *
 * Revision 1.9  1997/04/20 01:32:00  alan
 * Add ldb_int{8,16,32}_t types for the benefit of layout/ldbglue.h.
 *
 * Revision 1.8  1997/03/20 06:58:18  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.7  1997/02/25 10:57:03  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 1.6  1996/10/28 22:52:57  sar
 * correction added epilogue_size_t and epilogue_char_t
 *
 * Revision 1.5  1996/10/28  22:51:08  sar
 * Added epilogue_types_h epilogue_char_h
 *
 * Revision 1.4  1993/04/21  15:39:23  dab
 * No longer have cookie_t and renamed cardinal_t to fastint_t.
 *
 * Revision 1.3  1993/03/20  19:09:52  dab
 * changed pvoid_t to ptr_t
 *
 * Revision 1.2  1993/02/26  16:54:21  dab
 * Changed htypes.h to porttype.h
 *
 * Revision 1.1  1993/02/19  22:28:01  dab
 * Initial revision
 *
 */

#include <common/h/types.h>
