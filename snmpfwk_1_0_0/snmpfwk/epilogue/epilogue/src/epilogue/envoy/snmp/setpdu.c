/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/setpdu.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: setpdu.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:02  sar
 * Updated revision to 8.0
 *
 * Revision 7.6  1997/11/01 01:25:38  sar
 * In the packet we are processing save a pointer to the coarse lock we
 * are using so that we can free it later without needing to know which
 * lock it was.
 *
 * Revision 7.5  1997/10/23 06:19:47  sar
 * changed an error status code to allow us to get the snmp error index
 * correct if the first vbp has an error
 *
 * Revision 7.4  1997/10/22 03:10:26  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.3  1997/10/16 00:46:05  sar
 * Updated for agentx support, with most of the work in mibutils.c to deal
 * with interior leaves and the pdu processors to deal with agentx leaves,
 * especially their reference counts.
 *
 * Add a bug macro into the get_{read write}_lock calls to give the user
 * a chance if things start going wrong.
 *
 * Fixed a potential problem int the proxy code where we were trying to
 * stuff a 32 bit quantity into a pointer which may or may not work depending
 * on the platform.
 *
 * Revision 7.2  1997/03/20 06:49:17  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.3  1995/11/11  00:07:30  sar
 * added SNMP_user_set_failed as a user exit that will be called if,
 * after all the tests return, some of the tests have failed or after
 * all the undos return if a set fails.
 *
 * Revision 6.2  1995/11/01  00:46:48  sar
 * changed LOCCK to LOCK
 * modified the version test to process unknown versions as v2, allowing
 * new versions such as usec to be added without modifing setpdu
 *
 * Revision 6.1  1995/10/20  23:02:48  sar
 * removed reacquiring the parties
 * tiddied up the error tests
 *
 * Revision 6.0  1995/05/31  21:48:07  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/02  23:34:50  sar
 * Removed the no_creation code as the new v2 error sequence doesn't need it.
 *
 * Revision 5.3  1995/04/18  21:13:44  sar
 * use {for loc}_party_oid instead of {for loc}_oid.
 *
 * Revision 5.2  1995/03/21  00:30:26  sar
 * Basically rewritten to use the new async apis.
 *
 * Revision 5.1  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.6  1994/05/11  00:21:12  sar
 * Added a test to see if the object is writable in the v1 test sequence.
 *
 * Revision 4.5  1994/05/04  22:45:43  sar
 * Added code to allow a failure during the setproc, this shouldn't be
 * used in general.  Required changing the calling sequence to call_the_set
 * _routine. The only error returned is undo_failed.
 *
 * Revision 4.4  1994/04/29  21:15:53  sar
 * Added ifdefs for use with sub agent scheme, as well as updating find_object
 * _node to have another argument.  Modifed the view check system and
 * added code to reacquire parties after sets.
 *
 * Revision 4.3  1994/03/08  18:00:52  dab
 * Clear error_status and error_index in response packet.
 *
 * Revision 4.2  1993/10/06  19:50:40  sar
 * Upgraded error_response, it now handles incrementing counters and choosing
 * which error code to return.  It is called with a TPROC style error and
 * it checks for v1 or v2 and sends the appropriate error.
 *
 * Revision 4.1  1993/09/30  20:19:13  sar
 * Added code to test the size of the potential outgoing packet and to drop
 * the packet if necessary.
 * Rearranged the error tests and added some code to get the proper set
 * of errors for v2.
 * This includes a potential change to the testproc routines to understand
 * the TEST_CREATE flag and be able to test even if other errors may exist.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.11  1993/06/17  23:33:44  sar
 * removed the ENVOY_ from the UI64 routines.
 *
 * Revision 3.10  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.9  1993/04/30  22:57:32  sar
 * made the definition of call_the_set_routine use __() macro.
 *
 * Revision 3.8  1993/04/28  22:02:29  sar
 * moved the def of UINT_64_T and added some new types to the routines.
 *
 * Revision 3.7  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.6  1993/04/20  19:53:27  sar
 * I had removed the read-only check during sets (for v2) so I put it back
 * and cleaned up the code around it, also gave mibuitls another error return
 * so we can coose between no access and no such.
 *
 * Revision 3.5  1993/04/17  01:08:43  sar
 * added a new type of error to the error response, this allows another routine
 * to have set the error index and status fields.
 *
 * Revision 3.4  1993/03/20  00:25:43  sar
 * Updated some types for version two party type stuff (indicies, local
 * storage, status, mtu and privileges).
 * Also removed some unused break statements.
 *
 * Revision 3.3  1993/03/17  23:02:47  sar
 * added the view structure and the routines to use and manipulate it.
 * in mibutils and setpdu started using views.
 *
 * Revision 3.2  1993/02/26  22:37:51  sar
 * Added check for new return codes from the test procs.
 *
 * Revision 3.1  1993/02/17  21:09:52  sar
 * Removed the #define then and use of it.
 * Updated stats to deal with v1 and v2 (or both or neither).
 * Added data type for VT_UINTEGER32
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.107  91/11/07  17:15:28  dab
 * Return from testproc is 1 based not 0 based.
 * 
 * Revision 2.106  91/11/07  09:33:00  dab
 * Testproc can now return with a positive result which means report a
 * badValue error with an index of the returned result (1 based).
 * 
 * Revision 2.105  91/10/30  20:42:09  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.104  91/09/18  12:32:34  dab
 * Updated to use new macros from <asn1conf.h> and <snmpconf.h>.
 * 
 * Revision 2.103  91/08/15  12:31:09  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.102  91/08/14  10:47:28  dab
 * Added trace functions that Karl put in as macros.
 * 
 * Revision 2.101  91/08/12  12:31:57  dab
 * Changed local variables index to vb_index to avoid shadowing
 * common library routine index().
 * 
 * Revision 2.100  91/08/09  14:08:25  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:50  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.4   09 Aug 1990 13:40:36
 * Added -3 as a new potential return from testproc.  This allows a badValue
 * error to be returned.
 * Also fixed a misleading comment on an endif.
 * 
 *    Rev 2.3   22 Jun 1990 22:04:36
 * Removed a call to LEAVE_CRITICAL_SECTION which was misplaced during
 * a recent reorganization of the code.
 * 
 *    Rev 2.2   06 Jun 1990 23:58:36
 * Corrected a misleading comment.
 * 
 *    Rev 2.1   30 May 1990 11:32:52
 * Fixed a comment to properly describe the range of error return values
 * from testproc().
 * 
 *    Rev 2.0   31 Mar 1990 15:06:54
 * Release 2.00
 * 
 *    Rev 1.8   27 Apr 1989 15:56:06
 * Removed unused variables
 * 
 *    Rev 1.7   17 Mar 1989 21:41:30
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.6   11 Jan 1989 12:41:06
 * Added critical section macros.
 * 
 *    Rev 1.5   20 Sep 1988 18:35:32
 * Made note of the fact that under RFC1067, an attempt to set a read-only
 * variable should cause a noSuchName error rather than a readOnly error.
 * 
 *    Rev 1.4   19 Sep 1988 17:26:48
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.3   19 Sep 1988  9:24:00
 * Reversed change made in revision 1.2
 * 
 *    Rev 1.2   15 Sep 1988 20:04:08
 * Recalculated size of outgoing packet before transmission.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:20
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:47:06
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>

#include <bug.h>

/****************************************************************************
NAME:  SNMP_Process_Test_PDU

PURPOSE:  Process a set type pdu in an async fashion.  First we find objects
          for all the var binds (checking that the objects are in the view).
          If any var binds don't point to a valid object we flag a no such
          error.  After we have found all the objects we call the testproc
          routines to determine if the var binds may be set.

PARAMETERS:
        SNMP_PKT_T *    The decoded SET PDU

RETURNS:  int   0 all of the tests have been started successfully
                1 error of some sort, any error packet has been sent.
****************************************************************************/

int
  SNMP_Process_Test_PDU(SNMP_PKT_T *pktp)
{
VB_T *vbp;
int   count, indx, eret;
sbits32_t *err_stat;

#if INSTALL_ENVOY_SNMP_LOCK
/* if locks are installed we need to return the read lock and get
   a write lock */
if (pktp->lockflags != LOCK_READ) {
    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                         pktp->async_cookie);
    return(1);
    }
ENVOY_SNMP_RELEASE_READ_LOCK((*pktp->coarse_lock));
pktp->lockflags = LOCK_NOLOCK;
if (ENVOY_SNMP_GET_WRITE_LOCK((*pktp->coarse_lock))) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Process_Test_PDU: coarse lock is broken", 0));

    pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                         pktp->async_cookie);
    return(1);
    }
pktp->lockflags = LOCK_WRITE;

#endif /* INSTALL_ENVOY_SNMP_LOCK */

/* There is no need to really do much work on the PDU because   */
/* the returned form is almost exactly the received form.       */

/* Assume that things are going to work */
pktp->pdu.std_pdu.error_status = NO_ERROR;
pktp->pdu.std_pdu.error_index = 0;

/* Also, no check will be made for oversize response if this is */
/* version 1, because of the simularity: a no-error response    */
/* will be exactly the  same size as the GET REQUEST, an error  */
/* response may be a bit larger (perhaps by a byte or two), but */
/* we are forced to send it.  For Version 2 on the other hand we*/
/* need to check the size due to swapping the source and dest   */
/* parties and because the max packet sizes may be different.   */

#if INSTALL_ENVOY_SNMP_VERSION_2
if (pktp->snmp_version != SNMP_VERSION_1) {
    /* see if a maximum packet is small enough to fit in the
       given max size.  The initial sequence can expand by
       at most 2, the max error status is the same as the normal 
       one, and the max index can expand by at msot 2 so
       we do bufsize + 4 */
    if ((SNMP_Bufsize_For_Packet(pktp) + 4) > pktp->maxpkt) {
        ENVOY_Send_SNMP_Error_Packet(pktp, TOO_BIG, 0);
        return(1);
        }
    }
#endif /* INSTALL_ENVOY_SNMP_VERSION_2 */

/* get some initial information */
if (((vbp = pktp->pdu.std_pdu.std_vbl.vblist) == 0) ||
    ((count = pktp->pdu.std_pdu.std_vbl.vbl_count) == 0))
  return(0);

/* We will be looking at the error status a lot so we get a local pointer
   to it to make things easier */
err_stat = &(pktp->pdu.std_pdu.error_status);

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Process_Test_PDU: infrasturctue lock is broken", 0));

    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return(1);
    }
#endif

for(indx = 0; indx < count; indx++,vbp++) {
    /* Check whether the varBind is in the view */
    eret = find_object_node(vbp, pktp, 1);
    if (eret) {
        switch(eret) {
            case -1:
            default:
                *err_stat = NOT_WRITABLE;
                break;
            case -2:
                *err_stat = NO_ACCESS;
                break;
            case -3:
                *err_stat = GEN_ERR;
                indx = -1;
                break;
            }
        break;
        }

    /* Check the returned node, if it isn't a leaf it isn't
       modifiable */
    if (!(vbp->vb_ml.ml_flags & ML_IS_LEAF)) {
        *err_stat = NOT_WRITABLE;
        break;
        }

    /* second view check, this is necessary only if
       we aren't using the rfc1445 view scheme */
#if (!(INSTALL_ENVOY_SNMP_RFC1445_VIEWS))
    if (((vbp->vb_ml.ml_leaf)->write_mask & pktp->mib_view) == 0) {
        *err_stat = NO_ACCESS;
        break;
        }
#endif /* (!(INSTALL_ENVOY_SNMP_RFC1445_VIEWS)) */

    /* more checks on the node, see if the nodes access allows for writes */
    if (!((vbp->vb_ml.ml_leaf)->access_type & WRITE_ACCESS)) {
        *err_stat = NOT_WRITABLE;
        break;
        }
    /* Finally see if the vb's data type matches the node's type */
#if (INSTALL_ENVOY_AGENTX_MASTER)
    if ((vbp->vb_ml.ml_node)->node_type & AGENTX_LEAF)
        continue;
#endif

    if (vbp->vb_data_flags_n_type != (vbp->vb_ml.ml_leaf)->expected_tag) {
        *err_stat = WRONG_TYPE;
        break;
        }
    }

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);

if (*err_stat) {
    if (indx != -1)
        ENVOY_Send_SNMP_Error_Packet(pktp, *err_stat, SNMP_ERROR_INDEX(indx));
    else
        ENVOY_Send_SNMP_Error_Packet(pktp, *err_stat, 0);
    return(1);
    }

/* we have now found all of the requested objects and have passed all of
   the mib class tests, we now start up all of the tests and then return
   to await the responses */

/* Here we give the user a chance to examine the packet and exert control
   They have three options:
   Return -1 to indicate an inconsistent pdu, an error will be gennerated
   Return 0 to inidcate that normal processing should proceed, the user
        must not perform any sets though it may perfrom tests.
   Reurn 1 to indicate that it has performed all necessary processing.  */
#if defined(SNMP_validate_set_pdu)
switch(SNMP_validate_set_pdu(pktp)) {
    case -1:
        ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
        return(1);
    case 0:
        break;
    case 1:
    default:
        ENVOY_Send_SNMP_Packet(pktp);
        return(1);
    }
#endif

for(vbp = pktp->pdu.std_pdu.std_vbl.vblist; count; count--, vbp++) {
    /* If the test started flag is set then either we set it above or
       the testproc of some other var bind has claimed responsibility for
       this var bind.  In either case we skip this var bind and continue */
    if (vbp->vb_flags & (VFLAG_TEST_STARTED | VFLAG_TEST_DONE))
        continue;      
    ((vbp->vb_ml.ml_leaf)->testproc)
      (vbp->vb_ml.ml_last_match, vbp->vb_ml.ml_remaining_objid.num_components,
       vbp->vb_ml.ml_remaining_objid.component_list, pktp, vbp);
    vbp->vb_flags |= VFLAG_TEST_STARTED;

    /* test the error code, if we had an error then mark any remaining
       vbs as having been tested (so we won't trip over them later).
       We need to skip the first vb as that is the one that
       caused the failure.  Then break because we are done */
    if (*err_stat) {
        for (vbp++, count--; count; count--, vbp++)
            if (!(vbp->vb_flags & VFLAG_TEST_STARTED))
                vbp->vb_flags |= VFLAG_TEST_STARTED | VFLAG_TEST_DONE;
        break;
        }
    }


return(0);
}

/****************************************************************************
NAME:  SNMP_Process_Set_PDU

PURPOSE:  This is the second stage of the set processing.  At this
          point all of one set of operations have completed and it's 
          time to survey are handiwork and decide what we need to do
          next.  The phase word from the packet will indicate what phase
          of the set we are performing.
          If we have just finished the tests we either found an error
          and send an error response or we start the sets.
          If we have just finsihed the sets we either found an error
          and start the undos or we send a good response.
          If we have just finsihed the undos we send an error response.

PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU

RETURNS:  int    0 The caller should send and free the packet
                 1 This stage is finished but more stages are required
****************************************************************************/

int
  SNMP_Process_Set_PDU(SNMP_PKT_T *pktp)
{
VB_T *vbp, *tvbp;
int count, tcount;
sbits32_t *err_stat;

if (((vbp = pktp->pdu.std_pdu.std_vbl.vblist) == 0) ||
    ((count = pktp->pdu.std_pdu.std_vbl.vbl_count) == 0))
    return(0);

/* We will be looking at the error status a lot so we get a local pointer
   to it to make things easier */
err_stat = &(pktp->pdu.std_pdu.error_status);

/* The phase word specifies whice phase we are involved with, note
   that there aren't breaks between the cases.  This is intentional.
   This routine is essentially linear code with multiple entry points
   selected by the phase word.  The phase word allows us to wait
   for operations to compelete and then to re-enter the code efficiently */

switch(pktp->phase) {
    /* if phase is 0, we are waiting for the tests to finish
       we see if they have finished.  If so we test for errors
       and bail out if one occurred.  Otherwise we start up the
       sets.  If we detect an error while starting a test we mark
       any remaining objects as having been undone then set the phase
       word to indicate the undo phase and let the undo case
       deal with starting any undos required. */
    case 0:
        for(tvbp = vbp, tcount = count; tcount; tcount--, tvbp++)
            if (!(tvbp->vb_flags & VFLAG_TEST_DONE))
                return(1);

        /* If we failed a test send an error packet */
        if (*err_stat) {
            /* Here we tell the user that one of tests
               failed, this gives them a chance to clean
               up, perhaps after validate_set_pdu */
#if defined(SNMP_user_set_failed)
            SNMP_user_set_failed(pktp);
#endif
            return(0);
            }

        /* The user gets another chance to examine the packet and exert
           control.  They again have three options:
           -1 indicates a bad pdu, an error will be generated
            0 inidcates normal processing should proceed, the user may
              start or perfrom any sets they desire.  But they better
              indicate what they have done.  If the user has started some
              sets and one of them fails they may either wait until they
              all finish and return a -1 or 1, or they way set the flag
              bits for the other vbs to be already set & undone and then
              let this code wait for the remaining tests to finish.  They
              also have to set the err status correctly.
            1 indicates the user has performed all necessary processing.  */
#if defined(SNMP_user_pre_set)
        switch(SNMP_user_pre_set(pktp)) {
            case -1:
                *err_stat = GEN_ERR;
                return(0);
            case 0:
                break;
            case 1:
            default:
                return(0);
            }
#endif


        /* start the sets */
        for(tvbp = vbp, tcount = count; tcount; tcount--, tvbp++) {
            if (tvbp->vb_flags & (VFLAG_SET_STARTED | VFLAG_SET_DONE))
                continue;      
            ((tvbp->vb_ml.ml_leaf)->setproc)
              (tvbp->vb_ml.ml_last_match,
               tvbp->vb_ml.ml_remaining_objid.num_components,
               tvbp->vb_ml.ml_remaining_objid.component_list,
               pktp, tvbp);
            tvbp->vb_flags |= VFLAG_SET_STARTED;

            /* test the error code, if we had an error then mark any remaining
               vbs as having been undone (as they were never done to begin
               with).  We need to skip the first vb as that is the one that
               caused the failure.  Then break because we are done */
            if (*err_stat) {
                for (tvbp++, tcount--; tcount; tcount--, tvbp++)
                    if (!(tvbp->vb_flags & VFLAG_SET_STARTED))
                        tvbp->vb_flags |= VFLAG_UNDO_STARTED | VFLAG_UNDO_DONE;
                *err_stat = COMMIT_FAILED;
                pktp->phase = VFLAG_UNDO_STARTED;
                goto START_UNDO;
                }
            }

        pktp->phase = VFLAG_SET_STARTED;

    case VFLAG_SET_STARTED:
        /* If we didn't have any errors, we test and see if all the
           sets finished if so we are done and can request a response
           packet be sent otherwise there are still sets outstanding
           and we indicated that we need to wait some more */
        if (*err_stat == NO_ERROR) {
            for(tvbp = vbp, tcount = count; tcount; tcount--, tvbp++)
                if (!(tvbp->vb_flags & VFLAG_SET_DONE))
                    return(1);

#if defined(SNMP_user_post_set)
            /* The user gets another chance to examine the packet and exert
               control. */
            SNMP_user_post_set(pktp);
#endif
            return(0);
            }

        /* A set routine indicated an error, so we change the phase to
           undo_started and let the code for that case handle starting
           up the required undos.  */
        pktp->phase = VFLAG_UNDO_STARTED;
        *err_stat = COMMIT_FAILED;

START_UNDO:
    case VFLAG_UNDO_STARTED:
        for(tvbp = vbp, tcount = count; tcount; tcount--, tvbp++) {
            switch(tvbp->vb_flags & (VFLAG_SET_DONE | VFLAG_UNDO_BOTH)) {
                case 0:
                    /* The set hasn't finished so it doesn't require
                       an undo, but if it has one we start it up */
                    if (tvbp->undoproc) {
                        (tvbp->undoproc)
                          (tvbp->vb_ml.ml_last_match,
                           tvbp->vb_ml.ml_remaining_objid.num_components,
                           tvbp->vb_ml.ml_remaining_objid.component_list,
                           pktp, tvbp);
                        tvbp->vb_flags |= VFLAG_UNDO_STARTED;
                        }
                    break;

                case VFLAG_SET_DONE:
                    /* As the set has finished an undo is required,
                       if one doesn't exist we flag an undo_failed error
                       and set the undo_done flag */
                    if (tvbp->undoproc) {
                        (tvbp->undoproc)
                          (tvbp->vb_ml.ml_last_match,
                           tvbp->vb_ml.ml_remaining_objid.num_components,
                           tvbp->vb_ml.ml_remaining_objid.component_list,
                           pktp, tvbp);
                        tvbp->vb_flags |= VFLAG_UNDO_STARTED;
                        }
                    else {
                        *err_stat = UNDO_FAILED;
                        tvbp->vb_flags |= VFLAG_UNDO_DONE;
                        }
                    break;
                } /* switch (vbp->flags ... ) */
            } /* for(tvbp = vbp, ... ) */

    case VFLAG_UNDO_DONE:
        /* Test to see if all of the undos have finished yet */
        for(tvbp = vbp, tcount = count; tcount; tcount--, tvbp++)
            if (!(tvbp->vb_flags & VFLAG_UNDO_DONE))
                return(1);
        /* Here we tell the user that one of sets
           failed, this gives them a chance to clean
           up, perhaps after user_pre_set */
#if defined(SNMP_user_set_failed)
        SNMP_user_set_failed(pktp);
#endif
        return(0);
    } /* switch(pktp->phase) */

/* We should never get here, all of the options for the phase word should
   be covered in the above switch */
return(0);
}
