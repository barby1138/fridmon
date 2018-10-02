/* $Header: /services/cvsroot/fridmon/snmpfwk_1_0_0/snmpfwk/epilogue/epilogue/src/epilogue/envoy/auxi/viewmth.c,v 1.1 2006/06/29 12:17:26 smasyutin Exp $ */

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
 * $Log: viewmth.c,v $
 * Revision 1.1  2006/06/29 12:17:26  smasyutin
 * - epilogue version
 *
 * Revision 1.1  2004/09/23 09:28:06  denysg
 * no message
 *
 * Revision 1.1  2003/08/12 12:52:23  alext
 * no message
 *
 * Revision 8.0  1997/11/18 00:56:18  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/03/20 06:50:31  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1996/12/04 20:45:13  sar
 * corrected some error codes
 * tproc_resource_unavailable -> resource_unavailable
 * no_such_instance -> no_creation
 *
 * Revision 7.0  1996/03/15  22:21:24  sar
 * Updated revision to 7.0 and copyright to 96
 *
 * Revision 1.2  1995/11/11  00:00:48  sar
 * added snmp_view_destroy_backout which will be called after a
 * destroy_test succeeds but we need to backout any changes
 * added viewtable_destory_cleanup as a vb_priv_free to call the destroy
 * backout macro
 * added snmp_view_finished which will be called if the packet completes
 * successfully (so that data can be flushed).
 * mark the sets & undos as done if we have a request to destroy a view
 * that doesn't exist
 * added a return clause in viewtable_set if we can't deinstall a view.
 *
 * Revision 1.1  1995/11/01  00:55:20  sar
 * Initial revision
 *
 * Revision 1.1  1995/09/18  19:27:22  sar
 * Initial revision
 *
 */

#include <asn1conf.h>
#include <asn1.h>
#include <buffer.h>
#include <objectid.h>
#include <snmpdefs.h>
#include <snmp.h>
#include <mib.h>
#include <view.h>
#include <auxfuncs.h>

extern VIEWINDEX_T *viewroot;
#if !defined(SNMP_VIEW_FINISHED)
#define SNMP_VIEW_FINISHED
#endif

#if !defined(SNMP_VIEW_CREATE_HOOK)
#define SNMP_VIEW_CREATE_HOOK(NEW)
#endif
#if !defined(SNMP_VIEW_UPDATE_HOOK)
#define SNMP_VIEW_UPDATE_HOOK(OLD, NEW)
#endif
#if !defined(SNMP_VIEW_DESTROY_HOOK)
#define SNMP_VIEW_DESTROY_HOOK(OLD)
#endif
#if !defined(SNMP_VIEW_BACKOUT_HOOK)
#define SNMP_VIEW_BACKOUT_HOOK(NEW)
#endif

#if !defined(SNMP_VIEW_CREATE_TEST)
#define SNMP_VIEW_CREATE_TEST(PKT, NEW)      TPROC_GOOD
#endif
#if !defined(SNMP_VIEW_UPDATE_TEST)
#define SNMP_VIEW_UPDATE_TEST(PKT, OLD, NEW) TPROC_GOOD
#endif
#if !defined(SNMP_VIEW_DESTROY_TEST)
#define SNMP_VIEW_DESTROY_TEST(PKT, OLD)     TPROC_GOOD
#endif

/* view table lastmatch values */
#define LM_viewIndex            1
#define LM_viewSubtree          2
#define LM_viewMask             3
#define LM_viewType             4
#define LM_viewStorageType      5
#define LM_viewStatus           6
extern MIBLEAF_T viewMask_leaf, viewType_leaf, viewStorageType_leaf;
extern MIBLEAF_T viewStatus_leaf;

/****************************************************************************
NAME: viewtable_destroy_cleanup

PURPOSE: Cleanup after a view test (delete) succeeded but some other test
         failed.  We merely call the DESTROY_BACKOUT macro to allow
         the customer to restore any changed state.
         Expects a PTR_T or VB_T rather than a VIEWLEAF_T

PARAMETERS: 
        PTR_T    A pointer to the View to delete or
                 A pointer to the vb that points to the view to delete
                 case as a PTR_T

RETURNS: Nothing
****************************************************************************/

void
  viewtable_destroy_cleanup(PTR_T viewptr)
{
#if defined(SNMP_VIEW_DESTROY_BACKOUT)
VIEWLEAF_T *view;

#if INSTALL_ENVOY_40_VB_FREE_COMPAT
view = (VIEWLEAF_T *)viewptr;
#else
view = (VIEWLEAF_T *)(((VB_T *)viewptr)->vb_priv);
#endif

SNMP_VIEW_DESTROY_BACKOUT(view);
#endif /* #if defined(SNMP_VIEW_DESTROY_BACKOUT) */
}

/****************************************************************************
NAME: viewtable_test_cleanup

PURPOSE: Cleanup after a view test succeeded but some other test
         failed.  Delete the view, free the space
         and run BACKOUT_HOOK.
         Expects a PTR_T or VB_T rather than a VIEWLEAF_T

PARAMETERS: 
        PTR_T    A pointer to the View to delete or
                 A pointer to the vb that points to the view to delete
                 case as a PTR_T

RETURNS: Nothing
****************************************************************************/

void
  viewtable_test_cleanup(PTR_T viewptr)
{
VIEWLEAF_T *view;

#if INSTALL_ENVOY_40_VB_FREE_COMPAT
view = (VIEWLEAF_T *)viewptr;
#else
view = (VIEWLEAF_T *)(((VB_T *)viewptr)->vb_priv);
#endif

/* If this was a create then the same view was installed
   in the view tree and pointed to by the vb_priv pointer.
   In this case we need to deinstall the view as we cleanup. */
if (SNMP_View_Lookup(view->index, view->subtree.component_list,
                     view->subtree.num_components) == view)
    SNMP_View_Deinstall(view->index, view->subtree.component_list,
                        view->subtree.num_components);

SNMP_VIEW_BACKOUT_HOOK(view);
SNMP_View_Delete(view);
}

/****************************************************************************
NAME: viewtable_set_cleanup

PURPOSE: Free the given view.  Expects a PTR_T
         or VB_T rather than a VIEWLEAF_T

PARAMETERS: 
        PTR_T    A pointer to the View to delete or
                 A pointer to the vb that points to the view to delete
                 case as a PTR_T

RETURNS: Nothing
****************************************************************************/

void
  viewtable_set_cleanup(PTR_T viewptr)
{
if (viewptr) {
#if INSTALL_ENVOY_40_VB_FREE_COMPAT
    SNMP_View_Delete((VIEWLEAF_T *)viewptr);
#else
    SNMP_View_Delete((VIEWLEAF_T *)(((VB_T *)viewptr)->vb_priv));
#endif
    }
SNMP_VIEW_FINISHED;
}

/****************************************************************************
NAME:  viewtable_undo

PURPOSE:  This routine attempts to undo a previous set.  It expects the
          the old information to be pointed to be vb_priv and swaps the
          old with the new.  If vb_priv is empty it means that this was
          a creation and we deinstall the entry.  Finally we delete the
          new entry.

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T       *    Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  viewtable_undo(OIDC_T          last_match,
                 int             compc,
                 OIDC_T         *compl,
                 SNMP_PKT_T     *pktp,
                 VB_T           *vbp)
{
VIEWLEAF_T *newview, *oldview;

oldview = (VIEWLEAF_T *)vbp->vb_priv;
newview = SNMP_View_Lookup((UINT_16_T)compl[0], compl + 1, compc -1);

/* reset the vb_priv information so we don't have any accidents */
vbp->vb_priv      = 0;
vbp->vb_free_priv = 0;

/* If vb_priv is empty then this was a creation so we deinstall and free
   the entry */
if (oldview == 0) {
    if (newview != 0) {
#if defined(SNMP_VIEW_CREATE_UNDO)
        if (SNMP_VIEW_CREATE_UNDO(newview))
            undoproc_error(pktp, vbp, UNDO_FAILED);
        else
#endif
            {
            if (SNMP_View_Deinstall((UINT_16_T)compl[0],
                                    compl + 1, compc -1) == 0)
                SNMP_View_Delete(newview);
            }
        }
    }
else {
    /* Otherwise we either modifed the entry (if newparty != 0) or deleted the
       old entry.  For a modification we swap them and free the new block.  For
       a deletion we attempt to reinstall the old block and generate a failure
       if we can't. */
    if (newview) {
#if defined(SNMP_VIEW_UPDATE_UNDO)
        if (SNMP_VIEW_UPDATE_UNDO(oldview, newview)) {
            undoproc_error(pktp, vbp, UNDO_FAILED);
            }
        else
#endif
            {
            /* copy the old stuff to the new block.  we use the new block as
               it is the one threaded onto the view lists. */
            newview->type    = oldview->type;
            newview->storage = oldview->storage;
            newview->status  = oldview->status;
            EBufferClean(&newview->mask);
            MEMCPY(&newview->mask, &oldview->mask, sizeof(EBUFFER_T));
            EBufferInitialize(&oldview->mask);
            }
        SNMP_View_Delete(oldview);
        }
    else {
        if (SNMP_View_Install((UINT_16_T)compl[0], oldview)) {
            SNMP_View_Delete(oldview);
            undoproc_error(pktp, vbp, UNDO_FAILED);
            }
#if defined(SNMP_VIEW_DESTROY_UNDO) 
        else if (SNMP_VIEW_DESTROY_UNDO(oldview)) {
            undoproc_error(pktp, vbp, UNDO_FAILED);
            if (SNMP_View_Deinstall((UINT_16_T)compl[0],
                                    compl + 1, compc -1) == 0)
                SNMP_View_Delete(oldview);
            }
#endif
        }
    }

undoproc_good(pktp, vbp);
return;
}

/****************************************************************************
NAME:  viewtable_test

PURPOSE:  This routine collects all of the var binds that want to be set
          in a row and does value and consistency checking on those
          varbinds before trying to allocate any space. 

PARAMETERS:
        OIDC_T          Last component of the object id leading to 
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processsed.
        VB_T       *    Variable being processed.

RETURNS:  void
****************************************************************************/
/*ARGSUSED*/
void
  viewtable_test(OIDC_T          last_match,
                 int             compc,
                 OIDC_T         *compl,
                 SNMP_PKT_T     *pktp,
                 VB_T           *vbp)
{
VIEWLEAF_T *view, tempview, *newview;
INT_32_T value;
int ptret, cago = 0, create_row = 0;
ALENGTH_T nlen = 0;
VB_T *vbplist, *tvbp, *setvbp;
/* The status vb (if it exists) must be the first leaf in the vb chain and the
   vb for which the set is called for the set routine to work properly.
   To arrange this it must be the last leaf in the leaves list. */
static MIBLEAF_T *leaves[] = {&viewMask_leaf, &viewType_leaf,
                              &viewStorageType_leaf, &viewStatus_leaf, 0};
                            
/* we need an index that is less than 65536 */
if ((compc < 1) || (compl[0] > VIEW_MAXINDEXL)) {
    testproc_error(pktp, vbp, NO_CREATION);
    return;
    }

view = SNMP_View_Lookup((UINT_16_T)compl[0], compl + 1, compc - 1);

/* Note that the check for status is simpler than the general case as 
   this table has few objects and all but status have defvals.  This 
   means that several conditions can't occur.  So create and go is always
   acceptable and is almost the same as create and wait, active and not in
   service are also similiar. */

/* is the leaf writable, (if it's permanent it isn't writable) */
if (view != 0) {
    if (SNMP_View_Get_StorageType(view) == STO_PERM) {
        testproc_error(pktp, vbp, NOT_WRITABLE);      
        return;
        }

    MEMCPY(&tempview, view, sizeof(VIEWLEAF_T));
    }
else {
    SNMP_View_Set_Defaults(&tempview);
    create_row = 1;
    }
    
/* now get the list of var binds that may go into this structure */
vbplist = vb_extract_row_loose(pktp, vbp_to_index(pktp, vbp), leaves,
                               compc, compl);

/* Mark the incoming vbp as done, this allows us to return
   from anywhere in the routine without having to
   either have many calls to set the done bit or to
   bring everything into one spot.  This is ok because
   errors will set the correct bits anyway and this routine
   will not defer so when it returns the vbp will be done. */
testproc_good(pktp, vbp);

for (setvbp = vbplist, tvbp = vbplist; tvbp; tvbp = tvbp->vb_link) {
    switch (tvbp->vb_ml.ml_last_match) {
        case LM_viewMask:
            nlen = EBufferUsed(&tvbp->value_u.v_string);
            if (nlen > VIEW_MAXMASKLEN) {
                testproc_error(pktp, tvbp, WRONG_VALUE);
                return;
                }
            EBufferPreLoad(BFL_IS_STATIC, &tempview.mask,
                           EBufferStart(&tvbp->value_u.v_string), nlen);
            break;
        case LM_viewType:
            value = tvbp->value_u.v_number;
            if ((value != VIEW_INCLUDED) && (value != VIEW_EXCLUDED)) {
                testproc_error(pktp, tvbp, WRONG_VALUE);
                return;
                }
            SNMP_View_Set_Type(&tempview, value);
            break;
        case LM_viewStorageType:
            value = tvbp->value_u.v_number;
            if ((value < STO_OTHER) || (value > STO_NONVOL)) {
                testproc_error(pktp, tvbp, WRONG_VALUE);
                return;
                }
            SNMP_View_Set_StorageType(&tempview, (int)value);
            break;
        case LM_viewStatus:
            value = tvbp->value_u.v_number;
            switch (value) {
                case RS_ACTIVE:
                case RS_NIS:
                    if (create_row) {
                        testproc_error(pktp, tvbp, INCONSISTENT_VALUE);
                        return;
                        }
                    break;
                case RS_CAGO:
                    if (create_row == 0) {
                        testproc_error(pktp, tvbp, INCONSISTENT_VALUE);
                        return;
                        }
                    value = RS_ACTIVE;
                    cago = 1;
                    break;
                case RS_CAWAIT:
                    if (create_row == 0) {
                        testproc_error(pktp, tvbp, INCONSISTENT_VALUE);
                        return;
                        }
                    value = RS_NIS;
                    break;
                case RS_DESTROY:
                    break;
                default:
                    testproc_error(pktp, tvbp, WRONG_VALUE);
                    return;
                }
            SNMP_View_Set_Status(&tempview, (int)value);
            break;
        }
    }
    
/* Mark all vbs in the row except for setvbp as being tested, set and done.
   This means that setvbp is taking responsibility for all of the other
   vbs in the row.  Then mark setvbp as having it's test started  &
   done so we don't have to worry about it later, we can do this
   because the routine will complete and setvbp will be done */
for(tvbp = vbplist; tvbp; tvbp = tvbp->vb_link)
    if (tvbp != setvbp) {
        setproc_all_bits(pktp, tvbp);
        }
testproc_started(pktp, setvbp);
testproc_good(pktp, setvbp);


/* The values are ok  */
if (SNMP_View_Get_Status(&tempview) == RS_DESTROY) {
    if (view != 0) {
        ptret = SNMP_VIEW_DESTROY_TEST(pktp, view);
        if (ptret != NO_ERROR) {
            testproc_tproc_error(pktp, setvbp, ptret);
            }
        else {
            testproc_good(pktp, setvbp);
            setvbp->vb_priv      = (PTR_T)view;
            setvbp->vb_free_priv = viewtable_destroy_cleanup;
            }
        }
    else {
        /* We have a destroy request, but no view so we don't
           have to do anything during the set and, if necessary,
           the undo phases so we mark the setvbp as set and undone */
        setproc_all_bits(pktp, setvbp);
        }
    return;
    }
    
/* Because of how we do the backout stuff we always allocate a new
   viewleaf and set it up */

newview = SNMP_View_Create(compl + 1, compc - 1);
if (newview == 0) {
    testproc_error(pktp, setvbp, RESOURCE_UNAVAILABLE);
    return;
    }
newview->index   = tempview.index;
newview->type    = tempview.type;
newview->storage = tempview.storage;
newview->status  = tempview.status;

if (EBufferClone(&tempview.mask, &newview->mask) != 0) {
    SNMP_View_Delete(newview);
    testproc_error(pktp, setvbp, RESOURCE_UNAVAILABLE);
    return;
    }

/* if we are creating a new viewleaf we need to install it in the tree
   and run the create test, if we are updating an old viewleaf we just
   run the update test */
if (view == 0) {
    if (SNMP_View_Install((UINT_16_T)compl[0], newview) != 0) {
        SNMP_View_Delete(newview);
        testproc_error(pktp, setvbp, RESOURCE_UNAVAILABLE);
        return;
        }
    ptret = SNMP_VIEW_CREATE_TEST(pktp, newview);
    if (ptret != TPROC_GOOD) {
        if (SNMP_View_Deinstall((UINT_16_T)compl[0],
                                compl + 1, compc - 1) == 0)
            SNMP_View_Delete(newview);
        testproc_error(pktp, setvbp, RESOURCE_UNAVAILABLE);
        return;
        }
    }
else {
    ptret = SNMP_VIEW_UPDATE_TEST(pktp, view, newview);
    if (ptret != TPROC_GOOD) {
        SNMP_View_Delete(newview);
        testproc_error(pktp, setvbp, RESOURCE_UNAVAILABLE);
        return;
        }
    }

if (cago)
    SNMP_View_Set_Status(newview, RS_CAGO);

setvbp->vb_priv         = (PTR_T)newview;
setvbp->vb_free_priv    = viewtable_test_cleanup;
testproc_good(pktp, setvbp);
return;
}

/****************************************************************************
NAME:  viewtable_get

PURPOSE:  Find the appropriate entry in the acl table and attach information
          from it to the vbp using the getproc_got_* functions.
          If we can't find an entry indicate that by calling getproc_nosuchins.

PARAMETERS:
        OIDC_T          Last component of the object id leading to
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T *        Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processed.
        VB_T *          Variable being processed.

RETURNS: void

****************************************************************************/
void
  viewtable_get(OIDC_T           last_match,
                int              tcount,
                OIDC_T          *tlist,
                SNMP_PKT_T      *pktp,
                VB_T            *vbp)
{
VIEWLEAF_T *view;

/* There must be at least 1 subid,
   the first subid must be less than the max view,
   and the view must exist */
if ((tcount < 1) || (tlist[0] > VIEW_MAXINDEXL) ||
    ((view = SNMP_View_Lookup((UINT_16_T)tlist[0], tlist + 1, tcount - 1))
     == 0)) {
    getproc_nosuchins(pktp, vbp);
    return;
    }

/* we found a node dig out the information */
switch(last_match) {
    case LM_viewMask:
        getproc_got_string(pktp, vbp, SNMP_View_Get_MaskLen(view),
                           SNMP_View_Get_Mask(view), 0, VT_STRING);
        break;  
    case LM_viewType:
        getproc_got_int32(pktp, vbp, SNMP_View_Get_Type(view));
        break;
    case LM_viewStorageType:
        getproc_got_int32(pktp, vbp, SNMP_View_Get_StorageType(view));
        break;
    case LM_viewStatus:
        getproc_got_int32(pktp, vbp, SNMP_View_Get_Status(view));
        break;
    }

return;
}

/****************************************************************************
NAME:  viewtable_next

PURPOSE:  Find the appropriate entry in the acl table and attach information
          from it to the vbp using the getproc_got_* functions.
          If we can't find an entry indicate that by calling nextproc_no_next.

PARAMETERS:
        OIDC_T          Last component of the object id leading to
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T *        Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processed.
        VB_T *          Variable being processed.

RETURNS: void

****************************************************************************/
void
  viewtable_next(OIDC_T          last_match,
  int            tcount,
                OIDC_T          *tlist,
                 SNMP_PKT_T     *pktp,
                 VB_T           *vbp)
{
VIEWLEAF_T *view;
VIEWINDEX_T *vindex;
UINT_16_T indx = 0;
OIDC_T *rlist;

if (tcount > 0) {
    if (*tlist > VIEW_MAXINDEXL) {
        nextproc_no_next(pktp, vbp);
        return;
        }
    else
        indx = (UINT_16_T)(*tlist);
    }

for(vindex = viewroot; vindex != 0; vindex = vindex->next) {
    if (vindex->index < indx)
        continue;

    for (view = vindex->lexilist; view != 0; view = view->lexinext) {
        if (vindex->index == indx) {
            if (tcount - 1 < view->subtree.num_components) {
                if (oidorder(tlist + 1, view->subtree.component_list, 
                             tcount - 1) == 1)
                    continue;
                }
            else {
                if (oidorder(tlist + 1, view->subtree.component_list, 
                             view->subtree.num_components) != -1)
                    continue;
                }
            }
        rlist = (OIDC_T *)SNMP_memory_alloc((view->subtree.num_components + 1)
                                            * sizeof(OIDC_T));
        if (rlist == 0) {
            nextproc_error(pktp, vbp, GEN_ERR);
            return;
            }
        *rlist = (OIDC_T)vindex->index;
        MEMCPY(rlist + 1, view->subtree.component_list,
               view->subtree.num_components * sizeof(OIDC_T));
        nextproc_next_instance(pktp, vbp, view->subtree.num_components + 1, 
                               rlist);
        SNMP_memory_free(rlist);
        switch(last_match) {
            case LM_viewMask:
                getproc_got_string(pktp, vbp, SNMP_View_Get_MaskLen(view),
                                   SNMP_View_Get_Mask(view), 0, VT_STRING);
                break;  
            case LM_viewType:
                getproc_got_int32(pktp, vbp, SNMP_View_Get_Type(view));
                break;
            case LM_viewStorageType:
                getproc_got_int32(pktp, vbp, SNMP_View_Get_StorageType(view));
                break;
            case LM_viewStatus:
                getproc_got_int32(pktp, vbp, SNMP_View_Get_Status(view));
                break;
            }
        return;
        }
    }

nextproc_no_next(pktp, vbp);
return;
}

/****************************************************************************
NAME:  viewtable_set

PURPOSE:  Perform the set of the view, by the time we get here the
          viewleaf has been built and filled in by the test function.
          If the tree node points to the viewleaf then, this was a
          creation request and we already installed the leaf.  We do
          need to reset the vb_priv and vb_free_priv pointers so we
          won't free the data later.  Otherwise we need to install the
          new leaf and save a pointer to the old leaf in case an undo
          is required.

PARAMETERS:
        OIDC_T          Last component of the object id leading to
                        the leaf node in the MIB.  This is usually
                        the identifier for the particular attribute
                        in the table.
        int             Number of components in the unused part of the
                        object identifier
        OIDC_T     *    Unused part of the object identifier
        SNMP_PKT_T *    SNMP packet currently being processed.
        VB_T       *    Variable being processed.

RETURNS: void

****************************************************************************/
void
  viewtable_set(OIDC_T           last_match,
                int              tcount,
                OIDC_T          *tlist,
                SNMP_PKT_T      *pktp,
                VB_T            *vbp)
{
VIEWLEAF_T *oldview, *newview, tview;

oldview = SNMP_View_Lookup((UINT_16_T)tlist[0], tlist + 1, tcount - 1);
if ((last_match == LM_viewStatus) && (vbp->value_u.v_number == RS_DESTROY)) {
    if (oldview) {
        if (SNMP_View_Deinstall((UINT_16_T)tlist[0],
                                tlist + 1, tcount - 1)) {
            setproc_error(pktp, vbp, COMMIT_FAILED);
            return;
            }
        else {
            vbp->vb_priv = (PTR_T)oldview;
            vbp->vb_free_priv = viewtable_set_cleanup;
            undoproc_set(pktp, vbp, viewtable_undo);
            }
        }
    else /* no undo proc required */
        undoproc_good(pktp, vbp);
    setproc_good(pktp, vbp);
    return;
    }

if (oldview == 0) {
    setproc_error(pktp, vbp, COMMIT_FAILED);
    undoproc_good(pktp, vbp);
    return;
    }
newview = (VIEWLEAF_T *)vbp->vb_priv;

if (SNMP_View_Get_Status(newview) == RS_CAGO)
    SNMP_View_Set_Status(newview, RS_ACTIVE);

/* If the two views are equal then we are creating the view.
   We call the create hook and reset the priv info.
   Otherwise this is an update and we swap the info from the newview
   (from the vb_priv field) with that from the oldview.  We need to
   keep the old info around in case we need to undo the set.
   The newview storage space will be freed by vb_free_priv */
if (oldview == newview) {
    SNMP_VIEW_CREATE_HOOK(newview);
    vbp->vb_priv        = 0;
    }
else {
    SNMP_VIEW_UPDATE_HOOK(oldview, newview);
    /* copy the old stuff to a temp block */
    tview.type    = oldview->type;
    tview.storage = oldview->storage;
    tview.status  = oldview->status;
    MEMCPY(&tview.mask, &oldview->mask, sizeof(EBUFFER_T));
    /* copy the new stuff to the old block */
    oldview->type    = newview->type;
    oldview->storage = newview->storage;
    oldview->status  = newview->status;
    MEMCPY(&oldview->mask, &newview->mask, sizeof(EBUFFER_T));
    /* copy the stuff in the temp block (the old info) to the new block */
    newview->type    = tview.type;
    newview->storage = tview.storage;
    newview->status  = tview.status;
    MEMCPY(&newview->mask, &tview.mask, sizeof(EBUFFER_T));
    }

/* use the correct cleanup routine. */
vbp->vb_free_priv = viewtable_set_cleanup;

undoproc_set(pktp, vbp, viewtable_undo);
setproc_good(pktp, vbp);
return;
}

