/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/rpktasy.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: rpktasy.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:55  sar
 * Updated revision to 8.0
 *
 * Revision 7.7  1997/10/16 23:51:16  sar
 * Processing a packet with an unexpected type on an agent (trap, report,
 * inform or unknown) could result in an attempt to use a stale pointer.
 * The code has been rearranged to avoid this problem.
 *
 * If serialization is enabled processing a packet with an error could
 * cause other packets to be deferred forever.
 *
 * Revision 7.6  1997/10/16 00:45:59  sar
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
 * Revision 7.5  1997/05/13 15:37:59  sar
 * Swapped the error_complete and snmp_free lines in
 * process_packet_one so we aren't trying to operate
 * on a freed (and cleaned) packet structure.
 *
 * Revision 7.4  1997/03/20 06:49:09  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.3  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.2  1997/02/10 18:32:16  sar
 * Changed the translation of some v2 errors to v1 errors to match the
 * suggested mapping in the v2 to v1 mapping draft.
 *
 * Revision 7.1  1996/10/15  14:32:50  sar
 * Added entry point to allow internally generated packets to be processed
 * (that is a packet doesn't need to go through the decode routines).
 * Also moved some of the snmp counting code around in order to make
 * it consistent with the new routine.
 *
 * Revision 7.0  1996/03/18  20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.5  1996/01/10  16:34:47  sar
 * don't change report type pdus to response type pdus
 *
 * Revision 6.4  1995/11/14  22:44:42  sar
 * Renamed the silentdrops & proxydrops counters to match the mib def
 *
 * Revision 6.3  1995/11/04  01:52:51  sar
 * Modified the code to call proxies to be in line with the
 * current scheme, this was necessaitated by the demise of parties.
 *
 * Revision 6.1  1995/10/20  23:02:48  sar
 * remvoed party.h and v2 max packet stuff
 *
 * Revision 6.0  1995/05/31  21:47:53  sra
 * Release 6.0.
 *
 * Revision 1.6  1995/05/23  23:23:14  sar
 * Modifed the continue_reentrant code, the continue routines expect
 * their callers to have gotten the lock and will release it.  The callers
 * must get the lock before modifying the packet.
 *
 * Revision 1.5  1995/05/22  19:43:54  sar
 * Added continue_reentrant code
 *
 * Revision 1.4  1995/05/03  20:44:14  sar
 * Moved a variable declaration to be within an ifdefed block to avoid
 * compiler complaints when that block isn't being compiled.
 *
 * Revision 1.3  1995/05/02  23:34:50  sar
 * Incr req counter instead of set counter for get/next/bulk requests.
 *
 * Revision 1.2  1995/04/28  23:16:40  sar
 * Moved some stats counting around, add a call to the deferred run routine
 * to see if anything is on the deferred list, moved some of the packet
 * processing into a new routine so we have a routine for the GateKeeper.
 *
 * Revision 1.1  1995/03/21  00:09:23  sar
 * Initial revision
 *
 * Initial revision.
 */

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <snmpstat.h>
#include <objectid.h>

#include <bug.h>

/****************************************************************************
NAME:  snmp_finish_counters

PURPOSE:  Do the final counts for outbound snmp packets, generally
          called just before the call to io_complete.

PARAMETERS:
        SNMP_PKT_T *    The packet being encoded

RETURNS:  void
****************************************************************************/

/*ARGSUSED*/
static void
  snmp_finish_counters(SNMP_PKT_T *pktp)
{

#if INSTALL_ENVOY_SNMP_GROUP_V1
/* We successfully encoded the packet and are about to send it
   so increment any counters that count this packet */
inc_counter(snmp_stats.snmpOutPkts);
if (pktp->pdu.std_pdu.error_status) {
    inc_counter(snmp_stats.snmpOutGetResponses);
    switch (pktp->pdu.std_pdu.error_status) {
        case TOO_BIG:
            inc_counter(snmp_stats.snmpOutTooBigs);
            break;
        case BAD_VALUE:
            inc_counter(snmp_stats.snmpOutBadValues);
            break;
        case READ_ONLY:
            inc_counter(snmp_stats.snmpOutReadOnlys);
            break;
        case NO_SUCH_NAME:
            inc_counter(snmp_stats.snmpOutNoSuchNames);
            break;
        case GEN_ERR:
            inc_counter(snmp_stats.snmpOutGenErrs);
            break;
        }
    }
else {
    if (pktp->pdu_type == TRAP_PDU)
        inc_counter(snmp_stats.snmpOutTraps);
    }
#endif /* INSTALL_ENVOY_SNMP_GROUP_V1 */

return;
}

/****************************************************************************
NAME:  ENVOY_Send_SNMP_Error_Packet

PURPOSE:  Attempt to build and send an error packet, we expect the error
          status to be set but the pdu type to be that of the original
          packet (get, next, bulk, set).  This routine will set the
          pdu type to response.  If necessary it checks the size of
          the packet, if the packet is too big it may change the error
          and remove the vb list.  It attempts to encode the packet
          and send it using the completion routine, if it can't it 
          calls the error routine.
          
PARAMETERS:
        SNMP_PKT_T *    The decoded PDU
        INT_32_T        The error code
        INT_32_T        The error index

RETURNS:  void
****************************************************************************/
void
  ENVOY_Send_SNMP_Error_Packet(SNMP_PKT_T *pktp,
                               INT_32_T    ecode,
                               INT_32_T    eindex)
{
ATVALUE_T pdu_type;
ALENGTH_T bufsize;

/* Install the ecode and eindex in the packet */
pktp->pdu.std_pdu.error_status = ecode;
pktp->pdu.std_pdu.error_index = eindex;

/* If necessary clean up any unneeded vb lists and switch back to
   the original vb list that we saved earlier */
if (pktp->pdu.std_pdu.saved_vbl.vblist != 0) {
    Clean_vb_list(&pktp->pdu.std_pdu.std_vbl);
    pktp->pdu.std_pdu.std_vbl.vbl_count =
        pktp->pdu.std_pdu.saved_vbl.vbl_count;
    pktp->pdu.std_pdu.std_vbl.vblist = pktp->pdu.std_pdu.saved_vbl.vblist;
    pktp->pdu.std_pdu.saved_vbl.vblist = 0;
    pktp->pdu.std_pdu.saved_vbl.vbl_count = 0;
    }

/* Save the pdu type so we can check it later during the error action code,
   then make the packet a response pdu, and get the bufsize as we will
   need it in several places */
pdu_type = pktp->pdu_type;
if (pdu_type != REPORT_PDU)
  pktp->pdu_type = GET_RESPONSE_PDU;
bufsize = SNMP_Bufsize_For_Packet(pktp);

#if INSTALL_ENVOY_SNMP_VERSION_1
if (pktp->snmp_version == SNMP_VERSION_1) {
    /* If the pdu type isn't a set we need to determine if the size
       is acceptable.  If it isn't we toss the packet */
    if ((pdu_type != SET_REQUEST_PDU) && (bufsize > pktp->maxpkt)) {
        pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                             pktp->async_cookie);
        return;
        }

    /* For v1 packets we tranlsate any v2 errors back to v1 codes */
    switch (pktp->pdu.std_pdu.error_status) {
        case NO_ACCESS:
        case NO_CREATION:
        case NOT_WRITABLE:
        case INCONSISTENT_NAME:
        case AUTHORIZATION_ERROR:
            pktp->pdu.std_pdu.error_status  = NO_SUCH_NAME;
            break;
        case WRONG_TYPE:
        case WRONG_LENGTH:
        case WRONG_ENCODING:
        case WRONG_VALUE:
        case INCONSISTENT_VALUE:
            pktp->pdu.std_pdu.error_status = BAD_VALUE;
            break;
        case RESOURCE_UNAVAILABLE:
        case COMMIT_FAILED:
        case UNDO_FAILED:
            pktp->pdu.std_pdu.error_status = GEN_ERR;
            break;
        }
    }
#endif

#if INSTALL_ENVOY_SNMP_VERSION_2
if (pktp->snmp_version != SNMP_VERSION_1) {
    /* If the packet is tagged as toobig or will be toobig and the
       pdu isn't BULK we use the alternate pdu which uses a stripped
       vblist.  If the pdu was bulk or the alternate pdu was still
       too big we discard the packet. */

    if ((pktp->pdu.std_pdu.error_status == TOO_BIG) ||
        (bufsize > pktp->maxpkt)) {
        if (pdu_type == GET_BULK_REQUEST_PDU) {
            SGRPv2_INC_COUNTER(snmp_stats.snmpSilentDrops);
            pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                                 pktp->async_cookie);
            return;
            }
        /* strip the vbl */
        Clean_vb_list(&pktp->pdu.std_pdu.std_vbl);
        pktp->pdu.std_pdu.std_vbl.vblist = 0;
        pktp->pdu.std_pdu.std_vbl.vbl_count = 0;
        pktp->pdu.std_pdu.error_status = TOO_BIG;
        pktp->pdu.std_pdu.error_index = 0;
        bufsize = SNMP_Bufsize_For_Packet(pktp);
        if (bufsize > pktp->maxpkt) {
            SGRPv2_INC_COUNTER(snmp_stats.snmpSilentDrops);
            pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                                 pktp->async_cookie);
            return;
            }
        }
    }
#endif

/* Call the routine to count the stuff in the packet then
   call the io_complete routine, which should call the encode
   routine then send the packet */
snmp_finish_counters(pktp);
pktp->io_complete(&pktp->pkt_src, &pktp->pkt_dst, (PTR_T)pktp, bufsize,
                  pktp->async_cookie);
return;
}

/****************************************************************************
NAME:  ENVOY_Send_SNMP_Packet

PURPOSE:  Attempt to build and send a packet, if the error status is
          set or we find an error we call Envoy_Send_SNMP_Error_Packet,
          otherwise we attempt to do the encoding and transmit the packet
          using the io completion routine if we can't we call the
          error completion routine
          
PARAMETERS:
        SNMP_PKT_T *    The decoded PDU

RETURNS:  void
****************************************************************************/
void
  ENVOY_Send_SNMP_Packet(SNMP_PKT_T *pktp)
{
ALENGTH_T bufsize;

if (pktp->pdu.std_pdu.error_status != NO_ERROR) {
    /* if we had an error we send it */
    ENVOY_Send_SNMP_Error_Packet(pktp, pktp->pdu.std_pdu.error_status,
                                 pktp->pdu.std_pdu.error_index);
    return;
    }

/* get the size of the packet which we will need several times */
bufsize = SNMP_Bufsize_For_Packet(pktp);

/* For gets and nexts we need to test the size of the packet, if
   it would be too large we set the too_big error otherwise we
   don't need to do anything, the outgoing buffer will be built
   at the end of the routine. */

if (((pktp->pdu_type == GET_REQUEST_PDU) ||
     (pktp->pdu_type == GET_NEXT_REQUEST_PDU)) &&
     (bufsize > pktp->maxpkt)) {
    ENVOY_Send_SNMP_Error_Packet(pktp, TOO_BIG, 0);
    return;
    }

/* If the stats group is installed count up the objects we have processed
   we do it here so we now what type of packet we are dealing with */
#if INSTALL_ENVOY_SNMP_GROUP_V1
if (pktp->pdu.std_pdu.error_status == 0) {
    VBL_T *vbl;
    VB_T  *vbp;
    int i;
    switch(pktp->pdu_type) {
        case GET_REQUEST_PDU:
        case GET_NEXT_REQUEST_PDU:
        case GET_BULK_REQUEST_PDU:
            inc_counter(snmp_stats.snmpOutGetResponses);
            for(vbl = &pktp->pdu.std_pdu.std_vbl; vbl; vbl = vbl->vblp) {
                for(vbp = vbl->vblist, i = vbl->vbl_count; i; i--, vbp++) {
                    switch(vbp->vb_data_flags_n_type) {
                        case VT_NOSUCHINS:
                        case VT_NOSUCHOBJ:
                        case VT_ENDOFMIB:
                            break;
                        default:
                            inc_counter(snmp_stats.snmpInTotalReqVars);
                            break;
                        }
                    }
                }
            break;
        case SET_REQUEST_PDU:
            inc_counter(snmp_stats.snmpOutGetResponses);
            add_counter(snmp_stats.snmpInTotalSetVars,
                        pktp->pdu.std_pdu.std_vbl.vbl_count);
            break;
        }
    }
#endif

/* If we made it here we make the packet a response then try and encode
   it, if we can't we toss it otherwise we try and send the reply */
if (pktp->pdu_type != REPORT_PDU)
  pktp->pdu_type = GET_RESPONSE_PDU;

snmp_finish_counters(pktp);
pktp->io_complete(&pktp->pkt_src, &pktp->pkt_dst, (PTR_T)pktp, bufsize,
                  pktp->async_cookie);
return;
}

/****************************************************************************
NAME:  SNMP_continue_function

PURPOSE:  Examine the packet, if all of the operations have finished build
          and send a reply.  For GETs this is straightforward we simply
          need to see if all of the gets have finished. For NEXTs and
          we may need to reissue any requests that didn't find an
          instance or found an instance that wasn't in the view.  We wait
          to reissue the requests until all of the outstanding ops have
          finished so we can batch them if possible.  For BULKs we do the
          same as well as step the entire list if necessary.  For SETs
          we will need to call the setprocs if all of the testprocs succeeded.

PARAMETERS:
        SNMP_PKT_T *    The decoded GET PDU

RETURNS:  void
****************************************************************************/

void
  SNMP_Continue_function(SNMP_PKT_T *pktp)
{
VB_T *vbp;
int count;

/* For gets, nexts & bulks we check to see if the current set of ops 
   are done.  This is a straight forward check of the flags word.
   Then we do other work as required for gets we simply call the
   send routine.  For nexts we may need to redo any requests that 
   didn't find an acceptable next instance.  For bulks we need to
   do the next check and if that succceeds we may need to do another
   repetition.
   For sets we just call the set routine as the test to see what
   to do is more complicated then a simple flag check. */

switch(pktp->pdu_type) {
    case GET_REQUEST_PDU:
        /* Walk through the vblist to see if all ops have been completed */
        vbp = pktp->pdu.std_pdu.std_vbl.vblist;
        count = pktp->pdu.std_pdu.std_vbl.vbl_count;
        for(; count; count--, vbp++) {
            if (!(vbp->vb_flags & VFLAG_GET_DONE))
                goto cont_continue;
            }
        goto cont_finish;

    case GET_NEXT_REQUEST_PDU:
        /* we put the next request in a while loop so we can catch a redo
           that finished immmediately and didn't set up a call back */
        while(1) {
            /* Walk through the vblist to see if all ops have been completed */
            vbp = pktp->pdu.std_pdu.std_vbl.vblist;
            count = pktp->pdu.std_pdu.std_vbl.vbl_count;
            for(; count; count--, vbp++) {
                if (!(vbp->vb_flags & VFLAG_NEXT_DONE))
                    goto cont_continue;
                }

            switch(SNMP_Process_Next_Redo(pktp)) {
                case -1:
                case 0:
                    goto cont_finish;
                }
            }

    case SET_REQUEST_PDU:
        /* Sets may also require examining the packet to see if we are
           done or are waiting for a callback, but the function will
           be handled by the Process_Set_Pdu_Async routine. */
        if (SNMP_Process_Set_PDU(pktp) == 0)
            goto cont_finish;
        goto cont_continue;

#if (INSTALL_ENVOY_SNMP_VERSION_2)
    case GET_BULK_REQUEST_PDU:
        {
        VBL_T *vblp;
        /* we put the bulk request in a while loop so we can catch a redo
           or new repetition that finished immmediately and didn't set up
           a call back */
        while(1) {
            /* Find the correct vblist to work with.  Bulks may have
               several vblists chained togther, we want the last one
               which is the only one that may have ops outstanding. */
            for(vblp = &(pktp->pdu.std_pdu.std_vbl);
                vblp->vblp;
                vblp = vblp->vblp)
                ;
            /* Walk through the vblist to see if all ops have been completed */
            vbp = vblp->vblist;
            count = vblp->vbl_count;
            for(; count; count--, vbp++) {
                if (!(vbp->vb_flags & VFLAG_NEXT_DONE))
                    goto cont_continue;
                }

            /* First we test to see if we need to do any more work for
               the current repetition, if not (case 0) we see if there
               are more repetitions to do if not (case 0) we attempt to
               send the response packet.  Case -1 indicates an already
               handeled error. */
            switch(SNMP_Process_Next_Redo(pktp)) {
                case -1:
                    goto cont_finish;
                case 0:
                    if (SNMP_Process_Bulk_Redo(pktp) == 0)
                        goto cont_finish;
                    break;
                }
            } /* while(1) */
        }
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_2) */
    } /* switch(pktp->pdu_type) */

/* If CONTINUE_REENTRANT is installed this routine assumes that 
   the calling routine has acquired the write lock for this packet.
   This routine will always release the write lock.  */

cont_finish:
#if (INSTALL_ENVOY_CONTINUE_REENTRANT)
ENVOY_SNMP_RELEASE_WRITE_LOCK(pktp->continue_lock);
#endif

ENVOY_Send_SNMP_Packet(pktp);
SNMP_Free(pktp);

#if (INSTALL_ENVOY_SNMP_SERIALIZE)
GateRunDeferreds();
#endif /* (INSTALL_ENVOY_SNMP_SERIALIZE) */

return;

cont_continue:
#if (INSTALL_ENVOY_CONTINUE_REENTRANT)
ENVOY_SNMP_RELEASE_WRITE_LOCK(pktp->continue_lock);
#endif
return;
}

/****************************************************************************
NAME:  process_packet_two

PURPOSE:  A routine to do the mid stage processing for an snmp packet,
          this will allow us to have one block of code called by the 
          main processing branch or the deferral routine in serial.c

PARAMETERS:
        SNMP_PKT_T *    The packet being processed

RETURNS: void
****************************************************************************/

void
  process_packet_two(PTR_T dptr)
{
SNMP_PKT_T *rp = (SNMP_PKT_T *)dptr;

#if (INSTALL_ENVOY_CONTINUE_REENTRANT)
/* If necessary we acquire the packet continue write lock here
   It gets freed when we call the continue routine.  Any user
   routines that defer processing of a request should also use
   the write lock, this will stop us from stepping on each other
   when modifying the packet.
   We don't have any good options if we can't get the lock.
   The user shouldn't do that, if they do we generate a bug
   report and if they don't handle that we return which is
   the least bad thing to do. */
if (ENVOY_SNMP_GET_WRITE_LOCK(rp->continue_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_FATAL, 0,
        (BUG_OUT, "process_packet_two(): packet continue lock is broken", 0));
    return;
    }
#endif

switch(rp->pdu_type) {
   case GET_REQUEST_PDU:
        if (SNMP_Process_Get_PDU(rp)) {
            goto proc2_error;
            }
        break;
   case GET_NEXT_REQUEST_PDU:
   case GET_BULK_REQUEST_PDU:
        if (SNMP_Process_Next_PDU(rp)) {
            goto proc2_error;
            }
        break;

   case SET_REQUEST_PDU:
        if (SNMP_Process_Test_PDU(rp)) {
            goto proc2_error;
            }
        break;
   }

SNMP_Continue_function(rp);
return;

proc2_error:
#if (INSTALL_ENVOY_CONTINUE_REENTRANT)
ENVOY_SNMP_RELEASE_WRITE_LOCK(rp->continue_lock);
#endif

SNMP_Free(rp);

#if (INSTALL_ENVOY_SNMP_SERIALIZE)
GateRunDeferreds();
#endif /* (INSTALL_ENVOY_SNMP_SERIALIZE) */

return;
}

/****************************************************************************
NAME:  process_packet_one

PURPOSE:  Manage the execution of an internal SNMP packet
          (one that is already in an snmp_pkt_t structure)
          in an async fashion.

PARAMETERS:
        SNMP_PKT_T *    The packet, in its c-structure format, to process

RETURNS: void
****************************************************************************/

static void
  process_packet_one(SNMP_PKT_T *rp)
{
switch(rp->pdu_type) {
    case GET_REQUEST_PDU:
        SGRPv1_INC_COUNTER(snmp_stats.snmpInGetRequests);
        break;
    case GET_NEXT_REQUEST_PDU:
        SGRPv1_INC_COUNTER(snmp_stats.snmpInGetNexts);
        break;
    case SET_REQUEST_PDU:
        SGRPv1_INC_COUNTER(snmp_stats.snmpInSetRequests);
        break;
    case GET_BULK_REQUEST_PDU:
        break;
#if (INSTALL_ENVOY_SNMP_PROXY)
    case GET_RESPONSE_PDU:
        SGRPv1_INC_COUNTER(snmp_stats.snmpInGetResponses);
        SNMP_Proxy_Response(rp);
        SNMP_Free(rp);
        return;
#if (INSTALL_ENVOY_SNMP_VERSION_1)
    case TRAP_PDU:
        SGRPv1_INC_COUNTER(snmp_stats.snmpInTraps);
        SNMP_Proxy_Traps_And_Reports(rp);
        SNMP_Free(rp);
        return;
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_1) */
#if (INSTALL_ENVOY_SNMP_VERSION_2)
    case TRAP2_PDU:
        SGRPv1_INC_COUNTER(snmp_stats.snmpInTraps);
    case REPORT_PDU:
        SNMP_Proxy_Traps_And_Reports(rp);
        SNMP_Free(rp);
        return;
    case INFORM_REQUEST_PDU:
        if (rp->proxy_routine) {
            SNMP_Proxy_Request(rp);
            SNMP_Free(rp);
            return;
            }
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
        rp->error_complete(&rp->pkt_src, &rp->pkt_dst, 1, rp->async_cookie);
        SNMP_Free(rp);
        return;
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_2) */
#endif /* (INSTALL_ENVOY_SNMP_PROXY) */
    default:
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
        rp->error_complete(&rp->pkt_src, &rp->pkt_dst, 1, rp->async_cookie);
        SNMP_Free(rp);
        return;
    }

/* If proxy handling is installed see if this packet needs proxy
   handling.  */
#if (INSTALL_ENVOY_SNMP_PROXY)
if (rp->proxy_routine) {
    SNMP_Proxy_Request(rp);
    SNMP_Free(rp);
    return;
    }
#endif /* #if (INSTALL_ENVOY_SNMP_PROXY) */

#if (INSTALL_ENVOY_SNMP_SERIALIZE)
switch (GateKeeper_Add(GATE_SNMP, (PTR_T)rp, process_packet_two)) {
    case 0:
        break;
    case 1:
        return;
    default:
        ENVOY_Send_SNMP_Error_Packet(rp, GEN_ERR, 0);
        SNMP_Free(rp);
        return;
    }
#endif /* (INSTALL_ENVOY_SNMP_SERIALIZE) */

process_packet_two((PTR_T)rp);
}

/****************************************************************************
NAME:  Process_Rcvd_SNMP_Packet_Async

PURPOSE:  Manage the decoding and execution of an incoming SNMP packet
          in an async fashion.

PARAMETERS:
        size_t          Length of packet
        bits8_t    *    Pointer to the packet
        SNMPADDR_T *    Source of the packet
        SNMPADDR_T *    Destination of the packet (most likely
                        the address of the machine on which this
                        code is running.)
        ALENGTH_T       the maximum size of the outgoing packet
        void (*)(SNMPADDR_T *,  The io completion routine,
                 SNMPADDR_T *,  The args are dst, src and packet
                 EBUFFER_T  *)
        void (*)(SNMPADDR_T *,  The io error routine,
                 SNMPADDR_T *,  The args are dst, src and error code
                 int)
        PTR_T           Cookie to be handed to the completion routines.

RETURNS: void

NOTE: The call to the completion routine means that a valid snmp packet
was generated.  The packet may be a normal response or it could be a 
snmp error (for example too big) but the packet should be sent to the
original requester.
****************************************************************************/

void
  Process_Rcvd_SNMP_Packet_Async(size_t          pktl,
                                 bits8_t        *pktp,
                                 SNMPADDR_T     *for_addr,
                                 SNMPADDR_T     *loc_addr,
                                 ALENGTH_T       pktsize,
                                 IO_COMPLETE_T  *io_complete,
                                 ERR_COMPLETE_T *error_complete,
                                 PTR_T           complete_cookie)
{

SNMP_PKT_T *rp;
int error_code;

SGRPv1v2_INC_COUNTER(snmp_stats.snmpInPkts);

rp = SNMP_Decode_Packet_WER((unsigned char *)pktp, (int)pktl, for_addr,
                            loc_addr, &error_code);
if (rp == 0) {
    error_complete(for_addr, loc_addr, 1, complete_cookie);
    return;
    }
    
SNMP_TRACE_INPUT(rp);

/* See if the given maximum size is smaller than the default */
if (pktsize < rp->maxpkt)
    rp->maxpkt = pktsize;

/* Attach the completion routines to the packet structure */
rp->io_complete = io_complete;
rp->error_complete = error_complete;
rp->async_cookie = complete_cookie;
rp->cont = SNMP_Continue_function;

/* Check the error code returned from decode_pkt, if it isn't zero then
   the decode routine is requesting that we send an error packet to the
   requester.  We need to encode the packet and call the completion routine,
   if we can't do the encode we call the error routine. */

if (error_code != 0) {
    ENVOY_Send_SNMP_Packet(rp);
    SNMP_Free(rp);
    return;
    }

process_packet_one(rp);
}

/****************************************************************************
NAME:  Process_Internal_SNMP_Packet

PURPOSE:  The entry point to allow a user to process an internally
          constructed snmp packet, mostly this is a way to get the
          counters (in pkts) correct.

PARAMETERS:
        SNMP_PKT_T *    The packet, in its c-structure format, to process

RETURNS: void
****************************************************************************/

void
  Process_Internal_SNMP_Packet(SNMP_PKT_T *rp)
{
SGRPv1v2_INC_COUNTER(snmp_stats.snmpInPkts);
rp->cont = SNMP_Continue_function;
process_packet_one(rp);
}

/****************************************************************************
NAME:  SNMP_Process_Finish

PURPOSE:  This routine is called by the io_complete routines to finish
          processing a packet, primarily it encodes the packet into
          the buffer, it also incrments any error counters.

PARAMETERS:
        SNMP_PKT_T *    The packet being encoded
        EBUFFER_T  *    The buffer to encode into
        ALENGTH_T       The max size the packet will require

RETURNS:  int   0 for success
****************************************************************************/

/*ARGSUSED*/
int
  SNMP_Process_Finish(SNMP_PKT_T *pktp,
                      EBUFFER_T  *ebufp,
                      ALENGTH_T   need)
{

if (SNMP_Encode_Pkt_With_Siz(pktp, ebufp, need))
    return(-1);
return(0);
}

