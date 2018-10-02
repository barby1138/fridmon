/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/sa_mast.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: sa_mast.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:57  sar
 * Updated revision to 8.0
 *
 * Revision 7.7  1997/10/25 06:10:07  sar
 * Before processing a deferred (de)registration request save a copy of
 * the ipcfree pointer so that we can free the handle after the
 * (de)registration has occurred, even if the group structure which
 * contained the ipcfree pointer has been freed.
 *
 * Revision 7.6  1997/10/22 03:10:24  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.5  1997/10/16 23:50:42  sar
 * A bug in the handling of the next function for sub agents was fixed.
 * This bug could manifest itself if a next request contained two objects
 * with the same instance information that were registered from two
 * different sub agents.
 *
 * A bug in the handling of resetting some flag bits could cause problems
 * doing gets & nexts if you are using Master_Handler or Master_Handler_WR.
 * The problem will occur when the sub agent marks some subset of the objects
 * being nexted or gotten as unavailable.
 *
 * Revision 7.4  1997/10/16 00:46:01  sar
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
 * Revision 7.3  1997/03/20 06:49:11  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1997/01/16 20:08:46  sar
 * Modified the error handling code in the get & next mid routines
 * to expect a 1 based error index from the sub agent.
 *
 * Revision 7.0  1996/03/18  20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.4  1996/03/12  16:01:43  sar
 * Added a check for an empty vblist before doing a (de)registration
 * operation.
 *
 * Revision 6.3  1996/01/05  20:18:41  sar
 * Free the control block in master_query_replace_header.
 *
 * Revision 6.2  1995/11/01  00:46:48  sar
 * removed localio.h, bstring.h and party.h as well as bitstrings
 *
 * Revision 6.1  1995/06/07  20:09:21  sar
 * Made the opcode field in the ipc_comp structure a pointer and
 * added several ipcfree calls to clean up ipchandle if we aren't
 * going to use it.
 *
 * Revision 6.0  1995/05/31  21:47:57  sra
 * Release 6.0.
 *
 * Revision 5.13  1995/05/27  01:17:47  sar
 * Removed reqid from master_query_reply_handler's args.
 *
 * Revision 5.12  1995/05/26  00:50:29  sar
 * Updated some the method routines to deal with the tested/done flags better.
 * ,
 *
 * Revision 5.11  1995/05/25  03:03:42  sar
 * Dynamically allocate the ipc compatibility block for SA_Handler_WR.
 *
 * Revision 5.10  1995/05/23  23:23:14  sar
 * Modifed the continue_reentrant code, the continue routines expect
 * their callers to have gotten the lock and will release it.  The callers
 * must get the lock before modifying the packet.
 *
 * Revision 5.9  1995/05/22  19:43:54  sar
 * Updated to use the packet lock if it's installed (continue_reentrant).
 *
 * Revision 5.8  1995/05/20  02:37:57  sar
 * need to set the data_flags_n_type for older sub agents during get phase.
 *
 * Revision 5.7  1995/04/28  23:23:46  sar
 * If the serialization functions are installed use them to serialize
 * registration/deregistration requests.
 * Moved some utility functions into sa_mutil.c
 * Chopped out som non-ansi routine defs.
 *
 * Revision 5.6  1995/04/18  21:13:44  sar
 * Got rid of timer code - timeouts are now handled by the user.  Used the
 * new lock macros.  Removed the subagent_is_async ifdefs and instead
 * rewrote the code to use compaibility rotuines between the old style
 * user code (ipc routines) and the new api.  Added code to cleanup groups
 * when a packet is freed.  Moved sa_do_query here.
 *
 * Revision 5.5  1995/03/24  16:32:04  sar
 * Added timeout code to deal with packets when messages to subagents are lost.
 *
 * Revision 5.4  1995/03/21  00:30:26  sar
 * Added async capability, we can now send a message to a subagent
 * and continue processing the rest of the packet instead of blocking
 * until the sub agent responds.
 *
 * Revision 5.3  1994/10/13  19:24:50  sar
 * Added an error condition if a sub agent requests a unsupported object
 * type.
 * Copy the hdr_blk version number into the group version field.
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
 * Revision 1.4  1994/05/13  21:42:33  sar
 * Added some more type casts.
 *
 * Revision 1.3  1994/05/13  15:32:03  sar
 * Added some type casts to make the compilers happy.
 *
 * Revision 1.2  1994/05/12  17:16:29  sar
 * added snmp_version to the message header, modified some returns
 * from get routines to make them cleaner.
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

/* Calls to get and release the infrastructure lock were added to
   this code in order to allow it to coexist with the agentx code.
   These locks are used to mediate access to the mib tree.  They
   are not used for access to the other sub agent information such
   as group lists or instance lists.  That information is kept under
   it's previous locks - specifically coarse_lock.  If we get rid
   of that lock we will need to do some work in this code. */

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

#if (INSTALL_ENVOY_EPI_MASTER)

#if !defined(SNMP_SYSUPTIME)
#define SNMP_SYSUPTIME 0
#endif

extern INT_16_T group_cleanup_flag;

INT_32_T         GROUPID     = 1;
UINT_16_T        sa_reqid    = 0;

/* Lock for use with sub agent schemes */
ENVOY_SNMP_LOCK(SNMP_SubagentLock);

/* structure for use with asynchronous sub agents */
typedef struct SNMP_SA_CB_S
        {
        struct SNMP_SA_CB_S *next;
        UINT_16_T   reqid;
        SNMP_PKT_T *pktp;
        VB_T       *vbp;
        int         count;
        } SNMP_SA_CB_T;

SNMP_SA_CB_T *first_sacb = 0;

UINT_16_T
  SA_make_cb(SNMP_PKT_T *pktp, 
             VB_T       *vbp,
             int         count)
{
SNMP_SA_CB_T *ctl_blk;

ctl_blk = (SNMP_SA_CB_T *)SNMP_memory_alloc(sizeof(SNMP_SA_CB_T));
if (ctl_blk == 0)
    return(0);

#if (INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_SubagentLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SA_make_cb: subagent lock is broken", 0));

    SNMP_memory_free(ctl_blk);
    return(0);
    }
#endif

/* 0 is a reserved id so make sure we don't give it out */
if (sa_reqid == 0)
    sa_reqid++;

/* install the info into the state block */
ctl_blk->reqid  = sa_reqid++;
ctl_blk->pktp   = pktp;
ctl_blk->vbp    = vbp;
ctl_blk->count  = count;

/* and put the state block into the state list */
ctl_blk->next = first_sacb;
first_sacb = ctl_blk;

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_SubagentLock);

return(ctl_blk->reqid);
}

SNMP_SA_CB_T *
  SA_uninstall_cb(UINT_16_T reqid, int free_flag)
{
SNMP_SA_CB_T *cbp = 0, **pcbp;

#if (INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_SubagentLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SA_uninstall_cb: subagent lock is broken", 0));

    return(0);
    }
#endif

for(pcbp = &first_sacb; *pcbp; pcbp = &(*pcbp)->next)
    if ((*pcbp)->reqid == reqid) {
        cbp = *pcbp;
        *pcbp = cbp->next;
        break;
        }

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_SubagentLock);

if (free_flag && cbp) {
    SNMP_memory_free(cbp);
    return(0);
    }
else
    return(cbp);
}

/****************************************************************************
NAME:  leaf_active

PURPOSE:  Given a leaf determine if it is active (it's ipc link is still
          ok).  First we check the inactive bit in the leaf structure
          if it is set the leaf has already been marked as inactive and
          we return inactive (0).  Else we look at an active bit in the
          group structures, if it any are set the group is active and we
          return active (1).  Else we run the ipcayt routines which
          tells us if the ipc is active or not.  If any are active we
          set the group active bit.  This is a temporary bit and
          will be reset after we are done testing all the leaves in
          one registration request.  It allows us to call the ipcayt
          routine only once for each group.  If ipcayt returns inactive
          we go through the group marking all the leaves as inactive.
          We don't attempt to remove the leaves but we do remove and
          free the group information.

PARAMETERS:
        MIBLEAF_T     * Leaf node to be tested.

RETURNS:  0     Inactive
          1     Active
          
****************************************************************************/
INT_32_T
  leaf_active(MIBLEAF_T *leaf)
{
SA_GRP_LST_T *grplst;
SA_GROUP_T  *group;

if (leaf->node_type & LEAF_INACTIVE)
    return(0);

for (grplst = ((SA_COOKIE_T *)(leaf->user_cookie))->ichain;
     grplst != 0;
     grplst = grplst->ichain)
    if (grplst->group->flags & SA_GROUP_ACTIVE)
        return(1);
  
grplst = ((SA_COOKIE_T *)(leaf->user_cookie))->ichain;
while (grplst != 0) {
    group = grplst->group;
    /* run the ayt routine */
    if (group->ipcayt(group->ipchandle)) {
        /* the link is active */
        group->flags |= SA_GROUP_ACTIVE;
        return(1);
        }

    /* the link is inactive, remove the entry that points to this group
       from the ipc handle list, mark all items in this group as inactive 
       and remove the group structures */
    grplst = grplst->ichain;
    hand_remove_group(group);
    mark_group_inactive(group);
    }
return(0);
}

/****************************************************************************
NAME:  transfer_value

PURPOSE:  Transfer a value from one vbp to another vbp.  Generally
          this will be used to transfer the information returned
          from a sub agent to the packet in the master agent.

PARAMETERS:
        SNMP_PKT_T *    Packet we are processing
        VB_T    *       Variable to put the information into
        VB_T    *       Variable to take the information from

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  transfer_value(SNMP_PKT_T     *pktp,
                 VB_T           *vbp,
                 VB_T           *rvbp)
{
OCTET_T data_type;

data_type = vbp->vb_ml.ml_leaf->expected_tag;
switch (data_type) {
    case VT_NUMBER:
        getproc_got_int32(pktp, vbp, rvbp->value_u.v_number);
        break;
    case VT_COUNTER:
    case VT_GAUGE:
    case VT_TIMETICKS:
    case VT_UINTEGER32:
        getproc_got_uint32(pktp, vbp, rvbp->value_u.v_counter, data_type);
        break;
    case VT_STRING:
    case VT_OPAQUE:
        getproc_got_string(pktp, vbp, EBufferUsed(&(rvbp->value_u.v_string)),
                           EBufferStart(&(rvbp->value_u.v_string)),
                           rvbp->value_u.v_string.bflags, data_type);
        EBufferInitialize(&(rvbp->value_u.v_string));
        break;
    case VT_OBJECT:
        getproc_got_object_id(pktp, vbp, rvbp->value_u.v_object.num_components,
                              rvbp->value_u.v_object.component_list, 1);
        init_object_id(&(rvbp->value_u.v_object));
        break;
    case VT_EMPTY:
        getproc_got_empty(pktp, vbp);
        break;
    case VT_IPADDRESS:
        MEMCPY(&(vbp->value_u.v_network_address), 
               &(rvbp->value_u.v_network_address), 4);
        getproc_good(pktp, vbp);
        break;

/* We only include the following case if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
    case VT_COUNTER64:
        getproc_got_uint64(pktp, vbp, &(rvbp->value_u.v_counter64));
        break;
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */

    default:
        getproc_error(pktp, vbp, GEN_ERR);
        break;
    }

return;
}

/****************************************************************************
NAME:  SA_do_query

PURPOSE:  Do the common parts of a query request/reply for the manager
          given a vblist to send, a packet pointer, a group and
          a vblist for receiving, encode the message, send it using
          the ipc link.

PARAMETERS:
        SA_HEADER_T *   The header block, it have the opcode2 and index
                        fields filled in, everything else should be 0.
                        Other required fields will be filled in here.
                        It will be 0 and filled with the reply header
                        on return.
        VBL_T      *    The vbs to be sent.
        SNMP_PKT_T *    The packet we are working on.
        SA_GROUP_T *    The group to use for ipc stuff

RETURNS:   0    Success
          -1    Failure

****************************************************************************/
INT_32_T
  SA_do_query(SA_HEADER_T *hdr_blk,
              VBL_T       *vblist,
              SNMP_PKT_T  *pktp,
              SA_GROUP_T  *group)
{
EBUFFER_T ebuf;
SA_DEMUX_T demuxer;

if (group->flags & SA_GROUP_INACTIVE) {
    Master_Cleanup(hdr_blk->requestid,
                   SA_CLEANUP_TIMEOUT | SA_CLEANUP_INACTIVE);
    return(-1);
    }

EBufferInitialize(&ebuf);

EBufferPreLoad(BFL_IS_STATIC, &demuxer.string, EBufferStart(&pktp->community),
               EBufferUsed(&pktp->community));
init_object_id(&demuxer.object);

hdr_blk->version    = 1;
hdr_blk->opcode1    = SA_QUERY_REQUEST;
hdr_blk->encoding   = (OCTET_T)(group->encoding & 0xFF);
hdr_blk->snmp_version = (OCTET_T)(pktp->snmp_version & 0xFF);
hdr_blk->sysuptime  = SNMP_SYSUPTIME;
if (SA_Encode(vblist, hdr_blk, &demuxer, &ebuf)) {
    Master_Cleanup(hdr_blk->requestid, 0);
    return(-1);
    }

switch (group->ipcsend(&ebuf, group->ipchandle, hdr_blk->requestid)) {
    case 1:
    default:
        Master_Cleanup(hdr_blk->requestid,
                          SA_CLEANUP_TIMEOUT | SA_CLEANUP_INACTIVE);
        return(-1);
    case 2:
        Master_Cleanup(hdr_blk->requestid, SA_CLEANUP_TIMEOUT);
        return(-1);
    case 0:
        break;
    }
return(0);
}

/****************************************************************************
NAME:  SA_test_end

PURPOSE:  This routine is for use with sub agents. 
          It is the end routine for performing a test function,
          It examines the response from the sub agent, if there was
          an error it marks the packet as such and returns, otherwise
          it integrates the responses into the packet.

PARAMETERS:
        SNMP_PKT_T  *   SNMP packet currently being processsed.
        VB_T        *   Variable being processed.
        SA_HEADER_T *   header block from the sub agent
        VBL_T       *   vb list returned from the sub agent
        INT_32_T        error flag, included so we can clear the doit bits
                        before testing the flag.
RETURNS:  void

****************************************************************************/
/*ARGSUSED*/
void
  SA_test_end(SNMP_PKT_T        *pktp,
              VB_T              *vbp,
              SA_HEADER_T       *hdr,
              VBL_T             *rvblist,
              int                tcount)
{
VB_T *tvbp, *rvbp, *trvbp;

/* Tag the vbp as having been done, so we don't have to do so later */
testproc_good(pktp, vbp);

/* If there was a sub agent link error generate a gen err */
if (hdr->sa_error) {
    pktp->pdu.std_pdu.error_status = GEN_ERR;
    Clean_vb_list(rvblist);
    return;
    }

/* If there was a snmp error stuff it into the packet */
if (hdr->snmp_error) {
    pktp->pdu.std_pdu.error_status = hdr->snmp_error;
    pktp->pdu.std_pdu.error_index  = hdr->index;
    Clean_vb_list(rvblist);
    return;
    }

/* As we sent all of the vbs down they all better have come back */
if (pktp->pdu.std_pdu.std_vbl.vbl_count != rvblist->vbl_count) {
    pktp->pdu.std_pdu.error_status = GEN_ERR;
    Clean_vb_list(rvblist);
    return;
    }

rvbp = rvblist->vblist;
for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
    trvbp = &rvbp[vbp_to_index(pktp, tvbp)];
    tvbp->vb_flags      |= trvbp->vb_flags;
    tvbp->vb_priv        = trvbp->vb_priv;
    tvbp->vb_free_priv   = rvbp->vb_free_priv;
    trvbp->vb_priv       = 0;
    trvbp->vb_free_priv  = 0;
    }
Clean_vb_list(rvblist);
return;
}

/****************************************************************************
NAME:  SA_testproc

PURPOSE:  This routine is for use with sub agents, it collects all
          the vbs for a single sub agent and attempts to ship them
          there.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T  *       Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T    *       Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  SA_testproc(OIDC_T             last_match,
              int                compc,
              OIDC_T            *compl,
              SNMP_PKT_T        *pktp,
              VB_T              *vbp)
{
SA_COOKIE_T *cookie;
SA_GROUP_T *group;
SA_GRP_LST_T *grplst = 0;
SA_HEADER_T hdr_blk;
VB_T *tvbp, *lvbp;
VBL_T *vblist;
TREENODE_T *tnode;
int rlen;
INT_32_T i, ecode;

MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));
hdr_blk.opcode2 = SA_QUERY_TESTSET;
hdr_blk.index = vbp_to_index(pktp, vbp);

vblist = &(pktp->pdu.std_pdu.std_vbl);

if ((cookie = (SA_COOKIE_T *)(VB_TO_COOKIE(vbp))) == 0) {
    testproc_error(pktp, vbp, GEN_ERR);
    return;
    }

if (cookie->flags & SA_INSTANCE_TYPE) {
    tnode = TREE_Get(cookie->instance_tree, compl, compc, &rlen);
    if ((tnode == 0) || (rlen != compc)) {
        testproc_error(pktp, vbp, NO_CREATION);
        return;
        }
    if (TREE_Extract(tnode, (PTR_T *)(&grplst))) {
        testproc_error(pktp, vbp, GEN_ERR);
        return;
        }
    group = grplst->group;
    }
else {
    group = cookie->ichain->group;
   } 

/* Make sure no other method routine has inactiveated the group */
if (group->flags & SA_GROUP_INACTIVE) {
    testproc_error(pktp, vbp, NO_CREATION);
    return;
    }

/* Build and save the state block */
if ((hdr_blk.requestid = SA_make_cb(pktp, vbp, 0)) == 0) {
    testproc_error(pktp, vbp, GEN_ERR);
    return;
    }

lvbp = vbp;
/* Mark all the vbs before vbp to be sent but not done */
for (tvbp = vblist->vblist, i = vblist->vbl_count; tvbp != vbp; tvbp++, i--)
    tvbp->vb_flags |= VFLAG_SA_SENDIT;
/* Mark vbp as needing to be sent & done */
vbp->vb_flags |= VFLAG_SA_SENDIT | VFLAG_SA_DOIT;
/* Skip vbp, then mark the rest of the vbs in the group as needing to
   be sent & done, all other vbs to be sent */
for (tvbp++, i--; i > 0; tvbp++, i--) {
    if ((tvbp->vb_ml.ml_leaf->testproc == (ASY_TESTPROC_T *)SA_testproc) &&
        (SA_getgroup(tvbp->vb_ml.ml_remaining_objid.num_components,
                     tvbp->vb_ml.ml_remaining_objid.component_list,
                     (SA_COOKIE_T *)tvbp->vb_ml.ml_leaf->user_cookie)
         == group)) {
        lvbp->vb_link = tvbp;
        lvbp = tvbp;
        tvbp->vb_flags |= VFLAG_SA_SENDIT | VFLAG_SA_DOIT;
        }
    else
        tvbp->vb_flags |= VFLAG_SA_SENDIT;
    }

ecode = SA_do_query(&hdr_blk, vblist, pktp, group);
/* Reset the sendit and doit flags so we won't send them again */
for (tvbp = vblist->vblist, i = vblist->vbl_count; i; i--, tvbp++)
    tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
if (ecode) {
    testproc_error(pktp, vbp, GEN_ERR);
    return;
    }

/* Mark all but the first vbp as having been tested.  When we get a
   reply back from the sub agent we can either mark the first one as
   done and processing will continue or we can mark an error and
   processing will finish up */
for(tvbp = vbp->vb_link; tvbp; tvbp = tvbp->vb_link)
    testproc_good(pktp, tvbp);

return;
}

/****************************************************************************
NAME:  SA_get_end

PURPOSE:  This routine is for use with sub agents. 
          It is the end routine for performing a get function,
          it may also be used by the next functions.
          It examines the response from the sub agent, if there was
          an error it marks the packet as such and returns, otherwise
          it integrates the responses into the packet.

PARAMETERS:
        SNMP_PKT_T  *   SNMP packet currently being processsed.
        VB_T        *   Variable being processed.
        SA_HEADER_T *   header block from the sub agent
        VBL_T       *   vb list returned from the sub agent
        INT_32_T        error flag, included so we can clear the doit bits
                        before testing the flag.
RETURNS:  void

****************************************************************************/
/*ARGSUSED*/
void
  SA_get_end(SNMP_PKT_T         *pktp,
             VB_T               *vbp,
             SA_HEADER_T        *hdr,
             VBL_T              *rvblist,
             int                 tcount)
{
VB_T *tvbp, *rvbp;
INT_32_T i;

/* Mark the vbs in the chain as having their gets started & done */
for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
    getproc_started(pktp, tvbp);
    getproc_good(pktp, tvbp);
    tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    }

if (hdr->sa_error) {
    pktp->pdu.std_pdu.error_status = GEN_ERR;
    Clean_vb_list(rvblist);
    return;
    }

/* If there was a snmp error stuff it into the packet */
if (hdr->snmp_error) {
    /* Make sure the index is within range */
    if (hdr->index > tcount)
        pktp->pdu.std_pdu.error_status = GEN_ERR;
    else {
        pktp->pdu.std_pdu.error_status = hdr->snmp_error;
        /* Then translate the index back to the original range */
        for (i = hdr->index, tvbp = vbp; i; i--, tvbp = tvbp->vb_link)
            ;
        pktp->pdu.std_pdu.error_index  = vbp_to_index(pktp, tvbp) + 1;
        }
    getproc_good(pktp, vbp);
    Clean_vb_list(rvblist);
    return;
    }

/* Make sure that the number of returned vbs matches what we sent */
if (tcount != rvblist->vbl_count) {
    pktp->pdu.std_pdu.error_status = GEN_ERR;
    Clean_vb_list(rvblist);
    return;
    }

for (tvbp = vbp, rvbp = rvblist->vblist; tvbp; tvbp = tvbp->vb_link, rvbp++) {
    tvbp->vb_priv       = rvbp->vb_priv;
    tvbp->vb_free_priv  = rvbp->vb_free_priv;
    rvbp->vb_priv       = 0;
    rvbp->vb_free_priv  = 0;
    transfer_value(pktp, tvbp, rvbp);
    }
Clean_vb_list(rvblist);
return;
}

/****************************************************************************
NAME:  SA_get_mid

PURPOSE:  This routine is for use with sub agents. 
          It is the middle routine for performing a get function.
          It examines the response from the sub agent, if there was
          an error it marks the packet as such and returns, otherwise
          it attempts to ship a get request to the sub agent.

PARAMETERS:
        SNMP_PKT_T  *   SNMP packet currently being processsed.
        VB_T        *   Variable being processed.
        SA_HEADER_T *   header block from the sub agent
        VBL_T       *   vb list returned from the sub agent
        int             number of variables sent to the sub agent

RETURNS:  void

****************************************************************************/
/*ARGSUSED*/
void
  SA_get_mid(SNMP_PKT_T         *pktp,
             VB_T               *vbp,
             SA_HEADER_T        *hdr,
             VBL_T              *rvblist,
             int                 tcount)
{
VB_T *tvbp, *fvbp, *lvbp, *rvbp;
SA_GROUP_T *group;
INT_32_T i;

/* Reset the get started bits, so if we send something to the sub agent
   the work will be performed */
for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
    tvbp->vb_flags  &= ~(VFLAG_GET_STARTED | VFLAG_SA_SENDIT | VFLAG_SA_DOIT);

/* If we didn't have a sub agent error and did have an 
   snmp error but not a stuff it into the packet */
if ((hdr->sa_error == 0) && hdr->snmp_error) {
    /* Make sure the index is within range */
    if (hdr->index > tcount)
        pktp->pdu.std_pdu.error_status = GEN_ERR;
    else {
        pktp->pdu.std_pdu.error_status = hdr->snmp_error;
        /* Then translate the index back to the original range */
        if (hdr->index == 0)
            pktp->pdu.std_pdu.error_index = 0;
        else {
            /* make i 0 based, then step through the vbp lists */
            for (i = hdr->index - 1, tvbp = vbp; i; i--, tvbp = tvbp->vb_link)
                ;
            pktp->pdu.std_pdu.error_index  = vbp_to_index(pktp, tvbp) + 1;
            }
        }
    Clean_vb_list(rvblist);
    for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
        getproc_good(pktp, tvbp);
    return;
    }

/* If there was a sub agent link error or the number of returned
   vbs didn't match what we sent generate a gen err */
if ((hdr->sa_error) || (tcount != rvblist->vbl_count)) {
    getproc_error(pktp, vbp, GEN_ERR);
    Clean_vb_list(rvblist);
    for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
        getproc_good(pktp, tvbp);
    return;
    }

fvbp = vbp;
lvbp = vbp;
for (tvbp = fvbp, rvbp = rvblist->vblist; tvbp; rvbp++) {
    /* If we didn't find a vb in the sub agent mark the vb in the packet
       structure as not existing, then unlink it from the chain.
       fvbp is the first vbp in the linked list and must be updated
       if we unlink the current first vb */

    if (rvbp->vb_data_flags_n_type == VT_NOSUCHINS) {
        tcount--;
        getproc_nosuchins(pktp, tvbp);
        if (tvbp == fvbp) {
            fvbp = tvbp->vb_link;
            tvbp->vb_link = 0;
            tvbp = fvbp;
            }
        else {
            lvbp->vb_link = tvbp->vb_link;
            tvbp->vb_link = 0;
            tvbp = lvbp->vb_link;
            }
        }
    else {
        /* Copy the priv information to the packet structure */      
        tvbp->vb_priv            = rvbp->vb_priv;
        tvbp->vb_free_priv       = rvbp->vb_free_priv;
        rvbp->vb_priv            = 0;
        rvbp->vb_free_priv       = 0;
        /* move the last pointer, and the current pointer */
        lvbp = tvbp;
        tvbp = tvbp->vb_link;
        }
    }
Clean_vb_list(rvblist);

/* If an error occurred (a v1 packet got a no such back) we are done
   and can exit */
if (pktp->pdu.std_pdu.error_status) {
    for(tvbp = fvbp; tvbp; tvbp = tvbp->vb_link)
        getproc_good(pktp, tvbp);
    return;
    }

/* See if we have any vbs to get, if we don't we can return */
if (fvbp == 0)
    return;

/* reacquire the group, if we fail return a gen err */
group = SA_getgroup(fvbp->vb_ml.ml_remaining_objid.num_components,
                    fvbp->vb_ml.ml_remaining_objid.component_list,
                    (SA_COOKIE_T *)(VB_TO_COOKIE(fvbp)));
if (group == 0)
    goto getmid_error;

/* Otherwise we proceed to perform the get pass */
/* Send the get request to the sub agent */
MEMSET(hdr, 0, sizeof(SA_HEADER_T));
hdr->opcode2 = SA_QUERY_GET;

/* Build and save the state block */
if ((hdr->requestid = SA_make_cb(pktp, fvbp, tcount)) == 0)
    goto getmid_error;
  
/* set the sendit & doit flags for the chained vbs,
   and set the data_flags_n_type to keep older code happy */
for (tvbp = fvbp; tvbp; tvbp = tvbp->vb_link) {
    tvbp->vb_flags |= VFLAG_SA_SENDIT | VFLAG_SA_DOIT;
    tvbp->vb_data_flags_n_type = tvbp->vb_ml.ml_leaf->expected_tag;
    }

if (SA_do_query(hdr, &(pktp->pdu.std_pdu.std_vbl), pktp, group)) {
    for (tvbp = fvbp; tvbp; tvbp = tvbp->vb_link)
        tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    goto getmid_error;
    }

/* Go through the linked list reseting the sendit and doit flags so
   the next method routine won't send them again.  Also mark the vbs
   as having their gets started */
for(tvbp = fvbp; tvbp; tvbp = tvbp->vb_link) {
    tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    getproc_started(pktp, tvbp);
    }

return;

/* we come here if we had an error in the second half of the routine
   we need to setup the error and mark all vbs as having been done */
getmid_error:
getproc_error(pktp, vbp, GEN_ERR);
for(tvbp = fvbp; tvbp; tvbp = tvbp->vb_link)
    getproc_good(pktp, tvbp);
return;

}

/****************************************************************************
NAME:  SA_getproc

PURPOSE:  This routine is for use with sub agents. 
          It determines the correct sub agent for the given vbp,
          collects all the vbs for that sub agent and attempts
          to ship them to the specified sub agent.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T  *       Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T    *       Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  SA_getproc(OIDC_T              last_match,
             int                 compc,
             OIDC_T             *compl,
             SNMP_PKT_T         *pktp,
             VB_T               *vbp)
{
SA_COOKIE_T *cookie;
SA_GROUP_T *group;
SA_GRP_LST_T *grplst = 0;
SA_HEADER_T hdr_blk;
VB_T *tvbp, *lvbp;
VBL_T *vblist;
TREENODE_T *tnode;
int tcount, rlen;
INT_32_T i;

/* initialize some values */
MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));
hdr_blk.opcode2 = SA_QUERY_TESTGET;

vblist = &(pktp->pdu.std_pdu.std_vbl);

if ((cookie = (SA_COOKIE_T *)(VB_TO_COOKIE(vbp))) == 0) {
    getproc_error(pktp, vbp, GEN_ERR);
    return;
    }

if (cookie->flags & SA_INSTANCE_TYPE) {
    tnode = TREE_Get(cookie->instance_tree, compl, compc, &rlen);
    if ((tnode == 0) || (rlen != compc)) {
        getproc_nosuchins(pktp, vbp);
        return;
        }
    if (TREE_Extract(tnode, (PTR_T *)(&grplst))) {
        getproc_error(pktp, vbp, GEN_ERR);
        return;
        }
    group = grplst->group;
    }
else {
    group = cookie->ichain->group;
    }

/* Make sure no other method routine has inactiveated the group */
if (group->flags & SA_GROUP_INACTIVE) {
    getproc_nosuchins(pktp, vbp);
    return;
    }

lvbp = vbp;
tcount = 1;
for (tvbp = vbp + 1, i = vblist->vbl_count - (vbp_to_index(pktp, vbp) + 1);
     i > 0;
     i--, tvbp++) {
    if (tvbp->vb_flags & VFLAG_GET_STARTED)
        continue;
    if ((tvbp->vb_ml.ml_leaf->testproc == (ASY_TESTPROC_T *)SA_testproc) &&
        (SA_getgroup(tvbp->vb_ml.ml_remaining_objid.num_components,
                     tvbp->vb_ml.ml_remaining_objid.component_list,
                     (SA_COOKIE_T *)(VB_TO_COOKIE(tvbp)))
         == group)) {
        lvbp->vb_link = tvbp;
        lvbp = tvbp;
        tcount++;
        }
    }

/* Build and save the state block */
if ((hdr_blk.requestid = SA_make_cb(pktp, vbp, tcount)) == 0) {
    getproc_error(pktp, vbp, GEN_ERR);
    return;
    }
  
for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
    tvbp->vb_flags |= VFLAG_SA_SENDIT | VFLAG_SA_DOIT;

if (SA_do_query(&hdr_blk, vblist, pktp, group)) {
    for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
        tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    getproc_error(pktp, vbp, GEN_ERR);
    return;
    }

/* Go through the linked list reseting the sendit and doit flags so
   the next method routine won't send them again.  Also mark the vbs
   as having their gets started */
for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
    tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    getproc_started(pktp, tvbp);
    }

return;
}

/****************************************************************************
NAME:  SA_next_mid

PURPOSE:  This routine is for use with sub agents. 
          It is the mid routine for performing a next functions
          It examines the response from the sub agent, if there was
          an error it marks the packet as such and returns, otherwise
          it sets up the call to get the information from the subagent.

PARAMETERS:
        SNMP_PKT_T  *   SNMP packet currently being processsed.
        VB_T        *   Variable being processed.
        SA_HEADER_T *   header block from the sub agent
        VBL_T       *   vb list returned from the sub agent
        INT_32_T        error flag, included so we can clear the doit bits
                        before testing the flag.
        INT_32_T        instance flag 1 = instance type 0 = ojbect type
RETURNS:  void

****************************************************************************/
/*ARGSUSED*/
void
  SA_next_mid(SNMP_PKT_T        *pktp,
              VB_T              *vbp,
              SA_HEADER_T       *hdr,
              VBL_T             *rvblist,
              int                tcount,
              INT_32_T           instance_flag)
{
SA_GROUP_T  *group;
VB_T  *tvbp, *lvbp, *rvbp, *fvbp;
VBL_T *vblist;
INT_32_T i;
int num_comp;

/* Reset the next started bits, so if we send something to the sub agent
   the work will be performed */
for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
    tvbp->vb_flags  &= ~(VFLAG_NEXT_STARTED | VFLAG_SA_SENDIT | VFLAG_SA_DOIT);

/* If we didn't have a sub agent error and did have an 
   snmp error but not a stuff it into the packet */
if ((hdr->sa_error == 0) && hdr->snmp_error) {
    /* Make sure the index is within range */
    if (hdr->index > tcount)
        pktp->pdu.std_pdu.error_status = GEN_ERR;
    else {
        pktp->pdu.std_pdu.error_status = hdr->snmp_error;
        /* Then translate the index back to the original range */
        if (hdr->index == 0)
            pktp->pdu.std_pdu.error_index = 0;
        else {
            /* make i 0 based, then step through the vbp lists */
            for (i = hdr->index - 1, tvbp = vbp; i; i--, tvbp = tvbp->vb_link)
                ;
            pktp->pdu.std_pdu.error_index  = vbp_to_index(pktp, tvbp) + 1;
            }
        }
    Clean_vb_list(rvblist);
    for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
        nextproc_good(pktp, tvbp);
    return;
    }

/* If there was a sub agent link error or the number of returned
   vbs didn't match what we sent generate a gen err */
if ((hdr->sa_error) || (tcount != rvblist->vbl_count)) {
    nextproc_error(pktp, vbp, GEN_ERR);
    Clean_vb_list(rvblist);
    for(tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
        nextproc_good(pktp, tvbp);
    return;
    }

fvbp = vbp;
lvbp = vbp;
if (instance_flag) {
    for (tvbp = fvbp, rvbp = rvblist->vblist; tvbp; rvbp++) {
        /* If we didn't find a vb in the sub agent mark the vb in the packet
           structure as no next with a next instance flag, then unlink the vb
           from the chain.  Fvbp is the first vbp in the linked list and must
           be updated if we unlink the current first vb */
        if (rvbp->vb_data_flags_n_type == VT_NOSUCHINS) {
            nextproc_no_next(pktp, tvbp);
            tvbp->vb_flags |= VFLAG_NEXT_INSTANCE;
            tcount--;
            /* and unlink the vb from the chain */
            if (tvbp == fvbp) {
                fvbp = tvbp->vb_link;
                tvbp->vb_link = 0;
                tvbp = fvbp;
                }
            else {
                lvbp->vb_link = tvbp->vb_link;
                tvbp->vb_link = 0;
                tvbp = lvbp->vb_link;
                }
            }
        else {
            /* Copy the priv information to the packet structure */      
            tvbp->vb_priv       = rvbp->vb_priv;
            tvbp->vb_free_priv  = rvbp->vb_free_priv;
            rvbp->vb_priv       = 0;
            rvbp->vb_free_priv  = 0;
            /* move the last and current pointers */
            lvbp = tvbp;
            tvbp = tvbp->vb_link;
            }
        }
    } /* if (instance_flag) */
else {
    for (tvbp = fvbp, rvbp = rvblist->vblist; tvbp; rvbp++) {
        /* If we didn't find a vb in the sub agent mark the vb in the packet
           structure as no next, then unlink the vb from the chain.
           fvbp is the first vbp in the linked list and must be updated
           if we unlink the current first vb */

        if (rvbp->vb_data_flags_n_type == VT_ENDOFMIB) {
            nextproc_no_next(pktp, tvbp);
            tcount--;
            /* and unlink the vb from the chain */
            if (tvbp == fvbp) {
                fvbp = tvbp->vb_link;
                tvbp->vb_link = 0;
                tvbp = fvbp;
                }
            else {
                lvbp->vb_link = tvbp->vb_link;
                tvbp->vb_link = 0;
                tvbp = lvbp->vb_link;
                }
            }
        else {
            /* Copy the priv information to the packet structure */
            tvbp->vb_priv       = rvbp->vb_priv;
            tvbp->vb_free_priv  = rvbp->vb_free_priv;
            rvbp->vb_priv       = 0;
            rvbp->vb_free_priv  = 0;
            /* update objids */
            num_comp = tvbp->vb_obj_id.num_components -
                        tvbp->vb_ml.ml_remaining_objid.num_components;
            nextproc_next_instance(pktp, tvbp,
                                   rvbp->vb_obj_id.num_components - num_comp,
                                   rvbp->vb_obj_id.component_list + num_comp);
            /* move the last and current pointers */
            lvbp = tvbp;
            tvbp = tvbp->vb_link;
            }
        }
    } /* else (instance_flag) */

/* clean up the returned list */
Clean_vb_list(rvblist);

/* If an error occurred (a v1 packet got a no such back) we are done
   and can exit */
if (pktp->pdu.std_pdu.error_status) {
    for(tvbp = fvbp; tvbp; tvbp = tvbp->vb_link)
        nextproc_good(pktp, tvbp);
    return;
    }

/* See if we have any vbs to get, if we don't we can return */
if (fvbp == 0)
    return;

/* reacquire the group, if we fail return a gen err */
group = SA_getgroup(fvbp->vb_ml.ml_remaining_objid.num_components,
                    fvbp->vb_ml.ml_remaining_objid.component_list,
                    (SA_COOKIE_T *)(VB_TO_COOKIE(fvbp)));
if (group == 0)
    goto nextmid_error;

/* Now we perform the get pass, fvbp points to a linked list of vbs
   to be gotten */

/* Send the get request to the sub agent */
MEMSET(hdr, 0, sizeof(SA_HEADER_T));
hdr->opcode2 = SA_QUERY_GET;

/* Build and save the state block */
if ((hdr->requestid = SA_make_cb(pktp, fvbp, tcount)) == 0)
    goto nextmid_error;
  
/* set the sendit & doit flags for the chained vbs,
   and set the data_flags_n_type to keep older code happy */
for (tvbp = fvbp; tvbp; tvbp = tvbp->vb_link) {
    tvbp->vb_flags |= VFLAG_SA_SENDIT | VFLAG_SA_DOIT;
    tvbp->vb_data_flags_n_type = tvbp->vb_ml.ml_leaf->expected_tag;
    }

/* get the right vblist, we are always working on the last list */
for(vblist = &(pktp->pdu.std_pdu.std_vbl); vblist->vblp; vblist = vblist->vblp)
    ;

if (SA_do_query(hdr, vblist, pktp, group)) {
    for (tvbp = fvbp; tvbp; tvbp = tvbp->vb_link)
        tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    goto nextmid_error;
    }

/* Go through the linked list reseting the sendit and doit flags so
   the next method routine won't send them again.  Also mark the vbs
   as having their gets started */
for(tvbp = fvbp; tvbp; tvbp = tvbp->vb_link) {
    tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    nextproc_started(pktp, tvbp);
    }

return;

/* we come here if we had an error in the second half of the routine
   we need to setup the error and mark all vbs as having been done */
nextmid_error:
nextproc_error(pktp, vbp, GEN_ERR);
for(tvbp = fvbp; tvbp; tvbp = tvbp->vb_link)
    nextproc_good(pktp, tvbp);
return;

}

/****************************************************************************
NAME:  SA_nextproc

PURPOSE:  This routine is for use with sub agents, it collects all
          the vbs for a single sub agent and attempts to ship them
          there.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T  *       Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T    *       Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  SA_nextproc(OIDC_T             last_match,
              int                compc,
              OIDC_T            *compl,
              SNMP_PKT_T        *pktp,
              VB_T              *vbp)
{
SA_COOKIE_T *cookie, *tcookie;
SA_GROUP_T  *group;
SA_GRP_LST_T *grplst = 0;
SA_HEADER_T hdr_blk;
TREENODE_T *tnode;
VB_T  *tvbp, *lvbp;
VBL_T *vblist;
OIDC_T rlist[MAX_OID_COUNT];
int retcount, tcount;
INT_32_T i, indx;

MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));

/* get the right vblist, we are always working on the last list */
for(vblist = &(pktp->pdu.std_pdu.std_vbl); vblist->vblp; vblist = vblist->vblp)
    ;
indx = vbp - vblist->vblist;

if ((cookie = (SA_COOKIE_T *)(VB_TO_COOKIE(vbp))) == 0) {
    nextproc_error(pktp, vbp, GEN_ERR);
    return;
    }

/* Determine what ops we need to perform.
   If the object is an instance type we do a test followed by a get.
   For non-instance objects we do a next followed by a get.
   In both cases we collect any other objects for the same group with
   the same instance tagging. */

if (cookie->flags & SA_INSTANCE_TYPE) {
    /* find an instance for the given vbp */        
    tnode = TREE_GetPrev(cookie->instance_tree,
                         vbp->vb_ml.ml_remaining_objid.component_list,
                         vbp->vb_ml.ml_remaining_objid.num_components);
    while (1) {
        if ((tnode = TREE_GetNext(cookie->instance_tree, tnode)) == 0) {
            /* no instance exists, bail out */
            nextproc_no_next(pktp, vbp);
            return;
            }
        if (TREE_Extract(tnode, (PTR_T *)(&grplst)))
            continue;
        if ((retcount = TREE_Name(tnode, rlist, MAX_OID_COUNT)) == 0)
            continue;

        group = grplst->group;
        if (group->flags & SA_GROUP_INACTIVE)
            continue;

        nextproc_next_instance(pktp, vbp, retcount, rlist);
        
        break;
        }

    lvbp = vbp;

    /* Find other sub agent instance objects with the same group */
    tcount = 1;
    for (tvbp = vbp + 1, i = vblist->vbl_count - (indx + 1);
         i > 0;
         i--, tvbp++) {
        if (tvbp->vb_flags & VFLAG_NEXT_STARTED)
            continue;
        if (tvbp->vb_ml.ml_leaf->testproc != (ASY_TESTPROC_T *)SA_testproc)
            continue;

        tcookie = (SA_COOKIE_T *)(VB_TO_COOKIE(tvbp));
        if (tcookie == 0) {
            nextproc_error(pktp, vbp, GEN_ERR);
            return;
            }

        if (!(tcookie->flags & SA_INSTANCE_TYPE))
            continue;

        /* find an instance for tvbp */
        tnode = TREE_GetPrev(tcookie->instance_tree,
                             tvbp->vb_ml.ml_remaining_objid.component_list,
                             tvbp->vb_ml.ml_remaining_objid.num_components);
        while (1) {
            if ((tnode = TREE_GetNext(tcookie->instance_tree, tnode)) == 0) {
                /* no instance exists, have this var bind redone */
                nextproc_no_next(pktp, tvbp);
                break;
                }
            if (TREE_Extract(tnode, (PTR_T *)(&grplst)))
                continue;
            if ((retcount = TREE_Name(tnode, rlist, MAX_OID_COUNT)) == 0)
                continue;
            if (grplst->group != group)
                break;

            /* if we made it here we have an instance type for the same
               group so we add it to the vb_link list */
            lvbp->vb_link = tvbp;
            lvbp = tvbp;

            /* save the instance information */
            nextproc_next_instance(pktp, tvbp, retcount, rlist);

            tcount++;
            break;
            } /* while (1) */
        } /* for (tvbp = vbp + 1, ... */

    hdr_blk.opcode2 = SA_QUERY_TESTGET;

    /* Build and save the state block */
    if ((hdr_blk.requestid = SA_make_cb(pktp, vbp, tcount)) == 0) {
        nextproc_error(pktp, vbp, GEN_ERR);       
        return;
        }
  
    /* mark the var binds as needing to be sent to the sub
       agent for processing */
    for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
        tvbp->vb_flags |= VFLAG_SA_SENDIT | VFLAG_SA_DOIT;
    
    /* Send the query to the sub agent, if the query wasn't
       successful we mark the start vbp as requiring another try
       as we haven't accepted any others they will be retried anyway. */
    if (SA_do_query(&hdr_blk, vblist, pktp, group)) {
        nextproc_no_next(pktp, vbp);
        vbp->vb_flags |= VFLAG_NEXT_INSTANCE;
        for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
            tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
            }
        return;
        }

    for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
        tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
        nextproc_started(pktp, tvbp);
        }
    } /* if (cookie->flags & SA_INSTANCE_TYPE) */

else {
    group = cookie->ichain->group;
    if (group->flags & SA_GROUP_INACTIVE) {
        nextproc_no_next(pktp, vbp);
        return;
        }

    lvbp = vbp;

    /* Find other sub agent objects with the same group */
    tcount = 1;
    for (tvbp = vbp + 1, i = vblist->vbl_count - (indx + 1);
         i > 0;
         i--, tvbp++) {
        if (tvbp->vb_flags & VFLAG_NEXT_STARTED)
            continue;
        if (tvbp->vb_ml.ml_leaf->testproc != (ASY_TESTPROC_T *)SA_testproc)
            continue;

        tcookie = (SA_COOKIE_T *)(VB_TO_COOKIE(tvbp));
        if (tcookie == 0) {
            nextproc_error(pktp, tvbp, GEN_ERR);
            return;
            }

        if ((tcookie->flags & SA_INSTANCE_TYPE) ||      
            (group != tcookie->ichain->group))
            continue;

        lvbp->vb_link = tvbp;
        lvbp = tvbp;

        tcount++;
        }

    hdr_blk.opcode2 = SA_QUERY_NEXT;

    /* Build and save the state block */
    if ((hdr_blk.requestid = SA_make_cb(pktp, vbp, tcount)) == 0) {
        nextproc_error(pktp, vbp, GEN_ERR);        
        return;
        }
  
    /* mark the var binds as needing to be sent to the sub
       agent for processing */
    for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
        tvbp->vb_flags |= VFLAG_SA_SENDIT | VFLAG_SA_DOIT;
    
    /* Send the query to the sub agent, if the query wasn't
       successful set up the start vbp for a next */

    if (SA_do_query(&hdr_blk, vblist, pktp, group)) {
        for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
            tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
        nextproc_no_next(pktp, vbp);
        return;
        }

    for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
        tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
        nextproc_started(pktp, tvbp);
        }
    }
return;
}

/****************************************************************************
NAME:  SA_set_end

PURPOSE:  This routine is for use with sub agents. 
          It is the end routine for performing a get function,
          It examines the response from the sub agent, if there was
          an error it marks the packet as such and returns, otherwise
          it integrates the responses into the packet.

PARAMETERS:
        SNMP_PKT_T  *   SNMP packet currently being processsed.
        VB_T        *   Variable being processed.
        SA_HEADER_T *   header block from the sub agent
        VBL_T       *   vb list returned from the sub agent
        INT_32_T        error flag, included so we can clear the doit bits
                        before testing the flag.
RETURNS:  void

****************************************************************************/
/*ARGSUSED*/
void
  SA_set_end(SNMP_PKT_T         *pktp,
             VB_T               *vbp,
             SA_HEADER_T        *hdr,
             VBL_T              *rvblist,
             int                 tcount)
{
VB_T *tvbp, *rvbp, *trvbp;

/* Tag the vbp as having been done, so we don't have to do so later */
setproc_good(pktp, vbp);

/* If there was a sub agent link error generate an undo failed */
if (hdr->sa_error) {
    setproc_error(pktp, vbp, UNDO_FAILED);
    setproc_all_bits(pktp, vbp);
    Clean_vb_list(rvblist);
    return;
    }

/* If there was a snmp error stuff it into the packet, and
   clean the vb pointers as the sub agent should already
   have cleaned them up */
if (hdr->snmp_error) {
    pktp->pdu.std_pdu.error_status = hdr->snmp_error;
    pktp->pdu.std_pdu.error_index  = hdr->index;
    setproc_all_bits(pktp, vbp);
    for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
        tvbp->vb_priv = 0;
        tvbp->vb_free_priv = 0;
        }
    Clean_vb_list(rvblist);
    return;
    }

/* As we sent all of the vbs down they all better have come back */
if (pktp->pdu.std_pdu.std_vbl.vbl_count != rvblist->vbl_count) {
    pktp->pdu.std_pdu.error_status = GEN_ERR;
    Clean_vb_list(rvblist);
    return;
    }
    
rvbp = rvblist->vblist;
for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link) {
    trvbp = &rvbp[vbp_to_index(pktp, tvbp)];
    tvbp->vb_flags      |= trvbp->vb_flags;
    tvbp->vb_priv        = trvbp->vb_priv;
    tvbp->vb_free_priv   = rvbp->vb_free_priv;
    trvbp->vb_priv       = 0;
    trvbp->vb_free_priv  = 0;
    }
Clean_vb_list(rvblist);
}

/****************************************************************************
NAME:  SA_setproc

PURPOSE:  This routine is for use with sub agents, it collects all
          the vbs for a single sub agent and attempts to ship them
          there.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T  *       Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T    *       Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  SA_setproc(OIDC_T              last_match,
             int                 compc,
             OIDC_T             *compl,
             SNMP_PKT_T         *pktp,
             VB_T               *vbp)
{
SA_GROUP_T *group;
VB_T *tvbp;
VBL_T *vblist;
int i;
SA_HEADER_T hdr_blk;

MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));
hdr_blk.opcode2 = SA_QUERY_SET;
hdr_blk.index   = vbp_to_index(pktp, vbp);

vblist = &(pktp->pdu.std_pdu.std_vbl);

group = SA_getgroup(compc, compl, (SA_COOKIE_T *)(VB_TO_COOKIE(vbp)));
if (group == 0) {
    setproc_error(pktp, vbp, COMMIT_FAILED);
    return;
    }

/* Build and save the state block */
if ((hdr_blk.requestid = SA_make_cb(pktp, vbp, 0)) == 0) {
    setproc_error(pktp, vbp, GEN_ERR);
    return;
    }
  
for (tvbp = vblist->vblist, i = vblist->vbl_count; i; i--, tvbp++)
    tvbp->vb_flags |= VFLAG_SA_SENDIT;
for (tvbp = vbp; tvbp; tvbp = tvbp->vb_link)
    tvbp->vb_flags |= VFLAG_SA_DOIT;

if (SA_do_query(&hdr_blk, vblist, pktp, group)) {
    for (tvbp = vblist->vblist, i = vblist->vbl_count; i; i--, tvbp++)
        tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);
    setproc_error(pktp, vbp, GEN_ERR);
    return;
    }

for (tvbp = vblist->vblist, i = vblist->vbl_count; i; i--, tvbp++)
    tvbp->vb_flags &= ~(VFLAG_SA_SENDIT | VFLAG_SA_DOIT);

/* mark all but the first vbp as having been set, when we
   get a reply back from the sub agent we can either mark the
   first one as done and processing will continue or we can
   mark an error and processing will finish up */
for(tvbp = vbp->vb_link; tvbp; tvbp = tvbp->vb_link)
    setproc_all_bits(pktp, tvbp);

return;
}

/****************************************************************************
NAME:  SA_Register_Nodes

PURPOSE:  Given a vb list attempt to register all the named nodes.
          As a side effect this routine may remove some inactive nodes
          whether the routine itself succeds or fails.
          
PARAMETERS:
        VBL_T      *    List of vbs representing nodes to be registered.
        SA_GROUP_T *    Group structure for use with this set of nodes
        MIBNODE_T  *    Root node to use

RETURNS:  0             Success
          SA_NODE_ERROR         A problem was found with one or more nodes.
          SA_MEMORY_ERROR       Unable to allocate memory.
          
****************************************************************************/
INT_32_T
  SA_Register_Nodes(VBL_T      *vblist,
                    SA_GROUP_T *group,
                    MIBNODE_T  *mib_root)
{
INT_32_T i, j, errflg = 0;
VB_T *vbp;
MIBNODE_T *nodep;
MIBLEAF_T *leafp;
SA_GRP_LST_T *grplst, *prevgrplst;
SA_COOKIE_T *cookie;
INT_32_T error_ret = SA_MEMORY_ERROR;

#define N_TYPE LEAF_NODE | REMOVABLE | LEAF_DYNAMIC | COOKIE_DYNAMIC

/* walk through the vb list doing a find on each objid, if we find
   a node we check it out, if it is an inner node or if it wasn't created
   by this routine we flag it as an error.  If the node doesn't allow
   instances we check it's ipc link, if the ipc link is ok we flag it
   as an error, if the node does allow instances but the new node doesn't
   we check the ipc links of all the groups associated with the node
   and flag an error if any of them are still active */

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SA_register_nodes: infrastructure lock is broken", 0));
    return(SA_LOCK_ERROR);
    }
#endif

for(i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++) {
    j = Find_Node_From_Root(mib_root, &(vbp->vb_obj_id), &nodep);
    if (j == 2)         /* no node found */
        continue;
    if (j == 1) {       /* leaf node found before end of object id */
        vbp->value_u.v_uinteger32 = SA_POINTS_THROUGH_LEAF;
        errflg = 1;
        continue;
        }
    if ((nodep->node_type & NODE_TYPE) == INNER_NODE) {
        vbp->value_u.v_uinteger32 = SA_NODE_IS_INNER;
        errflg = 1;
        continue;
        }
    leafp = (MIBLEAF_T *)nodep;
    if (leafp->testproc != (ASY_TESTPROC_T *)SA_testproc) {
        vbp->value_u.v_uinteger32 = SA_NODE_NOT_SA;
        errflg = 1;
        continue;
        }
    /* the leaf is a sub agent type, if it is inactive we try to remove it
       if it is active we check to see if the vbp and leaf are both
       instance type objects.  If the leaf is active and the two objects
       are instance types we flag an error */
    if (leaf_active(leafp) == 0) {
        leafp = (MIBLEAF_T *)Remove_Node_From_Root(mib_root, 
                                                   &(vbp->vb_obj_id));
        if (leafp != 0) {
            if (leafp->user_cookie != 0)
                SNMP_memory_free(leafp->user_cookie);
            SNMP_memory_free(leafp);
            }
        continue;
        }

    if ((((SA_COOKIE_T *)leafp->user_cookie)->flags & SA_INSTANCE_TYPE) &&
        (vbp->sa_flags & SA_INSTANCE_TYPE)) {
        if (leafp->expected_tag != vbp->vb_data_flags_n_type) {
            vbp->value_u.v_uinteger32 = SA_NODE_TYPE_MISMATCH;
            errflg = 1;
            }
        else
            vbp->sa_flags |= SA_INSTANCE_TYPE_EXISTS;
        vbp->vb_priv = (PTR_T)leafp;
        continue;
        }

    vbp->value_u.v_uinteger32 = SA_NODE_ACTIVE;
    vbp->vb_priv = (PTR_T)leafp;
    errflg = 1;
    }

if (errflg) {
    /* if we found an active leaf, we set a bit in the cookie to indicate
       that the cookies group was active, we need to reset such bits here */
    for(i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++)
        if (vbp->vb_priv != 0) {
            leafp = (MIBLEAF_T *)(vbp->vb_priv);
            for (grplst = ((SA_COOKIE_T *)(leafp->user_cookie))->ichain;
                 grplst != 0;
                 grplst = grplst->ichain)
                grplst->group->flags &= ~SA_GROUP_ACTIVE;
            vbp->vb_priv = 0;
            }

    ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
    return(SA_NODE_ERROR);
    }

/* allocate the group list structures as well as any leaf and cookie
   structures that don't already exist.  If anything can't be allocated
   free everything we have already allocated */
for(i = 0, vbp = vblist->vblist, prevgrplst = 0;
    i < vblist->vbl_count;
    i++, vbp++, prevgrplst = grplst) {
    /* if the instance type exists flag isn't set we need to
       allocate and fill in the leaf and cookie structures */
    if (!(vbp->sa_flags & SA_INSTANCE_TYPE_EXISTS)) {
        leafp = (MIBLEAF_T *)SNMP_memory_alloc(sizeof(MIBLEAF_T));
        if (leafp == 0) {
            /* couldn't allocate a leaf, free up any already allocated space */
            vbp->value_u.v_uinteger32 = SA_GENERR;
vb_free:    vbp = vblist->vblist;
            for(; i > 0; i--, vbp++) {
                if (vbp->sa_flags & SA_INSTANCE_TYPE_EXISTS)
                    SNMP_memory_free(vbp->vb_priv);
                else {
                    grplst = (SA_GRP_LST_T *)vbp->vb_priv;
                    SNMP_memory_free(grplst->leaf->user_cookie);
                    SNMP_memory_free(grplst->leaf);
                    SNMP_memory_free(grplst);
                    }
                }
            for(vbp = vblist->vblist, i = vblist->vbl_count; i > 0; i--, vbp++)
                vbp->vb_priv = 0;

            ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
            return(error_ret);
            }

        cookie = (SA_COOKIE_T *)SNMP_memory_alloc(sizeof(SA_COOKIE_T));
        if (cookie == 0) {
            /* free the leaf and then hop up to vb_free to clean up the
               vblist */
            SNMP_memory_free(leafp);
            vbp->value_u.v_uinteger32 = SA_GENERR;
            goto vb_free;
            }

        /* fill in the cookie and leaf structures */
        cookie->ichain        = 0;
        cookie->instance_tree = 0;
        cookie->flags         = vbp->sa_flags;

        leafp->node_type    = LEAF_NODE | REMOVABLE | LEAF_DYNAMIC | COOKIE_DYNAMIC;
        leafp->expected_tag = (OCTET_T)(vbp->vb_data_flags_n_type & 0xFF);
        leafp->access_type  = (OCTET_T)(vbp->vb_access & 0xFF);
        leafp->testproc     = (ASY_TESTPROC_T *)SA_testproc;
        leafp->nextproc     = (ASY_NEXTPROC_T *)SA_nextproc;
        leafp->getproc      = (ASY_GETPROC_T *)SA_getproc;
        leafp->setproc      = (ASY_SETPROC_T *)SA_setproc;
        leafp->user_cookie  = (char *)cookie;
        leafp->locator      = 0;
        leafp->view_mask    = 0;
        leafp->write_mask   = 0;

        switch (leafp->expected_tag) {
            case VT_NUMBER:
            case VT_COUNTER:
            case VT_GAUGE:
            case VT_TIMETICKS:
            case VT_UINTEGER32:
            case VT_STRING:
            case VT_OPAQUE:
            case VT_OBJECT:
            case VT_EMPTY:
            case VT_IPADDRESS:
/* We only include the following case if v2 types are installed */
#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES)
            case VT_COUNTER64:
#endif /* (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_USE_V2_TYPES) */
                break;
            default:
                /* clean up the cookie and leaf and jump to vb_free */
                SNMP_memory_free(leafp);
                SNMP_memory_free(cookie);
                vbp->value_u.v_uinteger32 = SA_UNSUPPORTED_TYPE;
                error_ret = SA_UNSUPPORTED_TYPE;
                goto vb_free;
            }
        }
    else {  /* instance type does exist set leafp so we can use it */
        leafp = (MIBLEAF_T *)vbp->vb_priv;
        cookie = (SA_COOKIE_T *)leafp->user_cookie;
        }

    grplst = (SA_GRP_LST_T *)SNMP_memory_alloc(sizeof(SA_GRP_LST_T));
    if (grplst == 0) {
        if (!(vbp->sa_flags & SA_INSTANCE_TYPE_EXISTS)) {
            SNMP_memory_free(leafp);
            SNMP_memory_free(cookie);
            }
        vbp->value_u.v_uinteger32 = SA_GENERR;
        goto vb_free;
        }
    grplst->leaf     = leafp;
    grplst->group    = group;
    grplst->gchain   = prevgrplst;
    grplst->ichain   = cookie->ichain;
    grplst->instance = 0;
    vbp->vb_priv     = (PTR_T)grplst;
    }

/* now we circularize the group lists by pointing the first grplst back to
   the last grplst, and point the group to the group list */

grplst = (SA_GRP_LST_T *)(vblist->vblist->vb_priv);
grplst->gchain = prevgrplst;
group->group_list = grplst;

/* so we have all the nodes built now we start adding them,
   we don't have to worry about dealing with nodes we are replacing
   as we have already removed any nodes we would be replacing. */

for (i = 0, vbp = vblist->vblist; i < vblist->vbl_count; i++, vbp++) {
    grplst = (SA_GRP_LST_T *)vbp->vb_priv;
    /* hook the grp list to the chain */
    ((SA_COOKIE_T *)grplst->leaf->user_cookie)->ichain = grplst;
    /* if the instance doesn't already exist we need to add the node to the
       tree, otherwise we are done */
    if (!(vbp->sa_flags & SA_INSTANCE_TYPE_EXISTS))
        if (Add_Node_From_Root(mib_root, &(vbp->vb_obj_id),
                               (MIBNODE_T *)(grplst->leaf), &nodep)) {
            vbp->value_u.v_counter = SA_GENERR;
            errflg = 1;
            break;
            }
    }

/* add the group to the handle list */
if ((errflg) || (hand_add_group((SA_GROUP_T *)group))) {
    /* we were unable to add the group to the handle list, need to
       remove all the objects we registered and free the nodes and groups */
    for (vbp = vblist->vblist; i > 0; i--, vbp++) 
        if (vbp->sa_flags & SA_INSTANCE_TYPE_EXISTS) {
            grplst = (SA_GRP_LST_T *)vbp->vb_priv;
            cookie = (SA_COOKIE_T *)grplst->leaf->user_cookie;
            cookie->ichain = cookie->ichain->ichain;
            }
        else
            (void) Remove_Node_From_Root(mib_root, &(vbp->vb_obj_id));
    
    for(vbp = vblist->vblist, i = vblist->vbl_count; i > 0; i--, vbp++) {
        if (vbp->sa_flags & SA_INSTANCE_TYPE_EXISTS)
            SNMP_memory_free(vbp->vb_priv);
        else {
            grplst = (SA_GRP_LST_T *)vbp->vb_priv;
            SNMP_memory_free(grplst->leaf->user_cookie);
            SNMP_memory_free(grplst->leaf);
            SNMP_memory_free(grplst);
            }
        vbp->vb_priv = 0;
        }

    ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
    return(SA_MEMORY_ERROR);
    }

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
return(0);
}

/****************************************************************************
NAME:  SA_Deregister_Nodes

PURPOSE:  Given a vb list attempt to deregister all the named nodes.

PARAMETERS:
        VBL_T      *    List of vbs representing nodes to be registered.
        INT_32_T        Number of nodes to remove (we may not need to
                        remove all the nodes on the list)
        SA_GROUP_T *    Group structure for group list removal.
        MIBNODE_T  *    Root node to use

RETURNS:  0     Success
          1     Failure
          
          All nodes will be found and tested for removabliity before
          any are removed.  We attempt to remove all or none.

****************************************************************************/
INT_32_T
  SA_Deregister_Nodes(VBL_T      *vblist,
                      INT_32_T    count,
                      SA_GROUP_T *group,
                      MIBNODE_T  *mib_root)
{
INT_32_T i, j, errflg = 0;
VB_T *vbp;
MIBNODE_T *nodep;
MIBLEAF_T *leafp;
SA_GRP_LST_T *grplst, *tgrplst;

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SA_Deregister_Nodes: infrastructure lock is broken", 0));

    return(SA_LOCK_ERROR);
    }
#endif

/* for each vb do a find, if the node doesn't exist or isn't removable set 
   the correct bit in the flags word, and set the error flag.  If we make
   it through the whole list without any errors we start removing nodes */
if (count > vblist->vbl_count)
    count = vblist->vbl_count;

for(i = count, vbp = vblist->vblist; i > 0; i--, vbp++) {
    j = Find_Node_From_Root(mib_root, &(vbp->vb_obj_id), &nodep);
    if (j == 2) {       /* no node found */
        vbp->value_u.v_counter = SA_NO_SUCH_NODE;
        errflg = 1;
        continue;
        }
    if (j == 1) {       /* leaf node found before end of object id */
        vbp->value_u.v_counter = SA_POINTS_THROUGH_LEAF;
        errflg = 1;
        continue;
        }
    if ((nodep->node_type & NODE_TYPE) == INNER_NODE) {
        vbp->value_u.v_counter = SA_NODE_IS_INNER;
        errflg = 1;
        continue;
        }
    leafp = (MIBLEAF_T *)nodep;
    if (leafp->testproc != (ASY_TESTPROC_T *)SA_testproc) {
        vbp->value_u.v_counter = SA_NODE_NOT_SA;
        errflg = 1;
        continue;
        }
    /* The leaf is a sub agent type, make sure it is removable */
    if (!(nodep->node_type & REMOVABLE)) {
        vbp->value_u.v_counter = SA_GENERR;
        errflg = 1;
        }
    /* now we know we can remove it, save a copy of the pointer for later */
    vbp->vb_priv = (PTR_T)nodep;
    }

if (errflg) {
    for(i = count, vbp = vblist->vblist; i > 0; i--, vbp++)
        vbp->vb_priv = 0;
    ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
    return(SA_NODE_ERROR);
    }

for(i = count, vbp = vblist->vblist; i > 0; i--, vbp++) {
    /* we get the pointer back and if it has groups see if ours
       is one of them, if so we check if this is the last entry in
       the group list circle if so we remove the group and handle entries.
       Else we seperate this nodes group list entry from the group list
       circle adjusting the group point if necessary and run remove_grp_lst
       on the group list entry.  After that we check the leaf and if
       it is inactive we do a remove_node and free it. */
    leafp = (MIBLEAF_T *)vbp->vb_priv;
    if (leafp->user_cookie != 0)
        for (grplst = ((SA_COOKIE_T *)(leafp->user_cookie))->ichain;
             grplst != 0;
             grplst = grplst->ichain)
            if (grplst->group == group) {
                if (grplst->gchain == grplst) {
                    hand_remove_group(group);
                    group->ipcfree(group->ipchandle);
                    SNMP_memory_free(group);
                    }
                else {
                    for(tgrplst = grplst->gchain;
                        tgrplst->gchain != grplst;
                        tgrplst = tgrplst->gchain)
                        ;
                    if (group->group_list == grplst)
                        group->group_list = grplst->gchain;
                    tgrplst->gchain = grplst->gchain;
                    }
                remove_grp_lst(grplst);
                break;
                }
    /* We check the leaf inactive bit, if it is set (which we may have
       done in the last step if we removed it's last group list)
       we remove the node and free the space */
    if (leafp->node_type & LEAF_INACTIVE) {
        leafp = (MIBLEAF_T *)Remove_Node_From_Root(mib_root,
                                                   &(vbp->vb_obj_id));
        if (leafp->user_cookie != 0)
            SNMP_memory_free(leafp->user_cookie);
        SNMP_memory_free(leafp);
        }
    }

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
return(0);
}


/****************************************************************************
NAME:  SA_Register_Instances

PURPOSE:  Given a vb list attempt to register all the named instances.
          The list should be arranged as an object name with a bit
          set in the flags_and_errors word followed by a set of instances
          followed by another object name with the bit set and so on.
          
PARAMETERS:
        VBL_T      *    List of vbs representing instances to be registered.
        SA_GROUP_T *    Group structure for use with this set of instances
        MIBNODE_T  *    Root node to use

RETURNS:  0             Success
          SA_NODE_ERROR         A problem was found with one or more instances
          SA_MEMORY_ERROR       Unable to allocate memory.
          
****************************************************************************/
INT_32_T
  SA_Register_Instances(VBL_T      *vblist,
                        SA_GROUP_T *group,
                        MIBNODE_T  *mib_root)
{
INT_32_T i, j, errflg = 0, skipflg = 0;
int nc, nc_obj = 0;
VB_T *vbp;
MIBNODE_T *nodep;
MIBLEAF_T *leafp;
SA_GRP_LST_T *grplst = 0, *instance;
SA_COOKIE_T *cookie = 0;
TREENODE_T *treenode;

/* If vbp is an object leaf, find it, it must exist, be tagged as an
   instance type in the cookie flags and this group must be in it's list
   Set up the grp lst and group information.
   If vbp isn't an object leaf it must be an instance, see if this
   instance is already registered, in which case see if it is active,
   if it is active flag an error and continue onward.  If an instance
   exists but isn't active run mark_group_inactive which will get rid
   of it. */
/* make sure the first vbp is an object type */
vbp = vblist->vblist;
if (!(vbp->sa_flags & SA_OBJECT_TYPE)) {
    vbp->value_u.v_counter = SA_NODE_TYPE_MISMATCH;
    skipflg = 1;
    errflg = 1;
    }

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SA_Register_Instances: infrastructure lock is broken", 0));

    return(SA_LOCK_ERROR);
    }
#endif

for(i = vblist->vbl_count; i > 0; i--, vbp++)
    if (vbp->sa_flags & SA_OBJECT_TYPE) {
        skipflg = 0;
        nc_obj = vbp->vb_obj_id.num_components;
        if (Find_Node_From_Root(mib_root, &(vbp->vb_obj_id), &nodep)) {
            vbp->value_u.v_counter = SA_NO_SUCH_NODE;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        if ((nodep->node_type & NODE_TYPE) == INNER_NODE) {
            vbp->value_u.v_counter = SA_NODE_IS_INNER;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        leafp = (MIBLEAF_T *)nodep;
        if (leafp->testproc != (ASY_TESTPROC_T *)SA_testproc) {
            vbp->value_u.v_counter = SA_NODE_NOT_SA;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        cookie = (SA_COOKIE_T *)leafp->user_cookie;
        if (!(cookie->flags & SA_INSTANCE_TYPE)) {
            vbp->value_u.v_counter = SA_NODE_TYPE_MISMATCH;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        for (grplst = cookie->ichain;
             grplst != 0;
             grplst = grplst->ichain)
            if (grplst->group == group)
                break;
        if (grplst == 0) {
            vbp->value_u.v_counter = SA_NODE_TYPE_MISMATCH;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        vbp->vb_priv = (PTR_T)grplst;
        }
    else {
        /* if the leaf ojbect had an error we skip all of the instances
           associated with it */
        if (skipflg)
            continue;
        treenode = TREE_Get(cookie->instance_tree,
                            vbp->vb_obj_id.component_list + nc_obj,
                            vbp->vb_obj_id.num_components - nc_obj, &nc);
        if ((treenode == 0) || (vbp->vb_obj_id.num_components - nc_obj != nc))
            continue;
        (void)TREE_Extract(treenode, (PTR_T *)(&instance));
        if (instance->group->ipcayt(instance->group->ipchandle)) {
            vbp->value_u.v_counter = SA_NODE_ACTIVE;
            errflg = 1;
            continue;
            }
        }

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);

/* so did we have any errors? */
if (errflg) {
    for(i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++)
        vbp->vb_priv = 0;
    return(SA_NODE_ERROR);
    }

for(i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++)
    if (vbp->sa_flags & SA_OBJECT_TYPE) {
        nc_obj = vbp->vb_obj_id.num_components;
        grplst = (SA_GRP_LST_T *)vbp->vb_priv;
        cookie = (SA_COOKIE_T *)grplst->leaf->user_cookie;
        }
    else {
        instance = (SA_GRP_LST_T *)SNMP_memory_alloc(sizeof(SA_GRP_LST_T));
        if (instance == 0) {
            errflg = 1;
            break;
            }
        instance->group = group;
        instance->leaf  = grplst->leaf;
        instance->instance = 0;
        if (TREE_Add(&(cookie->instance_tree),
                     vbp->vb_obj_id.component_list + nc_obj,
                     vbp->vb_obj_id.num_components - nc_obj,
                     (PTR_T)instance, (TREENODE_T **)(&(instance->leaf)))) {
            errflg = 1;
            SNMP_memory_free(instance);
            break;
            }
        vbp->vb_priv = (PTR_T)instance;
        }

if (errflg) {
    for(j = vblist->vbl_count - i, vbp = vblist->vblist; j > 0; j--, vbp++) {
        if (vbp->sa_flags & SA_OBJECT_TYPE) {
            grplst = (SA_GRP_LST_T *)vbp->vb_priv;
            cookie = (SA_COOKIE_T *)grplst->leaf->user_cookie;
            }
        else {
            instance = (SA_GRP_LST_T *)vbp->vb_priv;
            (void) TREE_Delete(&(cookie->instance_tree), 
                               (TREENODE_T *)instance->leaf);
            SNMP_memory_free(instance);
            }
        vbp->vb_priv = 0;
        }
    for(;i > 0; i--, vbp++)
        vbp->vb_priv = 0;
    return(SA_MEMORY_ERROR);
    }

for(i = vblist->vbl_count, vbp = vblist->vblist; i > 0; i--, vbp++) {
    if (vbp->sa_flags & SA_OBJECT_TYPE)
        grplst = (SA_GRP_LST_T *)vbp->vb_priv;
    else {
        instance = (SA_GRP_LST_T *)vbp->vb_priv;
        if (grplst->instance == 0) {
            instance->instance = instance;
            grplst->instance = instance;
            }
        else {
            instance->instance = grplst->instance->instance;
            grplst->instance->instance = instance;
            }
        }
    vbp->vb_priv = 0;
    }
return(0);
}

/****************************************************************************
NAME:  SA_Deregister_Instances

PURPOSE:  Given a vb list attempt to deregister all the named instances
          
PARAMETERS:
        VBL_T      *    List of vbs representing instances to be deregistered.
        INT_32_T        Number of instances to remove (we may not need to
                        remove all the instances on the list)
        SA_GROUP_T *    Group structure for use with this set of instances
        MIBNODE_T  *    Root node to use

RETURNS:  0             Success
          SA_NODE_ERROR         A problem was found with one or more instances
          
****************************************************************************/
INT_32_T
  SA_Deregister_Instances(VBL_T      *vblist,
                          INT_32_T    count,    
                          SA_GROUP_T *group,
                          MIBNODE_T  *mib_root)
{
INT_32_T i, errflg = 0, skipflg = 0;
int nc, nc_obj = 0;
VB_T *vbp;
MIBNODE_T *nodep;
MIBLEAF_T *leafp;
SA_GRP_LST_T *grplst = 0, *instance, *tinstance;
SA_COOKIE_T *cookie = 0;
TREENODE_T *treenode;

/* First we make sure that all the instances exist and are removable
   find the leaf node, make sure it is a sub agent and instance type
   then make sure the instances exist and are associated with the 
   given group */
/* make sure the first vbp is an object type */
vbp = vblist->vblist;
if (!(vbp->sa_flags & SA_OBJECT_TYPE)) {
    vbp->value_u.v_counter = SA_NODE_TYPE_MISMATCH;
    skipflg = 1;
    errflg = 1;
    }

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SA_Deregister_Instances: infrastructure lock is broken",0));

    return(SA_LOCK_ERROR);
    }
#endif

if (count > vblist->vbl_count)
    count = vblist->vbl_count;

for(i = count; i > 0; i--, vbp++) {
    if (vbp->sa_flags & SA_OBJECT_TYPE) {
        skipflg = 0;
        nc_obj = vbp->vb_obj_id.num_components;
        if (Find_Node_From_Root(mib_root, &(vbp->vb_obj_id), &nodep)) {
            vbp->value_u.v_counter = SA_NO_SUCH_NODE;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        if ((nodep->node_type & NODE_TYPE) == INNER_NODE) {
            vbp->value_u.v_counter = SA_NODE_IS_INNER;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        leafp = (MIBLEAF_T *)nodep;
        if (leafp->testproc != (ASY_TESTPROC_T *)SA_testproc) {
            vbp->value_u.v_counter = SA_NODE_NOT_SA;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        cookie = (SA_COOKIE_T *)leafp->user_cookie;
        if (!(cookie->flags & SA_INSTANCE_TYPE)) {
            vbp->value_u.v_counter = SA_NODE_TYPE_MISMATCH;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        for (grplst = cookie->ichain; grplst != 0; grplst = grplst->ichain)
            if (grplst->group == group)
                break;
        if (grplst == 0) {
            vbp->value_u.v_counter = SA_NODE_TYPE_MISMATCH;
            skipflg = 1;
            errflg = 1;
            continue;
            }
        vbp->vb_priv = (PTR_T)grplst;
        }
    else {
        /* if the leaf ojbect had an error we skip all of the instances
           associated with it */
        if (skipflg)
            continue;
        treenode = TREE_Get(cookie->instance_tree,
                            vbp->vb_obj_id.component_list + nc_obj,
                            vbp->vb_obj_id.num_components - nc_obj, &nc);
        if ((treenode == 0) || (vbp->vb_obj_id.num_components - nc_obj != nc)){
            vbp->value_u.v_counter = SA_NO_SUCH_NODE;
            errflg = 1;
            continue;
            }
        (void)TREE_Extract(treenode, (PTR_T *)(&instance));
        if (instance->group != group) {
            vbp->value_u.v_counter = SA_NOT_OWNER;
            errflg = 1;
            continue;
            }
        vbp->vb_priv = (PTR_T)instance;
        }
    }

ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);

/* so did we have any errors? */
if (errflg) {
    for(i = count, vbp = vblist->vblist; i > 0; i--, vbp++)
        vbp->vb_priv = 0;
    return(SA_NODE_ERROR);
    }

for(i = count, vbp = vblist->vblist; i > 0; i--, vbp++)
    if (vbp->sa_flags & SA_OBJECT_TYPE) {
        grplst = (SA_GRP_LST_T *)vbp->vb_priv;
        cookie = (SA_COOKIE_T *)grplst->leaf->user_cookie;
        vbp->vb_priv = 0;
        }
    else {
        instance = (SA_GRP_LST_T *)vbp->vb_priv;
        (void) TREE_Delete(&(cookie->instance_tree),
                           (TREENODE_T *)instance->leaf);
        if (instance->instance == instance)
            grplst->instance = 0;
        else {
            if (grplst->instance == instance)
                grplst->instance = instance->instance;
            for (tinstance = grplst->instance;
                 tinstance->instance != instance;
                 tinstance = tinstance->instance)
                ;
            tinstance->instance = instance->instance;
            }
        SNMP_memory_free(instance);
        vbp->vb_priv = 0;
        }
return(0);
}

INT_32_T
  sa_asy_send(EBUFFER_T *buf,
              PTR_T      handle,
              UINT_16_T  reqid)
{
SA_IPC_COMP_T *ipc_cb = (SA_IPC_COMP_T *)handle;
INT_32_T ecode;
EBUFFER_T rbuf;
UINT_16_T qecode;

/* run the send routine, check for errors,
   then run the receive routine again check for errors,
   then call the query reply routine to handle any reply */
ecode = ipc_cb->ipcsend(buf, ipc_cb->ipchandle);
if (ecode)
    return(ecode);

EBufferInitialize(&rbuf);
ecode = ipc_cb->ipcrcv(&rbuf, ipc_cb->ipchandle);
if (ecode)
    return(ecode);

qecode = Master_Query_Reply_Handler(EBufferStart(&rbuf),
                                    EBufferUsed(&rbuf), 0);
EBufferClean(&rbuf);
/* if we couldn't decode the message, qecode == 0, signal that back
   to our caller */
if (qecode)
    return(0);
else
    return(2);
}

INT_32_T
  sa_asy_ayt(PTR_T handle)
{
SA_IPC_COMP_T *ipc_cb = (SA_IPC_COMP_T *)handle;
return(ipc_cb->ipcayt(ipc_cb->ipchandle));
}

void
  sa_asy_free(PTR_T handle)
{
SA_IPC_COMP_T *ipc_cb = (SA_IPC_COMP_T *)handle;
ipc_cb->ipcfree(ipc_cb->ipchandle);
SNMP_memory_free(handle);
}

void
  sa_asy_comp(OCTET_T    opcode,
              EBUFFER_T *ebuffp,
              VBL_T     *vblist,
              PTR_T      ipchandle)
{
SA_IPC_COMP_T *ipc_cb = (SA_IPC_COMP_T *)ipchandle;

*(ipc_cb->opcode) = opcode;
if (opcode == 0)
    return;

/* attempt to move or copy the buffer from the working ebuff to the
   one the user gave us */
if (EBufferStart(ipc_cb->buf) == 0) {
    EBufferPreLoad(BFL_IS_DYNAMIC, ipc_cb->buf, EBufferStart(ebuffp),
                   EBufferUsed(ebuffp));
    EBufferInitialize(ebuffp);
    }
else {
    if (EBufferUsed(ebuffp) > EBufferRemaining(ipc_cb->buf)) {
        *(ipc_cb->opcode) = 0;
        return;
        }
    EBufferAppend(ipc_cb->buf, ebuffp);
    }

/* if the opcode was for a trap we need to move the vblist information */
if (opcode == SA_TRAP_REQUEST) {
    MEMCPY(ipc_cb->vblist, vblist, sizeof(VBL_T));
    MEMSET(vblist, 0, sizeof(VBL_T));
    }

return;
}

/****************************************************************************
NAME:  Master_Handler_WR

PURPOSE:  Decode a sub agent packet and attempt to fulfill the request.

PARAMETERS:
        OCTET_T   *     Byte string for message
        ALENGTH_T       Length of the message
        IPCSEND_T *     To send messages to the subagent
        IPCRCV_T  *     To receive messages from the subagent
        IPCFREE_T *     A IPC routine to run to free the structure pointed
                        to be the ipc handle
        IPCAYT_T  *     A IPC routine to run to determine if the link is 
                        still active.
        PTR_T           A handle to give to the IPC routine to specify the
                        sub agent
        EBUFFER_T *     A buffer for any reply packets (may not be used)
        VBL_T     *     Space to put a list of nodes
        PTR_T           User private space, one use is for multiple mib trees

RETURNS:   0            Failure, drop the packet
          -1            Failure, send the error header in the reply buffer.
           opcode       Opcode of the decoded packet.
          
         This routine will deocde all unsolicited sub agent packets sent
         to a master agent.  There will be two types control (registration
         and deregistration requests) and traps.  If the message 
         is a control request this routine will decode the message,
         attempt to add or remove the listed nodes and create a reply
         for the originator of the message.  If it is a trap request
         it will decode the message and hand the information up to
         the calling routine for further processing.

****************************************************************************/
INT_32_T
  Master_Handler_WR(OCTET_T   *msg,
                    ALENGTH_T  msgl,
                    IPCSEND_T *ipcsend,
                    IPCRCV_T  *ipcrcv,
                    IPCFREE_T *ipcfree,
                    IPCAYT_T  *ipcayt,
                    PTR_T      ipchandle,
                    EBUFFER_T *rbuf,
                    VBL_T     *vblist,
                    PTR_T      user_priv)
{
SA_IPC_COMP_T *ipc_cb;
OCTET_T opcode = 0;

/* See if we can allocate a buffer for the compatibility block
   if we can't get even a small one we bail out now */
if ((ipc_cb = (SA_IPC_COMP_T *)SNMP_memory_alloc(sizeof(SA_IPC_COMP_T))) == 0)
    return(0);

ipc_cb->ipchandle = ipchandle;
ipc_cb->ipcrcv = ipcrcv;
ipc_cb->ipcsend = ipcsend;
ipc_cb->ipcfree = ipcfree;
ipc_cb->ipcayt = ipcayt;
ipc_cb->buf = rbuf;
ipc_cb->vblist = vblist;
ipc_cb->opcode = &opcode;

Master_Handler_Async(msg, msgl, sa_asy_comp, sa_asy_send, 0, sa_asy_free,
                     sa_asy_ayt, (PTR_T)ipc_cb, user_priv);

/* Check if opcode is an opcode or the error flag */
if (opcode == 255)
    return(-1);
else
    return((INT_32_T)opcode);
}

void
  sendit(IPCCOMP_T   *ipccomp,
         PTR_T        ipchandle,
         INT_32_T     errflag,
         SA_HEADER_T *hdr_blk,
         VBL_T       *vblist,
         OCTET_T     *errorhdr,
         int          short_flag)
{
EBUFFER_T ebuff;
OCTET_T opcode1;
SA_DEMUX_T demuxer;

EBufferInitialize(&ebuff);
EBufferInitialize(&demuxer.string);
init_object_id(&demuxer.object);

opcode1 = hdr_blk->opcode1;
hdr_blk->opcode1++;

/* if we are sending a short message back (just the header - no vbs) */
if ((short_flag) || (!errflag)) {
    if (errflag)
        hdr_blk->sa_error = (OCTET_T)(errflag & 0xFF);
    else {
        if (hdr_blk->sysuptime)
            SNMP_RESETSYSUPTIME;
        hdr_blk->sysuptime = SNMP_SYSUPTIME;
        }
    (void) SA_Encode_Hdr(errorhdr, hdr_blk);
    EBufferPreLoad(BFL_IS_DYNAMIC, &ebuff, errorhdr, SA_HEADERLEN);

    if (errflag)
        ipccomp(-1, &ebuff, vblist, ipchandle);
    else
        ipccomp(opcode1, &ebuff, vblist, ipchandle);      
    }
else {
    hdr_blk->sa_error = (OCTET_T)(errflag & 0xFF);
    if (SA_Encode(vblist, hdr_blk, &demuxer, &ebuff)) {
        /* we failed to create the outgoing message set an error */
        sendit(ipccomp, ipchandle, SA_GEN_ERROR, hdr_blk, 0, errorhdr, 1);
        return;
        }
    ipccomp(-1, &ebuff, vblist, ipchandle);
    }
EBufferClean(&ebuff);
return;
}

INT_32_T
  register_switch(OCTET_T     opcode,
                  VBL_T      *vblist,
                  SA_GROUP_T *group,
                  MIBNODE_T  *mib_root)
{
switch (opcode) {
    case SA_REM_OBJ_REQUEST:
        return(SA_Deregister_Nodes(vblist, vblist->vbl_count,
                                   group, mib_root));
    case SA_REG_INST_REQUEST:
        return(SA_Register_Instances(vblist, group, mib_root));
    case SA_REM_INST_REQUEST:
        return(SA_Deregister_Instances(vblist, vblist->vbl_count,
                                       group, mib_root));
    }
return(0);
}

void
  reg_ins_rtn(PTR_T data)
{
SA_REG_T *regblock = (SA_REG_T *)data;
INT_32_T errflag;
IPCFREE_T *ipcfree;

ipcfree = regblock->group->ipcfree;
errflag = register_switch(regblock->hdr_blk.opcode1, &regblock->vblist,
                          regblock->group, regblock->mib_root);

sendit(regblock->ipccomp, regblock->ipchandle, errflag, &regblock->hdr_blk,
       &regblock->vblist, regblock->errorhdr, 0);
Clean_vb_list(&regblock->vblist);
ipcfree(regblock->ipchandle);
SNMP_memory_free(regblock);
return;
}

void
  reg_obj_rtn(PTR_T data)
{
SA_REG_T *regblock = (SA_REG_T *)data;
INT_32_T errflag;
errflag = SA_Register_Nodes(&regblock->vblist, regblock->group,
                            regblock->mib_root);

if (errflag) {
    SNMP_memory_free(regblock->group);
    sendit(regblock->ipccomp, regblock->ipchandle, errflag, &regblock->hdr_blk,
           &regblock->vblist, regblock->errorhdr, 0);
    regblock->group->ipcfree(regblock->ipchandle);
    }
else {
    regblock->hdr_blk.index = regblock->group->groupid;
    sendit(regblock->ipccomp, regblock->ipchandle, errflag, &regblock->hdr_blk,
           &regblock->vblist, regblock->errorhdr, 0);
    }

Clean_vb_list(&regblock->vblist);
SNMP_memory_free(regblock);
return;
}

/****************************************************************************
NAME:  Master_Handler_Async

PURPOSE:  Decode a sub agent packet and attempt to fulfill the request.

PARAMETERS:
        OCTET_T   *     Byte string for message
        ALENGTH_T       Length of the message
        IPCCOMP_T *     Completion routine
        IPCSEND_AS_T *  To send messages to the subagent
        IPCRCV_T  *     To receive messages from the subagent (not used)
        IPCFREE_T *     A IPC routine to run to free the structure pointed
                        to by the ipc handle
        IPCAYT_T  *     A IPC routine to run to determine if the link is 
                        still active.
        PTR_T           A handle to give to the IPC routine to specify the
                        sub agent
        EBUFFER_T *     A buffer for any reply packets (may not be used)
        VBL_T     *     Space to put a list of nodes
        PTR_T           User private space, one use is for multiple mib trees

RETURNS:   0            Failure, drop the packet
          -1            Failure, send the error header in the reply buffer.
           opcode       Opcode of the decoded packet.
          
         This routine will deocde all unsolicited sub agent packets sent
         to a master agent.  There will be two types control (registration
         and deregistration requests) and traps.  If the message 
         is a control request this routine will decode the message,
         attempt to add or remove the listed nodes and create a reply
         for the originator of the message.  If it is a trap request
         it will decode the message and hand the information up to
         the calling routine for further processing.

****************************************************************************/
void
  Master_Handler_Async(OCTET_T          *msg,
                       ALENGTH_T         msgl,
                       IPCCOMP_T        *ipccomp,
                       IPCSEND_AS_T     *ipcsend,
                       IPCRCV_T         *ipcrcv,
                       IPCFREE_T        *ipcfree,
                       IPCAYT_T         *ipcayt,
                       PTR_T             ipchandle,
                       PTR_T             user_priv)
{
SA_HEADER_T hdr_blk;
INT_32_T errflag;
int short_flag = 0;
SA_GROUP_T *group = 0;
OCTET_T *errorhdr;
MIBNODE_T *mib_root;
SA_DEMUX_T demuxer;
VBL_T vblist;
#if (INSTALL_ENVOY_SNMP_SERIALIZE)
SA_REG_T *reggroup;
#endif

EBufferInitialize(&demuxer.string);
init_object_id(&demuxer.object);
MEMSET(&vblist, 0, sizeof(VBL_T));

/* See if we can allocate a string for use with outgoing packets,
   if we can't get even a small one we bail out now */
if ((errorhdr = (OCTET_T *)SNMP_memory_alloc(SA_HEADERLEN)) == 0) {
    ipccomp(0, 0, 0, ipchandle);
    /* Free the handle as we don't need it in this case */
    ipcfree(ipchandle);
    return;
    }

MEMSET(errorhdr, 0, SA_HEADERLEN);
MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));

errflag = SA_Decode(msg, msgl, &vblist, &hdr_blk, &demuxer);

/* Get the mib_root now so we can get rid of the demuxer, in the event
   of an error the demuxer may contain trash but the macro should be
   able to handle that */
#ifdef ENVOY_SA_MASTER_MIB_ROOT_NODE
mib_root = ENVOY_SA_MASTER_MIB_ROOT_NODE(user_priv, &demuxer.string,
                                         &demuxer.object);
#else
mib_root = (MIBNODE_T *)user_priv;
#endif

/* All done with the demuxer now */
EBufferClean(&demuxer.string);
Clean_Obj_ID(&demuxer.object);

if ((hdr_blk.sa_error) || (errflag == SA_UNKNOWN_VERSION)) {
    Clean_vb_list(&vblist);
    SNMP_memory_free(errorhdr);
    ipccomp(0, 0, 0, ipchandle);
    /* Free the handle as we don't need it in this case */
    ipcfree(ipchandle);
    return;
    }

if (errflag) {
    Clean_vb_list(&vblist);
    sendit(ipccomp, ipchandle, errflag, &hdr_blk, 0, errorhdr, 1);
    /* Free the handle as we don't need it in this case */
    ipcfree(ipchandle);
    return;
    }

switch (hdr_blk.opcode1) {
   case SA_REG_OBJ_REQUEST:
       if (vblist.vbl_count == 0) {
           /* The subagent has done a registration request with no
              vbs.  Tell him it succeeded and clean up */
           hdr_blk.index = 0;
           sendit(ipccomp, ipchandle, 0, &hdr_blk, 0, errorhdr, 1);
           /* Free the handle as we don't need it in this case */
           ipcfree(ipchandle);
           return;
           }

        /* allocate the group structure and start filling it in */
        hdr_blk.index = 0;
        group = (SA_GROUP_T *)SNMP_memory_alloc(sizeof(SA_GROUP_T));
        if (group == 0) {
            Clean_vb_list(&vblist);
            sendit(ipccomp, ipchandle, SA_MEMORY_ERROR, &hdr_blk, 0,
                   errorhdr, 1);
            /* Free the handle as we don't need it in this case */
            ipcfree(ipchandle);
            return;
            }
        group->ipcsend   = ipcsend;
        group->ipcrcv    = ipcrcv;
        group->ipcfree   = ipcfree;
        group->ipcayt    = ipcayt;
        group->ipchandle = ipchandle;
        group->encoding  = hdr_blk.encoding;
        group->flags     = 0;
        group->version   = hdr_blk.version;
        
#if (INSTALL_ENVOY_SNMP_LOCK)
        if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_CoarseLock) == 0) {
            group->groupid   = GROUPID++;
            errflag = SA_Register_Nodes(&vblist, group, mib_root);
            ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_CoarseLock);
            }
        else {
            BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
                (BUG_OUT, "Master_Handler_Async: coarse lock is broken", 0));

            errflag = SA_LOCK_ERROR;
            short_flag = 1;
            }
#endif
#if (INSTALL_ENVOY_SNMP_SERIALIZE)
        group->groupid   = GROUPID++;
        /* If we are being compatible with the old api we can't
           defer the request, we either service it or send back an error */
        if (ipccomp == sa_asy_comp) {
            if (GateKeeper_Test()) {
                errflag = SA_LOCK_ERROR;
                short_flag = 1;
                }
            else
                errflag = SA_Register_Nodes(&vblist, group, mib_root);
            }
        else { /* if (ipccomp == sa_asy_comp) */
            /* build registration group */
            reggroup = (SA_REG_T *)SNMP_memory_alloc(sizeof(SA_REG_T));
            if (reggroup == 0) {
                errflag = SA_LOCK_ERROR;
                short_flag = 1;
                }
            else { /* if (reggroup == 0) */
                reggroup->ipccomp = ipccomp;
                reggroup->ipchandle = ipchandle;
                reggroup->group = group;
                reggroup->mib_root = mib_root;
                reggroup->errorhdr = errorhdr;
                MEMCPY(&reggroup->vblist, &vblist, sizeof(VBL_T));
                MEMCPY(&reggroup->hdr_blk, &hdr_blk, sizeof(SA_HEADER_T));
                /* call gatekeeper to see if this event should be deferred */
                switch(GateKeeper_Add(GATE_SUBAGENT, reggroup, reg_obj_rtn)) {
                    case 0:
                        errflag = SA_Register_Nodes(&vblist, group, mib_root);
                        break;
                    case 1:
                        return;
                    case -1:
                        errflag = SA_LOCK_ERROR;
                        short_flag = 1;
                    }           
                SNMP_memory_free(reggroup);
                } /* if (reggroup == 0) */
            } /* if (ipccomp == sa_asy_comp) */
#endif  
        if (errflag) {
            SNMP_memory_free(group);
            sendit(ipccomp, ipchandle, errflag, &hdr_blk, &vblist,
                   errorhdr, short_flag);
            /* Free the handle as we don't need it in this case */
            ipcfree(ipchandle);
            }
        else {
            hdr_blk.index = group->groupid;
            sendit(ipccomp, ipchandle, errflag, &hdr_blk, &vblist,
                   errorhdr, short_flag);
            }
        Clean_vb_list(&vblist);
        return;

    case SA_REM_OBJ_REQUEST:
    case SA_REG_INST_REQUEST:
    case SA_REM_INST_REQUEST:
       if (vblist.vbl_count == 0) {
           /* The subagent has done a registration request with no
              vbs.  Tell him it succeeded and clean up */
           sendit(ipccomp, ipchandle, 0, &hdr_blk, 0, errorhdr, 1);
           /* Free the handle as we don't need it in this case */
           ipcfree(ipchandle);
           return;
           }

#if (INSTALL_ENVOY_SNMP_LOCK)
        if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_CoarseLock) == 0) {
            /* need to find the group from the groupid (index) information */
            if ((group = find_group_by_index(hdr_blk.index)) == 0) {
                errflag = SA_UNKNOWN_GRP;
                short_flag = 1;
                }
            else
                switch (hdr_blk.opcode1) {
                    case SA_REM_OBJ_REQUEST:
                        errflag = SA_Deregister_Nodes(&vblist,
                                                      vblist.vbl_count,
                                                      group, mib_root);
                        break;
                    case SA_REG_INST_REQUEST:
                        errflag = SA_Register_Instances(&vblist, group,
                                                        mib_root);
                        break;
                    case SA_REM_INST_REQUEST:
                        errflag = SA_Deregister_Instances(&vblist,
                                                          vblist.vbl_count,
                                                          group, mib_root);
                        break;
                        }
            ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_CoarseLock);
            }
        else {
            BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
                (BUG_OUT,"Master_Handler_Async: infrastructure lock is broken",
                 0));
            errflag = SA_LOCK_ERROR;
            short_flag = 1;
            }
#endif
#if (INSTALL_ENVOY_SNMP_SERIALIZE)
        /* need to find the group from the groupid (index) information */
        if ((group = find_group_by_index(hdr_blk.index)) == 0) {
            errflag = SA_UNKNOWN_GRP;
            short_flag = 1;
            }
        else { /* if ((group = find_group_by_index ...)) */
            /* If we are being compatible with the old api we can't
               defer the request, we either service it or send back an error */
            if (ipccomp == sa_asy_comp) {
                if (GateKeeper_Test()) {
                    errflag = SA_LOCK_ERROR;
                    short_flag = 1;
                    }
                else
                    errflag = register_switch(hdr_blk.opcode1, &vblist,
                                              group, mib_root);
                }
            else { /* if (ipccomp == sa_asy_comp) */

                /* build registration group */
                reggroup = (SA_REG_T *)SNMP_memory_alloc(sizeof(SA_REG_T));
                if (reggroup == 0) {
                    errflag = SA_LOCK_ERROR;
                    short_flag = 1;
                    }
                else { /* if (reggroup == 0) */
                    reggroup->ipccomp = ipccomp;
                    reggroup->ipchandle = ipchandle;
                    reggroup->group = group;
                    reggroup->mib_root = mib_root;
                    reggroup->errorhdr = errorhdr;
                    MEMCPY(&reggroup->vblist, &vblist, sizeof(VBL_T));
                    MEMCPY(&reggroup->hdr_blk, &hdr_blk, sizeof(SA_HEADER_T));
                    /*call gatekeeper to see if this event should be deferred*/
                    switch(GateKeeper_Add(GATE_SUBAGENT,reggroup,reg_ins_rtn)){
                        case 0:
                            errflag = register_switch(hdr_blk.opcode1, &vblist,
                                                      group, mib_root);
                            break;
                        case 1:
                            return;
                        case -1:
                            errflag = SA_LOCK_ERROR;
                            short_flag = 1;
                        }               
                    SNMP_memory_free(reggroup);
                    } /* if (reggroup == 0) */
                } /* if (ipccomp == sa_asy_comp) */
            } /* if ((group = find_group_by_index ...)) */
#endif  
        sendit(ipccomp, ipchandle, errflag, &hdr_blk, &vblist, errorhdr,
               short_flag);
        Clean_vb_list(&vblist);
        /* free the handle as we already have a handle and so won't use
           this one */
        ipcfree(ipchandle);
        break;

    case SA_TRAP_REQUEST:
        sendit(ipccomp, ipchandle, errflag, &hdr_blk, &vblist, errorhdr, 0);
        Clean_vb_list(&vblist);
        /* Free the handle as we don't need it in this case */
        ipcfree(ipchandle);
        break;

    default:
        sendit(ipccomp, ipchandle, SA_UNKNOWN_OPCODE1, &hdr_blk, 0,
               errorhdr, 1);
        Clean_vb_list(&vblist);
        /* Free the handle as we don't need it in this case */
        ipcfree(ipchandle);
        break;
    }

return;
}

/****************************************************************************
NAME:  Master_Query_Reply_Handler

PURPOSE:  This routine is for use with sub agents. 
          It is the routine for users to call with a reply to a query
          of a sub agent.  It will decode the message and call the
          appropriate routine to continue processing the packet.

PARAMETERS:
        OCTET_T   *     Byte string for message
        ALENGTH_T       Length of the message
        UINT_16_T       Request id
        int             Continue flag 1 = run continue, 0 = don't

RETURNS:  void

****************************************************************************/
/*ARGSUSED*/
UINT_16_T
  Master_Query_Reply_Handler(OCTET_T    *msg,
                             ALENGTH_T   msgl,
                             int         cont_flag)
{
SA_HEADER_T hdr_blk;
SA_DEMUX_T demuxer;
VBL_T rvblist;
SNMP_SA_CB_T *cbp;
SNMP_PKT_T *pktp;
VB_T *vbp;
OCTET_T opcode2;
UINT_16_T reqid;
int vb_count;

/* Initialize all the variables so we don't have any suprises */
EBufferInitialize(&demuxer.string);
init_object_id(&demuxer.object);
MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));
MEMSET(&rvblist, 0, sizeof(VBL_T));

/* if we have any type of decoding error, the message isn't a query
   reply or the request ids don't match drop the message and run
   the timeout routine to cleanup the packet and state block */
if (SA_Decode(msg, msgl, &rvblist, &hdr_blk, &demuxer) ||
    (hdr_blk.opcode1 != SA_QUERY_REPLY)) {
    Clean_vb_list(&rvblist);
    EBufferClean(&demuxer.string);
    Clean_Obj_ID(&demuxer.object);
    return(0);
    }

/* clean up some vars we won't be using */
EBufferClean(&demuxer.string);
Clean_Obj_ID(&demuxer.object);

/* set the request id for return */
reqid = hdr_blk.requestid;

/* Use the given reqid to find the control block */
cbp = SA_uninstall_cb(hdr_blk.requestid, 0);

/* No control block, bail out */
if (cbp == 0) {
    Clean_vb_list(&rvblist);
    return(reqid);
    }

pktp = cbp->pktp;
vbp = cbp->vbp;
vb_count = cbp->count;
opcode2 = hdr_blk.opcode2;

/* all done with cbp so free it */
SNMP_memory_free(cbp);

#if INSTALL_ENVOY_CONTINUE_REENTRANT
/* If necessary we acquire the packet continue write lock here
   It gets freed when we call the continue routine.
   We don't have any good options if we can't get the lock.
   The user shouldn't do that, if they do we generate a bug
   report and if they don't handle that we return which is
   the least bad thing to do. */
if (cont_flag)
    if (ENVOY_SNMP_GET_WRITE_LOCK(pktp->continue_lock)) {
        BUG(BUG_ENVOY_LOCKING, BUG_FATAL, 0,
            (BUG_OUT,
             "Master_Queery_Reply_Handler(): packet continue lock is broken",
             0));
        return(reqid);
        }
#endif

switch(pktp->pdu_type) {
    case GET_REQUEST_PDU:
        switch(opcode2) {
            case SA_QUERY_TESTGET:
                SA_get_mid(pktp, vbp, &hdr_blk, &rvblist, vb_count);
                break;
            case SA_QUERY_GET:
                SA_get_end(pktp, vbp, &hdr_blk, &rvblist, vb_count);
                break;
            default:
                getproc_error(pktp, vbp, GEN_ERR);
                Clean_vb_list(&rvblist);
                break;
            }
        break;
    case GET_NEXT_REQUEST_PDU:
    case GET_BULK_REQUEST_PDU:
        switch(opcode2) {
            case SA_QUERY_TESTGET:
                SA_next_mid(pktp, vbp, &hdr_blk, &rvblist, vb_count, 1);
                break;
            case SA_QUERY_NEXT:
                SA_next_mid(pktp, vbp, &hdr_blk, &rvblist, vb_count, 0);
                break;
            case SA_QUERY_GET:
                SA_get_end(pktp, vbp, &hdr_blk, &rvblist, vb_count);
                break;
            default:
                nextproc_error(pktp, vbp, GEN_ERR);
                Clean_vb_list(&rvblist);
                break;
            }
        break;
    case SET_REQUEST_PDU:
        switch(opcode2) {
            case SA_QUERY_TESTSET:
                SA_test_end(pktp, vbp, &hdr_blk, &rvblist, vb_count);
                break;
            case SA_QUERY_SET:
                SA_set_end(pktp, vbp, &hdr_blk, &rvblist, vb_count);
                break;
            default:
                if (pktp->phase == 0)
                    testproc_error(pktp, vbp, GEN_ERR);
                else
                    setproc_error(pktp, vbp, GEN_ERR);
                Clean_vb_list(&rvblist);
                break;
            }
        break;
    }

if (cont_flag)
    SNMP_Continue(pktp);
return(reqid);
}

void
  Master_Cleanup(UINT_16_T reqid,
                 UINT_16_T options)
{
SNMP_SA_CB_T *cbp;
VB_T *vbp, *tvbp;
SNMP_PKT_T *pktp;
SA_GROUP_T *group;

if ((cbp = SA_uninstall_cb(reqid, 0)) == 0)
    return;

vbp = cbp->vbp;
pktp = cbp->pktp;

/* all done with cbp so free the space */
SNMP_memory_free(cbp);

/* if the inactive flag is set then if we can get a group we mark
   it as inactive */
if (options & SA_CLEANUP_INACTIVE) {
    group = SA_getgroup(vbp->vb_ml.ml_remaining_objid.num_components,
                        vbp->vb_ml.ml_remaining_objid.component_list,
                        (SA_COOKIE_T *)(VB_TO_COOKIE(vbp)));
    if (group)
        group->flags |= SA_GROUP_INACTIVE;
    group_cleanup_flag = 1;
    }

#if INSTALL_ENVOY_CONTINUE_REENTRANT
/* If necessary we acquire the packet continue write lock here
   It gets freed when we call the continue routine.
   We don't have any good options if we can't get the lock.
   The user shouldn't do that, if they do we generate a bug
   report and if they don't handle that we return which is
   the least bad thing to do. */
if (options & SA_CLEANUP_CONTINUE)
    if (ENVOY_SNMP_GET_WRITE_LOCK(pktp->continue_lock)) {
        BUG(BUG_ENVOY_LOCKING, BUG_FATAL, 0,
            (BUG_OUT, "Master_Cleanup(): packet continue lock is broken", 0));
        return;
        }
#endif

/* try to fill in the packet for a reply */
switch(pktp->pdu_type) {
    case GET_REQUEST_PDU:
        getproc_error(pktp, vbp, GEN_ERR);
        for(; vbp; vbp = vbp->vb_link)
            getproc_good(pktp, vbp);
        break;
    case GET_NEXT_REQUEST_PDU:
    case GET_BULK_REQUEST_PDU:
        if ((options & SA_CLEANUP_TIMEOUT) == 0)
            nextproc_error(pktp, vbp, GEN_ERR);
        while(vbp) {
            nextproc_no_next(pktp, vbp);
            if (((SA_COOKIE_T *)(VB_TO_COOKIE(vbp)))->flags & SA_INSTANCE_TYPE)
                vbp->vb_flags |= VFLAG_NEXT_INSTANCE;
            tvbp = vbp->vb_link;
            vbp->vb_link = 0;
            vbp = tvbp;
            }
        break;
    case SET_REQUEST_PDU:
        if (vbp->vb_flags & VFLAG_SET_STARTED)
            setproc_error(pktp, vbp, COMMIT_FAILED);
        else
            setproc_error(pktp, vbp, GEN_ERR);
        for(; vbp; vbp = vbp->vb_link)
            setproc_all_bits(pktp, vbp);
        break;
    }

if (options & SA_CLEANUP_CONTINUE)
    SNMP_Continue(pktp);

return;
}

#endif /* (INSTALL_ENVOY_MASTER_AGENT) */


