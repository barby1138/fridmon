/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/user.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: user.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:05  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:49:24  sra
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
#include <buffer.h>
#include <snmpdefs.h>
#include <md5.h>
#include <user.h>

/*OCTET_T user1_string[] = "public";
USERLEAF_T user1 = { 0, {0,0,0,0,0,0,0,0,0,0,0,0},
                     {BFL_IS_STATIC, user1_string, user1_string + 6, 0},
                     0, {0, 0, 0, 0}, 0, {0, 0, 0, 0}, 1, 0, 0};*/
USERLEAF_T *root_user = 0;

USERLEAF_T *
  SNMP_User_Lookup(OCTET_T *agentid,
                   OCTET_T *uname,
                   int      uname_len)
{
USERLEAF_T *user;

for(user = root_user; user; user = user->next)
    if ((MEMCMP(agentid, user->agentid, 12) == 0) &&
        (uname_len == EBufferUsed(&(user->username))) &&
        (MEMCMP(uname, EBufferStart(&(user->username)), uname_len) == 0)) {
        return(user);
        }

return(0);
}

INT_16_T
  SNMP_User_Install(OCTET_T *agentid,
                    OCTET_T *uname,
                    int      uname_len,
                    AUTH_ROUTINE_T *auth,
                    OCTET_T *authkey)
{
USERLEAF_T *user, *ubuf, *kbuf = 0;
if (user = SNMP_User_Lookup(agentid, uname, uname_len))
    if (MEMCMP(agentid, user->agentid, 12) == 0)
        return(1);

if ((user = SNMP_memory_alloc(sizeof(USERLEAF_T))) == 0)
    return(-1);
if ((ubuf = SNMP_memory_alloc(uname_len)) == 0) {
    SNMP_memory_free(user);
    return(-1);
    }
if (auth && ((kbuf = SNMP_memory_alloc(16)) == 0)) {
    SNMP_memory_free(user);
    SNMP_memory_free(ubuf);
    return(-1);
    }

MEMSET(user, 0, sizeof(USERLEAF_T));
MEMCPY(user->agentid, agentid, 12);
MEMCPY(ubuf, uname, uname_len);
EBufferPreLoad(BFL_IS_DYNAMIC, &user->username, ubuf, uname_len);
if (auth) {
    user->auth = auth;
    MEMCPY(kbuf, authkey, 16);
    EBufferPreLoad(BFL_IS_DYNAMIC, &user->authkey, kbuf, 16);
    user->maxqos = 1;
    }
user->next = root_user;
root_user = user;
return(0);
}

