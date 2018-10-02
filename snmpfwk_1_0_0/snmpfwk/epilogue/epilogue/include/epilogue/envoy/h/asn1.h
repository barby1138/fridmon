/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/asn1.h,v 1.1 2006/06/29 12:17:07 smasyutin Exp $ */

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
 * $Log: asn1.h,v $
 * Revision 1.1  2006/06/29 12:17:07  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:09  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:20  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:47:58  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:28  sra
 * Release 6.0.
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
 * Revision 2.104  91/10/30  20:53:14  dab
 * Include asn1conf.h only if it's not already been included.
 * 
 * Revision 2.103  91/08/15  15:41:45  dab
 * Pulled configuration from asn1.h and snmp.h and put in
 * snmpconf.h and asn1conf.h.
 * 
 * Revision 2.102  91/08/14  10:55:24  dab
 * Removed type declarations to snmpconf.h and includes that file.
 * 
 * Revision 2.101  91/08/12  12:48:00  dab
 * Added typedef of PTR_T.
 * 
 * Revision 2.100  91/08/09  14:09:44  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:19  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:26
 * Release 2.00
 * 
 *    Rev 1.1   14 Dec 1989 16:01:16
 * Added support for Borland Turbo C compiler
 * 
 *    Rev 1.0   12 Sep 1988 10:46:12
 * Initial revision.
*/

#if (!defined(asn1_inc))
#define asn1_inc

#if (!defined(asn1conf_inc))
#include <envoy/h/asn1conf.h>
#endif

/* ASN.1 FLAGS (form and class), adjusted for position within an octet */
#define A_IDCF_MASK         0xE0        /* Mask for ID CLASS and FORM bits */
#define A_IDC_MASK          0xC0        /* Mask for ID CLASS bits */
#define A_IDF_MASK          0x20        /* Mask for ID FORM bit */
#define A_FLAG_MASK         0xE0
#define A_UNIVERSAL         0x00
#define A_APPLICATION       0x40
#define A_CONTEXT           0x80
#define A_PRIVATE           0xC0
#define A_PRIMITIVE         0x00
#define A_CONSTRUCTOR       0x20

#define A_DEFAULT_SCOPE     A_CONTEXT

/* ASN.1 basic types, all in UNIVERSAL scope */
#define A_NO_ID             0x00
#define A_BOOLEAN           0x01
#define A_INTEGER           0x02
#define A_BITSTRING         0x03
#define A_OCTETSTRING       0x04
#define A_NULL              0x05
#define A_OBJECTID          0x06
#define A_SEQUENCE          0x10
#define A_SET               0x11

/* ASN.1 defined types, all based on OCTETSTRING */
#define A_NUMERICSTRING     0x12
#define A_PRINTABLESTRING   0x13
#define A_T61STRING         0x14
#define A_TELETEXSTRING     A_T61STRING
#define A_VIDEOTEXSTRING    0x15
#define A_IA5STRING         0x16
#define A_GRAPHICSTRING     0x19
#define A_VISIBLESTRING     0x1A
#define A_GENERALSTRING     0x1B

/* Other ASN.1 defined universal types */
#define A_GENERALIZEDTIME   0x18
#define A_UTCTIME           0x17
#define A_OBJECTDESCRIPTOR  0x07

/****************************************************************************

NAME:  A_SizeOfLength

PURPOSE:  Compute the number of octets needed to hold an ASN.1 length field.

PARAMETERS:
            ALENGTH_T   The proposed length

RETURNS:    ALENGTH_T   The number of octets needed

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define A_SizeOfLength(S) ((ALENGTH_T)((S) <= 127 ? 1 : ((S) <= 255 ? 2 : 3)))

/****************************************************************************

NAME:  A_SizeOfType

PURPOSE:  Compute the number of octets needed to hold an ASN.1 type field

PARAMETERS:
            ATVALUE_T           The proposed type (without CLASS & FORM bits)

RETURNS:    unsigned int        The number of octets needed

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define A_SizeOfType(V) ((unsigned int) ((V) <= 30 ? 1 : \
                         ((V) <= 127 ? 2 : ((V) <= 16383 ? 3 : 4))))


/****************************************************************************
A_SizeOfOctetString --  Return total size that an octet string would
                        occupy when ASN.1 encoded (tag and length fields
                        are NOT included)

Parameters:
        ALENGTH_T       The size of the string

Returns: ALENGTH_T  Number of octets the octet string would occupy if
                    in ASN.1 encoding, NOT including the type/length fields
                    of the outermost wrapper.  Since these routines only
                    generate non-constructor encodings, the length is
                    exactly that given.
****************************************************************************/
#define A_SizeOfOctetString(L)  ((ALENGTH_T)(L))


/****************************************************************************
OBJECT IDENTIFIER
****************************************************************************/

/* The following structure is used to internally hold an object identifier */
/* Each component is limited in range from 0 to 64K-1.                     */

typedef struct  OBJ_ID_S
        {
        int     num_components; /* # of object id components    */
                                /* Note: this is the number of  */
                                /* components in the object id, */
                                /* which is one more than the # */
                                /* of subidentifiers in an      */
                                /* encoded object id.           */
        OIDC_T  *component_list;
        } OBJ_ID_T;

#endif  /* asn1_inc */

