/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/row_get.c,v 1.1 2006/06/29 12:17:25 smasyutin Exp $ */

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
 *  Copyright 1996-1997 Epilogue Technology Corporation.
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
 * $Log: row_get.c,v $
 * Revision 1.1  2006/06/29 12:17:25  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:06  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:22  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:13  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:50:21  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/15 22:21:24  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 1.1  1996/01/05  18:58:29  sar
 * Initial revision
 *
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <vbdefs.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <auxfuncs.h>

/* Build a list of all the varbinds with the same getproc as vbp.
 * Links them all together with vbp->vb_link.  Does not set any flags,
 * the caller is expected to do this. */
void
  group_by_getproc_and_instance(SNMP_PKT_T *pktp,
                                VB_T       *first_vbp,
                                int         compc,
                                OIDC_T     *compl)
{
  VBL_T *vblp;
  VB_T  *last_vbp;
  VB_T  *vbp;
  VB_T  *list;
  
#if INSTALL_ENVOY_SNMP_VERSION_1
  if (pktp->pdu_type == TRAP_PDU)
    vblp = &(pktp->pdu.trap_pdu.trap_vbl);  
  else
#endif
    /* find the last vblp, which is always the one we are working on */
    for (vblp = &(pktp->pdu.std_pdu.std_vbl); vblp->vblp; vblp = vblp->vblp)
      ;      

  list = first_vbp;
  list->vb_link = 0;
  for (last_vbp = &vblp->vblist[vblp->vbl_count], vbp = first_vbp + 1;
       vbp < last_vbp; vbp++) {
		if(vbp->vb_data_flags_n_type == VT_NOSUCHOBJ)/*noa fix - crasing 6-6-02*/
			continue;
    /* find varbinds with the same getproc which aren't already in use */
    if ((vbp->vb_ml.ml_leaf->getproc == first_vbp->vb_ml.ml_leaf->getproc) &&
        ((vbp->vb_flags & (VFLAG_SET_STARTED | VFLAG_SET_DONE |
                           VFLAG_TEST_STARTED | VFLAG_TEST_DONE)) == 0) &&
        (oidcmp2(compc, compl, vbp->vb_ml.ml_remaining_objid.num_components,
                 vbp->vb_ml.ml_remaining_objid.component_list) == 0)) {
      /* link onto the list */
      list->vb_link = vbp;
      list = vbp;
      vbp->vb_link = 0;
    }
  }
}

