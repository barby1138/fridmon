/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/sa_core.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 *  Copyright 1994-1997 Epilogue Technology Corporation.
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
 * $Log: sa_core.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:56  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/10/22 03:10:23  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.2  1997/03/20 06:49:10  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/11/01  00:46:48  sar
 * remvoed localio.h, bstring.h
 * removed no_pp stuff
 * removed nsap address and bitstrings
 *
 * Revision 6.0  1995/05/31  21:47:55  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/04/28  23:15:51  sar
 * Moved utilitiy functions for the master to sa_mutil.c
 *
 * Revision 5.3  1995/04/18  21:13:44  sar
 * Moved sa_do query to sa_mast.c, moved the necessary sections from
 * do_queryt into freeproc directly.
 *
 * Revision 5.2  1995/03/21  00:30:26  sar
 * Modified the do_query routine to not do an ipc_rcv call if
 * we are acting in an async mode.
 *
 * Revision 5.1  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 1.5  1994/05/13  21:47:23  sar
 * added some type casts to make the compilers happy.
 *
 * Revision 1.4  1994/05/12  17:16:29  sar
 * removed master from the sa_decode_asn1 arguments list
 * added snmp_version to the message header
 *
 * Revision 1.3  1994/05/10  20:16:03  sar
 * Removed party.h from the include list
 *
 * Revision 1.2  1994/05/06  20:31:27  sar
 * Modified use of demux to use both an object and a string all the time.
 * added some cleaning to try to avoid leaking memory.
 *
 * Revision 1.1  1994/05/05  19:59:41  sar
 * Initial revision
 * */

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <encode.h>
#include <decode.h>
#include <snmpdefs.h>
#include <subagent.h>
#include <buildpkt.h>
#include <objectid.h>

#if !defined(SNMP_SYSUPTIME)
#define SNMP_SYSUPTIME 0
#endif

#if (INSTALL_ENVOY_EPI_SUBAGENT_SCHEME)

#define CLEAN_DEMUX(D) \
  (EBufferClean(&((D)->string)), Clean_Obj_ID(&((D)->object)))

/****************************************************************************
NAME:  SA_Encode_Hdr

PURPOSE:  Encode the header of a sub agent message

PARAMETERS:
        OCTET_T   *     Pointer to the space for the header, it better
                        be long enough.
        SA_HEADER_T *   Information about the header block.

RETURNS:  0     Success
          1     Failure (bad argument in hdr_blk).

          On return the information in the header block will be put
          into the header string.

****************************************************************************/

INT_32_T
  SA_Encode_Hdr(OCTET_T     *header,
                SA_HEADER_T *hdr_blk)
{
OCTET_T opcode1;

/* we use opcode1 several times, make a local copy */
opcode1 = hdr_blk->opcode1;

/* check the arguments */
/* the version must be 1 and opcode1 must be in the correct range */
if ((hdr_blk->version != 1) || (opcode1 <= 0) || (opcode1 > SA_MAX_OPCODE))
    return(1);

/* If this is a query range check opcode2 */
if ((opcode1 == SA_QUERY_REQUEST) || (opcode1 == SA_QUERY_REPLY))
    if ((hdr_blk->opcode2 <= 0) || (hdr_blk->opcode2 > SA_QUERY_MAX))
        return(1);

*header++ = hdr_blk->version;
*header++ = hdr_blk->opcode1;
*header++ = hdr_blk->opcode2;
*header++ = hdr_blk->sa_error;
*header++ = hdr_blk->encoding;
*header++ = hdr_blk->snmp_version;

/* request id,  2 bytes */
*header++ = (OCTET_T)((hdr_blk->requestid >> 8) & 0xFF);
*header++ = (OCTET_T)(hdr_blk->requestid & 0xFF);

/* sysuptime, 4 bytes */
*header++ = (OCTET_T)((hdr_blk->sysuptime >> 24) & 0xFF);
*header++ = (OCTET_T)((hdr_blk->sysuptime >> 16) & 0xFF);
*header++ = (OCTET_T)((hdr_blk->sysuptime >>  8) & 0xFF);
*header++ = (OCTET_T)(hdr_blk->sysuptime & 0xFF);

/* index, 4 bytes */
*header++ = (OCTET_T)((hdr_blk->index >> 24) & 0xFF);
*header++ = (OCTET_T)((hdr_blk->index >> 16) & 0xFF);
*header++ = (OCTET_T)((hdr_blk->index >>  8) & 0xFF);
*header++ = (OCTET_T)(hdr_blk->index & 0xFF);

/* snmp error status, 4 bytes */
*header++ = (OCTET_T)((hdr_blk->snmp_error >> 24) & 0xFF);
*header++ = (OCTET_T)((hdr_blk->snmp_error >> 16) & 0xFF);
*header++ = (OCTET_T)((hdr_blk->snmp_error >>  8) & 0xFF);
*header   = (OCTET_T)(hdr_blk->snmp_error & 0xFF);
    
return(0);
}

/****************************************************************************
NAME:  SA_Decode_Hdr

PURPOSE:  Decode the header of a sub agent message

PARAMETERS:
        OCTET_T   *     Pointer to the message, it must be at least long
                        enought to hold a header.  (That is the calling
                        routine must check it's length).
        SA_HEADER_T *   Information about the header block.

RETURNS:  0     Success
          SA_UNKNOWN_VERSION  Failures
          SA_UNKNOWN_OPCODE1
          SA_UNKNOWN_OPCODE2

          On return the information in the header string will be put
          into the header block.

****************************************************************************/

INT_32_T
  SA_Decode_Hdr(OCTET_T     *header,
                SA_HEADER_T *hdr_blk)
{
OCTET_T opcode1;

hdr_blk->version  = *header++;
hdr_blk->opcode1  = *header++;
hdr_blk->opcode2  = *header++;
hdr_blk->sa_error = *header++;
hdr_blk->encoding = *header++;
hdr_blk->snmp_version = *header++;

/* request id, 2 bytes */
hdr_blk->requestid = (header[0] << 8) | header[1];
header += 2;

/* sysuptime, 4 bytes */
hdr_blk->sysuptime = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) |
                     header[3];
header += 4;

/* index, 4 bytes */
hdr_blk->index = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) |
                  header[3];
header += 4;
/* snmp error status, 4 bytes */
hdr_blk->snmp_error = (header[0] << 24) | (header[1] << 16) |
                      (header[2] << 8) | header[3];

/* check the information */
/* the version must be 1 and opcode1 must be in the correct range */
if (hdr_blk->version != 1)
    return(SA_UNKNOWN_VERSION);

opcode1 = hdr_blk->opcode1;
if ((opcode1 <= 0) || (opcode1 > SA_MAX_OPCODE))
    return(SA_UNKNOWN_OPCODE1);

/* If this is a query range check opcode2 */
if ((opcode1 == SA_QUERY_REQUEST) || (opcode1 == SA_QUERY_REPLY))
    if ((hdr_blk->opcode2 <= 0) || (hdr_blk->opcode2 > SA_QUERY_MAX))
        return(SA_UNKNOWN_OPCODE2);

return(0);
}

/****************************************************************************
NAME:  SA_Encode_ASN1

PURPOSE:  Encode a sub agent message in ASN1 format

PARAMETERS:
        VBL_T   *       List of vbs to be manipulated.
        OCTET_T   *     Header to add to the front of the message
        INT_32_T        Length of the header in bytes
        SA_HEADER_T *   Information about the header block.
        SA_DEMUX_T *    The community string or
                        localentity name and localtimeoid
                        for demuxing in the sub agent.
        EBUFFER_T *     A buffer structure to receive the encoded packet

RETURNS:  0             Success
          1             Illegal or unknown argument
          2             Not enough buffer space or unable to allocate buffer
                        space

         On a sucessful return, the ebuffer passed as a parameter will
         contain the encoded packet.  If necessary, space will be malloc-ed.

****************************************************************************/

INT_32_T
  SA_Encode_ASN1(VBL_T       *vblist,
                 OCTET_T     *header,
                 ALENGTH_T    headerlen,
                 SA_HEADER_T *hdr_blk,
                 SA_DEMUX_T  *demuxer,
                 EBUFFER_T   *ebuf)
{
INT_32_T  i, indx, newindx = 0, queryflg = 0;
VB_T  *vbp;
ALENGTH_T gen_size, vb_size, list_size, seq_size, msg_size, demux_str_size;
ALENGTH_T demux_oid_size;
OCTET_T opcode1;

/* first we need to work out how long the message will be
   We figure out what the vb list will look like from opcode1 and size
   the vb sequence accordingly */
list_size = 0;
opcode1 = hdr_blk->opcode1;
if ((opcode1 == SA_QUERY_REQUEST) || (opcode1 == SA_QUERY_REPLY) ||
    (opcode1 == SA_TRAP_REQUEST) || (opcode1 == SA_TRAP_REPLY)) {
    queryflg = 1;
    /* it is a query/trap message */
    /* this loop is done differently then the others so we can
       translate the old index (indx) into an index for the new list */
    for (i = 0, vbp = vblist->vblist, newindx = 0, indx = hdr_blk->index;
         i < vblist->vbl_count;
         i++, vbp++) {
        /* is this a vb we want to send? */
        if (!(vbp->vb_flags & VFLAG_SA_SENDIT))
            continue; /* nope */
        if (i < indx)
            newindx++;
        /* The tags on each item are always 1 octet long */
        gen_size = A_SizeOfObjectId(&(vbp->vb_obj_id));
        vb_size = 1 + A_SizeOfLength(gen_size) + gen_size;

        switch (vbp->vb_data_flags_n_type) {
            case VT_NUMBER:
                 gen_size = A_SizeOfInt(vbp->value_u.v_number);
                 break;
            case VT_COUNTER:
            case VT_GAUGE:
            case VT_TIMETICKS:
            case VT_UINTEGER32:
                 gen_size = A_SizeOfUnsignedInt(vbp->value_u.v_counter);
                 break;
            case VT_STRING:
            case VT_OPAQUE:
                 gen_size =
                   A_SizeOfOctetString(EBufferUsed(&(vbp->value_u.v_string)));
                 break;
            case VT_OBJECT:
                 gen_size = A_SizeOfObjectId(&(vbp->value_u.v_object));
                 break;
            case VT_NOSUCHINS:
            case VT_NOSUCHOBJ:
            case VT_ENDOFMIB:
            case VT_EMPTY:
            case 0:
                 gen_size = 0;
                 break;
            case VT_IPADDRESS:
                 gen_size = 4;
                 break;
/* We only do include the following case if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
            case VT_COUNTER64:
                 gen_size = A_SizeOfUnsignedInt64(&(vbp->value_u.v_counter64));
                 break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
            default:
                 break;
            }
        vbp->vb_data_length = gen_size;
        vb_size += 1 + A_SizeOfLength(gen_size) + gen_size;

        gen_size = A_SizeOfUnsignedInt((UINT_32_T)vbp->vb_flags);
        vb_size += 1 + A_SizeOfLength(gen_size) + gen_size;

        gen_size = A_SizeOfUnsignedInt((UINT_32_T)vbp->vb_priv);
        vb_size += 1 + A_SizeOfLength(gen_size) + gen_size;

        vbp->vb_seq_size = vb_size;
        list_size += 1 + A_SizeOfLength(vb_size) + vb_size;
        }
    }
else {
    /* it isn't a query/trap message, must be a control message */
    for (i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++) {
        /* is this a vb we want to send? */
        if (!(vbp->vb_flags & VFLAG_SA_SENDIT))
            continue; /* nope */

        /* The tags on each item are always 1 octet long */
        gen_size = A_SizeOfObjectId(&(vbp->vb_obj_id));
        vb_size = 1 + A_SizeOfLength(gen_size) + gen_size;

        gen_size  = A_SizeOfUnsignedInt((UINT_32_T)vbp->vb_data_flags_n_type);
        vb_size += 1 + A_SizeOfLength(gen_size) + gen_size;

        gen_size  = A_SizeOfUnsignedInt((UINT_32_T)vbp->vb_access);
        vb_size += 1 + A_SizeOfLength(gen_size) + gen_size;
        
        gen_size = A_SizeOfUnsignedInt(vbp->sa_flags);
        vb_size += 1 + A_SizeOfLength(gen_size) + gen_size;

        gen_size = A_SizeOfUnsignedInt(vbp->value_u.v_uinteger32);
        vb_size += 1 + A_SizeOfLength(gen_size) + gen_size;

        vbp->vb_seq_size = vb_size;
        list_size += 1 + A_SizeOfLength(vb_size) + vb_size;
        }
    }

/* Figure out what type of demuxer we have and determine it's size */
demux_str_size = A_SizeOfOctetString(EBufferUsed(&demuxer->string));
demux_oid_size = A_SizeOfObjectId(&demuxer->object);

/* total up the vb list with the rest of the message */
/* request id, sysuptime, op type, index, and error status all have
   tag + length = 2, the demux str, demux oid and vb list have a tag
   length of 1 but the length length may be greater than 1 */

seq_size = 1 + A_SizeOfLength(demux_str_size) + demux_str_size
         + 1 + A_SizeOfLength(demux_oid_size) + demux_oid_size
         + 1 + A_SizeOfLength(list_size) + list_size;

/* and now we can get to the total message size */
msg_size = headerlen    /* size of the incoming header */
         + 1            /* Tag for the asn1 implicit sequence */
         + A_SizeOfLength(seq_size)
         + seq_size;

/* Do we have or can we allocate enough space? */
if (ebuf->start_bp == 0) {
    /* The user didn't supply any space so we need to allocate some */
    OCTET_T *buffp;
    if ((buffp = (OCTET_T *)SNMP_memory_alloc(msg_size)) == 0)
        return(2);
    EBufferSetup(BFL_IS_DYNAMIC, ebuf, buffp, msg_size);
    }
else { /* Make sure there is enough space in the buffer the user gave us */
    if (EBufferRemaining(ebuf) < msg_size)
        return(2);
    }

/* copy the header we were passed into the buffer */
MEMCPY(ebuf->next_bp, header, (int)headerlen);
ebuf->next_bp = ebuf->next_bp + headerlen;
ebuf->remaining -= headerlen;

/* now start encoding the asn1 section of the message */
/* first is the implicit sequence header */
A_EncodeType(1, A_CONTEXT | A_CONSTRUCTOR, A_EncodeHelper, (OCTET_T *)ebuf);
A_EncodeLength(seq_size, A_EncodeHelper, (OCTET_T *)ebuf);

/* next is the demux field */
A_EncodeOctetString(VT_STRING & ~A_IDCF_MASK, VT_STRING & A_IDCF_MASK,
                    EBufferStart(&demuxer->string),
                    EBufferUsed(&demuxer->string),
                    A_EncodeHelper, (OCTET_T *)ebuf);
A_EncodeObjectId(A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE,
                 &demuxer->object, A_EncodeHelper, (OCTET_T *)ebuf);

/* Then comes the vb list */
/* Sequence header for the whole list */
A_EncodeType(A_SEQUENCE, A_UNIVERSAL | A_CONSTRUCTOR,
             A_EncodeHelper, (OCTET_T *)ebuf);
A_EncodeLength(list_size, A_EncodeHelper, (OCTET_T *)ebuf);

/* and finally the vbs, again we need to check if it is a ctl message
   to decide what to output */
if (queryflg)
    /* it is a query message, so the output is
       sequence, oid, value, vb flags, vb priv tag */
    for(i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++) {
        /* is this a vb we want to send? */
        if (!(vbp->vb_flags & VFLAG_SA_SENDIT))
            continue; /* nope */
        A_EncodeType(A_SEQUENCE, A_UNIVERSAL | A_CONSTRUCTOR,
                     A_EncodeHelper, (OCTET_T *)ebuf);
        A_EncodeLength(vbp->vb_seq_size, A_EncodeHelper, (OCTET_T *)ebuf);
        A_EncodeObjectId(A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE,
                         &(vbp->vb_obj_id), A_EncodeHelper, (OCTET_T *)ebuf);

        switch (vbp->vb_data_flags_n_type) {
            case VT_NUMBER:
                A_EncodeInt(VT_NUMBER & ~A_IDCF_MASK,
                            VT_NUMBER & A_IDCF_MASK,
                            vbp->value_u.v_number,
                            A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_COUNTER:
                A_EncodeUnsignedInt(VT_COUNTER & ~A_IDCF_MASK,
                                    VT_COUNTER & A_IDCF_MASK,
                                    vbp->value_u.v_counter,
                                    A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_GAUGE:
                A_EncodeUnsignedInt(VT_GAUGE & ~A_IDCF_MASK,
                                    VT_GAUGE & A_IDCF_MASK,
                                    vbp->value_u.v_gauge,
                                    A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_TIMETICKS:
                A_EncodeUnsignedInt(VT_TIMETICKS & ~A_IDCF_MASK,
                                    VT_TIMETICKS & A_IDCF_MASK,
                                    vbp->value_u.v_timeticks,
                                    A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_UINTEGER32:
                A_EncodeUnsignedInt(VT_UINTEGER32 & ~A_IDCF_MASK,
                                    VT_UINTEGER32 & A_IDCF_MASK,
                                    vbp->value_u.v_timeticks,
                                    A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_STRING:
                A_EncodeOctetString(VT_STRING & ~A_IDCF_MASK,
                                    VT_STRING & A_IDCF_MASK,
                                    vbp->value_u.v_string.start_bp,
                                    EBufferUsed(&(vbp->value_u.v_string)),
                                    A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_OPAQUE:
                A_EncodeOctetString(VT_OPAQUE & ~A_IDCF_MASK,
                                    VT_OPAQUE & A_IDCF_MASK,
                                    vbp->value_u.v_string.start_bp,
                                    EBufferUsed(&(vbp->value_u.v_string)),
                                    A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_OBJECT:
                A_EncodeObjectId(A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE,
                                 &(vbp->value_u.v_object),
                                 A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_EMPTY:
            case 0:
                A_EncodeType(VT_EMPTY & ~A_IDCF_MASK,
                             VT_EMPTY & A_IDCF_MASK,
                             A_EncodeHelper, (OCTET_T *)ebuf);
                A_EncodeLength(0, A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_IPADDRESS:
                A_EncodeOctetString(VT_IPADDRESS & ~A_IDCF_MASK,
                                    VT_IPADDRESS & A_IDCF_MASK,
                                    vbp->value_u.v_network_address,
                                    4, A_EncodeHelper, (OCTET_T *)ebuf);
                break;
            case VT_NOSUCHOBJ:
            case VT_NOSUCHINS:
            case VT_ENDOFMIB:
                A_EncodeType(vbp->vb_data_flags_n_type & ~A_IDCF_MASK,
                            (OCTET_T)(vbp->vb_data_flags_n_type & A_IDCF_MASK),
                             A_EncodeHelper, (OCTET_T *)ebuf);
                A_EncodeLength(0, A_EncodeHelper, (OCTET_T *)ebuf);
                break;
/* We only do include the following case if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
            case VT_COUNTER64:
                A_EncodeUnsignedInt64(VT_COUNTER64 & ~A_IDCF_MASK,
                                      VT_COUNTER64 & A_IDCF_MASK,
                                      &(vbp->value_u.v_counter64),
                                      A_EncodeHelper, (OCTET_T *)ebuf);
                break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
            default:
                break;
            }
        A_EncodeUnsignedInt(VT_UINTEGER32 & ~A_IDCF_MASK,
                            VT_UINTEGER32 & A_IDCF_MASK,
                            vbp->vb_flags, A_EncodeHelper,
                            (OCTET_T *)ebuf);
        /* encode the vb_priv_tag, the vb_priv and vb_free_priv must
           already have been translated to a sub agent tag */
        A_EncodeUnsignedInt(VT_UINTEGER32 & ~A_IDCF_MASK,
                            VT_UINTEGER32 & A_IDCF_MASK,
                            (UINT_32_T)vbp->vb_priv, A_EncodeHelper,
                            (OCTET_T *)ebuf);
        }
else
    /* it is a control message, so the output is
       sequence, oid, type, access, flags, and errors */
    for(i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++) {
        /* is this a vb we want to send? */
        if (!(vbp->vb_flags & VFLAG_SA_SENDIT))
            continue; /* nope */
        A_EncodeType(A_SEQUENCE, A_UNIVERSAL | A_CONSTRUCTOR,
                     A_EncodeHelper, (OCTET_T *)ebuf);
        A_EncodeLength(vbp->vb_seq_size, A_EncodeHelper, (OCTET_T *)ebuf);
        A_EncodeObjectId(A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE,
                         &(vbp->vb_obj_id), A_EncodeHelper, (OCTET_T *)ebuf);
        A_EncodeUnsignedInt(VT_UINTEGER32 & ~A_IDCF_MASK,
                            VT_UINTEGER32 & A_IDCF_MASK,
                            vbp->vb_data_flags_n_type,
                            A_EncodeHelper, (OCTET_T *)ebuf);
        A_EncodeUnsignedInt(VT_UINTEGER32 & ~A_IDCF_MASK,
                            VT_UINTEGER32 & A_IDCF_MASK,
                            vbp->vb_access,
                            A_EncodeHelper, (OCTET_T *)ebuf);
        A_EncodeUnsignedInt(VT_UINTEGER32 & ~A_IDCF_MASK,
                            VT_UINTEGER32 & A_IDCF_MASK,
                            vbp->sa_flags, A_EncodeHelper,
                            (OCTET_T *)ebuf);
        A_EncodeUnsignedInt(VT_UINTEGER32 & ~A_IDCF_MASK,
                            VT_UINTEGER32 & A_IDCF_MASK,
                            vbp->value_u.v_uinteger32, A_EncodeHelper,
                            (OCTET_T *)ebuf);
        }

return(0);
}

/****************************************************************************
NAME:  SA_Decode_ASN1

PURPOSE:  Decode a sub agent message in ASN1 format

PARAMETERS:
        LCL_FILE  *     A local stream structure continaing the msg to decode
        VBL_T     *     Space to put the returned list of vbs, the vbl_t
                        must exist space will be allocated for the vbs.
        SA_HEADER_T *   Structure to contain information about the header
        SA_DEMUX_T *    The community string or
                        localentity name and localtimeoid
                        for demuxing in the sub agent.

RETURNS:  0     Success
          SA_DECODE_FAILURE     We attempt to seperate the failures into
          SA_GEN_ERROR          a decode error and memory allocation failures
                                but some of the decode routines will flag
                                an alloc error as a deocde error.
          
         On a sucessful return, the VBL_T will contain a list of the vbs
         that were included in this packet.  If necessary, space will be
         malloc-ed.

****************************************************************************/
INT_32_T
  SA_Decode_ASN1(LCL_FILE    *in_stream,
                 VBL_T       *vblist,
                 SA_HEADER_T *hdr_blk,
                 SA_DEMUX_T  *demuxer)
{
ATVALUE_T       type;
OCTET_T         flags, opcode1;
ALENGTH_T       msgleng, leng, used, alength;
int             asn1err = 0, tell_place, start_place, items, i;
VB_T            *vbp;

/* Set all the return arguments to known numbers in case of errors */
vblist->vbl_count = 0;
vblist->vblp = 0;
vblist->vblist = 0;

/* Decode the top-level message sequence... */
flags    = A_DecodeTypeClass(in_stream);
type     = A_DecodeTypeValue(in_stream, &asn1err);
msgleng  = A_DecodeLength(in_stream, &asn1err);
leng     = Lcl_Size(in_stream);

/* Check for decoding errors and check the length of the message against
   the length given in the asn.1 wrapper and check the flags and type */
if ((asn1err) || (msgleng > leng) ||
    ((flags | type) != (A_CONTEXT | A_CONSTRUCTOR | 1)))
      return (SA_DECODE_FAILURE);

/* If necessary adjust the local stream size to be that of the asn.1 length */
if (msgleng < leng)
    Lcl_Resize(in_stream, msgleng, 1);

/* next are the demux fields */
flags    = A_DecodeTypeClass(in_stream);
type     = A_DecodeTypeValue(in_stream, &asn1err);
leng     = A_DecodeLength(in_stream, &asn1err);
if (asn1err)
    return(SA_DECODE_FAILURE);
if ((flags | type) != VT_STRING)
    return(SA_DECODE_FAILURE);
A_DecodeOctetStringData(in_stream, leng, &demuxer->string, &asn1err);
if (asn1err)
    return(SA_DECODE_FAILURE);

flags    = A_DecodeTypeClass(in_stream);
type     = A_DecodeTypeValue(in_stream, &asn1err);
leng     = A_DecodeLength(in_stream, &asn1err);
if (asn1err)
    return(SA_DECODE_FAILURE);

if ((flags | type) != VT_OBJECT)
    return(SA_DECODE_FAILURE);
A_DecodeObjectIdData(in_stream, leng, &demuxer->object, &asn1err);
if (asn1err)
    return(SA_DECODE_FAILURE);

/* Now we have a sequence of sequences, pull off and check type and length */
flags    = A_DecodeTypeClass(in_stream);
type     = A_DecodeTypeValue(in_stream, &asn1err);
leng     = A_DecodeLength(in_stream, &asn1err);
if (asn1err || (leng != Lcl_Size(in_stream)) || 
    ((flags | type) != (A_SEQUENCE | A_UNIVERSAL | A_CONSTRUCTOR)))
    return(SA_DECODE_FAILURE);
  
/* now we count the vbs */
/* figure out where we are so we can return here after we finish counting */
tell_place = Lcl_Tell(in_stream);
for (items = 0, used = 0; used < leng; items++) {
    start_place = Lcl_Tell(in_stream);
    if (Lcl_Eof(in_stream))
        break;

    /* Skip over the VarBind sequence */
    (void) A_DecodeTypeValue(in_stream, &asn1err);
    alength = A_DecodeLength(in_stream, &asn1err);
    if ((asn1err) || (Lcl_Seek(in_stream, (int)alength, 1) == -1))
        return(SA_DECODE_FAILURE);
    used = used + (ALENGTH_T)(Lcl_Tell(in_stream)) - (ALENGTH_T)start_place;
    }

/* return to the start of the vb list */
 (void) Lcl_Seek(in_stream, tell_place, 0);
  
/* Somebody is sending messages with no vbs */
if (items == 0)
    return(0);

/* we know haw many vbs we have, allocate space for them and set up vblist */
if ((vbp = VarBindList_Allocate(items)) == 0)
    return(SA_GEN_ERROR);
vblist->vbl_count = items;
vblist->vblist = vbp;

/* we have the vblist, start reading the vbs, decide what type of
   msg this is */
opcode1 = hdr_blk->opcode1;
if ((opcode1 == SA_QUERY_REQUEST) || (opcode1 == SA_QUERY_REPLY) ||
    (opcode1 == SA_TRAP_REQUEST) || (opcode1 == SA_TRAP_REPLY))
    /* it is a query message so the seq is: oid, value, vb_flags, priv tag */
    for (i = 0; i < items; vbp++) {
        /* seq tag and length */
        flags = A_DecodeTypeClass(in_stream);
        type  = A_DecodeTypeValue(in_stream, &asn1err);
        leng  = A_DecodeLength(in_stream, &asn1err);
            if (asn1err || 
                ((flags | type) != (A_SEQUENCE | A_UNIVERSAL | A_CONSTRUCTOR)))
                goto cleanup;

        /* objid */
        A_DecodeObjectId(in_stream, &(vbp->vb_obj_id), &asn1err);
        if (asn1err) goto cleanup;

        /* We increment the index (i) here so we can keep track of which oids 
           have already been filled in, in case we need to clean the oids */
        i++;

        flags = A_DecodeTypeClass(in_stream);
        type  = A_DecodeTypeValue(in_stream, &asn1err);
        leng  = A_DecodeLength(in_stream, &asn1err);
        if (asn1err) goto cleanup;
            
        vbp->vb_data_length = leng;
        vbp->vb_data_flags_n_type = flags | (OCTET_T)type;

        switch (vbp->vb_data_flags_n_type) {
            case VT_NUMBER:
                vbp->value_u.v_number =
                  A_DecodeIntegerData(in_stream, leng, &asn1err);
                break;
            case VT_COUNTER:
            case VT_GAUGE:
            case VT_TIMETICKS:
            case VT_UINTEGER32:
                vbp->value_u.v_counter =
                  (UINT_32_T)A_DecodeIntegerData(in_stream, leng, &asn1err);
                break;
            case VT_STRING:
            case VT_OPAQUE:
                A_DecodeOctetStringData(in_stream, leng,
                                        &(vbp->value_u.v_string), &asn1err);
                break;
            case VT_OBJECT:
                A_DecodeObjectIdData(in_stream, leng,
                                     &(vbp->value_u.v_object), &asn1err);
                break;
            case VT_NOSUCHOBJ:
            case VT_NOSUCHINS:
            case VT_ENDOFMIB:
            case VT_EMPTY:
            case 0:
                /* Empty has no contents to be decoded */
                break;
            case VT_IPADDRESS:
                {
                EBUFFER_T ipbuff;
                unsigned int iplen;

                MEMSET(vbp->value_u.v_network_address, 0, 4);
                EBufferInitialize(&ipbuff);
                A_DecodeOctetStringData(in_stream, leng, &ipbuff, &asn1err);
                iplen = min(4, EBufferUsed(&ipbuff));
                if (iplen) {
                    MEMCPY(vbp->value_u.v_network_address, ipbuff.start_bp,
                           iplen);
                    EBufferClean(&ipbuff);
                    }
                }
                break;
/* We only do include the following case if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
            case VT_COUNTER64:
                A_DecodeInteger64Data(in_stream, leng,
                                      &(vbp->value_u.v_counter64), &asn1err);
                break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
            default:
                goto cleanup;
            }

        if (asn1err) goto cleanup;

        /* unsigned int - vb_flags */
        vbp->vb_flags = (OCTET_T)(A_DecodeInteger(in_stream, &asn1err) & 0xFF);
        if (asn1err) goto cleanup;

        /* unsigned int - vb_priv_tag, pull out the tag, it will need to
           be translated later, the decoder will not handle it */
        vbp->vb_priv = (PTR_T)A_DecodeInteger(in_stream, &asn1err);
        if (asn1err) goto cleanup;
        }
else 
    /* it's a control msg, the seq is: oid, type, access, flags, errors */
    for (i = 0; i < items; vbp++) {
        /* set the vb flags to sendit as for control messages we always
           send all of them */
        vbp->vb_flags = VFLAG_SA_SENDIT;

        /* seq tag and length */
        flags = A_DecodeTypeClass(in_stream);
        type  = A_DecodeTypeValue(in_stream, &asn1err);
        leng  = A_DecodeLength(in_stream, &asn1err);
            if (asn1err || 
                ((flags | type) != (A_SEQUENCE | A_UNIVERSAL | A_CONSTRUCTOR)))
                goto cleanup;

        /* objid */
        A_DecodeObjectId(in_stream, &(vbp->vb_obj_id), &asn1err);
        if (asn1err) goto cleanup;

        /* We increment the index (i) here so we can keep track of which oids 
           have already been filled in, in case we need to clean the oids */
        i++;

        /* unsigned int - type */
        vbp->vb_data_flags_n_type = 
            (OCTET_T)(A_DecodeInteger(in_stream, &asn1err) & 0xFF);
        if (asn1err) goto cleanup;

        vbp->vb_access = 
            (OCTET_T)(A_DecodeInteger(in_stream, &asn1err) & 0xFF);
        if (asn1err) goto cleanup;

        /* unsigned int - flags */
        vbp->sa_flags = (UINT_32_T)A_DecodeInteger(in_stream, &asn1err);
        if (asn1err) goto cleanup;

        /* unsigned int - errors */
        vbp->value_u.v_uinteger32 = (UINT_32_T)A_DecodeInteger(in_stream,
                                                               &asn1err);
        if (asn1err) goto cleanup;
        }

return(0);

/* we will get here if we had an error decoding the packet and need to clean
   up the space we allocated for the oid structure */
cleanup:
/* set the count to the number of vbs we have already filled in */
vblist->vbl_count = i;
Clean_vb_list(vblist);

return(SA_DECODE_FAILURE);
}

/****************************************************************************
NAME:  SA_Encode

PURPOSE:  Encode a sub agent message

PARAMETERS:
        VBL_T   *       List of vbs to be manipulated.
                        In a control message they are nodes to be added/removed
                        In a query message they are vbs to be gotten/set.
        SA_HEADER_T *   Structure containing the header info for this msg
        SA_DEMUX_T *    The community string or
                        localentity name and localtimeoid
                        for demuxing in the sub agent.
        EBUFFER_T *     A buffer structure to contain the encoded packet

RETURNS:  0             Success
          1             Illegal or unknown argument
          2             Not enough buffer space or unable to allocate buffer
                        space

         On a sucessful return, the ebuffer passed as a parameter will
         contain the encoded packet.  If necessary, space will be malloc-ed.

****************************************************************************/
INT_32_T
  SA_Encode(VBL_T       *vblist,
            SA_HEADER_T *hdr_blk,
            SA_DEMUX_T  *demuxer,
            EBUFFER_T   *ebuf)
{
OCTET_T header[SA_HEADERLEN];

/* set up the header */
if (SA_Encode_Hdr(header, hdr_blk))
    return(1);

/* if necessary range check the index */
if (hdr_blk->index < 0)
    return(1);
if ((hdr_blk->opcode1 == SA_QUERY_REQUEST) ||
    (hdr_blk->opcode1 == SA_QUERY_REPLY))
    if (hdr_blk->index > vblist->vbl_count)
        return(1);

/* in any case the demuxer must point to something other than null,
   the object may be empty but it must exist */
if (demuxer == 0)
    return(1);

/* Call the encoding routine specified by the encoding argument */
switch(hdr_blk->encoding){
    case SA_ENCODING_ASN1:
        return(SA_Encode_ASN1(vblist, header, SA_HEADERLEN, hdr_blk,
                              demuxer, ebuf));
    default:
        return(1);
    }
}

/****************************************************************************
NAME:  SA_Decode

PURPOSE:  Decode a sub agent message

PARAMETERS:
        OCTET_T   *     Byte string for message
        ALENGTH_T       Length of the message
        VBL_T     *     Space to put the returned list of vbs, the vbl_t
                        must exist space will be allocated for the vbs.
        SA_HEADER_T *   Structure to hold header information
        SA_DEMUX_T *    The community string or
                        localentity name and localtimeoid
                        for demuxing in the sub agent.

RETURNS:  0     Success
          SA_UNKNOWN_VERSION
          SA_UNKNOWN_OPCODE1
          SA_UNKNOWN_OPCODE2
          SA_UNKNOWN_ENCODING
          SA_DECODE_FAILURE     We attempt to seperate the failures into
          SA_GEN_ERROR          a decode error and memory allocation failures
                                but some of the decode routines will flag
                                an alloc error as a deocde error.
          
         On a sucessful return, the VBL_T will contain a list of the vbs
         that were included in this packet.  If necessary, space will be
         malloc-ed.

****************************************************************************/
INT_32_T
  SA_Decode(OCTET_T     *msg,
            ALENGTH_T    msgl,
            VBL_T       *vblist,
            SA_HEADER_T *hdr_blk,
            SA_DEMUX_T  *demuxer)
{
LCL_FILE *in_stream, in_msg_stream;
INT_32_T errflag;

/* make sure the message is long enough to have a header, then decode it
   and start checking it */
if (msgl < SA_HEADERLEN)
    return(SA_SHORT_MSG);
errflag = SA_Decode_Hdr(msg, hdr_blk);
if (errflag)
    return(errflag);

/* in some cases the message may only be the unencoded header,
   in which case we are done */
if (msgl == SA_HEADERLEN)
    return(0);

/* set up the message as a local stream */
in_stream = Lcl_Open(&in_msg_stream, msg + SA_HEADERLEN, msgl - SA_HEADERLEN);
if (in_stream == 0)
    return(SA_GEN_ERROR);

/* Attempt to decode the data section of the message */
switch (hdr_blk->encoding) {
    case SA_ENCODING_ASN1:
        return(SA_Decode_ASN1(in_stream, vblist, hdr_blk, demuxer));
    default:
        return(SA_UNKNOWN_ENCODING);
    }
}

#endif /* #if (INSTALL_ENVOY_MASTER_AGENT || INSTALL_ENVOY_SUB_AGENT) */

