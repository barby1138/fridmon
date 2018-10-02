/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/buildpkt.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 * $Log: buildpkt.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:44  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:45  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:47:16  sra
 * Release 6.0.
 *
 * Revision 5.2  1995/05/08  20:32:04  sar
 * Did some casting between pointer arithmetic, ints and ALENGTH_Ts to
 * keep compilers happy.
 *
 * Revision 5.1  1995/03/21  00:25:01  sar
 * Added vbp_to_index and updated index_to_vbp to understand var bind list
 * chains.
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
 * Revision 3.2  1993/04/27  21:10:00  sar
 * Changed locate_vb to index_to_vbp
 *
 * Revision 3.1  1993/04/26  20:37:09  sar
 * Added ifdefs to allow clean makes of version 1 and 2, added bit strings,
 * arrange for deletion of acls with the party or context they refer to
 * is deleted.
 *
 * Revision 3.0  1992/04/03  19:52:37  dab
 * Release 3.0
 *
 * Revision 2.102  91/10/30  20:41:50  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.101  91/08/15  12:30:48  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:17  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:29  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:40
 * Release 2.00
 * 
 *    Rev 1.7   27 Apr 1989 15:55:58
 * Removed unused variables
 * 
 *    Rev 1.6   11 Jan 1989 12:11:22
 * Split into multiple files to prevent linker from picking up unused files.
 * 
 *    Rev 1.5   20 Sep 1988 22:51:38
 * Corrected erroneous call to EBufferInitialize (should have been
 * EBufferPreLoad.
 * 
 *    Rev 1.4   20 Sep 1988 19:11:50
 * Added procedure to build a VB list element containing a null value.
 * 
 *    Rev 1.3   19 Sep 1988 17:26:56
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.2   17 Sep 1988 18:57:14
 * Corrected miscalculation of whether a given var bind index was in-range.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:14
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:54
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>

/****************************************************************************
NAME:  index_to_vbp

PURPOSE:  Find the address of a VarBind item in a packet structure

PARAMETERS:
        SNMP_PKT_T *    Packet structure on which the data is to be bound
        int             Index to the particular VarBind to be used.
                                NOTE: This index zero based.

RETURNS:  VB_T *        (will be 0 on failure)
****************************************************************************/
VB_T *
  index_to_vbp(SNMP_PKT_T *pktp,
               int         indx)
{
VBL_T   *vblp;

/* If this is version 1 where we get the vblp from depends on the pdu type
   in version 2 we always get it from the standard area.
   In version 2 we have the complication of multiple vblists chained 
   together for the getbulk operation */
   
#if INSTALL_ENVOY_SNMP_VERSION_1
if (pktp->pdu_type == TRAP_PDU)
    vblp = &(pktp->pdu.trap_pdu.trap_vbl);
else
    vblp = &(pktp->pdu.std_pdu.std_vbl);
#else
vblp = &(pktp->pdu.std_pdu.std_vbl);
#endif

/* Check whether the requested index is within the VarBindList  */
for (; vblp; vblp = vblp->vblp) {
    if (vblp->vbl_count > indx)
        return &(vblp->vblist[indx]);
    else
        indx -= vblp->vbl_count;
    }

return 0;
}

/****************************************************************************
NAME:  vbp_to_index

PURPOSE:  Find the index of a VarBind item in a packet structure

PARAMETERS:
        SNMP_PKT_T *    Packet structure on which the data is to be bound
        VB_T       *    The varbind to find an index for.

RETURNS:  int   -1 on failure or 0 based index if the vbp is within range

****************************************************************************/
int
  vbp_to_index(SNMP_PKT_T *pktp,
               VB_T       *vbp)
{
VBL_T   *vblp;
int      count = 0;

/* If this is version 1 where we get the vblp from depends on the pdu type
   in version 2 we always get it from the standard area.
   In version 2 we have the complication of multiple vblists chained 
   together for the getbulk operation */
   
#if INSTALL_ENVOY_SNMP_VERSION_1
if (pktp->pdu_type == TRAP_PDU)
    vblp = &(pktp->pdu.trap_pdu.trap_vbl);
else
    vblp = &(pktp->pdu.std_pdu.std_vbl);
#else
vblp = &(pktp->pdu.std_pdu.std_vbl);
#endif

/* Check whether the requested index is within the VarBindList  */
if (vbp < vblp->vblist)
    return(-1);
for (; vblp; vblp = vblp->vblp) {
    if (vbp < vblp->vblist + vblp->vbl_count)
        return(count + ((int)(vbp - vblp->vblist)));
    else
        count += vblp->vbl_count;
    }

return(-1);
}

