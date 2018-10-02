/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/md5.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 *  Copyright 1993-1997 Epilogue Technology Corporation.
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

/* MD5.C - RSA Data Security, Inc., MD5 message-digest algorithm */
/*
 * Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 * rights reserved.
 *
 * License to copy and use this software is granted provided that it
 * is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 * Algorithm" in all material mentioning or referencing this software
 * or this function.
 *
 * License is also granted to make and use derivative works provided
 * that such works are identified as "derived from the RSA Data
 * Security, Inc. MD5 Message-Digest Algorithm" in all material
 * mentioning or referencing the derived work.
 *
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 *
 * These notices must be retained in any copies of any part of this
 * documentation and/or software.
 */

/*
 * $Log: md5.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:51  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:59  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1996/01/10  16:34:47  sar
 * removed no_pp stuff
 * added digest_append
 *
 * Revision 6.0  1995/05/31  21:47:30  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/07/26  21:18:23  sar
 * Added MD5digest_extended for use with data spanning multiple buffers.
 * ,.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 1.5  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 1.4  1993/04/23  23:31:53  sar
 * Added if !defined INSTALL_ENVOY_SNMP_VERSION_2 around the entire file.
 *
 */

#include <asn1conf.h>
#include <md5.h>
#if INSTALL_ENVOY_SNMP_VERSION_USEC

/* Constants for MD5Transform routine. */

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static OCTET_T PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits. */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
   Rotation is separate from addition to prevent recomputation.
   */
#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (UINT_32_T)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (UINT_32_T)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (UINT_32_T)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (UINT_32_T)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

/* Encodes input (UINT_32_T) into output (OCTET_T). Assumes len is
  a multiple of 4.
 */
static void
  Encode(OCTET_T   *output,
         UINT_32_T *input,
         UINT_16_T  len)
{
  UINT_16_T i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
    output[j] = (OCTET_T)(input[i] & 0xff);
    output[j+1] = (OCTET_T)((input[i] >> 8) & 0xff);
    output[j+2] = (OCTET_T)((input[i] >> 16) & 0xff);
    output[j+3] = (OCTET_T)((input[i] >> 24) & 0xff);
  }
}

/* Decodes input (OCTET_T) into output (UINT_32_T). Assumes len is
  a multiple of 4.
 */
static void
  Decode(UINT_32_T *output,
         OCTET_T   *input,
         UINT_16_T  len)
{
  UINT_16_T i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
    output[i] = ((UINT_32_T)input[j]) | (((UINT_32_T)input[j+1]) << 8) |
      (((UINT_32_T)input[j+2]) << 16) | (((UINT_32_T)input[j+3]) << 24);
}

/* MD5 basic transformation. Transforms state based on block.
 */
static void
  MD5Transform(UINT_32_T state[4],
               OCTET_T   block[64])
{
  UINT_32_T a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  Decode (x, block, 64);

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478L); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756L); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070dbL); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceeeL); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0fafL); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62aL); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613L); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501L); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8L); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7afL); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1L); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7beL); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122L); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193L); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438eL); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821L); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562L); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340L); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51L); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aaL); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105dL); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453L); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681L); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8L); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6L); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6L); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87L); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14edL); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905L); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8L); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9L); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8aL); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942L); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681L); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122L); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380cL); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44L); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9L); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60L); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70L); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6L); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127faL); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085L); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881d05L); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039L); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5L); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8L); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665L); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244L); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97L); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7L); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039L); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3L); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92L); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47dL); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1L); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4fL); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0L); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314L); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1L); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82L); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235L); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bbL); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391L); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  /* Zeroize sensitive information. */
  MEMSET (x, 0, sizeof (x));
}

/****************************************************************************
NAME: MD5digest

PURPOSE:  Compute the md5 digest of the given buffer, placing the result in
          the octet string

PARAMETERS: 
        OCTET_T    *    Buffer we're digesting
        UINT_16_T       length of buffer we're digesting
        OCTET_T    *    Space to hold the digest

RETURNS: void
****************************************************************************/

void
  MD5digest (OCTET_T *input,
             UINT_16_T inputLen,
             OCTET_T digest[16])
{
  UINT_16_T i, vb_index;
  UINT_32_T state[4], count[2];
  OCTET_T buffer[64];

  /* Load magic initialization constants. */
  state[0] = 0x67452301L;
  state[1] = 0xefcdab89L;
  state[2] = 0x98badcfeL;
  state[3] = 0x10325476L;

  /* Transform as many times as possible. */
  for (i = 0; i + 63 < inputLen; i += 64)
      MD5Transform (state, &input[i]);

  /* Buffer remaining input */
  vb_index = inputLen - i;
  MEMCPY(buffer, &input[i], vb_index);

  if (vb_index < 56)
    MEMCPY(&buffer[vb_index], PADDING, 56 - vb_index);
  else {
    MEMCPY(&buffer[vb_index], PADDING, 64 - vb_index);
    MD5Transform (state, buffer);
    MEMCPY(buffer, &PADDING[1], 56);
    }

  /* Encode number of bits */
  count[0] = (UINT_32_T)(inputLen << 3);
  count[1] = 0;
  Encode (&buffer[56], count, 8);
  MD5Transform (state, buffer);

  /* Store state in digest */
  Encode (digest, state, 16);

  /* Zeroize sensitive information */
  MEMSET (state, 0, sizeof (state));
  MEMSET (buffer, 0, sizeof (buffer));
}

/****************************************************************************
NAME: MD5digest_extended

PURPOSE:  Compute an md5 digest for a potentially long buffer or a series
          of buffers if it isn't practical to assemble the entire buffer
          in one section of memory.  To digestify a buffer the user
          would call this routine with an initial buffer and the initial
          flag set to 1.  The routine will then initialize the state 
          variables and start the digesting process.  The user continues
          by calling the routine with the returned state information and a
          new buffer until all of the data has been digested, at which
          time the digest will be written.  Except for the last buffer,
          buffers must be a multiple of 64 bytes, any data not ending on
          a 64 byte boundry will be ignored.  The calling routine
          indicates the end of data by setting the final flag to 1
          (which will cause the digest to be written).

PARAMETERS: 
        OCTET_T    *    Buffer we're digesting
        UINT_16_T       length of buffer we're digesting
        UINT_16_T       1 = initial block, 2 = final block, 3 = both
        UINT_32_T[4]    state block
        UINT_32_T[2]    overall length information
        OCTET_T    *    Space to hold the digest

RETURNS: void
****************************************************************************/

void
  MD5digest_extended (OCTET_T  *input,
                      UINT_16_T inputLen,
                      UINT_16_T flags,
                      UINT_32_T state[4],
                      UINT_32_T p_count[2],
                      OCTET_T   digest[16])
{
  UINT_16_T i, vb_index;
  UINT_32_T count[2];
  OCTET_T buffer[64];

  /* Load the state information, either from the magic constants
     or from the previous state information */

  if (flags & 1) {
    /* Load magic initialization constants. */
    state[0] = 0x67452301L;
    state[1] = 0xefcdab89L;
    state[2] = 0x98badcfeL;
    state[3] = 0x10325476L;
    /* zero the length information */
    p_count[0] = p_count[1] = 0;
    }

  /* Update the length information */
  p_count[0] += inputLen;
  if (p_count[0] < inputLen)
    p_count[1]++;
  
  /* Transform as many times as possible. */
  for (i = 0; i + 63 < inputLen; i += 64)
      MD5Transform (state, &input[i]);

  /* If this isn't the final buffer return,
     otherwise set up the final digest */
  if (!(flags & 2))
    return;

  /* Buffer remaining input */
  vb_index = inputLen - i;
  if (vb_index)
    MEMCPY(buffer, &input[i], vb_index);

  if (vb_index < 56)
    MEMCPY(&buffer[vb_index], PADDING, 56 - vb_index);
  else {
    MEMCPY(&buffer[vb_index], PADDING, 64 - vb_index);
    MD5Transform (state, buffer);
    MEMCPY(buffer, &PADDING[1], 56);
    }

  /* Encode number of bits */
  count[0] = p_count[0] << 3;
  count[1] = (p_count[1] << 3) + (p_count[0] >> 29);
  Encode (&buffer[56], count, 8);
  MD5Transform (state, buffer);

  /* Store state in digest */
  Encode (digest, state, 16);

  /* Zeroize sensitive information */
  MEMSET (state, 0, sizeof (state));
  MEMSET (buffer, 0, sizeof (buffer));
}

/****************************************************************************
NAME: MD5digest_append

PURPOSE:  Compute an md5 digest for two buffers.  The digest will be
          computed over the logically concatenation of the two buffers.
          This routine can be used to append a key (or other information)
          to a buffer without requiring the buffer be modified.

PARAMETERS: 
        OCTET_T    *    First Buffer we're digesting
        UINT_16_T       length of first buffer we're digesting
        OCTET_T    *    Second Buffer we're digesting
        UINT_16_T       length of second buffer we're digesting
        OCTET_T    *    Space to hold the digest

RETURNS: void
****************************************************************************/

void
  MD5digest_append (OCTET_T   *input,
                    UINT_16_T  inputLen,
                    OCTET_T   *input2,
                    UINT_16_T  inputLen2,
                    OCTET_T    digest[16])
{
/* We shall have up to three calls to the md5digest routine
   the first covers all of the full 64 byte blocks of the first
   input buffer, the second covers any remaining bytes from the
   first buffer and as much of the second buffer as fits into
   a 64 byte block, the third covers the remaining bytes in the
   second buffer */
UINT_32_T state[4], slength[2];
UINT_16_T full_len, mod_len, rem_len = 0, flags = 1;
OCTET_T buffer[64];

/* The number of bytes in buffer 1 modulo 64,
   The number of bytes in full 64 bytes blocks */
mod_len  = inputLen % 64;
full_len = inputLen - mod_len;

if (full_len) {
    /* process all of the full 64 bytes blocks of the first input */
    MD5digest_extended(input, full_len, 1, state, slength, digest);
    flags = 0;
    }

/* process any remaining bytes from input 1 and some from input 2*/
if (mod_len) {
    rem_len = 64 - mod_len;
    MEMCPY(buffer, input + full_len, mod_len);
    if (inputLen2 <= rem_len) {
        if (inputLen2)
            MEMCPY(buffer + mod_len, input2, inputLen2);
        MD5digest_extended(buffer, mod_len + inputLen2, flags | 2,
                           state, slength, digest);
        return;
        }
    else {
        MEMCPY(buffer + mod_len, input2, rem_len);
        MD5digest_extended(buffer, 64, flags, state, slength, digest);
        flags = 0;
        }
    }

MD5digest_extended(input2 + rem_len, inputLen2 - rem_len,
                   flags | 2, state, slength, digest);
return;
}

#endif /* INSTALL_ENVOY_SNMP_VERSION_USEC */
