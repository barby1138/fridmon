/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/common/h/bug.h,v 1.1 2006/06/29 12:17:06 smasyutin Exp $ */

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
 * Revision 1.1  2006/06/29 12:17:06  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:22  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:16  alext
 * no message
 *
 * Revision 1.3  1998/02/25 04:43:23  sra
 * Update copyrights.
 *
 * Revision 1.2  1998/02/05 22:34:41  josh
 * have to make sure bug.h includes common/h/bugdef.h
 *
 * Revision 1.1  1998/02/05 22:19:27  josh
 * moving commonly used include files into common/h from top-level directory
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

#ifndef EPILOGUE_BUG_H
#define EPILOGUE_BUG_H

/*
 * This file defines the framework of the BUG() macro mechanism.
 *
 * This is intended to be an extremely general way of handling code
 * tracing, exception processing, and debugging code, with support
 * both for arbitrary documentation strings and for making the entire
 * business go away if you want to save space in production code.  The
 * format is a little strange, mostly because of the relative
 * weaknesses of C (in particular, of its macro processor).
 *
 * There are some sample BUG() macro definitions at the end of this file.
 *
 * The general form of an invocation of the BUG() macro is
 *
 *   BUG( <bug-code>, <fatal-flag>, <cookie>, ( BUG_OUT, <printf-stuff> ));
 *
 * <bug-code>   (enum bug_codes)   indicates the bug that occurred
 *
 * <fatal-flag> (enum bug_actions) indicates "fatal" errors
 *
 * <cookie>     (void *) is for bug-specific useful data, if any
 *
 * <printf-stuff> is a parenthesised list of arguments suitable
 * for passing to a printf()-like function if you want a debugging log.
 *
 * In general, bug codes should be unique.  The only case where it makes
 * sense to use the same bug code in multiple places is if both the failure
 * and any recovery action are identical among the multiple invocations.
 * The bug codes themselves are defined in a separate file, bugdef.h.
 *
 * "fatal" errors means errors serious enough that Attache's attempts
 * to clean up after the problem may not be sufficent.  Attache will
 * always -try- to continue operation, but you may want to consider
 * just restarting your system if a "fatal" error occurs.
 *
 * The <cookie> is to allow particular BUG() invocations to pass
 * useful information up to your bug handler code.   Most BUG()
 * invocations will probably just pass a zero (null pointer).
 * NB: We make no promises whatsoever about consistancy of data passed
 * in this pointer between releases unless explictly documented in our
 * manuals.  So, in most cases, you should just ignore this field.
 *
 * <printf-stuff> has to be handled by in-line code if it's handled at all,
 * due to limitations of the C macro preprocessor.  It should probably
 * be ignored in a production system, it's just there to ease debugging.
 *
 * BUG_OUT is sort of a macro-level cookie; all invocations of BUG()
 * include this token, you can define it as a macro or an automatic
 * variable or whatever's convenient.  If you're just trying to get
 * basic output to work in a normal C environment, defining BUG_OUT
 * as a macro that expands to "stderr" might be a good choice.
 */
 
/*
 * <fatal-flag> values.
 */
enum bug_actions {
  BUG_FATAL,                    /* serious bug, consider rebooting now */
  BUG_CONTINUABLE               /* normal operation can continue, we think */
};

/*
 * <bug-code> values.  Check these with a switch() or if() statement,
 * don't depend on specific numeric values.
 */

enum bug_codes {

#define BUGDEF(x) x ,
#include <common/h/bugdef.h>
#undef BUGDEF

  BUG_MAX                       /* Number of bugs currently defined */
};

/*
 * I haven't figured out yet whether BUG() should be under a makmak
 * installation option or not.  This will do the right thing for now.
 */
#ifndef INSTALL_BUG
#ifdef BUG
#define INSTALL_BUG 1
#else
#define INSTALL_BUG 0
#endif
#endif

/*
 * Variant on BUG() which includes a test condition.
 */
#ifndef BUG_IF
#if INSTALL_BUG
#define BUG_IF(v,w,x,y,z) \
  do { if (v) BUG(w,x,y,z); } while (0)
#else
#define BUG_IF(v,w,x,y,z)
#endif
#endif

/*
 * Variant on standard C "assert()" mechanism.
 */
#ifndef BUG_ASSERT
#if INSTALL_BUG
#define BUG_ASSERT(x) \
  BUG_IF((!(x)), BUG_ASSERTION_FAILED, BUG_FATAL, 0,\
         (BUG_OUT, "Assertion \"%s\" failed, file %s line %d",\
          #x, __FILE__, __LINE__))
#else
#define BUG_ASSERT(x)
#endif
#endif

/*
 * Wrapper for support code for things like BUG_ASSERT().
 * Useful for declaring variables or adding code that is not itself
 * a BUG() statement but that will only be used when debugging is enabled.
 *
 * Eg:
 *
 *  void foo_init(void)
 *  {
 *    BUG_SUPPORT(static count = 0);
 *    BUG_IF(count++, BUG_HORRIBLE_BOTCH_IN_FOO_INIT, BUG_FATAL, 0,
 *           (BUG_OUT, "Arg, you called foo_init() again!"));
 *    ...
 *  }
 */
#ifndef BUG_SUPPORT
#if INSTALL_BUG
#define BUG_SUPPORT(x)  x
#else
#define BUG_SUPPORT(x)
#endif
#endif

/*
 * Default definition of BUG() if customer doesn't provide one.
 * Does nothing at all.
 */
#ifndef BUG
#define BUG(w,x,y,z)
#endif

/*
 * The rest of this file is a couple of sample definitions of the BUG()
 * macro that might be useful in your environment.
 */
#if 0

/*
 * Sample BUG() that just passes error indication to customer code.
 */
#define BUG(w,x,y,z) \
  glue_bug(w,x)

/*
 * Sample BUG() that logs errors and handles the fatal flag.
 * Logging depends on some POSIX-like features that are available on
 * many but not all systems.
 * Something like this might be useful in a debugging environment.
 *
 * NB: "fprintf z;" is not a typo.
 */
#define BUG_OUT stderr
#define BUG(w,x,y,z) \
  do {\
    char now_[30];\
    time_t now_time_ = time(0);\
    struct tm *now_tm_ = localtime(&now_time_);\
    strftime(now_, sizeof(now_), "%H:%M:%S", now_tm_);\
    fprintf(BUG_OUT, "%s: ", now_);\
    fprintf z ;\
    fputc('\n', BUG_OUT);\
    if ((x) == BUG_FATAL)\
      fprintf(BUG_OUT, "%s: Fatal error, cannot continue\n", now_);\
    fflush(BUG_OUT);\
    if ((x) == BUG_FATAL)\
      abort();\
  } while (0)

#endif /* 0 */

#endif /* EPILOGUE_BUG_H */
