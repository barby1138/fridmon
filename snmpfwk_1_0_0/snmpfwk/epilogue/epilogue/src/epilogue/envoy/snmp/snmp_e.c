/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/snmp_e.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 *  Copyright 1988-1997 Epilogue Technology Corporation.
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
 * $Log: snmp_e.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:04  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:49:21  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.3  1996/01/10  16:34:47  sar
 * In snmp_encode_packet figure out what the packet size will be
 * before calling snmp_user_get_encode_buffer
 * Usec updates - mostly doing the authentication if needed.
 *
 * Revision 6.2  1995/11/01  00:46:48  sar
 * added ifdefs to the version switch for pdu types
 * arranged to leave out the v1 trap code if we aren't compiling v1
 *
 * Revision 6.1  1995/10/20  23:02:48  sar
 * removed no_pp stuff & v2 party code
 *
 * Revision 6.0  1995/05/31  21:48:16  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/08  20:32:04  sar
 * Removed a garbage character.
 *
 * Revision 5.3  1995/03/21  00:30:26  sar
 * Switched to using for & loc to name parties
 * Removed the proxy code.
 *
 * Revision 5.2  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.1  1994/08/03  19:32:32  sar
 * We were using SizeOfInt() on the time ticks field when calculting the
 * size of a trap packet, we now use SizeOfUnsignedInt.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.13  1993/06/23  00:04:16  sar
 * Made ENABLE_PROXY be just PROXY in the install defs
 *
 * Revision 3.12  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.11  1993/05/19  20:26:47  sar
 * Tidied up types to make compilers happy (mostly int and unsigned int to
 * ALENGTH_T).
 *
 * Revision 3.10  1993/05/03  17:16:12  sar
 * Added routines for local time, removed {party context} oids from packet
 * structure, added asn1len to {party context} structures and
 * SNMP_{Party Context}_Name where needed to lookup the name for a given
 * {Party Context}.
 *
 * Revision 3.9  1993/04/30  22:54:08  sar
 * Added mechanism for minimial proxies and coarse grained locks.  The
 * latter required the addition of routines to get the name of a leaf given
 * a pointer to the leaf.
 *
 * Revision 3.8  1993/04/26  20:37:09  sar
 * Added ifdefs to allow clean makes of version 1 and 2, added bit strings,
 * arrange for deletion of acls with the party or context they refer to
 * is deleted.
 *
 * Revision 3.7  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.6  1993/04/19  18:28:34  sar
 * cleanup, mostly some correcting some non-ansi misspellings and adding
 * static defs.
 *
 * Revision 3.5  1993/03/25  21:18:09  sar
 * Added routines to encode/decode 64 bit integers/counter64s
 *
 * Revision 3.4  1993/03/20  00:25:43  sar
 * Removed some unused break statements.
 *
 * Revision 3.3  1993/03/05  03:58:02  dab
 * fixed up types
 *
 * Revision 3.2  1993/02/24  01:02:41  sar
 * Changed many call to use OIDC_T/lenght pairs instead of OBJ_ID_T as now
 * we don't need to specifically allocate the OBJ_ID_T.
 * Removed the name objid from the PARTYLEAF_T as the name may be determined from
 * the tree itself.  For efficiency purposes put the dest/src party names into
 * the pkt so we don't have to look them up frequently.
 *
 * Revision 3.1  1993/02/17  21:13:28  sar
 * Removed #define then and uses of it.
 * Moved code around to be more in line with it's use.
 * Modified the high level routines to check for v1 or v2 and call the
 * correct lower level routines.  (Yes it's has compile time flags).
 * Added bufsize_for_party_packet, and encode_party_packet.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.106  92/01/30  14:03:40  dab
 * No longer have to define the macro SNMP_user_get_encode_buffer.  To not
 * use this function leave macro undefined insteaed of define as 0.  This
 * gets rid of warnings from some compilers and results, potentially, in
 * slightly better code.
 * 
 * Revision 2.105  91/12/12  09:16:24  dab
 * Added macro call to SNMP_user_get_encode_buffer to allow the user to
 * supply the buffer to encode the SNMP packet into.
 * 
 * Revision 2.104  91/11/07  09:35:36  dab
 * Removed the authenticated encoding stuff.
 * 
 * Revision 2.103  91/10/30  20:42:13  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.102  91/09/18  12:32:40  dab
 * Updated to use new macros from <asn1conf.h> and <snmpconf.h>.
 * 
 * Revision 2.101  91/08/15  12:31:13  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:27  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:56  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.1   17 May 1990 16:02:44
 * On traps, the timestamp was being mis-encoded as an integer rather than
 * as an unsigned integer.
 * 
 *    Rev 2.0   31 Mar 1990 15:06:58
 * Release 2.00
 * 
 *    Rev 1.6   27 Apr 1989 15:56:08
 * Removed unused variables
 * 
 *    Rev 1.5   19 Sep 1988 17:26:46
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.4   17 Sep 1988 16:53:42
 * The agent-addr and time-stamp fields in trap pdus were being assigned
 * an incorrect ASN.1 tag.
 * 
 *    Rev 1.3   16 Sep 1988 15:23:28
 * Had "then" where "else" should have been!
 * 
 *    Rev 1.2   15 Sep 1988 18:48:06
 * Removed comment block for a non-existant procedure.
 * 
 *    Rev 1.1   14 Sep 1988 17:55:00
 * Removed redundant + signs that older C compilers complain about.
 * Also moved includes of system includes into libfunc.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:47:08
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <localio.h>
#include <buffer.h>
#include <encode.h>
#include <snmpdefs.h>
#include <snmp.h>

static  void            encode_snmp_normal_pdu __((SNMP_PKT_T *, EBUFFER_T *));
static  void            encode_var_bind_list   __((VBL_T *, EBUFFER_T *));
static  ALENGTH_T       set_vbl_sizes          __((VBL_T *));

static  ALENGTH_T       bufsize_for_normal_pkt __((SNMP_PKT_T *));
static  void            encode_snmp_common     __((EBUFFER_T *,  ALENGTH_T,
                                                   INT_32_T,  EBUFFER_T *));

#if (INSTALL_ENVOY_SNMP_VERSION_1)
static  ALENGTH_T       bufsize_for_trap_pkt   __((SNMP_PKT_T *));
static  void            encode_snmp_trap_pdu   __((SNMP_PKT_T *, EBUFFER_T *));

/****************************************************************************
        BUFSIZE_FOR_TRAP_PKT
****************************************************************************/
static ALENGTH_T
  bufsize_for_trap_pkt(SNMP_PKT_T *rp)
{
ALENGTH_T       alength;
/* In the following computations, the length of various tag and length  */
/* fields are given as constants.  This is possible because the tag     */
/* values are always low enough to fit into one octet.  And for various */
/* data types, in particular integers, the length field will always     */
/* occupy one octet.                                                    */

rp->pdu_length = 2      /* Tag and length of request_id (an integer) */
                 + A_SizeOfObjectId(&(rp->pdu.trap_pdu.enterprise_objid))
                 + 2     /* Tag and length of net_address (a string) */
                 + 4     /* Size of IP address in SMI */
                 + 2     /* Tag and length of generic_trap (an integer) */
                 + A_SizeOfInt(rp->pdu.trap_pdu.generic_trap)
                 + 2     /* Tag and length of specific_trap (an integer) */
                 + A_SizeOfInt(rp->pdu.trap_pdu.specific_trap)
                 + 2     /* Tag and length of trap_time_ticks (an uinteger) */
                 + A_SizeOfUnsignedInt(rp->pdu.trap_pdu.trap_time_ticks);

alength = set_vbl_sizes(&(rp->pdu.trap_pdu.trap_vbl));
rp->pdu_length += 1     /* Size of tag on VarBindList sequence */
                  + A_SizeOfLength(alength)
                  + alength;

alength = A_SizeOfOctetString(EBufferUsed(&(rp->community)));

rp->overall_length = 1  /* Size of tag on the PDU sequences */
                     + A_SizeOfLength(rp->pdu_length)
                     + rp->pdu_length
                     + 2    /* Tag and length of snmp_version (an integer) */
                     + A_SizeOfInt(rp->snmp_version)
                     + 1    /* Tag for the community octetstring */
                     + A_SizeOfLength(alength)
                     + alength;

alength = rp->overall_length
                    + 1 /* Size of tag for overall Message sequence */
                    + A_SizeOfLength(rp->overall_length);

return alength;
}

/****************************************************************************
        ENCODE_SNMP_TRAP_PDU
****************************************************************************/
static void
  encode_snmp_trap_pdu(SNMP_PKT_T *rp,
                       EBUFFER_T  *ebuffp)
{
/* Generate the PDU header */
A_EncodeType(rp->pdu_type, A_DEFAULT_SCOPE | A_CONSTRUCTOR,
             A_EncodeHelper, (OCTET_T *)ebuffp);
A_EncodeLength(rp->pdu_length, A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode enterprise */
A_EncodeObjectId(A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE,
                 &(rp->pdu.trap_pdu.enterprise_objid),
                 A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode agent-addr */
A_EncodeOctetString(VT_IPADDRESS & ~A_IDCF_MASK,
                    VT_IPADDRESS & A_IDCF_MASK,
                    rp->pdu.trap_pdu.net_address, 4,
                    A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode generic-trap */
A_EncodeInt(A_INTEGER, A_UNIVERSAL | A_PRIMITIVE,
            rp->pdu.trap_pdu.generic_trap,
            A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode specific-trap */
A_EncodeInt(A_INTEGER, A_UNIVERSAL | A_PRIMITIVE,
            rp->pdu.trap_pdu.specific_trap,
            A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode time-stamp */
A_EncodeUnsignedInt(VT_TIMETICKS & ~A_IDCF_MASK,
            VT_TIMETICKS & A_IDCF_MASK,
            rp->pdu.trap_pdu.trap_time_ticks,
            A_EncodeHelper, (OCTET_T *)ebuffp);

encode_var_bind_list(&(rp->pdu.trap_pdu.trap_vbl), ebuffp);
}
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_1) */

/****************************************************************************
        BUFSIZE_FOR_NORMAL_PKT
****************************************************************************/
static ALENGTH_T
  bufsize_for_normal_pkt(SNMP_PKT_T *rp)
{
ALENGTH_T       alength;

/* In the following computations, the length of various tag and length  */
/* fields are given as constants.  This is possible because the tag     */
/* values are always low enough to fit into one octet.  And for various */
/* data types, in particular integers, the length field will always     */
/* occupy one octet.                                                    */

rp->pdu_length = 2      /* Tag and length of request_id (an integer) */
                 + A_SizeOfInt(rp->pdu.std_pdu.request_id)
                 + 2     /* Tag and length of error_status (an integer) */
                 + A_SizeOfInt(rp->pdu.std_pdu.error_status)
                 + 2     /* Tag and length of error_index (an integer) */
                 + A_SizeOfInt(rp->pdu.std_pdu.error_index);

alength = set_vbl_sizes(&(rp->pdu.std_pdu.std_vbl));

rp->pdu_length += 1     /* Size of tag on VarBindList sequence */
                  + A_SizeOfLength(alength)
                  + alength;

alength = A_SizeOfOctetString(EBufferUsed(&(rp->community)));

rp->overall_length = 1  /* Size of tag on the PDU sequences */
                     + A_SizeOfLength(rp->pdu_length)
                     + rp->pdu_length
                     + 2    /* Tag and length of snmp_version (an integer) */
                     + A_SizeOfInt(rp->snmp_version)
                     + 1    /* Tag for the community octetstring */
                     + A_SizeOfLength(alength)
                     + alength;

rp->buffer_needed = rp->overall_length
                    + 1 /* Size of tag for overall Message sequence */
                    + A_SizeOfLength(rp->overall_length);

return rp->buffer_needed;
}

/****************************************************************************
        ENCODE_SNMP_COMMON
****************************************************************************/
static void
  encode_snmp_common(EBUFFER_T *ebuffp,
                     ALENGTH_T  overall_length,
                     INT_32_T   snmp_version,
                     EBUFFER_T *community)
{               
/* Generate the Message sequence header */
A_EncodeType(A_SEQUENCE, A_UNIVERSAL | A_CONSTRUCTOR,
             A_EncodeHelper, (OCTET_T *)ebuffp);
A_EncodeLength(overall_length, A_EncodeHelper, (OCTET_T *)ebuffp);

A_EncodeInt(A_INTEGER, A_UNIVERSAL | A_PRIMITIVE, snmp_version,
            A_EncodeHelper, (OCTET_T *)ebuffp);
A_EncodeOctetString(A_OCTETSTRING, A_UNIVERSAL | A_PRIMITIVE,
                    community->start_bp, EBufferUsed(community),
                    A_EncodeHelper, (OCTET_T *)ebuffp);
}

/****************************************************************************
        ENCODE_SNMP_NORMAL_PDU
****************************************************************************/
static void
  encode_snmp_normal_pdu(SNMP_PKT_T *rp,
                         EBUFFER_T  *ebuffp)
{
/* Generate the PDU header */
A_EncodeType(rp->pdu_type, A_DEFAULT_SCOPE | A_CONSTRUCTOR,
             A_EncodeHelper, (OCTET_T *)ebuffp);
A_EncodeLength(rp->pdu_length, A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode request-id */
A_EncodeInt(A_INTEGER, A_UNIVERSAL | A_PRIMITIVE, rp->pdu.std_pdu.request_id,
            A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode error-status */
A_EncodeInt(A_INTEGER, A_UNIVERSAL | A_PRIMITIVE,rp->pdu.std_pdu.error_status,
            A_EncodeHelper, (OCTET_T *)ebuffp);

/* Encode error-index */
A_EncodeInt(A_INTEGER, A_UNIVERSAL | A_PRIMITIVE, rp->pdu.std_pdu.error_index,
            A_EncodeHelper, (OCTET_T *)ebuffp);

encode_var_bind_list(&(rp->pdu.std_pdu.std_vbl), ebuffp);
}

/****************************************************************************
NAME:  encode_var_bind_list

PURPOSE:  Encode a VarBindList

PARAMETERS:
        VBL_T *         The VarBindList to be encoded

RETURNS:  Nothing
****************************************************************************/
static void
  encode_var_bind_list(VBL_T     *vblp,
                       EBUFFER_T *ebuffp)
{
VB_T            *vbp;

/* Generate the VarBindList sequence header */
A_EncodeType(A_SEQUENCE, A_UNIVERSAL | A_CONSTRUCTOR,
             A_EncodeHelper, (OCTET_T *)ebuffp);
A_EncodeLength(vblp->vbl_length, A_EncodeHelper, (OCTET_T *)ebuffp);

for (; vblp != (VBL_T *)0; vblp = vblp->vblp)
    if ((vbp = vblp->vblist) != (VB_T *)0) {
        int i;
        for (i = 0; i < vblp->vbl_count; i++, vbp++) {
            A_EncodeType(A_SEQUENCE, A_UNIVERSAL | A_CONSTRUCTOR,
                         A_EncodeHelper, (OCTET_T *)ebuffp);

            A_EncodeLength(vbp->vb_seq_size,
                           A_EncodeHelper, (OCTET_T *)ebuffp);

            A_EncodeObjectId(A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE,
                             &(vbp->vb_obj_id),
                             A_EncodeHelper, (OCTET_T *)ebuffp);

            switch (vbp->vb_data_flags_n_type) {
              case VT_NUMBER:
                   A_EncodeInt(VT_NUMBER & ~A_IDCF_MASK,
                               VT_NUMBER & A_IDCF_MASK,
                               vbp->value_u.v_number,
                               A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_COUNTER:
                   A_EncodeUnsignedInt(VT_COUNTER & ~A_IDCF_MASK,
                                       VT_COUNTER & A_IDCF_MASK,
                                       vbp->value_u.v_counter,
                                       A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_GAUGE:
                   A_EncodeUnsignedInt(VT_GAUGE & ~A_IDCF_MASK,
                                       VT_GAUGE & A_IDCF_MASK,
                                       vbp->value_u.v_gauge,
                                       A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_TIMETICKS:
                   A_EncodeUnsignedInt(VT_TIMETICKS & ~A_IDCF_MASK,
                                       VT_TIMETICKS & A_IDCF_MASK,
                                       vbp->value_u.v_timeticks,
                                       A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_STRING:
                   A_EncodeOctetString(VT_STRING & ~A_IDCF_MASK,
                                       VT_STRING & A_IDCF_MASK,
                                       vbp->value_u.v_string.start_bp,
                                       EBufferUsed(&(vbp->value_u.v_string)),
                                       A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_OPAQUE:
                    A_EncodeOctetString(VT_OPAQUE & ~A_IDCF_MASK,
                                       VT_OPAQUE & A_IDCF_MASK,
                                       vbp->value_u.v_string.start_bp,
                                       EBufferUsed(&(vbp->value_u.v_string)),
                                       A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_OBJECT:
                    A_EncodeObjectId(A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE,
                                     &(vbp->value_u.v_object),
                                     A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_EMPTY:
                   A_EncodeType(VT_EMPTY & ~A_IDCF_MASK,
                                VT_EMPTY & A_IDCF_MASK,
                                A_EncodeHelper, (OCTET_T *)ebuffp);     
                   A_EncodeLength(0, A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
              case VT_IPADDRESS:
                    A_EncodeOctetString(VT_IPADDRESS & ~A_IDCF_MASK,
                                       VT_IPADDRESS & A_IDCF_MASK,
                                       vbp->value_u.v_network_address,
                                       4,
                                       A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;

              case VT_NOSUCHOBJ:
              case VT_NOSUCHINS:
              case VT_ENDOFMIB:
                   A_EncodeType(vbp->vb_data_flags_n_type & ~A_IDCF_MASK,
                           (OCTET_T)(vbp->vb_data_flags_n_type & A_IDCF_MASK),
                           A_EncodeHelper, (OCTET_T *)ebuffp);  
                   A_EncodeLength(0, A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;

/* We only do include the following cases if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
              case VT_COUNTER64:
                   A_EncodeUnsignedInt64(VT_COUNTER64 & ~A_IDCF_MASK,
                                         VT_COUNTER64 & A_IDCF_MASK,
                                         &(vbp->value_u.v_counter64),
                                         A_EncodeHelper, (OCTET_T *)ebuffp);
                   break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
              default:
                   break;
              }
            }
        }
}

/****************************************************************************
NAME:  set_vbl_sizes

PURPOSE:  Scan a VarBindList, setting the internal lengths and computing
          the total length.

PARAMETERS:
        VBL_T *         The VarBindList structure to be scanned and set

RETURNS:  ALENGTH_T     The number of octets the VarBindList contents would
                        use if ASN.1 encoded.
****************************************************************************/
static ALENGTH_T
  set_vbl_sizes(VBL_T * vblp)
{
VB_T            *vbp;
ALENGTH_T       vblist_size;
VBL_T           *cur_vblp;

cur_vblp = vblp;
vblist_size = 0;

for (; cur_vblp != (VBL_T *)0; cur_vblp = cur_vblp->vblp)
    if ((vbp = cur_vblp->vblist) != (VB_T *)0) {
        int i;
        for (i = 0; i < cur_vblp->vbl_count; i++, vbp++) {
            ALENGTH_T vb_size;  /* Accumulator of size of VarBind sequence */
            ALENGTH_T obj_size;

            obj_size = A_SizeOfObjectId(&(vbp->vb_obj_id));
            vb_size = 1 /* The object ID tag is always 1 octet long */
                     + A_SizeOfLength(obj_size)
                     + obj_size;

            switch (vbp->vb_data_flags_n_type) {
              case VT_NUMBER:
                   vbp->vb_data_length = A_SizeOfInt(vbp->value_u.v_number);
                   break;
              case VT_COUNTER:
              case VT_GAUGE:
              case VT_TIMETICKS:
                   vbp->vb_data_length = A_SizeOfUnsignedInt(
                                                   vbp->value_u.v_counter);
                   break;
              case VT_STRING:
              case VT_OPAQUE:
                   {
                   ALENGTH_T used;
                   used = EBufferUsed(&(vbp->value_u.v_string));
                   vbp->vb_data_length = A_SizeOfOctetString(used);
                   }
                   break;
              case VT_OBJECT:
                   vbp->vb_data_length =
                           A_SizeOfObjectId(&(vbp->value_u.v_object));
                   break;
              case VT_NOSUCHINS:
              case VT_NOSUCHOBJ:
              case VT_ENDOFMIB:
              case VT_EMPTY:
                   vbp->vb_data_length = 0;
                   break;
              case VT_IPADDRESS:
                   vbp->vb_data_length = 4;
                   break;
/* We only do include the following cases if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
              case VT_COUNTER64:
                   vbp->vb_data_length = A_SizeOfUnsignedInt64(
                                                 &(vbp->value_u.v_counter64));
                   break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
              default:
                   break;
              }
            vbp->vb_seq_size = vb_size
                              + 1 /* The data tag is always 1 octet */
                              + A_SizeOfLength(vbp->vb_data_length)
                              + vbp->vb_data_length;

            vblist_size += vbp->vb_seq_size
                        + 1 /* The sequence tag is always 1 octet */
                        + A_SizeOfLength(vbp->vb_seq_size);
            }
        }

vblp->vbl_length = vblist_size;

return vblist_size;
}
/****************************************************************************
NAME:  vbsize

PURPOSE:  Compute and set the internal length of a var bind

PARAMETERS:
        VB_T *          The VarBind to be sized.

RETURNS:  ALENGTH_T     The number of octets the Var Bind would
                        use if ASN.1 encoded.
****************************************************************************/
ALENGTH_T
  vbsize(VB_T * vbp)
{

  ALENGTH_T vb_size;    /* Accumulator of size of VarBind sequence */
  ALENGTH_T obj_size;

  obj_size = A_SizeOfObjectId(&(vbp->vb_obj_id));
  vb_size = 1 /* The object ID tag is always 1 octet long */
            + A_SizeOfLength(obj_size)
            + obj_size;

  switch (vbp->vb_data_flags_n_type)
    {
    case VT_NUMBER:
        vbp->vb_data_length = A_SizeOfInt(vbp->value_u.v_number);
        break;
    case VT_COUNTER:
    case VT_GAUGE:
    case VT_TIMETICKS:
        vbp->vb_data_length = A_SizeOfUnsignedInt(vbp->value_u.v_counter);
        break;
    case VT_STRING:
    case VT_OPAQUE:
        {
        ALENGTH_T used;
        used = EBufferUsed(&(vbp->value_u.v_string));
        vbp->vb_data_length = A_SizeOfOctetString(used);
        break;
        }
    case VT_OBJECT:
        vbp->vb_data_length = A_SizeOfObjectId(&(vbp->value_u.v_object));
        break;
    case VT_NOSUCHINS:
    case VT_NOSUCHOBJ:
    case VT_ENDOFMIB:
    case VT_EMPTY:
        vbp->vb_data_length = 0;
        break;
    case VT_IPADDRESS:
        vbp->vb_data_length = 4;
        break;
/* We only do include the following cases if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
    case VT_COUNTER64:
        vbp->vb_data_length = A_SizeOfUnsignedInt64(
                                        &(vbp->value_u.v_counter64));
        break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
    default:
        break;
    }
  vbp->vb_seq_size = vb_size
                     + 1 /* The data tag is always 1 octet */
                     + A_SizeOfLength(vbp->vb_data_length)
                     + vbp->vb_data_length;

return (vbp->vb_seq_size
       + 1 /* The sequence tag is always 1 octet */
       + A_SizeOfLength(vbp->vb_seq_size));
}

/****************************************************************************
NAME:  SNMP_Bufsize_For_Packet

PURPOSE:  Compute how much buffer space is needed to hold a packet if
          it were encoded.

PARAMETERS:
        SNMP_PKT_T *    SNMP Packet structure

RETURNS:  unsigned int  The buffer size required.
          0 in the case that the packet has a version that the code doesn't
          understand, either a completely bad version or one that the code
          hasn't been compiled for.

NOTE:   This routine does not account for any size differences which may
        occur due to any special authentication encoding.
****************************************************************************/
ALENGTH_T
  SNMP_Bufsize_For_Packet(SNMP_PKT_T *rp)
{
  switch (rp->snmp_version) {
#if INSTALL_ENVOY_SNMP_VERSION_1
      case SNMP_VERSION_1:
          if (rp->pdu_type == TRAP_PDU)
              return bufsize_for_trap_pkt(rp);
          else
              return bufsize_for_normal_pkt(rp);
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_1 */
#if INSTALL_ENVOY_SNMP_VERSION_2
      case SNMP_VERSION_2:
          return bufsize_for_normal_pkt(rp);
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_2 */
#if INSTALL_ENVOY_SNMP_VERSION_USEC
      case SNMP_VERSION_USEC:
          return bufsize_for_normal_pkt(rp);
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_2 */
      default:
          /* incorrect version number */
          return(0);
      }
}

/****************************************************************************
NAME:  SNMP_Encode_Packet

PURPOSE:  Encode an SNMP packet.

PARAMETERS:
        SNMP_PKT_T *    SNMP Packet structure
        EBUFFER_T *     A buffer structure to receive the encoded packet

RETURNS:  0             Packet processed without error
          -1            Error encountered during packet processing

         On a sucessful return, the ebuffer passed as a parameter will
         contain the encoded packet.  If necessary, space will be malloc-ed.

****************************************************************************/
int
  SNMP_Encode_Packet(SNMP_PKT_T *rp,
                     EBUFFER_T  *ebuffp)
{
  ALENGTH_T need;

  need = SNMP_Bufsize_For_Packet(rp);

  /* Give the user a chance to allocate space from other areas */
#if defined(SNMP_user_get_encode_buffer)
  /* does the user want to get the space for us */
  if (ebuffp->start_bp == 0) {
      if (SNMP_user_get_encode_buffer(rp, need, ebuffp) != 0)
          return (-1);
      }
#endif

  /* translate this call into an encode packet with size call and add
     the size argument */

  return(SNMP_Encode_Pkt_With_Siz(rp, ebuffp, need));
}

/****************************************************************************
NAME:  SNMP_Encode_Pkt_With_Siz

PURPOSE:  Encode an SNMP packet, with the size as one of the arguments.

PARAMETERS:
        SNMP_PKT_T *    SNMP Packet structure
        EBUFFER_T *     A buffer structure to receive the encoded packet
        ALENGTH_T       The size of the required buffer for the packet.

RETURNS:  0             Packet processed without error
          -1            Error encountered during packet processing

         On a sucessful return, the ebuffer passed as a parameter will
         contain the encoded packet.  If necessary, space will be malloc-ed.

****************************************************************************/
int
  SNMP_Encode_Pkt_With_Siz(SNMP_PKT_T *rp,
                           EBUFFER_T  *ebuffp,
                           ALENGTH_T   need)
{
   /* Sanity check the space required variable */
  if (need == 0)
      return (-1);

  /* Allocate some space if necessary */
  if (ebuffp->start_bp == 0) {
      OCTET_T *buffp;
      /* Obtain space for the packet */
      if ((buffp = (OCTET_T *)SNMP_memory_alloc(need)) == 0)
          return (-1);
      EBufferSetup(BFL_IS_DYNAMIC, ebuffp, buffp, need);
      }
  else { /* Make sure there is enough space in the buffer the user gave us */
      if (EBufferRemaining(ebuffp) < need)
          return (-1);
      }

  /* figure out what version we have and call the proper routines to 
     do the encoding */
  switch (rp->snmp_version) {
#if INSTALL_ENVOY_SNMP_VERSION_1
      case SNMP_VERSION_1:
          /* encode the common header */
          encode_snmp_common(ebuffp, rp->overall_length, rp->snmp_version,
                             &(rp->community));
          /* encode the pdu */
          if (rp->pdu_type != TRAP_PDU)
              encode_snmp_normal_pdu(rp, ebuffp);
          else
              encode_snmp_trap_pdu(rp, ebuffp);
          return 0;
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_1 */
#if INSTALL_ENVOY_SNMP_VERSION_2
      case SNMP_VERSION_2:
          /* encode the common header */
          encode_snmp_common(ebuffp, rp->overall_length, rp->snmp_version,
                             &(rp->community));
          /* encode the pdu */
          encode_snmp_normal_pdu(rp, ebuffp);
          return 0;
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_2 */
#if INSTALL_ENVOY_SNMP_VERSION_USEC
      case SNMP_VERSION_USEC:
          {
          OCTET_T key[16];
          ALENGTH_T sptr, eptr;
          /* save a pointer to the start of the buffer */
          sptr = EBufferUsed(ebuffp);
          /* encode the common header */
          encode_snmp_common(ebuffp, rp->overall_length, rp->snmp_version,
                             &(rp->community));
          /* encode the pdu */
          encode_snmp_normal_pdu(rp, ebuffp);
          /* if necessary generate the authentication digest
             and reencode the modified header */
          if (rp->auth_routine) {
              /* save a pointer to the end of the buffer */
              eptr = EBufferUsed(ebuffp);

              /* save the auth key in case it needs to be reused */
              MEMCPY(key, EBufferStart(&rp->auth_key), 16);
              /* generate the digest */
              rp->auth_routine(EBufferStart(ebuffp), EBufferUsed(ebuffp),
                               key, 16, EBufferStart(&rp->auth_key));
              /* reset the buffer pointer so we can overwrite the header */
              EBufferSeek(ebuffp, sptr, 0);
              encode_snmp_common(ebuffp, rp->overall_length, rp->snmp_version,
                                 &(rp->community));
              /* reset the buffer pointer to the end of the packet */
              EBufferSeek(ebuffp, eptr, 0);
              /* restore the auth key */
              MEMCPY(EBufferStart(&rp->auth_key), key, 16);
              }
          return 0;
          }
#endif /* INSTALL_ENVOY_SNMP_VERSION_USEC */

      default:
          /* incorrect version number */
          return(-1);
      }
}
