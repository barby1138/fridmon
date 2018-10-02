/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/localio.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

/*****************************************************************************
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
 * $Log: localio.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:25  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/03/20 06:48:11  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1997/01/16 20:46:35  sar
 * changed uchar to bits8_t
 *
 * Revision 7.0  1996/03/18  20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1996/01/10  16:23:25  sar
 * Removed some no_pp stuff
 * Added the start of buffer macro
 *
 * Revision 6.0  1995/05/31  21:46:43  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.3  1993/05/19  20:07:52  sar
 * tiding up types to make compilers happy, (timestamps to UINT_32_T,
 * sizes to ALENGTH_T).
 *
 * Revision 3.2  1993/02/17  20:48:03  sar
 * Added Lcl_Next - macro: get current position in buffer.
 * Added Lcl_Dup  - defin: duplicate a lcl_file.
 * Changed Lcl_Size - macro: corrected lbuf->next to lbuf_next.
 *
 * Revision 3.1  92/07/31  11:54:43  dab
 * Added Lcl_Size().
 * 
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.101  92/02/05  18:49:59  dab
 * In lcl_* macros don't cast L as (LCL_FILE *) because it already
 * should be.
 * Made sure all returns of Lcl_Getc() were the same type.
 * 
 * Revision 2.100  91/08/09  14:09:46  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:26  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:26
 * Release 2.00
 * 
 *    Rev 1.2   19 Sep 1988 17:48:36
 * Added header and version identification lines.  No code changes.
*/

#if (!defined(localio_inc))
#define localio_inc

/* Get the standard types */
#if !defined(EPILOGUE_TYPES_H)
#include <types.h>
#endif

typedef struct LCL_FILE_S
        {
        bits8_t  lcl_flags;
        bits8_t *lbuf_start;
        bits8_t *lbuf_next;
        bits8_t *lbuf_end;      /* Address AFTER last byte */
        } LCL_FILE;

#define LCL_MALLOC  0x01    /* LCL_FILE was malloc-ed at open */
#define LCL_EOF     0x80    /* EOF encountered */

/* Start of buffer */
#define Lcl_Start(L) (L)->lbuf_start

#define Lcl_Eof(L) (((L)->lcl_flags&LCL_EOF)?-1:0)
                         
/* Current position in stream. */
#define Lcl_Tell(L) (int)((L)->lbuf_next - (L)->lbuf_start)

/* Address of current positon in buffer */
#define Lcl_Next(L) (L)->lbuf_next

/* Amount of data left in stream. */
#define Lcl_Size(L) (ALENGTH_T)((L)->lbuf_end - (L)->lbuf_next)

#define GETC_MACRO
#if defined(GETC_MACRO)
/* The following is an macro version of this routine Lcl_Getc */
#define Lcl_Getc(L)   ( (((L)->lcl_flags & LCL_EOF) ? -1 :      \
                         (((L)->lbuf_next < (L)->lbuf_end) ?    \
                          (int)(*((L)->lbuf_next++)) :  \
                          (((L)->lcl_flags |= LCL_EOF), -1))))
#endif  /* GETC_MACRO */

extern  LCL_FILE    *Lcl_Open   (LCL_FILE *, bits8_t *, int);
extern  void         Lcl_Close  (LCL_FILE *);
#if !defined(GETC_MACRO)
extern  int          Lcl_Getc   (LCL_FILE *);
#endif  /* GETC_MACRO */
extern  int          Lcl_Peekc  (LCL_FILE *);
extern  int          Lcl_Read   (LCL_FILE *, bits8_t *, int);
extern  int          Lcl_Seek   (LCL_FILE *, int, int);
extern  int          Lcl_Resize (LCL_FILE *, int, int);
extern  LCL_FILE    *Lcl_Dup    (LCL_FILE *, LCL_FILE *);

#endif
