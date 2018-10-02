/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/localio.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: localio.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:50  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/03/20 06:48:58  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1997/01/16 20:46:52  sar
 * changed uchar to bits8_t
 *
 * Revision 7.0  1996/03/18  20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  22:59:09  sar
 * removed no_pp stuff & casts of 0
 * modified memcpy to MEMCPY
 *
 * Revision 6.0  1995/05/31  21:47:26  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.1  1993/08/04  19:13:11  sra
 * Add a cast to make Borland C++ 3.1 happier.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.3  1993/05/14  19:02:36  sar
 * made oldval in lcl_resize a size_t and cast it to an int for return
 * to avoid compiler warnings.
 *
 * Revision 3.2  1993/03/06  21:55:24  dab
 * Lcl_Dup() shouldn't assume that newfile is initialized even it
 * if it's non-zero.
 *
 * Revision 3.1  1993/02/17  21:04:43  sar
 * Removed #define then and uses of it.
 * Added Lcl_Dup - duplicate an i/o stream.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.102  91/09/18  12:32:21  dab
 * Updated to use new macros from <asn1conf.h> and <snmpconf.h>.
 * 
 * Revision 2.101  91/08/15  12:31:02  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.100  91/08/09  14:08:22  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:44  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:48
 * Release 2.00
 * 
 *    Rev 1.4   27 Apr 1989 15:56:38
 * Removed unused variables
 * 
 *    Rev 1.3   12 Apr 1989 12:02:10
 * Added cast on SNMP_mem_alloc.
 * 
 *    Rev 1.2   23 Mar 1989 11:22:02
 * Revised Lcl_Open to return a null pointer if it can't obtain dynamic
 * memory for a local file descriptor.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:10
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:47:00
 * Initial revision.
*/

#include <asn1conf.h>
#include <localio.h>

/****************************************************************************
Local I/O package -- performs I/O like operations on data in a buffer.

Caveat: This version supports only input operations.
****************************************************************************/

/****************************************************************************
Lcl_Open -- Open a buffer for use as a local I/O stream

Parameters:
        LCL_FILE *      lfile
        bits8_t  *      buffer
        int             nbytes

If lfile == (LCL_FILE *)0 a local file descriptor will be allocated.

Returns: (LCL_FILE *) if sucessful, (LCL_FILE *)0 if unsucessful.
****************************************************************************/
LCL_FILE *
  Lcl_Open(register LCL_FILE *lfile,
                    bits8_t  *buff,
                    int       nbytes)
{
if (lfile == 0) {
    if ((lfile = (LCL_FILE *)SNMP_memory_alloc(sizeof(LCL_FILE))) == 0)
        return 0;
    else
        lfile->lcl_flags = LCL_MALLOC;
    }
else
    lfile->lcl_flags = 0;

lfile->lbuf_start = buff;
lfile->lbuf_next = buff;
lfile->lbuf_end = (bits8_t *)(buff + nbytes);
return (lfile);
}
    
/****************************************************************************
Lcl_Close -- Close a local file descriptor

Parameters:
        LCL_FILE *      lfile

Returns: nothing
****************************************************************************/
void
  Lcl_Close(register LCL_FILE *lfile)
{
if (lfile->lcl_flags & LCL_MALLOC) SNMP_memory_free((char *)lfile);
}

/****************************************************************************
Lcl_Getc -- Read a character from a local I/O stream

Parameters:
        LCL_FILE *      lfile

Returns: If sucessful: the character read, but in integer format
         If unsucessful: -1
****************************************************************************/
#if !defined(GETC_MACRO)
int
  Lcl_Getc(register LCL_FILE *lfile)
{
if (lfile->lcl_flags & LCL_EOF) return -1;
if (lfile->lbuf_next < lfile->lbuf_end)
        return (*(lfile->lbuf_next++));
   else {
        lfile->lcl_flags |= LCL_EOF;
        return -1;
        }
/*NOTREACHED*/
}
#endif  /* GETC_MACRO */

/****************************************************************************
Lcl_Peekc -- Peek at a character from a local I/O stream, the seek pointer
             is not advanced.

Parameters:
        LCL_FILE *      lfile

Returns: If sucessful: the character read, but in integer format
         If unsucessful: -1
****************************************************************************/
int
  Lcl_Peekc(register LCL_FILE *lfile)
{
if (lfile->lcl_flags & LCL_EOF)
    return -1;
if (lfile->lbuf_next < lfile->lbuf_end)
    return (*(lfile->lbuf_next));
else {
    lfile->lcl_flags |= LCL_EOF;
    return -1;
    }
/*NOTREACHED*/
}

/****************************************************************************
Lcl_Read -- Read a set of characters from a local I/O stream.

Parameters:
        LCL_FILE *      lfile
        bits8_t  *      ubuf
        int             nbytes

Returns: The number of bytes actually read.
****************************************************************************/
int
  Lcl_Read(LCL_FILE *lfile,
           bits8_t  *ubuf,
           int       nbytes)
{
/* This is a quick implementation, to be replaced later */
int orig_nbytes = nbytes;
bits8_t c;
while (nbytes > 0)
    {
    c = (bits8_t)Lcl_Getc(lfile);
    if (Lcl_Eof(lfile)) break;
    *ubuf++ = c;
    --nbytes;
    }
return (orig_nbytes - nbytes);
}

/****************************************************************************
Lcl_Seek -- Move the seek pointer to a give position in the local I/O buffer.

Parameters:
        LCL_FILE *      lfile
        int             offset
        int             whence:
                        0 to set pointer to offset bytes from the start
                        1 to move pointer by offset bytes
                        2 to set pointer to offset bytes from the end

Returns: 0 if sucessful, -1 if not.

Note: The "end" position is the byte AFTER the last one in the buffer
containing data.  Thus, a Lcl_Seek(.., 0, 2) will leave the caller at
the end-of-file.  The last byte is reached by Lcl_Seek(.., 1, 2).
****************************************************************************/
int
  Lcl_Seek(register LCL_FILE *lfile,
                    int       offset,
                    int       whence)
{
register bits8_t *next;

switch (whence)
    {
    case 0:
        next = (bits8_t *)(lfile->lbuf_start + offset);
        break;
    case 1:
        next = (bits8_t *)(lfile->lbuf_next + offset);
        break;
    case 2:
        next = (bits8_t *)(lfile->lbuf_end - offset);
        break;
    default:
        return -1;
    }

if ((next < lfile->lbuf_start) || (next > lfile->lbuf_end)) return -1;

if (next < lfile->lbuf_end) lfile->lcl_flags &= ~LCL_EOF;
lfile->lbuf_next = next;
return 0;
}

/****************************************************************************
Lcl_Resize -- Move the end-of-buffer position for the local I/O buffer.
              The buffer may be extended or contracted.

Parameters:
        LCL_FILE *      lfile
        int             offset
        int             whence:
                        0 to set new end to offset bytes from the start
                        1 to move new end by offset bytes from the current
                          read/write position
                        2 to set new end to offset bytes from the end

Returns: Previous value (relative to the start of the file).
         -1 indicates an error.
****************************************************************************/
int
  Lcl_Resize(register LCL_FILE *lfile,
                      int       offset,
                      int       whence)
{
size_t  oldval;

oldval = (size_t) (lfile->lbuf_end - lfile->lbuf_start);

switch (whence)
    {
    case 0:
        lfile->lbuf_end = (bits8_t *)(lfile->lbuf_start + offset);
        break;
    case 1:
         lfile->lbuf_end = (bits8_t *)(lfile->lbuf_next + offset);
        break;
    case 2:
         lfile->lbuf_end = (bits8_t *)(lfile->lbuf_end - offset);
        break;
    default:
        return -1;
    }

if (lfile->lbuf_next < lfile->lbuf_end)
    lfile->lcl_flags &= ~LCL_EOF;
else
    lfile->lcl_flags |= LCL_EOF;

return oldval;
}

/****************************************************************************
Lcl_Dup -- Duplicate an I/O stream

Parameters:
        LCL_FILE *      newfile
        LCL_FILE *      oldfile

If newfile == (LCL_FILE *)0 a local file descriptor will be allocated.
If newfile != 0, the structure is assumed to be uninitialized and we mark
the structure as not-malloc'd.

Returns: (LCL_FILE *) if sucessful, (LCL_FILE *)0 if unsucessful.
****************************************************************************/
LCL_FILE *
  Lcl_Dup(register LCL_FILE *newfile,
          register LCL_FILE *oldfile)
{
if (newfile == 0) {
    if ((newfile = (LCL_FILE *)SNMP_memory_alloc(sizeof(LCL_FILE))) == 0)
        return 0;
    else
        newfile->lcl_flags = LCL_MALLOC | oldfile->lcl_flags;
    }
else
    newfile->lcl_flags = oldfile->lcl_flags & ~LCL_MALLOC;

newfile->lbuf_start = oldfile->lbuf_start;
newfile->lbuf_next = oldfile->lbuf_next;
newfile->lbuf_end = oldfile->lbuf_end;
return (newfile);
}
    

