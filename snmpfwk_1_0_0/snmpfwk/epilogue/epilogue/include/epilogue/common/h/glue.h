/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/common/h/glue.h,v 1.1 2006/06/29 12:17:06 smasyutin Exp $ */

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
 *  Copyright 1997 Epilogue Technology Corporation.
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
 * $Log: glue.h,v $
 * Revision 1.1  2006/06/29 12:17:06  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:23  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:16  alext
 * no message
 *
 * Revision 1.2  1998/02/25 04:43:25  sra
 * Update copyrights.
 *
 * Revision 1.1  1997/08/21 17:23:43  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 */

#ifndef COMMON_GLUE_H
#define COMMON_GLUE_H

/*
 * Load the postamble portion of the customer's common.h file.
 */

#include <common.h>

/*
 * Default definitions for macros that stand in for ANSI functions.
 * These may need to be fine-tuned after we've run some compilation tests.
 */

#ifndef MEMCHR
#define MEMCHR(x,y,z)   memchr(x,y,z)
#endif

#ifndef MEMCMP
#define MEMCMP(x,y,z)   memcmp(x,y,z)
#endif

#ifndef MEMCPY
#define MEMCPY(x,y,z)   memcpy(x,y,z)
#endif

#ifndef MEMSET
#define MEMSET(x,y,z)   memset(x,y,z)
#endif

#ifndef STRCMP
#define STRCMP(x,y)     strcmp(x,y)
#endif

#ifndef STRCPY
#define STRCPY(x,y)     strcpy(x,y)
#endif

#ifndef STRLEN
#define STRLEN(x)       strlen(x)
#endif

#ifndef STRNCMP
#define STRNCMP(x,y,z)  strncmp(x,y,z)
#endif

/*
 * These two aren't ANSI functions, but most systems have equivilents.
 * They're case-insensitive versions of the corresponding ANSI functions.
 * We (should) provide implementations of these in the common library.
 */

#ifndef STRICMP
#define STRICMP(x,y)    stricmp(x,y)
#endif

#ifndef STRNICMP
#define STRNICMP(x,y,z) strnicmp(x,y,z)
#endif

#endif /* COMMON_GLUE_H */
