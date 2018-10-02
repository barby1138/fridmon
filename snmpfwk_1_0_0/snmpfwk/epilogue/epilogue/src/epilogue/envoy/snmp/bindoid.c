/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/bindoid.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 * $Log: bindoid.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:42  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:39  sra
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
 *
 * Revision 6.0  1995/05/31  21:47:10  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.1  1993/04/27  21:10:00  sar
 * Replace locate_vb with index_to_vbp, so we can have only one copy of them.
 *
 * Revision 3.0  1992/04/03  19:52:37  dab
 * Release 3.0
 *
 * Revision 2.102  91/10/30  20:41:39  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.101  91/08/15  12:30:45  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:15  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:25  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:38
 * Release 2.00
 * 
 *    Rev 1.0   11 Jan 1989 12:11:56
 * Initial revision.
 *
 * Separated from buildpkt.c on January 11, 1989.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <objectid.h>
#include <buildpkt.h>

/****************************************************************************
NAME:  SNMP_Bind_Object_ID

PURPOSE:  Bind an object identifier into a VarBindList

PARAMETERS:
        SNMP_PKT_T *    Packet structure on which the data is to be bound
        int             Index to the particular VarBind to be used.
                                NOTE: This index zero based.
        int             Number of components in the data's object identifier
        OIDC_T *        Components of the data's object identifier
        int             Number of components in the object id to be bound
        OIDC_T          Components in the object id to be bound

NOTE: The first component-count/component list in the parameters refers
to the object id which identifies the variable.  The second count/list
pair is the actual data to be bound.

RETURNS:  int   0 if OK, -1 if failed
****************************************************************************/
int
  SNMP_Bind_Object_ID(SNMP_PKT_T *pktp,
                      int         indx,
                      int         compc,
                      OIDC_T     *compl,
                      int         valc,
                      OIDC_T     *vall)
{
VB_T    *vbp;

if ((vbp = index_to_vbp(pktp, indx)) == 0)
    return -1;

if (build_object_id(compc, compl, &(vbp->vb_obj_id)) == -1)
    return -1;

if (build_object_id(valc, vall, &(vbp->value_u.v_object)) == -1) {
    Clean_Obj_ID(&(vbp->vb_obj_id));
    return -1;
    }

vbp->vb_data_flags_n_type = VT_OBJECT;

return 0;
}
