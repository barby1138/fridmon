/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/etimer.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: etimer.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:25  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:10  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:42  sra
 * Release 6.0.
 *
 * Revision 1.1  1995/03/20  23:07:47  sar
 * Initial revision
 *
 *
 */

#if (!defined(etimer_inc))
#define etimer_inc

#if (!defined snmp_inc)
#include <envoy/h/snmp.h>
#endif

/* timer structure. Timer memory allocation is expected to be managed by
 * code which calls into the timer system.
 *
 * Software using timers must call tm_init() on any timer before calling
 * any other tm_ functions on it.
 */

typedef struct ENVOY_TIMER_S {
        struct ENVOY_TIMER_S *link;     /* list link */
        bits32_t            expire;     /* relative expiration time in ms */
        void (*handler) __((struct ENVOY_TIMER_S *, void *));
                                        /* expiration handler */
        void               *cookie;     /* handler cookie */
        } ENVOY_TIMER_T;

/* timer calls
 */

void envoy_tm_init      __((ENVOY_TIMER_T *));          /* init timer */
int  envoy_tm_set       __((ENVOY_TIMER_T *, bits32_t));/* set timer */
int  envoy_tm_cancel    __((ENVOY_TIMER_T *));          /* cancel timer */

#endif /* etimer_inc */
