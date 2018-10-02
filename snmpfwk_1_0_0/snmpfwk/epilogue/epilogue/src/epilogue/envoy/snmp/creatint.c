/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/creatint.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 *  Copyright 1996-1997 Epilogue Technology Corporation.
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
 * $Log: creatint.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:46  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/11/01 01:25:36  sar
 * In the packet we are processing save a pointer to the coarse lock we
 * are using so that we can free it later without needing to know which
 * lock it was.
 *
 * Revision 7.4  1997/10/16 00:45:49  sar
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
 * Revision 7.3  1997/03/20 06:48:48  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1996/10/18 21:33:02  sar
 * Free the packet if validate community fails
 *
 * Revision 7.0  1996/10/15  14:52:09  sar
 * Initial version
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <buildpkt.h>
#include <snmpstat.h>

#include <bug.h>

/****************************************************************************
NAME:  SNMP_Create_Internal_Request

PURPOSE:  Build a snmp request packet for use with
          Process_Internal_SNMP_Packet().  This routine will
          allocate a snmp_pkt_t structure and fill in the appropriate
          header pieces as if a byte string had been decoded.  The
          caller is still responsible for attaching any desired varbinds
          via bind calls.  Note that we call the validate_community
          routine to actually validate the community and gather the
          mib view type information.

PARAMETERS:
        int             Packet type:
                        For version 1 GET_REQUEST_PDU, GET_NEXT_REQUEST_PDU or
                                      SET_REQUEST_PDU
                        For version 2 GET_REQUEST_PDU, GET_NEXT_REQUEST_PDU,
                                      SET_REQUEST_PDU, GET_BULK_REQUEST_PDU,
                        as defined in snmpdefs.h
        int             Protocol version -- SNMP_VERSION_1 or SNMP_VERSION_2
                        as defined in snmpdefs.h
        int             Community name length
        sbits8_t *      Community name 
        unsigned short int Dynamic or static flag for community name
        INT_32_T        Request ID
        int             Number of VarBindList elements needed (may be 0)
        int             nonreps - for use with getbulk, number of non repeaters
        int             maxreps - for use with getbulk, max repetitions

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

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, 0 on failure
****************************************************************************/

SNMP_PKT_T *
  SNMP_Create_Internal_Request(int             ptype,
                               int             version,
                               int             commleng,
                               sbits8_t       *community,
                               unsigned short int cflags,
                               INT_32_T        request_id,
                               int             num_vb,
                               int             nonreps,
                               int             maxreps,
                               ALENGTH_T       pktsize,
                               IO_COMPLETE_T  *io_complete,
                               ERR_COMPLETE_T *error_complete,
                               PTR_T           complete_cookie)
{
SNMP_PKT_T *rp;
SNMPADDR_T dummy_addr;

MEMSET(&dummy_addr, 0, sizeof(SNMPADDR_T));

/* check the pdu type, but only for options allowed
   by snmp_create_request2 but not by this routine.
   We leave most of the version and ptype checking to 
   snmp_create_request2. */
if ((ptype == INFORM_REQUEST_PDU) || (ptype == TRAP2_PDU))
    return(0);

/* create the packet structure */
rp = SNMP_Create_Request2(ptype, version, commleng, (char *)community,
                          request_id, num_vb, nonreps, maxreps);
if (rp == 0)
    return(0);

/* If the lock code is installed acquire the generic read lock */
#if INSTALL_ENVOY_SNMP_LOCK
if (ENVOY_SNMP_GET_READ_LOCK(SNMP_CoarseLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Create_Internal_Request: coarse lock broken", 0));
    SNMP_Free(rp);
    return(0);
    }

rp->lockflags = LOCK_READ;
rp->coarse_lock = &SNMP_CoarseLock;
#endif

switch(SNMP_validate_community(rp, &dummy_addr, &dummy_addr)) {
    case 0:
        break;
    case 1:
    default:
        SNMP_Free(rp);
        return(0);
    case 2:
        SNMP_Free(rp);
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInBadCommunityNames);
        return(0);
    case 3:
        SNMP_Free(rp);
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInBadCommunityUses);
        return(0);
    }

/* If necessary reload the community string, create_req2 thinks it should
   always be static, we give people an option, we do this after the
   validate step so that the community string isn't freed if we return
   an empty packet */
if (cflags != BFL_IS_STATIC)
    EBufferPreLoad(BFL_IS_DYNAMIC, &(rp->community), community, commleng);

/* reset the maximum packet size, note that we ignore the default
   so that the default can be used for wire based snmp while this
   is being used internally */
rp->maxpkt = pktsize;

/* Attach the completion routines to the packet structure */
rp->io_complete = io_complete;
rp->error_complete = error_complete;
rp->async_cookie = complete_cookie;
return(rp);
}

