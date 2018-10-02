/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/sas_ui64.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 *  Copyright 1995-1997 Epilogue Technology Corporation.
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
 * $Log: sas_ui64.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:06  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:23  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:15  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:50:24  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/15 22:21:24  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:50:54  sra
 * Release 6.0.
 *
 * Revision 1.1  1995/03/08  23:08:05  sar
 * Initial revision
 *
 *
 */

#include <ui64.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <auxfuncs.h>

/* Routine to clean up the space allocated by the uint 64 test routine */
void
  ui64_async_cleanup(PTR_T value)
{
#if INSTALL_ENVOY_40_VB_FREE_COMPAT
SNMP_memory_free(value);
#else
SNMP_memory_free(((VB_T *)value)->vb_priv);
#endif
return;
}

/* Undo the set, restore the information we saved to the vb_priv pointer
   during the test phase back to the cookie pointer */
void
  ui64_async_undo(OIDC_T         lastmatch,
                  int            compc,
                  OIDC_T        *compl,
                  SNMP_PKT_T    *pktp,
                  VB_T          *vbp)
{
UI64_Copy((UINT_64_T *)(VB_TO_COOKIE(vbp)), (UINT_64_T *)vbp->vb_priv);
SNMP_memory_free(vbp->vb_priv);
vbp->vb_free_priv = 0;

undoproc_good(pktp, vbp);
return;
}

/* Attempt to allocate space to save the current value in case an undo
   is required */
void
  ui64_async_test(OIDC_T         lastmatch,
                  int            compc,
                  OIDC_T        *compl,
                  SNMP_PKT_T    *pktp,
                  VB_T          *vbp)
{
UINT_64_T *value;

if ((value = (UINT_64_T *)SNMP_memory_alloc(sizeof(UINT_64_T))) == 0) {
    testproc_error(pktp, vbp, RESOURCE_UNAVAILABLE);
    return;
    }

UI64_Copy(value, (UINT_64_T *)(VB_TO_COOKIE(vbp)));
vbp->vb_priv = (PTR_T)value;
vbp->vb_free_priv = ui64_async_cleanup;
undoproc_set(pktp, vbp, ui64_async_undo);

testproc_good(pktp, vbp);
return;
}

/* Set 64-bit unsigned integer referenced by cookie. */
void
  ui64_async_set(OIDC_T          lastmatch,
                 int             compc,
                 OIDC_T         *compl,
                 SNMP_PKT_T     *pktp,
                 VB_T           *vbp)
{
UI64_Copy((UINT_64_T *)(VB_TO_COOKIE(vbp)), VB_GET_UINT64(vbp));
setproc_good(pktp, vbp);
return;
}
