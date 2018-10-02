/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/port/bsd44/attache.h,v 1.1 2006/06/29 12:17:11 smasyutin Exp $ */

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
 *  Copyright 1993-1997 Epilogue Technology Corporation.
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
 ****************************************************************************/

/*
 * $Log: attache.h,v $
 * Revision 1.1  2006/06/29 12:17:11  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:12  denysg
 * no message
 *
 * Revision 1.10  1998/03/12 18:23:43  meister
 * GLUE_IPSEC_RCV_SA_NOTUNNEL()
 *
 * Revision 1.9  1998/03/10 00:36:59  meister
 * Added back all my IPSEC glue functions which seem to have disappeared
 * when Mrf made her last update.
 *
 * Revision 1.8  1998/03/03 20:37:52  mrf
 * Change GLUE_SNTP_SET_TIME macro to take packet pointer
 *
 * Revision 1.7  1998/03/03 15:26:16  mrf
 * Comment out PRINTF declarations.
 *
 * Revision 1.6  1998/02/25 04:56:45  sra
 * Update copyrights.
 *
 * Revision 1.5  1998/02/18 22:34:58  sra
 * Add ip_loopback(), eradicate glue_pkt_handle().
 *
 * Revision 1.4  1998/02/13 00:42:46  mrf
 * Add conditionalized SNTP Client macro
 *
 * Revision 1.3  1997/09/15 15:48:26  sra
 * GLUE_NOW() migration version skew.
 *
 * Revision 1.2  1997/08/21 17:23:44  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 1.1  1997/08/13 15:17:29  mrf
 * added def for MAX_HW_ADDRESS_LEN for ipv6 nd
 *
 * Revision 1.0  1997/08/08 22:11:10  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.8  1997/08/07 23:01:26  meister
 * Added a do-nothing comment to attache.h
 *
 * Revision 0.7  1997/07/01 04:37:59  mrf
 * add commented-out PRINTF definition
 *
 * Revision 0.6  1997/05/14 06:22:12  sra
 * Enable inline byteswapping to silence some warnings.
 *
 * Revision 0.5  1997/04/15 06:00:14  sra
 * Update debugging code, add default TCP timeout handler.
 *
 * Revision 0.4  1997/03/20 06:57:57  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.2  1996/10/30 04:51:35  sra
 * Move glue stuff down to postamble section.
 *
 * Revision 0.1  1996/10/23  00:18:21  sar
 * *** empty log message ***
 *
 */

/* previous revision history when this was netbsd/attache.h 
 *
 * Revision 2.7  1996/10/16  16:44:02  sar
 * Added ifndefs around the memory and string functions so we
 * don't get compiler warnings.
 *
 * Revision 2.6  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.5  1996/02/13  21:07:54  sra
 * Fun with BUG() macros.
 *
 * Revision 2.4  1995/07/20  11:24:01  sra
 * More fun and games with optimized byte swapping.
 *
 * Revision 2.3  1995/06/28  19:41:56  sra
 * Add cookie argument to BUG() macro.  Change inline byteswap macros to
 * use GCC expression statements.
 *
 * Revision 2.2  1995/06/27  23:57:33  sra
 * Add BUG() definition for testing.
 *
 * Revision 2.1  1995/06/22  05:29:06  sra
 * Preliminary changes for multicast and OSPF.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.6  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PREAMBLE with #ifdef, not #if.
 *
 * Revision 1.5  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.4  1994/12/20  22:13:08  sra
 * Fix signature for crufty glue_pkt_handle() declaration.
 *
 * Revision 1.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.2  1994/09/04  06:13:38  sra
 * Clean up antique type names and install macros.
 *
 * Revision 1.1  1993/07/05  21:54:24  sra
 * Initial revision
 *
 */
/* comment to test CVS */
#ifdef CONFIG_PREAMBLE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PACKETS                     20
#define IP_MAX_PKTSIZE                  1536

#define TCP_MAXCONN                     128
#define TCP_DEFAULT_MAX_RETRANSMISSIONS 0

/* include PRINTF definition to enable printing/debugging code 
#ifndef PRINTF
#define PRINTF                          printf
#endif
*/

/*
 * Fun with our new run-time debugging facility.
 * This is still experimental, and defaults to uninstalled.
 */
#if defined(INSTALL_SNARK_BUG_CONF) && INSTALL_SNARK_BUG_CONF
#include <snark/h/bug_conf.h>
#endif

/*
 * Backwards compatability with old braindamage can be a real pain.
 * Since this port file is strictly for debugging, we set a couple
 * of configuration parameters to more reasonable values here.
 */

/* Please do install a retransmission timeout handler */
#define TCP_DEFAULT_MAX_RETRANSMISSIONS 0

/* Please buffer a reasonable number of ahead-of-sequence segments */
#define TCP_AHEAD_Q_MAX_DEFAULT 25

/* Maximum hardware address length, in bytes */
#define ATTACHE_MAX_HW_ADDRESS_LEN 6

#else /* CONFIG_PREAMBLE */

#include <install.h>

#ifndef GLUE_NOW
#define GLUE_NOW()                      glue_now()
extern bits32_t glue_now (void);
#endif

#ifndef GLUE_ALLOC
#define GLUE_ALLOC(x)                   ((void *) malloc(x))
#endif
#ifndef GLUE_FREE
#define GLUE_FREE(x)                    free((char *) (x))
#endif

#define GLUE_CKSUM(x, y)                glue_cksum((bits16_t *)(x), (y))
#define GLUE_CALLIN(x, y)               glue_callin(x, y)

/* define the GLUE_SNTP_SET_TIME macro if using SNTP Client */
#if INSTALL_ATTACHE_MULTICAST && INSTALL_ATTACHE_SNTP_CLIENT
#ifndef GLUE_SNTP_SET_TIME
#define GLUE_SNTP_SET_TIME(p, s, f)     glue_set_time((p),(s),(f))
#endif
#endif

/*
 * Fun with optimized ways of swapping bytes in GCC.
 */

#if 1

/*
 * "Safe" inline swap expressions using GCC "statement expressions".
 * Don't try these unless you're using GCC...
 */

#define GLUE_SWAP32(x) \
  ({ bits32_t _x_ = (x);\
     ((((_x_ & 0x000000ff) << 24) & 0xff000000)|\
      (((_x_ & 0x0000ff00) <<  8) & 0x00ff0000)|\
      (((_x_ & 0x00ff0000) >>  8) & 0x0000ff00)|\
      (((_x_ & 0xff000000) >> 24) & 0x000000ff)); })

#define GLUE_SWAP16(x) \
  ({ bits16_t _x_ = (x);\
     ((((_x_ &     0x00ff) <<  8) &     0xff00)|\
      (((_x_ &     0xff00) >>  8) &     0x00ff)); })

#else

static __inline__ bits32_t GLUE_SWAP32(bits32_t x)
{
  return ((((x & 0x000000ff) << 24) & 0xff000000)|
          (((x & 0x0000ff00) <<  8) & 0x00ff0000)|
          (((x & 0x00ff0000) >>  8) & 0x0000ff00)|
          (((x & 0xff000000) >> 24) & 0x000000ff));
}

static __inline__ bits16_t GLUE_SWAP16(bits16_t x)
{
  return ((((x &     0x00ff) <<  8) &     0xff00)|
          (((x &     0xff00) >>  8) &     0x00ff));
}

#endif

extern int glue_intlock (int);
extern bits16_t glue_cksum (bits16_t *, unsigned);
extern void glue_callin (bits32_t, void (*)(void));
extern void glue_set_time(packet *, bits32_t, bits32_t);

#ifndef PRINTF
#define PRINTF printf
#endif

#ifdef ATTACHE_IPSEC_H
#define GLUE_IPSEC_SA_EXPIRED(W,X,Y,Z) \
        glue_ipsec_sa_expired(W,X,Y,Z)
extern void glue_ipsec_sa_expired (ipsec_sa_dir_t, ipaddr_t *, 
                                   bits32_t, bits8_t);
#define GLUE_IPSEC_SND_PKT_DENIED(W,X,Y,Z)  \
        glue_ipsec_snd_pkt_denied((W),(X),(Y),(Z))

extern void glue_ipsec_snd_pkt_denied(packet *, ipaddr_t *, ipaddr_t *, int);


#define GLUE_IPSEC_RCV_PKT_BADSPI(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_badspi((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_badspi(packet *, ipaddr_t *, ipaddr_t *, 
                                      bits32_t, bits8_t);

#define GLUE_IPSEC_RCV_PKT_NOTPARTY(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_notparty((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_notparty(packet *, ipaddr_t *, ipaddr_t *,
                                        bits32_t, bits8_t);


#define GLUE_IPSEC_RCV_SA_NOTUNNEL(V,W,X,Y,Z) \
        glue_ipsec_rcv_sa_notunnel((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_sa_notunnel(packet *, ipaddr_t *, ipaddr_t *,
                                       bits32_t, bits8_t);


#define GLUE_IPSEC_RCV_TUNPKT_NOTIP(U,V,W,X,Y,Z) \
        glue_ipsec_rcv_tunpkt_notip((U),(V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_tunpkt_notip(packet *, packet *, ipaddr_t *, 
                                        ipaddr_t *, bits32_t, bits8_t);

#define GLUE_IPSEC_RCV_TUNPKT_NOTPARTY(S,T,U,V,W,X,Y,Z) \
        glue_ipsec_rcv_tunpkt_notparty((S),(T),(U),(V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_tunpkt_notparty(packet *, ipaddr_t *, ipaddr_t *, 
                                           packet *, ipaddr_t *, ipaddr_t *, 
                                           bits32_t, bits8_t);
                                           

#define GLUE_IPSEC_RCV_PKT_BADAH(V,W,X,Y) \
        glue_ipsec_rcv_pkt_badah((V),(W),(X),(Y))

extern void glue_ipsec_rcv_pkt_badah(packet *, ipaddr_t *, ipaddr_t *, 
                                     bits32_t);

#define GLUE_IPSEC_RCV_PKT_DENIED(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_denied((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_denied(packet *, ipaddr_t *, ipaddr_t *,
                                      ipsec_policy_flags_t, bits16_t);


#endif

#endif /* CONFIG_PREAMBLE */
