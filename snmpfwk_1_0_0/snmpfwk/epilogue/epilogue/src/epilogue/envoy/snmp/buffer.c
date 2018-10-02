/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/buffer.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $       */

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
 * $Log: buffer.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:44  sar
 * Updated revision to 8.0
 *
 * Revision 7.4  1997/10/16 00:39:40  sar
 * Cleaned up some possible problems in the seek routine, mostly
 * dealing with attempts to go beyond the buffer
 *
 * Revision 7.3  1997/03/20 06:48:44  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1996/10/07 23:55:58  sar
 * Added some parans in ebufferreplace to make sure the compare
 * is for the right objects.
 *
 * Revision 7.0  1996/03/18  20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  22:59:09  sar
 * removed no_pp stuff & casts of 0
 * modified memcpy to MEMCPY
 *
 * Revision 6.0  1995/05/31  21:47:15  sra
 * Release 6.0.
 *
 * Revision 5.1  1995/05/08  20:32:04  sar
 * Did some casting between pointer arithmetic, ints and ALENGTH_Ts to
 * keep compilers happy.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.1  1993/09/30  20:37:35  sar
 * Modified BufferAdjust to not allow extension of a static buffer that
 * would require freeing the current space.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.6  1993/04/29  15:27:42  sar
 * Removed a check that could never fail if (ALENGTH_T < 0)
 *
 * Revision 3.5  1993/03/20  00:25:43  sar
 * Updated some types for version two party type stuff (indicies, local
 * storage, status, mtu and privileges).
 * Also removed some unused break statements.
 *
 * Revision 3.4  1993/03/06  22:54:50  sar
 * Added test in EbufferAppend and EbufferAdd to avoid doing a memcpy of
 * zero length.
 *
 * Revision 3.3  1993/03/05  03:58:02  dab
 * fixed up types
 *
 * Revision 3.2  1993/03/02  19:06:46  sar
 * added Ebuffer_add which extends the string in a buffer by allocating a
 * new one copying the old one and then deleteing it.
 *
 * Revision 3.1  1993/02/17  21:01:07  sar
 * Removed #define then and use of it.
 * Added EBufferSeek which is like seek in unix, it moves the pointers to
 * new position in the buffer.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.103  91/09/18  12:32:03  dab
 * Updated to use new macros from <asn1conf.h> and <snmpconf.h>.
 * 
 * Revision 2.102  91/08/15  12:30:47  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.101  91/08/12  12:47:35  dab
 * Cast arguments to memcpy() and memset() to PTR_T.
 * 
 * Revision 2.100  91/08/09  14:08:17  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:28  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:40
 * Release 2.00
 * 
 *    Rev 1.4   27 Apr 1989 15:56:30
 * Removed unused variables
 * 
 *    Rev 1.3   17 Mar 1989 23:22:00
 * An attempt to clone a zero length buffer now results in a static buffer
 * with a length of zero at address zero.
 * 
 *    Rev 1.2   17 Mar 1989 21:41:42
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.1   14 Sep 1988 17:57:18
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:52
 * Initial revision.
*/

#include <buffer.h>

/****************************************************************************
NAME:  EBufferClone

PURPOSE:  Clone an extended buffer

PARAMETERS:
        EBUFFER_T *     Source buffer structure
        EBUFFER_T *     Destination buffer structure

RETURNS:  0 if sucessful, -1 if not
****************************************************************************/
int
  EBufferClone(EBUFFER_T * srcp,
               EBUFFER_T * dstp)
{
ALENGTH_T need;

need = EBufferUsed(srcp);

if (need != 0) {
    OCTET_T *newbuffp;
    if ((newbuffp = (OCTET_T *)SNMP_memory_alloc(need)) == 0) {
         /* Allocation failed */
        return -1;
        }

    (void) MEMCPY(newbuffp, srcp->start_bp, need);
    EBufferPreLoad(BFL_IS_DYNAMIC, dstp, newbuffp, need);
    }
else { /* Length is zero */
    EBufferPreLoad(BFL_IS_STATIC, dstp, 0, 0);
    }
return 0;
}

/****************************************************************************

NAME:  EBufferClean

PURPOSE:  Release the buffer memory if possible

PARAMETERS:
            EBUFFER_T *     Buffer descriptor

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
void
  EBufferClean(EBUFFER_T * ebuffp)
{
if ((ebuffp->bflags & BFL_IS_DYNAMIC) && (ebuffp->start_bp != 0))
    SNMP_memory_free((char *)(ebuffp->start_bp));

EBufferInitialize(ebuffp);
}

/****************************************************************************

NAME:  EBufferAppend

PURPOSE:  Append the contents of one buffer onto another.

PARAMETERS:
            EBUFFER_T *     First (and destination) buffer descriptor
            EBUFFER_T *     Second buffer descriptor

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
void
  EBufferAppend(EBUFFER_T * b1p,
                EBUFFER_T * b2p)
{
ALENGTH_T used;

if ((used = min(EBufferUsed(b2p), b1p->remaining)) != 0)
    (void)MEMCPY(b1p->next_bp, b2p->start_bp, used);
b1p->next_bp += used;
b1p->remaining -= used;
}

/****************************************************************************
NAME:  EBufferSeek

PURPOSE:  Move the pointers to a new position in the buffer.

PARAMETERS:
        EBUFFER_T *     Source buffer structure
        ALENGTH_T       Length to move the pointers.
        int             0 to set pointer to offset bytes from the start
                        1 to move pointer by offset bytes
                        2 to set pointer to offset bytes from the end

RETURNS:  0 if sucessful, -1 if not
****************************************************************************/
int
  EBufferSeek(EBUFFER_T  *ebuffp,
              ALENGTH_T   offset,
              int         whence)
{
switch (whence)
    {
    case 0:
        if (offset > ((ALENGTH_T)(ebuffp->next_bp - ebuffp->start_bp) +
                      ebuffp->remaining))
            return(-1);
        ebuffp->remaining = ((ALENGTH_T)(ebuffp->next_bp - ebuffp->start_bp))
                            + ebuffp->remaining - offset;
        ebuffp->next_bp = ebuffp->start_bp + offset;
        break;

    case 1:
        if (offset > ebuffp->remaining)
            return(-1);
        ebuffp->next_bp += offset;
        ebuffp->remaining -= offset;
        break;

    case 2:
        if (offset > ((ALENGTH_T)(ebuffp->next_bp - ebuffp->start_bp) +
                      ebuffp->remaining))
            return(-1);
        ebuffp->next_bp = ebuffp->next_bp + ebuffp->remaining - offset;
        ebuffp->remaining = offset;
        break;

    default:
        return (-1);
    }

return(0);
}

/****************************************************************************

NAME:  EBufferAdjust

PURPOSE:  Make the buffer at least the given length long.  If necessary
          allocate more space.  NOTE: you can not use this routine to
          extend a static buffer it will return a -1 failure.

PARAMETERS:
            EBUFFER_T *     Buffer descriptor
            ALENGTH_T       Minimum length for the buffer

RETURNS:    int         0 on success

RESTRICTIONS:  

BUGS:  
****************************************************************************/
int
  EBufferAdjust(EBUFFER_T * ebuffp,
                ALENGTH_T   length)
{
ALENGTH_T used;
OCTET_T *buf;

used = EBufferUsed(ebuffp);
if (used + ebuffp->remaining >= length)
    return(0);
if ((ebuffp->bflags != BFL_IS_DYNAMIC) ||
    ((buf = (OCTET_T *)SNMP_memory_alloc(length)) ==  0))
    return(-1);
if (used != 0)
    MEMCPY(buf, ebuffp->start_bp, used);
if ((ebuffp->bflags & BFL_IS_DYNAMIC) && (ebuffp->start_bp != 0))
    SNMP_memory_free((char *)(ebuffp->start_bp));
ebuffp->start_bp = buf;
ebuffp->next_bp = buf + used;
ebuffp->remaining = length - used;
return(0);
}

/****************************************************************************

NAME:  EBufferReplace

PURPOSE:  Replace the string currently in the ebuffer with a copy of the 
          given buffer.  If necessary allocate more space to grow the buffer.

PARAMETERS:
            EBUFFER_T *     EBuffer descriptor
            OCTET_T   *     buffer pointer
            ALENGTH_T       length for the buffer

RETURNS:    int         0 on success

RESTRICTIONS:  

BUGS:  
****************************************************************************/
int
  EBufferReplace(EBUFFER_T * ebuffp,
                 OCTET_T   * buffer,
                 ALENGTH_T   length)
{
OCTET_T *newbuf;

if (length != 0) {
    if (length > (EBufferUsed(ebuffp) + ebuffp->remaining)) {
        if ((newbuf = (OCTET_T *)SNMP_memory_alloc((int)length)) == 0)
            return(-1);
        if ((ebuffp->bflags & BFL_IS_DYNAMIC) && (ebuffp->start_bp != 0))
            SNMP_memory_free((char *)(ebuffp->start_bp));
        ebuffp->bflags = BFL_IS_DYNAMIC;
        ebuffp->start_bp = newbuf;
        }

    MEMCPY(ebuffp->start_bp, buffer, (int)length);
    }
EBufferSeek(ebuffp, length, 0);
return(0);
}

