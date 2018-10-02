/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/proxy.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: proxy.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:54  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/11/01 01:25:36  sar
 * In the packet we are processing save a pointer to the coarse lock we
 * are using so that we can free it later without needing to know which
 * lock it was.
 *
 * Revision 7.4  1997/10/22 03:10:29  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.3  1997/10/16 00:45:58  sar
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
 * Revision 7.2  1997/03/20 06:49:06  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.4  1996/02/27  00:02:24  sar
 * Added test for large repsones to gets and get-nexts.
 *
 * Revision 6.3  1996/01/10  16:34:47  sar
 * proper cast of proxy routines
 *
 * Revision 6.2  1995/11/14  22:44:42  sar
 * Moved the translation routines to auxi/proxy_{v1 v2}
 *
 * Revision 6.1  1995/11/04  01:52:51  sar
 * Basically rebuilt the code, this was necessitated by the
 * demise of parties and the removal of the transport info
 * that parties had carried.
 * The proxy capability was also expanded to cover the v1 case
 *
 * Revision 6.0  1995/05/31  21:47:50  sra
 * Release 6.0.
 *
 * Revision 1.5  1995/05/23  23:23:14  sar
 * Clean up the response packet's ppc oids before overwriting tehm.
 *
 * Revision 1.4  1995/04/28  23:16:40  sar
 * Changed tow of the loops that walk a linked list to use ** to walk
 * down the links.
 *
 * Revision 1.3  1995/04/18  21:13:44  sar
 * Use the new lock macros, added code to determine where to send v2 traps
 * from a v1 soruce we are proxying for.
 *
 * Revision 1.2  1995/03/22  18:41:13  sar
 * We now test that the pcb_root points to something before trying to
 * use it as a pointer.
 * Changed the method for sending packets from io_complete to
 * ENVOY_Send_SNMP_Packet, to take advantage of the size checks.
 *
 * Revision 1.1  1995/03/21  00:09:23  sar
 * Initial revision
 *
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <objectid.h>
#include <buffer.h>
#include <buildpkt.h>
#include <snmpstat.h>
#include <etimer.h>
#include <wkobj.h>

#include <bug.h>

#if INSTALL_ENVOY_SNMP_PROXY
ENVOY_SNMP_LOCK_EXTERN(SNMP_ProxyLock);

typedef struct PROXY_CB_S
        {
        struct PROXY_CB_S       *next;
        UINT_32_T                indx;
        UINT_32_T                reqid;
        EBUFFER_T                community;
        SNMPADDR_T               loc_addr;
        SNMPADDR_T               for_addr;
        IO_COMPLETE_T           *io_comp;
        ERR_COMPLETE_T          *err_comp;
        PTR_T                    async_cookie;
        ALENGTH_T                maxpkt;
        ATVALUE_T                pdu_type;
        INT_32_T                 original_version;
        } PROXY_CB_T;

PROXY_CB_T *pcb_root = 0;
UINT_32_T proxy_id = 0;

void
  Clean_PCB(PROXY_CB_T *pcb)
{
EBufferClean(&pcb->community);
SNMP_memory_free(pcb);
}

/****************************************************************************
NAME:  SNMP_Proxy_Timer

PURPOSE:  Routine to be called by the timer code to clean up the proxy
          list on timeouts.  The cookie is the index into the proxy
          list we look up the index and remove the control block.
          

PARAMETERS:
        struct envoy_timer *    the timer control block
        void               *    the cookie pointer

RETURNS: void
****************************************************************************/

void
  SNMP_Proxy_Timer(ENVOY_TIMER_T        *tm,
                   void                 *cookiep)
{
bits32_t reqid;
PROXY_CB_T *pcb = 0, **pcbp;

reqid = *(bits32_t *)cookiep;

#if INSTALL_ENVOY_SNMP_LOCK
/* We don't have any good options if we can't get the lock.
   So we require the user to not do that, if they do we generate
   a bug report and if they don't handle that we return which is
   the least bad thing to do. */
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_ProxyLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_FATAL, 0,
        (BUG_OUT, "SNMP_Proxy_Timer(): proxy lock is broken", 0));
    return;
    }
#endif

for(pcbp = &pcb_root; *pcbp; pcbp = &(*pcbp)->next)
    if ((*pcbp)->indx == reqid) {
        pcb = *pcbp;
        *pcbp = pcb->next;
        break;
        }

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_ProxyLock);

/* if we found a structure clean it up */
if (pcb) {
    pcb->err_comp(&pcb->for_addr, &pcb->loc_addr, 1, pcb->async_cookie);
    Clean_PCB(pcb);
    }

SNMP_memory_free(tm);

return;
}

/****************************************************************************
NAME:  SNMP_Proxy_Request

PURPOSE:  Provide proxy handling, we save state information and then
          call the routine supplied by the transport structure to
          do the actual proxy handling.

PARAMETERS:
        SNMP_PKT_T *    Packet we're working on

RETURNS: void
****************************************************************************/

void
  SNMP_Proxy_Request(SNMP_PKT_T *pktp)
{
PROXY_CB_T *pcb;
ENVOY_TIMER_T *tm;
UINT_32_T pid, old_reqid, *packetid;
INT_32_T ttl;

/* Size checks.  For sets and bulks we make sure that a response
   packet will be smaller than the current max packet, any size restrictions
   for other types will be handled by the packet sending routines. */

#if (INSTALL_ENVOY_SNMP_VERSION_2)
if ((pktp->pdu_type == GET_BULK_REQUEST_PDU) &&
    (pktp->maxpkt < (25 + EBufferUsed(&pktp->community)))) {
    SGRPv2_INC_COUNTER(snmp_stats.snmpSilentDrops);
    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                         pktp->async_cookie);
    return;
    }

if ((pktp->snmp_version != SNMP_VERSION_1) &&
    (pktp->pdu_type == SET_REQUEST_PDU) &&
    ((SNMP_Bufsize_For_Packet(pktp) + 4) > pktp->maxpkt)) {
    ENVOY_Send_SNMP_Error_Packet(pktp, TOO_BIG, 0);
    return;
    }
#endif

/* Attempt to get the proxy id, we need to lock it (in a reentrent system)
   to make sure the proxy ids are unique */
#if (INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_ProxyLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Proxy_Request: proxy lock broken", 0));
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return;
    }
#endif

pid = proxy_id++;
ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_ProxyLock);

/* save the old reqid for later and install the new reqid,
   then try and send the packet */
old_reqid = pktp->pdu.std_pdu.request_id;
pktp->pdu.std_pdu.request_id = pid;
ttl = ((INT_32_T (*) __((SNMP_PKT_T *)))(pktp->proxy_routine))(pktp);

/* restore the old reqid in case we need to send an error packet */
pktp->pdu.std_pdu.request_id = old_reqid;

/* If the proxy routine failed we send out an error */
if (ttl < 0) {
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return;
    }

/* Acquire the space for the proxy state block and the timer block */
if ((pcb = (PROXY_CB_T *)SNMP_memory_alloc(sizeof(PROXY_CB_T))) == 0) {
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return;
    }
EBufferInitialize(&pcb->community);

tm = (ENVOY_TIMER_T *)SNMP_memory_alloc(sizeof(ENVOY_TIMER_T) +
                                        sizeof(bits32_t));
if (tm == 0) {
    Clean_PCB(pcb);
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return;
    }

/* Move the community string from the packet structure
   to the proxy block structure.  This code resets the
   packet flags to STATIC  because the ownership now
   rests with the proxy block structure. */
EBufferPreLoad(pktp->community.bflags, &pcb->community,
               EBufferStart(&pktp->community),
               EBufferUsed(&pktp->community));
pktp->community.bflags = BFL_IS_STATIC;

pcb->indx = pid;
pcb->reqid = old_reqid;
MEMCPY(&pcb->loc_addr, &pktp->pkt_dst, sizeof(SNMPADDR_T));
MEMCPY(&pcb->for_addr, &pktp->pkt_src, sizeof(SNMPADDR_T));
pcb->io_comp = pktp->io_complete;
pcb->err_comp = pktp->error_complete;
pcb->async_cookie = pktp->async_cookie;
pcb->maxpkt = pktp->maxpkt;
pcb->pdu_type = pktp->pdu_type;
pcb->original_version = pktp->snmp_version;

/* We need to get the proxy lock before trying to install the timer and pcb
   blocks into their proper lists */
#if (INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_ProxyLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Proxy_Request: proxy lock broken", 0));
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    Clean_PCB(pcb);
    SNMP_memory_free(tm);
    return;
    }
#endif

/* attempt to set up a timer to clean up the pcb if we don't get a response
   packet, if we can't set up the timer remove the pcb and send an error
   packet */
envoy_tm_init(tm);
packetid = (bits32_t *)tm + 1;
*packetid = pcb->indx;
tm->cookie = (void *)packetid;
tm->handler = SNMP_Proxy_Timer;
if (envoy_tm_set(tm, ttl)) {
    ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_ProxyLock);
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    Clean_PCB(pcb);
    SNMP_memory_free(tm);
    return;
    }

/* We have succeeded in building all the right structures, attach
   the pcb to the list */
pcb->next = pcb_root;
pcb_root = pcb;

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_ProxyLock);
return;
}

/****************************************************************************
NAME:  SNMP_Proxy_Response

PURPOSE:  Provide proxy handling, attempt to restore the saved state
          and transmit a response packet to the original request.

PARAMETERS:
        SNMP_PKT_T *    Packet we're working on

RETURNS: void
****************************************************************************/

void
  SNMP_Proxy_Response(SNMP_PKT_T *pktp)
{
PROXY_CB_T *pcb = 0, **pcbp;
UINT_32_T reqid;
ALENGTH_T pktsize;

/* Dig out the saved state, and prepare the response packet */
reqid = pktp->pdu.std_pdu.request_id;

#if (INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_ProxyLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Proxy_Response: proxy lock broken", 0));
    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                         pktp->async_cookie);
    return;
    }
#endif

for(pcbp = &pcb_root; *pcbp; pcbp = &(*pcbp)->next)
    if ((*pcbp)->indx == reqid) {
        pcb = *pcbp;
        *pcbp = pcb->next;
        break;
        }

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_ProxyLock);

/* if we didn't find any state drop the packet */
if (pcb == 0) {
    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                         pktp->async_cookie);
    return;
    }

/* Indicate that we are done with the addressing information 
   from the respone packet */
pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 0, pktp->async_cookie);

/* restore the state */
pktp->pdu.std_pdu.request_id = pcb->reqid;
MEMCPY(&pktp->pkt_dst, &pcb->loc_addr, sizeof(SNMPADDR_T));
MEMCPY(&pktp->pkt_src, &pcb->for_addr, sizeof(SNMPADDR_T));
pktp->io_complete = pcb->io_comp;
pktp->error_complete = pcb->err_comp;
pktp->async_cookie = pcb->async_cookie;
pktp->maxpkt = pcb->maxpkt;
pktp->pdu_type = pcb->pdu_type;
pktp->snmp_version = pcb->original_version;

/* Clean up the current community string and then
   replace it with the saved community string */
EBufferClean(&pktp->community);
EBufferPreLoad(pcb->community.bflags, &pktp->community,
               EBufferStart(&pcb->community),
               EBufferUsed(&pcb->community));
EBufferInitialize(&pcb->community);

/* we are done with the pcb so clean it up */
Clean_PCB(pcb);

/* If the current packet doesn't have it we need to get the read lock */
#if (INSTALL_ENVOY_SNMP_LOCK)
if (pktp->lockflags == LOCK_NOLOCK) {
    if (ENVOY_SNMP_GET_READ_LOCK(SNMP_CoarseLock)) {
        BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
            (BUG_OUT,
             "SNMP_Proxy_Response: coarse lock broken",
             0));
        pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst,
                             1, pktp->async_cookie);
        return;
        }
    pktp->lockflags = LOCK_READ;
    pktp->coarse_lock = &SNMP_CoarseLock;
    }
#endif

/* determine if the packet is small enough to be sent out */
pktsize = SNMP_Bufsize_For_Packet(pktp);
if (pktsize > pktp->maxpkt) {
    switch (pktp->pdu_type) {
        case GET_REQUEST_PDU:
        case GET_NEXT_REQUEST_PDU:
            /* set up an alternate response, strip the vbl and
               tag it as too_big */
            Clean_vb_list(&pktp->pdu.std_pdu.std_vbl);
            pktp->pdu.std_pdu.std_vbl.vblist = 0;
            pktp->pdu.std_pdu.std_vbl.vbl_count = 0;
            pktp->pdu.std_pdu.error_status = TOO_BIG;
            pktp->pdu.std_pdu.error_index = 0;
            break;

#if (INSTALL_ENVOY_SNMP_VERSION_2)
        case GET_BULK_REQUEST_PDU:
            /* if we have a BULK request we may need to remove vbs to
               fit in to the maximum packet size */
            {
            int indx;
            VB_T *vbp;
            for (indx = pktp->pdu.std_pdu.std_vbl.vbl_count,
                 vbp  = pktp->pdu.std_pdu.std_vbl.vblist + indx - 1;
                 indx;
                 indx--, vbp--) {
                Clean_vb(vbp);
                /* Recalculate the pktsize, 2 is for the tag and length
                   fields, the length field may be greater than 1 but
                   that is ok, the only thing that will happen is we
                   may put less than the maximum in a buffer */
                pktsize -= 2 + vbp->vb_seq_size;
                if (pktsize < pktp->maxpkt) {
                    pktp->pdu.std_pdu.std_vbl.vbl_count = indx - 1;
                    break;
                    }
                }
            }
            break;
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_2) */
        }
    }

ENVOY_Send_SNMP_Packet(pktp);
return;
}

/****************************************************************************
NAME:  SNMP_Proxy_Traps_And_Reports

PURPOSE:  Provide proxy handling for traps and reports
          Figure out if we have the information for a proxy
          event, if so ship the message onward if not tag
          the appropriate error.  In any case indicate that
          we are finished with the packet and free it.

PARAMETERS:
        SNMP_PKT_T *    Packet we're working on

RETURNS: void
****************************************************************************/

void
  SNMP_Proxy_Traps_And_Reports(SNMP_PKT_T *pktp)
{
/* If we don't have a translation routine we bail out */
if (pktp->proxy_routine == 0) {
    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst,
                         1, pktp->async_cookie);
    return;
    }
    
/* If the current packet doesn't have it we need to get the read lock */
#if (INSTALL_ENVOY_SNMP_LOCK)
if (pktp->lockflags == LOCK_NOLOCK) {
    if (ENVOY_SNMP_GET_READ_LOCK(SNMP_CoarseLock)) {
        BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
            (BUG_OUT,
             "SNMP_Proxy_Traps_and_Reports: coarse lock broken",
             0));
        pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst,
                             1, pktp->async_cookie);
        return;
        }
    pktp->lockflags = LOCK_READ;
    pktp->coarse_lock = &SNMP_CoarseLock;
    }
#endif

if ((((INT_32_T (*) __((SNMP_PKT_T *)))(pktp->proxy_routine))(pktp)) < 0) 
    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                         pktp->async_cookie);
else
    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 0,
                         pktp->async_cookie);
return;
}

#endif /* INSTALL_ENVOY_SNMP_PROXY */
