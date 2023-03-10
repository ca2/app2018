#include "framework.h"


namespace user
{


   impact::impact()
   {

      m_puiViewNotify = NULL;

      m_pdocument = NULL;

      m_ulFlags |= object::flag_auto_delete;

   }


   impact::~impact()
   {

   }


   void impact::install_message_routing(::message::sender * pinterface)
   {

      ::user::box::install_message_routing(pinterface);

      IGUI_MSG_LINK(WM_VIEW, pinterface, this, &impact::_001OnView);
      IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &impact::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &impact::_001OnLButtonUp);
      IGUI_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &impact::_001OnMouseMove);
      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &impact::_001OnCreate);
      IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &impact::_001OnDestroy);
      //      IGUI_MSG_LINK(WM_LBUTTONDOWN    , pinterface, this, &impact::_001OnRButtonDown);
      IGUI_MSG_LINK(WM_MBUTTONDOWN, pinterface, this, &impact::_001OnMButtonDown);
      IGUI_MSG_LINK(WM_RBUTTONDOWN, pinterface, this, &impact::_001OnRButtonDown);
      IGUI_MSG_LINK(WM_INITIALUPDATE,pinterface,this,&impact::_001OnInitialUpdateMessage);


      IGUI_MSG_LINK(WM_MOUSEACTIVATE, pinterface, this, &impact::_001OnMouseActivate);
      //      IGUI_MSG_LINK(WM_DESTROY        , pinterface, this, &impact::_001OnDestroy);
      //    IGUI_MSG_LINK(WM_CREATE        , pinterface, this, &impact::_001OnCreate);

      // Standard commands for split pane
      //  //      connect_command(ID_WINDOW_SPLIT , &impact::_001OnSplitCmd);
      //    connect_command_probe(ID_WINDOW_SPLIT ,  &impact::_001OnUpdateSplitCmd);

      // Standard commands for next pane
      //  connect_command_probe(ID_NEXT_PANE    , &impact::_001OnUpdateNextPaneMenu);
      //connect_command(ID_NEXT_PANE   , &impact::_001OnNextPaneCmd);
      //      connect_command_probe(ID_PREV_PANE    , &impact::_001OnUpdateNextPaneMenu);
      //    connect_command(ID_PREV_PANE    , &impact::_001OnNextPaneCmd);

      //}}__MSG_MAP
      // special command for Initial Update


      /* ON_WM_MOUSEACTIVATE()

      // Standard commands for split pane
      ON_COMMAND_EX(ID_WINDOW_SPLIT, &impact::OnSplitCmd)
      ON_UPDATE_::user::command(ID_WINDOW_SPLIT, &impact::OnUpdateSplitCmd)

      // Standard commands for next pane
      ON_UPDATE_::user::command(ID_NEXT_PANE, &impact::OnUpdateNextPaneMenu)
      ON_COMMAND_EX(ID_NEXT_PANE, &impact::OnNextPaneCmd)
      ON_UPDATE_::user::command(ID_PREV_PANE, &impact::OnUpdateNextPaneMenu)
      ON_COMMAND_EX(ID_PREV_PANE, &impact::OnNextPaneCmd)
      //}}__MSG_MAP
      // special command for Initial Update
      ON_MESSAGE_VOID(WM_INITIALUPDATE, impact::OnInitialUpdate) */

   }


   ////////////////////////////////////////////////////////////////////////////
   //
   // ::user::impact second phase construction - bind to ::user::document

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


   void impact::_001OnCreate(::message::message * pmessage)
   {

      SCAST_PTR(::message::create, pmessagecreate, pmessage);

      // if ok, wire in the current ::user::document
      ASSERT(::user::impact::get_document() == NULL);

      sp(class ::create) pcreate((::create *) pmessagecreate->m_lpcreatestruct->lpCreateParams);

      // A ::user::impact should be created in a given context!
      if (pcreate != NULL && pcreate->m_pusercreate->m_pdocumentCurrent != NULL)
      {

         sp(::user::document) pdoc = pcreate->m_pusercreate->m_pdocumentCurrent;

         pdoc->add_view(this);

         ASSERT(::user::impact::get_document() != NULL);

      }
      else
      {
         TRACE(::aura::trace::category_AppMsg, 0, "Warning: Creating a pane with no ::user::document.\n");
      }

      ::user::frame_window * pframe = GetTypedParent < ::user::frame_window >();

      if(pframe != NULL && pframe->m_pviewMain == NULL)
      {

         pframe->m_pviewMain = this;

      }

      if(pmessagecreate->previous())
         return;

      {

         ::user::control_event ev;

         ev.m_puie      = this;

         ev.m_eevent    = ::user::event_on_create_view;

         ev.m_pmessage  = pmessage;

         on_control_event(&ev);

      }

      pmessagecreate->set_lresult(0);

   }


   void impact::_001OnDestroy(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      sp(::user::frame_window) pFrame = GetParentFrame();

      if (pFrame != NULL && pFrame->GetActiveView() == this)
      {

         pFrame->SetActiveView(NULL, false);    // deactivate during death

      }

      GetTopLevelFrame()->m_viewptraCommandHandlers.remove(this);
      if (m_pdocument != NULL)
      {

         m_pdocument->remove_view(this);

      }

      //   ::user::interaction::OnDestroy();
   }

   // self destruction
   void impact::PostNcDestroy()
   {
      ::user::interaction::PostNcDestroy();
      /*      if (is_set_ca_flag(object::flag_auto_delete))
            {
               // default for views is to allocate them on the heap
               //  the default post-cleanup is to 'delete this'.
               //  never explicitly call 'delete' on a ::user::impact
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

   void impact::route_command_message(::user::command * pcommand)
   {

      ::user::box::route_command_message(pcommand);

      if(pcommand->m_bRet)
      {

         return;

      }

      if (::user::impact::get_document() != NULL)
      {

         ::user::impact::get_document()->route_command_message(pcommand);

         if(pcommand->m_bRet)
         {

            return;

         }

      }

      sp(::user::interaction) puiParent = GetParent();

      while (puiParent.is_set() && puiParent->is_place_holder())
      {

         puiParent = puiParent->get_parent();

      }

      if (puiParent.is_set() && !puiParent->is_frame_window())
      {

         puiParent->route_command_message(pcommand);

         if (pcommand->m_bRet)
         {

            return;

         }

      }

      Application.route_command_message(pcommand);

      if (pcommand->m_bRet)
      {

         return;

      }

   }


   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact drawing support

   void impact::OnPaint()
   {
      // standard paint routine
      // trans   CPaintDC spgraphics(this);
      //   trans OnPrepareDC(&spgraphics);
      //   trans OnDraw(&spgraphics);
   }


   ::user::document * impact::get_document(::user::interaction * pui)
   {

      sp(::user::impact) pview = pui;

      if (pview != NULL)
         return NULL;

      return pview->get_document();

   }


   void impact::_001OnInitialUpdate()
   {

      on_update(NULL, 0, NULL);        // initial update

   }


   void impact::on_update(::user::impact * pSender, LPARAM lHint, object* pHint)
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
      //UNUSED(pSender);     // unused in release builds

      // invalidate the entire pane, erase background too
      //Invalidate(TRUE);

      Application.on_update_view(this,pSender,lHint,pHint);

   }

   void impact::on_simple_view_update_hint(sp(::user::impact) pviewSender, e_hint ehint, object * phint)
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

   void impact::OnViewUpdateHint(sp(::user::impact) pSender, LPARAM lHint, ::user::view_update_hint * pHint)
   {
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(pHint);
   }


   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact selection support

   bool impact::IsSelected(const object* pDocItem) const
   {
      ASSERT_VALID(pDocItem);
      //  UNUSED(pDocItem);    // unused in release builds

      return FALSE;   // not implemented, so not selected
   }

   void impact::OnActivateView(bool bActivate, sp(::user::impact) pActivateView, sp(::user::impact))
   {
      //    UNUSED(pActivateView);   // unused in release builds

      if (bActivate)
      {
         //ASSERT(pActivateView == this);

         // take the focus if this frame/::user::impact/pane is now active
         if (IsTopParentActive())
            SetFocus();
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

   // either re-activate the current ::user::impact, or set this ::user::impact to be active
   sp(::user::impact) pview = pParentFrame->GetActiveView();
   oswindow oswindow_Focus = ::GetFocus();
   if (pview == this &&
   get_handle() != oswindow_Focus && !::IsChild(get_handle(), oswindow_Focus))
   {
   // re-activate this ::user::impact
   OnActivateView(TRUE, this, this);
   }
   else
   {
   // activate this ::user::impact
   pParentFrame->SetActiveView(this);
   }
   }
   return nResult;
   }
   */

   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact scrolling support

   bool impact::OnScroll(UINT /*nScrollCode*/, UINT /*nPos*/, bool /*bDoScroll*/)
   {
      return FALSE;
   }

   bool impact::OnScrollBy(size /*sizeScroll*/, bool /*bDoScroll*/)
   {
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact drag/drop support

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
   // ::user::impact splitting commands


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


   void impact::OnUpdateSplitCmd(::user::command* pCmdUI)
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

   void impact::OnUpdateNextPaneMenu(::user::command* pCmdUI)
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

   ///////////////////////////////////////////////////////////////////////////////
   //// Printing support virtual functions (others in viewpr.cpp)

   //void impact::OnPrepareDC(::draw2d::graphics * pgraphics, CPrintInfo* pInfo)
   //{
   //   UNREFERENCED_PARAMETER(pInfo);
   //   ASSERT_VALID(pgraphics);
   //   UNUSED(pgraphics); // unused in release builds

   //   // Default to one page printing if doc length not known
   //}




   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact diagnostics

   void impact::dump(dump_context & dumpcontext) const
   {
      ::user::interaction::dump(dumpcontext);

      if (((impact *) this)->::user::impact::get_document() != NULL)
         dumpcontext << "with ::user::document: ";
      else
         dumpcontext << "with no ::user::document\n";
   }

   void impact::assert_valid() const
   {
      ::user::interaction::assert_valid();
   }


   void impact::_001OnView(::message::message * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      if (pbase->m_wparam == ::user::view_message_update)
      {

         ::user::document::update * pupdate = (::user::document::update *) pbase->m_lparam.m_lparam;

         on_update(pupdate->m_pSender, pupdate->m_lHint, pupdate->m_pHint);

      }
      else if (pbase->m_wparam == ::user::view_message_close_document)
      {

         get_document()->close_document();

      }

   }


   ::user::interaction * impact::create_view(::user::interaction * pimpactAlloc, ::user::view_creator_data * pcreatordata, ::user::interaction * pviewLast)
   {

      sp(::create) pcreate(allocer());

      pcreate->m_pusercreate = canew(::user::create);

      pcreate->m_pusercreate->m_puiNew = pimpactAlloc;

      pcreate->m_pusercreate->m_puiLastView = pviewLast;

      pcreate->m_pusercreate->m_pdocumentCurrent = get_document();

      return s_create_view(pcreate, pcreatordata->m_rectCreate, pcreatordata->m_pholder, pcreatordata->m_id);

   }


   ::user::interaction * impact::create_view(type * pinfo, ::user::document * pdoc,const RECT & rect, ::user::interaction * pwndParent, id id, ::user::interaction * pviewLast)
   {

      sp(type) info(pinfo);

      sp(::create) pcreate(allocer());

      pcreate->m_pusercreate = canew(::user::create);

      pcreate->m_pusercreate->m_typeinfoNewView = info;

      pcreate->m_pusercreate->m_puiLastView = pviewLast;

      if (pdoc == NULL)
      {

         pcreate->m_pusercreate->m_pdocumentCurrent = get_document();

      }
      else
      {

         pcreate->m_pusercreate->m_pdocumentCurrent = pdoc;

      }

      if (pwndParent == NULL)
      {

         pwndParent = this;

      }

      if (id.is_empty())
      {

         id = (const ::id &) pcreate->m_pusercreate->m_typeinfoNewView->name();

      }

      return s_create_view(pcreate, rect, pwndParent, id);

   }


   ::user::interaction * impact::s_create_view(type * pinfo, ::user::document * pdoc,const RECT & rect, ::user::interaction * pwndParent,id id, ::user::interaction * pviewLast)
   {

      sp(type) info(pinfo);

      sp(::create) pcreate(pdoc->allocer());

      pcreate->m_pusercreate = canew(::user::create);

      pcreate->m_pusercreate->m_typeinfoNewView = info;

      pcreate->m_pusercreate->m_puiLastView = pviewLast;

      pcreate->m_pusercreate->m_pdocumentCurrent = pdoc;

      return s_create_view(pcreate, rect, pwndParent, id);

   }

   ::user::interaction * impact::s_create_view(::create * pcreate,const RECT & rect, ::user::interaction * pwndParent,id id)
   {

      ASSERT(pcreate != NULL);

      ASSERT(pcreate->m_pusercreate->m_typeinfoNewView || pcreate->m_pusercreate->m_puiNew != NULL);

      ::aura::application * papp = pwndParent->get_app();

      ::user::interaction * pui;

      if (pcreate->m_pusercreate->m_puiNew != NULL)
      {

         pui = dynamic_cast < ::user::interaction * > (pcreate->m_pusercreate->m_puiNew);

      }
      else
      {

         // Note: can be a ::user::interaction with PostNcDestroy self cleanup
         pui = App(papp).alloc < ::user::interaction >(pcreate->m_pusercreate->m_typeinfoNewView);

         //::id idProperty = "bk." + string(typeid(*pui).name());

         if (pui == NULL)
         {

            //         TRACE1("Warning: Dynamic create of ::user::impact type %hs failed.\n", pContext->m_typeinfoNewView.name());
            return NULL;

         }

      }

      ASSERT_KINDOF(::user::interaction, pui);


      // views are always created with a border!
      if (!pui->create_window(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, pwndParent, id, pcreate))
      {

         //TRACE0("Warning: could not create ::user::impact for frame.\n");
         return NULL;        // can't continue without a ::user::impact

      }


      ::user::impact * pview = dynamic_cast < ::user::impact * > (pui);

      if (pview != NULL)
      {

         pview->_001OnInitialUpdate();

      }

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


   void impact::_001OnLButtonDown(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      pobj->previous();

      if (GetParentFrame() != NULL)
      {

         GetParentFrame()->SetActiveView(this);

      }

   }

   void impact::_001OnLButtonUp(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //SCAST_PTR(::message::mouse, pmouse, pobj);
   }

   void impact::_001OnMouseMove(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //   SCAST_PTR(::message::mouse, pmouse, pobj);
   }


   ::user::document * impact::get_document() const
   {

      ASSERT(this != NULL);

      return ((::user::impact *) this)->m_pdocument;

   }


   void impact::collaborate(::job * pjob)
   {
      {
         ::user::job * puserjob = (dynamic_cast < ::user::job * > (pjob));
         if (puserjob != NULL)
         {
            puserjob->m_pui = this;
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


   //void impact::on_draw_view_nc(::draw2d::dib * pdib)
   //{

   //   UNREFERENCED_PARAMETER(pgraphics);

   //}

   //void impact::on_draw_view(::draw2d::graphics * pgraphics, spa(::data::data) spadata)
   //{

   //   UNREFERENCED_PARAMETER(pgraphics);
   //   UNREFERENCED_PARAMETER(spadata);

   //}

   //void impact::defer_draw_view(::draw2d::dib * pdib)
   //{

   //   if (get_document() == NULL)
   //      return;

   //   spa(::data::data) spadata = get_document()->m_spadata;

   //   //spadata.add(get_document()->m_spdata);

   //   object_spa sync;

   //   for (index i = 0; i < spadata.get_count(); i++)
   //   {

   //      sync.add(spadata[i].data_mutex());

   //   }

   //   retry_multi_lock sl(sync, millis(1), millis(1));

   //   try
   //   {
   //      on_draw_view(pgraphics, spadata);
   //   }
   //   catch (...)
   //   {
   //   }

   //}

   //void impact::_001OnDraw(::draw2d::graphics * pgraphics)
   //{

   //   on_draw_view_nc(pgraphics);

   //   int32_t iTry = 0;

   //   bool bOk;

   //retry:

   //   bOk = true;

   //   try
   //   {
   //      defer_draw_view(pgraphics);
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
   // ::user::impact second phase construction - bind to document

   //const CHAR _vfxWndFrameOrView[] = __WNDFRAMEORVIEW;




   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact drawing support


   /*void impact::_001OnInitialUpdate()
   {
   on_update(NULL, 0, NULL);        // initial update
   }*/

   /*   void impact::on_update(::user::impact * pSender, LPARAM lHint, object * pHint)
   {
   ::user::impact::on_update(pSender, lHint, pHint);
   }
   */
   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact selection support

   /*   bool impact::IsSelected(const object* pDocItem) const
   {
   ASSERT_VALID(pDocItem);
   //      UNUSED(pDocItem);    // unused in release builds

   return FALSE;   // not implemented, so not selected
   }*/


   void impact::_001OnMouseActivate(::message::message * pobj)
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
         ASSERT(pParentFrame == pmouseactivate->get_desktop_window()
                || pmouseactivate->get_desktop_window()->IsChild(pParentFrame));

         // either re-activate the current ::user::impact, or set this ::user::impact to be active
         sp(::user::impact) pview = pParentFrame->GetActiveView();
         sp(::user::interaction) oswindow_Focus = Session.get_focus_ui();
         if (pview == this &&
               this != oswindow_Focus && !IsChild(oswindow_Focus))
         {
            // re-activate this ::user::impact
            OnActivateView(TRUE, this, this);
         }
         else
         {
            // activate this ::user::impact
            pParentFrame->SetActiveView(this);
         }
      }

      pmouseactivate->set_lresult(pmouseactivate->get_message());

      pmouseactivate->m_bRet = true;

   }


   void impact::_001CallOnDraw(::draw2d::graphics * pgraphics)
   {

      on_viewport_offset(pgraphics);

      synch_lock slView(m_pmutex);

      //synch_lock slDocument(get_document()->m_pmutex);.

      {

         draw_select ds(this, pgraphics);

         _001OnDraw(pgraphics);

      }

   }


   void impact::on_select()
   {
      sp(::user::frame_window) pParentFrame = (GetParentFrame());
      if (pParentFrame != NULL)
      {
         // eat it if this will cause activation
         /*      ASSERT(pParentFrame == pmouseactivate->get_desktop_window()
         || pmouseactivate->get_desktop_window()->IsChild(pParentFrame));*/

         // either re-activate the current ::user::impact, or set this ::user::impact to be active
         sp(::user::impact) pview = pParentFrame->GetActiveView();
         sp(::user::interaction) oswindow_Focus = Session.get_focus_ui();
         if (pview == this &&
               this != oswindow_Focus && !IsChild(oswindow_Focus))
         {
            // re-activate this ::user::impact
            OnActivateView(TRUE, this, this);
         }
         else
         {
            // activate this ::user::impact
            pParentFrame->SetActiveView(this);
         }
      }
   }


//#ifndef ANDROID
//
//   /////////////////////////////////////////////////////////////////////////////
//   // ::user::impact drag/drop support
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




   void impact::_001OnUpdateSplitCmd(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void impact::_001OnSplitCmd(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void impact::_001OnUpdateNextPaneMenu(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void impact::_001OnNextPaneCmd(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   // // IMPLEMENT_DYNAMIC for ::user::impact is in wincore.cpp for .OBJ granularity reasons

   //// IMPLEMENT_DYNAMIC(CSplitterWnd, ::user::interaction)   // for swap tuning
   //// IMPLEMENT_DYNAMIC(CCtrlView, ::user::impact)

   /////////////////////////////////////////////////////////////////////////////


   /////////////////////////////////////////////////////////////////////////////
   // ::user::impact's OnPrintPreview.  Here to force linkage

   void impact::_001OnFilePrintPreview(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   void impact::_001OnFilePrint(::message::message * pobj)
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
   TRACE1("Warning: Dynamic create of ::user::impact type %hs failed.\n",
   pContext->m_typeinfoNewView.name());
   return NULL;
   }
   ASSERT_KINDOF(::user::interaction, pview);

   // views are always created with a border!
   if (!pview->create(NULL, NULL, __WS_DEFAULT_VIEW,
   rect(0,0,0,0), this, nID, (sp(::create)) pContext))
   {
   TRACE0("Warning: could not create ::user::impact for frame.\n");
   return NULL;        // can't continue without a ::user::impact
   }

   sp(::user::impact) pview =  (pview);
   pview->_001OnInitialUpdate();
   if (afxData.bWin4 && (pview->GetExStyle() & WS_EX_CLIENTEDGE))
   {
   // remove the 3d style from the frame, since the ::user::impact is
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
   TRACE1("Warning: Dynamic create of ::user::impact type %hs failed.\n",
   pContext->m_typeinfoNewView.name());
   return NULL;
   }
   ASSERT_KINDOF(interaction_impl, pview);

   // views are always created with a border!
   if (!pview->create(NULL, NULL, __WS_DEFAULT_VIEW,
   rect(0,0,0,0), pwndParent, nID, (sp(::create)) pContext))
   {
   TRACE0("Warning: could not create ::user::impact for frame.\n");
   return NULL;        // can't continue without a ::user::impact
   }

   ( (pview))->_001OnInitialUpdate();
   if (afxData.bWin4 && (pview->GetExStyle() & WS_EX_CLIENTEDGE))
   {
   // remove the 3d style from the frame, since the ::user::impact is
   //  providing it.
   // make sure to recalc the non-client area
   ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
   }*/
   //   return pview;
   //}


   void impact::_001OnRButtonDown(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //SCAST_PTR(::message::mouse, pmouse, pobj);

      GetParentFrame()->SetActiveView((this));
   }

   void impact::_001OnMButtonDown(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //      SCAST_PTR(::message::mouse, pmouse, pobj);

      GetParentFrame()->SetActiveView((this));
   }


   bool impact::has_command_handler(::user::command * pcommand)
   {

      if (command_target::has_command_handler(pcommand))
      {

         return true;

      }

      if (get_document() != NULL)
      {

         if (get_document()->has_command_handler(pcommand))
         {

            return true;

         }

      }

      if (get_parent() != NULL)
      {

         if (get_parent()->has_command_handler(pcommand))
         {

            return true;

         }

      }

      return false;

   }


   void impact::walk_pre_translate_tree(::message::message * pobj,sp(::user::interaction) puiStop)
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


   ::database::key impact::calc_parent_data_key()
   {

      return get_document()->calc_data_key();

   }


   void impact::_001OnInitialUpdateMessage(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      _001OnInitialUpdate();

   }


   bool impact::is_local_data()
   {

      if(::user::box::is_local_data())
      {

         return true;

      }

      if(get_document() != NULL && get_document()->is_local_data())
      {

         return true;

      }

      return false;

   }


} // namespace user



