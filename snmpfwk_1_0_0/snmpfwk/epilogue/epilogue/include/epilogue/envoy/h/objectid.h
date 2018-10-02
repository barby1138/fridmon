/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/objectid.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: objectid.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:27  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:14  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:48  sra
 * Release 6.0.
 *
 * Revision 5.1  1995/03/20  23:11:33  sar
 * Minor cleanup.
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
 * Revision 3.3  1993/04/27  21:06:48  sar
 * added copy_object_id which does a straight copy, beware free one copy
 * while using the other.
 *
 * Revision 3.2  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 3.1  1993/02/17  20:49:21  sar
 * Added no_pp definition (#define __(x) x stuff)
 * Added init_object_id - macro: init an OBJ_ID_T to 0 components and 0 list.
 *
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.100  91/08/09  14:09:47  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:29  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:20
 * Release 2.00
 * 
 *    Rev 1.2   11 Jan 1989 12:46:46
 * Moved Clean_Obj_ID() to objectid.c
 * 
 *    Rev 1.1   19 Sep 1988 17:27:08
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:20
 * Initial revision.
*/

#if (!defined(objectid_inc))
#define objectid_inc

#if (!defined(asn1_inc))
#include <envoy/h/asn1.h>
#endif

#define clone_object_id(O, N) build_object_id(  \
                                ((OBJ_ID_T *)(O))->num_components,      \
                                ((OBJ_ID_T *)(O))->component_list, N)

#define init_object_id(O) ((OBJ_ID_T *)(O))->num_components = 0; \
                          ((OBJ_ID_T *)(O))->component_list = 0;

#define copy_object_id(O, N) \
(((OBJ_ID_T *)(N))->num_components = ((OBJ_ID_T *)(O))->num_components, \
 ((OBJ_ID_T *)(N))->component_list = ((OBJ_ID_T *)(O))->component_list)

extern  int             build_object_id __((int, OIDC_T *, OBJ_ID_T *));
extern  void            Clean_Obj_ID    __((OBJ_ID_T *));

#endif  /* objectid_inc */
