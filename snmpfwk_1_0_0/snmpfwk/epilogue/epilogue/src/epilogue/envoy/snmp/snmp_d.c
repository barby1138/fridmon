/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/snmp_d.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: snmp_d.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:03  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/11/01 01:25:39  sar
 * In the packet we are processing save a pointer to the coarse lock we
 * are using so that we can free it later without needing to know which
 * lock it was.
 *
 * Revision 7.4  1997/10/22 03:10:28  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.3  1997/10/16 00:46:07  sar
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
 * Revision 7.2  1997/03/20 06:49:18  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.7  1996/01/23  16:27:24  sar
 * Added send report pdu option for unknown agent id
 *
 * Revision 6.6  1996/01/10  16:34:47  sar
 * usec updates
 *
 * Revision 6.5  1995/11/16  22:04:38  sar
 * modified snmp_decode_packet to be cleaner with only one packet format
 *
 * Revision 6.4  1995/11/01  00:46:48  sar
 * added ifdefs to the version switch for pdu types
 * arranged to leave out the v1 trap code if we aren't compiling v1
 *
 * Revision 6.3  1995/10/20  23:02:48  sar
 * remvoed bitstring, nsapaddress, and uintger32 from decode & cleanup
 * routines
 * removed init/cleanup of party stuff from packet structure
 * remved the v2 party code
 *
 * Revision 6.2  1995/09/18  19:23:58  sar
 * Moved the endif for what gets compiled if we aren't compiling a
 * subagent only library.
 * Removed the NO_PP stuff.
 *
 * Revision 6.1  1995/06/05  20:58:44  sar
 * Moved the cleaning of the data area from clean_vb into clean_vb_data
 * so it can be used elsewhere (nextpdu.c for example).
 *
 * Revision 6.0  1995/05/31  21:48:12  sra
 * Release 6.0.
 *
 * Revision 5.10  1995/05/22  19:43:54  sar
 * Init per packet lock if continue_reentrant is installed.
 *
 * Revision 5.9  1995/05/20  02:37:57  sar
 * Corrected misspelling of INSTALL_ENVOY_SNMP_SERIALIZE
 *
 * Revision 5.8  1995/04/28  23:22:26  sar
 * Added a GateDecrement call in the cleanup routines
 * added a test of the serialization flag before compilinig in the call
 * to clean up inactive objects.
 *
 * Revision 5.7  1995/04/18  21:13:44  sar
 * Use new lock macros, some format changes and added group cleanup.
 *
 * Revision 5.6  1995/03/21  00:30:26  sar
 * Removed the definitions for the stats macros, they are now in snmpstat.h
 * Removed some fo the ifdefs associated with the lock code as
 * the defaults for the lock macros are now "successful"
 * Switch to using MEMSET and MEMCPY macros
 * Switch to using for & loc to name parties (instead of src & dst)
 * Updated the packet cleanup rotuines to deal with the saved_vbl struct
 * in the std_pdu struct.
 *
 * Revision 5.5  1994/10/12  20:25:37  sar
 * Moved the code to do vb_free_priv to be earlier in the clean_vb routine
 * so that the vb_free_priv routine can access the vb.
 * modified trap_time_ticks to be a uint32.
 *
 * Revision 5.4  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.3  1994/07/29  18:07:11  sar
 * Added a type check on a v1 pdu, any non-v1 pdus are flushed as ans1 errors.
 *
 * Revision 5.2  1994/07/26  21:59:37  sar
 * Moved the acquistion of the generic read lock from the packet allocation
 * routine to the decode block, after we allocate the packet.  This will
 * allow the packet allocation routine to be used more liberally.
 *
 * Revision 5.1  1994/05/17  20:37:37  sar
 * Added some break statements around the lock switch
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.10  1994/05/10  20:16:03  sar
 * moved VarBindList_Allocate so it will be available to sub agents.
 *
 * Revision 4.9  1994/05/06  20:31:27  sar
 * removed the ifdef v1 for the community string as we now use it for
 * the localentity string in v2.
 *
 * Revision 4.8  1994/05/04  23:48:43  sar
 * Made VBPRIVPROC's always take an argument of PTR_T (dropped the VB_T *)
 * possibility).
 *
 * Revision 4.7  1994/05/02  21:20:43  sar
 * fill in the localtimeobj after finding the context so that method
 * routines can use it.
 *
 * Revision 4.6  1994/05/02  17:02:30  sar
 * Added new decode routines *WTC (with type check) to type check the string
 * as it is being decoded.  Mostly these will be used in snmp_d.c
 *
 * Revision 4.5  1994/04/29  21:13:03  sar
 * added a typecast for SA_freeproc to make the compiler happy.
 *
 * Revision 4.4  1994/04/29  21:09:32  sar
 * Well, 4.3 was the wrong version so this time let's check in the correct
 * version.
 *
 * Revision 4.3  1994/04/29  20:54:59  sar
 * Moved the oids for src and dest party and context into the snmp_pkt struct.
 * Sub agent scheme modifications: moved code around to ifdef it more easily
 * set the view_index field while decoding a v2 packet, added the VB_FREE
 * compat option.
 *
 * Revision 4.2  1994/01/25  20:49:11  sar
 * Added a check of the pdu type during the header check for v2.
 * Previously a packet with an unknow pdu would make it through to the
 * acl check and fail with an auth error instead of causing a parse error.
 *
 * Revision 4.1  1993/09/30  19:54:41  sar
 * Several Fixes:
 * Stopped incrementing snmpInGenErrs as we were doing due to gen errs in
 * our code instead of incoming packets that had the InGenErr tag.
 * Removed a useless increment of snmpInASNParseErrs and made several
 * increments that were only counting if v1 was installed count if eith
 * v1 or v2 are installed.
 * Realigned some # statements to be in the first column to make some
 * preprocessors happy.
 * Added a incrementer for the UnknownDst in the case that the party is
 * known but not local.
 * Modified the code to deal with cleaning up a VBLP so that we clean up
 * the chain that a GetBulk may produce.
 * Finally corrected one comment.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.26  1993/06/09  21:15:25  sar
 * Changed the initialization of a vbl, we now 0 the whole thing and
 * initialize only what isn't 0.
 *
 * Revision 3.25  1993/06/07  18:26:04  sar
 * Modifed SNMP_Decode_Packet to be SNMP_Deocde_Packet_WER to allow
 * the decode routines to return an error (with error return) that
 * indicates the caller should send the packet back as a response.
 *
 * Revision 3.24  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.23  1993/05/22  01:04:31  sar
 * Cleaned up use of stats so we can compile if they aren't included.
 *
 * Revision 3.22  1993/05/19  21:00:49  sar
 * removed the unused vars authinfo_flags and authinfo_type
 *
 * Revision 3.21  1993/05/14  15:13:00  sar
 * renamed private field in packet structure to be user_private
 *
 * Revision 3.20  1993/05/05  15:00:30  sar
 * checked that the priv mechanism would work.
 *
 * Revision 3.19  1993/05/03  17:16:12  sar
 * Added routines for local time, removed {party context} oids from packet
 * structure, added asn1len to {party context} structures and
 * SNMP_{Party Context}_Name where needed to lookup the name for a given
 * {Party Context}.
 *
 * Revision 3.18  1993/04/30  22:54:08  sar
 * Added mechanism for minimial proxies and coarse grained locks.  The
 * latter required the addition of routines to get the name of a leaf given
 * a pointer to the leaf.
 *
 * Revision 3.17  1993/04/27  21:10:00  sar
 * Made clean_vb_list call clean_vb so there is only one piece of code to
 * maintain.
 *
 * Revision 3.16  1993/04/26  20:37:09  sar
 * Added ifdefs to allow clean makes of version 1 and 2, added bit strings,
 * arrange for deletion of acls with the party or context they refer to
 * is deleted.
 *
 * Revision 3.15  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.14  1993/04/17  01:06:51  sar
 * added a packet argument to the auth violation macro, check for private
 * routines to free data on the vbs while cleaning them.
 *
 * Revision 3.13  1993/03/26  17:49:25  sar
 * added test for proxyness and call to SNMP_PROXY_HOOK to handle the packet
 * if we are doing proxy.
 *
 * Revision 3.12  1993/03/25  21:18:09  sar
 * Added routines to encode/decode 64 bit integers/counter64s
 *
 * Revision 3.11  1993/03/24  01:01:40  sar
 * changed the silly marketing macro to be something more reasonable.
 *
 * Revision 3.10  1993/03/20  00:25:43  sar
 * Changed class to be a UINT_16_T and removed the use of mib_view for
 * version two packets.
 *
 * Revision 3.9  1993/03/07  01:19:39  sar
 * In snmp version 2 we now get the rp->mib_view from the context view index.
 * This lets us simulate views.  It will change when the view table is
 * defined.
 *
 * Revision 3.8  1993/03/05  03:58:02  dab
 * fixed up types
 *
 * Revision 3.6  1993/03/03  03:17:00  sar
 * Updated to deal with shadow rows.  I have defined a new enumeration for
 * row status (RS_RSNE = 0) which means that row status doesn't exist yet.
 * Most routines now check to see if the row is active before using it.
 * Also added SNMP_SILLY_MARKETING_MACRO as per Rob's request.
 *
 * Revision 3.5  1993/02/24  01:02:41  sar
 * Changed many call to use OIDC_T/lenght pairs instead of OBJ_ID_T as now
 * we don't need to specifically allocate the OBJ_ID_T.
 * Removed the name objid from the PARTYLEAF_T as the name may be determined from
 * the tree itself.  For efficiency purposes put the dest/src party names into
 * the pkt so we don't have to look them up frequently.
 *
 * Revision 3.4  1993/02/22  18:53:36  sar
 * Moved acl structures to their own tree
 * Added context structrues with attendent routines
 *
 * Revision 3.3  1993/02/17  21:10:39  sar
 * Added no_pp stuff.
 * Updated stats to deal with v1 and v2 (or both or neither).
 * Added ifdef compile flags for v1 or v2 or both.
 * Added code and routines to handle v2 (SNMP_Decode_Secure_Packet
 * acl_check).
 * Updated decode_pkt_to_vb to use new data types VT_UINTEGER32,
 * VT_NOSUCHOBJ, VT_NOSUCHINS and VT_ENDOFMIB.
 * Added Clean_vb to clean up as we may have chained some vbs to the
 * packet (for get bulk).
 * Added VBlist_Allocate to allocate a new vb for get bulk purposes.
 *
 * Revision 3.2  92/07/31  12:57:17  dab
 * Fixed up some compile errors.
 * 
 * Revision 3.1  92/07/31  11:53:29  dab
 * Added decode path for Secure SNMP.  Not yet tested.
 * 
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.107  92/02/05  18:46:32  dab
 * More casting to keep MSC happy.
 * 
 * Revision 2.106  91/11/07  09:34:05  dab
 * Moved where SNMP_validate_community() is called from.  It's now after
 * we've determined what type the PDU is.
 * 
 * Revision 2.105  91/11/05  10:59:51  dab
 * Removed the 'then's.
 * 
 * Revision 2.104  91/10/30  20:42:11  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.103  91/09/18  12:32:37  dab
 * Updated to use new macros from <asn1conf.h> and <snmpconf.h>.
 * 
 * Revision 2.102  91/08/15  12:31:11  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.101  91/08/12  12:47:39  dab
 * Cast arguments to memcpy() and memset() to PTR_T.
 * 
 * Revision 2.100  91/08/09  14:08:26  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:54  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:56
 * Release 2.00
 * 
 *    Rev 1.12   16 Dec 1989 14:07:48
 * A check added to snmp_free() to avoid freeing a non-allocated block.
 * 
 *    Rev 1.11   14 Dec 1989 16:01:00
 * Added support for Borland Turbo C compiler
 * 
 *    Rev 1.10   27 Apr 1989 15:56:30
 * Removed unused variables
 * 
 *    Rev 1.9   24 Mar 1989 17:26:30
 * Module "copyrite.c" included to create in-core image of Epilogue
 * copyright notice.  Old method via "snmp.h" did not work for all
 * compilers.
 * 
 *    Rev 1.8   23 Mar 1989 12:03:58
 * Added more checks to protect against mis-encoded ASN.1.
 * 
 *    Rev 1.7   19 Mar 1989 13:04:52
 * Added protection against zero length, short, or overlength IP addresses.
 * 
 *    Rev 1.6   18 Mar 1989 11:58:42
 * Made changes to conform to string decoding simplification in decode.c.
 * 
 *    Rev 1.5   17 Mar 1989 21:41:46
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.4   04 Mar 1989 10:35:06
 * Added cast to actual parameter on call to memset to avoid warnings on
 * some compilers.
 * 
 *    Rev 1.3   11 Jan 1989 12:46:42
 * Moved Clean_Obj_ID() to objectid.c
 * 
 *    Rev 1.2   19 Sep 1988 17:26:50
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:22
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:47:06
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <localio.h>
#include <buffer.h>
#include <decode.h>
#include <objectid.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <snmpstat.h>

#include <bug.h>

/*#if INSTALL_ENVOY_EPI_MASTER
#include <subagent.h>
#endif*/

#if INSTALL_ENVOY_SNMP_VERSION_USEC
#include <access.h>
#include <agent.h>
#include <context.h>
#include <user.h>
#include <md5.h>
static OIDC_T USEC_UNSUPPORTEDQOS[]    = {1, 3, 6, 1, 6, 3, 6, 1, 2, 1, 0};
static OIDC_T USEC_NOTINWINDOWS[]      = {1, 3, 6, 1, 6, 3, 6, 1, 2, 2, 0};
static OIDC_T USEC_UNKNOWNUSERNAMES[]  = {1, 3, 6, 1, 6, 3, 6, 1, 2, 3, 0};
static OIDC_T USEC_WRONGDIGESTVALUES[] = {1, 3, 6, 1, 6, 3, 6, 1, 2, 4, 0};
static OIDC_T USEC_UNKNOWNCONTEXTS[]        = {1,3,6,1,6,3,6,1,2,5,0};
static OIDC_T USEC_BADPARAMETERS[]          = {1,3,6,1,6,3,6,1,2,6,0};
static OIDC_T USEC_UNAUTHORIZEDOPERATIONS[] = {1,3,6,1,6,3,6,1,2,7,0};
#define USECSTATSLENGTH 11
static OIDC_T SNMP_ASNPARSEERRORS[] = {1,3,6,1,2,1,11,6,0};
static OIDC_T SNMP_PROXYDROPS[]     = {1,3,6,1,2,1,11,32,0};
#define SNMPSTATSLENGTH 9
#endif

/*
 * Internal Functions
 */
#if !defined(SNMP_AUTH_DECODE_VIOLATION)
#define SNMP_AUTH_DECODE_VIOLATION(pkt, x)
#endif 

/*lint -e617    */
#include "copyrite.c"
/*lint +e617    */

/****************************************************************************
NAME:  count_var_binds

PURPOSE:  To figure out how many VarBind items are in a VarBindList
          On entry, the input stream should be positioned to the start of the
          data (contents) part of the VarBindList.
          On exit, the input stream will be positioned to the start of the
          data (contents) part of the VarBindList.

PARAMETERS:
        LCL_FILE *      The input stream

RETURNS:  int           Count of the entries
                        -1 on error

Note:   This routine correctly handles the case where the VarBindList is
        empty.
****************************************************************************/
static int
  count_var_binds(LCL_FILE *    stream,
                  ALENGTH_T     leng)
{
  ALENGTH_T     used;
  int tell_place;       /* Offset in stream to VarBind data */
  int items;
  int asn1err = 0;

  tell_place = Lcl_Tell(stream);

  for(items = 0, used = 0; used < leng;) {
    ALENGTH_T   alength;
    int start_place, end_place;

    start_place = Lcl_Tell(stream);
    if (Lcl_Eof(stream))
      break;

    /* Skip over the VarBind sequence */
    (void) A_DecodeTypeValue(stream, &asn1err);
    alength = A_DecodeLength(stream, &asn1err);
    if (asn1err) {
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
        items = -1;
        break;
        }
    if (Lcl_Seek(stream, (int)alength, 1) == -1) {
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
        items = -1;
        break;
        }

    end_place = Lcl_Tell(stream);
    used = used + (ALENGTH_T)end_place - (ALENGTH_T)start_place;
    items++;
  }

  (void) Lcl_Seek(stream, tell_place, 0);
  return items;
}

/****************************************************************************
NAME:  decode_pkt_to_vb

PURPOSE:  Decode a VarBind from a packet
          On entry the input stream should be positioned to the tag field
          of the VarBind entry.
          On exit, the stream pointer will be positioned to at the start
          of the ASN.1 type field of AFTER the VarBind.

PARAMETERS:
        LCL_FILE   *    The input stream
        VB_T       *    The VB_T element to be filled.
        SNMP_PKT_T *    The packet being decoded.

RETURNS:  0 for sucess, -1 for failure.
****************************************************************************/
static int
  decode_pkt_to_vb(LCL_FILE     *stream,
                   VB_T         *vbp,
                   SNMP_PKT_T   *pkt)
{
  OCTET_T       flags;
  ATVALUE_T     id;
  ALENGTH_T     leng;
  int           asn1err = 0;

  A_DecodeObjectIdWTC(stream, &(vbp->vb_obj_id), &asn1err,
                      A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE);
  if (asn1err) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return -1;
      }
  flags = A_DecodeTypeClass(stream);
  id    = A_DecodeTypeValue(stream, &asn1err);
  leng  = A_DecodeLength(stream, &asn1err);
  if (asn1err) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return -1;
      }

  vbp->vb_data_length = leng;
  vbp->vb_data_flags_n_type = flags | (OCTET_T)id;

  switch (vbp->vb_data_flags_n_type) {
  case VT_NUMBER:
    vbp->value_u.v_number = A_DecodeIntegerData(stream, leng, &asn1err);
    break;
  case VT_COUNTER:
  case VT_GAUGE:
  case VT_TIMETICKS:
    vbp->value_u.v_counter = (UINT_32_T)A_DecodeIntegerData(stream, leng,
                                                            &asn1err);
    break;
  case VT_STRING:
  case VT_OPAQUE:
    A_DecodeOctetStringData(stream, leng, &(vbp->value_u.v_string),
                            &asn1err);
    break;
  case VT_OBJECT:
    A_DecodeObjectIdData(stream, leng, &(vbp->value_u.v_object), &asn1err);
    break;
  case VT_NOSUCHOBJ:
  case VT_NOSUCHINS:
  case VT_ENDOFMIB:
  case VT_EMPTY:
    /* Empty has no contents to be decoded */
    break;
  case VT_IPADDRESS:
    {
      EBUFFER_T ipbuff;
      unsigned int used;

      MEMSET(vbp->value_u.v_network_address, 0, 4);
      EBufferInitialize(&ipbuff);
      A_DecodeOctetStringData(stream, leng, &ipbuff, &asn1err);
      used = min(4, EBufferUsed(&ipbuff));
      if (used != 0) {
          MEMCPY(vbp->value_u.v_network_address, ipbuff.start_bp, used);
          EBufferClean(&ipbuff);
          }
    }
    break;

/* If we are using v2 types in general or we only have v2 installed
   we allow the following types all the time.
   Otherwise if we have v2 and v1 installed but not v2 types we need to
   check the type of the packet and if it's v1 reject these types.
   If we don't have v2 or v2 types installed the types will be rejected
   as the default case */
#if ((INSTALL_ENVOY_SNMP_USE_V2_TYPES) || \
     ((INSTALL_ENVOY_SNMP_VERSION_2) && !(INSTALL_ENVOY_SNMP_VERSION_1)))
  case VT_COUNTER64:
    A_DecodeInteger64Data(stream, leng, &(vbp->value_u.v_counter64),
                          &asn1err);
    break;

#else
#if ((INSTALL_ENVOY_SNMP_VERSION_2) && (INSTALL_ENVOY_SNMP_VERSION_1))
  case VT_COUNTER64:
    if (pkt->snmp_version == SNMP_VERSION_1) {
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
        return -1;
        }
    A_DecodeInteger64Data(stream, leng, &(vbp->value_u.v_counter64),
                          &asn1err);
    break;

#endif /* ((INSTALL_ENVOY_SNMP_VERSION_2) && (INSTALL_ENVOY_SNMP_VERSION_1)) */
#endif /* (INSTALL_ENVOY_SNMP_USE_V2_TYPES) */

  default:
    SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
    return -1;
  }

  if (asn1err) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return -1;
      }
  else
      return 0;

  /*NOTREACHED*/
}

/****************************************************************************
NAME:  decode_pkt_to_vblist

PURPOSE:  Decode a packet's VarBindList
          On entry the input stream should be positioned to the tag field
          of the first VarBind entry.
          On exit, the stream pointer will be positioned to at the start
          of the ASN.1 type field of AFTER the VarBindList, normally this
          will be at the end of the packet.

PARAMETERS:
        LCL_FILE  *     The input stream
        VBL_T     *     The VarBindList header to be filled-in
        int             The overall size of the packet as indicated by
                        the outermost ASN.1 wrapper for the SNMP packet
                        (plus the size of the outermost tag/length fields.)
        SNMP_PKT_T *    The packet being decoded

RETURNS:  0 for sucess, -1 for failure.
****************************************************************************/
static int
  decode_pkt_to_vblist(LCL_FILE   * stream,
                       VBL_T      * vblp,
                       int          asn1leng,
                       SNMP_PKT_T * pkt)
{
  VB_T  *vbp;
  int   vbcnt;
  int   i;
  int   asn1err = 0;

  /* Now deal with the VarBindList and sanity check the length field */
  (void) A_DecodeTypeValue(stream, &asn1err);
  vblp->vbl_length = A_DecodeLength(stream, &asn1err);
  if (asn1err || (vblp->vbl_length != Lcl_Size(stream))) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return -1;
      }

  /* Count the number of VarBinds.                      */
  vblp->vbl_count = 0;  /* Just in case things fail     */
  if ((vbcnt = count_var_binds(stream, vblp->vbl_length)) == -1)
    return -1;

  if (vbcnt == 0) { /* Handle case where the VarBindList is empty */
    vblp->vblist = 0;
  }
  else { /* The VarBindList has contents */
    vblp->vbl_count = vbcnt;
    if ((vblp->vblist = VarBindList_Allocate(vblp->vbl_count)) == 0) {
        return -1;
        }

    for (vbp = vblp->vblist, i = 0; i < vblp->vbl_count; i++, vbp++) {
        (void) A_DecodeTypeValue(stream, &asn1err);
        vbp->vb_seq_size = A_DecodeLength(stream, &asn1err);
        if (asn1err) {
            SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
            return -1;
            }
        if (decode_pkt_to_vb(stream, vbp, pkt) == -1)
            return -1;
        }
    }
  return 0;
}

/****************************************************************************
NAME:  SNMP_Decode_V1_Packet

PURPOSE:  Decode an V1 (RFC1157) or V2 SNMP packet.

PARAMETERS:
        SNMP_PKT_T *    Packet we're filling in.
        LCL_FILE *      Stream to decode packet from. The stream is positioned
                        after the first tag field (the tag that says it's an
                        V1 (RFC1157) packet).
        SNMPADDR_T *    Source of the packet
        SNMPADDR_T *    Destination of the packet (most likely
                        the address of the machine on which this
                        code is running.)
        int             asn1 length
RETURNS:  SNMP_PKT_T *  SNMP Packet structure, (SNMP_PKT_T *)0 on failure
****************************************************************************/

static SNMP_PKT_T *
  SNMP_Decode_V1_Packet(SNMP_PKT_T *    rp,
                        LCL_FILE   *    in_stream,
                        SNMPADDR_T *    pktsrc,
                        SNMPADDR_T *    pktdst,
                        int             asn1leng)
{
  ATVALUE_T     ptype;
  ALENGTH_T     plength;
  int           asn1err = 0;

  /* Extract the community string */
  A_DecodeOctetStringWTC(in_stream, &rp->community, &asn1err, A_OCTETSTRING,
                         A_UNIVERSAL | A_PRIMITIVE);
  if (asn1err) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return 0;
      }

  /* Decode the packet type                                             */
  /* Since all of the PDUs follow the same form, we can decode them     */
  /* without being concerned as to which PDU we are decoding.           */
  /* Furthermore, since the VarBindList is the last thing in the PDU,   */
  /* we can ignore the overall length of the sequence forming the PDU.  */

  if (A_DecodeTypeClass(in_stream) != (A_DEFAULT_SCOPE | A_CONSTRUCTOR)) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return 0;
      }

  /* Decode the pdu type */ 
  ptype = A_DecodeTypeValue(in_stream, &asn1err);
  plength = A_DecodeLength(in_stream, &asn1err);

  if (asn1err) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return 0;
      }

  /* Determine if we support the given pdu for the given version */
  switch(rp->snmp_version) {
#if INSTALL_ENVOY_SNMP_VERSION_1
      case SNMP_VERSION_1:
          if (ptype > MAX_V1_PDU) {
              SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
              return 0;
              }
          break;
#endif /* INSTALL_ENVOY_SNMP_VERSION_1 */
#if INSTALL_ENVOY_SNMP_VERSION_2
      case SNMP_VERSION_2:
          /* The trap type is not allowed in version 2 */
          if ((ptype > MAX_PDU) || (ptype == TRAP_PDU)) {
              SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
              return 0;
              }
          break;
#endif /* INSTALL_ENVOY_SNMP_VERSION_2 */
      default:
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInBadVersions);
          return(0);
      }

  /* Check whether there is an inconsistency between the PDU length and */
  /* the overall length indicated by the outermost ASN.1 wrapper.       */
  /* If so, the packet is ill-formed and must be rejected.              */
  if (asn1leng != (Lcl_Tell(in_stream) + (int)plength)) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return(0);
      }

  rp->pdu_type   = ptype;
  rp->pdu_length = plength;

  /* Check that the community string is valid.  This also gets the      */
  /* view mask for this transaction from the community string.  The     */
  /* source and destination addresses are passed through for the user   */
  /* routine to deal with.  It may just copy them into the snmp packet  */
  /* structure.                                                         */
  switch(SNMP_validate_community(rp, pktsrc, pktdst)) {
      case 0:
          break;
      case 1:
      default:
          return(0);
      case 2:
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInBadCommunityNames);
          return(0);
      case 3:
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInBadCommunityUses);
          return(0);
      }

#if (INSTALL_ENVOY_SNMP_VERSION_1)
  if (ptype == TRAP_PDU) {
      EBUFFER_T net_addr;
      unsigned int used;
    
      rp->pdu_type = TRAP_PDU;
      rp->pdu_length = plength;
      rp->pdu.trap_pdu.trap_vbl.vblist = 0;
    
      A_DecodeObjectIdWTC(in_stream, &(rp->pdu.trap_pdu.enterprise_objid),
                          &asn1err, A_OBJECTID, A_UNIVERSAL | A_PRIMITIVE);
      if (asn1err) {
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
          return(0);
          }

      MEMSET(rp->pdu.trap_pdu.net_address, 0, 4);
      EBufferInitialize(&net_addr);
      A_DecodeOctetStringWTC(in_stream, &net_addr, &asn1err, 
                             VT_IPADDRESS & ~A_IDCF_MASK,
                             VT_IPADDRESS & A_IDCF_MASK);
      if (asn1err) {
          EBufferClean(&net_addr);
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
          return(0);
          }

      used = min(4, EBufferUsed(&net_addr));
      if (used != 0) {
          MEMCPY(rp->pdu.trap_pdu.net_address, net_addr.start_bp, used);
          EBufferClean(&net_addr);
          }

      rp->pdu.trap_pdu.generic_trap    = A_DecodeIntegerWTC(in_stream,
                        &asn1err, A_INTEGER, A_UNIVERSAL | A_PRIMITIVE);
      rp->pdu.trap_pdu.specific_trap   = A_DecodeIntegerWTC(in_stream,
                        &asn1err, A_INTEGER, A_UNIVERSAL | A_PRIMITIVE);
      rp->pdu.trap_pdu.trap_time_ticks = (UINT_32_T)
                                        A_DecodeIntegerWTC(in_stream,
                                        &asn1err, VT_TIMETICKS & ~A_IDCF_MASK,
                                        VT_TIMETICKS & A_IDCF_MASK);
      if (asn1err) {
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
          return(0);
          }

      /* Now deal with the VarBindList */
      if (decode_pkt_to_vblist(in_stream, &(rp->pdu.trap_pdu.trap_vbl),
                               asn1leng, rp) == -1) {
          return 0;
          }
      return rp;
      } /* if (ptype == TRAP_PDU) */
#endif /* (INSTALL_ENVOY_SNMP_VERSION_1) */

  /* if we get here it's a non-trap form of pdu */
  rp->pdu.std_pdu.request_id = A_DecodeIntegerWTC(in_stream, &asn1err,
                                        A_INTEGER, A_UNIVERSAL | A_PRIMITIVE);
  rp->pdu.std_pdu.error_status = A_DecodeIntegerWTC(in_stream, &asn1err,
                                        A_INTEGER, A_UNIVERSAL | A_PRIMITIVE);
  rp->pdu.std_pdu.error_index = A_DecodeIntegerWTC(in_stream, &asn1err,
                                        A_INTEGER, A_UNIVERSAL | A_PRIMITIVE);
  if (asn1err) {
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return(0);
      }

  /* Now deal with the VarBindList */
  if (decode_pkt_to_vblist(in_stream, &(rp->pdu.std_pdu.std_vbl),
                           asn1leng, rp) == -1) {
      return 0;
      }

  return rp;
}

#if INSTALL_ENVOY_SNMP_VERSION_USEC

int
  SNMP_Decode_Type_And_Reqid(SNMP_PKT_T *rp,
                             LCL_FILE   *in_stream)
{
ATVALUE_T type;
ALENGTH_T length;
int asn1err = 0;
INT_32_T reqid;

/* Decode the type, determine if we support it and if the
   indicated length is consistent with the remaining buffer */
if (A_DecodeTypeClass(in_stream) != (A_DEFAULT_SCOPE | A_CONSTRUCTOR)) {
    return(1);
    }
type = A_DecodeTypeValue(in_stream, &asn1err);
length = A_DecodeLength(in_stream, &asn1err);
if ((asn1err) || (type > MAX_PDU) || (type == TRAP_PDU) ||
    (Lcl_Size(in_stream) < (int)length))
    return(1);

rp->pdu_type   = type;
rp->pdu_length = length;

reqid = A_DecodeIntegerWTC(in_stream, &asn1err, A_INTEGER,
                           A_UNIVERSAL | A_PRIMITIVE);
if (asn1err)
    return(1);

rp->pdu.std_pdu.request_id = reqid;
return(0);
}

/****************************************************************************
NAME:  SNMP_Decode_Model_1_Parameters

PURPOSE:  Decode a model 1 parameters string, which has already been placed
          into the packet (in the community field), into it's components
          This routine doesn't make copies of the string valued
          sub components, instead it uses the memory from the parameters
          string.  This means that the sub components should NOT be
          freed or overwrittn and that the parameters string must not
          be freed until we are finished with the sub components.

          The format is:
          octets 
          1     Model (must be 1, but checked earlier)
          1     qos
          12    agentid
          4     agentboots
          4     agenttime
          2     max packet size
          1     userlen
          1 - 16        username
          1     authlen
          0 - 255       authdigest
          0 - 40        context

PARAMETERS:
        SNMP_PKT_T *    Packet we're filling in.
        UINT_32_T  *    Pointer to agentboots for returning the value
        UINT_32_T  *    Pointer to agenttime for returning the value
        OCTET_T    *    Pointer to qos for returning the value

RETURNS:  0 on success, non-zero on failure.
****************************************************************************/

static int
  SNMP_Decode_Model_1_Parameters(SNMP_PKT_T *rp,
                                 UINT_32_T  *agentboots,
                                 UINT_32_T  *agenttime,
                                 OCTET_T    *qos)
{
OCTET_T *pstring;
UINT_32_T oct1, oct2, oct3, oct4;
ALENGTH_T plen, tlen;

pstring = EBufferStart(&rp->community);
plen = EBufferUsed(&rp->community);

/* Check for minimum length, and step over the model field */
if (plen-- < 27)
    return(1);
pstring++;

/* get the qos, agentid, agentboots and agenttime fields */
*qos = *pstring++; plen--;
MEMCPY(rp->agentid, pstring, 12);
pstring += 12; plen -= 12;

oct1 = *pstring++;
oct2 = *pstring++;
oct3 = *pstring++;
oct4 = *pstring++;
plen -= 4;
*agentboots = (oct1 << 24) + (oct2 << 16) + (oct3 << 8) + oct4;

oct1 = *pstring++;
oct2 = *pstring++;
oct3 = *pstring++;
oct4 = *pstring++;
plen -= 4;
*agenttime = (oct1 << 24) + (oct2 << 16) + (oct3 << 8) + oct4;
if (*agenttime >= 0x80000000L)
    return(1);

oct1 = *pstring++;
oct2 = *pstring++;
plen -=2;
rp->maxpkt = (oct1 << 8) + oct2;

/* user name */
tlen = *pstring++; plen--;
/* Sanity check the length */
if ((tlen < 1) || (tlen > 16) || (tlen > plen))
    return(1);

EBufferPreLoad(BFL_IS_STATIC, &rp->user, pstring, tlen);
pstring += tlen; plen -= tlen;

if (plen == 0)
    return(1);
/* authentication digest */
tlen = *pstring++; plen--;
/* Sanity check the length, we will perform the digest checking elsewhere */
if ((tlen > plen) || (tlen > 255))
    return(1);

EBufferPreLoad(BFL_IS_STATIC, &rp->auth_key, pstring, tlen);
pstring += tlen; plen -= tlen;

/* Whatever is left (which may be nothing) is the context field */
if (plen > 40)
    return(1);

EBufferPreLoad(BFL_IS_STATIC, &rp->context, pstring, plen);

return(0);
}

/****************************************************************************
NAME:  Send_Report_PDU

PURPOSE:  Fill in the appropriate fields for a report pdu

PARAMETERS:

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, (SNMP_PKT_T *)0 on failure
****************************************************************************/

SNMP_PKT_T *
  Send_Report_PDU(SNMP_PKT_T *rp,
                  int         need_reqid,
                  LCL_FILE   *in_stream,
                  OCTET_T     qos,
                  OIDC_T     *obj,
                  int         objlen,
                  UINT_32_T   value,
                  int        *error_ret)
{
VBL_T *vblp;
OCTET_T *pstring;
UINT_32_T atime, userlen;

/* we only do report pdus if the report flag is set in the qos word */
if ((qos & USEC_REPORT) == 0)
    return(0);

/* If necessary & possible dig out the reqid */
if (need_reqid && !(qos & USEC_PRIV))
    (void) (SNMP_Decode_Type_And_Reqid(rp, in_stream));

/* Clean up any information about the current packet that
   we won't need, currently any varbinds we have decoded */
vblp = &rp->pdu.std_pdu.std_vbl;
Clean_vb_list(vblp);
rp->auth_routine = 0;

/* set up the packet as a report pdu with noauth/nopriv */
rp->snmp_version = SNMP_VERSION_USEC;
rp->pdu_type = REPORT_PDU;
rp->pdu.std_pdu.error_status = 0;
rp->pdu.std_pdu.error_index = 0;

userlen = EBufferUsed(&rp->user);
if ((pstring = (OCTET_T *)SNMP_memory_alloc(26 + userlen)) == 0)
    return(0);

MEMSET(pstring, 0, 26 + userlen);
pstring[0] = 1;
pstring[1] = 0;
MEMCPY(pstring + 2, usec_agentID, 12);
pstring[14] = (OCTET_T)((usec_agentBoots >> 24) & 0xFF);
pstring[15] = (OCTET_T)((usec_agentBoots >> 16) & 0xFF);
pstring[16] = (OCTET_T)((usec_agentBoots >> 8) & 0xFF);
pstring[17] = (OCTET_T)(usec_agentBoots & 0xFF);
atime = usec_offsetTime + SNMP_USECTIME;
pstring[18] = (OCTET_T)((atime >> 24) & 0xFF);
pstring[19] = (OCTET_T)((atime >> 16) & 0xFF);
pstring[20] = (OCTET_T)((atime >> 8) & 0xFF);
pstring[21] = (OCTET_T)(atime & 0xFF);
pstring[22] = 5;
pstring[23] = 0xdc;
pstring[24] = userlen;
MEMCPY(pstring + 25, EBufferStart(&rp->user), userlen);

/* clean up the old community */
EBufferClean(&rp->community);
/* and add in the new one */
EBufferPreLoad(BFL_IS_DYNAMIC, &rp->community, pstring, 26 + userlen);

/* allocate space for the var bind list and stuff the counter in */
if ((vblp->vblist = VarBindList_Allocate(1)) == 0)
    return(0);
vblp->vbl_count = 1;
if (SNMP_Bind_Unsigned_Integer(rp, 0, objlen, obj, VT_COUNTER, value))
    return(0);

/* tag this as requiring a response to be sent */
*error_ret = 1;
return(rp);
}

/****************************************************************************
NAME:  SNMP_Decode_USEC_Packet

PURPOSE:  Decode a USEC SNMP packet.

PARAMETERS:
        SNMP_PKT_T *    Packet we're filling in.
        LCL_FILE *      Stream to decode packet from. The stream is positioned
                        after the first tag field (the tag that says it's an
                        USEC packet.
        SNMPADDR_T *    Source of the packet
        SNMPADDR_T *    Destination of the packet (most likely
                        the address of the machine on which this
                        code is running.)
        int             asn1 length
        int        *    error return
RETURNS:  SNMP_PKT_T *  SNMP Packet structure, (SNMP_PKT_T *)0 on failure
****************************************************************************/

static SNMP_PKT_T *
  SNMP_Decode_USEC_Packet(SNMP_PKT_T *    rp,
                          LCL_FILE   *    in_stream,
                          SNMPADDR_T *    pktsrc,
                          SNMPADDR_T *    pktdst,
                          int             asn1leng,
                          int        *    error_ret)
{
ATVALUE_T     type;
ALENGTH_T     length;
int           asn1err = 0;
OCTET_T       flags, qos, *digestptr, *pstring, compdigest[16];
UINT_32_T     lcl_atime, rcv_atime, lcl_aboots, rcv_aboots;
USERLEAF_T    *user;
AGENTLEAF_T   *agent;
CONTEXTLEAF_T *context;
ACCESSLEAF_T  *access_entry;

/* Stuff the source & destination addresses into the packet structure */
MEMCPY(&(rp->pkt_src), pktsrc, sizeof(SNMPADDR_T));
MEMCPY(&(rp->pkt_dst), pktdst, sizeof(SNMPADDR_T));

/* set the reqid to the know max value in case we need to send a report pdu */
rp->pdu.std_pdu.request_id = 0x7FFFFFFFL;

/* Extract the parameters string, this is done by hand so we
   can get a pointer to the digest string in the message
   (we will need the pointer later to allow us to recompute
   the digest for authentication purposes) */

flags  = (A_DecodeTypeClass(in_stream));
type   = A_DecodeTypeValue(in_stream, &asn1err);
length = A_DecodeLength(in_stream, &asn1err);
if (asn1err || (flags != (A_UNIVERSAL | A_PRIMITIVE)) ||
    (type != A_OCTETSTRING) ||
    (length == 0) || (length > Lcl_Size(in_stream))) {
    SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
    return(0);
    }

/* Save a pointer to the beginning of the community string
   which we shall update to point at the digest later.
   Then adjust the in_stream pointer to be after the
   parameters string */
digestptr = Lcl_Next(in_stream);
Lcl_Seek(in_stream, length, 1);

/* Check the model, if it isn't 1 we don't know what it
   is, so we bump the error counter and bail out */
if (*digestptr != 1) {
    SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsBadParameters);
    return(0);
    }

if ((pstring = (OCTET_T *)SNMP_memory_alloc(length)) == 0)
    return(0);

/* Then copy out the parameters string */
MEMCPY(pstring, digestptr, length);
EBufferPreLoad(BFL_IS_DYNAMIC, &rp->community, pstring, length);
if (SNMP_Decode_Model_1_Parameters(rp, &rcv_aboots, &rcv_atime, &qos))  {
    SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsBadParameters);
    return(0);
    }
/* and if necessary reset the report flag in the qos field so
   we don't request a report */

*(pstring + 1) = *(pstring + 1) & ~(USEC_REPORT);

/* adjust the digestptr to point to the digest */
digestptr +=  26 + EBufferUsed(&rp->user);

/* lookup the agentid, if this is a manager we give the
   user the option of installing the agent information */
agent = SNMP_Agent_Lookup(rp->agentid);
if (agent == 0) {
    /*sar*/
    /*eventually add manager check*/
    SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnknownContexts);
    return(Send_Report_PDU(rp, 1, in_stream, qos,
                           USEC_UNKNOWNCONTEXTS, USECSTATSLENGTH,
                           snmp_stats.usecStatsUnknownContexts,
                           error_ret));
    return(0);
    }

/* Find the context in the local database */
context = SNMP_Context_Lookup(rp->agentid,
                              EBufferStart(&rp->context),
                              EBufferUsed(&rp->context));

if (context == 0) {
    SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnknownContexts);
    return(Send_Report_PDU(rp, 1, in_stream, qos,
                           USEC_UNKNOWNCONTEXTS, USECSTATSLENGTH,
                           snmp_stats.usecStatsUnknownContexts,
                           error_ret));
    }

/* Find the username in the local database */
user = SNMP_User_Lookup(rp->agentid,
                        EBufferStart(&rp->user),
                        EBufferUsed(&rp->user));
if (user == 0) {
    SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnknownUserNames);
    return(Send_Report_PDU(rp, 1, in_stream, qos,
                           USEC_UNKNOWNUSERNAMES, USECSTATSLENGTH,
                           snmp_stats.usecStatsUnknownUserNames, error_ret));
    }

/* Compare the requested qos field with the maximum qos field */
if ((SNMP_User_Get_Maxqos(user) < (qos & (USEC_PRIV | USEC_AUTH)))) {
    SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnsupportedQoS);
    return(Send_Report_PDU(rp, 1, in_stream, qos,
                           USEC_UNSUPPORTEDQOS, USECSTATSLENGTH,
                           snmp_stats.usecStatsUnsupportedQoS, error_ret));
    }

/* Now that we have the user we can decrypt the packet if necessary */
/* insert code to call decryption routines here */

if (SNMP_Decode_Type_And_Reqid(rp, in_stream)) {
    SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
    return(Send_Report_PDU(rp, 0, in_stream, qos,
                           SNMP_ASNPARSEERRORS, SNMPSTATSLENGTH,
                           snmp_stats.snmpInASNParseErrs, error_ret));
    }

rp->pdu.std_pdu.error_status = A_DecodeIntegerWTC(in_stream, &asn1err,
                                                  A_INTEGER,
                                                  A_UNIVERSAL | A_PRIMITIVE);
rp->pdu.std_pdu.error_index = A_DecodeIntegerWTC(in_stream, &asn1err,
                                                 A_INTEGER,
                                                 A_UNIVERSAL | A_PRIMITIVE);
if (asn1err) {
    /* asn1 parse error */
    SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
    return(Send_Report_PDU(rp, 0, in_stream, qos,
                           SNMP_ASNPARSEERRORS, SNMPSTATSLENGTH,
                           snmp_stats.snmpInASNParseErrs, error_ret));
    }

/* If qos indicates that this is an authenticated message we
   need to test out the digest information, the checks will be
   slightly different depending on the locality of the context */

/* Stuff the correct agentboots & agenttime into the parameters string,
   in case we want to send a reply to this packet */
    
lcl_aboots = agent->agentboots;
lcl_atime = SNMP_USECTIME + agent->offsettime;
pstring[14] = (OCTET_T)((lcl_aboots >> 24) & 0xFF);
pstring[15] = (OCTET_T)((lcl_aboots >> 16) & 0xFF);
pstring[16] = (OCTET_T)((lcl_aboots >> 8) & 0xFF);
pstring[17] = (OCTET_T)(lcl_aboots & 0xFF);
pstring[18] = (OCTET_T)((lcl_atime >> 24) & 0xff);
pstring[19] = (OCTET_T)((lcl_atime >> 16) & 0xff);
pstring[20] = (OCTET_T)((lcl_atime >>  8) & 0xff);
pstring[21] = (OCTET_T)(lcl_atime & 0xff);

if (qos & (USEC_PRIV | USEC_AUTH)) {
    /* Deal with the digest, first we need to get the user's 
       authentication key and place it into the byte string we
       are decoding, then we need to run the digest routine
       and compare the output */
    /* currently assume the md5 digest with a a 16 byte digest and key */
    rp->auth_routine = SNMP_User_Get_Auth(user);
    MEMCPY(digestptr, EBufferStart(SNMP_User_Get_Authkey(user)), 16);
    rp->auth_routine(Lcl_Start(in_stream), asn1leng,
                     EBufferStart(SNMP_User_Get_Authkey(user)), 16,
                     compdigest);
    /* zero out the key so that it isn't compromised */
    MEMSET(digestptr, 0, 16);
    if (MEMCMP(EBufferStart(&rp->auth_key), compdigest, 16) != 0) {
        SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsWrongDigestValues);
        if (SNMP_Context_Local(context)) {
            /* we only send a report if this is a local context */
            /* if desired generate trap request */
            return(Send_Report_PDU(rp, 0, in_stream, qos,
                                   USEC_WRONGDIGESTVALUES, USECSTATSLENGTH,
                                   snmp_stats.usecStatsWrongDigestValues,
                                   error_ret));
            }
        }

    /* Stuff the users key into the parameters string
       so it can be used for an outgoing packet */
    /* Eventually we also need to copy the priv key for later use */
    
    MEMCPY(pstring + 26 + EBufferUsed(&rp->user), 
           EBufferStart(SNMP_User_Get_Authkey(user)), 16);
    
    if (SNMP_Context_Local(context)) {
        if ((lcl_aboots == 0xffffffffL) || (lcl_aboots != rcv_aboots) ||
            (lcl_atime + 150 - rcv_atime > 300)) {
            SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsNotInWindows);
            /* if the report flags is set we send a report */
            if (qos & USEC_REPORT) {
                rp->pdu_type = REPORT_PDU;
                rp->pdu.std_pdu.error_status = 0;
                rp->pdu.std_pdu.error_index = 0;

                /* allocate space for the var bind list and
                   stuff the counter in */
                if ((rp->pdu.std_pdu.std_vbl.vblist =
                     VarBindList_Allocate(1)) == 0) 
                    return(0);
                rp->pdu.std_pdu.std_vbl.vbl_count = 1;
                if (SNMP_Bind_Unsigned_Integer(rp, 0, USECSTATSLENGTH,
                                        USEC_NOTINWINDOWS,
                                        VT_COUNTER,
                                        snmp_stats.usecStatsNotInWindows))
                    return(0);

                /* tag this as requiring a response to be sent */
                *error_ret = 1;
                return(rp);
                }
            return(0);
            }
        }
    else { /* remote context */
        if ((lcl_aboots == 0xffffffffL) || (lcl_aboots > rcv_aboots) ||
            ((lcl_aboots == rcv_aboots) && (lcl_atime > rcv_atime) &&
             (lcl_atime - rcv_atime > 150))) {
              SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsNotInWindows);
              /* we may want to invoke time sync options */
              return(0);
              }
        /* update the boots and time fields */
        if (agent->agentboots != rcv_aboots) {
            agent->agentboots = rcv_aboots;
            agent->offsettime = rcv_atime - SNMP_USECTIME;
            agent->lasttime = rcv_atime;
            }
        else {
            if (agent->lasttime <= rcv_atime) {
                agent->offsettime = rcv_atime - SNMP_USECTIME;
                agent->lasttime = rcv_atime;
                }
            }
        }
    } /* if (qos & (USEC_PRIV | USEC_AUTH)) */

/* Now deal with the VarBindList */
if (decode_pkt_to_vblist(in_stream, &(rp->pdu.std_pdu.std_vbl),
                         asn1leng, rp) == -1) {
    /*sar*/
    /* add report pdu stuff */
    return(0);
    }

switch(rp->pdu_type) {
    case GET_REQUEST_PDU:
    case GET_NEXT_REQUEST_PDU:
    case SET_REQUEST_PDU:
    case GET_BULK_REQUEST_PDU:
        if (SNMP_Context_Remote(context)) {
            SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnauthorizedOperations);
            return(Send_Report_PDU(rp, 0, in_stream, qos,
                                   USEC_UNAUTHORIZEDOPERATIONS,
                                   USECSTATSLENGTH,
                                   snmp_stats.usecStatsUnauthorizedOperations,
                                   error_ret));
            }

        /* If this is an agent we need to look up the access rights
           for <user><context><qos>, determine that the operation
           is allowed and what the views should be */

        access_entry = SNMP_Access_Lookup (rp->agentid, 
                                           EBufferStart(&rp->user),
                                           EBufferUsed(&rp->user),
                                           EBufferStart(&rp->context),
                                           EBufferUsed(&rp->context),
                                           qos);
        if (access_entry &&
            ((1<<(rp->pdu_type)) & SNMP_Access_Get_Privileges(access_entry))) {
            if (rp->pdu_type == SET_REQUEST_PDU)
                rp->view_index = SNMP_Access_Get_WriteIndex(access_entry);
            else
                rp->view_index = SNMP_Access_Get_ReadIndex(access_entry);
            if (SNMP_Context_Proxy(context)) {
                /* get proxy info, if not enough info */
                /*sar*/ /*
                  SGRP_v2_INC_COUNTER(snmp_stats.snmpProxyDrops);
                  return(Send_Report_PDU(rp, 0, in_stream, qos,
                           SNMP_PROXYDROPS, SNMPSTATSLENGTH,
                           snmp_stats.snmpProxyDrops, error_ret)); */
                }
            }
        else {
            SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnauthorizedOperations);
            return(Send_Report_PDU(rp, 0, in_stream, qos,
                                   USEC_UNAUTHORIZEDOPERATIONS,
                                   USECSTATSLENGTH,
                                   snmp_stats.usecStatsUnauthorizedOperations,
                                   error_ret));
            }
        break;

    case REPORT_PDU:
/* if report appropriate maintenance function is invoked */
        break;

    case INFORM_REQUEST_PDU:
    case TRAP2_PDU:
        if (SNMP_Context_Local(context)) {
            SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnauthorizedOperations);
            return(Send_Report_PDU(rp, 0, in_stream, qos,
                                   USEC_UNAUTHORIZEDOPERATIONS,
                                   USECSTATSLENGTH,
                                   snmp_stats.usecStatsUnauthorizedOperations,
                                   error_ret));
            }
        if (SNMP_Context_Proxy(context)) {
            /* attempt to find all proxy options */
            }
        break;

    case GET_RESPONSE_PDU:
        if ((SNMP_Context_Local(context)) && !(SNMP_Context_Proxy(context))) {
            SGRP_USEC_INC_COUNTER(snmp_stats.usecStatsUnauthorizedOperations);
            return(Send_Report_PDU(rp, 0, in_stream, qos,
                                   USEC_UNAUTHORIZEDOPERATIONS,
                                   USECSTATSLENGTH,
                                   snmp_stats.usecStatsUnauthorizedOperations,
                                   error_ret));
            }
        /* decode and return, if it's a proxy it should get picked up
           by the proxy return code */
        break;
    default:
        /* asn1 parse error */
        SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
        return(Send_Report_PDU(rp, 0, in_stream, qos,
                               SNMP_ASNPARSEERRORS, SNMPSTATSLENGTH,
                               snmp_stats.snmpInASNParseErrs, error_ret));
    }

return(rp);
}
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_USEC */

/****************************************************************************
NAME:  SNMP_Decode_Packet_WER

PURPOSE:  Decode an SNMP packet with an optional error return.

PARAMETERS:
        unsigned char * Address of the packet
        int             length of the packet
        SNMPADDR_T *    Source of the packet
        SNMPADDR_T *    Destination of the packet (most likely
                        the address of the machine on which this
                        code is running.)
        int        *    error return, if it is (int *)0, don't
                        return an error.

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, (SNMP_PKT_T *)0 on failure
****************************************************************************/
SNMP_PKT_T *
  SNMP_Decode_Packet_WER(unsigned char  * pktp,
                         int              pktl,
                         SNMPADDR_T     * pktsrc,
                         SNMPADDR_T     * pktdst,
                         int            * error_ret)
{
  SNMP_PKT_T    *rp, *ret_rp;
  ATVALUE_T     type;
  ALENGTH_T     overall_length;
  OCTET_T       flags;
  LCL_FILE      *in_stream, in_pkt_stream;
  int           asn1leng, temperror, asn1err = 0;
  INT_32_T      snmp_version;

  /* if the caller did not supply error_ret set that up and
     init it to the no error state */
  if (error_ret == 0)
      error_ret = &temperror;
  *error_ret = 0;

  /* Set up packet as a local stream. */
  if ((in_stream = Lcl_Open(&in_pkt_stream, pktp, pktl)) == 0)
      return 0;
  
  /* Decode the top-level message sequence... */
  flags          = A_DecodeTypeClass(in_stream);
  type           = A_DecodeTypeValue(in_stream, &asn1err);
  overall_length = A_DecodeLength(in_stream, &asn1err);
  if (asn1err) {
      Lcl_Close(in_stream);
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return(0);
      }

  /* Validate that the length provided by the caller is consistent with */
  /* the length given by the ASN.1 wrapper...                           */
  /* If necessary, shrink the local I/O buffer to match.                */
  asn1leng = Lcl_Tell(in_stream) + (int)(overall_length);

  if (asn1leng < pktl) {
      (void) Lcl_Resize(in_stream, asn1leng, 0);
      }
  else {
      if (asn1leng > pktl) {
          Lcl_Close(in_stream);
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
          return(0);
          }
      }

  /* Test the flags & type info to see if we have something that
     looks like the right kind of packet */

  if ((flags | type) != (A_UNIVERSAL | A_CONSTRUCTOR | A_SEQUENCE)) {
      /* oh well, this isn't an snmp packet, bail out */
      Lcl_Close(in_stream);
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return(0);
      }

  /* get the version stamp */
  snmp_version = A_DecodeIntegerWTC(in_stream, &asn1err, A_INTEGER,
                                    A_UNIVERSAL | A_PRIMITIVE);
  if (asn1err) {
      Lcl_Close(in_stream);
      SGRPv1v2_INC_COUNTER(snmp_stats.snmpInASNParseErrs);
      return(0);
      }

  /* Get the SNMP packet structure. */
  if ((rp = SNMP_Allocate()) == 0) {
      Lcl_Close(in_stream);
      return(0);
      }

  /* If the lock code is installed acquire the generic read lock */
#if (INSTALL_ENVOY_SNMP_LOCK)
  if (ENVOY_SNMP_GET_READ_LOCK(SNMP_CoarseLock)) {
      BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
          (BUG_OUT,
           "SNMP_Decode_Packet_WER: coarse lock is broken",
           0));

      SNMP_Free(rp);
      Lcl_Close(in_stream);
      return(0);
      }
  rp->coarse_lock = &SNMP_CoarseLock;
  rp->lockflags = LOCK_READ;
#endif


  rp->snmp_version = snmp_version;
  switch(snmp_version) {
#if INSTALL_ENVOY_SNMP_VERSION_1
      case SNMP_VERSION_1:
          ret_rp = SNMP_Decode_V1_Packet(rp, in_stream, pktsrc, pktdst,
                                         asn1leng);
          break;
#endif
#if INSTALL_ENVOY_SNMP_VERSION_2
      case SNMP_VERSION_2:
          ret_rp = SNMP_Decode_V1_Packet(rp, in_stream, pktsrc, pktdst,
                                         asn1leng);
          break;
#endif
#if INSTALL_ENVOY_SNMP_VERSION_USEC
      case SNMP_VERSION_USEC:
          ret_rp = SNMP_Decode_USEC_Packet(rp, in_stream, pktsrc, pktdst,
                                           asn1leng, error_ret);
          break;
#endif
      default:
          SGRPv1v2_INC_COUNTER(snmp_stats.snmpInBadVersions);
          ret_rp = 0;
          break;
      }

  if (ret_rp == 0){
      SNMP_Free(rp);
      }
  Lcl_Close(in_stream);
  return(ret_rp);
}
