/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/access.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 * $Log: access.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:34  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:30  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 1.2  1996/02/29  20:31:40  sar
 * added a default define to keep compilers happy
 *
 * Revision 1.1  1996/01/10  16:34:47  sar
 * Initial revision
 *
 */

#include <asn1.h>
#include <snmpdefs.h>
#include <buffer.h>
#include <access.h>

/*OCTET_T access1_string[] = "public";
ACCESSLEAF_T access1 = {0, {0,0,0,0,0,0,0,0,0,0,0,0},
                        {BFL_IS_STATIC, access1_string, access1_string + 6, 0},
                        {0,0,0,0}, 0, 43, 1, 1, 0, 0};*/
ACCESSLEAF_T *root_access = 0;

ACCESSLEAF_T *
  SNMP_Access_Lookup(OCTET_T *agentid,
                     OCTET_T *uname,
                     int      uname_len,
                     OCTET_T *cname,
                     int      cname_len,
                     OCTET_T  qos)
{
ACCESSLEAF_T *access;
for(access = root_access; access; access = access->next)
    if ((MEMCMP(agentid, access->agentid, 12) == 0) &&
       (uname_len == EBufferUsed(&(access->username))) &&
       (MEMCMP(uname, EBufferStart(&(access->username)), uname_len) == 0) &&
       (cname_len == EBufferUsed(&(access->contextname))) &&
       (MEMCMP(cname, EBufferStart(&(access->contextname)), cname_len) == 0) &&
       ((qos & (USEC_PRIV | USEC_AUTH)) >= access->qos))
        return(access);

return(0);
}

INT_16_T
  SNMP_Access_Install(OCTET_T *agentid,
                      OCTET_T *uname,
                      int      uname_len,
                      OCTET_T *cname,
                      int      cname_len,
                      OCTET_T  qos,
                      INT_32_T privileges,
                      UINT_16_T rind,
                      UINT_16_T wind)
{
ACCESSLEAF_T *access;
OCTET_T *ubuf, *cbuf = 0;

if (SNMP_Access_Lookup(agentid, uname, uname_len, cname, cname_len, qos))
    return(1);
if ((access = SNMP_memory_alloc(sizeof(ACCESSLEAF_T))) == 0)
    return(-1);
if ((ubuf = SNMP_memory_alloc(uname_len)) == 0) {
    SNMP_memory_free(access);
    return(-1);
    }
if (cname_len && ((cbuf = SNMP_memory_alloc(cname_len)) == 0)) {
    SNMP_memory_free(ubuf);
    SNMP_memory_free(access);
    return(-1);
    }

MEMCPY(access->agentid, agentid, 12);
MEMCPY(ubuf, uname, uname_len);
EBufferPreLoad(BFL_IS_DYNAMIC, &access->username, ubuf, uname_len);
if (cname_len) {
    MEMCPY(cbuf, cname, cname_len);
    EBufferPreLoad(BFL_IS_DYNAMIC, &access->contextname, cbuf, cname_len);
    }
else
    EBufferInitialize(&access->contextname);
access->qos = qos;
access->privileges = privileges;
access->readindex = rind;
access->writeindex = wind;
/*printf("acc entry: agentid %2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x\n",
       agentid[0],agentid[1],agentid[2],agentid[3],agentid[4],agentid[5],
       agentid[6],agentid[7],agentid[8],agentid[9],agentid[10],agentid[11]);
printf("uname: %s*/
access->next = root_access;
root_access = access;

return(0);
}
