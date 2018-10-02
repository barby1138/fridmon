/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/creatreq.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 * $Log: creatreq.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:38  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:47  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:50  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.2  1996/01/10  16:34:47  sar
 * usec updates
 *
 * Revision 6.1  1995/10/20  23:02:48  sar
 * removed no_pp stuff
 * removed create_v2_request & create_half_trap
 *
 * Revision 6.0  1995/05/31  21:47:19  sra
 * Release 6.0.
 *
 * Revision 5.2  1995/03/21  00:25:01  sar
 * Use the new party names (for & loc instaead of src & dst)
 *
 * Revision 5.1  1994/10/03  21:22:28  sar
 * Added the function SNMP_Create_V2_Half_Trap.  This routine
 * create a v2 type snmp_pkt_t set up for a v2 trap but without
 * any party or context information.  This packet is for use with
 * the create_and_send_traps routines.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.9  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.8  1993/05/17  14:03:35  sar
 * changed SNMP_Create_Party_Request to be SNMP_Create_V2_Request
 *
 * Revision 3.7  1993/05/03  17:16:12  sar
 * Added routines for local time, removed {party context} oids from packet
 * structure, added asn1len to {party context} structures and
 * SNMP_{Party Context}_Name where needed to lookup the name for a given
 * {Party Context}.
 *
 * Revision 3.6  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.5  1993/04/19  18:28:34  sar
 * cleanup, mostly some correcting some non-ansi misspellings and adding
 * static defs.
 *
 * Revision 3.4  1993/03/03  03:17:00  sar
 * Updated to deal with shadow rows.  I have defined a new enumeration for
 * row status (RS_RSNE = 0) which means that row status doesn't exist yet.
 * Most routines now check to see if the row is active before using it.
 * Also added SNMP_SILLY_MARKETING_MACRO as per Rob's request.
 *
 * Revision 3.3  1993/02/24  01:02:41  sar
 * Changed many call to use OIDC_T/lenght pairs instead of OBJ_ID_T as now
 * we don't need to specifically allocate the OBJ_ID_T.
 * Removed the name objid from the PARTYLEAF_T as the name may be determined from
 * the tree itself.  For efficiency purposes put the dest/src party names into
 * the pkt so we don't have to look them up frequently.
 *
 * Revision 3.2  1993/02/17  21:02:06  sar
 * Added SNMP_Create_Party_Request to create and fill in packet structures
 * for v2.  Moved some of the common code between v1 and v2 into a seperate
 * routine to shrink code size.
 *
 * Revision 3.1  92/04/28  10:37:08  dab
 * Fixed comments to reference snmpdefs.h instead of snmp.h
 * 
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.102  91/10/30  20:41:56  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.101  91/08/15  12:30:57  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:19  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:33  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:42
 * Release 2.00
 * 
 *    Rev 1.0   11 Jan 1989 12:11:20
 * Initial revision.
 *
 * Separated from buildpkt.c on January 11, 1989.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <buildpkt.h>

#if INSTALL_ENVOY_SNMP_VERSION_USEC
#include <agent.h>
#include <user.h>
#endif

/****************************************************************************
NAME:  create_request

PURPOSE:  Allocate an SNMP request packet and the vbl list for it.

PARAMETERS:
        int             Number of VarBindList elements needed (may be 0)

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, 0 on failure
****************************************************************************/
SNMP_PKT_T *
  create_request(int num_vb)
{
SNMP_PKT_T      *rp;

if ((rp = SNMP_Allocate()) == 0)
    return 0;

/* Is the VarBindList empty ? */
if ((rp->pdu.std_pdu.std_vbl.vbl_count = num_vb) == 0) /* yes */
    rp->pdu.std_pdu.std_vbl.vblist = 0;

else { /* No, the VarBindList has contents */
    if ((rp->pdu.std_pdu.std_vbl.vblist = VarBindList_Allocate(num_vb)) == 0) {
        SNMP_Free(rp);
        return 0;
        }
    }
return rp;
}

/****************************************************************************
NAME:  SNMP_Create_Request2

PURPOSE:  Begin building an SNMP request packet.

PARAMETERS:
        int             Packet type:
                        For version 1 GET_REQUEST_PDU, GET_NEXT_REQUEST_PDU or
                                      SET_REQUEST_PDU
                        For version 2 GET_REQUEST_PDU, GET_NEXT_REQUEST_PDU,
                                      SET_REQUEST_PDU, GET_BULK_REQUEST_PDU,
                                      INFORM_REQUEST_PDU, TRAP2_PDU
                        as defined in snmpdefs.h
        int             Protocol version -- SNMP_VERSION_1 or SNMP_VERSION_2
                        as defined in snmpdefs.h
        int             Community name length
        char *          Community name (Must be static or global)
        INT_32_T        Request ID
        int             Number of VarBindList elements needed (may be 0)
        int             nonreps - for use with getbulk, number of non repeaters
        int             maxreps - for use with getbulk, max repetitions

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, 0 on failure
****************************************************************************/
SNMP_PKT_T *
  SNMP_Create_Request2(int       ptype,
                       int       version,
                       int       commleng,
                       char     *community,
                       INT_32_T  request_id,
                       int       num_vb,
                       int       nonreps,
                       int       maxreps)

{
SNMP_PKT_T *rp;

/* Deteremine if the pdu type is acceptable for the requested version */
switch (version) {
#if INSTALL_ENVOY_SNMP_VERSION_1
    case SNMP_VERSION_1:
        if ((ptype != GET_REQUEST_PDU) &&
            (ptype != GET_NEXT_REQUEST_PDU) &&
            (ptype != SET_REQUEST_PDU))
            return 0;
        break;
#endif /* INSTALL_ENVOY_SNMP_VERSION_1 */
#if INSTALL_ENVOY_SNMP_VERSION_2
    case SNMP_VERSION_2:
        if ((ptype != GET_REQUEST_PDU)      &&
            (ptype != GET_NEXT_REQUEST_PDU) &&
            (ptype != SET_REQUEST_PDU)      && 
            (ptype != GET_BULK_REQUEST_PDU) &&
            (ptype != INFORM_REQUEST_PDU)   &&
            (ptype != TRAP2_PDU))
            return 0;
        break;
#endif /* INSTALL_ENVOY_SNMP_VERSION_2 */
    default: /* unknown version number */
        return(0);
    }

if ((rp = create_request(num_vb)) == 0)
    return 0;

rp->snmp_version                = version;
rp->pdu_type                    = (ATVALUE_T)ptype;
rp->pdu.std_pdu.request_id      = request_id;
if (ptype == GET_BULK_REQUEST_PDU) {
    rp->pdu.std_pdu.error_status = nonreps;
    rp->pdu.std_pdu.error_index  = maxreps;
    }
else {
    rp->pdu.std_pdu.error_status = 0;
    rp->pdu.std_pdu.error_index  = 0;
    }

EBufferPreLoad(BFL_IS_STATIC, &(rp->community), community, commleng);

return rp;
}

#if INSTALL_ENVOY_SNMP_VERSION_USEC
/****************************************************************************
NAME:  SNMP_Create_Request_USEC

PURPOSE:  Begin building an SNMP USEC packet.

PARAMETERS:
        int             Packet type: GET_REQUEST_PDU, GET_NEXT_REQUEST_PDU,
                                     SET_REQUEST_PDU, GET_BULK_REQUEST_PDU,
                                     INFORM_REQUEST_PDU, TRAP2_PDU
                        as defined in snmpdefs.h
        int             Protocol version -- SNMP_VERSION_USEC
                        as defined in snmpdefs.h
        OCTET_T         qos choice
        OCTET_T[12]     agentid
        int             User name length
        char *          User name
        int             Context name length
        char *          Context name
        INT_32_T        Request ID
        int             Number of VarBindList elements needed (may be 0)
        int             nonreps - for use with getbulk, number of non repeaters
        int             maxreps - for use with getbulk, max repetitions

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, 0 on failure
****************************************************************************/
SNMP_PKT_T *
  SNMP_Create_Request_USEC(int       ptype,
                           int       version,
                           int       maxsize,
                           OCTET_T   qos,
                           OCTET_T  *agentid,
                           int       uname_len,
                           OCTET_T  *uname,
                           int       cname_len,
                           OCTET_T  *cname,
                           INT_32_T  request_id,
                           int       num_vb,
                           int       nonreps,
                           int       maxreps)

{
SNMP_PKT_T *rp;
AGENTLEAF_T *agent;
USERLEAF_T *user;
OCTET_T *parameters;
int param_len, auth_len;
UINT_32_T atemp;

/* Deteremine if the pdu type is acceptable for the requested version */
if ((ptype > MAX_PDU) || (ptype == GET_RESPONSE_PDU) ||
    (ptype == TRAP_PDU) || (ptype == REPORT_PDU)) 
    return 0;

/* size check the string arguments */
if ((uname_len > 16) || (cname_len > 40))
    return 0;

if ((agent = SNMP_Agent_Lookup(agentid)) == 0)
    return 0;

if ((user = SNMP_User_Lookup(agentid, uname, uname_len)) == 0)
    return 0;
/* see if we can provide the requested qos */
if ((qos & (USEC_AUTH | USEC_PRIV)) > SNMP_User_Get_Maxqos(user))
    return (0);
if (qos & (USEC_AUTH | USEC_PRIV)) {
    if (SNMP_User_Get_Auth(user))
        auth_len = EBufferUsed(SNMP_User_Get_Authkey(user));
    else
        return (0);
    }
else
    auth_len = 0;

param_len = 26 + uname_len + auth_len + cname_len;
if ((parameters = SNMP_memory_alloc(param_len)) == 0)
    return 0;

if ((rp = create_request(num_vb)) == 0) {
    SNMP_memory_free(parameters);
    return 0;
    }

rp->snmp_version                = version;
rp->pdu_type                    = (ATVALUE_T)ptype;
rp->pdu.std_pdu.request_id      = request_id;
if (ptype == GET_BULK_REQUEST_PDU) {
    rp->pdu.std_pdu.error_status = nonreps;
    rp->pdu.std_pdu.error_index  = maxreps;
    }
else {
    rp->pdu.std_pdu.error_status = 0;
    rp->pdu.std_pdu.error_index  = 0;
    }

parameters[0] = 1;
parameters[1] = qos;
MEMCPY(parameters + 2, agentid, 12);
atemp = agent->agentboots;
parameters[14] = (OCTET_T)((atemp >> 24) & 0xFF);
parameters[15] = (OCTET_T)((atemp >> 16) & 0xFF);
parameters[16] = (OCTET_T)((atemp >> 8) & 0xFF);
parameters[17] = (OCTET_T)(atemp & 0xFF);
atemp = agent->offsettime + SNMP_USECTIME;
parameters[18] = (OCTET_T)((atemp >> 24) & 0xFF);
parameters[19] = (OCTET_T)((atemp >> 16) & 0xFF);
parameters[20] = (OCTET_T)((atemp >> 8) & 0xFF);
parameters[21] = (OCTET_T)(atemp & 0xFF);
parameters[22] = (OCTET_T)((maxsize >> 8) & 0xFF);
parameters[23] = (OCTET_T)(maxsize & 0xFF);
parameters[24] = uname_len & 0xFF;
MEMCPY(parameters + 25, uname, uname_len);
parameters[25 + uname_len] = auth_len;
if (auth_len) {
    MEMCPY(parameters + 26 + uname_len,
           EBufferStart(SNMP_User_Get_Authkey(user)), auth_len);
    rp->auth_routine = SNMP_User_Get_Auth(user);
    }

if (cname_len)
    MEMCPY(parameters + 26 + uname_len + auth_len, cname, cname_len);

EBufferPreLoad(BFL_IS_DYNAMIC, &(rp->community), parameters, param_len);
EBufferPreLoad(BFL_IS_STATIC, &(rp->user), parameters + 25, uname_len);
EBufferPreLoad(BFL_IS_STATIC, &(rp->auth_key), parameters + 26 + uname_len,
               auth_len);
EBufferPreLoad(BFL_IS_STATIC, &(rp->context),
               parameters + 26 + uname_len + auth_len, cname_len);

return rp;
}
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_USEC */

