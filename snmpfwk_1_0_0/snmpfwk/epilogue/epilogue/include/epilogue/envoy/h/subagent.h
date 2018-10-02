/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/subagent.h,v 1.1 2006/06/29 12:17:08 smasyutin Exp $ */

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
 * $Log: subagent.h,v $
 * Revision 1.1  2006/06/29 12:17:08  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:29  sar
 * Updated revision to 8.0
 *
 * Revision 7.4  1997/10/22 03:07:28  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.3  1997/10/16 00:25:19  sar
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
 * Revision 7.2  1997/03/20 06:48:20  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/06/07  20:08:33  sar
 * Made the opcode field in the ipc_comp structure a pointer.
 *
 * Revision 6.0  1995/05/31  21:46:54  sra
 * Release 6.0.
 *
 * Revision 5.9  1995/05/27  01:18:31  sar
 * removed reqid from master_query_reply_handler's args.
 *
 * Revision 5.8  1995/04/28  22:41:35  sar
 * Added some definitions for use with an async master handler
 * (opccomp_t & sa_reg_t).
 *
 * Revision 5.7  1995/04/18  20:44:34  sar
 * This version is to correct the revision history from the last revision.
 * Added some tags for cleanup routine, got rid of the priv tag structure
 * added some new type sfor use with async sub agent stuff on the master
 * side and some type cleanup to make compilers happy.
 *
 * Revision 5.6  1995/04/18  20:41:28  sar
 * Added some new macros and modified the old macros so we don't requires
 * ifdefs in all of the mainline code.  Instead we use the new macros
 * which will be defined to nothing if locks aren't installed.
 *
 * Revision 5.5  1995/03/24  16:30:24  sar
 * Added SA_GROUP_INACTIVE flag
 *
 * Revision 5.4  1995/03/20  23:11:33  sar
 * Added async routines.
 *
 * Revision 5.3  1994/10/13  19:22:35  sar
 * added a define for unsupported type (for objects).
 *
 * Revision 5.2  1994/10/13  19:18:59  sar
 * Added a version field to the group structure.
 *
 * Revision 5.1  1994/08/04  22:05:29  sar
 * Added multiple mib tree capability, including macros to translate the
 * old apis into the new api's
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 1.7  1994/05/13  21:48:32  sar
 * made one of the flags be 32 bits instead of 16 so it matches elsewhere.
 *
 * Revision 1.6  1994/05/12  17:19:27  sar
 * changed the unused field in hdr_blk to be snmp_version.
 *
 * Revision 1.5  1994/05/06  20:29:54  sar
 * Made SA_DEMUX_T have both an object and a string instead of a union
 * of the two.
 *
 * Revision 1.4  1994/05/05  19:56:46  sar
 * added some new function defs so I could break subagent.c into
 * sa_core, sa_subagt, and sa_master files.
 *
 * Revision 1.3  1994/05/04  23:47:31  sar
 * made VBPRIVPROC's always take a PTR_T as argument (SA_freeproc for one).
 *
 * Revision 1.2  1994/05/02  21:19:26  sar
 * Added a lock error define.
 *
 * Revision 1.1  1994/04/29  20:08:18  sar
 * Initial revision
 * */
#if (!defined(subagent_inc))
#define subagent_inc

#include <envoy/h/asn1.h>
#include <envoy/h/snmpdefs.h>
#include <envoy/h/buffer.h>
#include <envoy/h/mib.h>
#include <envoy/h/tree.h>

/* length of the sub agent header string */
/* currently version, opcode1, opcode2,
   error field, encoding scheme, snmp version, reqid (16 bits),
   sysuptime (4 bytes), index/group (4 bytes), snmp error (4 bytes) */
#define SA_HEADERLEN 20

/* sub agent error codes */
#define SA_GEN_ERROR          1
#define SA_UNKNOWN_VERSION    2
#define SA_UNKNOWN_OPCODE1    3
#define SA_UNKNOWN_OPCODE2    4
#define SA_UNKNOWN_ENCODING   5
#define SA_DECODE_FAILURE     6
#define SA_ENCODE_FAILURE     7
#define SA_UNKNOWN_NODE       8
#define SA_UNKNOWN_TAG        9
#define SA_UNKNOWN_GRP       10
#define SA_SHORT_MSG         11
#define SA_IPC_ERROR         12
#define SA_LOCK_ERROR        13
#define SA_NODE_ERROR        14 /* error codes for reg/dereg replies */
#define SA_MEMORY_ERROR      15
#define SA_UNSUPPORTED_TYPE  16
#define SA_NO_SAVED_PACKET   17

/* Encoding options */
#define SA_ENCODING_ASN1 1

/* subagent opcodes */
#define SA_REG_OBJ_REQUEST       1
#define SA_REG_OBJ_REPLY         2
#define SA_REM_OBJ_REQUEST       3
#define SA_REM_OBJ_REPLY         4
#define SA_REG_INST_REQUEST      5
#define SA_REG_INST_REPLY        6
#define SA_REM_INST_REQUEST      7
#define SA_REM_INST_REPLY        8

#define SA_QUERY_REQUEST         9
#define SA_QUERY_REPLY          10
#define SA_TRAP_REQUEST         11
#define SA_TRAP_REPLY           12
#define SA_MAX_OPCODE           12

/* subagent operation types for use with query request/replies */
#define SA_QUERY_TESTGET    1
#define SA_QUERY_TESTSET    2
#define SA_QUERY_TESTCREATE 3
#define SA_QUERY_NEXT       4
#define SA_QUERY_GET        5
#define SA_QUERY_SET        6
#define SA_QUERY_UNDO       7
#define SA_QUERY_MAX        7

/* flag and error definitions, will need to move this to snmpdefs.h */
/* 0 = only the group, 1 = all */
#define SA_HOWMANY      0x01
#define SA_HOWMANY_GROUP   0
#define SA_HOWMANY_ALL     1

#define SA_INSTANCE_TYPE        0x02    /* for use in obj reg requests */
#define SA_OBJECT_TYPE          0x02    /* for use in instance reg requests */
#define SA_INSTANCE_TYPE_EXISTS 0x04

#define SA_GENERR                1
#define SA_NODE_IS_INNER         2
#define SA_NODE_NOT_SA           3
#define SA_NODE_ACTIVE           4
#define SA_NODE_TYPE_MISMATCH    5
#define SA_POINTS_THROUGH_LEAF   6
#define SA_NO_SUCH_NODE          7
#define SA_NOT_OWNER             8

/* options for SA_Master_Cleanup */
#define SA_CLEANUP_TIMEOUT  0x01        /* timeout (1) or error (0) */
#define SA_CLEANUP_INACTIVE 0x02        /* inactivate the group (1) */
#define SA_CLEANUP_CONTINUE 0x04        /* run the continue routine (1) */

typedef struct SA_DEMUX_S {
              EBUFFER_T string;
              OBJ_ID_T  object;
              } SA_DEMUX_T;

typedef void     IPCCOMP_T      __((OCTET_T, EBUFFER_T *, VBL_T *, PTR_T));
typedef INT_32_T IPCSEND_AS_T   __((EBUFFER_T *buf, PTR_T ipchandle,
                                    UINT_16_T reqid));

typedef INT_32_T IPCSEND_T      __((EBUFFER_T *buf, PTR_T ipchandle));

typedef INT_32_T IPCRCV_T       __((EBUFFER_T *buf, PTR_T ipchandle));

typedef void     IPCFREE_T      __((PTR_T ipchandle));

typedef INT_32_T IPCAYT_T       __((PTR_T ipchandle));

typedef struct SA_GROUP_S {
        IPCSEND_AS_T *ipcsend;  /* send a query message to ipchandle */
        IPCRCV_T  *ipcrcv;      /* recv a query message from ipchandle */
        IPCFREE_T *ipcfree;     /* ipc routine to free the ipc handle */
        IPCAYT_T  *ipcayt;      /* ipc routine to do are you there,
                                   1 = ipc link ok, 0 = ipc link fail */
        PTR_T      ipchandle;   /* handle to give to the ipc routine, it
                                   should contain addressing info about the
                                   sub agent */
        struct SA_GRP_LST_S *group_list;        /* Backpointer to the list of
                                                   nodes that use this group */
        INT_32_T   groupid;     /* groupid */
        INT_32_T   encoding;    /* type of encoding to use for transmission */
        OCTET_T    flags;       /* flags */
        OCTET_T    version;     /* the version of the sub agent protocol the
                                   sub agent is expecting */
        } SA_GROUP_T;

#define SA_GROUP_ACTIVE 0x01
#define SA_GROUP_INACTIVE 0x02

typedef struct SA_GRP_LST_S {
        MIBLEAF_T           *leaf;      /* back pointer to leaf */
        SA_GROUP_T          *group;     /* pointer to group for this list */
        struct SA_GRP_LST_S *gchain;    /* for chaining elements of the group
                                           list together */
        struct SA_GRP_LST_S *ichain;    /* for chaining elements of an objects
                                           instance list together */
        struct SA_GRP_LST_S *instance;  /* list of instances for this group and
                                           object */
        } SA_GRP_LST_T;

typedef struct SA_COOKIE_S {
        SA_GRP_LST_T *ichain;           /* pointer to the chain of groups for
                                           this object */
        TREENODE_T    *instance_tree;   /* pointer to the root of the instance
                                           tree for this object */
        UINT_32_T     flags;
        } SA_COOKIE_T;


typedef struct SA_HANDLE_S {
        SA_GROUP_T         *group;      /* pointer to a group */
        struct SA_HANDLE_S *nexthand;   /* pointer to next handle in chain */
        } SA_HANDLE_T;

typedef struct SA_HEADER_S {
        OCTET_T         version;
        OCTET_T         opcode1;
        OCTET_T         opcode2;
        OCTET_T         sa_error;
        OCTET_T         encoding;
        OCTET_T         snmp_version;
        UINT_16_T       requestid;
        UINT_32_T       sysuptime;
        INT_32_T        index;
        INT_32_T        snmp_error;
        } SA_HEADER_T;

typedef struct SA_IPC_COMP_S {
        PTR_T      ipchandle;
        IPCRCV_T  *ipcrcv;
        IPCSEND_T *ipcsend;
        IPCFREE_T *ipcfree;
        IPCAYT_T  *ipcayt;
        EBUFFER_T *buf;
        VBL_T     *vblist;
        OCTET_T   *opcode;
        } SA_IPC_COMP_T;

typedef struct SA_REG_S {
        IPCCOMP_T  *ipccomp;
        PTR_T       ipchandle;
        SA_GROUP_T *group;
        MIBNODE_T  *mib_root;
        OCTET_T    *errorhdr;
        VBL_T       vblist;
        SA_HEADER_T hdr_blk;
        } SA_REG_T;

#if (INSTALL_ENVOY_EPI_SUBAGENT_SCHEME)
extern INT_32_T SA_Encode_Hdr   __((OCTET_T *, SA_HEADER_T *));
extern INT_32_T SA_Decode_Hdr   __((OCTET_T *, SA_HEADER_T *));
extern INT_32_T SA_Encode       __((VBL_T *, SA_HEADER_T *,
                                    SA_DEMUX_T *, EBUFFER_T *));
extern INT_32_T SA_Decode       __((OCTET_T *, ALENGTH_T, VBL_T *,
                                    SA_HEADER_T *, SA_DEMUX_T *));
#endif

#if (INSTALL_ENVOY_EPI_SUB)
typedef void SA_REG_COMPLETE_T __((INT_32_T ecode, SA_HEADER_T *hdr_blk,
                                   VBL_T *vblp, PTR_T cookie));
typedef void SA_IO_COMPLETE_T  __((PTR_T pktp, SA_HEADER_T *hdr_blk,
                                   PTR_T cookie));
typedef void SA_ERR_COMPLETE_T __((int error_code, PTR_T cookie));

extern void     SA_Handler_Async __((OCTET_T *, ALENGTH_T, PTR_T,
                                     SA_IO_COMPLETE_T *, SA_ERR_COMPLETE_T *,
                                     SA_REG_COMPLETE_T *, PTR_T));
extern INT_32_T SA_Handler_Finish   __((PTR_T, SA_HEADER_T *, EBUFFER_T *));
extern void     SA_Handler_Cleanup  __((PTR_T, SA_HEADER_T *));
extern INT_32_T SA_Handler_WR    __((OCTET_T *, ALENGTH_T, EBUFFER_T *,
                                     SA_HEADER_T *, VBL_T *, PTR_T));
#define SA_Handler(MSG, LEN, RBUF, HDR, VB) \
        SA_Handler_WR(MSG, LEN, RBUF, HDR, VB, 0)
#define SA_Handler_Continue(PKTP) ((PKTP)->cont(PKTP))

#endif

#if (INSTALL_ENVOY_EPI_MASTER)
extern INT_32_T     hand_add_group      __((SA_GROUP_T *));
extern void         hand_remove_group   __((SA_GROUP_T *));
extern SA_GROUP_T  *find_group_by_index __((INT_32_T));

extern void         remove_grp_lst  __((SA_GRP_LST_T *));
extern void     mark_group_inactive __((SA_GROUP_T *));
extern SA_GROUP_T * SA_getgroup     __((int, OIDC_T *, SA_COOKIE_T *));
extern INT_32_T     SA_do_query     __((SA_HEADER_T *, VBL_T *,
                                        SNMP_PKT_T *, SA_GROUP_T *));
extern void         SA_freeproc     __((PTR_T));
extern INT_32_T   Master_Handler_WR __((OCTET_T *, ALENGTH_T, IPCSEND_T *,
                                        IPCRCV_T *, IPCFREE_T *, IPCAYT_T *,
                                        PTR_T, EBUFFER_T *, VBL_T *, PTR_T));
#define Master_Handler(MSG, LEN, SND, RCV, FRE, AYT, HAND, RBUF, VBL) \
        Master_Handler_WR(MSG, LEN, SND, RCV, FRE, AYT, HAND, RBUF, VBL, 0)

extern void     Master_Handler_Async __((OCTET_T *, ALENGTH_T, IPCCOMP_T *,
                                         IPCSEND_AS_T *, IPCRCV_T *,
                                         IPCFREE_T *, IPCAYT_T *,
                                         PTR_T, PTR_T));

extern UINT_16_T Master_Query_Reply_Handler __((OCTET_T *, ALENGTH_T, int));

extern void     Master_Cleanup             __((UINT_16_T, UINT_16_T));
extern void     Cleanup_Group              __((void));
#endif

#endif /* subagent_inc */
