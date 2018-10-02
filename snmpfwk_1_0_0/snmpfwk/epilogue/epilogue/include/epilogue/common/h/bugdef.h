/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/common/h/bugdef.h,v 1.1 2006/06/29 12:17:06 smasyutin Exp $ */

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
 *  Copyright 1998 Integrated Systems, Inc.
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
*****************************************************************************/

/*
 * $Log: bugdef.h,v $
 * Revision 1.1  2006/06/29 12:17:06  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:22  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:16  alext
 * no message
 *
 * Revision 1.2  1998/02/25 04:43:24  sra
 * Update copyrights.
 *
 * Revision 1.1  1998/02/05 22:19:28  josh
 * moving commonly used include files into common/h from top-level directory
 *
 * Revision 3.26  1997/11/20 17:33:15  sra
 * Clear out a bunch of unused PAP and CHAP bug codes.
 *
 * Revision 3.25  1997/11/19 21:14:42  josh
 * Liaison bug definitions
 *
 * Revision 3.24  1997/11/18 03:18:24  sra
 * Brand new PAP and CHAP code for PPP.
 *
 * Revision 3.23  1997/10/28 00:51:48  sra
 * Convert PPP code to use BUG() macro.
 * Bang on CHAP and PAP code some more.
 * Create test driver for PPP under Snark/BSD.
 *
 * Revision 3.22  1997/10/15 19:49:36  sra
 * Preliminary cleanup of PPP PAP and CHAP modules.
 *
 * Revision 3.21  1997/08/15 22:14:57  lowell
 * PAP/CHAP polishing
 *
 * Revision 3.20  1997/08/14 16:04:33  lowell
 * boilerplate
 *
 * Revision 3.19  1997/07/29 00:18:41  lowell
 * PAP/CHAP messages for logging already in file
 *
 * Revision 3.18  1997/07/17 02:15:56  alan
 * New errors for a new Decorum
 *
 * Revision 3.17  1997/06/17 19:00:20  lowell
 * more extensive TFTP debugging
 *
 * Revision 3.16  1997/05/30 17:00:18  lowell
 * TFTP (got rid of old TFTP_DEBUG)
 *
 * Revision 3.15  1997/05/23 04:13:32  sra
 * Add BUG_IPADDR_TEST_UNKNOWN_CODE.
 *
 * Revision 3.14  1997/05/22 03:35:25  sra
 * Get rid of struct udp_hdr.  Change pkt_rcv_info into an array, convert
 * receive path to use it instead of passing pointer arguments in upcalls.
 *
 * Revision 3.13  1997/05/17 07:39:02  sra
 * Change IP protocol dispatch to use ip_listener table.
 *
 * Revision 3.12  1997/03/20 06:58:16  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 3.11  1997/03/19 04:11:35  sra
 * BUG() now common to all products, so move the associated .h files.
 *
 * Revision 3.10  1997/02/24 14:43:18  josh
 * new bug definitions for Courier RIP and Attache Plus TCP
 *
 * Revision 3.9  1996/10/28 18:19:01  lowell
 * Decorum messages
 * Looks like this needs to be freed of its ownership by Attache...
 *
 * Revision 3.8  1996/03/22  10:04:12  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 3.7  1996/03/14  17:51:55  lowell
 * DHCP packet dumper code
 *
 * Revision 3.6  1995/12/02  09:15:04  sra
 * Add a bunch of Courier stuff.
 *
 * Revision 3.5  1995/09/28  23:06:59  sra
 * Fix Courier identifiers and copyrights for release packaging.
 *
 * Revision 3.4  1995/06/30  23:24:10  sra
 * Back out hairy interface to RCI TRACE() to a simpler scheme.
 *
 * Revision 3.3  1995/06/30  22:44:00  sra
 * Add RCI trace bug codes.
 *
 * Revision 3.2  1995/06/30  16:10:40  lowell
 * first few ARP and DHCP bug definitions (more to come)
 *
 * Revision 3.1  1995/06/27  06:20:23  sra
 * Add some debugging code to catch botched packet buffer allocs/frees.
 *
 * Revision 3.0  1995/05/10  22:36:18  sra
 * Release 3.0.
 *
 * Revision 1.1  1995/04/25  21:39:43  sra
 * Initial revision
 *
 */

/*
 * BUGDEF() table (see bug.h)
 *
 * NB: This file intentionally does NOT protect against multiple inclusion.
 *
 * Bug symbol definitions.  This file is included by bug.h,
 * and may be included by other modules as necessary.  It's a separate
 * file to make it easy to build a translation tables, bitvectors, and
 * so forth.
 *
 * There should not be anything in this file except comments and
 * invocations of the BUGDEF() macro.
 *
 * We'll probably add more of these in every release.  Check these
 * with a switch() or if() statement, don't depend on specific numeric
 * values.
 */

BUGDEF(BUG_ASSERTION_FAILED)                    /* bug.h */
BUGDEF(BUG_RIP_MBZ_WASNT)                       /* attache/net/rip.c */
BUGDEF(BUG_RIP_BAD_ENTRY)                       /* attache/net/rip.c */
BUGDEF(BUG_RIP_COULDNT_CREATE_ROUTE)            /* attache/net/rip.c */
BUGDEF(BUG_RIP_COULDNT_CREATE_ROUTE_EXT)        /* attache/net/rip.c */
BUGDEF(BUG_RIP_DURCV)                           /* attache/net/rip.c */
BUGDEF(BUG_RIP_NO_BUFFERS)                      /* attache/net/rip.c */
BUGDEF(BUG_RIP_SKIPPING_ADDRESS)                /* attache/net/rip.c */
BUGDEF(BUG_RIP_SKIPPING_BROADCAST_ADDRESS)      /* attache/net/rip.c */
BUGDEF(BUG_RIP_ROUTE_CHANGE)                    /* attache/net/rip.c */
BUGDEF(BUG_RIP_CLEAR_ROUTE_CACHE)               /* attache/net/rip.c */
BUGDEF(BUG_RIP_BAD_BROADCAST_ADDRESS)           /* attache/net/rip.c */
BUGDEF(BUG_IPOPT_TOO_LONG)                      /* attache/net/ip_opt.c */
BUGDEF(BUG_PKT_INIT_NO_MEMORY)                  /* attache/net/packet.c */
BUGDEF(BUG_PKT_ALLOC_ALREADY)                   /* attache/net/packet.c */
BUGDEF(BUG_PKT_FREE_ALREADY)                    /* attache/net/packet.c */
BUGDEF(BUG_IP_SEND_FREED)                       /* attache/net/ip_send.c */
BUGDEF(BUG_ARP_NULL_POINTER)                    /* attache/net/arp.c */
BUGDEF(BUG_DHCP_BAD_PACKET)                     /* (unused?) */
BUGDEF(BUG_DHCP_PACKET_DUMP)                    /* attache/net/dhcp.c */
BUGDEF(BUG_COURIER_TRACE)                       /* courier/common/trace_ut.h */
BUGDEF(BUG_COURIER_IPI_ENABLE_FAILED)           /* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_IPI_INIT_FAILED)             /* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_IPI_REENABLE_FAILED)         /* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_OSPF_INIT_FAILED)            /* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_OSPF_RCV_NO_IFB)             /* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_OSPF_RCV_IFB_DOWN)           /* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_RIP_RCV_NO_IFB)              /* courier/common/udp_ut.c */
BUGDEF(BUG_COURIER_RIP_RCV_IFB_DOWN)            /* courier/common/udp_ut.c */
BUGDEF(BUG_IF_ATTACH_IFB_CREATE_FAILED)         /* attache/net/if.c */
BUGDEF(BUG_COURIER_TCP_CANT_ALLOCATE_TCB)       /* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_TCP_CANT_BIND_HOST_PASSIVE)  /* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_TCP_MULTIPLE_LISTENS)        /* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_TCP_START_FAILED)            /* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_FATAL_ERROR)                 /* courier/sys_incl.h */
BUGDEF(BUG_DECORUM_BAD_DATE_TYPE)               /* decorum/http/date.c */
BUGDEF(BUG_DECORUM_BAD_VERSION)                 /* (unused) */
BUGDEF(BUG_DECORUM_CLIENT_ERROR)                /* decorum/http/http.c */
BUGDEF(BUG_DECORUM_ENV_DUPLICATE_NAME)          /* decorum/http/envcfg.c */
BUGDEF(BUG_DECORUM_INFO)                        /* decorum/http/xxx.c */
BUGDEF(BUG_DECORUM_MISSING_INBUF)               /* (unused) */
BUGDEF(BUG_DECORUM_NO_DATE)                     /* decorum/http/date.c */
BUGDEF(BUG_DECORUM_UNEXPECTED_ERR)              /* decorum/http/xxx.c */
BUGDEF(BUG_DECORUM_XATTACHE_ERROR)              /* decorum/http/xattache.c */
BUGDEF(BUG_DECORUM_XATTACHE_INFO)               /* decorum/http/xattache.c */
BUGDEF(BUG_LIAISON_CANT_BIND_OBJECT)            /* liaison/xrmon/xsnmp.c */
BUGDEF(BUG_LIAISON_UNKNOWN_OBJECT)              /* liaison/xrmon/xsnmp.c */
BUGDEF(BUG_LIAISON_CANT_ENCODE_TRAP)            /* liaison/xrmon/xsnmp.c */
BUGDEF(BUG_SNARK_UNEXPECTED_EOF)                /* snark/test/main.c */
BUGDEF(BUG_TCP_ACK_DATA_FREEING)                /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_ACK_DATA_HUSHING)                /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_ACK_DATA_SQUEEZING)              /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_ACK_DATA_FAST_RETRANSMIT_DONE)   /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_CLOSE_INVALID_CONNECTION)        /* attache/net/tcp.c */
BUGDEF(BUG_TCP_CLOSE_INVALID_STATE)             /* attache/net/tcp.c */
BUGDEF(BUG_TCP_ENTER_FUNC)                      /* attache/net/tcpintnl.h */
BUGDEF(BUG_TCP_GET_SND_MSS)                     /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_HANDLE_ACK_DALLY)                /* attache/net/tcp.c */
BUGDEF(BUG_TCP_HANDLE_SWS_DALLY)                /* attache/net/tcp.c */
BUGDEF(BUG_TCP_POST_DATA_AHEAD)                 /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_POST_DATA_OBSOLETE)              /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_POST_DATA_POSTING)               /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_PRINT_PKTHDR)                    /* attache/net/tcpintnl.h */
BUGDEF(BUG_TCP_RCV_ACKED_UNSENT_DATA)           /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_ACKING_UNACCEPTABLE)         /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_LOG_PKT)                     /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_REUSING_SAVED_PKT)           /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_SAVING_EARLY_PKT)            /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_SCHEDULING_ZERO_WINDOW_PROBE) /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_FAST_RETRANSMIT_EQUAL)       /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_FAST_RETRANSMIT_GREATER)     /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RETRANSMIT_HANDLER)              /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_RTT_LOG_1)                       /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RTT_LOG_2)                       /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_SEND_COPYING)                    /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_DECIDED_TO_SEND)            /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_ENTRY_LOG)                  /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_FORBIDDEN_BY_SWS)           /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_LOG_PKT)                    /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_NEW_DATA_BEYOND_WINDOW)     /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_NOTHING_TO_DO)              /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_PKTALLOC_FAILED)            /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_SCHEDULING_RETRANSMISSION)  /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SET_ACK_DALLY)                   /* attache/net/tcp.c */
BUGDEF(BUG_TCP_SET_RCV_MSS)                     /* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SET_SWS_DALLY)                   /* attache/net/tcp.c */
BUGDEF(BUG_TCP_SET_TIME_WAIT)                   /* attache/net/tcp.c */
BUGDEF(BUG_TFTP_BAD_PORT)                       /* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_CNXN_NOTFOUND)                  /* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_ALLOC_PKT_FAIL)                 /* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_ALLOC_MEM_FAIL)                 /* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_TIMEOUT)                        /* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_WRONG_ACK)                      /* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_WRONG_BLOCK)                    /* attache/net/tftpc.c */
BUGDEF(BUG_IP_RCV_UNRECOGNIZED_IP_VERSION)      /* attache/net/ip_rcv.c */
BUGDEF(BUG_PKT_APPEND_OVERFLOW)                 /* attache/h/packet.h */
BUGDEF(BUG_IPADDR_TEST_UNKNOWN_CODE)            /* attache/net/ipaddr.c */
BUGDEF(BUG_PPP_CHAP_PKT_ALLOC_FAILED)           /* attache/net/pppchap.c */
BUGDEF(BUG_ATTACHE_PPP_LCP_BAD_TIMEOUT)         /* attache/net/ppp.c */
BUGDEF(BUG_ATTACHE_PPP_IPCP_BAD_TIMEOUT)        /* attache/net/ppp.c */
BUGDEF(BUG_ATTACHE_PPP_LCP_EVENT)               /* attache/net/ppp.c */
BUGDEF(BUG_ATTACHE_PPP_IPCP_EVENT)              /* attache/net/ppp.c */
BUGDEF(BUG_SNARK_PPP_AUTH_STATUS)               /* snark/lib/netconf.c */
BUGDEF(BUG_PPP_CHAP_CHALLENGE_TIMEOUT)          /* attache/net/pppchap.c */
BUGDEF(BUG_PPP_CHAP_SERVER_UNEXPECTED_STATE)    /* attache/net/pppchap.c */
BUGDEF(BUG_PPP_CHAP_CLIENT_UNEXPECTED_STATE)    /* attache/net/pppchap.c */
BUGDEF(BUG_PPP_AUTH_ALLOC_FAILED)               /* attache/net/pppauth.c */
BUGDEF(BUG_PPP_AUTH_BAD_FLAGS)                  /* attache/net/pppauth.c */
BUGDEF(BUG_PPP_CHAP_SERVER_ENABLED)             /* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_PAP_SERVER_ENABLED)              /* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_NEGOTIATING_CHAP)                /* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_NEGOTIATING_PAP)                 /* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_LCP_UNKNOWN_NAK)                 /* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_LCP_UNKNOWN_REQ)                 /* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_LCP_RCV)                         /* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_PAP_CLIENT_UNEXPECTED_STATE)     /* attache/net/ppppap.c */
BUGDEF(BUG_PPP_PAP_PKT_ALLOC_FAILED)            /* attache/net/ppppap.c */
BUGDEF(BUG_PPP_PAP_REQUEST_TIMEOUT)             /* attache/net/ppppap.c */
BUGDEF(BUG_PPP_PAP_SERVER_UNEXPECTED_STATE)     /* attache/net/ppppap.c */

/* End of bugdef.h */
