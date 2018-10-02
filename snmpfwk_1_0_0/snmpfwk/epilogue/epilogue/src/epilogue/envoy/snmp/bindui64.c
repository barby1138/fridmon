/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/bindui64.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 *  Copyright 1993-1997 Epilogue Technology Corporation.
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
 * $Log: bindui64.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:43  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:41  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  22:59:09  sar
 * removed no_pp stuff & casts of 0
 * modified memcpy to MEMCPY
 *
 * Revision 6.0  1995/05/31  21:47:12  sra
 * Release 6.0.
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <objectid.h>
#include <buildpkt.h>
#include <ui64.h>

/****************************************************************************
NAME:  SNMP_Bind_64_Unsigned_Integer

PURPOSE:  Bind an unsigned 64 bit integer into a VarBindList

PARAMETERS:
        SNMP_PKT_T *    Packet structure on which the data is to be bound
        int             Index to the particular VarBind to be used.
                                NOTE: This index zero based.
        int             Number of components in the data's object identifier
        OIDC_T *        Components of the data's object identifier
        OCTET_T         Data type, VT_COUNTER64
        UINT_64_T *     The value to be bound.

RETURNS:  int   0 if OK, -1 if failed
****************************************************************************/
int
  SNMP_Bind_64_Unsigned_Integer(SNMP_PKT_T *pktp,
                                int         indx,
                                int         compc,
                                OIDC_T     *compl,
                                OCTET_T     flags_n_type,
                                UINT_32_T   high,
                                UINT_32_T   low)
{
VB_T    *vbp;

if (flags_n_type != VT_COUNTER64)
    return (-1);

if ((vbp = index_to_vbp(pktp, indx)) == 0)
    return (-1);

if (build_object_id(compc, compl, &(vbp->vb_obj_id)) == -1)
    return (-1);

vbp->vb_data_flags_n_type = flags_n_type;
vbp->value_u.v_counter64.high = high;
vbp->value_u.v_counter64.low = low;

return 0;
}
