/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/creatrap.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 * $Log: creatrap.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:46  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:49  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.2  1995/11/01  00:46:48  sar
 * Added a version check, create_trap_request is only useable for version 1
 *
 * Revision 6.1  1995/10/20  22:59:09  sar
 * removed no_pp stuff & casts of 0
 * modified memcpy to MEMCPY
 *
 * Revision 6.0  1995/05/31  21:47:18  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/10/12  20:20:28  sar
 * trap_time_ticks is now a uint so we don't have to cast timestamp to
 * an int.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.3  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.2  1993/04/26  20:37:09  sar
 * Added ifdefs to allow clean makes of version 1 and 2, added bit strings,
 * arrange for deletion of acls with the party or context they refer to
 * is deleted.
 *
 * Revision 3.1  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.0  1992/04/03  19:52:37  dab
 * Release 3.0
 *
 * Revision 2.103  91/10/30  20:41:55  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.102  91/08/15  12:30:57  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.101  91/08/12  12:47:37  dab
 * Cast arguments to memcpy() and memset() to PTR_T.
 * 
 * Revision 2.100  91/08/09  14:08:18  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:32  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:42
 * Release 2.00
 * 
 *    Rev 1.1   17 Mar 1989 21:41:50
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.0   11 Jan 1989 12:11:22
 * Initial revision.
 *
 * Separated from buildpkt.c on January 11, 1989.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <objectid.h>
#include <buildpkt.h>

#if INSTALL_ENVOY_SNMP_VERSION_1
/****************************************************************************
NAME:  SNMP_Create_Trap

PURPOSE:  Begin building an SNMP trap packet.

PARAMETERS:
        int             Protocol version -- must be SNMP_VERSION_1 as
                        defined in snmp.h
        int             Community name length
        char *          Community name (Must be static or global)
        int             Number of components in enterprise object id
        OIDC_T *        Components of the enterprise id
        OCTET_T *       An IP address of this device, consists of four
                        bytes of IP address in standard network order
        int             Generic trap type
        INT_32_T        Specific trap type
        UINT_32_T       Time of trap
        int             Number of VarBindList elements needed (may be 0)

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, (SNMP_PKT_T *)0 on failure
****************************************************************************/
SNMP_PKT_T *
  SNMP_Create_Trap(int        version,
                   int        commleng,
                   char      *community,
                   int        enterprz_c,
                   OIDC_T    *enterprz_l,
                   OCTET_T   *agent_ip,
                   int        generic,
                   INT_32_T   specific,
                   UINT_32_T  timestamp,
                   int        num_vb)
{
SNMP_PKT_T      *rp;

if (version != SNMP_VERSION_1)
    return 0;

if ((rp = SNMP_Allocate()) == 0)
    return 0;

rp->pdu_type = TRAP_PDU;

rp->snmp_version = SNMP_VERSION_1;
if (build_object_id(enterprz_c, enterprz_l,
                    &(rp->pdu.trap_pdu.enterprise_objid)) == -1) {
    SNMP_Free(rp);
    return 0;
    }

MEMCPY((PTR_T)rp->pdu.trap_pdu.net_address, (PTR_T)agent_ip, 4);
rp->pdu.trap_pdu.generic_trap = generic;
rp->pdu.trap_pdu.specific_trap = specific;
rp->pdu.trap_pdu.trap_time_ticks = timestamp;

EBufferPreLoad(BFL_IS_STATIC, &(rp->community), community, commleng);

if ((rp->pdu.trap_pdu.trap_vbl.vbl_count = num_vb) == 0) {
    /* Handle case where the VarBindList is empty */
    rp->pdu.trap_pdu.trap_vbl.vblist = 0;
    }
else { /* The VarBindList has contents */
    if ((rp->pdu.trap_pdu.trap_vbl.vblist = VarBindList_Allocate(num_vb))
          == 0) {
        SNMP_Free(rp);
        return 0;
        }
    }
return rp;
}
#endif /* INSTALL_ENVOY_SNMP_VERSION_1 */
