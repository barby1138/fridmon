/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/encode.c,v 1.1 2006/06/29 12:17:27 smasyutin Exp $       */

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
 * $Log: encode.c,v $
 * Revision 1.1  2006/06/29 12:17:27  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:48  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:53  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  23:02:48  sar
 * removed no_pp stuff, casts of zero
 * changed memcpy to MEMCPY
 *
 * Revision 6.0  1995/05/31  21:47:22  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.3  1994/05/09  21:03:44  sar
 * Added L for long to some constants.
 *
 * Revision 4.2  1994/01/25  20:43:50  sar
 * Removed some macro code that allowed oids to be 16 or 32 bits long
 * we now only deal with 32 bit oids.
 * Made the temporary buffer for oid encoding be 5 bytes long instead
 * of 4 bytes, this was needed due to the asn1 encoding of a large >2g
 * oid.
 *
 * Revision 4.1  1993/09/30  20:40:15  sar
 * Corrected the computation of unused bits in the A_EncodeBitString
 * routine.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.2  1993/04/26  20:37:09  sar
 * Added ifdefs to allow clean makes of version 1 and 2, added bit strings,
 * arrange for deletion of acls with the party or context they refer to
 * is deleted.
 *
 * Revision 3.1  1993/03/25  21:18:09  sar
 * Added routines to encode/decode 64 bit integers/counter64s
 *
 * Revision 3.0  1992/04/03  19:52:37  dab
 * Release 3.0
 *
 * Revision 2.106  92/02/11  12:53:45  dab
 * Typo'd type of argument 'func' to routine A_EncodeSubId() as EHELPER_T
 * instead of EHELPER_T *.  Only the Zortech compiler noticed.
 * 
 * Revision 2.105  92/02/05  18:46:31  dab
 * More casting to keep MSC happy.
 * 
 * Revision 2.103  91/10/30  20:41:58  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.102  91/08/15  12:31:00  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.101  91/08/12  12:44:13  dab
 * Use EHELPER_T to declare all the various helper routines.  This
 * typedef (from encode.h) handles the problem of declaring the routines
 * arguments or not as the compiler requires.
 * 
 * Revision 2.100  91/08/09  14:08:20  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:37  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.1   23 Nov 1990 13:45:48
 * Added a cast to calls to A_EncodeType() on the flags parameter
 * to satisfy Microsoft C version 6.
 * 
 *    Rev 2.0   31 Mar 1990 15:06:46
 * Release 2.00
 * 
 *    Rev 1.5   05 Jun 1989  0:11:16
 * Corrected: error in encoding unsigned integers which have the high-order
 * bit set (value >= 2,147,483,648).
 * 
 *    Rev 1.4   28 Apr 1989 16:51:44
 * Added protection against encoding null object identifiers.
 * 
 *    Rev 1.3   17 Mar 1989 21:41:34
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.2   19 Sep 1988 17:26:52
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:04
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:56
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <encode.h>

/* Turn on the following define to make A_SizeOfSubId() into a macro */
#define SUBID_MACRO

#if !defined(SUBID_MACRO)
static  ALENGTH_T       A_SizeOfSubId(OIDC_T);
#endif  /* SUBID_MACRO */
static  void            A_EncodeSubId(OIDC_T, EHELPER_T *, OCTET_T *);

/****************************************************************************
A_SizeOfInt -- Return total size that an integer would occupy when
               ASN.1 encoded (tag and length fields are not included).

Parameters:
        INT_32_T    The integer (signed 32 bit)

Returns: ALENGTH_T  Number of octets the integer would occupy if
                    in ASN.1 encoding
****************************************************************************/
ALENGTH_T
  A_SizeOfInt(INT_32_T i)
{
if (i >= 0L)
   return (i <= 0x0000007FL ? 1 :               /* <= 127       */
          (i <= 0x00007FFFL ? 2 :               /* <= 32767     */
          (i <= 0x007FFFFFL ? 3 :               /* <= 8388607   */
           4)));                                /* > 8388607    */
else
    return (i >= (INT_32_T)0xFFFFFF80L ? 1 :    /* >= -128      */
           (i >= (INT_32_T)0XFFFF8000L ? 2 :    /* >= -32768    */
           (i >= (INT_32_T)0XFF800000L ? 3 :    /* >= -8388608  */
            4)));                               /* < -8388608   */
}

/****************************************************************************
A_SizeOfUnsignedInt -- Return total size that an unsigned integer would
                       occupy when ASN.1 encoded (tag and length fields
                       are not included).

Parameters:
        UINT_32_T    The integer (unsigned 32 bit)

Returns: ALENGTH_T  Number of octets the integer would occupy if
                    in ASN.1 encoding
****************************************************************************/
ALENGTH_T
  A_SizeOfUnsignedInt(UINT_32_T i)
{
return (i <= 0x0000007FL ? 1 :          /* <= 127       */
       (i <= 0x00007FFFL ? 2 :          /* <= 32768     */
       (i <= 0x007FFFFFL ? 3 :          /* <= 8388607   */
       (i <= 0x7FFFFFFFL ? 4 :          /* <= 2147483647*/
        5))));                          /* >  2147483647*/
}

/****************************************************************************
A_SizeOfSubId -- Compute the number of bytes required to hold a
                 subidentifier from an object id.
                 ASN.1 encoded (tag and length fields are not included)

Parameters:
        OIDC_T

Returns: ALENGTH_T  Number of octets needed in ASN.1 encoding
****************************************************************************/
#if !defined(SUBID_MACRO)
static ALENGTH_T
  A_SizeOfSubId(OIDC_T i)
{
return (i <= 0x7F ? 1 : (i <= 0x3FFF ? 2 : (i <= 0x1FFFFFL ? 3 :
       (i <= 0x0FFFFFFFL ? 4 : 5))));
}
#else   /* SUBID_MACRO */
#define A_SizeOfSubId(I)  (ALENGTH_T)((OIDC_T)(I) <= 0x7F ? 1 :         \
                          ((OIDC_T)(I) <= 0x3FFF ? 2 :          \
                          ((OIDC_T)(I) <= 0x1FFFFFL ? 3 :       \
                          ((OIDC_T)(I) <= 0x0FFFFFFFL ? 4 : 5))))
#endif  /* SUBID_MACRO */

/****************************************************************************
A_SizeOfObjectId -- Return total size that an object ID would occupy when
                ASN.1 encoded (tag and length fields are not included)

Parameters:
        OBJ_ID_T *      Pointer to the internal object Id structure

Returns: ALENGTH_T  Number of octets the object ID would occupy if
                    in ASN.1 encoding

Note: It is assumed by this routine that the object identifier has at least
two components.
****************************************************************************/
ALENGTH_T
  A_SizeOfObjectId(OBJ_ID_T *objp)
{
ALENGTH_T leng;
OIDC_T *cp = objp->component_list;
int i;
OIDC_T x;

if (objp->num_components == 0)
    return 0;

/* Compute the value of the first subidentifier from the values of the  */
/* first two components.                                                */
   {
   x = *cp++;
   x = x * 40 + *cp++;
   leng = A_SizeOfSubId(x);
   }

for (i = 2; i < objp->num_components; i++)
   {
   x = *cp++;
   leng += A_SizeOfSubId(x);
   }
return leng;
}

/* We only inlcude ui64 function if the type is installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)

/****************************************************************************
A_SizeOfUnsignedInt64 -- Return total size that a big unsigned integer would
                         occupy when ASN.1 encoded (tag and length fields
                         are not included).

Parameters:
        UINT_64_T *   The integer (unsigned 64 bit)

Returns: ALENGTH_T  Number of octets the integer would occupy if
                    in ASN.1 encoding
****************************************************************************/
ALENGTH_T
  A_SizeOfUnsignedInt64(UINT_64_T *i)
{
if (i->high == 0)
    return (i->low <= 0x0000007FL ? 1 :         /* <= 127       */
           (i->low <= 0x00007FFFL ? 2 :         /* <= 32768     */
           (i->low <= 0x007FFFFFL ? 3 :         /* <= 8388607   */
           (i->low <= 0x7FFFFFFFL ? 4 :         /* <= 2147483647*/
            5))));                              /* >  2147483647*/
else
    return (i->high <= 0x0000007FL ? 5 :        /* <= 127       */
           (i->high <= 0x00007FFFL ? 6 :        /* <= 32768     */
           (i->high <= 0x007FFFFFL ? 7 :        /* <= 8388607   */
           (i->high <= 0x7FFFFFFFL ? 8 :        /* <= 2147483647*/
            9))));                              /* >  2147483647*/
}

#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */

/****************************************************************************
A_EncodeType -- Encode an ASN.1 type field into buffer.

Parameters:
        ATVALUE_T        The type value
        OCTET_T          A_IDCF_MASK flag values
        ALENGTH_T (*f()) Function to be called to take generated data
        OCTET_T *        Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

Returns: nothing
****************************************************************************/
void
  A_EncodeType(ATVALUE_T  id,
               OCTET_T    flags,
               EHELPER_T *func,
               OCTET_T   *funcparm)
{
flags &= A_IDCF_MASK;
if (id <= (ATVALUE_T)30) {
    OCTET_T c;
    c = flags | (OCTET_T) id;
    (void)(*func)(funcparm, &c, (ALENGTH_T)sizeof(OCTET_T));
    }
else {
    /* Build a partial reverse order version of the result and then */
    /* reverse it again back to correct order */
    OCTET_T buff[5], reverse[4];  /* Can't handle more than 4 octets */
    OCTET_T *bp = buff;
    OCTET_T *rp = reverse;
    unsigned short int count = 0;      /* Should never exceed 4 */
    ALENGTH_T cnt;
    *bp++ = (OCTET_T)((flags & A_IDCF_MASK) | 0x1F);
    while (id > 0)
        {
        *rp++ = (OCTET_T) (id & 0x7F);
        id >>= 7;
        count++;
        }
    cnt = count + 1;
    while ((count--) > 1)
        {
        *bp++ = (OCTET_T)(*(--rp) | 0x80);
        }
    *bp++ = *(--rp);
    (void)(*func)(funcparm, buff, cnt);
    }
}

/****************************************************************************
A_EncodeLength -- Encode an ASN.1 definite form length field into buffer.

Parameters:
        ALENGTH_T        Length to be encoded
        ALENGTH_T (*f()) Function to be called to take generated data
        OCTET_T *        Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

Returns: nothing
****************************************************************************/
void
  A_EncodeLength(ALENGTH_T  leng,
                 EHELPER_T *func,
                 OCTET_T   *funcparm)
{
if (leng <= (ALENGTH_T)127) {
    OCTET_T c;
    c = (OCTET_T) leng;
    (void)(*func)(funcparm, &c, (ALENGTH_T)sizeof(OCTET_T));
    }
else {
    OCTET_T buff[OCTETS_PER_INT32+1];
    OCTET_T reverse[OCTETS_PER_INT32];
    OCTET_T *bp = buff;
    OCTET_T *rp = reverse;
    unsigned short int count = 0; /* Never exceeds OCTETS_PER_INT32 */
    ALENGTH_T cnt;
    while (leng > 0)
        {
        *rp++ = (OCTET_T) leng;
        /*lint -e704    */
        leng >>= 8;
        /*lint +e704    */
        count++;
        }
    *bp++ = (OCTET_T)(((OCTET_T) count) | (OCTET_T) 0x80);
    cnt = count + 1;
    while ((count--) > 0)
        {
        *bp++ = *(--rp);
        }
    (void)(*func)(funcparm, buff, cnt);
  }
}

/****************************************************************************
A_EncodeInt -- generate ASN.1 format of integer (WITH TYPE & LENGTH)

Parameters:
    ATVALUE_T        The type value
    OCTET_T          A_IDC_MASK flag values
    INT_32_T         The integer to convert (signed 32 bit)
    ALENGTH_T (*f()) Function to be called to take generated data
    OCTET_T *        Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

Returns: nothing

****************************************************************************/

void
  A_EncodeInt(ATVALUE_T  id,
              OCTET_T    flags,
              INT_32_T   value,
              EHELPER_T *func,
              OCTET_T   *funcparm)
{
ALENGTH_T leng;
OCTET_T   *rp;
OCTET_T   buff[OCTETS_PER_INT32];

leng = A_SizeOfInt(value);
A_EncodeType(id, (OCTET_T)(flags & A_IDC_MASK), func, funcparm);
A_EncodeLength(leng, func, funcparm);
rp = buff + (unsigned short int)leng;
for(;;)
    {
    *(--rp) = (OCTET_T) value;
    if (rp == buff) break;
    /*lint -e704        */
    value >>= 8;
    /*lint +e704        */
    }

  (void)(*func)(funcparm, buff, leng);
}

/****************************************************************************
A_EncodeUnsignedInt -- generate ASN.1 format of integer (WITH TYPE & LENGTH)
                       where the local form of the integer is unsigned.

Parameters:
    ATVALUE_T        The type value
    OCTET_T          A_IDC_MASK flag values
    UINT_32_T        The integer to convert (unsigned 32 bit)
    ALENGTH_T (*f()) Function to be called to take generated data
    OCTET_T *        Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

Returns: nothing

****************************************************************************/
void
  A_EncodeUnsignedInt(ATVALUE_T  id,
                      OCTET_T    flags,
                      UINT_32_T  value,
                      EHELPER_T *func,
                      OCTET_T   *funcparm)
{
ALENGTH_T leng, xleng;
OCTET_T   *rp;
OCTET_T   buff[OCTETS_PER_INT32+1];

leng = A_SizeOfUnsignedInt(value);
A_EncodeType(id, (OCTET_T)(flags & A_IDC_MASK), func, funcparm);
A_EncodeLength(leng, func, funcparm);
rp = buff + (unsigned short int)leng;
/* If the unsigned number takes 5 octets, the high order octet is merely */
/* a zero byte to hold the zero sign.                                    */
for(xleng = leng; xleng--;)
    {
    *(--rp) = (OCTET_T) value;
    value >>= 8;  /* This better be shifting zeros into the high end!   */
    }

  (void)(*func)(funcparm, buff, leng);
}

/****************************************************************************
A_EncodeOctetString -- Generate ASN.1 format of octet string (WITH TYPE &
                       LENGTH)

Parameters:
    ATVALUE_T        The type value
    OCTET_T          A_IDC_MASK flag values
    OCTET_T *        Address of the string
    ALENGTH_T        Length of the string
    ALENGTH_T (*f()) Function to be called to take generated data
    OCTET_T *        Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

****************************************************************************/
void
  A_EncodeOctetString(ATVALUE_T  id,
                      OCTET_T    flags,
                      OCTET_T   *osp,
                      ALENGTH_T  oslen,
                      EHELPER_T *func,
                      OCTET_T   *funcparm)
{
/* Do a primitive encoding */
A_EncodeType(id, (OCTET_T)(flags & A_IDC_MASK), func, funcparm);
A_EncodeLength(oslen, func, funcparm);
if (oslen != 0) 
    (void)(*func)(funcparm, osp, oslen);
}

/****************************************************************************
A_EncodeSubId -- generate ASN.1 format of a subidentifier from an
                 object identifier

Parameters:
    OIDC_T      The subidentifier to encode
    ALENGTH_T (*f())    Function to be called to take generated data
    OCTET_T *           Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

Returns: nothing

****************************************************************************/
static void
  A_EncodeSubId(OIDC_T     value,
                EHELPER_T *func,
                OCTET_T   *funcparm)
{
ALENGTH_T leng;
OCTET_T   *rp;
OCTET_T   buff[OCTETS_PER_INT32+1];
OCTET_T   last;

leng = A_SizeOfSubId(value);

for(rp = buff + leng, last = 0x00; rp != buff;)
    {
    *(--rp) = (OCTET_T)(value & 0x007F) | last;
    value >>= 7;
    last = 0x80;
    }

  (void)(*func)(funcparm, buff, leng);
}

/****************************************************************************
A_EncodeObjectId -- generate ASN.1 format of Object ID (WITH TYPE & LENGTH)

Parameters:
    ATVALUE_T        The type value
    OCTET_T          A_IDC_MASK flag values
    OBJ_ID_T *       Pointer to the internal object Id structure
    ALENGTH_T (*f()) Function to be called to take generated data
    OCTET_T *        Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

Returns: nothing

****************************************************************************/
void
  A_EncodeObjectId(ATVALUE_T  id,
                   OCTET_T    flags,
                   OBJ_ID_T  *objp,
                   EHELPER_T *func,
                   OCTET_T   *funcparm)
{
ALENGTH_T leng;
OIDC_T *cp = objp->component_list;
int i;

leng = A_SizeOfObjectId(objp);
A_EncodeType(id, (OCTET_T)(flags & A_IDC_MASK), func, funcparm);
A_EncodeLength(leng, func, funcparm);

if (leng == 0) 
    return;

/* Merge the first two components of the object identifier to form the  */
/* first subidentifier.                                                 */
   {
   OIDC_T x;
   x = *cp++;
   x = x * 40 + *cp++;
   A_EncodeSubId(x, func, funcparm);
   }

/* Handle the remaining components, each as its own subidentifier.      */
for (i = 2; i < objp->num_components; i++)
   {
   A_EncodeSubId(*cp++, func, funcparm);
   }
}

/* We only inlcude the ui64 function if the type is installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)

/****************************************************************************
A_EncodeUnsignedInt64 -- generate ASN.1 format of integer (WITH TYPE & LENGTH)
                         where the local form of the integer is unsigned 64.

Parameters:
    ATVALUE_T        The type value
    OCTET_T          A_IDC_MASK flag values
    UINT_64_T *      The integer to convert (unsigned 64 bit)
    ALENGTH_T (*f()) Function to be called to take generated data
    OCTET_T *        Parameter to be passed unchanged to the function.

Notes:  The function whose address is passed as a parameter is called zero
or more times to take away some accumulated data.  The function is called
with these parameters:
        OCTET_T *   The parameter (funcparm) passed to this routine
        OCTET_T *   The buffer where the data resides
        ALENGTH_T   The number of octets in the buffer.

The function should return the number of octets consumed, type ALENGTH_T.
The function should return a zero if it has taken all the data it wants.

Returns: nothing

****************************************************************************/
void
  A_EncodeUnsignedInt64(ATVALUE_T  id,
                        OCTET_T    flags,
                        UINT_64_T *value,
                        EHELPER_T *func,
                        OCTET_T   *funcparm)
{
ALENGTH_T leng, xleng;
OCTET_T   *rp;
OCTET_T   buff[(OCTETS_PER_INT32*2)+1];

leng = A_SizeOfUnsignedInt64(value);
A_EncodeType(id, (OCTET_T)(flags & A_IDC_MASK), func, funcparm);
A_EncodeLength(leng, func, funcparm);
rp = buff + (unsigned short int)leng;
/* If the unsigned number takes 9 octets, the high order octet is merely */
/* a zero byte to hold the zero sign.                                    */
if (leng <= 4)
    for(xleng = leng; xleng--;) {
        *(--rp) = (OCTET_T) value->low;
        value->low >>= 8;
        }
else {
    for(xleng = 4; xleng--;) {
        *(--rp) = (OCTET_T) value->low;
        value->low >>= 8;
        }
    for(xleng = leng - 4; xleng--;) {
        *(--rp) = (OCTET_T) value->high;
        value->high >>= 8;
        }  
    }

  (void)(*func)(funcparm, buff, leng);
}
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */

/****************************************************************************

NAME:  A_EncodeHelper

PURPOSE:  Collect encoded data from the ASN.1 encoding routines and
          place it into a buffer.

PAREMETERS:
        EBUFFER_T * The "opaque" parameter (funcparm) passed to the encoding
                    routines.
        OCTET_T *   The buffer where the encoded data resides
        ALENGTH_T   The number of encoded octets in the buffer.

RETURNS:  Returns the number of octets consumed, zero is returned if
          no more data is desired.  (This may not, however, prevent
          subsequent calls.)

RESTRICTIONS:  
        Can not handle length > 64K

BUGS:  

****************************************************************************/
ALENGTH_T
  A_EncodeHelper(PTR_T      ptr,
                 OCTET_T   *bp,
                 ALENGTH_T  leng)
{
EBUFFER_T *ebp = ptr;
ALENGTH_T actual;
actual = min(leng, ebp->remaining);
switch (actual)
   {
   case 0:
        break;

   case 1:
        *(ebp->next_bp++) = *bp;
        ebp->remaining--;
        break;

   case 2:
        *(ebp->next_bp++) = *bp++;
        *(ebp->next_bp++) = *bp;
        ebp->remaining -= 2;
        break;

   default:
        (void) MEMCPY(ebp->next_bp, bp, actual);
        ebp->remaining -= actual;
        ebp->next_bp += (unsigned short)actual;
        break;
   }
return (actual);
}
