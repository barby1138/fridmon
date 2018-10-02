/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/include/epilogue/envoy/h/asn1conf.h,v 1.1 2006/06/29 12:17:07 smasyutin Exp $ */

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
 * $Log: asn1conf.h,v $
 * Revision 1.1  2006/06/29 12:17:07  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:26:09  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:30  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:20  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/10/30 04:23:16  sar
 * Update type information mostly convert bits16_ts to ALENGTH_Ts
 * also add MAX_ALENGTH
 *
 * Revision 7.4  1997/10/22 03:07:28  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.3  1997/08/21 17:23:44  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 7.2  1997/03/20 06:47:59  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 */

#if !defined(asn1conf_inc)
#define asn1conf_inc

#if !defined(EPILOGUE_INSTALL_H)
#include <install.h>
#endif

/* Get the standard types and the install defines */
#if !defined(EPILOGUE_TYPES_H)
#include <types.h>
#endif

/* !!! These are here for backwards compatibility only while we're switching
 * over to the new configuration system. */
#if (INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2)
#define SGRP 1
#endif

/*****************************************************************
 * Portable types definitions for backwards compatibility.  These
 * are now defined in terms of things defined by <types.h> and
 * therefore <oemtypes.h> so any changes should be made in
 * <oemtypes.h>.
 *****************************************************************/
        /* various sizes of integers */
typedef sbits32_t       INT_32_T;
typedef bits32_t        UINT_32_T;
typedef sbits16_t       INT_16_T;
typedef bits16_t        UINT_16_T;
typedef bits8_t         UINT_8_T;
typedef bits8_t         OCTET_T ;
        /* A generic pointer type.  Use void * if your compiler can hack
         * it, otherwise char * is probably the right thing. */
typedef ptr_t           PTR_T;
        /* SNMP ASN.1 tags and lengths can always fit into 16 bits...   */
typedef bits16_t        ATVALUE_T;      /* ASN.1 type values            */
typedef bits16_t        ALENGTH_T;      /* ASN.1 contents field lengths */
        /* this is used as both a maximum and a mask */
#define MAX_ALENGTH     0xFFFF

#define OCTETS_PER_INT32    (sizeof(INT_32_T)/sizeof(OCTET_T))
#define OCTETS_PER_INT16    (sizeof(INT_16_T)/sizeof(OCTET_T))


/*
 * get the common system configuration
 */
#include <common/h/config.h>

/*
 * get the system configuration for envoy
 */
#define CONFIG_PREAMBLE 1
#include <envoy.h>
#undef CONFIG_PREAMBLE


/*****************************************************************
 * Set the size of object identifier components.  32 bits is recommended
 * but 16 bits may work and may save a little space.
 * This may be overridden in <oemtypes.h> by defining __TYPES_HAVE_OIDC_T_
 *****************************************************************/
#ifndef __TYPES_HAVE_OIDC_T_
typedef bits32_t        OIDC_T;
#endif

/* min and max macros if not provided by the system */
#if (!defined(max))
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#if (!defined(min))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/* These are the standard C functions that we use in Envoy.  If the
 * customer's compiler supplies these correctly then nothing more need be
 * done.  If not, then <envoy.h> can override.  The most common case is
 * that the system provides them but with the wrong type arguments (char *
 * instead of void *).  The macros can then fix this with casts. */
#ifndef MEMCPY
#define MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif
#ifndef MEMSET
#define MEMSET(dst, val, n) memset(dst, val, n)
#endif
#ifndef MEMCMP
#define MEMCMP(s1, s2, n) memcmp(s1, s2, n)
#endif
#ifndef STRLEN
#define STRLEN(s) strlen(s)
#endif

/* Error checking, Look for inconsistencies or necessary configuration
   that hasn't been done */

/* Check that that the lock & serialization options are consistent */
#if (INSTALL_ENVOY_SNMP_LOCK && INSTALL_ENVOY_SNMP_SERIALIZE)
#error Must not have both the lock code and the serialization code installed.
#error You should modify the configuration file and remove either
#error the ENVOY_SNMP_LOCK or the ENVOY_SNMP_SERIALIZE option
#endif

#if (INSTALL_ENVOY_CONTINUE_REENTRANT && !INSTALL_ENVOY_SNMP_LOCK)
#error Must have locks installed to use reentrant continues.
#error You should modify the configuration file and install
#error the ENVOY_SNMP_LOCK option.
#endif

/* check that the options for our sub agent scheme are consistent */
#if ((INSTALL_ENVOY_EPI_MASTER || INSTALL_ENVOY_EPI_SUB) && \
     !(INSTALL_ENVOY_EPI_SUBAGENT_SCHEME))
#error You must install ENVOY_EPI_SUBAGENT_SCHEME with ENVOY_EPI_MASTER
#error and ENVOY_EPI_SUB
#endif

#if (INSTALL_ENVOY_EPI_MASTER)
#if !(INSTALL_ENVOY_MAIN_AGENT)
#error You must install ENVOY_MAIN_AGENT for use with a master agent
#endif

#if !(INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
#error You must install ENVOY_SNMP_RFC1445_VIEWS for use with a master agent
#endif

#if (!INSTALL_ENVOY_SNMP_LOCK && !INSTALL_ENVOY_SNMP_SERIALIZE)
#error Must have either the locking scheme or the serialization scheme
#error installed.  You should modify the configuration file and install
#error either the ENVOY_SNMP_LOCK or ENVOY_SNMP_SERIALIZE option
#endif
#endif

/* check that the options for the agentx scheme are consistent */
#if ((INSTALL_ENVOY_AGENTX_MASTER || INSTALL_ENVOY_AGENTX_SUB) && \
     !(INSTALL_ENVOY_AGENTX))
#error You must install ENVOY_AGENTX with ENVOY_AGENTX_MASTER
#error and ENVOY_AGENTX_SUB
#endif


#if (INSTALL_ENVOY_EPI_MASTER)
#if (INSTALL_ENVOY_MAIN_AGENT && !(INSTALL_ENVOY_SNMP_RFC1445_VIEWS))
#error You must install ENVOY_SNMP_RFC1445_VIEWS for use with a master agent
#endif

#if (!INSTALL_ENVOY_SNMP_LOCK && !INSTALL_ENVOY_SNMP_SERIALIZE)
#error Must have either the locking scheme or the serialization scheme
#error installed.  You should modify the configuration file and install
#error either the ENVOY_SNMP_LOCK or ENVOY_SNMP_SERIALIZE option
#endif
#endif

/* If we don't have a main agent we shouldn't be counting stats */
#if ((INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2) && \
     !(INSTALL_ENVOY_MAIN_AGENT))
#error Must not have the stats groups installed if building only a 
#error sub agent.  You should modify the configuration file
#error for the makmak program and either uninstall the SNMP_GROUP
#error options, add the MASTER agent option or remove the SUB agent option
#endif

#endif /* asn1conf_inc */
