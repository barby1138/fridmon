/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/view_chk.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: view_chk.c,v $
 * Revision 1.1  2006/06/29 12:17:28  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:04  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:40  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:57:06  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:49:25  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:01:11  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 1.1  1995/11/11  00:07:30  sar
 * Initial revision
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <objectid.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <view.h>

VIEWINDEX_T *viewroot = 0;

/****************************************************************************
NAME: SNMP_View_Check

PURPOSE: Given a view index and id (object id) find the corresponding
         leaf and determine if the object id is in the view.  If the
         determinate flag is set then we return a yes or no answer 
         about the given id, if the indeterminate flag is set we
         return a yes, no or maybe.  Maybe indicates that there may be
         instances under the given id that are yes and some that are
         no.

PARAMETERS: 
        UINT_16_T       The index for the view.
        OIDC_T    *     The id to check on
        int             The length of the name
        int             Determinate or indeterminate check.

RETURNS: int    VIEW_INCLUDED      1 if the object is in the view
                VIEW_EXCLUDED      2 if it isn't 
                VIEW_INDETERMINATE 3 if are not sure, this will only be
                                     returned if the indeterminate flag
                                     was set in the arguments.
****************************************************************************/

int
  SNMP_View_Check(UINT_16_T indx,
                  OIDC_T   *oidc,
                  int       oidclen,
                  int       option)
{
VIEWINDEX_T *vindex;
VIEWLEAF_T  *view;
int          i, j, used, vc = 0;
OIDC_T      *testoidc, *subtree;
OCTET_T     *mask;

vindex = viewroot;
while(1) {
    if ((vindex == 0) || (vindex->index > indx))
        return(VIEW_EXCLUDED);

    if (vindex->index == indx)
        break;
    vindex = vindex->next;
    }

/* Check the determinate flag, if we are doing a determinate check
   we just step through the list until the id we are testing has at
   least as many subids as the subtree's.
   If we are doing an indeterminate check we have to check all of the
   subtrees that are longer than the id to see if any of them 
   could be an instance of this id.  If some are we need to determine
   if we know if all items under the id are the same (included or
   excluded). */

if (option == VIEW_CHECK_DET) {
    for (view = vindex->masklist; 
        (view != 0) && (oidclen < view->subtree.num_components);
         view = view->masknext)
        /* no body */
        ;
    }
else {
    for (view = vindex->masklist; 
        (view != 0) && (oidclen < view->subtree.num_components);
         view = view->masknext) {
        if ((view->status != RS_ACTIVE) || (vc == 1))
            continue;
        i = 0;
        j = oidclen;
        testoidc = oidc;
        subtree = view->subtree.component_list;
        mask = EBufferStart(&view->mask);
        used = EBufferUsed(&view->mask) * 8;
        while (i < j) {
            if (*testoidc != *subtree) {
                if (i < used) {
                    if (mask[i/8] & (0x80 >> (i%8)))
                        break;
                    }
                else
                    break;
                }
            i++;
            testoidc++;
            subtree++;
            }
        if (i == j) {
            if (vc == 0)
                vc = view->type;
            else
                if (vc != view->type)
                    return(VIEW_INDETERMINATE);
            }
        }
    }

/* so we have checked any views that may be under the subtree
   now we check the id itself. */

for (; view != 0; view = view->masknext) {
    if (view->status != RS_ACTIVE)
        continue;
    i = 0;
    j = view->subtree.num_components;
    testoidc = oidc;
    subtree = view->subtree.component_list;
    mask = EBufferStart(&view->mask);
    used = EBufferUsed(&view->mask) * 8;
    while (i < j) {
        if (*testoidc != *subtree) {
            if (i < used) {
                if (mask[i/8] & (0x80 >> (i%8)))
                    break;
                }
            else
                break;
            }
        i++;
        testoidc++;
        subtree++;
        }
    if (i == j) {
        if ((vc == 0) || (vc == view->type))
            return(view->type);
        return(VIEW_INDETERMINATE);
        }
    }

if (vc == VIEW_INCLUDED)
    return(VIEW_INDETERMINATE);
return(VIEW_EXCLUDED);
}
