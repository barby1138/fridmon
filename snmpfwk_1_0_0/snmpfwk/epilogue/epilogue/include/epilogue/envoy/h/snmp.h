/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/snmp.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: snmp.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:27  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/10/22 03:07:30  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.4  1997/10/16 00:25:18  sar
 * Primarily this set of modifications is for the agentx protocol.
 * Most of the work was in the agentx.h and ax_mth.h files.  Most
 * other files (snmpdefs.h, vbdefs.h and mib.h) received updates
 * to their structures and snmp got some new lock macros.
 *
 * One other change of note was the modification of continue
 * processing.  Previously we called a specific continue routine
 * directly, now we store a pointer to the corret continue
 * function in the snmp packet structure and call that.  We include
 * macros for backwards compatibility.
 *
 * Revision 7.3  1997/08/21 17:23:44  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 7.2  1997/03/20 06:48:16  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:59:45  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/02  23:29:47  sar
 * added some semi-colons in the lock macros.
 *
 * Revision 5.3  1995/04/18  20:41:28  sar
 * Added some new macros and modified the old macros so we don't require
 * ifdefs in all of the mainline code.  Instead we use the new macros
 * which will be defined to nothgin if locks aren't installed.
 *
 * Revision 5.2  1995/03/20  23:11:33  sar
 * Redo the defaults for the lock scheme.  They macros are now
 * defined to look like "good" returns so the main line code does not
 * have to deal with if defs.  Some places that have large blocks of
 * lock code may still choose to use if defs.
 *
 * Revision 5.1  1994/10/12  20:11:47  sar
 * Added an ifdef around the def for the coarse lock
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 1.6  1993/05/28  18:01:05  sar
 * added the define as nothing macros for snmp_trace_input and output.
 *
 * Revision 1.5  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo> where it was correct.
 *
 * Revision 1.4  1993/04/30  22:58:26  sar
 * Added mechanism for minimal proxies and coarse grained locks.
 *
 * Revision 1.3  1993/04/26  20:33:14  sar
 * Added bitstrings, and have deletion of parties or contexts delete acls.
 *
 * Revision 1.2  1993/03/03  20:06:55  dab
 * If the config file doesn't set SNMP_SILLY_MARKETING_MACRO
 * then set it to something that wan't make a mess here.
 *
 * Revision 1.1  1993/02/26  17:08:46  dab
 * Initial revision
 *
 * 
 */

#if (!defined(snmp_inc))
#define snmp_inc

#if (!defined(snmpdefs_inc))
#include <envoy/h/snmpdefs.h>
#endif

#include <common/h/glue.h>
#include <envoy.h>

/* Define some macros as nothing if the user hasn't defined them */
#if !defined(SNMP_TRACE_INPUT)
#define SNMP_TRACE_INPUT(pkt)
#endif

#if !defined(SNMP_TRACE_OUTPUT)
#define SNMP_TRACE_OUTPUT(pkt)
#endif

/* If we are using the lock code specify coarse lock for everybody to use.
   We also specify a larger macro that gets a lock and if it can't
   executes it's other arguments */
#if INSTALL_ENVOY_SNMP_LOCK
ENVOY_SNMP_LOCK_EXTERN(SNMP_CoarseLock);
ENVOY_SNMP_LOCK_EXTERN(SNMP_infrastructure_lock);

#define ENVOY_SNMP_LOCK_AND_1ARGS(get, lock, com1) \
        if (get(lock)) { com1; }
#define ENVOY_SNMP_LOCK_AND_2ARGS(get, lock, com1, com2) \
        if (get(lock)) { com1; com2; }
#define ENVOY_SNMP_LOCK_AND_3ARGS(get, lock, com1, com2, com3) \
        if (get(lock)) { com1; com2; com3; }
#define ENVOY_SNMP_LOCK_AND_4ARGS(get, lock, com1, com2, com3, com4) \
        if (get(lock)) { com1; com2; com3; com4; }
#else /* INSTALL_ENVOY_SNMP_LOCK */
/* Don't use the lock stuff if the user doesn't define it */
#define ENVOY_SNMP_LOCK(name)
#define ENVOY_SNMP_LOCK_EXTERN(name);
#define ENVOY_SNMP_GET_READ_LOCK(name) 0
#define ENVOY_SNMP_RELEASE_READ_LOCK(name)
#define ENVOY_SNMP_GET_WRITE_LOCK(name) 0
#define ENVOY_SNMP_RELEASE_WRITE_LOCK(name)
#define ENVOY_SNMP_LOCK_AND_1ARGS(get, lock, com1)
#define ENVOY_SNMP_LOCK_AND_2ARGS(get, lock, com1, com2)
#define ENVOY_SNMP_LOCK_AND_3ARGS(get, lock, com1, com2, com3)
#define ENVOY_SNMP_LOCK_AND_4ARGS(get, lock, com1, com2, com3, com4)
#endif /* INSTALL_ENVOY_SNMP_LOCK */

#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
#define ENVOY_AX_MA_RELEASE_READ_LOCK(name)  ENVOY_SNMP_RELEASE_READ_LOCK(name)
#define ENVOY_AX_MA_RELEASE_WRITE_LOCK(name) ENVOY_SNMP_RELEASE_WRITE_LOCK(name)
#else
#define ENVOY_AX_MA_RELEASE_READ_LOCK(name)
#define ENVOY_AX_MA_RELEASE_WRITE_LOCK(name)
#endif

#endif /* snmp_inc */
