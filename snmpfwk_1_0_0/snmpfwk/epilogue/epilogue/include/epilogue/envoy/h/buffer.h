/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/buffer.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: buffer.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:23  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:05  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:34  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.6  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo> where it was correct.
 *
 * Revision 3.5  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 3.4  1993/03/20  00:21:50  sar
 * Replaced EBufferAdd with EBufferAdjust which takes a length and if
 * necessary extends the buffer.
 * Added EBufferReplace which puts the given string into the buffer
 * extending the buffer if necessary.
 *
 * Revision 3.3  1993/03/05  03:56:25  dab
 * fixed up types
 *
 * Revision 3.2  1993/03/02  19:04:38  sar
 * added definition of EBuffer_add which extends the string in a buffer.
 *
 * Revision 3.1  1993/02/17  20:46:41  sar
 * Added EBufferStart - macro: obtain address of first byte in a buffer.
 * Added EBufferSeek - definition: move the pointer in a buffer.
 *
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.102  1992/03/31  14:04:08  dab
 * Fixed EBufferReset to fixup the remaining field too.
 *
 * Revision 2.101  92/02/04  11:04:37  dab
 * Changed the EBufferReset macro to return void.
 * 
 * Revision 2.100  91/08/09  14:09:44  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:21  romkey
 * Initial revision
 * 
 * 
 * Revision 2.0   31-Mar-1990 15:11:22 dab
 * Release 2.00
 * 
 *    Rev 1.1   19 Sep 1988 17:27:04
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:12
 * Initial revision.
*/

#if (!defined(buffer_inc))
#define buffer_inc

#if (!defined(asn1_inc))
#include <envoy/h/asn1.h>
#endif

/* The EBUFFER_T structure is used to control encoding into a buffer    */
typedef struct EBUFFER_S
        {
        unsigned short int      bflags;
        OCTET_T     *start_bp;  /* Start of the buffer.                     */
        OCTET_T     *next_bp;   /* Next location in buffer to be filled     */
        ALENGTH_T    remaining; /* Number of empty spots remaining in buffer*/
        } EBUFFER_T;

/* Values for bflags    */
#define BFL_IS_DYNAMIC          0x0001  /* Buffer was obtained by malloc    */
#define BFL_IS_STATIC           0x0000  /* Buffer is statically allocated   */

/****************************************************************************

NAME:  EBufferInitialize

PURPOSE:  Initialize a buffer to a know, but not usable state
          EBufferSetup must be used to make the buffer ready for use.

PARAMETERS:
            EBUFFER_T *     Buffer descriptor

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferInitialize(E) (((EBUFFER_T *)(E))->bflags=0,   \
                              ((EBUFFER_T *)(E))->next_bp=(OCTET_T *)0,  \
                              ((EBUFFER_T *)(E))->start_bp=(OCTET_T *)0,  \
                              ((EBUFFER_T *)(E))->remaining=(ALENGTH_T)0)

/****************************************************************************

NAME:  EBufferSetup

PURPOSE:  Setup a buffer to receive ASN.1 encoded data

PARAMETERS:
            unsigned int    BFL_xxx flags from buffer.h
            EBUFFER_T *     Buffer descriptor
            OCTET_T *       Address of the buffer
            ALENGTH_T       Length of the buffer

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferSetup(F,E,B,L) (((EBUFFER_T *)(E))->bflags=(F),  \
                               ((EBUFFER_T *)(E))->start_bp=(OCTET_T *)(B),  \
                               ((EBUFFER_T *)(E))->next_bp=(OCTET_T *)(B),  \
                               ((EBUFFER_T *)(E))->remaining=(ALENGTH_T)(L))

/****************************************************************************

NAME:  EBufferPreLoad

PURPOSE:  Pre-load data into a buffer

PARAMETERS:
            unsigned int    BFL_xxx flags from buffer.h
            EBUFFER_T *     Buffer descriptor
            OCTET_T *       Address of the buffer containing the data
            ALENGTH_T       Length of the data

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferPreLoad(F,E,B,L) (       \
        ((EBUFFER_T *)(E))->bflags=(F), \
        ((EBUFFER_T *)(E))->start_bp=(OCTET_T *)(B),    \
        ((EBUFFER_T *)(E))->next_bp=((OCTET_T *)(B)+(ALENGTH_T)(L)),    \
        ((EBUFFER_T *)(E))->remaining=(ALENGTH_T)0)

/****************************************************************************

NAME:  EBufferNext

PURPOSE:  Obtain address of next free byte in a buffer

PARAMETERS:
            EBUFFER_T *     Buffer descriptor

RETURNS:    OCTET_T *       Buffer descriptor

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferNext(E)  (((EBUFFER_T *)(E))->next_bp)

/****************************************************************************

NAME:  EBufferStart

PURPOSE:  Obtain address of first byte in a buffer

PARAMETERS:
            EBUFFER_T *     Buffer descriptor

RETURNS:    OCTET_T *       Buffer descriptor

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferStart(E) (((EBUFFER_T *)(E))->start_bp)

/****************************************************************************

NAME:  EBufferUsed

PURPOSE:  Indicate how many octets are currently in the buffer.

PARAMETERS:
            EBUFFER_T *     Buffer descriptor

RETURNS:    ALENGTH_T       Number of octets used.

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferUsed(E)  ((ALENGTH_T)((((EBUFFER_T *)(E))->next_bp) -    \
                                     (((EBUFFER_T *)(E))->start_bp)))

/****************************************************************************

NAME:  EBufferRemaining

PURPOSE:  Indicate how many octets are currently unused in the buffer.

PARAMETERS:
            EBUFFER_T *     Buffer descriptor

RETURNS:    ALENGTH_T       Number of octets unused.

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferRemaining(E)     (((EBUFFER_T *)(E))->remaining)

/****************************************************************************

NAME:  EBufferReset

PURPOSE:  Reset the buffer to an empty state, just like after EBufferSetup

PARAMETERS:
            EBUFFER_T *     Buffer descriptor

RETURNS:    Nothing

RESTRICTIONS:  

BUGS:  
****************************************************************************/
#define EBufferReset(E) ( \
                         (((EBUFFER_T *)(E))->remaining += \
                          ((EBUFFER_T *)(E))->next_bp - \
                          ((EBUFFER_T *)(E))->start_bp), \
                         ((void)(((EBUFFER_T *)(E))->next_bp =  \
                                 ((EBUFFER_T *)(E))->start_bp)))

extern  int     EBufferClone   __((EBUFFER_T *, EBUFFER_T *));
extern  void    EBufferClean   __((EBUFFER_T *));
extern  void    EBufferAppend  __((EBUFFER_T *, EBUFFER_T *));
extern  int     EBufferSeek    __((EBUFFER_T *, ALENGTH_T, int));
extern  int     EBufferAdjust  __((EBUFFER_T *, ALENGTH_T));
extern  int     EBufferReplace __((EBUFFER_T *, OCTET_T *, ALENGTH_T));

#endif  /* buffer_inc */
