/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/snmpdefs.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $        */

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
 * $Log: snmpdefs.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:28  sar
 * Updated revision to 8.0
 *
 * Revision 7.6  1997/10/22 03:07:29  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.5  1997/10/16 23:52:54  sar
 * We need to document that using constant strings in the getproc_got_string
 * macro could lead to unhappiness.
 *
 * We added a macro to allow customers to define what SNMPADDR_T should be,
 * and increased the length of the string to 16 bytes in the default case.
 *
 * changed testproc_error to be a routine
 *
 * Revision 7.4  1997/10/16 00:25:18  sar
 * Primarily this set of modifications is for the agentx protocol.
 * Most of the work was in the agentx.h and ax_mth.h files.  Most
 * other files (snmpdefs.h, vbdefs.h and mib.h) received updates
 * to their structures and snmp got some new lock macros.
 *
 * One other change of note was the modification of continue
 * processing.  Previously we called a specific continue routine
 * directly, now we store a pointer to the corret continue
 * function in the snmp packet structure and call that.  We include
 * macros for backwards compatibility.
 *
 * Revision 7.3  1997/03/20 06:48:17  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1996/10/15 14:40:22  sar
 * Added process_internal_snmp_packet
 *
 * Revision 7.0  1996/03/18  20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.5  1996/01/23  16:26:19  sar
 * Changed usec_report flag from 0x40 to 0x04
 *
 * Revision 6.4  1996/01/10  16:23:25  sar
 * USEC updates
 *
 * Revision 6.3  1995/11/14  22:42:26  sar
 * Replaced proxy_trap_{v1 v2} with proxy_traps_and_reports
 *
 * Revision 6.2  1995/11/04  01:52:03  sar
 * Added the proxy routine type and a proxy_routine field in
 * the snmp_pkt_t structure if proxy is installed.
 *
 * Revision 6.1  1995/10/20  22:53:35  sar
 * removed bitstring include file & macro
 * removed party info from snmp_pkt structure
 * added report pdu tag
 *
 * Revision 6.0  1995/05/31  21:46:51  sra
 * Release 6.0.
 *
 * Revision 5.10  1995/05/22  19:41:18  sar
 * Added error check to make sure that the lock option is installed
 * if the continue_reentrant option is installed.
 *
 * Revision 5.9  1995/05/08  20:31:09  sar
 * changed the args to nextproc_next_instance & getproc_got_object_id
 * to make compilers happier.
 *
 * Revision 5.8  1995/05/06  01:39:54  sar
 * We now include auxmth.h and some other method routine defs so that the
 * mib compiler won't need to generate defs for routines included in auxi.
 *
 * Revision 5.7  1995/05/05  23:26:09  sar
 * Set the vb_data_n_type flag for getproc_got_bitstring.
 *
 * Revision 5.6  1995/04/28  22:42:16  sar
 * Added definitions for use with the serialization (gatekeeper) code.
 *
 * Revision 5.5  1995/03/20  23:11:33  sar
 * Moved the vb specific information to vbdefs.h and included that file here.
 * Added declarations for the routines & macros that are used in the
 * new async scheme.  This included adding several variables to the
 * snmp_pkt structure and the std_pdu structure.
 * Modified the party names to be "for" and "loc" instead of "src" and "dest".
 *
 * Revision 5.4  1994/10/12  20:12:36  sar
 * made the trap_time_ticks a uint_32 instead of int_32
 *
 * Revision 5.3  1994/09/29  18:31:12  sar
 * Rearrange some items to deal with removing v2 types from v1 settings if
 * they aren't installed.
 *
 * Revision 5.2  1994/07/29  18:08:12  sar
 * Added MAX_V1_PDU for use in type checking v1 pdus.
 *
 * Revision 5.1  1994/07/28  23:29:12  sar
 * Added a field in the SNMP_PKT_T structure for holding a pointer to
 * the mib tree to use for this packet.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.7  1994/05/09  21:02:30  sar
 * Added sub_agent option to #if clause in deciding to include sa_flags and
 * vb_access.
 *
 * Revision 4.6  1994/05/06  20:29:54  sar
 * Took out the ifdef v1 for community string as we know use if to hold
 * the v2 localentity string.
 *
 * Revision 4.5  1994/05/04  23:47:31  sar
 * made VBPRIVPROC's always take a PTR_T as argument (SA_freeproc for one).
 *
 * Revision 4.4  1994/05/04  22:43:48  sar
 * modified the call_the_set_routine calling sequence.
 *
 * Revision 4.3  1994/05/02  21:19:26  sar
 * Added a localtimeobj in snmp_pkt_t for use with sub agent type stuff.
 *
 * Revision 4.2  1994/04/29  20:00:21  sar
 * Mostly sub agent stuff, added new fields to snmp_pkt structure
 * added externs for several routines, modified the vbprivproc definition
 * for use with the VB_FREE_COMPAT option.
 *
 * Revision 4.1  1993/10/06  19:47:11  sar
 * added the TPROC_READ_ONLY and TPROC_TOO_BIG flags for use with error_response
 * modified tproc_set_error to use vb_to_index + 1 to get the index's to
 * be 1 based (what snmp uses) rather than 0 based (what envoy internals use).
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.20  1993/06/07  18:31:09  sar
 * Made SNMP_Decode_Packet a macro calling SNMP_Decode_Packet_WER
 * which is newly defined.
 *
 * Revision 3.19  1993/06/02  23:05:55  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.18  1993/05/31  17:45:26  sar
 * fixed a typo con_notloal to con_notlocal
 *
 * Revision 3.17  1993/05/19  20:07:52  sar
 * tiding up types to make compilers happy, (timestamps to UINT_32_T,
 * sizes to ALENGTH_T).
 *
 * Revision 3.16  1993/05/14  15:13:29  sar
 * renamed private field in packet structure to be user_private
 *
 * Revision 3.15  1993/05/03  17:14:17  sar
 * Modified context local time to allow it to be set and got rid of party and
 * context oids in the packet structure which leads to asn1len in the
 * party and context structure.
 *
 * Revision 3.14  1993/04/30  22:58:26  sar
 * Added mechanism for minimal proxies and coarse grained locks.
 *
 * Revision 3.13  1993/04/28  22:00:38  sar
 * Moved the definition of UINT_64_T to ui64.h and update others
 *
 * Revision 3.12  1993/04/27  21:06:48  sar
 * Removed index_to_vbp it is now in buildpkt.h
 * used qualifier for the include names (envoy/h/...)
 *
 * Revision 3.11  1993/04/26  20:33:14  sar
 * Added bitstrings, and have deletion of parties or contexts delete acls.
 *
 * Revision 3.10  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 3.9  1993/04/21  15:31:44  dab
 * removed define __() macro since everyone should get this from <types.h>
 *
 * Revision 3.8  1993/04/17  01:03:30  sar
 * changes for shadow area row creation and updating of some error codes.
 *
 * Revision 3.7  1993/03/25  21:15:21  sar
 * Added types for uint 64 (counter64) and nsapaddress (translates to string)
 *
 * Revision 3.6  1993/02/26  22:36:17  sar
 * Changed INCONISTENT_VALUE to INCONSISTENT_VALUE
 *
 * Revision 3.5  1993/02/26  20:27:30  sar
 * added inconsistent name to the list of pdu errors.
 *
 * Revision 3.4  1993/02/26  17:29:49  sar
 * added defines for MAX_PDU and ALL_CLASS to allow writing code that
 * won't have to change if we add new pdu types.
 *
 * Revision 3.3  1993/02/24  00:59:50  sar
 * Removed the name objid from the partyleaf and put a copy of it in the
 * pkt.  Also Updated many of the SNMP_(Party, Context, ACL)_* files
 * to use a OIDC_T/length instead of a OBJ_ID_T so we don't have to
 * allocate an OBJ_ID_T when we don't have one.
 *
 * Revision 3.2  1993/02/17  20:52:52  sar
 * Added no_pp definition (#define _(x) x stuff)
 * Added more tags (nsapaddress, counter64, uinteger32 and the exception
 * tags nosuchobj, nosuchins, and endofmib).
 * Added a VBL_S pointer to the VBL_S structure for get bulk chaining.
 * Added new defines for new classes and errors
 * Added SNMP_Encode_Auth_Msg - definition for encoding auth messages.
 * Updated SNMP_PKT_S to contain version 2 options.
 *
 * Revision 3.1  92/07/31  11:55:02  dab
 * Started changes for parties for Secure SNMP.
 * 
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.106  92/02/27  16:47:00  dab
 * Added return code #define's for the TESTPROC.
 * 
 * Revision 2.105  91/11/07  09:37:45  dab
 * Changed the view mask to be 32 bits.
 * 
 * Revision 2.104  91/11/05  17:32:12  dab
 * Added VB_TO_INDEX() macro.
 * 
 * Revision 2.103  91/10/30  20:45:46  dab
 * Used to be snmp.h.  Now the config file is snmp.h and all the
 * SNMP definitions are here.
 * 
 * Revision 2.102  91/09/12  13:50:51  dab
 * Re-ordered so SNMP_STATS_T is defined after snmpconf.h is included
 * so it gets the proper definition for SGRP.
 * 
 * Revision 2.101  91/08/15  15:41:46  dab
 * Pulled configuration from asn1.h and snmp.h and put in
 * snmpconf.h and asn1conf.h.
 * 
 * Revision 2.100  91/08/09  14:09:48  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:31  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.3   17 Jun 1990 10:39:42
 * Corrected erroneous definition of inc_gauge and dec_gauge macros.
 * 
 *    Rev 2.2   06 Jun 1990  5:32:40
 * Added definition of RFC1157.
 * 
 *    Rev 2.1   24 May 1990 16:40:52
 * Changed the authentication enable flag in snmp_stats from a long to a short.
 * 
 *    Rev 2.0   31 Mar 1990 15:11:24
 * Release 2.00
 * 
 *    Rev 1.9   24 Apr 1989 18:42:00
 * Added definition of VERSION_RFC1098 as a synonym for VERSION_RFC1067.
 * 
 *    Rev 1.8   24 Mar 1989 17:25:30
 * Removed reference to the variable named "etc_copyright".  The in-core
 * copyright notice now comes in via module "snmp_d.c".
 * 
 *    Rev 1.7   17 Mar 1989 21:42:02
 * Calls to memcpy/memset protected against zero lengths
 * 
 *    Rev 1.6   11 Jan 1989 13:23:32
 * Added a reference to the copyright string to force it into memory.
 * 
 *    Rev 1.5   11 Jan 1989 12:46:44
 * Moved Clean_Obj_ID() to objectid.c
 * 
 *    Rev 1.4   19 Sep 1988 17:27:12
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.3   17 Sep 1988 14:40:56
 * Corrected the SNMP version number -- the format in RFC1067 is poorly
 * done!!!!!!!
 * 
 *    Rev 1.2   15 Sep 1988 20:04:22
 * Added macro to convert VBL offset into an error index.
 * 
 *    Rev 1.1   14 Sep 1988 19:23:30
 * Added definitions for the UDP port numbers assigned to SNMP.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:22
 * Initial revision.
*/

#if (!defined(snmpdefs_inc))
#define snmpdefs_inc

#if (!defined(asn1_inc))
#include <envoy/h/asn1.h>
#endif

#if (!defined(buffer_inc))
#include <envoy/h/buffer.h>
#endif

#if (!defined(mib_inc))
#include <envoy/h/mib.h>
#endif

#if (!defined(ui64_inc))
#include <envoy/h/ui64.h>
#endif

#if (!defined(vb_inc))
#include <envoy/h/vbdefs.h>
#endif

#define VERSION_RFC1067                 0 /* Classic SNMP */
/* RFC1067, RFC1098, and RFC1157 are equivalent */
#define VERSION_RFC1098                 0
#define VERSION_RFC1157                 0
#define SNMP_VERSION_1                  0 /* now being tagged as version 1 */
#define SNMP_VERSION_2                  1 /* SNMPV2 */
#define SNMP_VERSION_USEC               2 /* User Security option */

/* Define the ports assigned to SNMP for the reception of various types */
/* of packets.                                                          */
#define SNMP_REQUEST_PORT               161
#define SNMP_TRAP_PORT                  162

/* The following represents a source or destination address in
   a generalized form (which happens to be isomorphic to the
   familiar sockaddr structure found with the socket interface.)
   It is the default Envoy will use for an address structure,
   however it may be overridden by defining ENVOY_SNMPADDR_T. */
struct SNMPADDR_S
        {
        unsigned short  snmp_family;
        unsigned char   snmp_data[16];
        };

#ifdef ENVOY_SNMPADDR_T
typedef ENVOY_SNMPADDR_T SNMPADDR_T;
#else
typedef struct SNMPADDR_S SNMPADDR_T;
#endif

/* Typedefs for io and error completion routines */
typedef void IO_COMPLETE_T  __((SNMPADDR_T *for_addr, SNMPADDR_T *loc_addr,
                                PTR_T pktp, ALENGTH_T need, PTR_T cookie));
typedef void ERR_COMPLETE_T __((SNMPADDR_T *for_addr, SNMPADDR_T *loc_addr,
                                int error_code, PTR_T cookie));

/* The proxy routine really takes an argument of SNMP_PKT_T *
   but we can't use it as we haven't defined it */
typedef INT_32_T PROXY_ROUTINE_T __(( ));

typedef void AUTH_ROUTINE_T __((OCTET_T *, UINT_16_T, OCTET_T *,
                                UINT_16_T, OCTET_T *));

/* Define the internal representation of an SNMP packet */
typedef struct SNMP_PKT_S
        {
        ALENGTH_T       buffer_needed;  /* Size of buffer needed   */
        ALENGTH_T       overall_length; /* Message Sequence length */
        UINT_32_T       mib_view;       /* Set of mib views which this  */
                                        /* request can see. Use 0xFFFFFFFF */
                                        /* to participate in all views. */
        OCTET_T         flags;          /* See below.                   */
        UINT_16_T       lcl_ident;      /* Local transaction identifer  */
        SNMPADDR_T      pkt_src;        /* Where this packet came from. */
        SNMPADDR_T      pkt_dst;        /* Where this packet came to.   */
        IO_COMPLETE_T   *io_complete;    /* Routines to call when processing */
        ERR_COMPLETE_T  *error_complete; /* is complete */
        void (*cont)(struct SNMP_PKT_S *); /* routine to call continue proc */
        PTR_T           async_cookie;   /* cookie for use with completions */
        OCTET_T         phase;          /* what phase we are running, should
                                           be the same size as vb_flags */
        ALENGTH_T       maxpkt;         /* the maximum packet we can build */
        char *          user_private;   /* Anything one wants -- but    */
                                        /* remember, if you attach      */
                                        /* something here, you got to   */
                                        /* free it in release_private!! */
        INT_32_T        snmp_version;
        UINT_16_T       view_index;
        EBUFFER_T       community;      /* used for the community string  */
        ATVALUE_T       pdu_type;
        ALENGTH_T       pdu_length;
        union {
              /* For GetRequest, GetNextRequest,        */
              /* GetResponse, SetRequest,               */
              /* GetBulkRequest, InformRequest,         */
              /* and Trap2 PDUs.                        */

              struct {
                     INT_32_T   request_id;
                     INT_32_T   error_status;
                     INT_32_T   error_index;
                     VBL_T      std_vbl;
                     VBL_T      saved_vbl;
                     INT_32_T   max_reps;
                     int        non_reps;
                     } std_pdu;

#if INSTALL_ENVOY_SNMP_VERSION_1
              /* For Trap PDU   */
              struct {
                     OBJ_ID_T           enterprise_objid;
                     unsigned char      net_address[4];
                     INT_32_T           generic_trap;
                     INT_32_T           specific_trap;
                     UINT_32_T          trap_time_ticks;
                     VBL_T              trap_vbl;
                     } trap_pdu;
#endif
              } pdu;
        MIBNODE_T      *mib_root;
        OCTET_T         lockflags;
        OCTET_T         gate_decr;
#if INSTALL_ENVOY_SNMP_LOCK
        ENVOY_LOCK_T   *coarse_lock;
#endif
#if INSTALL_ENVOY_AGENTX
        struct ENVOY_AX_PKT_S  *ax_pkt;
        bits32_t                ax_trans_id;
#endif
#if INSTALL_ENVOY_CONTINUE_REENTRANT
        ENVOY_LOCK_T    continue_lock;
#endif
#if INSTALL_ENVOY_EPI_SUB
        OCTET_T         sa_opcode2;
        OCTET_T         sa_encoding;
#endif
#if INSTALL_ENVOY_SNMP_PROXY
        PROXY_ROUTINE_T *proxy_routine;
#endif
#if INSTALL_ENVOY_SNMP_VERSION_USEC
        AUTH_ROUTINE_T  *auth_routine;
        EBUFFER_T        user;
        EBUFFER_T        context;
        EBUFFER_T        auth_key;
        OCTET_T          agentid[12];
#endif
        } SNMP_PKT_T;


/* Values for pdu_type (class and form bits are not included) */
#define GET_REQUEST_PDU                 0
#define GET_NEXT_REQUEST_PDU            1
#define GET_RESPONSE_PDU                2
#define SET_REQUEST_PDU                 3
#define TRAP_PDU                        4
#define GET_BULK_REQUEST_PDU            5
#define INFORM_REQUEST_PDU              6
#define TRAP2_PDU                       7
#define REPORT_PDU                      8
#define MAX_V1_PDU                      4
#define MAX_PDU                         8
#define NO_PDU                       0xFF

/* values for pdu classes in mgt communication class, this is equal
   to 2 to the value of the context tag for the pdu                  */
#define GET_REQUEST_CLASS                 1
#define GET_NEXT_REQUEST_CLASS            2
#define GET_RESPONSE_CLASS                4
#define SET_REQUEST_CLASS                 8
#define TRAP_CLASS                       16
#define GET_BULK_REQUEST_CLASS           32
#define INFORM_REQUEST_CLASS             64
#define TRAP2_CLASS                     128
#define REPORT_CLASS                    256
#define ALL_CLASS                       511

/* Values for error_status      */
#ifndef NO_ERROR
#define NO_ERROR                 0
#endif
#define TOO_BIG                  1
#define NO_SUCH_NAME             2
#define BAD_VALUE                3
#define READ_ONLY                4
#define GEN_ERR                  5
#define NO_ACCESS                6
#define WRONG_TYPE               7
#define WRONG_LENGTH             8
#define WRONG_ENCODING           9
#define WRONG_VALUE             10
#define NO_CREATION             11
#define INCONSISTENT_VALUE      12
#define RESOURCE_UNAVAILABLE    13
#define COMMIT_FAILED           14
#define UNDO_FAILED             15
#define AUTHORIZATION_ERROR     16
#define NOT_WRITABLE            17
#define INCONSISTENT_NAME       18
#define LAST_ERROR              19

/* Values for generic_trap      */
#define COLD_START              0
#define WARM_START              1
#define LINK_DOWN               2
#define LINK_UP                 3
#define AUTH_FAILURE            4
#define EGP_NEIGHBOR_LOSS       5
#define ENTERPRISE_SPECIFIC     6

/* For lock flags in the packet structre */
#define LOCK_NOLOCK 0x00
#define LOCK_READ   0x01
#define LOCK_WRITE  0x02

/* Values for qos in usec */
#define USEC_AUTH   0x01
#define USEC_PRIV   0x02
#define USEC_REPORT 0x04

/* macro to translate a vbp into an index, this is included for
   backwards compatibility, the preferred method is to call
   vbp_to_index directly */
#define VB_TO_INDEX(pkt, vbp)    vbp_to_index(pkt, vbp)

/* various vb utility routines */
extern  int     vbp_to_index    __((SNMP_PKT_T *, VB_T *));
extern  VB_T *  index_to_vbp    __((SNMP_PKT_T *, int));

/* Return values from TEST routine */
#define TPROC_GOOD                       0
#define TPROC_NOSUCH                    -1
#define TPROC_GENERR                    -2
#define TPROC_BADVALUE                  -3
#define TPROC_READ_ONLY                 -4
#define TPROC_TOO_BIG                   -5

#define TPROC_NO_ACCESS                 -NO_ACCESS
#define TPROC_WRONG_TYPE                -WRONG_TYPE
#define TPROC_WRONG_LENGTH              -WRONG_LENGTH
#define TPROC_WRONG_ENCODING            -WRONG_ENCODING
#define TPROC_WRONG_VALUE               -WRONG_VALUE
#define TPROC_NO_CREATION               -NO_CREATION
#define TPROC_INCONSISTENT_VALUE        -INCONSISTENT_VALUE
#define TPROC_RESOURCE_UNAVAILABLE      -RESOURCE_UNAVAILABLE
#define TPROC_COMMIT_FAILED             -COMMIT_FAILED
#define TPROC_UNDO_FAILED               -UNDO_FAILED
#define TPROC_AUTHORIZATION_ERROR       -AUTHORIZATION_ERROR
#define TPROC_NOT_WRITABLE              -NOT_WRITABLE
#define TPROC_INCONSISTENT_NAME         -INCONSISTENT_NAME
#define TPROC_LAST_ERROR                -LAST_ERROR

#define TPROC_BADVALUE_INDEX(I)  (I + 1)
#define TPROC_SET_ERROR(P, V, E) ((P)->pdu.std_pdu.error_index = \
                                                (vbp_to_index(P, V) + 1),\
                              (P)->pdu.std_pdu.error_status = (INT_32_T)(E),\
                              TPROC_LAST_ERROR)
#define SNMP_ERROR_INDEX(I)     (I + 1)

/* Defines for the security violations in the decode and auth code */
/* Decode first */
#define SNMP_DEC_VIO_UNKNOWN_DST  1
#define SNMP_DEC_VIO_DST_NOTLOCAL 2
#define SNMP_DEC_VIO_UNKNOWN_SRC  3
#define SNMP_DEC_VIO_FAILED_VFY   4
#define SNMP_DEC_VIO_UNKNOWN_CON  5
#define SNMP_DEC_VIO_CON_NOTLOCAL 6
#define SNMP_DEC_VIO_BAD_OPR1     7
#define SNMP_DEC_VIO_BAD_OPR2     8
#define SNMP_DEC_VIO_MAX          8

/* auth next */
#define SNMP_AUTH_VIO_LIFETIME   1
#define SNMP_AUTH_VIO_FAILED_VFY 2

/* Macros to manipulate Integers, Counters, and Gauges. */
/* These assume that these are held as longs.           */
/* Integers are signed, counters and gauges are not.    */
#define inc_integer(I)          ((I)++)
#define dec_integer(I)          ((I)--)
#define add_integer(I,V)        ((I) += (V))
#define sub_integer(I,V)        ((I) -= (V))

/* Counter's can't be decremented */
#define inc_counter(C)          (C++)
#define add_counter(C,V)        (C += (V))

/* Gauges latch at the maximum value */
#define GGMAX   ((unsigned long)0xFFFFFFFF)
#define inc_gauge(G)    ((unsigned long)(G) != GGMAX ? G++ : G)
#define dec_gauge(G)    ((unsigned long)(G) != GGMAX ? G-- : G)
#define add_gauge(G,V)  (G =    \
                        (unsigned long)(V) > (GGMAX - (unsigned long)(G)) ? \
                                GGMAX : \
                                (unsigned long)(G) + (unsigned long)(V))
#define sub_gauge(G,V)  (G -= ((unsigned long)(G) != GGMAX ?    \
                                        (unsigned long)(V) : 0))

#define SNMP_Decode_Packet(buf, len, saddr, daddr) \
                SNMP_Decode_Packet_WER(buf, len, saddr, daddr, 0)

extern  void    envoy_init __((void));

extern  int     Process_Received_SNMP_Packet __((unsigned char *, int,
                                                 SNMPADDR_T *, SNMPADDR_T *,
                                                 EBUFFER_T *));
extern  void    Process_Rcvd_SNMP_Packet_Async __((size_t, bits8_t *,
                                                   SNMPADDR_T *, SNMPADDR_T *,
                                                   ALENGTH_T, IO_COMPLETE_T *,
                                                   ERR_COMPLETE_T *, PTR_T));
#define SNMP_Continue(PKTP) ((PKTP)->cont(PKTP))

extern void     Process_Internal_SNMP_Packet   __((SNMP_PKT_T *));

extern  SNMP_PKT_T *    SNMP_Decode_Packet_WER __((unsigned char *, int,
                                                   SNMPADDR_T *, SNMPADDR_T *,
                                                   int *));
extern  SNMP_PKT_T *    SNMP_Allocate        __((void));
extern  void            SNMP_Free            __((SNMP_PKT_T *));
extern  void            Initialize_Pkt       __((SNMP_PKT_T *));

extern  ALENGTH_T       SNMP_Bufsize_For_Packet  __((SNMP_PKT_T *));
extern  int             SNMP_Encode_Pkt_With_Siz __((SNMP_PKT_T *,
                                                     EBUFFER_T *,
                                                     ALENGTH_T));
extern  int             SNMP_Encode_Packet   __((SNMP_PKT_T *, EBUFFER_T *));
extern  int             SNMP_Encode_Auth_Msg __((SNMP_PKT_T *, EBUFFER_T *));
extern  void            SNMP_Proxy_Request   __((SNMP_PKT_T *));
extern  void            SNMP_Proxy_Response  __((SNMP_PKT_T *));
extern  void            SNMP_Proxy_Traps_And_Reports __((SNMP_PKT_T *));

extern  int             SNMP_Process_Get_PDU  __((SNMP_PKT_T *));

extern  int             SNMP_Process_Test_PDU __((SNMP_PKT_T *));
extern  int             SNMP_Process_Set_PDU  __((SNMP_PKT_T *));

extern  int             SNMP_Process_Next_PDU  __((SNMP_PKT_T *));
extern  int             SNMP_Process_Next_Redo __((SNMP_PKT_T *));
extern  int             SNMP_Process_Bulk_Redo __((SNMP_PKT_T *));

extern  int             SNMP_Process_Finish __((SNMP_PKT_T *, EBUFFER_T *,
                                                ALENGTH_T));

extern  void            ENVOY_Send_SNMP_Packet       __((SNMP_PKT_T *));
extern  void            ENVOY_Send_SNMP_Error_Packet __((SNMP_PKT_T *,
                                                         INT_32_T, INT_32_T));

extern  int             scan_vb_for_locator __((SNMP_PKT_T *, int, UINT_16_T));
extern  int             oidcmp              __((int, OIDC_T *, int, OIDC_T *));
extern  int             find_object_node    __((VB_T *, SNMP_PKT_T *, int));
extern  int             find_next_object    __((VB_T *, OBJ_ID_T *,
                                                SNMP_PKT_T *, int));

#define getproc_started(P, V)   ((V)->vb_flags |= VFLAG_GET_STARTED)
#define getproc_good(P, V)      ((V)->vb_flags |= VFLAG_GET_DONE)
#define getproc_error(P, V, E)  ((P)->pdu.std_pdu.error_status = E, \
                                 (V)->vb_flags |= VFLAG_GET_DONE)

#define nextproc_started(P, V)  ((V)->vb_flags |= VFLAG_NEXT_STARTED)
#define nextproc_good(P, V)     ((V)->vb_flags |= VFLAG_NEXT_DONE)
#define nextproc_no_next(P, V)  ((V)->vb_flags |= VFLAG_NEXT_DONE | \
                                                    VFLAG_NEXT_REDO)
#define nextproc_error(P, V, E) ((P)->pdu.std_pdu.error_status = E, \
                                 (V)->vb_flags |= VFLAG_NEXT_DONE)

#define getproc_got_int32(P, V, D) ((V)->vb_data_flags_n_type = VT_NUMBER, \
                                    (V)->value_u.v_number = D, \
                                    (V)->vb_flags |= VFLAG_GET_DONE)
#define getproc_got_uint32(P, V, D, T) ((V)->vb_data_flags_n_type = T,\
                                        (V)->value_u.v_counter = D, \
                                        (V)->vb_flags |= VFLAG_GET_DONE)
#define getproc_got_uint64(P, V, D) \
                                ((V)->vb_data_flags_n_type = VT_COUNTER64, \
                                 (V)->value_u.v_counter64.high = (D)->high, \
                                 (V)->value_u.v_counter64.low  = (D)->low, \
                                 (V)->vb_flags |= VFLAG_GET_DONE)
#define getproc_got_uint64_high_low(P, V, H, L) \
                                ((V)->vb_data_flags_n_type = VT_COUNTER64, \
                                 (V)->value_u.v_counter64.high = H, \
                                 (V)->value_u.v_counter64.low  = L, \
                                 (V)->vb_flags |= VFLAG_GET_DONE)
#define getproc_got_ip_address(P, V, D) \
                                ((V)->vb_data_flags_n_type = VT_IPADDRESS, \
                                 MEMCPY(&((V)->value_u.v_network_address), \
                                        &(D), 4), \
                                 (V)->vb_flags |= VFLAG_GET_DONE)
#define getproc_got_empty(P, V) ((V)->vb_data_flags_n_type = VT_EMPTY, \
                                 (V)->vb_flags |= VFLAG_GET_DONE)
#define getproc_got_string(P, V, S, D, F, T) \
                        ((V)->vb_data_flags_n_type = T, \
                         (V)->vb_flags |= VFLAG_GET_DONE, \
                         EBufferPreLoad((F) ? BFL_IS_DYNAMIC : BFL_IS_STATIC,\
                                        &((V)->value_u.v_string), D, S))
extern void     getproc_nosuchins       __((SNMP_PKT_T *, VB_T *));
extern void     getproc_got_object_id   __((SNMP_PKT_T *, VB_T *, int,
                                            OIDC_T *, int));
extern void     nextproc_next_instance  __((SNMP_PKT_T *, VB_T *, int,
                                            OIDC_T *));
extern void     testproc_tproc_error    __((SNMP_PKT_T *, VB_T *, int));
extern void     testproc_error          __((SNMP_PKT_T *, VB_T *, sbits32_t));

#define testproc_started(P, V)  ((V)->vb_flags |= VFLAG_TEST_STARTED)
#define testproc_good(P, V)     ((V)->vb_flags |= VFLAG_TEST_DONE)
#define setproc_started(P, V)   ((V)->vb_flags |= VFLAG_SET_STARTED)
#define setproc_good(P, V)      ((V)->vb_flags |= VFLAG_SET_DONE)
#define setproc_error(P, V, E)  ((P)->pdu.std_pdu.error_status = E,      \
                                 (V)->vb_flags |= VFLAG_SET_DONE)
#define undoproc_set(P, V, R)   ((V)->undoproc = (UNDOPROC_T *)(R))
#define undoproc_started(P, V)  ((V)->vb_flags |= VFLAG_UNDO_STARTED)
#define undoproc_good(P, V)     ((V)->vb_flags |= VFLAG_UNDO_DONE)
#define undoproc_error(P, V, E) ((P)->pdu.std_pdu.error_status = E,      \
                                 (V)->vb_flags |= VFLAG_UNDO_DONE)
#define setproc_all_bits(P, V)  ((V)->vb_flags |= VFLAG_TEST_STARTED |  \
                                                  VFLAG_TEST_DONE |     \
                                                  VFLAG_SET_STARTED |   \
                                                  VFLAG_SET_DONE |      \
                                                  VFLAG_UNDO_STARTED |  \
                                                  VFLAG_UNDO_DONE)

#if (INSTALL_ENVOY_SNMP_SERIALIZE)
typedef void GATE_CONTINUE_T __((PTR_T));

extern int  GateKeeper_Test     __((void));
extern int  GateKeeper_Add      __((INT_16_T, PTR_T, GATE_CONTINUE_T *));
extern void GateRunDeferreds    __((void));
extern void GateDecrement       __((SNMP_PKT_T *));

#define GATE_SNMP     1
#define GATE_SUBAGENT 2

#endif

#if (!defined(auxmth_inc))
#include <envoy/h/auxmth.h>
#endif

extern int std_next __((OIDC_T      lastmastch,
                        int         tcount,
                        OIDC_T     *tlist,
                        OIDC_T     *rlist,
                        char       *cookie,
                        SNMP_PKT_T *pktp));

extern void std_next_async  __((OIDC_T      lastmastch,
                                int         tcount,
                                OIDC_T     *tlist,
                                SNMP_PKT_T *pktp,
                                VB_T       *vbp));

/* We define the following functions for use with the compatibility routines */
extern void async_compat_test __((OIDC_T         lastmatch,
                                  int            compc,
                                  OIDC_T        *compl,
                                  SNMP_PKT_T    *pktp,
                                  VB_T          *vbp));

extern void async_compat_get  __((OIDC_T         lastmatch,
                                  int            compc,
                                  OIDC_T        *compl,
                                  SNMP_PKT_T    *pktp,
                                  VB_T          *vbp));

extern void async_compat_set  __((OIDC_T         lastmatch,
                                  int            compc,
                                  OIDC_T        *compl,
                                  SNMP_PKT_T    *pktp,
                                  VB_T          *vbp));

extern void async_compat_next __((OIDC_T         lastmatch,
                                  int            compc,
                                  OIDC_T        *compl,
                                  SNMP_PKT_T    *pktp,
                                  VB_T          *vbp));

#endif  /* snmpdefs_inc */
