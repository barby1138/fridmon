/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/exists.c,v 1.1 2006/06/29 12:17:25 smasyutin Exp $ */

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
 * $Log: exists.c,v $
 * Revision 1.1  2006/06/29 12:17:25  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:05  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:19  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:55:57  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:49:47  sra
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
 * Revision 6.0  1995/05/31  21:50:30  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:29:57  sar
 * Updated revision to 5.0 and copyright to 1994
 *
 * Revision 4.1  1993/09/30  19:37:53  sar
 * Modified it_exists to return TPROC_NO_CREATION if we are trying to set
 * or create this object.
 *
 * Revision 4.0  1993/06/24  15:58:43  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 3.1  1993/03/01  17:14:17  dab
 * Changed variable named index to vb_index so it doesn't shadow index()
 * routine that some systems have.
 *
 * Revision 3.0  1992/04/03  19:53:01  dab
 * Release 3.0
 *
 * Revision 1.2  91/10/30  20:37:57  dab
 * Directly include asn1conf.h and snmpdefs.h.
 * 
 * Revision 1.1  91/10/24  11:05:06  dab
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
#include <auxfuncs.h>

/* Test function for simple mib variables that have exactly one
 * instance of 0.
 */
int
  it_exists(int         form,           /* TEST_SET & TEST_GET from mib.h */
            OIDC_T      lastmatch,      /* Last component matched */
            int         compc,
            OIDC_T *    compl,
            char *      cookie,
            SNMP_PKT_T *pktp,
            int         vb_index)       /* Index (zero based) of VB_T   */
{
  /* For non-tabular parameters, the "object instance", i.e. the unused */
  /* portion of the object identifier, must be a single component of    */
  /* value zero. */
  if ((compc == 1) && (*compl == 0))
    return 0;

  /* If this was a get we return no such, for sets or creates we return 
     no creation */
  if (form == TEST_GET)
      return(TPROC_NOSUCH);
  else
      return(TPROC_NO_CREATION);    
}
