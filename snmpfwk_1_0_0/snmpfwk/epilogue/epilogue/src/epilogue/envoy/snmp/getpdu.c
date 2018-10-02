/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/getpdu.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $       */

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
 * $Log: getpdu.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:39  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:49  sar
 * Updated revision to 8.0
 *
 * Revision 7.4  1997/10/22 03:10:21  sar
 * Modified the installation options for agentx and the older sub agent scheme
 *
 * Also moved the allocation of the locks into envoy_init (env_init.c),
 * so they can be initialized if necessary.
 *
 * Revision 7.3  1997/10/16 00:45:53  sar
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
 * Revision 7.2  1997/03/20 06:48:56  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/11/01  00:46:48  sar
 * Added a default option in the version case statement to allow
 * new versions such as usec to be added without touching getpdu
 *
 * Revision 6.0  1995/05/31  21:47:24  sra
 * Release 6.0.
 *
 * Revision 5.3  1995/03/21  00:25:01  sar
 * Basically rewritten to use the new async apis.
 *
 * Revision 5.2  1994/09/29  18:37:21  sar
 * If we aren't using the v2 types remove the code to handle them mostly
 * this means ifdef the code.  In mibutils.c and snmp_d.c we need to
 * examine the version of the packet in some cases to reject v2 types for
 * a v1 packet when both v1 and v2 are installed.
 *
 * Revision 5.1  1994/06/17  21:00:45  sar
 * After we are done with the incoming error-status and error_index
 * set them to 0 so we can use it for error replies from gets.
 *
 * Revision 5.0  1994/05/16  15:42:42  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.6  1994/05/12  17:16:29  sar
 * in call_the_get_routine, for get routines that return pointers check
 * to see if the pointer is 0 in which case we zero the object value.
 *
 * Revision 4.5  1994/05/04  22:45:43  sar
 * Added code to allow a failure during the getproc.  This shouldn't be
 * used in general.
 *
 * Revision 4.4  1994/04/29  21:19:45  sar
 * Added some ifdefs for the sub agent scheme which required moving some
 * code around, modified the call to find_object_node and get_ui64.
 *
 * Revision 4.3  1994/01/25  20:46:16  sar
 * SNMPv2 requires a too big error return to have the var binds removed
 * we were not doing this.
 *
 * Revision 4.2  1993/11/11  22:00:51  sar
 * Increment the no such name counter when generating a no such error
 * the testproc failed.
 *
 * Revision 4.1  1993/09/30  20:28:23  sar
 * Added some size testing at the start of the routine to catch packets taht
 * will be too large for get bulk and silent drop them, also modified get
 * bulk to return no var bind packets if size so dictates.  Modified the
 * code to decide the non repeaters and max reps in the face of different
 * word sizes and such.
 *
 * Revision 4.0  1993/06/24  15:45:46  sar
 * Updated revision to 4.0 and copyright to 93
 *
 * Revision 3.18  1993/06/17  23:33:44  sar
 * removed the ENVOY_ from the UI64 routines.
 *
 * Revision 3.17  1993/06/07  18:26:04  sar
 * Modified call to get a 64 bit unsigned int.
 *
 * Revision 3.16  1993/06/02  23:09:24  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.15  1993/05/24  20:38:20  sar
 * Moved the use of vblp to be inside the ifdef v2 areas so we won't
 * get any compiler warnings about not using it.
 *
 * Revision 3.14  1993/05/19  20:25:22  sar
 * Tidied up types to make compilers happy (mostly int to ALENGTH_T).
 *
 * Revision 3.13  1993/05/14  19:02:36  sar
 * Mad maxreps and reps int_32_T and nonreps a size_t to avoid compileer
 * warnings.
 *
 * Revision 3.12  1993/05/03  17:16:12  sar
 * Added routines for local time, removed {party context} oids from packet
 * structure, added asn1len to {party context} structures and
 * SNMP_{Party Context}_Name where needed to lookup the name for a given
 * {Party Context}.
 *
 * Revision 3.11  1993/04/28  22:02:29  sar
 * moved the def of UINT_64_T and added some new types to the routines.
 *
 * Revision 3.10  1993/04/23  23:31:53  sar
 * In getbulk processing added check to see if all of the varbinds are at end of
 * mib.
 *
 * Revision 3.9  1993/04/22  20:06:36  sar
 * Much updating of macros and install options, mostly we now use
 * ISNTALL_ENVOY_SNMP_VERSION_1 or _2, VERSION_RFCXXXX is now SNMP_VERSION_2
 * and other similiar items.
 *
 * Revision 3.8  1993/04/20  19:54:59  sar
 * Added code to handle adding new auth and priv routines, and cleaned up
 * the party set code and a padding argument in the priv structure.
 *
 * Revision 3.7  1993/04/19  18:28:34  sar
 * cleanup, mostly some correcting some non-ansi misspellings and adding
 * static defs.
 *
 * Revision 3.6  1993/03/24  01:01:40  sar
 * added a check on the non repeaters to make sure that we don't go off the
 * end of the vb list if non repeaters > count of var binds.
 *
 * Revision 3.5  1993/03/05  03:58:02  dab
 * fixed up types
 *
 * Revision 3.4  1993/03/01  20:52:16  sar
 * moved some definitions to allow clean version 1 compiles.
 *
 * Revision 3.3  1993/03/01  17:13:33  dab
 * Changed varialbe named index so it doesn't shadow index() routine
 * that some systems have.
 *
 * Revision 3.2  1993/02/24  01:02:41  sar
 * Changed many call to use OIDC_T/lenght pairs instead of OBJ_ID_T as now
 * we don't need to specifically allocate the OBJ_ID_T.
 * Removed the name objid from the PARTYLEAF_T as the name may be determined from
 * the tree itself.  For efficiency purposes put the dest/src party names into
 * the pkt so we don't have to look them up frequently.
 *
 * Revision 3.1  1993/02/17  21:02:56  sar
 * Update stat keeping for v1 and v2 (or both or neither).
 * Added build_error_packet which create an error packet and does some
 * v2 size checking.
 * Updated Process_SNMP_GetClass_Pdu to handle the getbulk case.
 * (The other cases are treated as special cases of the getbulk case.)
 * Added VT_UINTEGER32 value case to the get area.
 *
 * Revision 3.0  92/04/03  19:52:37  dab
 * Release 3.0
 * 
 * Revision 2.108  92/02/24  11:11:51  dab
 * Don't do an extra copy on dynamic returns from get_oid.
 * 
 * Revision 2.107  92/02/24  10:57:37  dab
 * In call_the_get_function, added the dynamic argument to get_oid and
 * the addr argument to get_ip_address.
 * 
 * Revision 2.106  92/01/30  14:02:43  dab
 * Fixed up calls to getprocs so they had the proper procedure prototypes
 * including parameters for those compilers that care.
 * 
 * Revision 2.105  91/10/30  20:41:59  dab
 * Directly include asn1conf.h, snmpdefs.h, and snmpstat.h (if needed).
 * 
 * Revision 2.104  91/09/20  11:17:09  dab
 * Added static argument to getproc calls for strings.
 * 
 * Revision 2.103  91/08/15  12:31:01  dab
 * Removed <libfuncs.h>.
 * 
 * Revision 2.102  91/08/14  10:46:53  dab
 * Added trace functions that Karl put in as macros.
 * 
 * Revision 2.101  91/08/12  12:47:38  dab
 * Cast arguments to memcpy() and memset() to PTR_T.
 * 
 * Revision 2.100  91/08/09  14:08:21  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:23:43  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:06:34
 * Release 2.00
 * 
 *    Rev 1.9   27 Apr 1989 15:56:24
 * Removed unused variables
 * 
 *    Rev 1.8   19 Mar 1989 12:28:28
 * A call to memcpy was being made when the correct routine was memset.
 * This resulted in a copy of 4 bytes from address zero.  This occurred
 * when an attempt to get an IP address failed.
 * 
 *    Rev 1.7   17 Mar 1989 21:41:38
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.6   19 Sep 1988 17:26:06
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.5   19 Sep 1988  9:24:02
 * Reversed change made in revision 1.2
 * 
 *    Rev 1.4   15 Sep 1988 21:35:40
 * On the previous update, not all of the get procedures had the necessary
 * casts added.  This update fixes that.
 * 
 *    Rev 1.3   15 Sep 1988 21:22:06
 * Added casts to calls to the getproc to avoid improper type conversions
 * when "int" and pointers are of different sizes.
 * 
 *    Rev 1.2   15 Sep 1988 20:03:50
 * Recalculated size of outgoing packet before transmission.
 * 
 *    Rev 1.1   14 Sep 1988 17:57:06
 * Moved includes of system include files into libfuncs.h.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:58
 * Initial revision.
*/

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>
#include <objectid.h>

#include <bug.h>

/****************************************************************************
NAME:  SNMP_Process_Get_PDU

PURPOSE:  Process a get type pdu in an async fashion.  First we find objects
          for all the var binds (checking that the objects are in the view).
          If any var binds don't point to a valid object we flag a no such
          event.  For v1 this means an error, for v2 an exception.  After
          we have found all the objects we call the getproc routines to
          find the value for each var bind.

PARAMETERS:
        SNMP_PKT_T *    The decoded GET PDU

RETURNS:  int   0 all of the gets have been started successfully
                1 error of some sort, any error packet has been sent.
****************************************************************************/

int
  SNMP_Process_Get_PDU(SNMP_PKT_T *pktp)
{
VB_T *vbp, *newvbp, *oldvbp;
int   count, indx, eret;
sbits32_t *err_stat;

/* get some initial information */
if (((oldvbp = pktp->pdu.std_pdu.std_vbl.vblist) == 0) ||
    ((count = pktp->pdu.std_pdu.std_vbl.vbl_count) == 0))
    return(0);

/* We will be looking at the error status a lot so we get a local pointer
   to it to make things easier */
err_stat = &(pktp->pdu.std_pdu.error_status);

/* assume that everything will work */
*err_stat = NO_ERROR;
pktp->pdu.std_pdu.error_index = 0;

/* Copy the vb list so we can use the original list if we have an error */
if ((newvbp = VarBindList_Allocate(count)) == 0) {
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return(1);
    }
for(indx = count, vbp = newvbp; indx; indx--, oldvbp++, vbp++) {
    vbp->vb_data_flags_n_type = VT_EMPTY;
    if (clone_object_id(&oldvbp->vb_obj_id, &vbp->vb_obj_id))
        break;
    }
if (indx != 0) {
    /* we had an error trying to clone an object id (probably couldn't
       get the space), clean up any vbs we modified and free the newvbp
       space */
    for(vbp = newvbp; indx < count; indx++, vbp++)
        Clean_vb(vbp);
    SNMP_memory_free(newvbp);
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return(1);
    }

/* We have successfully copied the sections of the vblist we need
   move the old vblist to the saved area and install the new vblist
   into the current area */
pktp->pdu.std_pdu.saved_vbl.vbl_count = count;
pktp->pdu.std_pdu.saved_vbl.vblist = pktp->pdu.std_pdu.std_vbl.vblist;
pktp->pdu.std_pdu.std_vbl.vblist = newvbp;

/* Attempt to find all the requested objects */

/* Try to get the infrastructure lock, we release it using
   the ENVOY_AX_MA_RELEASE_WRITE_LOCK macro */
#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
if (ENVOY_SNMP_GET_WRITE_LOCK(SNMP_infrastructure_lock)) {
    BUG(BUG_ENVOY_LOCKING, BUG_CONTINUABLE, 0,
        (BUG_OUT, "SNMP_Process_Get_PDU: infrastructure lock broken", 0));
    ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
    return(1);
    }
#endif

for(indx = 0, vbp = newvbp; indx < count; indx++, vbp++) {
    eret = find_object_node(vbp, pktp, 1);

#if (INSTALL_ENVOY_AGENTX_MASTER)
    if (eret == -3) {
        ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
        ENVOY_Send_SNMP_Error_Packet(pktp, GEN_ERR, 0);
        return(1);
        }
#endif        

    if ((eret != 0) ||
        ((vbp->vb_ml.ml_flags & ML_IS_LEAF) == 0) ||
        (((vbp->vb_ml.ml_leaf)->access_type & READ_ACCESS) == 0)) {
        switch(pktp->snmp_version) {
#if INSTALL_ENVOY_SNMP_VERSION_1
            case SNMP_VERSION_1:
                ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);
                /* If version 1 we send a NO_SUCH_NAME error back.
                   We set up the packet for a nosuch name error,
                   then attempt to send it */
                ENVOY_Send_SNMP_Error_Packet(pktp, NO_SUCH_NAME,
                                             SNMP_ERROR_INDEX(indx));
                return(1);
#endif /* INSTALL_ENVOY_SNMP_VERSION_2 */

#if INSTALL_ENVOY_SNMP_VERSION_2
            case SNMP_VERSION_2:
            default:
                /* If not version 1 we send a NO_SUCH_OBJECT exception back.
                   We set the data_flags_n_type of the current vbp to
                   NO_SUCH_OBJECT, tag the vbp as having been done
                   and continue with the rest */
                vbp->vb_data_flags_n_type = VT_NOSUCHOBJ;
                vbp->vb_flags |= VFLAG_GET_STARTED | VFLAG_GET_DONE;
                break;
#endif /* INSTALL_ENVOY_SNMP_VERSION_2 */
            }
        } /* if ((find_object...) */
    } /* for(indx = 0...) */
ENVOY_AX_MA_RELEASE_WRITE_LOCK(SNMP_infrastructure_lock);

/* Now we loop through the vblist again and start all the gets */
for(vbp = newvbp; count; count--, vbp++) {
    /* If the get started flag is set then either we set it above or
       the getproc of some other var bind has claimed responsibility for
       this var bind.  In either case we skip this var bind and continue */
    if (vbp->vb_flags & (VFLAG_GET_STARTED | VFLAG_GET_DONE))
        continue;      
    ((vbp->vb_ml.ml_leaf)->getproc)
      (vbp->vb_ml.ml_last_match, vbp->vb_ml.ml_remaining_objid.num_components,
       vbp->vb_ml.ml_remaining_objid.component_list, pktp, vbp);
    vbp->vb_flags |= VFLAG_GET_STARTED;

    /* test the error code, if we had an error then mark any remaining
       vbs as having been gotten (so we won't trip over them later).
       We need to skip the first vb as that is the one that
       caused the failure.  Then break because we are done */
    if (*err_stat) {
        for (vbp++, count--; count; count--, vbp++)
            if (!(vbp->vb_flags & VFLAG_GET_STARTED))
                vbp->vb_flags |= VFLAG_GET_STARTED | VFLAG_GET_DONE;
        break;
        }
    }

return(0);
}

/*lint +e715    */

