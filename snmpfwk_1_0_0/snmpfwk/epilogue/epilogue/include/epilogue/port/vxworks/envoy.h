/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/port/vxworks/envoy.h,v 1.1 2006/06/29 12:17:13 smasyutin Exp $ */

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
 * Revision 1.1  2006/06/29 12:17:13  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:18  denysg
 * no message
 *
 * Revision 1.3  1998/02/25 04:57:09  sra
 * Update copyrights.
 *
 * Revision 1.2  1997/11/04 00:29:37  sar
 * Changed the agentx sub agent timeout period from 10000 (10 seconds)
 * which was used for testing to 1000 (1 second).
 *
 * Revision 1.1  1997/10/23 06:21:30  sar
 * Updates for agentx, this includes some new installation options and
 * renaming the older ones for the older subagent scheme
 *
 * Revision 1.0  1997/08/08 22:11:23  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.5  1997/03/20 06:57:53  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.4  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.3  1997/01/15 22:30:42  sar
 * moved location of nvutils.h to snark/snmptalk
 *
 * Revision 0.2  1996/10/28  02:44:53  sra
 * Fix missing comment marker.
 *
 * Revision 0.1  1996/10/23  00:14:56  sar
 * *** empty log message ***
 *
 */

/* previous revision history when this was envoy/port/sunos/envoy.h
 * with rev and copyright updates removed
 *
 * Revision 6.5  1996/02/27  00:03:03  sar
 * Updated partytime to be usectime
 *
 * Revision 6.4  1995/11/14  22:37:59  sar
 * If proxy is installed release_private is proxy_release_private
 *
 * Revision 6.3  1995/11/10  23:59:08  sar
 * stopped using the pre & post set user exits for the nvview stuff
 *
 * Revision 6.2  1995/11/06  20:05:22  sar
 * Removed validate_community's dependency on install v1
 * removed validate_address & validate_vesion
 *
 * Revision 6.1  1995/10/20  23:15:55  sar
 * removed party, context and acl macros
 *
 * Revision 5.6  1995/05/22  19:39:24  sar
 * Modified the lock code to add some new macros.
 *
 * Revision 5.5  1995/05/02  23:39:31  sar
 * Added the envoy timer macros.
 *
 * Revision 5.4  1995/05/01  21:58:15  sar
 * Put the example lock macros in an ifdef block
 *
 * Revision 5.3  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PREAMBLE with #ifdef, not #if.
 *
 * Revision 5.2  1994/11/01  17:59:04  sar
 * *_add_mod routines now take a flag as well as an data block.
 *
 * Revision 5.1  1994/09/29  20:39:39  sar
 * added macros for a simple locking scheme to test the compiliation of
 * our code
 * Modified the macros for use with the non volatile storage stuff to
 * use the new (as of fall 94) scheme.
 *
 * Revision 4.3  1994/05/12  17:24:05  sar
 * moved the include for nvutils.h to make it available for everybody
 * that needs it.
 *
 * Revision 4.2  1994/05/10  20:21:45  sar
 * moved view defines aroudn so they are available in other than v2 settings.
 *
 * Revision 4.1  1994/05/09  21:06:41  sar
 * Added define for SNMP_RESETSYSUPTIME to be nothing.
 *
 */


#ifdef CONFIG_PREAMBLE

/*****************************************************************
 *
 * System Includes:
 *
 * Envoy uses memcpy(), memset(), and strlen().  The appropriate
 * include file should be put here to see that these routines are
 * properly declared.  If no such include file exists on the system
 * then declare memcpy() and memset() here directly.  Envoy also
 * uses min() and max().  Include the system's definition or use the
 * macros supplied below.
 *****************************************************************/

#include <memLib.h>
/* #include <string.h> */

/*****************************************************************
 * Memory allocation routines.  Envoy needs to allocate memory to
 * decode the packet into and also to store the response data for GET's
 * and GET-NEXT's.
 *****************************************************************/

/*----------------------------------------------------------------
 * Make sure that the routines used by the SNMP_memory_alloc() and
 * SNMP_memory_free() macros are declared.  If system memory routines
 * are used (e.g. malloc() and free()) then include the appropriate
 * system header file here.
 ----------------------------------------------------------------*/

/* get malloc and free declarations */
#include <stdlib.h>

/* The memory allocation macros */
#define SNMP_memory_alloc(need) malloc(need)
#define SNMP_memory_free(buf)   free(buf)

/* Define the maximum packet size this implementation will accept.      */
/* There is no hard upper limit.  SNMP_MAX_PACKET_SIZE should not be    */
/* reduced below the value of SNMP_MIN_PACKET_SIZE.                     */
#define SNMP_MAX_PACKET_SIZE            5000 /* shai 2/2003 */
#define SNMP_MIN_PACKET_SIZE            484 /* shouldn't change */

/* Say what is the maximum number of components of an object identifer  */
/* which we can handle, including those in the instance.                */
#define MAX_OID_COUNT                   32

#if (INSTALL_ENVOY_SNMP_LOCK)
typedef struct ENVOY_LOCK_S
        {
        int readers;
        int writers;
        } ENVOY_LOCK_T;
#endif

#else

/*****************************************************************
 *
 * The following macro definitions allow access to the user configurable
 * part of the SNMP code.
 *
 *****************************************************************/

/*****************************************************************
 * Include any declarations needed for the routines specified below
 * by the macros.  Don't put <snmp.h> here because this file is
 * included by that one.
 *****************************************************************/

extern int validate_SNMP_community __((SNMP_PKT_T *, SNMPADDR_T *,
                                       SNMPADDR_T *));
/*----------------------------------------------------------------
 SNMP_validate_community -- Check an operation against the community name.

Parameters:
        SNMP_PKT_T *pkt         The received packet (decoded format)
        SNMPADDR_T *src         Source of the packet
        SNMPADDR_T *dst         Destination of the packet (most likely
                                the address of the machine on which this
                                code is running.)

Returns:
        This procedure should return 0 if happy with the community and
        1 if not.

        This routine should fill in the packet structure with the mib
        view mask associated with the community string.
        This routine may hang additional data onto the "private" field of
        the packet structure.  The user will be given the opportinity to
        release that memory via SNMP_release_private().

 ----------------------------------------------------------------*/
#define SNMP_validate_community(pkt, src, dst) \
  validate_SNMP_community(pkt, src, dst)


#if defined(INSTALL_ENVOY_SNMP_VERSION_USEC)

/* The time routine, this must be defined */
typedef long USECTIME_T;
extern time_t	   time (time_t *_tod);
#define SNMP_USECTIME (USECTIME_T)time((long *)0)
#endif /* INSTALL_ENVOY_SNMP_VERSION_USEC */


#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
#include <snark/snmptalk/nvutils.h>

#define SNMP_VIEW_CREATE_TEST(PKT, NEW)         SNMP_NV_View_Add_Mod(NEW, 0)
#define SNMP_VIEW_UPDATE_TEST(PKT, OLD, NEW)    SNMP_NV_View_Add_Mod(NEW, 0)
#define SNMP_VIEW_DESTROY_TEST(PKT, OLD)        SNMP_NV_View_Add_Mod(OLD, 1)
#define SNMP_VIEW_BACKOUT_HOOK(NEW)             SNMP_NV_Clean()
#define SNMP_VIEW_CREATE_UNDO(NEW)              (SNMP_NV_Clean(), 0)
#define SNMP_VIEW_UPDATE_UNDO(OLD, NEW)         (SNMP_NV_Clean(), 0)
#define SNMP_VIEW_DESTROY_UNDO(OLD)             (SNMP_NV_Clean(), 0)
#define SNMP_VIEW_DESTROY_BACKOUT(OLD)          SNMP_NV_Clean()
#define SNMP_VIEW_FINISHED                      SNMP_NV_Doit()

#endif /* version2, master agent, v2 views */

#if (INSTALL_ENVOY_EPI_MASTER)
#define SNMP_RESETSYSUPTIME
#endif

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

/*----------------------------------------------------------------
 SNMP_release_private -- Get rid of a any private data attached to the packet
                    structure.  May be left undefined in which case nothing
                    is done with the private field when the SNMP packet
                    structure is freed.

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/
/* #define SNMP_release_private(pkt) release_private(pkt) */

#if (INSTALL_ENVOY_SNMP_PROXY)
extern void proxy_release_private __((SNMP_PKT_T *));
#define SNMP_release_private(pkt) proxy_release_private(pkt)
#endif /* (INSTALL_ENVOY_SNMP_PROXY) */

/*----------------------------------------------------------------
 SNMP_user_get_encode_buffer -- Allows the user the opportunity to get the
                        buffer that the SNMP packet will be encoded into.
                        If not used then leave undefined.

 Parameters:
        SNMP_PKT_T      *pkt    The packet being encoded.
        unsigned int    need    The amount of storage needed.
        EBUFFER_T       *ebuffp The ebuffer to fill in with the actual
                                buffer.

 Returns:
        0   successful
        1   not successful
 ----------------------------------------------------------------*/
/* #define SNMP_user_get_encode_buffer(pkt, need, ebuffp) 0 */


/*----------------------------------------------------------------
 SNMP_trace_input
 SNMP_trace_output
                -- for implementing packet tracing through the
                   SNMP code.  May not be left undefined but may
                   instead be defined as nothing.

 Parameters:
        SNMP_PKT_T *pkt         The packet being traced

 Returns: Nothing
 ----------------------------------------------------------------*/
#define SNMP_TRACE_INPUT(pkt)
#define SNMP_TRACE_OUTPUT(pkt)

/*****************************************************************
 * The next series of macros are hooks into the processing of SET PDUs.
 * For most uses you should be able to leave these undefined.
 *****************************************************************/

/*----------------------------------------------------------------
 SNMP_validate_set_pdu -- Perform a global validation of a SET PDU.

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns:
         -1 If the PDU is bad and should be rejected with a GEN_ERR.
          0 If the PDU is good and normal handling should proceed.
         +1 If the PDU is good and this routine has performed all of
            the set operations internally.
 ----------------------------------------------------------------*/
/* #define SNMP_validate_set_pdu(pkt) validate_set_pdu(pkt) */

/*----------------------------------------------------------------
 SNMP_user_pre_set -- Perform a global validation of a SET PDU after all of the
                 test procedures have been called and given the "go ahead".

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns:
         -1 If the PDU is bad and should be rejected with a GEN_ERR.
          0 If the PDU is good and normal handling should proceed.
         +1 If the PDU is good and this routine has performed all of
            the set operations internally.
 ----------------------------------------------------------------*/
/* #define SNMP_user_pre_set(pkt) user_pre_set(pkt) */

/*----------------------------------------------------------------
 SNMP_user_post_set -- Perform any final activities after all of the set
                  procedures have been called.

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/
/* #define SNMP_user_post_set(pkt) user_post_set(pkt) */

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
#define ENVOY_AX_SA_TIMEOUT 10000
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

extern void     envoy_call_timer __((bits32_t, void (*)(void)));
#define ENVOY_CALL_TIMER(T, H)       envoy_call_timer(T, H)

#if (INSTALL_ATTACHE)
extern  bits32_t    glue_now   __((void));
#define ENVOY_NOW() glue_now()
#else
extern  bits32_t    envoy_now  __((void));
#define ENVOY_NOW() envoy_now()
#endif /* (INSTALL_ATTACHE) */

#endif /* CONFIG_PREAMBLE */
