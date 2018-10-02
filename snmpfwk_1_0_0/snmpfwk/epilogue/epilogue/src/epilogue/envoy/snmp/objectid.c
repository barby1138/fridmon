/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/objectid.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: objectid.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:54  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:49:05  sra
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
 * Revision 6.0  1995/05/31  21:47:37  sra
 * Release 6.0.
 *
 * Revision 5.1  1995/03/21  00:25:01  sar
 * Minor cleanup, removed unneeded casts of 0 and started using MEMCPY
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.1  1993/02/17  21:06:14  sar
 * Removed the #define the and uses of it.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.103  91/10/30  20:42:06  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.102  91/09/18  12:32:26  dab
 * Updated to use new macros from <asn1conf.h> and <snmpconf.h>.
 * 
 * Revision 2.101  91/08/15  12:31:06  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:23  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:46  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:50
 * Release 2.00
 * 
 *    Rev 1.4   17 Mar 1989 21:41:28
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.3   04 Mar 1989 10:35:34
 * Added cast to actual parameter on call to memcpy to avoid warnings on
 * some compilers.
 * 
 *    Rev 1.2   11 Jan 1989 12:46:44
 * Moved Clean_Obj_ID() to objectid.c
 * 
 *    Rev 1.1   14 Sep 1988 17:57:16
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:47:04
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <objectid.h>

/****************************************************************************
NAME:  build_object_id

PURPOSE:  Build an object id structure

PARAMETERS:
        int             Number of components in the old structure
        OIDC_T *        List of components in the old structure
        OBJ_ID_T *      The new Object Identifier structure

RETURNS:  0 if sucessful, -1 if not
****************************************************************************/
int
  build_object_id(int       oldc,
                  OIDC_T   *oldl,
                  OBJ_ID_T *new)
{
new->component_list = 0;  /* Just in case the list is empty */

if ((new->num_components = oldc) != 0) {
  unsigned int need;

  need = (unsigned int)(sizeof(OIDC_T) * new->num_components);
  if ((new->component_list = (OIDC_T *)SNMP_memory_alloc(need)) == 0)
      return -1;

  if (need != 0) 
      MEMCPY(new->component_list, oldl, need);
  }

return 0;
}

/****************************************************************************
NAME:  Clean_Obj_ID

PURPOSE:  Clean up an OBJ_ID_T structure

PARAMETERS:     OBJ_ID_T *

RETURNS:  Nothing
****************************************************************************/
void
  Clean_Obj_ID(OBJ_ID_T *objp)
{
if (objp->component_list != 0){
    SNMP_memory_free(objp->component_list);
    objp->component_list = 0;
    }
objp->num_components = 0;
}
