// This is core API library.
//
//
//
//
//
//
//
//




// like ON_MESSAGE but no return value
/*#define ON_MESSAGE_VOID(message, memberFxn) \
{ message, 0, 0, 0, ::core::Sig_vv, \
(__PMSG)(__PMSGW)(void (__MSG_CALL ::user::interaction::*)())&memberFxn },*/
// IMPLEMENT_DYNAMIC(::aura::impact, ::user::interaction)

/////////////////////////////////////////////////////////////////////////////
// ::aura::impact

// BEGIN_MESSAGE_MAP(::aura::impact, ::user::interaction)
//{{__MSG_MAP(::aura::impact)
/*   ON_WM_PAINT()
ON_WM_MOUSEACTIVATE()

ON_WM_DESTROY()

// Standard commands for split pane
ON_COMMAND_EX(ID_WINDOW_SPLIT, OnSplitCmd)
ON_UPDATE_COMMAND_UI(ID_WINDOW_SPLIT, OnUpdateSplitCmd)

// Standard commands for next pane
ON_UPDATE_COMMAND_UI(ID_NEXT_PANE, OnUpdateNextPaneMenu)
ON_COMMAND_EX(ID_NEXT_PANE, OnNextPaneCmd)
ON_UPDATE_COMMAND_UI(ID_PREV_PANE, OnUpdateNextPaneMenu)
ON_COMMAND_EX(ID_PREV_PANE, OnNextPaneCmd)
//}}__MSG_MAP
// special command for Initial Update
ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)*/
// END_MESSAGE_MAP()
#include "framework.h" // from "base/user/user.h"
#include "base/user/user.h"


#define WM_INITIALUPDATE    0x0364  // (params unused) - sent to children


namespace aura
{


   impact::impact()
   {

      m_pdocument = NULL;

      m_ulFlags |= element::flag_auto_delete;

   }


   impact::~impact()
   {
   }

   void impact::install_message_handling(::message::dispatch * pinterface)
   {
      ::database::user::interaction::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_VIEW, pinterface, this, &impact::_001OnView);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &impact::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &impact::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &impact::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &impact::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &impact::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_INITIALUPDATE, pinterface, this, &impact::_001OnInitialUpdate);

      //      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN    , pinterface, this, &impact::_001OnRButtonDown);
      IGUI_WIN_MSG_LINK(WM_MBUTTONDOWN, pinterface, this, &impact::_001OnMButtonDown);
      IGUI_WIN_MSG_LINK(WM_RBUTTONDOWN, pinterface, this, &impact::_001OnRButtonDown);


      IGUI_WIN_MSG_LINK(WM_MOUSEACTIVATE, pinterface, this, &impact::_001OnMouseActivate);
      //      IGUI_WIN_MSG_LINK(WM_DESTROY        , pinterface, this, &impact::_001OnDestroy);
      //    IGUI_WIN_MSG_LINK(WM_CREATE        , pinterface, this, &impact::_001OnCreate);

      // Standard commands for split pane
      //  //      connect_command(ID_WINDOW_SPLIT , &impact::_001OnSplitCmd);
      //    connect_update_cmd_ui(ID_WINDOW_SPLIT ,  &impact::_001OnUpdateSplitCmd);

      // Standard commands for next pane
      //  connect_update_cmd_ui(ID_NEXT_PANE    , &impact::_001OnUpdateNextPaneMenu);
      //connect_command(ID_NEXT_PANE   , &impact::_001OnNextPaneCmd);
      //      connect_update_cmd_ui(ID_PREV_PANE    , &impact::_001OnUpdateNextPaneMenu);
      //    connect_command(ID_PREV_PANE    , &impact::_001OnNextPaneCmd);

      //}}__MSG_MAP
      // special command for Initial Update


      /* ON_WM_MOUSEACTIVATE()

      // Standard commands for split pane
      ON_COMMAND_EX(ID_WINDOW_SPLIT, &impact::OnSplitCmd)
      ON_UPDATE_COMMAND_UI(ID_WINDOW_SPLIT, &impact::OnUpdateSplitCmd)

      // Standard commands for next pane
      ON_UPDATE_COMMAND_UI(ID_NEXT_PANE, &impact::OnUpdateNextPaneMenu)
      ON_COMMAND_EX(ID_NEXT_PANE, &impact::OnNextPaneCmd)
      ON_UPDATE_COMMAND_UI(ID_PREV_PANE, &impact::OnUpdateNextPaneMenu)
      ON_COMMAND_EX(ID_PREV_PANE, &impact::OnNextPaneCmd)
      //}}__MSG_MAP
      // special command for Initial Update
      ON_MESSAGE_VOID(WM_INITIALUPDATE, impact::OnInitialUpdate) */

   }


   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact second phase construction - bind to ::aura::document

   bool impact::pre_create_window(::user::create_struct & cs)
   {

      ASSERT(cs.style & WS_CHILD);

      if (cs.style & WS_BORDER)
      {
#ifdef WINDOWSEX
         cs.dwExStyle |= WS_EX_CLIENTEDGE;
#endif
         cs.style &= ~WS_BORDER;
      }

      return TRUE;

   }


   void impact::_001OnCreate(signal_details * pobj)
   {
      SCAST_PTR(::message::create, pcreate, pobj);



      if (pcreate->previous())
         return;

      // if ok, wire in the current ::aura::document
      ASSERT(::aura::impact::get_document() == NULL);
      sp(::create) pContext = (::create *) pcreate->m_lpcreatestruct->lpCreateParams;

      // A ::aura::impact should be created in a given context!
      if (pContext != NULL && pContext->m_user->m_pCurrentDoc != NULL)
      {
         pContext->m_user->m_pCurrentDoc->add_view(this);
         ASSERT(::aura::impact::get_document() != NULL);
      }
      else
      {
         TRACE(::aura::trace::category_AppMsg, 0, "Warning: Creating a pane with no ::aura::document.\n");
      }

      pcreate->set_lresult(0);
   }

   void impact::_001OnDestroy(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      sp(::user::frame_window) pFrame = GetParentFrame();
      if (pFrame != NULL && pFrame->GetActiveView() == this)
         pFrame->SetActiveView(NULL);    // deactivate during death

      if(m_pdocument != NULL)
         m_pdocument->remove_view(this);

      //   ::user::interaction::OnDestroy();
   }

   // self destruction
   void impact::PostNcDestroy()
   {
      ::user::interaction::PostNcDestroy();
/*      if (is_set_ca_flag(element::flag_auto_delete))
      {
         // default for views is to allocate them on the heap
         //  the default post-cleanup is to 'delete this'.
         //  never explicitly call 'delete' on a ::aura::impact
         delete_this();
      }*/

      //release();

   }

   void impact::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
   {
      /* trans   ENSURE_ARG(lpClientRect != NULL);
      if (nAdjustType != 0)
      {
      // allow for special client-edge style
      ::AdjustWindowRectEx(lpClientRect, 0, FALSE, GetExStyle());

      // default behavior for in-place editing handles scrollbars
      uint32_t dwStyle = GetStyle();
      if (dwStyle & WS_VSCROLL)
      {
      int32_t nAdjust = afxData.cxVScroll;
      if (dwStyle & WS_BORDER)
      nAdjust -= CX_BORDER;
      lpClientRect->right += nAdjust;
      }
      if (dwStyle & WS_HSCROLL)
      {
      int32_t nAdjust = afxData.cyHScroll;
      if (dwStyle & WS_BORDER)
      nAdjust -= CY_BORDER;
      lpClientRect->bottom += nAdjust;
      }
      return;
      }*/

      // call default to place borders outside of client rect
      ::user::interaction::CalcWindowRect(lpClientRect, nAdjustType);
   }

   /////////////////////////////////////////////////////////////////////////////
   // Command routing

   bool impact::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)

   {

      // first pump through pane
      if (::user::interaction::_001OnCmdMsg(pcmdmsg))
         return TRUE;
      sp(::user::interaction) pwndex;
      if (base_class < ::aura::impact > ::bases(GetParent()))
      {
         pwndex = GetParent();
         if (pwndex != NULL)
         {
            if (pwndex->_001OnCmdMsg(pcmdmsg))
               return TRUE;
         }
      }

      // then pump through document
      if (::aura::impact::get_document() != NULL)
      {
         if (::aura::impact::get_document()->_001OnCmdMsg(pcmdmsg))
            return TRUE;
         sp(::aura::impact) pview = ::aura::impact::get_document()->get_view(0);
         ASSERT_VALID(pview);
         if (pview != this && pview != GetParent())
         {
            pwndex = (pview);
            if (pwndex != NULL)
            {
               if (pwndex->::user::interaction::_001OnCmdMsg(pcmdmsg))
                  return TRUE;
            }
         }

      }


      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact drawing support

   void impact::OnPaint()
   {
      // standard paint routine
      // trans   CPaintDC spgraphics(this);
      //   trans OnPrepareDC(&spgraphics);
      //   trans OnDraw(&spgraphics);
   }

   ::aura::document * impact::get_document(::user::interaction * pui)
   {
      sp(::aura::impact) pview = pui;
      if (pview != NULL)
         return NULL;
      return pview->get_document();
   }

   void impact::_001OnInitialUpdate(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      on_update(NULL, 0, NULL);        // initial update
   }

   void impact::on_update(::aura::impact * pSender, LPARAM lHint, object* pHint)
   {
      if (pHint != NULL)
      {
         if (base_class < ::user::view_update_hint > ::bases(pHint))
         {
            ::user::view_update_hint * puh = dynamic_cast < ::user::view_update_hint * > (pHint);
            OnViewUpdateHint(pSender, lHint, puh);
         }
      }
      if (lHint >= hint_begin && lHint < hint_end)
      {
         on_simple_view_update_hint(pSender, (e_hint)lHint, pHint);
      }
      ASSERT(pSender != this);
      UNUSED(pSender);     // unused in release builds

      // invalidate the entire pane, erase background too
      //Invalidate(TRUE);
   }

   void impact::on_simple_view_update_hint(sp(::aura::impact) pviewSender, e_hint ehint, object * phint)
   {

      switch (ehint)
      {
      case hint_set_edit_file:
      {
                                post_simple_command(simple_command_set_edit_file, (LPARAM)phint);
      }
         break;
      default:
         break;
      };

   }

   void impact::OnViewUpdateHint(sp(::aura::impact) pSender, LPARAM lHint, ::user::view_update_hint * pHint)
   {
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(pHint);
   }


   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact selection support

   bool impact::IsSelected(const object* pDocItem) const
   {
      ASSERT_VALID(pDocItem);
      UNUSED(pDocItem);    // unused in release builds

      return FALSE;   // not implemented, so not selected
   }

   void impact::OnActivateView(bool bActivate, sp(::aura::impact) pActivateView, sp(::aura::impact))
   {
      UNUSED(pActivateView);   // unused in release builds

      if (bActivate)
      {
         ASSERT(pActivateView == this);

         // take the focus if this frame/::aura::impact/pane is now active
         // trans      if (IsTopParentActive())
         //trans   SetFocus();
      }
   }

   void impact::OnActivateFrame(UINT /*nState*/, sp(::user::frame_window) /*pFrameWnd*/)
   {
   }

   /* trans
   int32_t impact::OnMouseActivate(::window_sp pDesktopWnd, UINT nHitTest, UINT message)
   {
   int32_t nResult = ::user::interaction::OnMouseActivate(pDesktopWnd, nHitTest, message);
   if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
   return nResult;   // frame does not want to activate

   sp(::user::frame_window) pParentFrame = GetParentFrame();
   if (pParentFrame != NULL)
   {
   // eat it if this will cause activation
   ASSERT(pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame));

   // either re-activate the current ::aura::impact, or set this ::aura::impact to be active
   sp(::aura::impact) pview = pParentFrame->GetActiveView();
   oswindow oswindow_Focus = ::GetFocus();
   if (pview == this &&
   get_handle() != oswindow_Focus && !::IsChild(get_handle(), oswindow_Focus))
   {
   // re-activate this ::aura::impact
   OnActivateView(TRUE, this, this);
   }
   else
   {
   // activate this ::aura::impact
   pParentFrame->SetActiveView(this);
   }
   }
   return nResult;
   }
   */

   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact scrolling support

   bool impact::OnScroll(UINT /*nScrollCode*/, UINT /*nPos*/, bool /*bDoScroll*/)
   {
      return FALSE;
   }

   bool impact::OnScrollBy(size /*sizeScroll*/, bool /*bDoScroll*/)
   {
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact drag/drop support

   ///*DROPEFFECT impact::OnDragScroll(uint32_t /*dwKeyState*/, point /*point*/)
   //{
   //#ifndef ___NO_OLE_SUPPORT
   // return DROPEFFECT_SCROLL; // this means do the default
   //#else
   // return 0;
   //#endif
   //}

   //DROPEFFECT impact::OnDragEnter(COleDataObject* /*pDataObject*/,
   // uint32_t /*dwKeyState*/, point /*point*/)
   //{
   // return 0;   // DROPEFFECT_NONE
   //}

   //DROPEFFECT impact::OnDragOver(COleDataObject* /*pDataObject*/,
   //uint32_t /*dwKeyState*/, point /*point*/)
   //{
   //return 0;   // DROPEFFECT_NONE
   //}

   //bool impact::OnDrop(COleDataObject* /*pDataObject*/,
   //DROPEFFECT /*dropEffect*/, point /*point*/)
   //{
   // return FALSE;
   //}

   //DROPEFFECT impact::OnDropEx(COleDataObject* /*pDataObject*/,
   // DROPEFFECT /*dropEffect*/, DROPEFFECT /*dropEffectList*/, point /*point*/)
   //{
   // return (DROPEFFECT)-1;  // not implemented
   //}

   //void impact::OnDragLeave()
   //{
   //}

   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact splitting commands


/*   CScrollBar* impact::GetScrollBarCtrl(int32_t nBar) const
   {

      ///ASSERT(nBar == SB_HORZ || nBar == SB_VERT);
      // trans   if (GetStyle() & ((nBar == SB_HORZ) ? WS_HSCROLL : WS_VSCROLL))
      //   {
      // it has a regular windows style scrollbar (no control)
      //      return NULL;
      //   }


      return NULL;
   }*/


   void impact::OnUpdateSplitCmd(cmd_ui* pCmdUI)
   {
      UNREFERENCED_PARAMETER(pCmdUI);
      /*ENSURE_ARG(pCmdUI != NULL);
      CSplitterWnd* pSplitter = GetParentSplitter(this, FALSE);
      pCmdUI->Enable(pSplitter != NULL && !pSplitter->IsTracking());*/
   }

   bool impact::OnSplitCmd(UINT)
   {
      /*   CSplitterWnd* pSplitter = GetParentSplitter(this, FALSE);
      if (pSplitter == NULL)
      return FALSE;

      ASSERT(!pSplitter->IsTracking());
      pSplitter->DoKeyboardSplit();*/
      return TRUE;    // attempted at least
   }

   void impact::OnUpdateNextPaneMenu(cmd_ui* pCmdUI)
   {
      UNREFERENCED_PARAMETER(pCmdUI);
      /*ASSERT(pCmdUI->m_nID == ID_NEXT_PANE ||
      pCmdUI->m_nID == ID_PREV_PANE);
      CSplitterWnd* pSplitter = GetParentSplitter(this, FALSE);
      pCmdUI->Enable(pSplitter != NULL &&
      pSplitter->CanActivateNext(pCmdUI->m_nID == ID_PREV_PANE));*/
   }

   bool impact::OnNextPaneCmd(UINT nID)
   {
      UNREFERENCED_PARAMETER(nID);
      /*   CSplitterWnd* pSplitter = GetParentSplitter(this, FALSE);
      if (pSplitter == NULL)
      return FALSE;

      ASSERT(nID == ID_NEXT_PANE || nID == ID_PREV_PANE);
      pSplitter->ActivateNext(nID == ID_PREV_PANE);*/
      return TRUE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Printing support virtual functions (others in viewpr.cpp)

   void impact::OnPrepareDC(::draw2d::graphics * pgraphics, CPrintInfo* pInfo)
   {
      UNREFERENCED_PARAMETER(pInfo);
      ASSERT_VALID(pgraphics);
      UNUSED(pgraphics); // unused in release builds

      // Default to one page printing if doc length not known
   }




   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact diagnostics


   void impact::dump(dump_context & dumpcontext) const
   {
      ::user::interaction::dump(dumpcontext);

      if (((impact *) this)->::aura::impact::get_document() != NULL)
         dumpcontext << "with ::aura::document: ";
      else
         dumpcontext << "with no ::aura::document\n";
   }

   void impact::assert_valid() const
   {
      ::user::interaction::assert_valid();
   }




   void impact::_001OnView(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj)
      if (pbase->m_wparam == 0)
      {
         ::aura::document::update * pupdate = (::aura::document::update *) pbase->m_lparam.m_lparam;
         on_update(pupdate->m_pSender, pupdate->m_lHint, pupdate->m_pHint);
      }
   }


   ::user::interaction * impact::create_view(type * pinfo, ::aura::document * pdoc,const RECT & rect, ::user::interaction * pwndParent, id id, ::user::interaction * pviewLast)
   {

      sp(type) info(pinfo);

      sp(::create) cacc(allocer());

      stacker < ::aura::create_context > cc(cacc->m_user);

      cc->m_typeinfoNewView = info;

      cc->m_pLastView = pviewLast;

      if (pdoc == NULL)
      {
         cc->m_pCurrentDoc = get_document();
      }
      else
      {
         cc->m_pCurrentDoc = pdoc;
      }

      if (pwndParent == NULL)
      {
         pwndParent = this;
      }

      if (id.is_empty())
      {

         id = (const ::id &) cc->m_typeinfoNewView->name();

      }

      return s_create_view(cacc, rect, pwndParent, id);

   }


   ::user::interaction * impact::s_create_view(type * pinfo, ::aura::document * pdoc,const RECT & rect, ::user::interaction * pwndParent,id id, ::user::interaction * pviewLast)
   {

      sp(type) info(pinfo);

      sp(::create) cacc(pdoc->allocer());

      stacker < ::aura::create_context > cc(cacc->m_user);

      cc->m_typeinfoNewView = info;

      cc->m_pLastView = pviewLast;

      cc->m_pCurrentDoc = pdoc;

      return s_create_view(cacc, rect, pwndParent, id);

   }

   ::user::interaction * impact::s_create_view(::create * pContext,const RECT & rect, ::user::interaction * pwndParent,id id)
   {

      // trans   ASSERT(pwndParent->get_handle() != NULL);
      // trans   ASSERT(::IsWindow(pwndParent->get_handle()));

      ASSERT(pContext != NULL);

      ASSERT(pContext->m_user->m_typeinfoNewView || pContext->m_user->m_puiNew != NULL);


      ::aura::application * papp = pwndParent->get_app();

      sp(::user::interaction) pui;

      if (pContext->m_user->m_puiNew != NULL)
      {

         pui = pContext->m_user->m_puiNew;

      }
      else
      {

         // Note: can be a ::user::interaction with PostNcDestroy self cleanup
         pui = App(papp).alloc(pContext->m_user->m_typeinfoNewView);
         if (pui == NULL)
         {
            //         TRACE1("Warning: Dynamic create of ::aura::impact type %hs failed.\n", pContext->m_typeinfoNewView.name());
            return NULL;
         }

      }

      ASSERT_KINDOF(::user::interaction, pui);

      
      // views are always created with a border!
      if (!pui->create_window(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, pwndParent, id, pContext))
      {

         //TRACE0("Warning: could not create ::aura::impact for frame.\n");
         return NULL;        // can't continue without a ::aura::impact

      }


      sp(::aura::impact) pview = pui;
      if (pview != NULL)
      {
         pview->_001OnInitialUpdate(NULL);
      }
      /*   if (afxData.bWin4 && (pview->GetExStyle() & WS_EX_CLIENTEDGE))
      {
      // remove the 3d style from the frame, since the ::aura::impact is
      //  providing it.
      // make sure to recalc the non-client area
      ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
      }*/


      if (pui != NULL)
      {

         if (pui->GetParent() != NULL)
         {



            if (pui->GetParent()->is_place_holder())
            {

               pui->GetParent()->place(pui);

            }

         }

      }

      return pui;

   }


   void impact::_001OnLButtonDown(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //   SCAST_PTR(::message::mouse, pmouse, pobj);

      if (GetParentFrame() != NULL)
      {

         GetParentFrame()->SetActiveView(this);

      }

   }

   void impact::_001OnLButtonUp(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //SCAST_PTR(::message::mouse, pmouse, pobj);
   }

   void impact::_001OnMouseMove(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //   SCAST_PTR(::message::mouse, pmouse, pobj);
   }


   ::aura::document * impact::get_document() const
   {

      ASSERT(this != NULL);

      return ((::aura::impact *) this)->m_pdocument;

   }


   void impact::collaborate(::job * pjob)
   {
      {
         ::user::job * puserjob = (dynamic_cast < ::user::job * > (pjob));
         if (puserjob != NULL)
         {
            puserjob->m_pview = this;
         }
      }
   }



   int32_t impact::get_total_page_count(::job * pjob)
   {
      UNREFERENCED_PARAMETER(pjob);
      return 1;
   }


   ::user::interaction::e_type impact::get_window_type()
   {
      return type_view;
   }


   //void impact::on_draw_view_nc(::draw2d::graphics * pdc)
   //{

   //   UNREFERENCED_PARAMETER(pdc);

   //}

   //void impact::on_draw_view(::draw2d::graphics * pdc, spa(::data::data) spadata)
   //{

   //   UNREFERENCED_PARAMETER(pdc);
   //   UNREFERENCED_PARAMETER(spadata);

   //}

   //void impact::defer_draw_view(::draw2d::graphics * pdc)
   //{

   //   if (get_document() == NULL)
   //      return;

   //   spa(::data::data) spadata = get_document()->m_spadata;

   //   //spadata.add(get_document()->m_spdata);

   //   sync_object_ptra sync;

   //   for (index i = 0; i < spadata.get_count(); i++)
   //   {

   //      sync.add(spadata[i].data_mutex());

   //   }

   //   retry_multi_lock sl(sync, millis(1), millis(1));

   //   try
   //   {
   //      on_draw_view(pdc, spadata);
   //   }
   //   catch (...)
   //   {
   //   }

   //}

   //void impact::_001OnDraw(::draw2d::graphics * pdc)
   //{

   //   on_draw_view_nc(pdc);

   //   int32_t iTry = 0;

   //   bool bOk;

   //retry:

   //   bOk = true;

   //   try
   //   {
   //      defer_draw_view(pdc);
   //   }
   //   catch (...)
   //   {
   //      bOk = false;
   //   }

   //   iTry++;
   //   if (!bOk && iTry < 9)
   //      goto retry;

   //}


   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact second phase construction - bind to document

   //const CHAR _vfxWndFrameOrView[] = __WNDFRAMEORVIEW;




   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact drawing support


   /*void impact::_001OnInitialUpdate(signal_details * pobj)
   {
   on_update(NULL, 0, NULL);        // initial update
   }*/

   /*   void impact::on_update(::aura::impact * pSender, LPARAM lHint, object * pHint)
   {
   ::aura::impact::on_update(pSender, lHint, pHint);
   }
   */
   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact selection support

   /*   bool impact::IsSelected(const object* pDocItem) const
   {
   ASSERT_VALID(pDocItem);
   //      UNUSED(pDocItem);    // unused in release builds

   return FALSE;   // not implemented, so not selected
   }*/


   void impact::_001OnMouseActivate(signal_details * pobj)
   {
      SCAST_PTR(::message::mouse_activate, pmouseactivate, pobj);
      pobj->previous();
      //int32_t nResult = pmouseactivate->get_lresult();

      if (pmouseactivate->get_message() == MA_NOACTIVATE
         || pmouseactivate->get_message() == MA_NOACTIVATEANDEAT)
      {
         pmouseactivate->set_lresult(pmouseactivate->get_message()); // frame does not want to activate
         pmouseactivate->m_bRet = true;
      }

      sp(::user::frame_window) pParentFrame = (GetParentFrame());
      if (pParentFrame != NULL)
      {
         // eat it if this will cause activation
         ASSERT(pParentFrame == pmouseactivate->GetDesktopWindow()
            || pmouseactivate->GetDesktopWindow()->IsChild(pParentFrame));

         // either re-activate the current ::aura::impact, or set this ::aura::impact to be active
         sp(::aura::impact) pview = pParentFrame->GetActiveView();
         sp(::user::interaction) oswindow_Focus = System.get_focus_guie();
         if (pview == this &&
            this != oswindow_Focus && !IsChild(oswindow_Focus))
         {
            // re-activate this ::aura::impact
            OnActivateView(TRUE, this, this);
         }
         else
         {
            // activate this ::aura::impact
            pParentFrame->SetActiveView(this);
         }
      }

      pmouseactivate->set_lresult(pmouseactivate->get_message());

      pmouseactivate->m_bRet = true;

   }

   void impact::on_select()
   {
      sp(::user::frame_window) pParentFrame = (GetParentFrame());
      if (pParentFrame != NULL)
      {
         // eat it if this will cause activation
         /*      ASSERT(pParentFrame == pmouseactivate->GetDesktopWindow()
         || pmouseactivate->GetDesktopWindow()->IsChild(pParentFrame));*/

         // either re-activate the current ::aura::impact, or set this ::aura::impact to be active
         sp(::aura::impact) pview = pParentFrame->GetActiveView();
         sp(::user::interaction) oswindow_Focus = System.get_focus_guie();
         if (pview == this &&
            this != oswindow_Focus && !IsChild(oswindow_Focus))
         {
            // re-activate this ::aura::impact
            OnActivateView(TRUE, this, this);
         }
         else
         {
            // activate this ::aura::impact
            pParentFrame->SetActiveView(this);
         }
      }
   }


//#ifndef ANDROID
//
//   /////////////////////////////////////////////////////////////////////////////
//   // ::aura::impact drag/drop support
//
//   DROPEFFECT impact::OnDragScroll(uint32_t /*dwKeyState*/, point /*point*/)
//   {
//#if !defined(___NO_OLE_SUPPORT) && !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS) && !defined(ANDROID) && !defined(SOLARIS)
//      return DROPEFFECT_SCROLL; // this means do the default
//#else
//      return 0;
//#endif
//   }
//
//   DROPEFFECT impact::OnDragEnter(COleDataObject* /*pDataObject*/,
//      uint32_t /*dwKeyState*/, point /*point*/)
//   {
//      return 0;   // DROPEFFECT_NONE
//   }
//
//   DROPEFFECT impact::OnDragOver(COleDataObject* /*pDataObject*/,
//      uint32_t /*dwKeyState*/, point /*point*/)
//   {
//      return 0;   // DROPEFFECT_NONE
//   }
//
//   bool impact::OnDrop(COleDataObject* /*pDataObject*/,
//      DROPEFFECT /*dropEffect*/, point /*point*/)
//   {
//      return FALSE;
//   }
//
//   DROPEFFECT impact::OnDropEx(COleDataObject* /*pDataObject*/,
//      DROPEFFECT /*dropEffect*/, DROPEFFECT /*dropEffectList*/, point /*point*/)
//   {
//      return (DROPEFFECT)-1;  // not implemented
//   }
//
//   void impact::OnDragLeave()
//   {
//   }
//
//
//#endif // ANDROID




   void impact::_001OnUpdateSplitCmd(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void impact::_001OnSplitCmd(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void impact::_001OnUpdateNextPaneMenu(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void impact::_001OnNextPaneCmd(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   // // IMPLEMENT_DYNAMIC for ::aura::impact is in wincore.cpp for .OBJ granularity reasons

   //// IMPLEMENT_DYNAMIC(CSplitterWnd, ::user::interaction)   // for swap tuning
   //// IMPLEMENT_DYNAMIC(CCtrlView, ::aura::impact)

   /////////////////////////////////////////////////////////////////////////////


   /////////////////////////////////////////////////////////////////////////////
   // ::aura::impact's OnPrintPreview.  Here to force linkage

   void impact::_001OnFilePrintPreview(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   void impact::_001OnFilePrint(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }



   /*
   sp(::user::interaction) impact::CreateView(sp(::create) pContext, UINT nID)
   {
   ASSERT(IsWindow());
   ASSERT(pContext != NULL);
   ASSERT(pContext->m_typeinfoNewView != NULL);

   // Note: can be a ::user::interaction with PostNcDestroy self cleanup
   sp(::user::interaction) pview =  (System.alloc(pContext->m_typeinfoNewView));
   if (pview == NULL)
   {
   TRACE1("Warning: Dynamic create of ::aura::impact type %hs failed.\n",
   pContext->m_typeinfoNewView.name());
   return NULL;
   }
   ASSERT_KINDOF(::user::interaction, pview);

   // views are always created with a border!
   if (!pview->create(NULL, NULL, __WS_DEFAULT_VIEW,
   rect(0,0,0,0), this, nID, (sp(::create)) pContext))
   {
   TRACE0("Warning: could not create ::aura::impact for frame.\n");
   return NULL;        // can't continue without a ::aura::impact
   }

   sp(::aura::impact) pview =  (pview);
   pview->_001OnInitialUpdate(NULL);
   if (afxData.bWin4 && (pview->GetExStyle() & WS_EX_CLIENTEDGE))
   {
   // remove the 3d style from the frame, since the ::aura::impact is
   //  providing it.
   // make sure to recalc the non-client area
   ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
   }*/
   /*   return pview;
   }*/


   /*sp(::user::interaction) impact::CreateView(sp(::create) pContext, UINT nID, ::user::interaction  * pwndParent)
   {
   ASSERT(pwndParent->IsWindow());
   ASSERT(pContext != NULL);
   ASSERT(pContext->m_typeinfoNewView != NULL);

   // Note: can be a interaction_impl with PostNcDestroy self cleanup
   ::window_sp pview = (pwndParent->System.alloc(pContext->m_typeinfoNewView));
   if (pview == NULL)
   {
   TRACE1("Warning: Dynamic create of ::aura::impact type %hs failed.\n",
   pContext->m_typeinfoNewView.name());
   return NULL;
   }
   ASSERT_KINDOF(interaction_impl, pview);

   // views are always created with a border!
   if (!pview->create(NULL, NULL, __WS_DEFAULT_VIEW,
   rect(0,0,0,0), pwndParent, nID, (sp(::create)) pContext))
   {
   TRACE0("Warning: could not create ::aura::impact for frame.\n");
   return NULL;        // can't continue without a ::aura::impact
   }

   ( (pview))->_001OnInitialUpdate(NULL);
   if (afxData.bWin4 && (pview->GetExStyle() & WS_EX_CLIENTEDGE))
   {
   // remove the 3d style from the frame, since the ::aura::impact is
   //  providing it.
   // make sure to recalc the non-client area
   ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
   }*/
   //   return pview;
   //}


   void impact::_001OnRButtonDown(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //SCAST_PTR(::message::mouse, pmouse, pobj);

      GetParentFrame()->SetActiveView((this));
   }

   void impact::_001OnMButtonDown(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      SCAST_PTR(::message::mouse, pmouse, pobj);

      GetParentFrame()->SetActiveView((this));
   }


   bool impact::_001HasCommandHandler(id id)
   {

      if (command_target_interface::_001HasCommandHandler(id))
         return true;

      if (get_document() != NULL)
      {

         if (get_document()->_001HasCommandHandler(id))
            return true;

      }



      return false;

   }


   void impact::walk_pre_translate_tree(signal_details * pobj,sp(::user::interaction) puiStop)
   {


      try
      {

         GetParentFrame()->pre_translate_message(pobj);

      }
      catch(...)
      {

      }

      if(pobj->m_bRet)
         return;

      pre_translate_message(pobj);

   }



} // namespace aura












