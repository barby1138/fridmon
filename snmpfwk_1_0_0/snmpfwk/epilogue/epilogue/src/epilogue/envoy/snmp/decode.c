/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/decode.c,v 1.1 2006/06/29 12:17:27 smasyutin Exp $       */

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
 * $Log: decode.c,v $
 * Revision 1.1  2006/06/29 12:17:27  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:47  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:51  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  23:02:48  sar
 * removed no_pp stuff, casts of zero and bitstring routines
 *
 * Revision 6.0  1995/05/31  21:47:20  sra
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
 * Revision 4.4  1994/05/04  22:45:43  sar
 * Corrected name of a routine, the WTC had been left off.
 *
 * Revision 4.3  1994/05/02  17:02:30  sar
 * Added new decode routines *WTC (with type check) to type check the string
 * as it is being decoded.  Mostly these will be used in snmp_d.c
 *
 * Revision 4.2  1994/04/29  21:30:03  sar
 * Added a check in the decode integer data routine to make sure that
 * the 5th byte of unsigned integers == 0.
 *
 * Revision 4.1  1993/09/30  20:41:13  sar
 * Corrected the test for acceptable number of unused bits in decoding
 * a bit string.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.5  1993/06/17  23:33:44  sar
 * Fixed the decodebitstring routine.
 *
 * Revision 3.4  1993/06/13  02:41:25  sar
 * fixed a BISTRING to be BITSTRING.
 *
 * Revision 3.3  1993/04/26  20:37:09  sar
 * Added ifdefs to allow clean makes of version 1 and 2, added bit strings,
 * arrange for deletion of acls with the party or context they refer to
 * is deleted.
 *
 * Revision 3.2  1993/03/25  21:18:09  sar
 * Added routines to encode/decode 64 bit integers/counter64s
 *
 * Revision 3.1  1992/06/16  15:58:41  dab
 * Got rid of the "then"s.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.104  92/02/05  18:46:19  dab
 * More casting to keep MSC happy.
 * 
 * Revision 2.103  91/10/30  20:41:56  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.102  91/09/18  12:32:13  dab
 * Updated to use new macros from <asn1conf.h> and <snmpconf.h>.
 * 
 * Revision 2.101  91/08/15  12:30:58  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:19  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:35  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:44
 * Release 2.00
 * 
 *    Rev 1.9   14 Dec 1989 16:00:48
 * Added support for Borland Turbo C compiler
 * 
 *    Rev 1.8   04 Jul 1989 12:03:38
 * DecodeIntegerData() was improperly decoding negative integers
 * when the length was less than 4 bytes.
 * 
 *    Rev 1.7   27 Apr 1989 15:56:00
 * Removed unused variables
 * 
 *    Rev 1.6   12 Apr 1989 12:02:40
 * Added cast on value returned by SNMP_mem_alloc().
 * 
 *    Rev 1.5   23 Mar 1989 11:55:56
 * Merged the decode helper into the one routine that used it.
 * 
 *    Rev 1.4   18 Mar 1989 11:56:42
 * Unused octet string handling code removed.
 * Octet string decoding simplified and hardened against zero length strings.
 * 
 *    Rev 1.3   17 Mar 1989 21:41:56
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.2   19 Sep 1988 17:26:54
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.1   14 Sep 1988 17:54:20
 * Removed improper casts in calls to SNMP_mem_alloc().
 * Also moved includes of system includes into libfunc.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:56
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <localio.h>
#include <decode.h>
#include <buffer.h>

/****************************************************************************
NAME:  A_DecodeTypeValue

PURPOSE:  Decode the numeric part of an ASN.1 type from a stream.
          The data stream is read using the local I/O package.
          On exit, the stream pointer will be positioned to the byte
          *AFTER* the type.

NOTE:     The Class portion of the type is NOT decoded here, only the
          value portion.
          The user should call A_DecodeTypeClass *BEFORE* calling this
          routine in order to get the class.

PARAMETERS:  LCL_FILE *     A stream descriptor (already open)
             int *          Receives an error code, if any.

RETURNS:  ATVALUE_T         The type value

RESTRICTIONS:  It is assumed that the stream does not reach EOF before the
                end of the field.
****************************************************************************/
ATVALUE_T
  A_DecodeTypeValue(LCL_FILE *  lfile,
                    int *       errp)
{
  register OCTET_T oct;

  oct = (OCTET_T) (Lcl_Getc(lfile) & ~A_IDCF_MASK);

  if (Lcl_Eof(lfile)) {
    *errp = AE_PREMATURE_END;
    return (ATVALUE_T) 0;
  }

  if (oct != 0x1F) {    /* Are there extension bytes? */
    /* No extensions, type is in oct */
    return (ATVALUE_T) oct;
  }
  else {   /* Type is in extension octets */
    register ATVALUE_T t = 0;
    for(;;) {
      oct = (OCTET_T)Lcl_Getc(lfile);
      if (Lcl_Eof(lfile)) {
        *errp = AE_PREMATURE_END;
        return t;
      }
      if (!(oct & 0x80))
        break;                  /* Hit final byte, we'll use*/
                                /* it at the end of the loop*/

      t |= (ATVALUE_T)(oct & 0x7F);   /* Deal with a non-final byte */
      t <<= 7;
    }
    t |= (ATVALUE_T) oct;       /* Take care of the final byte (the one */
                                /* without the 0x80 continuation bit.)  */
    return t;
  }
/*NOTREACHED*/
}

/****************************************************************************
NAME:  A_DecodeLength

PURPOSE:  Decode an ASN.1 length from a stream.
          The data stream is read using the local I/O package.
          On exit, the stream pointer will be positioned to the byte
          *AFTER* the length.

PARAMETERS:  LCL_FILE *     Stream descriptor
             int *          Receives an error code, if any.

RETURNS:  ALENGTH_T -- the length.
          If the length is indefinite, (ALENGTH_T)-1 is returned.

RESTRICTIONS:  The stream must be open.
               It is assumed that the stream will not reach EOF before the
               length is decoded.
****************************************************************************/
ALENGTH_T
  A_DecodeLength(LCL_FILE *     lfile,
                 int      *     errp)
{
  OCTET_T oct;

  oct = (OCTET_T)Lcl_Getc(lfile);
  if (Lcl_Eof(lfile)) {
    *errp = AE_PREMATURE_END;
    return (ALENGTH_T) 0;
  }

  /* Indefinite form? */
  if (oct == 0x80) {
    *errp = AE_INDEFINITE_LENGTH;
    return (ALENGTH_T) -1;
  }

  if (!(oct & 0x80))  /* Short or long format? */
    return (ALENGTH_T) oct;   /* Short format */
  else {   /* Long format */
    register OCTET_T lsize;
    register ALENGTH_T len = 0;

    lsize = oct & (OCTET_T)0x7F; /* Get # of bytes comprising length field */
    while (lsize-- != 0) {
      len <<= 8;
      len |= (OCTET_T)Lcl_Getc(lfile);
      if (Lcl_Eof(lfile)) {
        *errp = AE_PREMATURE_END;
        return (ALENGTH_T) 0;
      }
    }
    return len;
  }
  /*NOTREACHED*/
}

/********************
A_DecodeOctetStringData

PURPOSE:  Pull an octet string from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the data field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        ALENGTH_T       Length of octet string, from its ASN.1 header
        EBUFFER_T *     Control structure to receive the data.
        int *           Receives an error code, if any.

Returns: Nothing

Note:   On return, the "start_bp" component of the buffer structure
        points to a "malloc"-ed area in which the octet string is held.
        Note that the octet string is NOT null terminated, may contain
        internal nulls. A null pointer, (char *)0, is used if no area
        is malloc-ed.
        If the string is of zero length, a dummy buffer is established
        which appears to be have a static buffer of length zero at
        address zero.
********************/
void
  A_DecodeOctetStringData(LCL_FILE  *stream,
                          ALENGTH_T  length,
                          EBUFFER_T *ebuffp,
                          int       *errp)
{
  if ((length != 0) && (length != (ALENGTH_T) -1)) {
    OCTET_T     *buffp;
    ALENGTH_T got;
    if ((buffp = (OCTET_T *)SNMP_memory_alloc(length)) == 0)
      return;
    EBufferSetup(BFL_IS_DYNAMIC, ebuffp, buffp, length);
    got = (ALENGTH_T)Lcl_Read(stream, ebuffp->next_bp, (int)length);
    if (got == length) {
      ebuffp->remaining -= got;
      ebuffp->next_bp += (unsigned short)got;
    }
    else *errp = AE_PREMATURE_END;
  }
  else { /* Length is either zero or indeterminate */
    EBufferPreLoad(BFL_IS_STATIC, ebuffp, 0, 0);
  }
}

/********************
A_DecodeOctetString

PURPOSE:  Pull an octet string from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the octet string's type field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        EBUFFER_T *     Control structure to receive the data.
        int *           Receives an error code, if any.

Returns: Nothing

Note:   On return, the "start_bp" component of the buffer structure
        points to a "malloc"-ed area in which the octet string is held.
        Note that the octet string is NOT null terminated, may contain
        internal nulls. A null pointer, (char *)0, is used if no area
        is malloc-ed.
********************/
void
  A_DecodeOctetString(LCL_FILE  *stream,
                      EBUFFER_T *ebuffp,
                      int       *errp)
{
  ALENGTH_T     os_length;

  (void) A_DecodeTypeValue(stream, errp);
  os_length = A_DecodeLength(stream, errp);
  if (*errp == 0) {
    A_DecodeOctetStringData(stream, os_length, ebuffp, errp);
  }
  else { /* On a decoding error, pretend we have a zero length string */
    EBufferPreLoad(BFL_IS_STATIC, ebuffp, 0, 0);
  }
}

/********************
A_DecodeOctetStringWTC

PURPOSE:  Pull an octet string from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the octet string's type field.
          This version does type checking.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        EBUFFER_T *     Control structure to receive the data.
        int *           Receives an error code, if any.
        ATVALUE_T       The type value
        OCTET_T         A_IDC_MASK flag values

Returns: Nothing

Note:   On return, the "start_bp" component of the buffer structure
        points to a "malloc"-ed area in which the octet string is held.
        Note that the octet string is NOT null terminated, may contain
        internal nulls. A null pointer, (char *)0, is used if no area
        is malloc-ed.
********************/
void
  A_DecodeOctetStringWTC(LCL_FILE  *    stream,
                         EBUFFER_T *    ebuffp,
                         int       *    errp,
                         ATVALUE_T      id,
                         OCTET_T        flags)
{
  ALENGTH_T     os_length;

  if ((A_DecodeTypeClass(stream) != flags) ||
      (A_DecodeTypeValue(stream, errp) != id)) {
      if (*errp == 0)
          *errp = AE_WRONG_TYPE;
      /* On a decoding error, pretend we have a zero length string */
      EBufferPreLoad(BFL_IS_STATIC, ebuffp, 0, 0);
      return;
      }
  os_length = A_DecodeLength(stream, errp);
  if (*errp == 0) {
    A_DecodeOctetStringData(stream, os_length, ebuffp, errp);
  }
  else { /* On a decoding error, pretend we have a zero length string */
    EBufferPreLoad(BFL_IS_STATIC, ebuffp, 0, 0);
  }
}


/********************
A_DecodeIntegerData

PURPOSE:  Pull an integer from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the data field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        ALENGTH_T       Length of contents field, from the ASN.1 header
        int *           Receives an error code, if any.

Returns: INT_32_T       (See note below)

NOTE: If the received value is really unsigned, then the caller should
merely cast the value returned by this procedure to an UINT_32_T.

WARNING: If the integer occupies more than 4 octets, then high order precision
will be lost, including the sign bit.  For unsigned values in which the
basic value occupies all 4 octets, the sign octet, containing a zero sign
bit, will be lost but will not damage the returned value.
********************/
INT_32_T
  A_DecodeIntegerData(LCL_FILE  *stream,
                      ALENGTH_T  length,
                      int       *errp)
{
  INT_32_T ivalue = 0;
  int    firstone = 1;
  OCTET_T        oct;

  while(length-- != 0) {
    oct = (OCTET_T) Lcl_Getc(stream);
    if (Lcl_Eof(stream)) {
        *errp = AE_PREMATURE_END;
        return ivalue;
        }

    /* See whether we are receiving something that has the sign bit set, or
       if this is a 5 byte unsigned int check the first byte, it must be 0 */
    if (firstone) {
        firstone = 0;
        if ((length == 4) && (oct != 0)) {
            *errp = AE_WRONG_VALUE;
            return ivalue;
            }
        if (oct & (OCTET_T)0x80)
            ivalue = (INT_32_T)-1;
        }
/*lint -e703    */
    ivalue <<= 8;
/*lint +e703    */
    ivalue |= oct;      /* 'oct' better not be sign extended!!! */
    }
  return ivalue;
}

/********************
A_DecodeInteger

PURPOSE:  Pull an integer from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the integer's ASN.1 type field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        int *           Receives an error code, if any.

Returns: INT_32_T;
********************/
INT_32_T
  A_DecodeInteger(LCL_FILE *stream,
                  int      *errp)
{
  (void) A_DecodeTypeValue(stream, errp);
  return A_DecodeIntegerData(stream, A_DecodeLength(stream, errp), errp);
}

/********************
A_DecodeIntegerWTC

PURPOSE:  Pull an integer from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the integer's ASN.1 type field.
          As we pull off the class and type we check them against
          the arguments to make sure they are correct.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        int *           Receives an error code, if any.
        ATVALUE_T       The type value
        OCTET_T         A_IDC_MASK flag values

Returns: INT_32_T;
********************/
INT_32_T
  A_DecodeIntegerWTC(LCL_FILE * stream,
                     int      * errp,
                     ATVALUE_T  id,
                     OCTET_T    flags)
{
  if ((A_DecodeTypeClass(stream) != flags) ||
      (A_DecodeTypeValue(stream, errp) != id)) {
      if (*errp == 0)
          *errp = AE_WRONG_TYPE;
      return (0);
      }
  return A_DecodeIntegerData(stream, A_DecodeLength(stream, errp), errp);
}

/********************
A_DecodeObjectIdData

PURPOSE:  Pull an object identifier from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the data field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        ALENGTH_T       Length of contents field, from the ASN.1 header
        OBJ_ID_T *      Object identifier structure to receive the object id.
                        The "component_list" will be "malloc"ed.
                        component_list == (OIDC_T *)0 indicates that
                        there is no component_list.
        int *           Receives an error code, if any.

Returns: Nothing
********************/
void
  A_DecodeObjectIdData(LCL_FILE  *stream,
                       ALENGTH_T  length,
                       OBJ_ID_T  *objp,
                       int       *errp)
{
  int content_offset;           /* Offset in stream where contents begins */
  int left;                     /* Count of unused contents bytes */
  int subids;                   /* Number of subidentifiers     */
  int subid_num;
  OIDC_T        subid_val;      /* Value of a subidentifier */
  OIDC_T        *cp;
  unsigned char c;

  objp->num_components = 0;
  objp->component_list = 0;

  /* Remember where the contents begins */
  content_offset = Lcl_Tell(stream);

  /* Count the number of components */
  for(subids = 0, left = (int)length; left > 0; left--) {
    c = (unsigned char)Lcl_Getc(stream);
    if (Lcl_Eof(stream)) {
      *errp = AE_PREMATURE_END;
      return;
    }

    /* Skip all bytes but ones which are the last in a subidentifier.   */
    /* In other words skip all which have the high order bit set.       */
    if (c & 0x80)
      continue;
    subids++;
  }

  (void) Lcl_Seek(stream, content_offset, 0);

  /* Null object id if no subidentifier fields */
  if (subids == 0)
    return;

  /* Try to get space for the components list */
  cp = (OIDC_T *)SNMP_memory_alloc((unsigned int)(sizeof(OIDC_T) * (subids + 1)));
  if (cp == 0)
    return;

  objp->num_components = subids + 1;
  objp->component_list = cp;

  /* Decode the subids and components */
  for (subid_num = 0; subid_num < subids; subid_num++) {
    /* Decode the subidentifier */
    for (subid_val = 0;;) {
      c = (unsigned char)Lcl_Getc(stream);
      if (Lcl_Eof(stream)) {
        *errp = AE_PREMATURE_END;
        return;
      }
      subid_val <<= 7;
      subid_val |= (OIDC_T)(c & 0x7F);
      if (!(c & 0x80))
        break;
    }

    /* Is this the first subidentifier?                 */
    /* i.e. the one that contains TWO components?       */
    if (subid_num == 0) {
      if (subid_val < 40) {
        *cp++ = 0;
        *cp++ = subid_val;
      }
      else {
        if (subid_val < 80) {
          *cp++ = 1;
          *cp++ = subid_val - 40;
        }
        else {
          *cp++ = 2;
          *cp++ = subid_val - 80;
        }
      }
    }
    else { /* subid_num != 0, i.e. this is not the first subidentifier */
      *cp++ = subid_val;
    }
  }

  return;
}

/********************
A_DecodeObjectId

PURPOSE:  Pull an object identifer from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the object identifier's ASN.1 type field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        OBJ_ID_T *      Object identifier structure to receive the object id.
                        The "component_list" will be "malloc"ed.
                        component_list == (OIDC_T *)0 indicates that
                        there is no component_list.
        int *           Receives an error code, if any.
                
Returns: Nothing
********************/

void
  A_DecodeObjectId(LCL_FILE *stream,
                   OBJ_ID_T *objp,
                   int      *errp)
{
  ALENGTH_T leng;
  (void) A_DecodeTypeValue(stream, errp);
  leng = A_DecodeLength(stream, errp);
  if (*errp == 0)
    A_DecodeObjectIdData(stream, leng, objp, errp);
  return;
}

/********************
A_DecodeObjectIdWTC

PURPOSE:  Pull an object identifer from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the object identifier's ASN.1 type field.
          This version does type checking
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        OBJ_ID_T *      Object identifier structure to receive the object id.
                        The "component_list" will be "malloc"ed.
                        component_list == (OIDC_T *)0 indicates that
                        there is no component_list.
        int *           Receives an error code, if any.
        ATVALUE_T       The type value
        OCTET_T         A_IDC_MASK flag values

                
Returns: Nothing
********************/

void
  A_DecodeObjectIdWTC(LCL_FILE  *stream,
                      OBJ_ID_T  *objp,
                      int       *errp,
                      ATVALUE_T  id,
                      OCTET_T    flags)
{
  ALENGTH_T leng;
  if ((A_DecodeTypeClass(stream) != flags) ||
      (A_DecodeTypeValue(stream, errp) != id)) {
      if (*errp == 0)
          *errp = AE_WRONG_TYPE;
      return;
      }
  leng = A_DecodeLength(stream, errp);
  if (*errp == 0)
    A_DecodeObjectIdData(stream, leng, objp, errp);
  return;
}

/* We only install ui64 routine if the type is installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
/********************
A_DecodeInteger64Data

PURPOSE:  Pull up to a 64 bit integer from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the data field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        ALENGTH_T       Length of contents field, from the ASN.1 header
        UINT_64_T *     Space for the received int
        int *           Receives an error code, if any.

Returns: void

WARNING: If the integer occupies more than 8 octets, then high order precision
will be lost, including the sign bit.  For unsigned values in which the
basic value occupies all 8 octets, the sign octet, containing a zero sign
bit, will be lost but will not damage the returned value.
********************/
void
  A_DecodeInteger64Data(LCL_FILE   *stream,
                         ALENGTH_T  length,
                         UINT_64_T *value,
                         int       *errp)
{
  int    firstone = 1;
  OCTET_T        oct;

  value->high = value->low = 0;
  while(length-- != 0) {
    oct = (OCTET_T) Lcl_Getc(stream);
    if (Lcl_Eof(stream)) {
      *errp = AE_PREMATURE_END;
      return;
    }

    /* See whether we are receiving something that has the sign bit set */
    if (firstone) {
      firstone = 0;
      if (oct & (OCTET_T)0x80) {
        value->high = (UINT_32_T)0xFFFFFFFFL;
        value->low  = (UINT_32_T)0xFFFFFFFFL;
        }
    }

    /* Decide which of the two words to put the octet into   */
    /* if length >= 4 we are working on the high order bytes */
    /* equal sign is because we already decremented length   */
    if (length >= 4) {
        value->high <<= 8;
        value->high |= oct;     /* 'oct' better not be sign extended!!! */
        }
    else {
        value->low <<= 8;
        value->low |= oct;      /* 'oct' better not be sign extended!!! */
        }
  }
  return;
}

/********************
A_DecodeInteger64

PURPOSE:  Pull up to a 64 bit integer from an ASN.1 stream.
          The data stream is read using the local I/O package.
          On entry stream pointer should be positioned to the first byte
          of the integer's ASN.1 type field.
          On exit, the stream pointer will be positioned to at the start
          of the next ASN.1 type field.

Parameters:
        LCL_FILE *      Stream descriptor
        UINT_64_T *     Space to receive the int
        int *           Receives an error code, if any.

Returns: INT_32_T;
********************/
void
  A_DecodeInteger64(LCL_FILE  * stream,
                    UINT_64_T * value,
                    int       * errp)
{
  (void) A_DecodeTypeValue(stream, errp);
  A_DecodeInteger64Data(stream, A_DecodeLength(stream, errp), value, errp);
  return;
}
#endif
