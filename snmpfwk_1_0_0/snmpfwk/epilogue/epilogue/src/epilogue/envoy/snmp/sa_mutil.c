/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/sa_mutil.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: sa_mutil.c,v $
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
 * Revision 7.4  1997/10/22 03:10:25  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.3  1997/10/16 00:46:02  sar
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
 * Revision 7.2  1997/03/20 06:49:14  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:48:01  sra
 * Release 6.0.
 *
 * Revision 1.1  1995/04/28  22:44:06  sar
 * Initial revision
 *
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <subagent.h>
#include <snmp.h>
#include <objectid.h>

#include <bug.h>

#if (INSTALL_ENVOY_EPI_MASTER)

#if !defined(SNMP_SYSUPTIME)
#define SNMP_SYSUPTIME 0
#endif

SA_HANDLE_T     *handle_list = 0;
INT_16_T         group_cleanup_flag = 0;

/****************************************************************************
NAME:  remove_grp_lst

PURPOSE:  Given a pointer to a group list, unhook the group list
          from it's mib leaf's group chain and free it, free any instances
          associated with the group, and if this was the last group
          on the mib leaf's chain free the cookie and mark the leaf
          as inactive.

PARAMETERS:
        SA_GRP_LST_T    pointer to a group list entry

RETURNS:  INT_32_T      0 this wasn't the last group on the chain.
                        1 this was the last group the leaf is now inactive.
          
****************************************************************************/
void
  remove_grp_lst(SA_GRP_LST_T *grplst)
{
SA_GRP_LST_T *tgrplst, *instance, *tinstance;
MIBLEAF_T *lp;
SA_COOKIE_T *cookiep;

/* set up some pointers we will need later */
lp = grplst->leaf;
cookiep = (SA_COOKIE_T *)lp->user_cookie;

/* if we have any instances free them */
if (grplst->instance) {
    instance = grplst->instance;
    do {
        tinstance = instance->instance;
        TREE_Delete(&(cookiep->instance_tree), (TREENODE_T *)instance->leaf);
        SNMP_memory_free(instance);
        instance = tinstance;
        }
    while (instance != grplst->instance);
    }

/* unhook the grplst from the leaf's chain of instance group lists. */
if (cookiep->ichain == grplst)
    cookiep->ichain = grplst->ichain;
else {
    for(tgrplst = cookiep->ichain;
        tgrplst->ichain != grplst;
        tgrplst = tgrplst->ichain)
        ;
    tgrplst->ichain = grplst->ichain;
    }

/* free the grp lst space */
SNMP_memory_free(grplst);

/* if this was the last entry in the chain 0 the cookie,
   the cookie must be freed when the leaf is freed, in register
   or deregister nodes */

if (cookiep->ichain == 0) {
    lp->node_type |= LEAF_INACTIVE;
    cookiep->instance_tree = 0;
    }
return;
}

/****************************************************************************
NAME:  find_group_by_index

PURPOSE:  Use the given index to find a group

PARAMETERS:
        INT_32_T        index to search for

RETURNS:  SA_GROUP_T*   the found group, 0 if no group found
          
****************************************************************************/
SA_GROUP_T *
  find_group_by_index(INT_32_T indx)
{
SA_HANDLE_T *handle;

for (handle = handle_list; handle; handle = handle->nexthand)
    if (handle->group->groupid == indx)
        return(handle->group);

return(0);
}

/****************************************************************************
NAME:  mark_group_inactive

PURPOSE:  Given a pointer to a group mark all entries in that group's list
          as inactive, free everything we can, (the group and group list
          stuff will be freed).

PARAMETERS:
        SA_GROUP_T      pointer to a group

RETURNS:  void
          
****************************************************************************/
void
  mark_group_inactive(SA_GROUP_T *group)
{
SA_GRP_LST_T *grplst, *nextgrplst, *curgrplst;

grplst = group->group_list;
curgrplst = grplst->gchain;
while (curgrplst != grplst) {
    nextgrplst = curgrplst->gchain;
    (void) (remove_grp_lst(curgrplst));
    curgrplst = nextgrplst;
}
remove_grp_lst(grplst);

/* this is the last pointer to the group so we call the ipcfree routine
   and then free the group itself */
group->ipcfree(group->ipchandle);
SNMP_memory_free(group);
return;
}

/*ARGSUSED*/
SA_GROUP_T *
  SA_getgroup(int          compc,
              OIDC_T      *compl,
              SA_COOKIE_T *cookie)
{
TREENODE_T *tnode;
SA_GRP_LST_T *grplst = 0;
int rlen;

/* Given a cookie and instance information find a group.
   If the cookie is an instance type, search the instance tree
   Else use the group infor from the first grplst in the cookie. */

if (cookie == 0)
    return(0);
if (cookie->flags & SA_INSTANCE_TYPE) {
    tnode = TREE_Get(cookie->instance_tree, compl, compc, &rlen);
    if ((tnode == 0) || (rlen != compc))
        return(0);
    if (TREE_Extract(tnode, (PTR_T *)(&grplst)))
        return(0);
    return(grplst->group);
    }
else
    return(cookie->ichain->group);
}

/****************************************************************************
NAME:  SA_priv_tag_to_vb_master

PURPOSE:  Translate the priv tags in a vblist into the correct form for
          a master (vb_priv_tag & sa_vbpriv_free).  0 is a well known tag that
          means there is no private information.

PARAMETERS:
        VBL_T      *    The vblist being translated
        INT_32_T        The number of vbs to check 
        
RETURNS:  void

****************************************************************************/
void
  SA_priv_tag_to_vb_master(VBL_T      *vblist,
                           INT_32_T    count)
{
VB_T *vbp;
INT_32_T i;

for (i = count, vbp = vblist->vblist; i > 0; i--, vbp++) {
    /* is this a vb we need to modify ? */
    if (!(vbp->vb_flags & VFLAG_SA_DOIT))
        continue; /* nope */

    /* if the tag is 0 then both pieces of data are 0 */
    if (vbp->vb_priv == 0) {
        vbp->vb_free_priv = 0;
        continue;
        }

    /* Otherwise we install the free procedure */
    vbp->vb_free_priv = (VBPRIVPROC_T *)SA_freeproc;
    }
}

/****************************************************************************
NAME:  SA_freeproc

PURPOSE:  The routine to free resources reserved during the test phase.
          
PARAMETERS:
          PTR_T         A pointer to the VB_T being cleaned.

RETURNS:  nothing

****************************************************************************/
void
  SA_freeproc(PTR_T vbptr)
{
VB_T *vbp;
VBL_T vblist;
SA_GROUP_T *group;
SNMP_PKT_T pkt;
SA_HEADER_T hdr_blk;
EBUFFER_T ebuf;
SA_DEMUX_T demuxer;

/* point the vbp at the right place */
vbp = (VB_T *)vbptr;

/* we don't need any of the infomration from the packet to do the free */
Initialize_Pkt(&pkt);
pkt.snmp_version = SNMP_VERSION_1;

group = SA_getgroup(vbp->vb_ml.ml_remaining_objid.num_components,
                    vbp->vb_ml.ml_remaining_objid.component_list,
                    (SA_COOKIE_T *)vbp->vb_ml.ml_leaf->user_cookie);

MEMSET(&hdr_blk, 0, sizeof(SA_HEADER_T));
hdr_blk.opcode2         = SA_QUERY_UNDO;
hdr_blk.version         = 1;
hdr_blk.opcode1         = SA_QUERY_REQUEST;
hdr_blk.encoding        = (OCTET_T)(group->encoding & 0xFF);
hdr_blk.snmp_version    = (OCTET_T)SNMP_VERSION_1;
hdr_blk.sysuptime       = SNMP_SYSUPTIME;

vbp->vb_flags = VFLAG_SA_SENDIT | VFLAG_SA_DOIT;
vblist.vbl_count = 1;
vblist.vblist    = vbp;
vblist.vblp      = 0;

EBufferInitialize(&ebuf);
EBufferInitialize(&demuxer.string);
init_object_id(&demuxer.object);

if (SA_Encode(&vblist, &hdr_blk, &demuxer, &ebuf))
    return;

(void)group->ipcsend(&ebuf, group->ipchandle, 0);
return;
}

/****************************************************************************
NAME:  hand_add_group
  
PURPOSE:  Given a pointer to a sub agent group (SA_GROUP_T *) allocate a
          handle entry and add it to the handle list.
  
PARAMETERS:
SA_GROUP_T    * pointer to group entry being added.
  
RETURNS: INT_32_T  0 success
                   1 failure
  
****************************************************************************/
INT_32_T
  hand_add_group(SA_GROUP_T *group)
{
SA_HANDLE_T *handle;

handle = (SA_HANDLE_T *)SNMP_memory_alloc(sizeof(SA_HANDLE_T));
if (handle == 0)
    return(1);
handle->group    = group;
handle->nexthand = handle_list;
handle_list      = handle;
return(0);
}

/****************************************************************************
NAME:  hand_replace_group
  
PURPOSE:  Given pointers to two sub agent groups (SA_GROUP_T *) find the
          entry (if any) in the handle list that points to the first group
          pointer and make it point to the second group pointer.  This
          can be used when trying to remove the first group entry.
  
PARAMETERS:
        SA_GROUP_T    * Pointer to group entry being replaced
        SA_GROUP_T    * pointer to group entry being added.
  
RETURNS:  void
  
  
****************************************************************************/
void
  hand_replace_group(SA_GROUP_T *rem_group,
                     SA_GROUP_T *add_group)
{
SA_HANDLE_T *handle;
  
if (handle_list == 0)
    return;
if (rem_group == handle_list->group) {
    handle_list->group = add_group;
    return;
    }
for(handle = handle_list; handle != 0; handle = handle->nexthand)
    if (rem_group == handle->group) {
        handle->group = add_group;
        break;
        }
return;
}

/****************************************************************************
NAME:  hand_remove_group
  
PURPOSE:  Given a pointer to a sub agent group, find the handle entry that
          point to the group and remvoe it.
  
PARAMETERS:
        SA_GROUP_T    * Pointer to group being removed.
  
RETURNS:  void
  
  
****************************************************************************/
void
  hand_remove_group(SA_GROUP_T *group)
{
SA_HANDLE_T *handle, *handle2;
  
if (handle_list == 0)
    return;
  
if (group == handle_list->group) {
    handle = handle_list;
    handle_list = handle->nexthand;
    SNMP_memory_free(handle);
    return;
    }
  
for (handle = handle_list->nexthand, handle2 = handle_list;
     handle != 0;
     handle2 = handle, handle = handle->nexthand)
    if (group == handle->group) {
        handle2->nexthand = handle->nexthand;
        SNMP_memory_free(handle);
        break;
        }
  
return;
}

/****************************************************************************
NAME:  hand_remove_handle
  
PURPOSE:  Given a pointer to a sub agent ipc handle structure find any
          entries in the handle list that point to groups that use
          that ipc handle.  Mark the groups as inactive and remove the
          handle entries.
  
PARAMETERS:
          PTR_T pointer to an ipc handle structure      
  
RETURNS:  void
  
****************************************************************************/
void
  hand_remove_handle(PTR_T ipchandle)
{
SA_HANDLE_T *handle, **phandlep;

for (phandlep = &handle_list; *phandlep; )
    if ((*phandlep)->group->ipchandle == ipchandle) {
        handle = *phandlep;
        *phandlep = handle->nexthand;
        mark_group_inactive(handle->group);
        SNMP_memory_free(handle);
        }
    else
        phandlep = &(*phandlep)->nexthand;

return;
}

/****************************************************************************
NAME:  Cleanup_Group

PURPOSE:  Walk through the group lists and cleanup any groups that
          have been marked inactive.

PARAMETERS:     void

RETURNS:        void

****************************************************************************/
void
  Cleanup_Group(void)
{
SA_HANDLE_T *handle, **phandlep;

/* Test the cleanup flag to see if we need to do any work */
if (group_cleanup_flag == 0)
    return;

/* Get the write lock for the database so we won't modify
   the database while somebody is using it */
#if (INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_CoarseLock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "Cleanup_Group: coarse lock is broken", 0));
    return;
    }
#endif

/* Step through the handle list and free any groups that have
   the inactive bit set */

for (phandlep = &handle_list; *phandlep; )
    if ((*phandlep)->group->flags & SA_GROUP_INACTIVE) {
        handle = *phandlep;
        *phandlep = handle->nexthand;
        mark_group_inactive(handle->group);
        SNMP_memory_free(handle);
        }
    else
        phandlep = &(*phandlep)->nexthand;

group_cleanup_flag = 0;

ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_CoarseLock);
return;
}

#endif /* #if (INSTALL_ENVOY_EPI_MASTER) */


