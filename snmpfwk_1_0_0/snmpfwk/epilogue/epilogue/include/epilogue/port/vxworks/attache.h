/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/port/vxworks/attache.h,v 1.1 2006/06/29 12:17:13 smasyutin Exp $ */

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
 * $Log: attache.h,v $
 * Revision 1.1  2006/06/29 12:17:13  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:18  denysg
 * no message
 *
 * Revision 1.12  1998/03/12 18:23:44  meister
 * GLUE_IPSEC_RCV_SA_NOTUNNEL()
 *
 * Revision 1.11  1998/03/11 21:10:04  meister
 * Added some parameters to IPSEC glue macros
 *
 * Revision 1.10  1998/03/11 20:20:49  mrf
 * Comment out PRINTF declaration (again)
 *
 * Revision 1.9  1998/03/03 20:36:43  mrf
 * Change GLUE_SNTP_SET_TIME macro to pass packet pointer
 *
 * Revision 1.8  1998/03/03 15:26:16  mrf
 * Comment out PRINTF declarations.
 *
 * Revision 1.7  1998/02/27 23:38:12  meister
 * Add stdio.h
 *
 * Revision 1.6  1998/02/27 22:30:36  meister
 * Added ipsec glue functions
 *
 * Revision 1.5  1998/02/25 04:57:07  sra
 * Update copyrights.
 *
 * Revision 1.4  1998/02/18 22:34:59  sra
 * Add ip_loopback(), eradicate glue_pkt_handle().
 *
 * Revision 1.3  1998/02/16 17:43:08  mrf
 * Add prototype for glue_set_time()
 *
 * Revision 1.2  1998/02/13 17:49:37  mrf
 * Add conditionalized SNTP set-time macro.
 *
 * Revision 1.1  1997/09/15 15:48:28  sra
 * GLUE_NOW() migration version skew.
 *
 * Revision 1.0  1997/08/08 22:11:22  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.9  1997/05/01 23:11:55  sra
 * Work around broken SunOS memcmp().
 *
 * Revision 0.8  1997/03/20 06:57:51  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.7  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.6  1997/02/25 07:59:40  sra
 * Comment out glue_pkt_handle(), we don't need it anymore.
 *
 * Revision 0.5  1997/02/24 00:56:59  sra
 * Support for run-time configuration of BUG() macros.
 *
 * Revision 0.4  1997/01/17 23:11:38  sra
 * Fiddle configuration parameters to make this configuration a little
 * more suitable for performance testing.
 *
 * Revision 0.3  1996/10/30 18:28:52  sra
 * The curse of htonx() strikes again.
 *
 * Revision 0.2  1996/10/30  04:54:49  sra
 * Move glue stuff down to postamble section.
 *
 * Revision 0.1  1996/10/23  00:14:56  sar
 * *** empty log message ***
 *
 */

/* Previous revision history when this was snark/port/attache.h
 * 
 * Revision 2.2  1996/10/16  16:49:55  sar
 * Added ifndefs around the memory and string routines
 * to keep compilers happy.
 *
 * Revision 2.1  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
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
 * Revision 1.4  1994/12/20  22:13:46  sra
 * Fix signature for crufty glue_pkt_handle() declaration.
 *
 * Revision 1.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.2  1994/09/04  06:13:38  sra
 * Clean up antique type names and install macros.
 *
 * Revision 1.1  1994/01/09  23:57:29  sra
 * Add support for NIT under SunOS 4.1.2.
 *
 */

#ifdef CONFIG_PREAMBLE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>

#define MAX_PACKETS                     50
#define IP_MAX_PKTSIZE                  1536

#define TCP_MAXCONN                     128

#ifndef MEMCPY
#define MEMCPY(x, y, z)                 memcpy(x, y, z)
#endif

/*
 * SunOS 4.1.4's memcmp() does signed character comparision, so replace it.
 * This inline code will only work with gcc, but this particular port file
 * is only used for testing and demos anyway, so what the heck.
 */
#ifndef MEMCMP
#define MEMCMP(x, y, z) \
  ({ \
    register unsigned char *_x_ = (void *) (x), *_y_ = (void *) (y); \
    register unsigned _z_ = (z); \
    int _r_ = 0; \
    while (_z_-- > 0) \
      if ((_r_ = (*_x_++) - (*_y_++)) != 0) \
        break; \
    _r_; \
  })
#endif

#ifndef MEMSET
#define MEMSET(x, y, z)                 memset(x, y, z)
#endif

#ifndef STRCMP
#define STRCMP(x, y)                    strcmp(x, y)
#endif

#ifndef STRCPY
#define STRCPY(x, y)                    strcpy(x, y)
#endif

#ifndef STRLEN
#define STRLEN(x)                       strlen(x)
#endif

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

#else /* CONFIG_PREAMBLE */

#include <install.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef htons
#undef htons
#endif

#ifdef htonl
#undef htonl
#endif

extern unsigned short htons (unsigned short);
extern unsigned long  htonl (unsigned long);

#define GLUE_ALLOC(x)                   ((void *) malloc(x))
#define GLUE_FREE(x)                    free((char *) (x))
#define GLUE_CKSUM(x, y)                glue_cksum((bits16_t *)(x), (y))
#define GLUE_SWAP16(x)                  ((bits16_t) htons((unsigned short) (x)))
#define GLUE_SWAP32(x)                  ((bits32_t) htonl((unsigned long) (x)))
#define GLUE_CALLIN(x, y)               glue_callin(x, y)

extern int glue_intlock (int);
extern bits16_t glue_cksum (bits16_t *, unsigned);
extern void glue_callin (bits32_t, void (*)(void));

#ifndef GLUE_NOW
#define GLUE_NOW()                      glue_now()
extern bits32_t glue_now (void);
#endif

/* define the GLUE_SNTP_SET_TIME macro if using SNTP Client */
#if INSTALL_ATTACHE_MULTICAST && INSTALL_ATTACHE_SNTP_CLIENT
#ifndef GLUE_SNTP_SET_TIME
#define GLUE_SNTP_SET_TIME(p, s, f)     glue_set_time((p),(s),(f))
extern void glue_set_time(packet *, bits32_t, bits32_t);
#endif
#endif

/* Define PRINTF for debugging and printing code 
#ifndef PRINTF
#define PRINTF printf
#endif
*/

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


#define GLUE_IPSEC_RCV_TUNPKT_NOTIP(U,V,W,X,Y,Z) \
        glue_ipsec_rcv_tunpkt_notip((U),(V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_tunpkt_notip(packet *, packet *, ipaddr_t *, 
                                        ipaddr_t *, bits32_t, bits8_t);

#define GLUE_IPSEC_RCV_TUNPKT_NOTPARTY(S,T,U,V,W,X,Y,Z) \
        glue_ipsec_rcv_tunpkt_notparty((S),(T),(U),(V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_tunpkt_notparty(packet *, ipaddr_t *, ipaddr_t *, 
                                           packet *, ipaddr_t *, ipaddr_t *, 
                                           bits32_t, bits8_t);

#define GLUE_IPSEC_RCV_SA_NOTUNNEL(V,W,X,Y,Z) \
        glue_ipsec_rcv_sa_notunnel((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_sa_notunnel(packet *, ipaddr_t *, ipaddr_t *,
                                       bits32_t, bits8_t);


#define GLUE_IPSEC_RCV_PKT_BADAH(V,W,X,Y) \
        glue_ipsec_rcv_pkt_badah((V),(W),(X),(Y))

extern void glue_ipsec_rcv_pkt_badah(packet *, ipaddr_t *, ipaddr_t *, 
                                     bits32_t);

#define GLUE_IPSEC_RCV_PKT_DENIED(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_denied((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_denied(packet *, ipaddr_t *, ipaddr_t *,
                                      ipsec_policy_flags_t, bits16_t);


#endif /* ATTACHE_IPSEC */


#endif /* CONFIG_PREAMBLE */
