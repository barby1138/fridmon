/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/sa_sagt.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 *  Copyright 1994-1997 Epilogue Technology Corporation.
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
 * $Log: sa_sagt.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:58  sar
 * Updated revision to 8.0
 *
 * Revision 7.7  1997/11/01 01:25:37  sar
 * In the packet we are processing save a pointer to the coarse lock we
 * are using so that we can free it later without needing to know which
 * lock it was.
 *
 * Revision 7.6  1997/10/27 21:08:53  sar
 * Moved the freeing of privlist to make sure it was freed even if we
 * had some errors.
 *
 * Revision 7.5  1997/10/22 03:10:26  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.4  1997/10/16 23:49:56  sar
 * If there is an error while performing a set mark any remaining variables
 * as having their sets started and done to keep the continue routine happy.
 *
 * Revision 7.3  1997/10/16 00:46:04  sar
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
 * Revision 7.2  1997/03/20 06:49:15  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.3  1995/11/16  20:36:42  sar
 * Added an init of demuxer.object to avoid accidents with uninit data.
 *
 * Revision 6.2  1995/11/01  00:46:48  sar
 * removed the snmpv2 ifdefs for localtime object ids
 *
 * Revision 6.1  1995/06/05  21:00:33  sar
 * Made the timeout period between testprocs and setprocs configurable
 * via ENVOY_SA_SET_TIMEOUT.
 *
 * Revision 6.0  1995/05/31  21:48:03  sra
 * Release 6.0.
 *
 * Revision 5.11  1995/05/29  03:26:49  sar
 * Corrected the check of the vb_flags in sa_handler_continue.
 *
 * Revision 5.10  1995/05/23  23:26:39  sar
 * Modified the continue_reentrant code, the continue routine expects
 * it's caller to have gottent the lock and will release it.
 *
 * Revision 5.9  1995/05/22  19:43:54  sar
 * Probhibit processing writes while reads are outstanding
 * Rearrange the use of setpdu so that we don't have a race condition
 * between the timer code and the main line code.
 * Added continue_reentrant code.
 *
 * Revision 5.8  1995/04/18  21:13:44  sar
 * Twiddled some type casting to make compilers happy.
 *
 * Revision 5.7  1995/03/24  16:32:04  sar
 * Added code to set a timer to cleanup the saved packet.
 *
 * Revision 5.6  1995/03/21  00:30:26  sar
 * Added async cabability.  Also cleaned up the packet saving routines.
 * We now can have only one outstanding set request at a time, and do not
 * try to keep state for gets or nexts at all.
 *
 * Revision 5.5  1994/10/28  19:45:57  sar
 * Tightened up the use and clean up of vb priv stuff.
 *
 * Revision 5.4  1994/10/20  22:28:00  sar
 * typecasting stuff to make compilers happy.
 *
 * Revision 5.3  1994/10/13  19:31:31  sar
 * Moved the line to set the tested bit to be after the call to the
 * test routines so that the extract_row routines will work.
 * Moved the break statement in the testset block to break if the testproc
 * fails, we were breaking on any test.
 *
 * Revision 5.2  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.1  1994/08/04  22:04:16  sar
 * Added multiple mib tree capability
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 1.4  1994/05/13  15:32:03  sar
 * Added some type casts to make the compilers happy.
 *
 * Revision 1.3  1994/05/12  17:16:29  sar
 * added snmp_version to the message header
 *
 * Revision 1.2  1994/05/10  20:16:03  sar
 * Removed party.h from the include list
 *
 * Revision 1.1  1994/05/09  21:03:44  sar
 * Initial revision
 *
 * Revision 1.2  1994/05/06  20:31:27  sar
 * Modified use of demux to use both an object and a string all the time.
 * added some cleaning to try to avoid leaking memory.
 *
 * Revision 1.1  1994/05/05  19:59:41  sar
 * Initial revision
 * */

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <encode.h>
#include <decode.h>
#include <snmpdefs.h>
#include <subagent.h>
#include <buildpkt.h>
#include <objectid.h>
#include <etimer.h>

#include <bug.h>

#if (INSTALL_ENVOY_EPI_SUB)

#define CLEAN_DEMUX(D) \
  (EBufferClean(&((D)->string)), Clean_Obj_ID(&((D)->object)))

#ifndef ENVOY_SA_SET_TIMEOUT
#define ENVOY_SA_SET_TIMEOUT 1000
#endif

#ifndef ENVOY_SA_SUBAGENT_MIB_ROOT_NODE
#define ENVOY_SA_SUBAGENT_MIB_ROOT_NODE(UP, P) (P)->mib_root = (MIBNODE_T *)UP
#endif

typedef struct PRIVBLOCK_S
        {
        PTR_T            data;
        VBPRIVPROC_T    *free_rtn;
        } PRIVBLOCK_T;

typedef struct COMPAT_BLOCK_S {
        INT_32_T         flags;
        SA_HEADER_T     *hdr_blk;
        VBL_T           *vblist;
        EBUFFER_T       *ebufp;
        } COMPAT_BLOCK_T;

ENVOY_TIMER_T tm;

SNMP_PKT_T *saved_pktp = 0;
INT_32_T readers = 0;

int finish_error;

void
  SA_Subagent_Timer(ENVOY_TIMER_T *tmp,
                    void          *cookiep)
{
envoy_tm_init(tmp);
if (saved_pktp) {
    SNMP_Free(saved_pktp);
    saved_pktp = 0;
    }
return;
}

/****************************************************************************
NAME:  sa_compat_reg

PURPOSE:  A registration completion routine for use with the
          backwards compatibility routine for SA_Handler_WR.
          It puts any error code as well as the vblist and
          header blcoks into a structure that the
          received packet specified as the private info.

PARAMETERS:
        int             The error code
        SA_HEADER_T *   Pointer to the header block
        VBL_T       *   Pointer to the vblist
        PTR_T           Pointer to private information block
                        in this case a COMPAT_BLOCK_T.

RETURNS:  nothing
****************************************************************************/

void 
  sa_compat_reg(INT_32_T     ecode,
                SA_HEADER_T *hdr_blk,
                VBL_T       *vblp,
                PTR_T        cptr)
{
COMPAT_BLOCK_T *cookie = (COMPAT_BLOCK_T *)cptr;
cookie->flags = ecode;
MEMCPY(cookie->hdr_blk, hdr_blk, sizeof(SA_HEADER_T));
MEMCPY(cookie->vblist, vblp, sizeof(VBL_T));
MEMSET(vblp, 0, sizeof(VBL_T));
return;  
}

/****************************************************************************
NAME:  sa_compat_error

PURPOSE:  An error completion routine for use with the backwards compatibility
          routine for SA_Handler_WR.  It puts the error
          code into a structure that the received packet specified as
          the private info.

PARAMETERS:
        int             The error code
        PTR_T           Pointer to private information block
                        in this case a COMPAT_BLOCK_T.

RETURNS:  nothing
****************************************************************************/

void 
  sa_compat_error(int    ecode,
                  PTR_T  cptr)
{
COMPAT_BLOCK_T *cookie;
cookie = (COMPAT_BLOCK_T *)cptr;
cookie->flags = (INT_32_T)ecode;
return;
}

/****************************************************************************
NAME:  sa_compat_io

PURPOSE:  An io completion routine for use with the backwards compatibility
          routine for SA_Handler_WR.  It attempts to
          encode the packet into the ebuffer given to the received routine.
          It finds this ebuffer in the private information area.  If
          the encode fails it puts an error code into the private area.

PARAMETERS:
        PTR_T           Pointer to the packet structure
        SA_HEADER_T *   Pointer to the header block
        PTR_T           Pointer to private information block
                        in this case a COMPAT_BLOCK_T.

RETURNS:  nothing
****************************************************************************/

void 
  sa_compat_io(PTR_T             pktp,
               SA_HEADER_T      *hdr_blk,
               PTR_T             cookie)
{
EBUFFER_T *ebuffp;
SA_HEADER_T *hdr_blk1;

ebuffp = ((COMPAT_BLOCK_T *)cookie)->ebufp;
hdr_blk1 = ((COMPAT_BLOCK_T *)cookie)->hdr_blk;

((COMPAT_BLOCK_T *)cookie)->flags = 0;
MEMCPY(hdr_blk1, hdr_blk, sizeof(SA_HEADER_T));
hdr_blk1->opcode1 = SA_QUERY_REQUEST;

if (SA_Handler_Finish(pktp, hdr_blk, ebuffp))
   SA_Handler_Cleanup(pktp, hdr_blk);
return;
}

/****************************************************************************
NAME:  SA_Handler_Continue_Function

PURPOSE:  Sub agent continuation function.  This function tests the
          given packet and if the packet is finished calls the completion
          routine.

          This function was called SA_Handler_Continue, until that
          was turned into a macro that would call the continuation
          routine from the packet structure.  

PARAMETERS:
        SNMP_PKT_T  *   Packet to examine

RETURNS: void
          
****************************************************************************/
void
  SA_Handler_Continue_Function(SNMP_PKT_T *pktp)
{
VB_T *vbp;
INT_32_T count;
SA_HEADER_T hdr_blk;
PRIVBLOCK_T *privlist = 0, *tprivlist;
int first_vbp = 0;

vbp = pktp->pdu.std_pdu.std_vbl.vblist;
count = pktp->pdu.std_pdu.std_vbl.vbl_count;

switch (pktp->sa_opcode2) {
    case SA_QUERY_TESTSET:
        for(; count; count--, vbp++)
            if ((vbp->vb_flags & VFLAG_SA_DOIT) &&
                (!(vbp->vb_flags & VFLAG_TEST_DONE)))
                goto sa_cont;
        break;
    case SA_QUERY_TESTGET:
    case SA_QUERY_GET:
        for(; count; count--, vbp++)
            if ((vbp->vb_flags & VFLAG_SA_DOIT) &&
                (!(vbp->vb_flags & VFLAG_GET_DONE)))
                goto sa_cont;
        readers--;
        break;
    case SA_QUERY_NEXT:
        for(; count; count--, vbp++)
            if ((vbp->vb_flags & VFLAG_SA_DOIT) &&
                (!(vbp->vb_flags & VFLAG_NEXT_DONE)))
                goto sa_cont;
        /* Translate any async style comment back into data_n_flags style */
        vbp = pktp->pdu.std_pdu.std_vbl.vblist;
        count = pktp->pdu.std_pdu.std_vbl.vbl_count;
        for(; count; count--, vbp++)
            if (vbp->vb_flags & VFLAG_NEXT_REDO)
                vbp->vb_data_flags_n_type = VT_ENDOFMIB;
        readers--;
        break;
    case SA_QUERY_SET:
        for(; count; count--, vbp++)
            if ((vbp->vb_flags & VFLAG_SA_DOIT) &&
                (!(vbp->vb_flags & VFLAG_SET_DONE)))
                goto sa_cont;
        break;
    case SA_QUERY_UNDO:
    case SA_QUERY_TESTCREATE:
        break;
    }

#if (INSTALL_ENVOY_CONTINUE_REENTRANT)
ENVOY_SNMP_RELEASE_WRITE_LOCK(pktp->continue_lock);
#endif

/* All the routines have finished, pack up the information and send it
   back to the master, cleaning up anything we can */
hdr_blk.version    = 1;
hdr_blk.opcode1    = SA_QUERY_REPLY;
hdr_blk.opcode2    = pktp->sa_opcode2;
hdr_blk.sa_error   = 0;
hdr_blk.encoding   = pktp->sa_encoding;
hdr_blk.snmp_version = (OCTET_T)(pktp->snmp_version & 0xFF);
hdr_blk.requestid    = (UINT_16_T)(pktp->pdu.std_pdu.request_id & 0xFFFF);
hdr_blk.sysuptime    = 1;
hdr_blk.index      = 0;
hdr_blk.snmp_error = 0;

/* If this is a testset request we need to save the vb priv info
   temporarily.  We set up only one vbp with information
   so that only one free routine will be called.  After we have
   created the packet we restore the vb priv information and
   save the packet until a set or undo request arrives.
   Otherwise we run any vbpriv routines before doing the encode. */
vbp = pktp->pdu.std_pdu.std_vbl.vblist;
count = pktp->pdu.std_pdu.std_vbl.vbl_count;
if ((hdr_blk.opcode2 == SA_QUERY_TESTSET) &&
    (pktp->pdu.std_pdu.error_status == NO_ERROR)){

    privlist = (PRIVBLOCK_T *)SNMP_memory_alloc(((int)count) *
                                                sizeof(PRIVBLOCK_T));
    if (privlist == 0) {
        hdr_blk.sa_error = SA_MEMORY_ERROR;
        ((SA_IO_COMPLETE_T *)pktp->io_complete)(0, &hdr_blk,
                                                pktp->async_cookie);
        SNMP_Free(pktp);
        return;
        }

    for (tprivlist = privlist; count; count--, vbp++, tprivlist++) {
        tprivlist->data = vbp->vb_priv;
        tprivlist->free_rtn = vbp->vb_free_priv;
        if (first_vbp && (vbp->vb_flags & VFLAG_SA_DOIT)) {
            vbp->vb_free_priv = 0;
            vbp->vb_priv = (PTR_T)1;
            first_vbp = 0;
            }
        else {
            vbp->vb_free_priv = 0;
            vbp->vb_priv = 0;
            }
        }
    }
else {
    for (; count; count--, vbp++)
        if (vbp->vb_free_priv) {
#if INSTALL_ENVOY_40_VB_FREE_COMPAT
            (*vbp->vb_free_priv)(vbp->vb_priv);
#else
            (*vbp->vb_free_priv)(vbp);
#endif
            vbp->vb_free_priv = 0;
            vbp->vb_priv = 0;
            }
        }

/* set up any error index and status information */
hdr_blk.index      = pktp->pdu.std_pdu.error_index;
hdr_blk.snmp_error = pktp->pdu.std_pdu.error_status;

finish_error = 0;
((SA_IO_COMPLETE_T *)pktp->io_complete)((PTR_T)pktp, &hdr_blk,
                                        pktp->async_cookie);
/* Now we either free the packet or restore the vb information and
   save a pointer to the packet for later use */
if ((hdr_blk.opcode2 == SA_QUERY_TESTSET) &&
    (pktp->pdu.std_pdu.error_status == NO_ERROR)){
   vbp = pktp->pdu.std_pdu.std_vbl.vblist;
   count = pktp->pdu.std_pdu.std_vbl.vbl_count;
   tprivlist = privlist;

    /* See if the completion routine indicated an error */
    if (finish_error) {
        for (; count; count--, vbp++, tprivlist++) {
            if (tprivlist->free_rtn)
#if INSTALL_ENVOY_40_VB_FREE_COMPAT
                (*tprivlist->free_rtn)(tprivlist->data);
#else
                (*tprivlist->free_rtn)(vbp);
#endif
            }
        SNMP_Free(pktp);
        }
    else {
        for (; count; count--, vbp++, tprivlist++) {
            vbp->vb_priv = tprivlist->data;
            vbp->vb_free_priv = tprivlist->free_rtn;
            }

        envoy_tm_init(&tm);
        tm.handler = SA_Subagent_Timer;
        if (envoy_tm_set(&tm, ENVOY_SA_SET_TIMEOUT))
            SNMP_Free(pktp);
        else
            saved_pktp = pktp;
        }
    SNMP_memory_free(privlist);
    }
else
    SNMP_Free(pktp);

return;

sa_cont:
#if (INSTALL_ENVOY_CONTINUE_REENTRANT)
ENVOY_SNMP_RELEASE_WRITE_LOCK(pktp->continue_lock);
#endif
return;
}

/****************************************************************************
NAME:  SA_Handler_WR

PURPOSE:  Decode a sub agent packet and attempt to fulfill the request.

PARAMETERS:
        OCTET_T     *   Byte string for message
        ALENGTH_T       Length of the message
        EBUFFER_T   *   A buffer for any reply packets (may not be used)
        SA_HEADER_T *   Space for header information
        VBL_T       *   Space to put the returned list of nodes
        PTR_T       *   User private space, one use is for multiple mib trees

RETURNS:  0             Success
          SA_UNKNOWN_VERSION
          SA_UNKNOWN_OPCODE1
          SA_UNKNOWN_OPCODE2
          SA_UNKNOWN_ENCODING
          SA_DECODE_FAILURE
          SA_ENCODE_FAILURE
          SA_GEN_ERROR
          
         This routine will decode all sub agent packets sent to a sub-agent.
         There are two basic types control (registration and deregistration
         replies) and queries (requests from the master).  VBL_T will
         be filled in with a list of the nodes in the packet, under some
         error conditions the list may not be complete.  If
         it is a query request this routine will run the agent code and
         create a reply which will be returned in the second EBUFER_T
         (again allocating space if necessary).  NOTE if there was a sub
         agent link type of error (UNKNOWN_ENCODING for example) the
         return code will be set but other fields may not be.

****************************************************************************/
INT_32_T
  SA_Handler_WR(OCTET_T     *msg,
                ALENGTH_T    msgl,
                EBUFFER_T   *rbuf,
                SA_HEADER_T *hdr_blk,
                VBL_T       *vblist,
                PTR_T        user_priv)
{
COMPAT_BLOCK_T cblock;
cblock.flags = 0;
cblock.hdr_blk = hdr_blk;
cblock.vblist = vblist;
cblock.ebufp = rbuf;

SA_Handler_Async(msg, msgl, user_priv, sa_compat_io,
                 sa_compat_error, sa_compat_reg, &cblock);

return(cblock.flags);

}

/****************************************************************************
NAME:  SA_Handler_Async

PURPOSE:  Decode a sub agent packet and attempt to fulfill the request.
          Using the async method routines.
          This routine will decode all sub agent packets sent to a sub-agent.
          If the packet is a control packet (registration or deregistration
          reply) we will call the reg_complete routine with pointers
          to the header block, a vbl list containing a list of nodes and
          the user supplied completion cookie.  Under some conditions
          the node list may be incomplete.
          If it is a query request the routine will attempt to process the
          the request and call io_complete to handle transmission of the
          outbound buffer.
          If an error occurs we shall call the error_complete routine.
          Possible error codes are:
          SA_UNKNOWN_VERSION,  SA_UNKNOWN_OPCODE1, SA_UNKNOWN_OPCODE2,
          SA_UNKNOWN_ENCODING, SA_DECODE_FAILURE,  SA_ENCODE_FAILURE
          SA_GEN_ERROR

PARAMETERS:
        OCTET_T     *   Byte string for message
        ALENGTH_T       Length of the message
        PTR_T           User private space, one use is for multiple mib trees
        SA_IO_COMPLETE_T,       The io_completion routine.  Args are the
                                the packet and header block to send
                                and the completion cookie
        SA_ERR_COMPLETE_T       The error_completion routine.  Args are the
                                error code and the completion cookie
        SA_REG_COMPLETE_T       The reg/dereg completion routine
                                Args are an error code, the header block,
                                a list of nodes, and the completion cookie.
        PTR_T           Cookie for use with the completion routines

RETURNS: void
          
****************************************************************************/
void
  SA_Handler_Async(OCTET_T     *msg,
                   ALENGTH_T    msgl,
                   PTR_T        user_priv,
                   SA_IO_COMPLETE_T  *  io_complete,
                   SA_ERR_COMPLETE_T *  error_complete,
                   SA_REG_COMPLETE_T *  reg_complete,
                   PTR_T        completion_cookie)
{
SNMP_PKT_T *pktp = 0;
VBL_T vblist;
SA_HEADER_T hdr_blk;
SA_DEMUX_T demuxer;
INT_32_T count, errflag, *err_stat;
VB_T *vbp;
OCTET_T opcode2;

/* Initialize some variables */
MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));
MEMSET(&vblist, 0, sizeof(VBL_T));
EBufferInitialize(&demuxer.string);
init_object_id(&demuxer.object);

/* Decode the msg */
errflag = SA_Decode(msg, msgl, &vblist, &hdr_blk, &demuxer);

/* To avoid any problems with a master sending down
   vb priv type information we 0 it all out here */
for (count = vblist.vbl_count, vbp = vblist.vblist; count; count--, vbp++) {
    vbp->vb_priv = 0;
    vbp->vb_free_priv = 0;
    }

/* If we don't understand the version drop the packet as we can't
   be sure that sending an unknown version packet won't
   cause an infinite loop */
if (errflag == SA_UNKNOWN_VERSION) {
    CLEAN_DEMUX(&demuxer);
    error_complete(SA_UNKNOWN_VERSION, completion_cookie);
    return;
    }

/* If this is a response to a sub agent request return the information
   to our caller */
if (hdr_blk.opcode1 != SA_QUERY_REQUEST) {
    reg_complete(errflag, &hdr_blk, &vblist, completion_cookie);
    CLEAN_DEMUX(&demuxer);
    Clean_vb_list(&vblist);
    return;
    }

/* This should be a query request and it shouldn't have an sa error.
   If it does bad things are going on so we drop the packet. */
if (hdr_blk.sa_error) {
    error_complete(hdr_blk.sa_error, completion_cookie);
    CLEAN_DEMUX(&demuxer);
    Clean_vb_list(&vblist);
    return;
    }

/* We now switch opcode1 to be query reply so we can use the hdr_blk
   for the outbound packet */
hdr_blk.opcode1 = SA_QUERY_REPLY;

/* We had an error and should send a packet back to the master agent */
if (errflag) {
    hdr_blk.sa_error = (OCTET_T)(errflag & 0xFF);
    io_complete(0, &hdr_blk, completion_cookie);
    return;
    }

opcode2 = hdr_blk.opcode2;

switch (opcode2) {
    case SA_QUERY_TESTSET:
    case SA_QUERY_TESTCREATE:
        /* Check if we have read packets outstanding, in which case we
           shouldn't go trying to write the database.
           No there isn't suppossed to be a break between these
           two options and the next set */
        if (readers != 0) {
            Clean_vb_list(&vblist);
            CLEAN_DEMUX(&demuxer);
            hdr_blk.sa_error = SA_LOCK_ERROR;
            io_complete(0, &hdr_blk, completion_cookie);
            return;
            }

    case SA_QUERY_TESTGET:
    case SA_QUERY_GET:
    case SA_QUERY_NEXT:
        /* Check if we already have a saved packet, in which case we
           are in the middle of a write and shouldn't do anything but
           the corresponding set or undo */
        if (saved_pktp != 0) {
            Clean_vb_list(&vblist);
            CLEAN_DEMUX(&demuxer);
            hdr_blk.sa_error = SA_LOCK_ERROR;
            io_complete(0, &hdr_blk, completion_cookie);
            return;
            }
        /* Allocate space for the packet and fill it in. */
        if ((pktp = SNMP_Allocate()) == 0) {
            Clean_vb_list(&vblist);
            CLEAN_DEMUX(&demuxer);
            hdr_blk.sa_error = SA_MEMORY_ERROR;
            io_complete(0, &hdr_blk, completion_cookie);
            return;
            }

        /* fill in the packet structure */
        pktp->snmp_version = hdr_blk.snmp_version;
        pktp->cont = SA_Handler_Continue_Function;
        /* After transfering the info to the packet structure we
           0 the demuxers so we won't have accidents */
        EBufferPreLoad(BFL_IS_DYNAMIC, &pktp->community,
                       EBufferStart(&demuxer.string),
                       EBufferUsed(&demuxer.string));
        EBufferInitialize(&demuxer.string);
        Clean_Obj_ID(&demuxer.object);

        ENVOY_SA_SUBAGENT_MIB_ROOT_NODE(user_priv, pktp);

        vbp = pktp->pdu.std_pdu.std_vbl.vblist = vblist.vblist;
        count = pktp->pdu.std_pdu.std_vbl.vbl_count = vblist.vbl_count;
        /* Again after we transfer the info to the packet we 0 the
           vblist so we won't have accidents */
        vblist.vblist = 0;
        vblist.vbl_count = 0;

        /* If required get the locks */
#if INSTALL_ENVOY_SNMP_LOCK
        if (opcode2 == SA_QUERY_TESTSET) {
            if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_CoarseLock)) {
                BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
                    (BUG_OUT, "SA_Handler_Async: coarse lock is broken", 0));

                SNMP_Free(pktp);
                hdr_blk.sa_error = SA_LOCK_ERROR;
                io_complete(0, &hdr_blk, completion_cookie);
                return;
                }
            else {
                pktp->lockflags = LOCK_WRITE;
                pktp->coarse_lock = &SNMP_CoarseLock;
                }
            }
        else {
            if (ENVOY_SNMP_GET_READ_LOCK(SNMP_CoarseLock)) {
                BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
                    (BUG_OUT,
                     "SA_Handler_Async: coarse lock is broken",
                     0));

                SNMP_Free(pktp);
                hdr_blk.sa_error = SA_LOCK_ERROR;
                io_complete(0, &hdr_blk, completion_cookie);
                return;
                }
            else {
                pktp->lockflags = LOCK_READ;
                pktp->coarse_lock = &SNMP_CoarseLock;
                }
            }
#endif

        /* we loop through the vb list finding mib leaf nodes for each of
           the vbs */
        for(; count; count--, vbp++)
            if (vbp->vb_flags & VFLAG_SA_DOIT)
                if (find_object_node(vbp, pktp, 0)) {
                    /* couldn't find a node go to the cleanup area */
                    SNMP_Free(pktp);
                    hdr_blk.sa_error = SA_UNKNOWN_NODE;
                    io_complete(0, &hdr_blk, completion_cookie);
                    return;
                    }
        break;
    case SA_QUERY_SET:
    case SA_QUERY_UNDO:
        /* As we are going to use the information from the saved packet
           we clean up the incoming infomration */
        Clean_vb_list(&vblist);
        CLEAN_DEMUX(&demuxer);
        (void) envoy_tm_cancel(&tm);
        if ((pktp = saved_pktp) == 0) {
            hdr_blk.sa_error = SA_NO_SAVED_PACKET;
            io_complete(0, &hdr_blk, completion_cookie);
            return;
            }
        saved_pktp = 0;
        break;
    }

/* We always use the most recent completion & opcode information*/
pktp->io_complete = (IO_COMPLETE_T *)io_complete;
pktp->error_complete = (ERR_COMPLETE_T *)error_complete;
pktp->async_cookie = completion_cookie;
pktp->sa_opcode2 = opcode2;
pktp->sa_encoding = hdr_blk.encoding;
pktp->pdu.std_pdu.request_id = hdr_blk.requestid;

#if (INSTALL_ENVOY_CONTINUE_REENTRANT)
/* If necessary we acquire the packet continue write lock here
   It gets freed when we call the continue routine.
   We don't have any good options if we can't get the lock.
   The user shouldn't do that, if they do we generate a bug
   report and if they don't handle that we return which is
   the least bad thing to do. */
if (ENVOY_SNMP_GET_WRITE_LOCK(pktp->continue_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_FATAL, 0,
        (BUG_OUT, "SA_Handler_Async(): packet continue lock is broken", 0));
    return;
    }
#endif

err_stat = &(pktp->pdu.std_pdu.error_status);
count = pktp->pdu.std_pdu.std_vbl.vbl_count;
vbp = pktp->pdu.std_pdu.std_vbl.vblist;

switch (opcode2) {
    case SA_QUERY_TESTGET:
    case SA_QUERY_GET:
        readers++;
        /* For any vars that we shouldn't do set their done bits so we
           won't trip on them later */
        for(; count; count--, vbp++) {
            if (!(vbp->vb_flags & VFLAG_SA_DOIT)) {
                vbp->vb_flags |= VFLAG_GET_STARTED | VFLAG_GET_DONE;        
                continue;
                }
            if (vbp->vb_flags & (VFLAG_GET_STARTED | VFLAG_GET_DONE))
                continue;
            ((vbp->vb_ml.ml_leaf)->getproc)
              (vbp->vb_ml.ml_last_match,
               vbp->vb_ml.ml_remaining_objid.num_components,
               vbp->vb_ml.ml_remaining_objid.component_list,
               pktp, vbp);
            vbp->vb_flags |= VFLAG_GET_STARTED;
            /* test the error code, if we had an error then mark any remaining
               vbs as having been tested (so we won't trip over them later).
               We need to skip the first vb as that is the one that
               caused the failure.  Then break because we are done.  */
            if (*err_stat) {
                for (vbp++, count--; count; count--, vbp++)
                    if (!(vbp->vb_flags & VFLAG_GET_STARTED))
                        vbp->vb_flags |= VFLAG_GET_STARTED | VFLAG_GET_DONE;
                break;
                }
            }
        break;
    case SA_QUERY_TESTSET:
        /* For any vars that we shouldn't do set their done bits so we
           won't trip on them */
        for(; count; count--, vbp++) {
            if (!(vbp->vb_flags & VFLAG_SA_DOIT)) {
                setproc_all_bits(pktp, vbp);
                continue;
                }
            if (vbp->vb_flags & (VFLAG_TEST_STARTED | VFLAG_TEST_DONE))
                continue;
            ((vbp->vb_ml.ml_leaf)->testproc)
              (vbp->vb_ml.ml_last_match,
               vbp->vb_ml.ml_remaining_objid.num_components,
               vbp->vb_ml.ml_remaining_objid.component_list,
               pktp, vbp);
            vbp->vb_flags |= VFLAG_TEST_STARTED;
            /* test the error code, if we had an error then mark any remaining
               vbs as having been tested (so we won't trip over them later).
               We need to skip the first vb as taht is the one that
               caused the failure.  Then break because we are done.  */
            if (*err_stat) {
                for (vbp++, count--; count; count--, vbp++)
                    if (!(vbp->vb_flags & VFLAG_TEST_STARTED))
                        vbp->vb_flags |= VFLAG_TEST_STARTED | VFLAG_TEST_DONE;
                break;
                }
            }
        break;

    case SA_QUERY_NEXT:
        readers++;
        /* For any vars that we shouldn't do set their done bits so we
           won't trip on them */
        for(; count; count--, vbp++) {
            if (!(vbp->vb_flags & VFLAG_SA_DOIT)) {
                vbp->vb_flags |= VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE;
                continue;
                }
            if (vbp->vb_flags & (VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE))
                continue;
            ((vbp->vb_ml.ml_leaf)->nextproc)
              (vbp->vb_ml.ml_last_match,
               vbp->vb_ml.ml_remaining_objid.num_components,
               vbp->vb_ml.ml_remaining_objid.component_list,
               pktp, vbp);
            vbp->vb_flags |= VFLAG_NEXT_STARTED;
            /* test the error code, if we had an error then mark any remaining
               vbs as having been tested (so we won't trip over them later).
               We need to skip the first vb as that is the one that
               caused the failure.  Then break because we are done.  */
            if (*err_stat) {
                for (vbp++, count--; count; count--, vbp++)
                    if (!(vbp->vb_flags & VFLAG_NEXT_STARTED))
                        vbp->vb_flags |= VFLAG_NEXT_STARTED | VFLAG_NEXT_DONE;
                break;
                }
            }
        break;

    case SA_QUERY_SET:
        for(; count; count--, vbp++) {
            if (vbp->vb_flags & (VFLAG_SET_STARTED | VFLAG_SET_DONE))
                continue;
            ((vbp->vb_ml.ml_leaf)->setproc)
              (vbp->vb_ml.ml_last_match,
               vbp->vb_ml.ml_remaining_objid.num_components,
               vbp->vb_ml.ml_remaining_objid.component_list,
               pktp, vbp);
            vbp->vb_flags |= VFLAG_SET_STARTED;
            /* test the error code, if we had an error then mark any remaining
               vbs as having been undone (as they were never done to begin
               with).  We need to skip the first vb as that is the one that
               caused the failure.  Then break because we are done.  */
            if (*err_stat) {
                for (vbp++, count--; count; count--, vbp++)
                    if (!(vbp->vb_flags & VFLAG_SET_STARTED))
                        vbp->vb_flags |= VFLAG_SET_STARTED  | VFLAG_SET_DONE |
                                         VFLAG_UNDO_STARTED | VFLAG_UNDO_DONE;
                break;
                }
            }
        break;

    case SA_QUERY_UNDO:
        break;

    case SA_QUERY_TESTCREATE:
        for(; count; count--, vbp++)
            vbp->vb_flags |= VFLAG_TEST_STARTED | VFLAG_TEST_DONE;
        break;
    }

SA_Handler_Continue_Function(pktp);
return;
}


/****************************************************************************
NAME:  SA_Handler_Finish

PURPOSE:  Finishing routine, to be called by the io_completion routine
          this will enocde the header block and packet information into
          the ebuff structure.  If the packet is the null pointer
          we only encode the header using a static buffer if the
          ebuffer doesn't have any space.

PARAMETERS:
        PTR_T           The packet to encode
        SA_HEADER_T *   the header block to encode
        EBUFFER_T   *   The buffer to put the encoded packet into

RETURNS: int    success = 0, failure = non-zero
          
****************************************************************************/
INT_32_T
  SA_Handler_Finish(PTR_T        pkt,
                    SA_HEADER_T *hdr_blk,
                    EBUFFER_T   *rbuf)
{
static OCTET_T errorhdr[SA_HEADERLEN];
SA_DEMUX_T demuxer;
SNMP_PKT_T *pktp;

if (pkt) {
    pktp = (SNMP_PKT_T *)pkt;
    /* rebuild a demuxer */
    init_object_id(&demuxer.object);
    EBufferPreLoad(BFL_IS_STATIC, &demuxer.string,
                   EBufferStart(&pktp->community),
                   EBufferUsed(&pktp->community));
    return(SA_Encode(&(pktp->pdu.std_pdu.std_vbl), hdr_blk, &demuxer, rbuf));
    }
else {
    (void) SA_Encode_Hdr(errorhdr, hdr_blk);
    if (rbuf->start_bp == 0) 
        EBufferPreLoad(BFL_IS_STATIC, rbuf, errorhdr, SA_HEADERLEN);
    else {
        if (EBufferRemaining(rbuf) < SA_HEADERLEN)
            return(SA_ENCODE_FAILURE);
        MEMCPY(rbuf->next_bp, errorhdr, SA_HEADERLEN);
        rbuf->next_bp += SA_HEADERLEN;
        rbuf->remaining -= SA_HEADERLEN;
        }
    }
return(0);
}

/****************************************************************************
NAME:  SA_Handler_Cleanup

PURPOSE:  Cleanup routine to be called by the io completion routine
          if it detects an error (such as memory unavailable).

PARAMETERS: 
        PTR_T           The packet being encoded (may be zero)
        SA_HEADER_T *   The header being encoded

RETURNS: void
          
****************************************************************************/
void
  SA_Handler_Cleanup(PTR_T        pkt,
                     SA_HEADER_T *hdr_blk)
{
finish_error = 1;
return;
}
#endif /* #if (INSTALL_ENVOY_EPI_SUB) */
