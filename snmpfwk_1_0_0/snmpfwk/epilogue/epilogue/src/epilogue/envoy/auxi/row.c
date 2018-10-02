/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/row.c,v 1.1 2006/06/29 12:17:25 smasyutin Exp $ */

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
 * $Log: row.c,v $
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
 * Revision 7.3  1997/03/20 06:50:20  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1996/10/06 03:46:13  sar
 * Corrected ending condition in scan_vb_for_leaf, switched from
 * vbl_count to vbl_count - 1
 *
 * Revision 7.0  1996/03/15  22:21:24  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1996/01/05  19:02:18  sar
 * Removed no_pp style definitions as well as unnecessary include files
 *
 * Revision 6.0  1995/05/31  21:50:52  sra
 * Release 6.0.
 *
 * Revision 5.1  1995/03/16  21:13:45  sar
 * Modified internal calls to use a var bind pointer instead of an index.
 *
 * Revision 5.0  1994/05/16  15:29:57  sar
 * Updated revision to 5.0 and copyright to 1994
 *
 * Revision 4.0  1993/06/24  15:58:43  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 3.4  1993/06/02  23:07:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.3  1993/04/27  20:52:51  sar
 * moved index_to_vbp to snmp/buildpkt.c (where a copy already existed).
 * updated the makefile to ignore indx_vbp.c.
 * update row.c to use buildpkt.h to find index_to_vbp.
 *
 * Revision 3.2  1993/04/26  20:22:55  sar
 * Added ifdefs to allow clean version 1 or version 2 makes.
 *
 * Revision 3.1  1993/02/26  17:25:51  sar
 * made scan_for_leaf_and_match and scan_vb_for_leaf useable by outside
 * routines (removed the static declaration).\
 *
 * Revision 3.0  1992/04/03  19:53:01  dab
 * Release 3.0
 *
 * Revision 1.6  92/03/31  12:23:55  dab
 * Changed flags in the create_row structure again.
 * 
 * Revision 1.5  92/03/31  11:17:11  dab
 * Use flags in create_row structure instead of just needed field.
 * 
 * Revision 1.4  92/03/26  15:08:00  dab
 * Fixed bug where scan_for_leaf_and_match went into an infinte loop if
 * it hit an error.
 * 
 * Revision 1.3  92/03/05  16:37:25  romkey
 * corrected 'tatic' to 'static'
 * 
 * Revision 1.2  92/02/27  16:48:56  dab
 * Changed to use MIBLEAF_T* instead of a locator value.
 * 
 * Revision 1.1  91/11/06  14:42:36  dab
 * Initial revision
 * 
 */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <auxfuncs.h>

/* Scans the varbind list of the packet, starting at vb_index, looking for a
 * varbind that references the MIB node leaf.  Returns a pointer to the
 * varbind or 0 if it doesn't find any. */
VB_T *
  scan_vb_for_leaf(SNMP_PKT_T   *pktp,
                   VB_T         *vbp,
                   MIBLEAF_T    *leaf)
{
VB_T  *last_vbp;
VBL_T *vblp;
  
#if INSTALL_ENVOY_SNMP_VERSION_1
if (pktp->pdu_type == TRAP_PDU)
    vblp = &(pktp->pdu.trap_pdu.trap_vbl);  
else
#endif
    /* find the last vblp, which is always the one we are working on */
    for (vblp = &(pktp->pdu.std_pdu.std_vbl); vblp->vblp; vblp = vblp->vblp)
        ;      

for (last_vbp = &vblp->vblist[vblp->vbl_count - 1]; vbp <= last_vbp; vbp++) {
    if (vbp->vb_ml.ml_leaf == leaf)
        return vbp;
    }
  
return 0;       /* Not found */
}

/* Look for the varbind matching leaf node and also having the same instance
 * as compc/compl. */
VB_T *
  scan_for_leaf_and_match(SNMP_PKT_T    *pkt,
                          VB_T          *vbp,
                          MIBLEAF_T     *leaf,
                          int            compc,
                          OIDC_T        *compl)
{

vbp--;  /* set up vbp so scan_vb_for_leaf starts at vbp */

while (1) {
    vbp = scan_vb_for_leaf(pkt, vbp + 1, leaf);
    if (vbp == 0)
        return 0;                       /* we didn't find a match */
    /* Check the instance */
    if (((vbp->vb_flags & VFLAG_ALREADY_TEST) == 0) &&
        (compc == vbp->vb_ml.ml_remaining_objid.num_components) &&
        (oidorder(vbp->vb_ml.ml_remaining_objid.component_list,
                  compl, compc) == 0))
        return vbp;
    }
}

/* Find other varbinds mentioned in the row structure that have an instance
 * matching compc/compl.  All are marked as tested and all but the first is
 * marked as seted and they're linked off the first varbind mentioned in row.
 * If any varbinds are missing that are marked as needed then an error is
 * returned.  The first one is assumed needed regardless of how it's marked.
 */
VB_T *
  vb_extract_row(SNMP_PKT_T     *pkt,
                 int             start_indx,
                 int             compc,
                 OIDC_T         *compl,
                 int             row_len,
                 struct create_row *row)
{
VB_T *start_vbp, *first_vb, *vbp;
VB_T **vb_linkp;

/* set up the starting vbp */
start_vbp = index_to_vbp(pkt, start_indx);

/* Look up the first one and set it up as the first in the linked
 * list and as the only one whos set method will be called. */
first_vb = scan_for_leaf_and_match(pkt, start_vbp, row->leaf, compc, compl);
if (first_vb == 0)
    return 0;
first_vb->vb_link = 0;
vb_linkp = &first_vb->vb_link;
first_vb->vb_flags |= VFLAG_ALREADY_TEST;

/* Now get all the rest of the row, returning 0 if needed elements
 * are missing. */
for (row++, row_len--; row_len; row++, row_len--) {
    vbp = scan_for_leaf_and_match(pkt, start_vbp, row->leaf, compc, compl);
    if (vbp != 0) {
        /* Link it in and mark it done. */
        vbp->vb_link = 0;
        *vb_linkp = vbp;
        vb_linkp = &vbp->vb_link;
        vbp->vb_flags |= (row->flags & ROW_FLAG_CALL_SET) ?
          VFLAG_ALREADY_TEST :
          VFLAG_ALREADY_TEST | VFLAG_ALREADY_SET;
        }
    else if (row->flags & ROW_FLAG_NEEDED)
        return 0;                       /* needed piece is missing */
    }

return first_vb;
}
