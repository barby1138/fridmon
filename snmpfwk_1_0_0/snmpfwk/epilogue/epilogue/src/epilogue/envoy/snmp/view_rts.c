/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/snmp/view_rts.c,v 1.1 2006/06/29 12:17:28 smasyutin Exp $ */

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
 * $Log: view_rts.c,v $
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
 * Revision 7.2  1997/03/20 06:49:26  sra
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
#include <auxfuncs.h>

#if !defined(SNMP_VIEW_DESTROY_HOOK)
#define SNMP_VIEW_DESTROY_HOOK(VIEW)
#endif

/****************************************************************************
NAME: SNMP_View_Create

PURPOSE: Create a view leaf with the given subtree and install the
         default values.

PARAMETERS: 
         OIDC_T       The subtree oidc
         int          The subtree length

RETURNS: VIEWLEAF_T *  A view structure with the default values.
                        (VIEWLEAV_T *)0 on error.
****************************************************************************/

VIEWLEAF_T *
  SNMP_View_Create(OIDC_T *oidc,
                   int     oidclen)
{
VIEWLEAF_T *view;

view = (VIEWLEAF_T *)SNMP_memory_alloc(sizeof(VIEWLEAF_T));
if (view == 0)
    return (0);

if (build_object_id(oidclen, oidc, &view->subtree) != 0) {
    SNMP_memory_free(view);
    return (0);
    }

SNMP_View_Set_Defaults(view);
return(view);
}

/****************************************************************************
NAME: SNMP_View_Delete

PURPOSE: Given a view delete it and free the space.

PARAMETERS: 
        VIEWLEAF_T *   A pointer to the view to delete.

RETURNS: Nothing
****************************************************************************/

void
  SNMP_View_Delete(VIEWLEAF_T *view)
{
if (view == 0)
    return;
Clean_Obj_ID(&view->subtree);
EBufferClean(&view->mask);
SNMP_memory_free(view);
}

/****************************************************************************
NAME: SNMP_View_Install

PURPOSE: Given a view, index and objid install the view in the view area

PARAMETERS: 
         UINT_16_T    The view index
         VIEWLEAF_T * The view to stick in the view area

RETURNS: int       Success = 0, Failure = non zero
****************************************************************************/

int
  SNMP_View_Install(UINT_16_T   indx,
                    VIEWLEAF_T *view)
{
VIEWINDEX_T *vindex, **prevvindex, *newvindex;
VIEWLEAF_T *curview, **prevview;
int order, length;

/* look through the index list, if we can't find an index create one and
   link the view to it and it into the list. */
vindex = viewroot;
prevvindex = &viewroot;
while (1) {
    if ((vindex == 0) || (vindex->index > indx)) {
        newvindex = (VIEWINDEX_T *)SNMP_memory_alloc(sizeof(VIEWINDEX_T));
        if (newvindex == 0)
            return(-1);
        newvindex->index     = indx;
        newvindex->next      = vindex;
        newvindex->lexilist  = view;
        newvindex->masklist  = view;
        newvindex->quickmask = 0;

        *prevvindex = newvindex;
        view->index = indx;
        return(0);
        }
    if (vindex->index == indx)
        break;

    prevvindex = &(vindex->next);
    vindex = vindex->next;
    }

/* the index already exists  */
/* install the view in the masklist */
curview = vindex->masklist;
prevview = &(vindex->masklist);
while (1) {
    if ((curview == 0) || 
        (view->subtree.num_components > curview->subtree.num_components))
        break;

    if (view->subtree.num_components == curview->subtree.num_components) {
        order = oidorder(view->subtree.component_list,
                         curview->subtree.component_list,
                         view->subtree.num_components);
        if (order == 1)
            break;
        if (order == 0)
            return(-1);
        }
    prevview = &(curview->masknext);
    curview = curview->masknext;
    }
*prevview = view;
view->masknext = curview;

/* install the view in the lexilist */
curview = vindex->lexilist;
prevview = &(vindex->lexilist);
while (1) {
    if (curview == 0)
        break;

    length = (view->subtree.num_components <= curview->subtree.num_components) 
      ? view->subtree.num_components : curview->subtree.num_components;

    order = oidorder(view->subtree.component_list,
                     curview->subtree.component_list,
                     length);
    if (order == -1)
        break;
    if (order == 0) {
        if (view->subtree.num_components < curview->subtree.num_components)
            break;
        if (view->subtree.num_components == curview->subtree.num_components)
            return(-1);
        }
    prevview = &(curview->lexinext);
    curview = curview->lexinext;
    }
*prevview = view;
view->lexinext = curview;

/* put the index into the view leaf for admin purposes */
view->index = indx;
return(0);
}

/****************************************************************************
NAME: SNMP_View_Name

PURPOSE: Given a view get it's name (oidc)

PARAMETERS: 
        VIEWEAF_T   *   A pointer to the view
        UINT_16_T   *   The index of the view
        OIDC_T      *   Space for the returned name
        int             length of the space
          
RETURNS: int   the number of ids in the name 0 on failure
****************************************************************************/

int
  SNMP_View_Name(VIEWLEAF_T *view,
                 UINT_16_T  *indx,
                 OIDC_T     *oidc,
                 int         len)
{
int i;

if (view == 0)
    return(0);

*indx = view->index;
if (view->subtree.num_components > len)
    return(view->subtree.num_components);

for (i = view->subtree.num_components - 1; i >= 0; i--)
    oidc[i] = view->subtree.component_list[i];
return(view->subtree.num_components);
}

/****************************************************************************
NAME: SNMP_View_Deinstall

PURPOSE: Given an index and subtree remove the named view from the area.

PARAMETERS: 
         UINT_16_T      The index
         OIDC_T *       The component list of the subtree
         int            The lenght of the component list

RETURNS: int            Success = 0, Failure = -1
****************************************************************************/
int
  SNMP_View_Deinstall(UINT_16_T  indx,
                      OIDC_T    *oidc,
                      int        oidclen)
{
VIEWINDEX_T *vindex, **prevvindex;
VIEWLEAF_T  *view, **prevview;

/* step through the index list until we find the correct one, they are 
   ordered so if we find an index greater than ours, ours doesn't exist
   and we are done */
vindex = viewroot;
prevvindex = &viewroot;
while(1) {
    if ((vindex == 0) || (vindex->index > indx))
        return(-1);
    if (vindex->index == indx)
        break;
    prevvindex = &(vindex->next);
    vindex = vindex->next;
    }

/* find the requested view in the mask list.  the mask list is ordered by
   number of components (largest first) then by oid (lexi last first).  
   So if we find a size less than ours, ours doesn't exist and we are done. */
view = vindex->masklist;
prevview = &(vindex->masklist);
while(1) {
    if ((view == 0) || (oidclen > view->subtree.num_components))
        return(-1);
    if (oidcmp(oidclen, oidc, view->subtree.num_components,
               view->subtree.component_list) == 1)
        break;
    prevview = &(view->masknext);
    view = view->masknext;
    }
/* unlink the view */
SNMP_VIEW_DESTROY_HOOK(view);
*prevview = view->masknext;

/* find the requested view in the lexi list.  the lexi list is ordered by
   oid in standard lexi order. */
for (view = vindex->lexilist, prevview = &(vindex->lexilist);
     oidcmp(oidclen, oidc, view->subtree.num_components,
            view->subtree.component_list) == 0;
     prevview = &(view->lexinext),
     view = view->lexinext)
    /* no body to the for loop */
    ;

/* unlink the view */
*prevview = view->lexinext;

/* if there are no more views for this index we remove it as well */
if (vindex->masklist == 0) {
    *prevvindex = vindex->next;
    SNMP_memory_free(vindex);
    }
return(0);
}

/****************************************************************************
NAME: SNMP_View_Lookup

PURPOSE: Given a view index and id (object id) find the corresponding
         VIEWLEAF_T

PARAMETERS: 
        UINT_16_T       The index for the view.
        OIDC_T    *     The id to find a VIEWLEAF_T for.
        int             The length of the name

RETURNS: VIEWLEAF_T *   The view structure corresponding to the name.
                        On error (VIEWLEAF_T *) 0;
****************************************************************************/

VIEWLEAF_T *
  SNMP_View_Lookup(UINT_16_T indx,
                   OIDC_T   *oidc,
                   int       oidclen)
{
VIEWINDEX_T *vindex;
VIEWLEAF_T *view;

vindex = viewroot;
while(1) {
    if ((vindex == 0) || (vindex->index > indx))
        return(0);

    if (vindex->index == indx)
        break;
    vindex = vindex->next;
    }

view = vindex->masklist;
while((view != 0) && (oidclen <= view->subtree.num_components)){
    if (oidcmp(oidclen, oidc, view->subtree.num_components,
               view->subtree.component_list) == 1)
        return(view);
    view = view->masknext;
    }
return(0);
}

/****************************************************************************
NAME: SNMP_View_Next

PURPOSE: Given a pointer to a view find the next view.  If the argument
         is a 0 return the first view in the tree, if the argument
         is the last view return 0.

PARAMETERS: 
        VIEWLEAF_T *    The view to start with

RETURNS: VIEWLEAF_T *   The next view in the tree, 0 if there isn't a 
                        next view
****************************************************************************/

VIEWLEAF_T *
  SNMP_View_Next(VIEWLEAF_T *view)
{
VIEWINDEX_T *vindex;
UINT_16_T    indx;

/* if we don't have a view we get the very first view */
if (view == 0) {
    if (viewroot == 0)
        return(0);
    return(viewroot->lexilist);
    }

/* see if we have another view in this family */
if (view->lexinext != 0)
    return(view->lexinext);

/* we don't have another view in this family so we find the next family
   which requires us to find the index in the current view and find whatever
   is after that */
for (vindex = viewroot, indx = view->index; vindex != 0; vindex = vindex->next)
    if (vindex->index > indx)
        return(vindex->lexilist);

/* no more entries */
return(0);
}

/****************************************************************************
NAME: SNMP_View_Set_Mask

PURPOSE: Set the mask to a string

PARAMETERS: 
        VIEWLEAF_T  * The view to update
        OCTET_T     * The buffer
        int           The length of the buffer
        
RETURNS: int   0 = success
****************************************************************************/
int
  SNMP_View_Set_Mask(VIEWLEAF_T *view,
                     OCTET_T    *buf,
                     int         buflen)
{
if ((buflen < 0) || (buflen > 16))
    return(-1);

/* copy the new mask into the old mask area */
return(EBufferReplace(&view->mask, buf, (ALENGTH_T)buflen));
}

