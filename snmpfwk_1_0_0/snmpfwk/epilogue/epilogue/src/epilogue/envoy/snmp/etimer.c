/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/etimer.c,v 1.1 2006/06/29 12:17:27 smasyutin Exp $ */

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
 * $Log: etimer.c,v $
 * Revision 1.1  2006/06/29 12:17:27  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:49  sar
 * Updated revision to 8.0
 *
 * Revision 7.4  1997/10/22 03:10:27  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.3  1997/10/16 00:45:51  sar
 * Updated for agentx support, with most of the work in mibutils.c to deal
 * with interior leaves and the pdu processors to deal with agentx leaves,
 * especially their reference counts.
 *
 * Add a bug macro into the get_{read write}_lock calls to give the user
 * a chance if things start going wrong.
 *
 * Fixed a potential problem int the proxy code where we were trying to
 * stuff a 32 bit quantity into a pointer which may or may not work depending
 * on the platform.
 *
 * Revision 7.2  1997/03/20 06:48:55  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:47:23  sra
 * Release 6.0.
 *
 * Revision 1.4  1995/05/02  23:34:50  sar
 * Copied sra's newest code (so basically the file is new).
 *
 * Revision 1.3  1995/04/28  23:13:36  sar
 * Modified one of the loops to use the next field instead of the current
 * entry.
 *
 * Revision 1.2  1995/04/18  21:19:26  sar
 * Use the new lock macros and modfiy the code to set a timer to take into
 * account any time that has elapsed since the first timer was put on the
 * list.
 *
 * Revision 1.1  1995/03/21  00:09:23  sar
 * Initial revision
 *
 *
 */

#include <types.h>
#include <envoy/h/snmp.h>
#include <envoy/h/etimer.h>

#include <bug.h>

/* This code was adapted from the attache timer code for use in the
   Envoy product, the major difference is the addition of if defs to
   use the envoy locking scheme if it is installed */

/* Timers are stored in an ordered linked list. Each timer has a field,
   t->expire, which indicates when it will expire. The expire field is
   set to the number of milliseconds it will expire after its predecessor.
   The first time in the list is set to the number of milliseconds it was
   originally set to.
 
   successive timers that are all set to expire at the same time have
   t->expire set to 0.
  
   The timer code calls ENVOY_CALL_TIMER() to set the one timer provided by the
   OS; it tells the OS to call envoy_timer_exp() within a certain number of
   milliseconds. envoy_timer_exp() processes the first timer on the list, and
   all others after it with t->expire set to 0 until it finds a non-0 timer.
 
   envoy_tm_cancel() attempts to clear a timer and remove it from the list,
   if it fails (can't get the lock) it returns 1, in this case the user
   should not free the timer block.  They may try to cancel the timer
   later and then free the timer block.
 
   we can tell whether a timer is currently in use by checking its
   expire field. This field is NOT_IN_USE when the timer is not in use. */

ENVOY_SNMP_LOCK_EXTERN(SNMP_TimerLock);

static ENVOY_TIMER_T *envoy_timer_list = 0;

static void envoy_timer_exp(void);
static void envoy_timer_update(void);

#define NOT_IN_USE      0xFFFFFFFFL
#define IN_USE(tm)      ((tm)->expire != NOT_IN_USE)

/*
 * Initialize a timer to a known state (ie, mark timer as not in use).
 */
void
  envoy_tm_init (ENVOY_TIMER_T *tm)
{
tm->expire = NOT_IN_USE;
}

/*
 * Bring the expire times of all timers currently in the queue up to date.
 * ALL code that modifies the timer queue MUST call this function FIRST!
 * They must also acquire the write lock for the timer code.
 */
static void
  envoy_timer_update(void)
{
static bits32_t envoy_last_update;
bits32_t now = ENVOY_NOW();
bits32_t elapsed = now - envoy_last_update;
ENVOY_TIMER_T *t = envoy_timer_list;

while (t && elapsed > t->expire) {
    elapsed -= t->expire;
    t->expire = 0;
    t = t->link;
    }

if (t && elapsed > 0)
    t->expire -= elapsed;

envoy_last_update = now;
}

/*
 * This function is called when the OS timer fires.  This code presumes
 * that we will not be called until we should have been, and then,
 * precisely once.  [This may no longer be true, but let's be paranoid....]
 *
 * We have to be able to deal with the timer list changing as we run through
 * it, so we loop bringing the expire times up to date and processing the
 * first timer on the queue if we're reached its expire time.  If there's
 * anything left on the queue when we finish processing expired timers,
 * we schedule the OS timer to wake us up when it's time to resume processing.
 *
 * Processing a timer means dequeuing it and calling its handler.
 */
static void
  envoy_timer_exp(void)
{
ENVOY_TIMER_T *t;
void (*handler)(ENVOY_TIMER_T *, void *);
void *cookie;

while(1) {

#if INSTALL_ENVOY_SNMP_LOCK
    /* We don't have any good options if we can't get the lock.
       So we require the user to not do that, if they do we generate
       a bug report and if they don't handle that we reschedule the
       timer for a short time in the future which is the the least
       bad thing to do. */

    if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_TimerLock)) {
        BUG(BUG_ENVOY_LOCKING, BUG_FATAL, 0,
            (BUG_OUT, "envoy_timer_exp(): timer lock is broken", 0));
        ENVOY_CALL_TIMER(100, envoy_timer_exp);
        return;
        }
#endif

    envoy_timer_update();
    if (envoy_timer_list && envoy_timer_list->expire == 0) {
        t = envoy_timer_list;
        envoy_timer_list = t->link;
        t->expire = NOT_IN_USE;
        handler = t->handler;
        cookie = t->cookie;
        }
    else {
        if (envoy_timer_list)
            ENVOY_CALL_TIMER(envoy_timer_list->expire, envoy_timer_exp);
        ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_TimerLock);
        return;
        }
    ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_TimerLock);
    handler(t, cookie);
    }
}

/*
 * Set a timer to expire "when" milliseconds from now, canceling it first
 * if it was already set.  We won't queue a timer that has no handler.
 *
 * For fairness, if there are other timers already queued that will
 * expire at the same time as this one, we put this one at the end of
 * the group so the others will get to run first.
 *
 * If, when we're done with the insertion, this timer is the new head
 * of the queue, we reset the OS timer accordingly.
 */
int
  envoy_tm_set (ENVOY_TIMER_T *tm, bits32_t when)
{
register ENVOY_TIMER_T **tt;

if (!tm || !tm->handler)
    return(1);
if (IN_USE(tm))
    if (envoy_tm_cancel(tm))
        return(1);

#if INSTALL_ENVOY_SNMP_LOCK
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_TimerLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "envoy_tm_set: timer lock broken", 0));
    return(1);
    }
#endif

envoy_timer_update();

/* Skip down the queue looking for the right place to insert,
   adjusting the relative expire time as we go. */

tm->expire = when;
for (tt = &envoy_timer_list;
     *tt && (*tt)->expire <= tm->expire;
     tt = &(*tt)->link)
    tm->expire -= (*tt)->expire;

/* Insert the timer, and adjust the relative expiration time of
   the timer immediately following it, if there is one.  */
tm->link = *tt;
*tt = tm;
if (tm->link)
    tm->link->expire -= tm->expire;

/* If we inserted at the front of the queue, set the OS timer. */
if (tm == envoy_timer_list)
    ENVOY_CALL_TIMER(envoy_timer_list->expire, envoy_timer_exp);

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_TimerLock);

return(0);
}

/*
 * Cancel a timer.  We have to adjust the relative expiration time
 * of the timer immediately following the one we're canceling.
 * If we're canceling the first timer and the adjusted expiration time
 * of the new head of the queue is still in the future, we reset the OS
 * timer.  We don't need to reset the OS timer if we're not canceling
 * the head of the queue.  We don't want to reset the OS timer if
 * the OS is already running behind schedule, because we expect the OS
 * timer we've already scheduled to expire Real Soon Now.
 */
int
  envoy_tm_cancel (ENVOY_TIMER_T *tm)
{
register ENVOY_TIMER_T **tt;
boolean_t was_first;

if (!tm || !IN_USE(tm))
    return(0);

#if INSTALL_ENVOY_SNMP_LOCK
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_TimerLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "envoy_tm_cancel: timer lock broken", 0));
    return(1);
    }
#endif

tt = &envoy_timer_list;
was_first = (tm == envoy_timer_list);

envoy_timer_update();

if (tm->link)
    tm->link->expire += tm->expire;

while (*tt && *tt != tm)
    tt = &(*tt)->link;
if (*tt)
    *tt = tm->link;

tm->expire = NOT_IN_USE;

if (was_first && envoy_timer_list && envoy_timer_list->expire > 0)
    ENVOY_CALL_TIMER(envoy_timer_list->expire, envoy_timer_exp);

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_TimerLock);

return(0);
}

#ifdef  PRINTF
void
  envoy_tm_print(void)
{
ENVOY_TIMER_T *t;
  
if (ENVOY_SNMP_GET_READ_LOCK(SNMP_TimerLock)) {
    PRINTF("Unable to get read lock for timer list\n");
    return;
    }

PRINTF("Timer List:");
for (t = envoy_timer_list; t; t = t->link)
    PRINTF(" %lx(%lu)", (unsigned long) t, (unsigned long) (t->expire));
PRINTF("\n");

ENVOY_SNMP_RELEASE_READ_LOCK(SNMP_TimerLock);
}
#endif /* PRINTF */

