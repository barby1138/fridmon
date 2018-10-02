/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/asy_back.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 *  Copyright 1995-1997 Epilogue Technology Corporation.
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
 * $Log: asy_back.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:35  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:32  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  22:59:09  sar
 * nextpdu.c removed bitstring, uinteger32, nsapaddress
 *
 * Revision 6.0  1995/05/31  21:47:00  sra
 * Release 6.0.
 *
 * Revision 1.6  1995/05/09  22:35:29  sar
 * Corrected the call to the get routine in async_compat_next to use
 * the newly discovered oid rather than whatever was passed in.
 *
 * Revision 1.5  1995/05/08  20:52:42  sar
 * deleted some code that was commented out.
 *
 * Revision 1.4  1995/05/08  20:32:04  sar
 * cast the routines when calling the old setprocs to make sure compilers
 * do the right thing with the args.
 *
 * Revision 1.3  1995/05/02  23:34:50  sar
 * Moved the declarations of the v2 object types into the v2 type block.
 *
 * Revision 1.2  1995/04/28  23:12:28  sar
 * Dropped the static/dynamice flag in the call to nextproc_next_instance
 *
 * Revision 1.1  1995/03/21  00:09:23  sar
 * Initial revision
 *
 * Initial revision.
 */

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <objectid.h>

/****************************************************************************
NAME:  async_back_compat_test

PURPOSE:  This is a glue routine to translate from the async method
          routine api to the older sync method routine api.  This
          will allow older method routines to be used with the async
          engine code.
          For tests we simply call the old test routine with the
          appropriate arguments.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T       *    Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  async_compat_test(OIDC_T       last_match,
                    int          compc,
                    OIDC_T      *compl,
                    SNMP_PKT_T  *pktp,
                    VB_T        *vbp)
{
COMPAT_ROUTINES_T *crts;
int rcode;

crts = (COMPAT_ROUTINES_T *)vbp->vb_ml.ml_leaf->user_cookie;
rcode = (crts->testproc)(1, last_match, compc, compl, crts->cookie, pktp,
                         vbp_to_index(pktp, vbp));
if (rcode == 0)
    testproc_good(pktp, vbp);
else
    testproc_tproc_error(pktp, vbp, rcode);
return;
}

/****************************************************************************
NAME:  async_get_routines

PURPOSE:  Routine to hadle calling the old type get routines, used
          by both async_compat_{get next}.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T       *    Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  async_get_routines(OIDC_T      last_match,
                     int         compc,
                     OIDC_T     *compl,
                     SNMP_PKT_T *pktp,
                     VB_T       *vbp)
{
COMPAT_ROUTINES_T *crts;
OCTET_T data_type, *cp;
INT_32_T int_value;
UINT_32_T uint_value;
OBJ_ID_T *objp;
int length, dynamic = 0;

crts = (COMPAT_ROUTINES_T *)vbp->vb_ml.ml_leaf->user_cookie;

data_type = vbp->vb_ml.ml_leaf->expected_tag;
switch (data_type) {
    case VT_NUMBER:
        int_value = 
          (*((INT_32_T (*) __((OIDC_T, int, OIDC_T *, char *, SNMP_PKT_T *)))
             (crts->getproc))) (last_match, compc, compl, crts->cookie, pktp);
        getproc_got_int32(pktp, vbp, int_value);
        break;
    case VT_COUNTER:
    case VT_GAUGE:
    case VT_TIMETICKS:
        uint_value = 
          (*((UINT_32_T (*) __((OIDC_T, int, OIDC_T *, char *, SNMP_PKT_T *)))
             (crts->getproc))) (last_match, compc, compl, crts->cookie, pktp);
        getproc_got_uint32(pktp, vbp, uint_value, data_type);
        break;
    case VT_STRING:
    case VT_OPAQUE:
        cp = (*((OCTET_T * (*) __((OIDC_T, int, OIDC_T *, char *,
                                   int *, SNMP_PKT_T *, int *)))
                (crts->getproc))) (last_match, compc, compl, crts->cookie,
                                   &length, pktp, &dynamic);
        getproc_got_string(pktp, vbp, length, cp, dynamic, data_type);
        break;
    case VT_OBJECT:
        objp = (*((OBJ_ID_T * (*) __((OIDC_T, int, OIDC_T *,
                                      char *, SNMP_PKT_T *, int *)))
                  (crts->getproc))) (last_match, compc, compl,
                                     crts->cookie, pktp, &dynamic);
        if (objp == 0)
            /* no object id was returned */
            getproc_got_object_id(pktp, vbp, 0, 0, 0);
        else {
            getproc_got_object_id(pktp, vbp, objp->num_components,
                                  objp->component_list, dynamic);
            if (dynamic)
                SNMP_memory_free(objp);
            }
        break;
    case VT_EMPTY:
        getproc_got_empty(pktp, vbp);
        break;
    case VT_IPADDRESS:
        cp = (*((OCTET_T * (*) __((OIDC_T, int, OIDC_T *, char *,
                                   SNMP_PKT_T *, UINT_32_T *)))
                (crts->getproc)))(last_match, compc, compl, crts->cookie,
                                  pktp, &uint_value);
        if (cp != 0)
            MEMCPY(&uint_value, cp, 4);
        getproc_got_ip_address(pktp, vbp, uint_value);
        break;

/* We only include the following case if v2 types is installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
    case VT_COUNTER64:
        {
        UINT_64_T *uint64_value;
        uint64_value = (*((UINT_64_T * (*) __((OIDC_T, int, OIDC_T *,
                                               char *, SNMP_PKT_T *, int *)))
                          (crts->getproc))) (last_match, compc, compl,
                                             crts->cookie, pktp, &dynamic);
        if (uint64_value == 0)
            /* no counter was returned */
            getproc_got_uint64_high_low(pktp, vbp, 0, 0);
        else {
            getproc_got_uint64(pktp, vbp, uint64_value);
            if (dynamic)
                SNMP_memory_free(uint64_value);
            }
        break;
        }
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */

    default:
        getproc_error(pktp, vbp, GEN_ERR);
        break;
    }

return;
}

/****************************************************************************
NAME:  async_compat_next

PURPOSE:  This is a glue routine to translate from the async method
          routine api to the older sync method routine api.  This
          will allow older method routines to be used with the async
          engine code.
          For nexts we call the old next followed by an old get
          and then insert the information into the packet.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T       *    Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  async_compat_next(OIDC_T       last_match,
                    int          compc,
                    OIDC_T      *compl,
                    SNMP_PKT_T  *pktp,
                    VB_T        *vbp)
{
COMPAT_ROUTINES_T *crts;
int rcount;
OIDC_T rlist[MAX_OID_COUNT];

crts = (COMPAT_ROUTINES_T *)vbp->vb_ml.ml_leaf->user_cookie;

rcount = crts->nextproc(last_match, compc, compl, rlist, crts->cookie, pktp);
if (rcount == 0) {
    nextproc_no_next(pktp, vbp);
    return;
    }
nextproc_next_instance(pktp, vbp, rcount, rlist);

async_get_routines(last_match, rcount, rlist, pktp, vbp);
return;
}

/****************************************************************************
NAME:  async_compat_get

PURPOSE:  This is a glue routine to translate from the async method
          routine api to the older sync method routine api.  This
          will allow older method routines to be used with the async
          engine code.
          For gets we do a test with the correct arguments followed
          by a get and then insert the information into the packet.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T       *    Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  async_compat_get(OIDC_T        last_match,
                   int           compc,
                   OIDC_T       *compl,
                   SNMP_PKT_T   *pktp,
                   VB_T         *vbp)
{
COMPAT_ROUTINES_T *crts;
int rcode;

crts = (COMPAT_ROUTINES_T *)vbp->vb_ml.ml_leaf->user_cookie;
rcode = (crts->testproc)(0, last_match, compc, compl, crts->cookie,
                         pktp, vbp_to_index(pktp, vbp));

if (rcode) {
    getproc_nosuchins(pktp, vbp);
    return;
    }

async_get_routines(last_match, compc, compl, pktp, vbp);
return;
}

/****************************************************************************
NAME:  async_compat_set

PURPOSE:  This is a glue routine to translate from the async method
          routine api to the older sync method routine api.  This
          will allow older method routines to be used with the async
          engine code.
          For sets all we can do is call the set routine, we would
          like to be able to use undos but they didn't exist so we
          can't.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T       *    Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  async_compat_set(OIDC_T        last_match,
                   int           compc,
                   OIDC_T       *compl,
                   SNMP_PKT_T   *pktp,
                   VB_T         *vbp)
{
COMPAT_ROUTINES_T *crts;

crts = (COMPAT_ROUTINES_T *)vbp->vb_ml.ml_leaf->user_cookie;

switch (vbp->vb_ml.ml_leaf->expected_tag) {
    case VT_NUMBER:
    case VT_COUNTER:
    case VT_GAUGE:
    case VT_TIMETICKS:
        (*((void (*) __((OIDC_T, int, OIDC_T *, char *,
                         INT_32_T, SNMP_PKT_T *, int)))
           (crts->setproc)))(last_match, compc, compl, crts->cookie,
                             vbp->value_u.v_number, pktp,
                             vbp_to_index(pktp, vbp));
        break;
    case VT_STRING:
    case VT_OPAQUE:
        (*((void (*) __((OIDC_T, int, OIDC_T *, char *,
                         char *, int, SNMP_PKT_T *, int)))
           (crts->setproc)))(last_match, compc, compl, crts->cookie,
                             (char *)vbp->value_u.v_string.start_bp,
                             (int)EBufferUsed(&vbp->value_u.v_string),
                             pktp, vbp_to_index(pktp, vbp));
        break;
    case VT_OBJECT:
        (*((void (*) __((OIDC_T, int, OIDC_T *, char *,
                         int, OIDC_T *, SNMP_PKT_T *, int)))
         (crts->setproc)))(last_match, compc, compl, crts->cookie,
                                 vbp->value_u.v_object.num_components,
                                 vbp->value_u.v_object.component_list,
                                 pktp, vbp_to_index(pktp, vbp));
        break;
    case VT_EMPTY:
        break;
    case VT_IPADDRESS:
        /* Calling sequence to set IP address is same as for strings */
        (*((void (*) __((OIDC_T, int, OIDC_T *, char *,
                         char *, int, SNMP_PKT_T *, int)))
           (crts->setproc)))(last_match, compc, compl, crts->cookie,
                             (char *)vbp->value_u.v_network_address, 4,
                             pktp, vbp_to_index(pktp, vbp));
        break;
/* We only include the following cases if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
    case VT_COUNTER64:
        (*((void (*) __((OIDC_T, int, OIDC_T *, char *,
                         UINT_32_T, UINT_32_T, SNMP_PKT_T *, int)))
           (crts->setproc)))(last_match, compc, compl, crts->cookie,
                             UI64_High(&vbp->value_u.v_counter64),
                             UI64_Low(&vbp->value_u.v_counter64),
                             pktp, vbp_to_index(pktp, vbp));
        break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
    default:
        setproc_error(pktp, vbp, GEN_ERR);
        return;
    }

setproc_good(pktp, vbp);
return;
}
