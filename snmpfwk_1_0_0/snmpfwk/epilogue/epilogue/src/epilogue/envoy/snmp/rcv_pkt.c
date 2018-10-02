/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/rcv_pkt.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: rcv_pkt.c,v $
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
 * Revision 7.3  1997/10/16 23:51:49  sar
 * In async_compat_io the code included if SNMP_user_get_encode_buffer
 * was defined used the wrong name for the packet variable and attempted
 * to return an error value rather than saving it into the compatibility
 * block.
 *
 * Revision 7.2  1997/03/20 06:49:07  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:47:52  sra
 * Release 6.0.
 *
 * Revision 5.3  1995/03/21  00:30:26  sar
 * Made Process_Received_SNMP_Packet into a glue routine, allowing
 * the user to continue using it for backwards compatibility.  The real
 * work is done by the async processing routine.
 * Several other routines were written to provide the necessary async
 * functions.
 *
 * Revision 5.2  1994/07/29  18:05:34  sar
 * Replaced incrementing snmpInBadTypes with incrementing snmpInASNParseErrs.
 * Removed some casts of 0 as they aren't needed.
 *
 * Revision 5.1  1994/07/22  19:50:40  sar
 * Added a missing open bracket ({), at line 307.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.2  1994/04/29  21:50:48  sar
 * Modified the party/context manipulation code to take advantage of
 * the oids stored in the packet structure.
 *
 * Revision 4.1  1993/09/30  20:35:37  sar
 * Corrected the couting of snmpOutGetResponses.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.12  1993/06/23  00:04:16  sar
 * Made ENABLE_PROXY be just PROXY in the install defs
 *
 * Revision 3.11  1993/06/07  18:26:04  sar
 * Modifed SNMP_Decode_Packet to be SNMP_Deocde_Packet_WER to allow
 * the decode routines to return an error (with error return) that
 * indicates the caller should send the packet back as a response.
 *
 * Revision 3.10  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.9  1993/05/03  17:16:12  sar
 * Added routines for local time, removed {party context} oids from packet
 * structure, added asn1len to {party context} structures and
 * SNMP_{Party Context}_Name where needed to lookup the name for a given
 * {Party Context}.
 *
 * Revision 3.8  1993/04/30  22:54:08  sar
 * Added mechanism for minimial proxies and coarse grained locks.  The
 * latter required the addition of routines to get the name of a leaf given
 * a pointer to the leaf.
 *
 * Revision 3.7  1993/04/28  22:02:29  sar
 * started adding proxy capabilities.
 *
 * Revision 3.6  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.5  1993/03/05  03:58:02  dab
 * fixed up types
 *
 * Revision 3.4  1993/03/01  20:51:30  sar
 * moved some definitions to allow clean version 1 compiles.
 *
 * Revision 3.3  1993/02/26  17:33:20  sar
 * Before calling the set routine swap the src and dest party and oid info
 * so that the set will enocde the packet correctly.
 *
 * Revision 3.2  1993/02/24  01:02:41  sar
 * Changed many call to use OIDC_T/lenght pairs instead of OBJ_ID_T as now
 * we don't need to specifically allocate the OBJ_ID_T.
 * Removed the name objid from the PARTYLEAF_T as the name may be determined from
 * the tree itself.  For efficiency purposes put the dest/src party names into
 * the pkt so we don't have to look them up frequently.
 *
 * Revision 3.1  1993/02/17  21:06:50  sar
 * Removed #define then and uses of it.
 * Updated stats to deal with v1 and v2 (or both or neither).
 * Updated ifdef compile for v1 or v2.
 * Updated to deal with v2 packets.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.103  91/10/30  20:42:08  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.102  91/08/15  12:31:08  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.101  91/08/14  10:47:26  dab
 * Added trace functions that Karl put in as macros.
 * 
 * Revision 2.100  91/08/09  14:08:24  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:48  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:52
 * Release 2.00
 * 
 *    Rev 1.9   14 Dec 1989 16:00:54
 * Added support for Borland Turbo C compiler
 * 
 *    Rev 1.8   27 Apr 1989 15:56:28
 * Removed unused variables
 * 
 *    Rev 1.7   04 Mar 1989 13:22:58
 * When SNMP_Decode_Packet returned an error, an attempt was made to
 * free a non-existent packet structure, causing chaos.
 * 
 *    Rev 1.6   11 Jan 1989 12:27:40
 * Fix Process_Received_SNMP_Packet to reject any but REQUEST packets.
 * 
 *    Rev 1.5   19 Sep 1988 17:26:44
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.4   17 Sep 1988 20:41:54
 * A user-supplied routine is now called to generate the authentication
 * trap packet.
 * 
 *    Rev 1.3   17 Sep 1988 12:20:44
 * Moved packet validation macros out of rcv_pkt.c into libfuncs.h.
 * 
 *    Rev 1.2   15 Sep 1988 20:05:06
 * After parsing an erroneous packet, the packet structure was not being freed.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:20
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:47:06
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <snmpdefs.h>
#include <snmp.h>

typedef struct COMPAT_BLOCK_S {
        int              flags;
        EBUFFER_T       *ebufp;
        } COMPAT_BLOCK_T;

/****************************************************************************
NAME:  async_compat_error

PURPOSE:  An error completion routine for use with the backwards compatibility
          routine for Process_SNMP_Received_Packet.  It puts the error
          code into a structure that the received packet specified as
          the private info.  An error code of 0 indicates that there
          wasn't an error but the packet shouldn't be sent (most likely
          due to proxy processing).

PARAMETERS:
        SNMPADDR_T *    Foreign address of  packet
        SNMPADDR_T *    Local address of packet
        int             The error code
        PTR_T           Pointer to private information block
                        in this case a COMPAT_BLOCK_T.

RETURNS:  nothing
****************************************************************************/

void 
  async_compat_error(SNMPADDR_T *for_addr,
                     SNMPADDR_T *loc_addr,
                     int         ecode,
                     PTR_T       cookie)
{
if (ecode)
    ((COMPAT_BLOCK_T *)cookie)->flags = -1;
else
    ((COMPAT_BLOCK_T *)cookie)->flags = 1;
}

/****************************************************************************
NAME:  async_compat_io

PURPOSE:  An io completion routine for use with the backwards compatibility
          routine for Process_SNMP_Received_Packet.  It attempts to
          encode the packet into the ebuffer given to the received routine.
          It finds this ebuffer in the private information area.  If
          the encode fails it puts an error code into the private area.

PARAMETERS:
        SNMPADDR_T *    Foreign address of  packet
        SNMPADDR_T *    Local address of packet
        PTR_T           Pointer to the packet structure
        ALENGTH_T       Max space the packet will require
        PTR_T           Pointer to private information block
                        in this case a COMPAT_BLOCK_T.

RETURNS:  nothing
****************************************************************************/

void 
  async_compat_io(SNMPADDR_T     *for_addr,
                  SNMPADDR_T     *loc_addr,
                  PTR_T           pktp,
                  ALENGTH_T       need,
                  PTR_T           cookie)
{
SNMP_PKT_T *pkt = (SNMP_PKT_T *)pktp;
EBUFFER_T *ebuffp;
ebuffp = ((COMPAT_BLOCK_T *)cookie)->ebufp;

/* Give the user a chance to allocate space from other areas */
#if defined(SNMP_user_get_encode_buffer)
if (ebuffp->start_bp == 0)
    if (SNMP_user_get_encode_buffer(pkt, need, ebuffp) != 0) {
        ((COMPAT_BLOCK_T *)cookie)->flags = -1;
        return;
        }
#endif

/* Attempt to encode the packet into the buffer, note that the buf size
   required has already been worked out in need */
if (SNMP_Process_Finish(pkt, ebuffp, need))
    ((COMPAT_BLOCK_T *)cookie)->flags = -1;
else
    ((COMPAT_BLOCK_T *)cookie)->flags = 0;
return;
}

/****************************************************************************
NAME:  Process_Received_SNMP_Packet

PURPOSE:  Manage the decoding of an incoming SNMP packet.

PARAMETERS:
        unsigned char * Address of the packet
        int             length of the packet
        SNMPADDR_T *    Source of the packet
        SNMPADDR_T *    Destination of the packet (most likely
                        the address of the machine on which this
                        code is running.)
        EBUFFER_T *     Buffer to hold any generated result packet.
                        Space to hold the packet will be allocated if
                        necessary.
                        The EBUFFER_T structure must have been initialized
                        using EBufferInitialize() or been setup using
                        EBufferSetup().

RETURNS:  int           0: A good response of some type is in the result
                           buffer.
                        -1: An error, the result buffer contents should be
                            discarded.
                        1: No error and no response packet, this may occur
                           if a proxy routine is called and chooses to send
                           a packet itself (perhaps to the final target)
NOTE: The result packet may be any valid SNMP response packet.
****************************************************************************/

int
  Process_Received_SNMP_Packet(unsigned char *  pktp,
                               int              pktl,
                               SNMPADDR_T *     pktsrc,
                               SNMPADDR_T *     pktdst,
                               EBUFFER_T *      rebuffp)
{
COMPAT_BLOCK_T cblock;
ALENGTH_T pktsize;

cblock.flags = 0;
cblock.ebufp = rebuffp;

if ((pktsize = EBufferRemaining(rebuffp)) == 0)
    pktsize = SNMP_MAX_PACKET_SIZE;

Process_Rcvd_SNMP_Packet_Async((size_t)pktl, (bits8_t *)pktp, pktsrc,
                               pktdst, pktsize, async_compat_io,
                               async_compat_error, (PTR_T)&cblock);
return(cblock.flags);
}

