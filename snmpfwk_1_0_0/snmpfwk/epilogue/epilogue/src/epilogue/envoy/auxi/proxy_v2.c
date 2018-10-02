/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/proxy_v2.c,v 1.1 2006/06/29 12:17:25 smasyutin Exp $ */

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
 * $Log: proxy_v2.c,v $
 * Revision 1.1  2006/06/29 12:17:25  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:06  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:22  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:13  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:50:19  sra
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
#include <objectid.h>
#include <buffer.h>
#include <buildpkt.h>
#include <wkobj.h>
#include <proxy_tr.h>

/* We include the proxy translation routine if we install v2 and
   the v1 to v2 trap routine if v1 is installed as well */

#if (INSTALL_ENVOY_SNMP_PROXY && INSTALL_ENVOY_SNMP_VERSION_2)

static OIDC_T csoidc[] = {SNMP_COLDSTART};
static OIDC_T wsoidc[] = {SNMP_WARMSTART};
static OIDC_T ldoidc[] = {SNMP_LINKDOWN};
static OIDC_T luoidc[] = {SNMP_LINKUP};
static OIDC_T afoidc[] = {SNMP_AUTHENTICATIONFAILURE};
static OIDC_T egoidc[] = {SNMP_EGPNEIGHBORLOSS};
static OIDC_T stoidc[] = {SNMP_SNMPTRAPOID, 0};
static OIDC_T sysupoidc[] = {1, 3, 6, 1, 2, 1, 1, 3, 0};
static OIDC_T enteroidc[] = {SNMP_SNMPTRAPENTERPRISE, 0};

#if INSTALL_ENVOY_SNMP_VERSION_1
/****************************************************************************
NAME:  SNMP_Proxy_Trap_V1_To_V2

PURPOSE:  Translate a v1 trap to a v2 trap, encode it
          and call the proxy transport routine to send it.

PARAMETERS:
        SNMP_PKT_T *    Packet we're working on

RETURNS: INT_32_T       < 0 means failure, otherwise a time out period is
                        returned indicating how long the transport routine
                        thinks it should take for a response.  Not all
                        options will generate a response (indeed this one
                        doesn't expect a response).
         
****************************************************************************/

INT_32_T
  SNMP_Proxy_Trap_V1_To_V2(SNMP_PKT_T *v1_pkt)
{
PROXY_BLOCK_T *pblockp;
SNMP_PKT_T *v2_pkt;
VB_T *vbp;
OBJ_ID_T *eoid;
OIDC_T *tempoidc;
EBUFFER_T ebuf;
INT_32_T eflag;
int incount, outcount, tempcount, free_tempoidc = 0, i;

pblockp = (PROXY_BLOCK_T *)(v1_pkt->user_private);
incount = v1_pkt->pdu.trap_pdu.trap_vbl.vbl_count;
outcount = incount + 3;

/* Attempt to allocate a new packet */
if ((v2_pkt = SNMP_Create_Request2(TRAP2_PDU, SNMP_VERSION_2,
                                   EBufferUsed(&pblockp->community),
                                   (char *)EBufferStart(&pblockp->community),
                                   0, outcount, 0, 0)) == 0)
    return(-1);

/* we may use the enterprise oid several times so get a pointer */
eoid = &v1_pkt->pdu.trap_pdu.enterprise_objid;

/* copy sysuptime into the first varbind */
if (SNMP_Bind_Timeticks(v2_pkt, 0, sizeof(sysupoidc)/sizeof(OIDC_T),
                        sysupoidc, v1_pkt->pdu.trap_pdu.trap_time_ticks)) {
    SNMP_Free(v2_pkt);
    return(-1);
    }

/* work out the snmptrapoid and put it into the second varbind */
switch (v1_pkt->pdu.trap_pdu.generic_trap) {
    case COLD_START:
        tempoidc = csoidc;
        tempcount = sizeof(csoidc)/sizeof(OIDC_T);
        break;
    case WARM_START:
        tempoidc = wsoidc;
        tempcount = sizeof(wsoidc)/sizeof(OIDC_T);
        break;
    case LINK_DOWN:     
        tempoidc = ldoidc;
        tempcount = sizeof(ldoidc)/sizeof(OIDC_T);
        break;
    case LINK_UP:
        tempoidc = luoidc;
        tempcount = sizeof(luoidc)/sizeof(OIDC_T);
        break;
    case AUTH_FAILURE:
        tempoidc = afoidc;
        tempcount = sizeof(afoidc)/sizeof(OIDC_T);
        break;
    case EGP_NEIGHBOR_LOSS:
        tempoidc = egoidc;
        tempcount = sizeof(egoidc)/sizeof(OIDC_T);
        break;
    case ENTERPRISE_SPECIFIC:
        tempcount = eoid->num_components + 2;
        tempoidc = (OIDC_T *)SNMP_memory_alloc(sizeof(OIDC_T) * tempcount);
        if (tempoidc == 0) {
            SNMP_Free(v2_pkt);
            return(-1);
            }
        free_tempoidc = 1;
        MEMCPY(tempoidc, eoid->component_list,
               sizeof(OIDC_T) * (tempcount - 2));
        tempoidc[tempcount - 2] = 0;
        tempoidc[tempcount - 1] = v1_pkt->pdu.trap_pdu.specific_trap;
        break;
    default:
        SNMP_Free(v2_pkt);
        return(-1);
    }

eflag = (INT_32_T)SNMP_Bind_Object_ID(v2_pkt, 1, sizeof(stoidc)/sizeof(OIDC_T),
                                      stoidc, tempcount, tempoidc);
if (free_tempoidc)
    SNMP_memory_free(tempoidc);
if (eflag) {
    SNMP_Free(v2_pkt);
    return(-1);
    }

/* copy the vblist into the third through outcount - 2 */
for (vbp = v1_pkt->pdu.trap_pdu.trap_vbl.vblist, i = 2;
     incount;
     incount--, i++, vbp++) {
    switch(vbp->vb_data_flags_n_type) {
        case VT_NUMBER:
            if (SNMP_Bind_Integer(v2_pkt, i,
                                  vbp->vb_obj_id.num_components,
                                  vbp->vb_obj_id.component_list,
                                  vbp->value_u.v_number)) {
                SNMP_Free(v2_pkt);
                return(-1);
                }
            break;
        case VT_COUNTER:
        case VT_GAUGE:
        case VT_TIMETICKS:
            if (SNMP_Bind_Unsigned_Integer(v2_pkt, i,
                                           vbp->vb_obj_id.num_components,
                                           vbp->vb_obj_id.component_list,
                                           vbp->vb_data_flags_n_type,
                                           vbp->value_u.v_counter)) {
                SNMP_Free(v2_pkt);
                return(-1);
                }
            break;
    case VT_IPADDRESS:
            if (SNMP_Bind_IP_Address(v2_pkt, i,
                                     vbp->vb_obj_id.num_components,
                                     vbp->vb_obj_id.component_list,
                                     vbp->value_u.v_network_address)) {
                SNMP_Free(v2_pkt);
                return(-1);
                }
            break;
        case VT_OBJECT:
            if (SNMP_Bind_Object_ID(v2_pkt, i,
                                    vbp->vb_obj_id.num_components,
                                    vbp->vb_obj_id.component_list,
                                    vbp->value_u.v_object.num_components,
                                    vbp->value_u.v_object.component_list)) {
                SNMP_Free(v2_pkt);
                return(-1);
                }
            break;
        case VT_STRING:
        case VT_OPAQUE:
            if (SNMP_Bind_String(v2_pkt, i,
                                 vbp->vb_obj_id.num_components,
                                 vbp->vb_obj_id.component_list,
                                 vbp->vb_data_flags_n_type,
                                 EBufferUsed(&vbp->value_u.v_string),
                                 EBufferStart(&vbp->value_u.v_string),
                                 0)) {
                SNMP_Free(v2_pkt);
                return(-1);
                }
            break;
        case VT_EMPTY:
            if (SNMP_Bind_Null(v2_pkt, i,
                               vbp->vb_obj_id.num_components,
                               vbp->vb_obj_id.component_list)) {
                SNMP_Free(v2_pkt);
                return(-1);
                }
            break;
        default:
            SNMP_Free(v2_pkt);
            return(-1);
        }
    }
/* copy the enterprise information into the last var bind (outcount - 1) */
if (SNMP_Bind_Object_ID(v2_pkt, outcount - 1, sizeof(enteroidc)/sizeof(OIDC_T),
                        enteroidc, eoid->num_components,
                        eoid->component_list)) {
    SNMP_Free(v2_pkt);
    return(-1);
    }

/* So we have finally got the packet built, attempt to encode and send it */
EBufferInitialize(&ebuf);
eflag = (INT_32_T)SNMP_Encode_Packet(v2_pkt, &ebuf);
SNMP_Free(v2_pkt);
if (eflag)
    return(-1);
eflag = pblockp->transport_routine(&ebuf, pblockp->transport_block);
EBufferClean(&ebuf);
return(eflag);
}
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_1 */

/****************************************************************************
NAME:  SNMP_Proxy_V2_Translation

PURPOSE:  Translate packets to v2 format then encode the packet and
          call the transport routine to send the byte string.
          Traps are changed to trap2s, all other pdus are
          unchanged

PARAMETERS:
        SNMP_PKT_T *    Packet we're working on

RETURNS: INT_32_T       < 0 means failure, otherwise a time out period is
                        returned indicating how long the transport routine
                        thinks it should take for a response.  Not all
                        options will generate a response.
         
****************************************************************************/

INT_32_T
  SNMP_Proxy_V2_Translation(SNMP_PKT_T *pktp)
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
    case GET_BULK_REQUEST_PDU:
    case TRAP2_PDU:
    case INFORM_REQUEST_PDU:
    case REPORT_PDU:
        break;
#if (INSTALL_ENVOY_SNMP_VERSION_1)
    case TRAP_PDU:
        return(SNMP_Proxy_Trap_V1_To_V2(pktp));
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_1) */
    default:
        /* Unknown packet types */
        return(-1);
    }

/* Save the version stamp and community string so we can restore
   them after we encode the packet, then replace them with version 2
   and the new community string */
version = pktp->snmp_version;
pktp->snmp_version = SNMP_VERSION_2;

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
#endif /* #if (INSTALL_ENVOY_SNMP_PROXY ...) */

 
