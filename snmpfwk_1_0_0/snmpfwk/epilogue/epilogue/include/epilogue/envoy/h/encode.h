/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/encode.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 *  Copyright 1986-1997 Epilogue Technology Corporation.
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
 * $Log: encode.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:24  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:09  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  22:53:35  sar
 * removed bitstring & uinteger32 options
 *
 * Revision 6.0  1995/05/31  21:46:41  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/09/29  18:31:12  sar
 * Rearrange some items to deal with removing v2 types from v1 settings if
 * they aren't installed.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.4  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo> where it was correct.
 *
 * Revision 3.3  1993/04/28  22:00:38  sar
 * Moved the definition of UINT_64_T to ui64.h and update others
 *
 * Revision 3.2  1993/04/26  20:33:14  sar
 * Added bitstrings, and have deletion of parties or contexts delete acls.
 *
 * Revision 3.1  1993/03/25  21:15:21  sar
 * Added types for uint 64 (counter64) and nsapaddress (translates to string)
 *
 * Revision 3.0  1992/04/03  19:52:27  dab
 * Release 3.0
 *
 * Revision 2.101  91/08/12  12:53:01  dab
 * Added EHELPER_T so as to declare the arguments to the helper function.
 * 
 * Revision 2.100  91/08/09  14:09:46  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:24  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:24
 * Release 2.00
 * 
 *    Rev 1.1   19 Sep 1988 17:27:06
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:16
 * Initial revision.
*/

#if (!defined(encode_inc))
#define encode_inc

#if (!defined(asn1_inc))
#include <envoy/h/asn1.h>
#endif

#if (!defined(buffer_inc))
#include <envoy/h/buffer.h>
#endif

#if (!defined(ui64_inc))
#include <envoy/h/ui64.h>
#endif

typedef ALENGTH_T       (EHELPER_T)     __((PTR_T, OCTET_T *, ALENGTH_T));
extern  ALENGTH_T       A_SizeOfInt             __((INT_32_T));
extern  ALENGTH_T       A_SizeOfUnsignedInt     __((UINT_32_T));
extern  ALENGTH_T       A_SizeOfObjectId        __((OBJ_ID_T *));

/* We only install some functions if their types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
extern  ALENGTH_T       A_SizeOfUnsignedInt64   __((UINT_64_T *));
#endif

extern  void            A_EncodeType            __((ATVALUE_T, OCTET_T,
                                                    EHELPER_T *, OCTET_T *));
extern  void            A_EncodeLength          __((ALENGTH_T, EHELPER_T *,
                                                    OCTET_T *));
extern  void            A_EncodeInt             __((ATVALUE_T, OCTET_T,
                                                    INT_32_T,  EHELPER_T *,
                                                    OCTET_T *));
extern  void            A_EncodeUnsignedInt     __((ATVALUE_T, OCTET_T,
                                                    UINT_32_T, EHELPER_T *,
                                                    OCTET_T *));
extern  void            A_EncodeOctetString     __((ATVALUE_T, OCTET_T,
                                                    OCTET_T *, ALENGTH_T,
                                                    EHELPER_T *, OCTET_T *));
extern  void            A_EncodeObjectId        __((ATVALUE_T, OCTET_T,
                                                    OBJ_ID_T *, EHELPER_T *,
                                                    OCTET_T *));

/* We only install some functions if their types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
extern  void            A_EncodeUnsignedInt64   __((ATVALUE_T, OCTET_T,
                                                    UINT_64_T *, EHELPER_T *,
                                                    OCTET_T *));
#endif

extern  EHELPER_T       A_EncodeHelper;

#endif  /* encode_inc */

