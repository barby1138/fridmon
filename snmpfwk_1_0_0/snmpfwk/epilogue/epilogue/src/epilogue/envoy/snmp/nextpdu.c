/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/nextpdu.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: nextpdu.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:53  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/10/22 03:10:23  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.4  1997/10/16 00:45:56  sar
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
 * Revision 7.3  1997/03/20 06:49:02  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1996/03/25 15:10:34  sar
 * If we have a getbulk with no variables requested clean up
 * the packet and return an empty vb list.
 *
 * Revision 7.0  1996/03/18  20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.6  1996/01/15  19:45:11  sar
 * In the size check for getbulk, made updating the vblcount
 * be dependent on bufsize <= maxpkt rather than bufzsize < maxpkt.
 *
 * Revision 6.5  1995/11/14  22:44:42  sar
 * Renamed the silentdrops & proxydrops counters to match the mib def
 *
 * Revision 6.4  1995/11/02  01:55:22  sar
 * Moved an endif that was blocking out a }
 *
 * Revision 6.3  1995/11/01  00:46:48  sar
 * Added a default option in the version case statement to allow
 * new versions such as usec to be added without touching nextpdu
 *
 * Revision 6.2  1995/10/20  23:02:48  sar
 * rearranged the view code so we use the rfc1445 view scheme for both
 * v1 & v2 if INSTALL_ENVOY_SNMP_RFC1445_VIEWS is installed or the old
 * scheme if it isn't
 *
 * Revision 6.1  1995/06/05  20:58:44  sar
 * Added a call to clean_vb_data if the instance returned by a nextproc
 * isn't in our view.
 *
 * Revision 6.0  1995/05/31  21:47:35  sra
 * Release 6.0.
 *
 * Revision 1.3  1995/05/03  20:44:14  sar
 * Moved a variable declaration to be within an ifdefed block to avoid
 * compiler complaints when that block isn't being compiled.
 *
 * Revision 1.2  1995/04/18  21:13:44  sar
 * Added a check at the start of proxess_next_redo to handle a packet
 * that has already been tagged with an error.
 *
 * Revision 1.1  1995/03/21  00:09:23  sar
 * Initial revision
 *
 * 
 */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>
#include <objectid.h>
#include <view.h>
#include <snmpstat.h>

#include <bug.h>

/****************************************************************************
NAME:  SNMP_Process_Next_PDU

PURPOSE:  Process a next or bulk type pdu in an async fashion.  First we
          find possible objects for all the var binds (checking that the
          objects are in the view).  If any var binds would send us off the
          end of the mib  we flag a end of mib event.  For v1 this means a
          no such error, for v2 an exception.  After we have found all the
          objects we call the nextproc routines to find the value for
          each var bind.

PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU

RETURNS:  int   0 all of the nexts for this rep have been started successfully
                1 error of some sort, any error packet has been sent.
****************************************************************************/

int
  SNMP_Process_Next_PDU(SNMP_PKT_T *pktp)
{
VB_T *vbp, *newvbp, *oldvbp;
INT_32_T maxreps = 1, *err_stat;
int nonreps = 0, count, indx;

/* get some initial information */
if (((oldvbp = pktp->pdu.std_pdu.std_vbl.vblist) == 0) ||
    ((count = pktp->pdu.std_pdu.std_vbl.vbl_count) == 0))
  return(0);

/* We will be looking at the error status a lot so we get a local pointer
   to it to make things easier */
err_stat = &(pktp->pdu.std_pdu.error_status);

#if INSTALL_ENVOY_SNMP_VERSION_2
if (pktp->pdu_type == GET_BULK_REQUEST_PDU) {
    /* Test the packet and see if even an empty var bind list
       will casue a too big, if so we drop the packet */
        if (pktp->maxpkt < (25 + EBufferUsed(&pktp->community))) {
            SGRPv2_INC_COUNTER(snmp_stats.snmpSilentDrops);
            /* sar may need to add a different return here */
            pktp->error_complete(&pktp->pkt_src, &pktp->pkt_dst, 1,
                                 pktp->async_cookie);
            return(1);
            }

    /* Pull the maxreps and nonreps information from the packet and
       rationalize it. */
    maxreps = pktp->pdu.std_pdu.error_index;
    if (*err_stat < 0)
        nonreps = 0;
    else if (*err_stat >= count) {
        nonreps = count;
        maxreps = 1;
        }
    else
        nonreps = (int)(*err_stat & 0xFFFF);      

    if (maxreps < 1) {
        maxreps = 1;
        /* This is kinda wierd, the manager sent a bulk packet but doesn't
           want any of the repeaters, adjust the counters */
        if (nonreps == 0) {
            Clean_vb_list(&pktp->pdu.std_pdu.std_vbl);
            pktp->pdu.std_pdu.std_vbl.vblist = 0;
            pktp->pdu.std_pdu.std_vbl.vbl_count = 0;
            return(0);
            }
        count = nonreps;
        }
    }
#endif

/* Fill in the max_reps and non_reps fields.  If this is a bulk packet
   we pulled the information from the error fields. If it isn't
   a bulk packet we are using default values */
pktp->pdu.std_pdu.max_reps = maxreps;
pktp->pdu.std_pdu.non_reps = nonreps;

/* assume that everything will work */
*err_stat = NO_ERROR;
pktp->pdu.std_pdu.error_index = 0;

/* Some errors may require us to use the original list so we save the
   original and allocate space for a new one.  The objid for the new vbs
   will be filled in by the find_next_object routine, but we will need to
   clone the object id if find_next_object tells us their aren't any
   more objects. */
if ((newvbp = VarBindList_Allocate(count)) == 0) {
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return(1);
    }
pktp->pdu.std_pdu.saved_vbl.vbl_count = pktp->pdu.std_pdu.std_vbl.vbl_count;
pktp->pdu.std_pdu.saved_vbl.vblist = oldvbp;
pktp->pdu.std_pdu.std_vbl.vbl_count = count;
pktp->pdu.std_pdu.std_vbl.vblist = newvbp;

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Process_Next_PDU: infrastructure lock broken", 0));
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return(1);
    }
#endif

/* Attempt to find all the requested objects
   Note: if we find an error we set indx = count to break the loop */
for(indx = 0, vbp = newvbp; indx < count; indx++, vbp++, oldvbp++) {
    switch(find_next_object(vbp, &(oldvbp->vb_obj_id), pktp, 1)) {
        case 0:
            /* Nothing was found, generate an error for v1 or
               an exception for v2 */
            switch(pktp->snmp_version) {
#if INSTALL_ENVOY_SNMP_VERSION_1
                case SNMP_VERSION_1:
                    /* If version 1 we send a NO_SUCH_NAME error back.
                       We set up the packet for a nosuch name error. */
                    *err_stat = NO_SUCH_NAME;
                    pktp->pdu.std_pdu.error_index = SNMP_ERROR_INDEX(indx);
                    indx = count;
                    break;
#endif
#if INSTALL_ENVOY_SNMP_VERSION_2
                case SNMP_VERSION_2:
                default:
                    /* If not version 1 we send a END_OF_MIB exception back.
                       We set the data_flags_n_type of the current vbp
                       to END_OF_MIB, clone the object id and tag the
                       vbp as having been done and continue with the rest */
                    vbp->vb_data_flags_n_type = VT_ENDOFMIB;
                    vbp->vb_flags |= VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE;
                    if (clone_object_id(&oldvbp->vb_obj_id, &vbp->vb_obj_id)) {
                        /* We were unable to clone an object so we return
                           a gen err */
                        *err_stat = GEN_ERR;
                        indx = count;
                        }
                    break;
#endif
                }
            break;
        case -1:
            /* An internal error occurred send back a generr */
            *err_stat = GEN_ERR;
            indx = count;
            break;
        } /* switch(find_next_object...) */
    } /* for(indx = 0...) */

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
if (*err_stat) {
    ENVOY_Send_SNMP_Error_Packet(pktp, *err_stat,
                                 pktp->pdu.std_pdu.error_index);
    return(1);
    }

/* Now we loop through the vblist again and start all the nexts */
for(vbp = newvbp; count; count--, vbp++) {
    /* If the next started flag is set then either we set it above or
       the nextproc of some other var bind has claimed responsibility for
       this var bind.  In either case we skip this var bind and continue */
   if (vbp->vb_flags & (VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE))
        continue;
    ((vbp->vb_ml.ml_leaf)->nextproc)
      (vbp->vb_ml.ml_last_match, vbp->vb_ml.ml_remaining_objid.num_components,
       vbp->vb_ml.ml_remaining_objid.component_list, pktp, vbp);
    vbp->vb_flags |= VFLAG_NEXT_STARTED;

    /* test the error code, if we had an error then mark any remaining
       vbs as having been gotten (so we won't trip over them later).
       We need to skip the first vb as that is the one that
       caused the failure.  Then break because we are done */
    if (*err_stat) {
        for (vbp++, count--; count; count--, vbp++)
            if (!(vbp->vb_flags & VFLAG_NEXT_STARTED))
                vbp->vb_flags |= VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE;
        break;
        }
    }

return(0);
}

/****************************************************************************
NAME:  SNMP_Process_Next_Redo

PURPOSE:  This is the second stage of the next or bulk processing.
          We look through the pdu after all of the first set of nextprocs
          have finished.  For each pdu we have several possibilities
          the next proc may have found something which is in our
          view, it may have found something that isn't in our view
          (if we are using v2 views) or it may not have found anything.
          After we've looked over the entire list we do next procs for
          any vbs in case 2 or 3.

PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU

RETURNS:  int    0 this stage of processing is finished continue onward.
                -1 An (error) packet needs to be sent the caller should
                   do so and then free pktp.
                 1 Several nextprocs were restarted we need to redo this
                   stage
****************************************************************************/

int
  SNMP_Process_Next_Redo(SNMP_PKT_T *pktp)
{
VBL_T *vblp;
VB_T *vbp, *tvbp, *oldvbp;
int count, indx, eret, restart_flag = 0;
OBJ_ID_T target;
sbits32_t *err_stat;

/* If somebody has already set up an error, then we don't need to do
   anything here except return an error indication to our caller */
err_stat = &(pktp->pdu.std_pdu.error_status);
if (*err_stat != NO_ERROR)
    return(-1);

/* Get some initial information.  We need to determine which var bind list
   to use.  Nexts have only one but bulks may have several chained together.
   We want the last one as it will be the only one with variables that may
   need to be redone.  We also get a pointer to the object list as the
   base for this round.  We will need this pointer in the case of a v2
   end of mib exception as we are suppossed to send back the object we
   attempted a next from. */

vblp = &(pktp->pdu.std_pdu.std_vbl);
if (vblp->vblp == 0)
    oldvbp = pktp->pdu.std_pdu.saved_vbl.vblist;
else
    for(oldvbp = vblp->vblist + pktp->pdu.std_pdu.non_reps, vblp = vblp->vblp;
        vblp->vblp;
        oldvbp = vblp->vblist, vblp = vblp->vblp)
        ;
if (((vbp = vblp->vblist) == 0) || ((count = vblp->vbl_count) == 0))
    return(0);

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Process_Next_Redo: infrastructure lock broken", 0));
    *err_stat = GEN_ERR;
    return(-1);
    }
#endif

/* Note: if we find an error we set indx = count to break the loop */
for(tvbp = vbp, indx = 0; indx < count; tvbp++, indx++) {
    if (!(tvbp->vb_flags & VFLAG_NEXT_REDO)) {
#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
        /* If we are using the rfc1445 view scheme we may need to redo
           the view check now that we know the instance of the variable */
        if (((tvbp->vb_flags & VFLAG_NEXT_VCBITS) == VFLAG_NEXT_VCREQ) && 
            (SNMP_View_Check(pktp->view_index, tvbp->vb_obj_id.component_list,
                             tvbp->vb_obj_id.num_components, VIEW_CHECK_DET) !=
             VIEW_INCLUDED)) {
            /* Reset any flags that were set and free any space that was
               allocated by the method routines */
            tvbp->vb_flags &= ~VFLAG_NEXT_FLAGS | VFLAG_NEXT_VCREQ;
            Clean_vb_data(tvbp);
            restart_flag = 1;

#if INSTALL_ENVOY_AGENTX_MASTER
            /* Reset the include flag as we don't won't to get
               the current object, that's the one we just found
               was not in the view */
            tvbp->ax_flags &= ~ENVOY_AX_FLAGS_INCLUDE;
#endif
            }
        else
            tvbp->vb_flags |= VFLAG_NEXT_VCDONE;
#endif /* (INSTALL_ENVOY_SNMP_RFC1445_VIEWS) */
        continue;
        } /* if (!(tvbp->vb_flags & VFLAG_NEXT_REDO)) { */

    /* We need to redo the given vbp */

#if (INSTALL_ENVOY_EPI_MASTER)
    /* If the master agent is installed we have an extra flag,
       VFLAG_NEXT_INSTANCE to test.  It indicates that the var bind
       was for an instance type object in a sub agent and that we
       should redo the next with the current object instead of
       getting a new object */
    if (tvbp->vb_flags & VFLAG_NEXT_INSTANCE) {      
        tvbp->vb_flags &= ~(VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE |
                            VFLAG_NEXT_REDO | VFLAG_NEXT_INSTANCE);
        restart_flag = 1;
        continue;
        }
#endif /* (INSTALL_ENVOY_EPI_MASTER) */

#if INSTALL_ENVOY_AGENTX_MASTER
    /* We set the include flag in case the object we find is
       an agentx object.  The include flag will inform the sub
       agent that it should try a get of the object id before
       trying a next */
    tvbp->ax_flags |= ENVOY_AX_FLAGS_INCLUDE;
    

    /* if agentx is installed and this is an agentx leaf
       we use the boundary oid as the start of the next search.
       we need to free what is currently in the vb_obj_id
       and move the boundary info into the target oid */
    if (tvbp->vb_ml.ml_node->node_type & AGENTX_LEAF) {
        Clean_Obj_ID(&tvbp->vb_obj_id);
        target.num_components = tvbp->ax_search_end.num_components;
        target.component_list = tvbp->ax_search_end.component_list;
        tvbp->ax_search_end.num_components = 0;
        tvbp->ax_search_end.component_list = 0;

        tvbp->vb_ml.ml_leaf->ref_count--;
        if (tvbp->vb_ml.ml_leaf->ref_count == 0)
            SNMP_memory_free(tvbp->vb_ml.ml_leaf);
        tvbp->vb_ml.ml_leaf = 0;
        }
    else
#endif /* INSTALL_ENVOY_AGENTX_MASTER */

    {
    /* Redo is required, next proc was unable to find anything, go
       to next obj.  Chop off any instance qualifier and increment
       the last subid of the object id */
    target.num_components = tvbp->vb_obj_id.num_components -
                            tvbp->vb_ml.ml_remaining_objid.num_components;
    target.component_list = tvbp->vb_obj_id.component_list;
    tvbp->vb_obj_id.component_list = 0;
    tvbp->vb_obj_id.num_components = 0;

    while(1) {
        if (target.num_components == 0)
            break;

        target.component_list[target.num_components-1]++;
        if (target.component_list[target.num_components-1])
            break;
        target.num_components--;
        }
    }

    tvbp->vb_flags &= ~VFLAG_NEXT_FLAGS;

    if (target.num_components == 0)
        eret = 0;
    else
        eret = find_next_object(tvbp, &target, pktp, 1);

    Clean_Obj_ID(&target);

    switch(eret) {
        case 0:
            switch(pktp->snmp_version) {
#if (INSTALL_ENVOY_SNMP_VERSION_1)
                case SNMP_VERSION_1:
                    /* For version 1 we send back a NO_SUCH_NAME error */
                    *err_stat = NO_SUCH_NAME;
                    pktp->pdu.std_pdu.error_index = SNMP_ERROR_INDEX(indx);
                    indx = count;
                    break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_1) */
#if (INSTALL_ENVOY_SNMP_VERSION_2)
                case SNMP_VERSION_2:
                default:
                    /* For version 2 we send back an END_OF_MIB exception.
                       We set the data_flags_n_type of the current vbp to
                       END_OF_MIB, clone the object id and tag the vbp as
                       having been done and continue with the rest */
                    tvbp->vb_data_flags_n_type = VT_ENDOFMIB;
                    tvbp->vb_flags |= VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE;
                    if (clone_object_id((&(oldvbp + indx)->vb_obj_id),
                                        &tvbp->vb_obj_id)) {
                        /* We were unable to clone an object so we return
                           a gen err */
                        *err_stat = GEN_ERR;
                        indx = count;
                        }
                    break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2) */
                } /* switch(pktp->snmp_version) { */
            break;
        case -1:
            /* An internal error occurred send back a generr */
            Clean_Obj_ID(&target);
            *err_stat = GEN_ERR;
            indx = count;
            break;
        default:
            restart_flag = 1;
            break;     
        } /* switch(eret) { */
    } /* for(tvbp = vbp, indx = 0; indx < count; tvbp++, indx++) { */

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
if (*err_stat)
    return(-1);

/* If necessary we loop through the vblist again and start all the nexts */
if (restart_flag) {
    for(indx = 0, tvbp = vbp; indx < count; indx++, tvbp++) {
        /* If the next started flag is set then either we set it above or
           the nextproc of some other var bind has claimed responsibility for
           this var bind.  In either case we skip this var bind and continue */
        if (tvbp->vb_flags & (VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE))
            continue;
        ((tvbp->vb_ml.ml_leaf)->nextproc)
          (tvbp->vb_ml.ml_last_match,
           tvbp->vb_ml.ml_remaining_objid.num_components,
           tvbp->vb_ml.ml_remaining_objid.component_list, pktp, tvbp);
        tvbp->vb_flags |= VFLAG_NEXT_STARTED;
        }
    return(1);
    }
return(0);
}

#if (INSTALL_ENVOY_SNMP_VERSION_2)
/****************************************************************************
NAME:  SNMP_Process_Bulk_Redo

PURPOSE:  This is the third stage of bulk processing.  To get here
          we have successfully finished getting information about one
          repetition's objects.  We do a size check to determine if
          we need to remove some of the objects.  If we have enough space
          we check to see if we want to try another round 
          (if maxreps > 1 && some variable doesn't have an end of mib
          exception)

PARAMETERS:
        SNMP_PKT_T *    The decoded NEXT/BULK PDU

RETURNS:  int    0 The caller should send and free the packet
                 1 Several nextprocs were started we need to redo this
                   stage
****************************************************************************/

int
  SNMP_Process_Bulk_Redo(SNMP_PKT_T *pktp)
{
VBL_T *vblp;
VB_T *vbp, *tvbp, *newvbp;
int count, indx, nonreps, restart_flag = 0;
ALENGTH_T bufsize;
sbits32_t *err_stat;

/* Find the correct vblist to work with.  Bulks may have several vblists
   chained togther, we want the last one (the only one that may have ops
   outstanding).  The other pdus only have one vblist */
for(vblp = &(pktp->pdu.std_pdu.std_vbl); vblp->vblp; vblp = vblp->vblp)
    ;
if (((vbp = vblp->vblist) == 0) || ((count = vblp->vbl_count) == 0))
    return(0);
if (vblp == &(pktp->pdu.std_pdu.std_vbl))
    nonreps = pktp->pdu.std_pdu.non_reps;
else
    nonreps = 0;

/* Do the size check, if the packet would exceed the maximum 
   start removing vbs until the packet is smaller than the max.
   At that point we are done and can return 0 */
bufsize = SNMP_Bufsize_For_Packet(pktp);
if (bufsize > pktp->maxpkt) {
    for(indx = count, tvbp = vbp + indx - 1; indx; indx--, tvbp--) {
        Clean_vb(tvbp);
        /* Recalculate the bufsize, 2 is for the tag and length fields,
           the length field may be greater than 1 but that is ok, the only
           thing that will happen is we may put less than the maximum in
           a buffer */
        bufsize -= 2 + tvbp->vb_seq_size;
        if (bufsize <= pktp->maxpkt) {
            vblp->vbl_count = indx - 1;
            return(0);
            }
        }
    return(0);
    }

/* Determine if we want to try another round of nexts */
if (pktp->pdu.std_pdu.max_reps <= 1)
    return(0);
for(indx = count - nonreps, tvbp = vbp + nonreps; indx; indx--, tvbp++)
    if (tvbp->vb_data_flags_n_type != VT_ENDOFMIB)
        break;
if (indx == 0)
    return(0);

/* set up the err_stat as we shall use it several times */
err_stat = &(pktp->pdu.std_pdu.error_status);

/* So we want to try another round, start by decrementing the reps number
   then set up the next vblist in the chain */
pktp->pdu.std_pdu.max_reps--;
  
if ((vblp->vblp = VBList_Allocate(count - nonreps)) == 0) {
    *err_stat = GEN_ERR;
    return(0);
    }

/* Attempt to find the correct object for each of the repeaters.  Currently
   we copy all of the objs and then start any required nexts.  Eventually
   we should copy objects tagged as ENDOFMIB (we aren't going to find anything
   new) and tabular objects.  We will run find_next_object on scalar objects
   as there won't be anymore instances. */

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Process_Bulk_Redo: infrastructure lock broken", 0));
    *err_stat = GEN_ERR;
    return(0);
    }
#endif

/* Note: if we find an error we set indx = 0 to break the loop */
for(indx = count - nonreps, tvbp = vbp + nonreps, newvbp = vblp->vblp->vblist;
    indx > 0;
    indx--, tvbp++, newvbp++) {
    newvbp->vb_flags = tvbp->vb_flags & VFLAG_NEXT_VCREQ;
    if (clone_object_id((&tvbp->vb_obj_id), (&newvbp->vb_obj_id))) {
        /* We were unable to clone an object so we return a gen err */      
        *err_stat = GEN_ERR;
        indx = 0;
        }

    if (tvbp->vb_data_flags_n_type == VT_ENDOFMIB) {
        newvbp->vb_data_flags_n_type = VT_ENDOFMIB;
        newvbp->vb_flags |= VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE;
        }
    else {
        restart_flag = 1;
        /* And copy the mib leaf information */
        newvbp->vb_ml.ml_node = tvbp->vb_ml.ml_node;
        newvbp->vb_ml.ml_flags = ML_IS_LEAF;
        newvbp->vb_ml.ml_last_match = tvbp->vb_ml.ml_last_match;
        newvbp->vb_ml.ml_remaining_objid.num_components =
            tvbp->vb_ml.ml_remaining_objid.num_components;
        newvbp->vb_ml.ml_remaining_objid.component_list =
            newvbp->vb_obj_id.component_list +
            newvbp->vb_obj_id.num_components - 
            newvbp->vb_ml.ml_remaining_objid.num_components;

#if INSTALL_ENVOY_AGENTX_MASTER
        /* if agentx is installed we need to move the boundary object id */
        if (tvbp->vb_ml.ml_node->node_type & AGENTX_LEAF) {
            newvbp->ax_search_end.num_components = 
              tvbp->ax_search_end.num_components;
            newvbp->ax_search_end.component_list =
              tvbp->ax_search_end.component_list;

            tvbp->ax_search_end.num_components = 0;
            tvbp->ax_search_end.component_list = 0;

            /* if possible update the reference counts */
            if (tvbp->vb_ml.ml_leaf->ref_count == AX_MAX_REF_COUNT) {
                *err_stat = GEN_ERR;
                indx = 0;
                }
            tvbp->vb_ml.ml_leaf->ref_count++;
            }
#endif /* INSTALL_ENVOY_AGENTX_MASTER */
        }
    }

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
if (*err_stat)
    return(0);

if (restart_flag) {
    for(indx = count - nonreps, newvbp = vblp->vblp->vblist;
        indx;
        indx--, newvbp++) {
        /* If the next started flag is set then either we set it above or
           the nextproc of some other var bind has claimed responsibility for
           this var bind.  In either case we skip this var bind and continue */
        if (newvbp->vb_flags & (VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE))
            continue;
        ((newvbp->vb_ml.ml_leaf)->nextproc)
          (newvbp->vb_ml.ml_last_match,
           newvbp->vb_ml.ml_remaining_objid.num_components,
           newvbp->vb_ml.ml_remaining_objid.component_list, pktp, newvbp);
        newvbp->vb_flags |= VFLAG_NEXT_STARTED;
        }
    return(1);
    }
else
    return(0);
}
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_2) */
