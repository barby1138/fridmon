/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/vbdefs.h,v 1.1 2006/06/29 12:17:09 smasyutin Exp $ */

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
 * $Log: vbdefs.h,v $
 * Revision 1.1  2006/06/29 12:17:09  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:10  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:31  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:33  sar
 * Updated revision to 8.0
 *
 * Revision 7.6  1997/10/26 01:44:43  sar
 * Always define the 64 bit counter and include it in the vb defintion
 * instead of forgetting about it if we don't have v2 or v2 types installed.
 *
 * Revision 7.5  1997/10/22 03:07:28  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.4  1997/10/16 23:52:27  sar
 * We changed the macro VB_GET_IP_ADDRESS to provide a more consistent
 * type signature from compilers.  Previously it could have been either
 * a pointer to an array of bytes or a pointer to a byte.  It should
 * now resolve to a pointer to an array of bytes.
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
 * Revision 7.2  1997/03/20 06:48:25  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.2  1995/10/20  22:53:35  sar
 * removed bitstring include file & macro
 * removed nsapaddress
 * added VT_BITS & VT_UNSIGNED32
 *
 * Revision 6.1  1995/06/05  20:57:49  sar
 * Added clean_vb_data.
 *
 * Revision 6.0  1995/05/31  21:46:57  sra
 * Release 6.0.
 *
 * Revision 1.1  1995/03/20  23:07:47  sar
 * Initial revision
 *
 *
 */

#if (!defined(vb_inc))
#define vb_inc

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

/* Definition for routine to free private vb space */
typedef void VBPRIVPROC_T __((PTR_T));

/* Definition for routine to undo sets */
typedef void UNDOPROC_T __((OIDC_T lastmatch, int compc, OIDC_T *compl, ...));

/* Define the internal shape of a VarBind       */
typedef struct VB_S
        {
        struct VB_S *   vb_link;          /* For the user to use, usually   */
                                          /*  to build a list of related    */
                                          /*  VB_Ts.                        */
        ALENGTH_T       vb_seq_size;      /* Overall length of the VarBind  */
                                          /* sequence when encoded.         */
        OBJ_ID_T        vb_obj_id;        /* Object id for this VarBind     */
        OCTET_T         vb_data_flags_n_type;  /* Class form & type of data */
        OCTET_T         vb_flags;         /* See below                      */
        ALENGTH_T       vb_data_length;   /* Length of the data when encode */
        MIBLOC_T        vb_ml;            /* The mib-leaf associated with   */
                                          /* this VarBind.                  */
        union   {
                INT_32_T        v_number;       /* Integer kinds of things  */
                UINT_32_T       v_counter;      /* Unsigned int things      */
                EBUFFER_T       v_string;       /* Octetstring things       */
                OBJ_ID_T        v_object;       /* Object id things         */
                unsigned char   v_network_address[4];
                UINT_64_T       v_counter64;    /* 64 bit unsigned things   */
                } value_u;

        union   {
                ptr_t    p_ptr;           /* storage space for moving info  */
                bits32_t p_int;           /* between test and set routines  */
                } priv;

        VBPRIVPROC_T *  vb_free_priv;     /* pointer to routine to free the */
                                          /* private storage space          */
        UNDOPROC_T   *  undoproc;         /* pointer to routine to undo sets*/
#if (INSTALL_ENVOY_EPI_SUBAGENT_SCHEME)
        UINT_32_T       sa_flags;
        OCTET_T         vb_access;
#endif
#if (INSTALL_ENVOY_AGENTX)
        OBJ_ID_T        ax_search_end;
        OCTET_T         ax_flags;
#endif
        } VB_T;
#define v_gauge         v_counter
#define v_timeticks     v_counter
#define v_uinteger32    v_counter
#define v_opaque        v_string

#define vb_priv         priv.p_ptr
#define vb_int          priv.p_int

/* Values for vb_data_flags_n_type      */
#define VT_NUMBER       A_INTEGER
#define VT_STRING       A_OCTETSTRING
#define VT_BITS         A_OCTETSTRING           /* same as a string */
#define VT_OBJECT       A_OBJECTID
#define VT_EMPTY        A_NULL
#define VT_IPADDRESS    (A_APPLICATION | 0)
#define VT_COUNTER      (A_APPLICATION | 1)
#define VT_GAUGE        (A_APPLICATION | 2)
#define VT_UNSIGNED32   (A_APPLICATION | 2)     /* same as a guage */
#define VT_TIMETICKS    (A_APPLICATION | 3)
#define VT_OPAQUE       (A_APPLICATION | 4)
#define VT_COUNTER64    (A_APPLICATION | 6)
#define VT_UINTEGER32   (A_APPLICATION | 7)

/* values for exception data types */
#define VT_NOSUCHOBJ    (A_CONTEXT | 0)
#define VT_NOSUCHINS    (A_CONTEXT | 1)
#define VT_ENDOFMIB     (A_CONTEXT | 2)

/* Values for vb_flags  */
#define VFLAG_ALREADY_TEST      0x01
#define VFLAG_ALREADY_SET       0x02
#define VFLAG_ALREADY_GET       0x02
#define VFLAG_SA_SENDIT         0x04
#define VFLAG_SA_DOIT           0x08

/* more values for vb_flags some of them need to be alligned with the
   already_* flags, the rest are put in as we have space */

#define VFLAG_TEST_STARTED      0x10
#define VFLAG_TEST_DONE         0x01
#define VFLAG_SET_STARTED       0x20
#define VFLAG_SET_DONE          0x02
#define VFLAG_UNDO_STARTED      0x40
#define VFLAG_UNDO_DONE         0x80
#define VFLAG_UNDO_BOTH         (VFLAG_UNDO_STARTED | VFLAG_UNDO_DONE)

#define VFLAG_GET_STARTED       0x20
#define VFLAG_GET_DONE          0x02

#define VFLAG_NEXT_STARTED      0x20
#define VFLAG_NEXT_DONE         0x02
#define VFLAG_NEXT_REDO         0x10
#define VFLAG_NEXT_VCREQ        0x40
#define VFLAG_NEXT_VCDONE       0x80
#define VFLAG_NEXT_INSTANCE     0x01
#define VFLAG_NEXT_VCBITS       (VFLAG_NEXT_VCREQ | VFLAG_NEXT_VCDONE)
#define VFLAG_NEXT_FLAGS        0xF3

#define ENVOY_AX_FLAGS_INCLUDE     0x01
#define ENVOY_AX_FLAGS_CLEAN_LEAF  0x02
#define ENVOY_AX_FLAGS_CLEAN_INDEX 0x04

/* Define the shape of the VarBindList */
typedef struct VBL_S
        {
        ALENGTH_T       vbl_length;     /* Length of the VarBindList seq */
        int             vbl_count;      /* Number of Var Bind items      */
        VB_T            *vblist;
        struct VBL_S    *vblp;          /* for get bulk chaining purposes */
        } VBL_T;


/* macros to get the data from a vbp */
#define VB_GET_INT32(V)          ((V)->value_u.v_number)
#define VB_GET_UINT32(V)         ((V)->value_u.v_counter)
#define VB_GET_IP_ADDRESS(V)     ((V)->value_u.v_network_address)
#define VB_GET_STRING(V)        &((V)->value_u.v_string)
#define VB_GET_OBJECT_ID(V)     &((V)->value_u.v_object)
#define VB_GET_UINT64(V)        &((V)->value_u.v_counter64)

/* macro to get a pointer to a cookie, so that the user
   doesn't have to understand what a vb looks like */
#define VB_TO_COOKIE(vbp)       (vbp)->vb_ml.ml_leaf->user_cookie

/* various vb utility routines */
extern  VB_T *          VarBindList_Allocate __((int));
extern  VBL_T *         VBList_Allocate      __((int));
extern  ALENGTH_T       vbsize               __((VB_T *));
extern  void            Clean_vb_data        __((VB_T *));
extern  void            Clean_vb             __((VB_T *));
extern  void            Clean_vb_list        __((VBL_T *));

#endif /* (!defined(vb_inc)) */
