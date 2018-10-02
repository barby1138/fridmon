/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/port/bsd44/envoy.h,v 1.1 2006/06/29 12:17:11 smasyutin Exp $ */

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
 * $Log: envoy.h,v $
 * Revision 1.1  2006/06/29 12:17:11  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:14  denysg
 * no message
 *
 * Revision 1.4  1998/02/25 04:56:47  sra
 * Update copyrights.
 *
 * Revision 1.3  1997/11/04 00:29:35  sar
 * Changed the agentx sub agent timeout period from 10000 (10 seconds)
 * which was used for testing to 1000 (1 second).
 *
 * Revision 1.2  1997/10/22 03:01:21  sar
 * Changed ENVOY_MASTER_AGENT to ENVOY_EPI_MASTER for clarity,
 *
 * Started using ENVOY_AGENTX_MASTER & ENVOY_AGENTX_SUB for better
 * granularity
 *
 * Revision 1.1  1997/10/21 03:00:28  sar
 * Added agentx macro defintions
 *
 * Revision 1.0  1997/08/08 22:11:12  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.5  1997/07/24 21:15:13  alan
 * Decorum now has: DECORUM_MAX_PACKET_SIZE, DECORUM_DEFAULT_RECEIVE_BUFLEN
 *
 * Revision 0.4  1997/03/20 06:57:59  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/03/18 23:30:10  sar
 * Made use of glue_now or envoy_now depend on Attache being
 * installed.
 *
 * Revision 0.2  1997/02/25  10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/23 00:18:21  sar
 * *** empty log message ***
 *
 */

/* previous revision history when this was netbsd/envoy.h
 *
 * Revision 2.6  1996/10/16  00:10:00  sar
 * Corrected proxy_release_private to use a pointer.
 *
 * Revision 2.5  1996/10/15  20:13:59  sar
 * Added declaration for proxy release
 *
 * Revision 2.4  1996/10/15  17:37:53  sar
 * Added proxy release and timer routine definitions.
 *
 * Revision 2.3  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.2  1996/02/27  00:04:18  sar
 * removed validate_address, made v2 use validate_community and
 * updated partytime to be usectime.
 *
 * Revision 2.1  1995/05/23  23:22:33  sar
 * Added some more lock macros.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.10  1995/05/03  01:00:11  sar
 * Added default envoy timer macros.
 *
 * Revision 1.9  1995/05/01  21:27:13  sar
 * Put ifdef statements around the example lock macros
 *
 * Revision 1.8  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PREAMBLE with #ifdef, not #if.
 *
 * Revision 1.7  1995/04/26  08:03:12  sra
 * Back out to pre-NVutil version until the NVutil changes
 * compile correctly in this configuration.  Sorry, SAR.
 *
 * Revision 1.6  1995/03/24  17:01:35  sar
 * Updated to have definitions for the nvutils code and the timer code.
 *
 * Revision 1.5  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.4  1994/10/20  19:21:21  sar
 * Added resetsysuptime and a simple lock scheme to allow the lock code
 * to be compiled.
 *
 * Revision 1.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.2  1993/08/03  22:03:39  sra
 * Don't use defined() to test install options.
 *
 * Revision 1.1  1993/07/05  21:54:24  sra
 * Initial revision
 *
 */

#ifdef CONFIG_PREAMBLE

#include <stdlib.h>
#include <string.h>

#define SNMP_memory_alloc(x)            malloc(x)
#define SNMP_memory_free(x)             free(x)

#if (INSTALL_ENVOY_SNMP_LOCK)
typedef struct ENVOY_LOCK_S
        {
        int readers;
        int writers;
        } ENVOY_LOCK_T;
#endif

#else

typedef long USECTIME_T;
#define SNMP_USECTIME                   ((USECTIME_T) time((long *) 0))
extern long time();

/* This first should be the same as DECORUM_MAX_PACKET_SIZE in decorum.h */
#define SNMP_MAX_PACKET_SIZE            1400
#define SNMP_MIN_PACKET_SIZE            484

#define MAX_OID_COUNT                   32

#define validate_SNMP_version(x)                ((x) == SNMP_VERSION_1)

extern int validate_SNMP_community
  (SNMP_PKT_T *, SNMPADDR_T *, SNMPADDR_T *);

#define SNMP_validate_community(x, y, z)        validate_SNMP_community(x,y,z)

#if (INSTALL_ENVOY_SNMP_PROXY)
extern void proxy_release_private (SNMP_PKT_T *);

#define SNMP_release_private(x)   proxy_release_private(x)
#endif

#if 0
#define SNMP_release_private(x)                         release_private(x)
#define SNMP_user_get_encode_buffer(x, y, z)            0
#define SNMP_validate_set_pdu(x)                        validate_set_pdu(x)
#define SNMP_user_pre_set(x)                            user_pre_set(x)
#define SNMP_user_post_set(x)                           user_post_set(x)
#endif

#if (INSTALL_ENVOY_EPI_MASTER)
#define SNMP_RESETSYSUPTIME
#endif

#define SNMP_TRACE_INPUT(x)
#define SNMP_TRACE_OUTPUT(x)

/* here are the routines that agentx requires */
#if INSTALL_ENVOY_AGENTX_MASTER

/* first we do the master agentx macros */
extern bits32_t envoy_get_sysuptime();
extern sbits32_t envoy_ax_transport_type(ptr_t);
extern int       envoy_ax_transport_string(ptr_t, ALENGTH_T *,
                                           bits8_t **, int *);

#define ENVOY_GET_SYSUPTIME(CON) envoy_get_sysuptime()
#define ENVOY_AX_TRANSPORT_TYPE(COOKIE)   envoy_ax_transport_type(COOKIE)
#define ENVOY_AX_TRANSPORT_STRING(COOKIE, NEED, BUF, DYN) \
        envoy_ax_transport_string(COOKIE, NEED, BUF, DYN)
#define ENVOY_AX_TRANSPORTS_SUPPORTED    {0}

/* Default values are fine for us */
/*
#define ENVOY_AX_DEFAULT_TIMEOUT 1

#define ENVOY_AX_FIND_MIB(CONTEXT) (EBufferUsed(CONTEXT) ? 0 : &mib_root_node)

#define ENVOY_AX_MA_AC_ADD(PKTP, MIBROOT)    GEN_ERR
#define ENVOY_AX_MA_AC_REMOVE(PKTP, MIBROOT) GEN_ERR
#define ENVOY_AX_MA_AC_CLEAN(SESS_ID)
*/
#endif /* #if INSTALL_ENVOY_AGENTX_MASTER */ 

#if INSTALL_ENVOY_AGENTX_SUB
/* then we do the subagent agentx macros */
/* default values are fine for us */
/*
#define ENVOY_AX_SA_MIB_ROOT_NODE(CKE, PKT) 0
#define ENVOY_AX_SA_TIMEOUT 1000
*/

#endif /* #if INSTALL_ENVOY_AGENTX_SUB */ 

#if (INSTALL_ENVOY_SNMP_LOCK)
/* Here's a simple example to test the locking code, a lock is 2 uints
   one for reading, one for writing.  When a routine gets a lock the
   value is incremented by 1, when a routine frees a lock it is decremented
   by 1.  We are allowed multiple readers and 1 writer.  This code
   won't block so if a lock can't be an error is returned */

#define ENVOY_SNMP_LOCK(name)                   ENVOY_LOCK_T name = {0, 0}
#define ENVOY_SNMP_LOCK_EXTERN(name)    extern  ENVOY_LOCK_T name

#define ENVOY_SNMP_INIT_LOCK(name)   ((name).readers = 0, (name).writers = 0)

#define ENVOY_SNMP_GET_READ_LOCK(name) \
        (((name).writers) ? 1 : ((name).readers++, 0))
#define ENVOY_SNMP_RELEASE_READ_LOCK(name)  ((name).readers--)

#define ENVOY_SNMP_GET_WRITE_LOCK(name) \
        (((name).readers || (name).writers) ? 1 : ((name).writers++, 0))
#define ENVOY_SNMP_RELEASE_WRITE_LOCK(name) ((name).writers--)

#endif /* (INSTALL_ENVOY_SNMP_LOCK) */

/* These connect the Envoy timer routines to the system.
   If attache is installed we use its routines and merge all of the
   envoy timers into a single timeer in the attache timer queue.
   If attache isn't installed we expect envoy_now() to be defined
   to connect to the system directly. */

extern void envoy_call_timer   __((bits32_t, void(*)(void)));
#define ENVOY_CALL_TIMER(T, W) envoy_call_timer(T, W)

#if (INSTALL_ATTACHE)
extern  bits32_t    glue_now __((void));
#define ENVOY_NOW() glue_now()
#else
extern  bits32_t    envoy_now __((void));
#define ENVOY_NOW() envoy_now()
#endif /* (INSTALL_ATTACHE) */

#endif /* CONFIG_PREAMBLE */
