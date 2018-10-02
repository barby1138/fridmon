/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/proxy_v1.c,v 1.1 2006/06/29 12:17:25 smasyutin Exp $ */

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
 * $Log: proxy_v1.c,v $
 * Revision 1.1  2006/06/29 12:17:25  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:05  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:22  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:12  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:50:18  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/15 22:21:24  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 1.1  1995/11/14  22:40:40  sar
 * Initial revision
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <buffer.h>
#include <proxy_tr.h>

#if INSTALL_ENVOY_SNMP_VERSION_1
/****************************************************************************
NAME:  SNMP_Proxy_V1_Translation

PURPOSE:  Translate packets to v1 format then encode the packet and
          call the transport routine to send the byte string.
          Gets, nexts, sets and traps aren't changed, bulks
          are mapped to a next, trap2s are changed to traps
          reports and informs are dropped.

PARAMETERS:
        SNMP_PKT_T *    Packet we're working on

RETURNS: INT_32_T       < 0 means failure, otherwise a time out period is
                        returned indicating how long the transport routine
                        thinks it should take for a response.  Not all
                        options will generate a response.
         
****************************************************************************/

INT_32_T
  SNMP_Proxy_V1_Translation(SNMP_PKT_T *pktp)
{
EBUFFER_T ebuf, cbuf;
INT_32_T eflag, version;
PROXY_BLOCK_T *pblockp;

/* Get a copy of the proxy block structure for ease of use */
pblockp = (PROXY_BLOCK_T *)(pktp->user_private);

/* Make sure we have a transport routine */
if (pblockp->transport_routine == 0)
    return(-1);

switch(pktp->snmp_version) {
    case GET_REQUEST_PDU:
    case GET_NEXT_REQUEST_PDU:
    case SET_REQUEST_PDU:
    case TRAP_PDU:
        break;
#if (INSTALL_ENVOY_SNMP_VERSION_2)
    case GET_BULK_REQUEST_PDU:
        pktp->pdu_type = GET_NEXT_REQUEST_PDU;
        pktp->pdu.std_pdu.error_status = 0;
        pktp->pdu.std_pdu.error_index = 0;
        break;
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_2) */
    case TRAP2_PDU:
    case INFORM_REQUEST_PDU:
    case REPORT_PDU:
    default:
        /* There is no V1 equivalent to these cases so we drop the packet */
        return(-1);
    }

/* Save the version stamp and community string so we can restore
   them after we encode the packet, then replace them with version 1
   and the new community string */
version = pktp->snmp_version;
pktp->snmp_version = SNMP_VERSION_1;

/* save the current community */
EBufferPreLoad(pktp->community.bflags, &cbuf,
               EBufferStart(&pktp->community),
               EBufferUsed(&pktp->community));
/* install the new community */
EBufferPreLoad(pblockp->community.bflags, &pktp->community,
               EBufferStart(&pblockp->community),
               EBufferUsed(&pblockp->community));

EBufferInitialize(&ebuf);
eflag = (INT_32_T)SNMP_Encode_Packet(pktp, &ebuf);

/* Switch the packet back to the original version and community string */
pktp->snmp_version = version;
/* restore the community */
EBufferPreLoad(cbuf.bflags, &pktp->community, EBufferStart(&cbuf),
               EBufferUsed(&cbuf));
/* init the saved one for safety */
EBufferInitialize(&cbuf);

if (eflag)
    return(-1);
eflag = pblockp->transport_routine(&ebuf, pblockp->transport_block);
EBufferClean(&ebuf);
return(eflag);
}
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_1 */

