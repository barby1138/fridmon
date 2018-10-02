/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/scan_loc.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 *  Copyright 1991-1997 Epilogue Technology Corporation.
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
 * $Log: scan_loc.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:06  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:23  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:16  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:50:26  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/15 22:21:24  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1996/01/05  19:02:18  sar
 * Removed no_pp style definitions as well as unnecessary include files
 *
 * Revision 6.0  1995/05/31  21:50:55  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:29:57  sar
 * Updated revision to 5.0 and copyright to 1994
 *
 * Revision 4.0  1993/06/24  15:58:43  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 3.2  1993/06/02  23:07:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.1  1993/04/26  20:22:55  sar
 * Added ifdefs to allow clean version 1 or version 2 makes.
 *
 * Revision 3.0  1992/04/03  19:53:01  dab
 * Release 3.0
 *
 * Revision 1.2  91/10/30  20:38:41  dab
 * Directly include asn1conf.h and snmpdefs.h.
 * 
 * Revision 1.1  91/10/24  11:09:05  dab
 * Initial revision
 * 
 */

#include <asn1conf.h>
#include <asn1.h>
#include <localio.h>
#include <buffer.h>
#include <decode.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>
#include <auxfuncs.h>

/****************************************************************************
NAME:  scan_vb_for_locator

PURPOSE:  scan the VB list of a packet structure looking for a particular
          locator value.

PARAMETERS:
          SNMP_PKT_T *  The whole received packet.
          int           The index of the VB_T where to start the search
                        (0 is the index of the first VB_T).
          UINT_16_T     The desired locator.

RETURNS:  int           The index in the varBindList of the VB_T which
                        references the MIB leaf with the specified locator.
                        -1 if none are found.

CAVEAT: It is assumed that the vb_ml field of the various VB_Ts has been
        already set and has valid contents.

NOTE: It is possible for a VB list to have more VB_Ts which
reference the same MIB leaf.

****************************************************************************/
int
scan_vb_for_locator(SNMP_PKT_T *        pktp,
                    int                 vb_index,
                    UINT_16_T           loc)
{
  VB_T *        vbp;
  VBL_T *       vblp;
  
#if INSTALL_ENVOY_SNMP_VERSION_1
  if (pktp->pdu_type == TRAP_PDU)
    vblp = &(pktp->pdu.trap_pdu.trap_vbl);  
  else
#endif
    vblp = &(pktp->pdu.std_pdu.std_vbl);

  for (vbp = &(vblp->vblist[vb_index]);
       vb_index < vblp->vbl_count;
       vb_index++,  vbp++) {
    if ((vbp->vb_ml.ml_leaf)->locator == loc)
      return vb_index;
  }
  
  return -1;    /* Not found */
}
