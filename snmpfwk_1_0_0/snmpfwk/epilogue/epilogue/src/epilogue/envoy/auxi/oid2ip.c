/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/oid2ip.c,v 1.1 2006/06/29 12:17:25 smasyutin Exp $ */

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
 * $Log: oid2ip.c,v $
 * Revision 1.1  2006/06/29 12:17:25  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:05  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:22  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:10  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:50:14  sra
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
 * Revision 6.0  1995/05/31  21:50:50  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:29:57  sar
 * Updated revision to 5.0 and copyright to 1994
 *
 * Revision 4.0  1993/06/24  15:58:43  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 3.0  1992/04/03  15:32:31  dab
 * Release 3.0
 *
 * Revision 1.5  92/03/30  18:42:07  dab
 * checked in with -k by dab at 92.04.03.15.31.34.
 * 
 * Revision 1.5  92/03/30  18:42:07  dab
 * Changed arguments so oid_to_ip() could return an error if the instance
 * wasn't legal for an IP address.
 * 
 * Revision 1.4  92/02/05  16:58:19  dab
 * Added cast on assignment from OIDC_T to OCTET_T.
 * 
 * Revision 1.3  91/11/03  09:52:42  dab
 * Forgot to increment a loop variable.
 * 
 * Revision 1.2  91/10/30  20:38:40  dab
 * Directly include asn1conf.h and snmpdefs.h.
 * 
 * Revision 1.1  91/10/30  18:12:45  dab
 * Initial revision
 * 
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

/* oid_to_ip() converts a list of ODIC_T components into an IP address
 * (in network byte order) and returns that address in *addr and
 * returns non-zero if the instance is not legal for an IP address.
 * If tcount is less then 4 then the remainder of the address is zero
 * filled.  */
int
  oid_to_ip(int tcount,
            OIDC_T *tlist,
            UINT_32_T *addr)
{
  union {
    UINT_32_T laddr;
    OCTET_T baddr[4];
  } ip_address;
  int i;

  ip_address.laddr = 0;
  
  if (tcount > 4)
    tcount = 4;

  for (i = 0; tcount; tcount--, i++) {
    if (*tlist > 255)
      return 1;                 /* not a legal component for an IP address */
    ip_address.baddr[i] = (OCTET_T)*tlist++;
  }

  *addr = ip_address.laddr;
  return 0;
}
