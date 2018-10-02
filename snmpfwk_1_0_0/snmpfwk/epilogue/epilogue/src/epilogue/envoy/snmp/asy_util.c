/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/asy_util.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 * $Log: asy_util.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:03  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:37  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:36  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/10/16 23:49:22  sar
 * created testproc_error to allow for changing which error
 * to return depending on the type of the packet
 *
 * Revision 7.2  1997/03/20 06:48:35  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:47:01  sra
 * Release 6.0.
 *
 * Revision 1.4  1995/05/08  20:32:04  sar
 * changed the args to nextproc_next_instance & getproc_got_object_id
 * to make compilers happier.
 *
 * Revision 1.3  1995/04/28  23:12:28  sar
 * Dropped the static/dynamice flag in the call to nextproc_next_instance
 *
 * Revision 1.2  1995/04/18  21:13:44  sar
 * Removed an unnecessary break.
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
NAME:  getproc_nosuchins

PURPOSE:  Routine to tag a variable as either a nosuchins exception in v2
          or a nosuchobject error in v1.

PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU
        VB_T       *    The var bind to attach the string to

RETURNS: void
****************************************************************************/
void
  getproc_nosuchins(SNMP_PKT_T  *pktp,
                    VB_T        *vbp)
{
if (pktp->snmp_version == SNMP_VERSION_1) {
    pktp->pdu.std_pdu.error_status = NO_SUCH_NAME;
    pktp->pdu.std_pdu.error_index = vbp_to_index(pktp, vbp) + 1;
    }
else
    vbp->vb_data_flags_n_type = VT_NOSUCHINS;

vbp->vb_flags |= VFLAG_GET_DONE;
}

/****************************************************************************
NAME:  getproc_got_object_id

PURPOSE:  Routine to attache an object id to a var bind.

PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU
        VB_T       *    The var bind to attach the string to
        size_t          The length of the string
        OIDC_T     *    The string to attach
        int             The dynamic flag static (0), dynamic (1)

RETURNS: void
****************************************************************************/
void
  getproc_got_object_id(SNMP_PKT_T      *pktp,
                        VB_T            *vbp,
                        int              sz,
                        OIDC_T          *oidc,
                        int              dynamic)
{
vbp->vb_flags |= VFLAG_GET_DONE;
vbp->vb_data_flags_n_type = VT_OBJECT;
if (dynamic) {
    vbp->value_u.v_object.num_components = sz;
    vbp->value_u.v_object.component_list = oidc;
    }
else {
    if (build_object_id(sz, oidc, &(vbp->value_u.v_object)))
        /* An error occurred trying to copy the object id, probably
           we couldn't allocate memory, so we set up for an error return */
        pktp->pdu.std_pdu.error_status = GEN_ERR;
    }

}

/****************************************************************************
NAME:  nextproc_next_instance

PURPOSE:  Routine to attache a string to a var bind.
          This routine assumes that the remaining object information
          in the mib_loc structure has been filled in (done by
          find_next_object).  It further assumes that the oidc pointed
          to by the remaining block is a part of the  oidc in the
          vb_obj_id block.  So if the new id is shorter or equal to
          the remaining oid we don't need to allocate anything
          otherwise we need to allocate space and copy the old object
          and the new instance into that space then attache it correctly
          and clean up.
          
PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU
        VB_T       *    The var bind to attach the string to
        size_t          The length of the string
        OIDC_T     *    The string to attach

RETURNS: void
****************************************************************************/

void
  nextproc_next_instance(SNMP_PKT_T     *pktp,
                         VB_T           *vbp,
                         int             sz,
                         OIDC_T         *oidc)
{
int nlen, olen;
OIDC_T *noidc;
OBJ_ID_T *robj;

if (sz == 0)
    return;
robj = &vbp->vb_ml.ml_remaining_objid;
olen = vbp->vb_obj_id.num_components - robj->num_components;
nlen = olen + sz;
if (sz > robj->num_components) {
    if ((noidc = (OIDC_T *)SNMP_memory_alloc(nlen * sizeof(OIDC_T))) == 0) {
        pktp->pdu.std_pdu.error_status = GEN_ERR;
        vbp->vb_flags |= VFLAG_NEXT_DONE;
        return;
        }
    MEMCPY(noidc, vbp->vb_obj_id.component_list, olen * sizeof(OIDC_T));
    Clean_Obj_ID(&vbp->vb_obj_id);
    vbp->vb_obj_id.component_list = noidc;
    robj->component_list = noidc + olen;
    }

MEMCPY(robj->component_list, oidc, sz * sizeof(OIDC_T));
robj->num_components = sz;

vbp->vb_obj_id.num_components = nlen;
}

/****************************************************************************
NAME:  testproc_error

PURPOSE:  Install a test type error into the packet structure.

          As there is a different ordering of some errors
          in version 1 vs version 2 we try to handle that
          here.  The customer may try to overwrite the error
          status.  We allow for the overwrite if:
          packet type   current status  new status
          version 1     any value       no_such_name
                                        no_creation
          version 2     no_such_name    bad_value
                        no_creation     wrong_length
                                        wrong_value

          rfc1905 section 4.2.5
          step 6 is the check for WRONG_VALUE which maps to BAD_VALUE
          step 7 is the check for NO_CREATION which maps to NO_SUCH_NAME

          rfc1157 section 4.1.5
          step 1 is the view check which results in NO_SUCH_NAME
          step 2 is the check for BAD_VALUE
          
PARAMETERS:
        SNMP_PKT_T *    The decoded PDU
        VB_T       *    A var bind 
        sbits32_t       the error code

RETURNS: void
****************************************************************************/
void
  testproc_error(SNMP_PKT_T *pktp,
                 VB_T       *vbp,
                 sbits32_t   err_code)
{
/* mark the varbind as done */
vbp->vb_flags |= VFLAG_TEST_DONE;

if (pktp->pdu.std_pdu.error_status) {
    if (pktp->snmp_version == SNMP_VERSION_1) {
        if ((err_code != NO_CREATION) && (err_code != NO_SUCH_NAME))
            return;
        }
    else {
        if (((pktp->pdu.std_pdu.error_status != NO_SUCH_NAME) &&
             (pktp->pdu.std_pdu.error_status != NO_CREATION)) ||
            ((err_code != BAD_VALUE) && (err_code != WRONG_LENGTH) &&
             (err_code != WRONG_VALUE)))
            return;
        }
    }

pktp->pdu.std_pdu.error_status = err_code;
pktp->pdu.std_pdu.error_index  = vbp_to_index(pktp, vbp) + 1;
}

/****************************************************************************
NAME:  testproc_tproc_error

PURPOSE:  Routine to translate a tproc style error to a standard error
          and attach the result to the packet structure.  If error is 
          > 0, then we have a bad value error and we use error as the
          vbp index.  Otherwise we work out what the correct error is
          and use the given vb_t for the index (if necessary).
          
PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU
        VB_T       *    A var bind 
        int             the tproc style error

RETURNS: void
****************************************************************************/
void
  testproc_tproc_error(SNMP_PKT_T       *pktp,
                       VB_T             *vbp,
                       int               err_code)
{
INT_32_T ecode;

if (err_code > 0) {
    pktp->pdu.std_pdu.error_status = BAD_VALUE;
    pktp->pdu.std_pdu.error_index = (INT_32_T)err_code;
    vbp->vb_flags |= VFLAG_TEST_DONE;
    return;
    }

switch(err_code) {
    case TPROC_READ_ONLY:
    case TPROC_NOSUCH:
        ecode = NO_SUCH_NAME;
        break;
    default:
    case TPROC_GENERR:
        ecode = GEN_ERR;
        break;
    case TPROC_BADVALUE:
        ecode = BAD_VALUE;
        break;
    case TPROC_TOO_BIG:
        ecode = TOO_BIG;
        break;
    case TPROC_LAST_ERROR:
        /* don't need to do anything, they are already in the packet */
        vbp->vb_flags |= VFLAG_TEST_DONE;
        return;
/* For the following cases the return code is the negative of the */
/* true (snmp) error code so we set the status to be the          */
/* negative of the err_code.                                      */
    case TPROC_NO_ACCESS:
    case TPROC_NO_CREATION:
    case TPROC_WRONG_TYPE:
    case TPROC_WRONG_LENGTH: 
    case TPROC_WRONG_ENCODING:
    case TPROC_WRONG_VALUE:
    case TPROC_INCONSISTENT_VALUE:
    case TPROC_RESOURCE_UNAVAILABLE:
    case TPROC_COMMIT_FAILED:
    case TPROC_UNDO_FAILED:
    case TPROC_AUTHORIZATION_ERROR:
    case TPROC_NOT_WRITABLE:
    case TPROC_INCONSISTENT_NAME:
        ecode = (INT_32_T)(-err_code);
        break;
    }

pktp->pdu.std_pdu.error_status = ecode;
pktp->pdu.std_pdu.error_index  = vbp_to_index(pktp, vbp) + 1;
vbp->vb_flags |= VFLAG_TEST_DONE;
}
