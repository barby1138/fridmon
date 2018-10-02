/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/vb_rtns.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: vb_rtns.c,v $
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
 * Revision 1.4  1997/11/01 01:25:40  sar
 * In the packet we are processing save a pointer to the coarse lock we
 * are using so that we can free it later without needing to know which
 * lock it was.
 *
 * Revision 1.3  1997/10/22 03:10:27  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 1.2  1997/10/21 02:58:24  sar
 * Move some of the macro defintions out to the port files,
 * Also move the leaf decrement function to vb_rtns
 *
 * Revision 1.1  1997/10/16 00:37:39  sar
 * Routines from breaking up snmp_d.c.  These were spun off so that
 * programs that need to manipulate snmp packets but didn't want to
 * decode an asn1 stream could avoid the excess routines.  Generally
 * these programs will be sub agents.
 *
 */

/* These functions were originally in snmp_d.c, they were moved here
   to allow other functions to access them without including the 
   packet decode routines.  More revision log entries may be found there */

#include <asn1conf.h>
#include <asn1.h>
#include <localio.h>
#include <buffer.h>
#include <decode.h>
#include <objectid.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <snmpstat.h>

#if INSTALL_ENVOY_EPI_MASTER
#include <subagent.h>
#endif

#if INSTALL_ENVOY_AGENTX_MASTER
#include <agentx.h>
#endif

#include <bug.h>

/****************************************************************************
NAME:  Clean_vb_data

PURPOSE:  Clean up any dynamically allocated memory which may be attached
          to a VarBinds data area

PARAMETERS:
        VB_T *          The VarBind to be cleaned

RETURNS:  nothing
****************************************************************************/
void
  Clean_vb_data(VB_T *vbp)
{
switch (vbp->vb_data_flags_n_type) {
    case VT_STRING:
    case VT_OPAQUE:
        if (vbp->value_u.v_string.start_bp != 0)
            EBufferClean(&(vbp->value_u.v_string));
        break;
    case VT_OBJECT:
        Clean_Obj_ID(&(vbp->value_u.v_object));
        break;
    }
}

/****************************************************************************
NAME:  Clean_vb

PURPOSE:  Clean up any dynamically allocated memory which may be attached
          to an VarBind

PARAMETERS:
        VB_T *          The VarBind to be cleaned

RETURNS:  nothing
****************************************************************************/
void
  Clean_vb(VB_T *vbp)
{

  /* if needed, clean up after an aborted set */
  if (vbp->vb_free_priv != 0) {
#if INSTALL_ENVOY_40_VB_FREE_COMPAT
#if INSTALL_ENVOY_EPI_MASTER
      if (vbp->vb_free_priv == (VBPRIVPROC_T *)SA_freeproc)
          SA_freeproc((PTR_T)vbp);
      else
#endif
          (*vbp->vb_free_priv)(vbp->vb_priv);
#else
      (*vbp->vb_free_priv)((PTR_T)vbp);
#endif
      vbp->vb_free_priv = 0;
      }

  Clean_Obj_ID(&(vbp->vb_obj_id));
  Clean_vb_data(vbp);

#if INSTALL_ENVOY_AGENTX_MASTER
  /* If this is an agentx leaf we need to adjust the reference counter
     for the leaf and if it becomes 0 free the leaf.  Currently we grab
     the lock here, though this might go into another routine sometime */
  if (vbp->vb_ml.ml_leaf && (vbp->vb_ml.ml_leaf->node_type & AGENTX_LEAF)) {

#if INSTALL_ENVOY_SNMP_LOCK
      /* Try to get the infrastructure lock.  We don't have any
         good options if we can't get the lock so the user shouldn't
         do that.  If they do we generate a bug report and if they
         don't handle that we skip cleaning up the leaf which is
         the least bad thing to do */
      if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
          BUG(BUG_ENVOY_LOCKING, BUG_FATAL, 0,
              (BUG_OUT, "Clean_vb(): infrastructure lock broken", 0));
          return;
          }
#endif

      vbp->vb_ml.ml_leaf->ref_count--;
      if (vbp->vb_ml.ml_leaf->ref_count == 0) {
          SNMP_memory_free(vbp->vb_ml.ml_leaf);
          }

      ENVOY_SNMP_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
      }
  Clean_Obj_ID(&vbp->ax_search_end);
#endif
}

/****************************************************************************
NAME:  Clean_vb_list

PURPOSE:  Clean up any dynamically allocated memory which may be attached
          to an VarBindList

PARAMETERS:
        VBL_T *         The VarBindList to be cleaned

RETURNS:  nothing
****************************************************************************/
void
  Clean_vb_list(VBL_T * vblp)
{
  VB_T  *vbp;
  VBL_T *vblpcur, *vblpnext;
  int    first = 1; /* first will be 1 during the first loop, 0 after*/

  /* set up the initial value then loop until we run out of vbl's */
  vblpcur = vblp;
  while(vblpcur != 0) {
      /* clean up the vblist associated with this vbl */
      if ((vbp = vblpcur->vblist) != 0) {
          int i;
          for (i = 0; i < vblpcur->vbl_count; i++, vbp++) {
              Clean_vb(vbp);
              }
          SNMP_memory_free((char *)(vblpcur->vblist));
          }

      /* get a pointer to the next vbl */
      vblpnext = vblpcur->vblp;

      /* if the current one isn't the first one, free it */
      if (first) {
          /* set the flag to be not the first one */
          first = 0;
          vblpcur->vbl_count = 0;
          }
      else
          SNMP_memory_free ((char *)(vblpcur));

      /* set the current one to the next one and do the loop */
      vblpcur = vblpnext;
      }
}

/****************************************************************************
NAME:  Clean_Pkt

PURPOSE:  Clean up any dynamically allocated memory which may be attached
          to an SNMP packet structure.

PARAMETERS:
        SNMP_PKT_T *    The packet structure to be cleaned

RETURNS:  nothing
****************************************************************************/
static void
  Clean_Pkt(SNMP_PKT_T *        rp)
{
#if defined(SNMP_release_private)
SNMP_release_private(rp);
#endif

/* Decide what we need to clean up.  If we have v1 installed we check if
   this is a trap type pdu in which case we clean the trap vbl, otherwise
   we clean the standard and saved vbls */

if (rp->community.start_bp != 0)
    EBufferClean(&(rp->community));

#if INSTALL_ENVOY_SNMP_VERSION_1
if (rp->pdu_type == TRAP_PDU) { /* Its a trap form of pdu */
    Clean_Obj_ID(&(rp->pdu.trap_pdu.enterprise_objid));
    Clean_vb_list(&(rp->pdu.trap_pdu.trap_vbl));
    }
else {
    Clean_vb_list(&(rp->pdu.std_pdu.std_vbl));
    Clean_vb_list(&(rp->pdu.std_pdu.saved_vbl));
    }
#else
Clean_vb_list(&(rp->pdu.std_pdu.std_vbl));
Clean_vb_list(&(rp->pdu.std_pdu.saved_vbl));
#endif /* INSTALL_ENVOY_SNMP_VERSION_1 */

#if INSTALL_ENVOY_SNMP_LOCK
if (rp->coarse_lock) {
    switch (rp->lockflags) {
        case LOCK_READ:
            ENVOY_SNMP_RELEASE_READ_LOCK((*rp->coarse_lock));
            break;
        case LOCK_WRITE:
            ENVOY_SNMP_RELEASE_WRITE_LOCK((*rp->coarse_lock));
            break;
        case LOCK_NOLOCK:
        default:
            break;
        }
    }
#endif

Initialize_Pkt(rp);
}

/****************************************************************************
NAME:  SNMP_Free

PURPOSE:  Release an SNMP packet structure.

PARAMETERS:
        SNMP_PKT_T *    The packet structure to be released

RETURNS:  nothing
****************************************************************************/
void
  SNMP_Free(SNMP_PKT_T *rp)
{

if (rp != 0) {
#if (INSTALL_ENVOY_MAIN_AGENT && INSTALL_ENVOY_SNMP_SERIALIZE)
    /* If we are serializing packets we need to adjust the
       reader/writer counts */
    GateDecrement(rp);
#endif /* (INSTALL_ENVOY_SNMP_SERIALIZE) */
    Clean_Pkt(rp);
    SNMP_memory_free((char *)rp);
    }

#if (INSTALL_ENVOY_EPI_MASTER && !INSTALL_ENVOY_SNMP_SERIALIZE)
/* If this is a master agent and we aren't serializing events then
   we need to try and cleanup any groups that were made inactive */
Cleanup_Group();
#endif
}

/****************************************************************************
NAME:  VarBindList_Allocate

PURPOSE:  Allocate a fresh set of VarBind structures

PARAMETERS:     Number of VarBind entries

RETURNS:  VB_T *        List of VarBind structures, (VB_T *)0 on failure
****************************************************************************/
VB_T *
  VarBindList_Allocate(int elements)
{
  VB_T  *vbp;
  int    need;

  need = sizeof(VB_T) * elements;
  if (need != 0) {
    if ((vbp = (VB_T *)SNMP_memory_alloc((unsigned int)need)) != 0)
      MEMSET(vbp, 0, need);
    return vbp;
  }
  return 0;
}

/****************************************************************************
NAME:  VBList_Allocate

PURPOSE:  Allocate a fresh vbl with a set of VarBind structures

PARAMETERS:     Number of VarBind entries

RETURNS:  VBL_T *       Pointer to varbind list, (VBL_T *)0 on failure
****************************************************************************/
VBL_T *
  VBList_Allocate(int elements)
{
  VBL_T *vblp;

  if ((vblp = (VBL_T *)SNMP_memory_alloc(sizeof(VBL_T))) != 0) {
      MEMSET(vblp, 0, sizeof(VBL_T));
      if ((vblp->vblist = VarBindList_Allocate(elements)) != 0) {
          vblp->vbl_count = elements;
          return vblp;
          }
      }
  SNMP_memory_free((char *)vblp);
  return 0;
}

/****************************************************************************
NAME:  Initialize_Pkt

PURPOSE:  Initialize an SNMP packet structure.

PARAMETERS:
        SNMP_PKT_T *    The packet structure to be initialized

RETURNS:  nothing
****************************************************************************/
void
  Initialize_Pkt(SNMP_PKT_T *rp)
{
  MEMSET(rp, 0, sizeof(SNMP_PKT_T));
  rp->pdu_type = NO_PDU;
  rp->maxpkt = SNMP_MAX_PACKET_SIZE;

  EBufferInitialize(&rp->community);
  
  rp->lockflags = LOCK_NOLOCK;
  rp->gate_decr = 0;
#if INSTALL_ENVOY_CONTINUE_REENTRANT
  ENVOY_SNMP_INIT_LOCK(rp->continue_lock);
#endif
}

/****************************************************************************
NAME:  SNMP_Allocate

PURPOSE:  Allocate a fresh SNMP packet structure and get the world read lock.
          The world read lock will be freed in SNMP_Free.

PARAMETERS:     None

RETURNS:  SNMP_PKT_T *  SNMP Packet structure, (SNMP_PKT_T *)0 on failure
****************************************************************************/
SNMP_PKT_T *
  SNMP_Allocate()
{
  SNMP_PKT_T    *rp;

  rp = (SNMP_PKT_T *)SNMP_memory_alloc(sizeof(SNMP_PKT_T));
  if (rp == 0)
    return 0;

  Initialize_Pkt(rp);

  return rp;
}


