/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/access.h,v 1.1 2006/06/29 12:17:07 smasyutin Exp $ */

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
 *  Copyright 1996-1997 Epilogue Technology Corporation.
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
 * $Log: access.h,v $
 * Revision 1.1  2006/06/29 12:17:07  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:09  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:19  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:47:55  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 1.1  1996/01/10  16:23:25  sar
 * Initial revision
 *
 */

#if (!defined(access_inc))
#define access_inc
#include <asn1.h>
#include <buffer.h>

typedef struct ACCESSLEAF_S
        {
        struct ACCESSLEAF_S *next;
        OCTET_T         agentid[12];
        EBUFFER_T       username;
        EBUFFER_T       contextname;
        OCTET_T         qos;
        INT_32_T        privileges;
        UINT_16_T       readindex;
        UINT_16_T       writeindex;
        INT_16_T        memory;
        INT_16_T        status;
        } ACCESSLEAF_T;

/* agentid, user string, user string length,
   context string, context string length, qos */
extern ACCESSLEAF_T * SNMP_Access_Lookup  __((OCTET_T *, OCTET_T *, int,
                                              OCTET_T *, int, OCTET_T));
extern INT_16_T       SNMP_Access_Install __((OCTET_T *, OCTET_T *, int,
                                             OCTET_T *, int, OCTET_T,
                                             INT_32_T, UINT_16_T, UINT_16_T));

#define SNMP_Access_Get_Context(A)      &((A)->contextname)
#define SNMP_Access_Get_QOS(A)          (A)->qos
#define SNMP_Access_Get_Privileges(A)   (A)->privileges
#define SNMP_Access_Get_ReadIndex(A)    (A)->readindex
#define SNMP_Access_Get_WriteIndex(A)   (A)->writeindex
#define SNMP_Access_Get_Memory(A)       (A)->memory
#define SNMP_Access_Get_Status(A)       (A)->status

#define SNMP_Access_Set_Privileges(A, V)        (A)->privileges = V
#define SNMP_Access_Set_ReadIndex(A, V)         (A)->readindex = V
#define SNMP_Access_Set_WriteIndex(A, V)        (A)->wrieindex = V
#define SNMP_Access_Set_Memory(A, V)            (A)->memory = V
#define SNMP_Access_Set_Status(A, V)            (A)->Status = V

#endif /* (!defined(access_inc)) */
