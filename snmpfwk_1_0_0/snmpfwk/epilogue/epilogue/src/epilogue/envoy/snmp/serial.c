/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/serial.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: serial.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:59  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/10/22 03:20:04  sar
 * Modified the installation options for the older sub agent scheme
 *
 * Revision 7.2  1997/03/20 06:49:16  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:48:05  sra
 * Release 6.0.
 *
 * Revision 1.1  1995/04/28  22:46:09  sar
 * Initial revision
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <snmpdefs.h>
#include <snmp.h>
#if (INSTALL_ENVOY_EPI_MASTER)
#include <subagent.h>
#endif

#if (INSTALL_ENVOY_SNMP_SERIALIZE)
extern INT_16_T group_cleanup_flag;

INT_32_T Gate_Readers = 0;
INT_32_T Gate_Writers = 0;
INT_32_T Gate_Running = 0;

typedef struct GATEKEEPER_S
        {
        INT_16_T             opflag; /* the op type that was deferred */
        PTR_T                data;   /* snmp packet or sub agent reg info */
        
        struct GATEKEEPER_S *next;   /* the next entry in the chain */
        GATE_CONTINUE_T     *rtn;    /* routine to run to start the op */
        } GATEKEEPER_T;

GATEKEEPER_T *gateroot = 0;

/****************************************************************************
NAME:  GateKeeper_Test

PURPOSE:   Determine if there are any outstanding requests (if readers
           or writers is non zero).  This routine may be used to determine
           if it is safe to modify the database.

PARAMETERS: none

RETURNS:    int  0 Perform the operation
                 1 The operation should be deferred

****************************************************************************/

int
  GateKeeper_Test(void)
{
if (Gate_Writers || Gate_Readers)
    return(1);
return(0);
}

/****************************************************************************
NAME:  GateKeeper_Add

PURPOSE:   Determine if the requested operation should be carried out
           immediately or deferred until other operations already under
           way are completed.  If the op is deferred it is attached to
           the deferred list.  If the op is for a snmp packet and isn't
           deferred then a counter (readers or writers) is incremented
           to keep tracke of outstanding operations.  These counters
           must be decremented when the operation completes.
          
PARAMETERS:     INT_16_T        request type, snmp packet, sub agent
                                registration or sub agent cleanup.
                PTR_T           Pointer to the data for the operation
                                snmp packet or sub agent block.
RETURNS:        int      0 Perform the operation
                         1 The operation should be deferred
                        -1 Internal failure
****************************************************************************/

int
  GateKeeper_Add(INT_16_T opflag,
                 PTR_T    data,
                 GATE_CONTINUE_T *rtn)
{
SNMP_PKT_T *pktp = (SNMP_PKT_T *)data;
GATEKEEPER_T *gateentry, **gatep;
#if (INSTALL_ENVOY_EPI_MASTER)
if (group_cleanup_flag == 0) {
    if (opflag == GATE_SNMP) {
        switch(pktp->pdu_type) {
            case GET_REQUEST_PDU:
            case GET_NEXT_REQUEST_PDU:
            case GET_BULK_REQUEST_PDU:
                if (Gate_Writers == 0) {
                    pktp->gate_decr = 1;
                    Gate_Readers++;
                    return(0);
                    }
                break;
            case SET_REQUEST_PDU:
                if ((Gate_Writers == 0) && (Gate_Readers == 0)) {
                    pktp->gate_decr = 1;
                    Gate_Writers++;
                    return(0);
                    }
                break;
            }
        }
    else {
        if ((Gate_Writers == 0) && (Gate_Readers == 0))
            return(0);
        }
    }
#else /* (INSTALL_ENVOY_EPI_MASTER) */
switch(pktp->pdu_type) {
   case GET_REQUEST_PDU:
   case GET_NEXT_REQUEST_PDU:
   case GET_BULK_REQUEST_PDU:
        if (Gate_Writers == 0) {
            pktp->gate_decr = 1;
            Gate_Readers++;
            return(0);
            }
        break;
    case SET_REQUEST_PDU:
        if ((Gate_Writers == 0) && (Gate_Readers == 0)) {
            pktp->gate_decr = 1;
            Gate_Writers++;
            return(0);
            }
        break;
    }
#endif /* (INSTALL_ENVOY_EPI_MASTER) */
/* Defer the operation */
gateentry = (GATEKEEPER_T *)SNMP_memory_alloc(sizeof(GATEKEEPER_T));
if (gateentry == 0)
    return(-1);
gateentry->opflag = opflag;
gateentry->data = data;
gateentry->rtn = rtn;
for (gatep = &gateroot; *gatep; gatep = &(*gatep)->next)
    ;
gateentry->next = *gatep;
*gatep = gateentry;
return(1);
}

/****************************************************************************
NAME:  GateRunDeferreds

PURPOSE:   Start up any deferred operations
          
PARAMETERS: void

RETURNS: void
****************************************************************************/

void
  GateRunDeferreds(void)
{
SNMP_PKT_T *pktp;
GATEKEEPER_T *gateentry;
if (Gate_Running || Gate_Writers || Gate_Readers)
    return;

Gate_Running = 1;

#if (INSTALL_ENVOY_EPI_MASTER)
Cleanup_Group();
#endif

gateentry = gateroot;
while(gateentry) {
    if (gateentry->opflag == GATE_SNMP) {
        pktp = (SNMP_PKT_T *)gateentry->data;        
        switch(pktp->pdu_type) {
            case GET_REQUEST_PDU:
            case GET_NEXT_REQUEST_PDU:
            case GET_BULK_REQUEST_PDU:
                if (Gate_Writers) {
                    Gate_Running = 0;
                    return;
                    }
                Gate_Readers++;
                pktp->gate_decr = 1;
                gateentry->rtn(gateentry->data);
                break;
            case SET_REQUEST_PDU:
                if (Gate_Writers || Gate_Readers) {
                    Gate_Running = 0;
                    return;
                    }
                Gate_Writers++;
                pktp->gate_decr = 1;
                gateentry->rtn(gateentry->data);
                break;
            }
        }
    else {
        if (Gate_Writers || Gate_Readers) {
            Gate_Running = 0;
            return;
            }
        gateentry->rtn(gateentry->data);
        }
    gateroot = gateentry->next;
    SNMP_memory_free(gateentry);
    gateentry = gateroot;
    }

Gate_Running = 0;
return;
}

/****************************************************************************
NAME:  GateDecrement

PURPOSE:  Decrement the reader or writer counters when processing of a packet
          completes
          
PARAMETERS: SNMP_PKT_T * The packet we are finishing

RETURNS: void
****************************************************************************/

void
  GateDecrement(SNMP_PKT_T *pktp)
{
if (pktp->gate_decr) {
    if (Gate_Writers)
        Gate_Writers = 0;
    if (Gate_Readers)
        Gate_Readers--;
    }
}

#endif /* (INSTALL_ENVOY_SNMP_SERIALIZE) */
