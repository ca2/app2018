#include "framework.h"


namespace user
{


   tab_view::tab_view(::aura::application * papp) :
      object(papp),
      ::user::tab(papp),
      place_holder_container(papp)
   {

      m_flagNonClient.unsignalize(non_client_background);
      m_flagNonClient.unsignalize(non_client_focus_rect);
      get_data()->m_pcallback       = this;
      m_pdroptargetwindow           = NULL;
      m_pviewdata                   = NULL;
      m_pviewdataOld                = NULL;

   }


   tab_view::~tab_view()
   {
   }

   void tab_view::assert_valid() const
   {
      impact::assert_valid();
   }

   void tab_view::dump(dump_context & dumpcontext) const
   {
      impact::dump(dumpcontext);
   }


   void tab_view::_001OnCreate(::message::message * pobj)
   {
//      SCAST_PTR(::message::create, pcreate, pobj);
      if(pobj->previous())
         return;

      //on_create_tabs();

   }




   void tab_view::on_update(::user::impact * pSender, LPARAM lHint, object* pHint)
   {

      if (lHint == 0 && pHint == NULL)
      {

         if (get_tab_count() <= 1 && _001GetSel() < 0)
         {

            _001SetSel(0);

         }

      }

      if(m_pviewcreator != NULL)
      {

         m_pviewcreator->on_update(get_document(), pSender, lHint, pHint);

      }

      impact::on_update(pSender, lHint, pHint);

   }

   void tab_view::_001OnSetFocus(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      if(get_view_uie() != NULL)
      {
         get_view_uie()->SetFocus();
      }

   }

   bool tab_view::pre_create_window(::user::create_struct& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

      return impact::pre_create_window(cs);
   }

   void tab_view::rotate()
   {
      index iSel = _001GetSel();

      if(iSel < 0)
         iSel = 0;
      else
         iSel++;

      if(iSel >= _001GetPaneCount())
         iSel = 0;

      if(iSel >= _001GetPaneCount())
         return;

      _001SetSel(iSel);

   }

   void tab_view::_001OnMenuMessage(::message::message * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_wparam == 0 && pbase->m_lparam == 0)
      {
         set_cur_tab_by_id(m_pviewdataOld->m_id);
      }
   }

   void tab_view::install_message_routing(::message::sender * pinterface)
   {
      impact::install_message_routing(pinterface);
      ::user::tab::install_message_routing(pinterface);
      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &tab_view::_001OnCreate);
      IGUI_MSG_LINK(WM_USER + 1122  , pinterface, this, &tab_view::_001OnMenuMessage);
      IGUI_MSG_LINK(WM_SETFOCUS, pinterface, this, &tab_view::_001OnSetFocus);
   }


   void tab_view::_001OnTabClick(int32_t iTab)
   {

      ::user::tab::_001OnTabClick(iTab);

      //_001SetSel(iTab);

   }


   void tab_view::_001OnDropTab(index iTab, e_position eposition)
   {

      id id1 = ::user::tab::tab_id(::user::tab::_001GetSel());

      id id2 = ::user::tab::tab_id(iTab);

      id id3 = var(id1).get_string() + "->:<-" + var(id2).get_string();

      ::user::tab_pane * ppane1 = get_pane_by_id(id1);

      ::user::tab_pane * ppane2 = get_pane_by_id(id2);

      string strName1 = ppane1->m_istrTitleEx;

      string strName2 = ppane2->m_istrTitleEx;

      add_tab(strName1 + "->:<-" + strName2,id3);

      ::user::view_creator_data * pcreatordata = m_pviewcreator->allocate_creator_data(id3);

      sp(::user::place_holder) pholder = get_new_place_holder(get_data()->m_rectTabClient);

      pcreatordata->m_pholder = pholder;

      int iPaneNew = (int) id_pane(id3);

      get_data()->m_panea[iPaneNew]->m_pholder = pholder;

      sp(::user::split_view) psplitview = impact::create_view < ::user::split_view >(null_rect(),pholder,id3);

      pcreatordata->m_pwnd = psplitview;

      if(eposition == position_top || eposition == position_bottom)
      {
         psplitview->SetSplitOrientation(orientation_horizontal);
      }
      else
      {
         psplitview->SetSplitOrientation(orientation_vertical);
      }

      psplitview->SetPaneCount(2);

      psplitview->set_position_rate(0,0.5);

      psplitview->initialize_split_layout();

      rect rect0;

      rect rect1;

      psplitview->m_panea[0]->m_pholder->GetClientRect(rect0);

      psplitview->m_panea[1]->m_pholder->GetClientRect(rect1);

      sp(::user::interaction) pwnd1;

      sp(::user::interaction) pwnd2;

      if(eposition == position_top || eposition == position_left)
      {

         pwnd1 = get_impact(::user::tab::tab_id(::user::tab::_001GetSel()), rect1)->m_pwnd;

         pwnd2 = get_impact(::user::tab::tab_id(iTab),rect0)->m_pwnd;

         psplitview->SetPane(0,pwnd2,false);

         psplitview->SetPane(1,pwnd1,false);

      }
      else
      {

         pwnd1 = get_impact(::user::tab::tab_id(::user::tab::_001GetSel()),rect0)->m_pwnd;

         pwnd2 = get_impact(::user::tab::tab_id(iTab),rect1)->m_pwnd;

         psplitview->SetPane(0,pwnd1,false);

         psplitview->SetPane(1,pwnd2,false);

      }

      remove_tab_by_id(id1);

      remove_tab_by_id(id2);

      ::user::tab::on_layout();

      get_data()->m_iDragTab = -1;

      get_data()->m_bDrag = false;

   }


   ::user::interaction * tab_view::_001GetTabWnd(int32_t iTab)
   {

      if (m_pviewcreator == NULL)
      {

         return NULL;

      }

      if(m_pviewcreator->get(::user::tab::tab_id(iTab)) != NULL)
      {

         return m_pviewcreator->get(::user::tab::tab_id(iTab))->m_pwnd;

      }
      else
      {

         return NULL;

      }

   }


   void tab_view::_001DropTargetWindowInitialize(::user::tab * pinterface)
   {

      create_tab_by_id(::user::tab::tab_id(pinterface->get_data()->m_iDragTab));

      m_pdroptargetwindow = new tab_drop_target_window(this, (int32_t) pinterface->get_data()->m_iDragTab);
      rect rect;
      rect = pinterface->get_data()->m_rectTabClient;
      pinterface->ClientToScreen(&rect);
      m_pdroptargetwindow->create_window_ex(WS_EX_LAYERED, NULL, NULL, 0, rect, NULL, id());
      System.add_frame(m_pdroptargetwindow);
      m_pdroptargetwindow->ShowWindow(SW_SHOW);
      m_pdroptargetwindow->SetWindowPos(ZORDER_TOPMOST, rect, SWP_SHOWWINDOW);

   }

   void tab_view::_001DropTargetWindowRelay(::user::tab * pinterface)
   {
      UNREFERENCED_PARAMETER(pinterface);
      RedrawWindow();
   }

   void tab_view::_001DropTargetWindowFinalize(::user::tab * pinterface)
   {
      UNREFERENCED_PARAMETER(pinterface);
      if(m_pdroptargetwindow != NULL)
      {
         //System.remove_frame(m_pdroptargetwindow);
         //m_pdroptargetwindow->DestroyWindow();
         //m_pdroptargetwindow = NULL;
      }
   }


   void tab_view::_001OnShowTab(tab * ptab)
   {

      if(ptab == this)
      {

         on_show_view();

         on_stage_view();

      }

   }


   void tab_view::on_show_view()
   {

      id id = tab_id(_001GetSel());

      class id idSplit;

      rect rectTabClient = get_data()->m_rectTabClient;

      ::user::view_creator_data * pcreatordata = get_impact(id, rectTabClient);

      if (pcreatordata == NULL)
      {

         return;

      }

      index iPane = ::user::tab::id_pane(id);

      if(iPane >= 0)
      {

         if(pcreatordata->m_pholder != NULL)
         {

            get_data()->m_panea[iPane]->m_pholder = pcreatordata->m_pholder;

         }
         else if(pcreatordata->m_pwnd != NULL)
         {

            if(pane_holder(iPane) == NULL)
            {

               get_data()->m_panea[iPane]->m_pholder = place(pcreatordata->m_pwnd,get_data()->m_rectTabClient);

            }
            else
            {

               get_data()->m_panea[iPane]->m_pholder->m_uiptraChild.remove_all();

               get_data()->m_panea[iPane]->m_pholder->hold(pcreatordata->m_pwnd);

            }

         }
         else
         {

            get_data()->m_panea[iPane]->m_pholder = get_new_place_holder(get_data()->m_rectTabClient);

         }

      }

      if(pcreatordata->m_strCreatorDataTitle.has_char())
      {

         index iPane = tab_pane(_001GetSel());

         if (iPane >= 0 && get_data()->m_panea[iPane]->m_id == pcreatordata->m_id)
         {

            get_data()->m_panea[iPane]->m_istrTitleEx = pcreatordata->m_strCreatorDataTitle;

         }

      }

      idSplit = pcreatordata->m_idSplit;

      if(pcreatordata != m_pviewdata)
      {

         m_pviewdataOld = m_pviewdata;

         m_pviewdata = pcreatordata;

         if(m_pviewdata->m_eflag.is_signalized(::user::view_creator_data::flag_hide_all_others_on_show))
         {

            ::user::view_creator::view_map::pair * ppair = m_pviewcreator->m_viewmap.PGetFirstAssoc();

            while(ppair != NULL)
            {

               try
               {

                  if(ppair->m_element2 != m_pviewdata)
                  {

                     if(ppair->m_element2->m_pholder != NULL)
                     {

                        ppair->m_element2->m_pholder->ShowWindow(SW_HIDE);

                     }

                  }

               }
               catch(...)
               {

               }

               ppair = m_pviewcreator->m_viewmap.PGetNextAssoc(ppair);

            }

         }

      }

   }


   void tab_view::on_stage_view()
   {

      if (m_pviewdata == NULL || m_pviewdata->m_pholder == NULL)
      {

         return;

      }

      if (m_pviewdata->m_pwnd == NULL)
      {

         if (m_pviewdata->m_pholder->m_uiptraChild.get_count() > 0)
         {

            m_pviewdata->m_pwnd = m_pviewdata->m_pholder->m_uiptraChild[0];

         }

      }

      rect rectClient;

      m_pviewdata->m_pholder->GetClientRect(rectClient);

      rect rectTabClient = get_data()->m_rectTabClient;

      if (rectTabClient.area() > 0)
      {

         rectTabClient -= rectTabClient.top_left();

         if (rectClient != rectTabClient)
         {

            m_pviewdata->m_pholder->SetWindowPos(ZORDER_TOP, get_data()->m_rectTabClient, 0);

         }
         else
         {

            m_pviewdata->m_pholder->SetWindowPos(ZORDER_TOP, get_data()->m_rectTabClient, SWP_NOSIZE | SWP_NOMOVE);

         }

      }

      m_pviewdata->m_pholder->ShowWindow(SW_SHOW);


      if (m_pviewcreator != dynamic_cast < ::user::view_creator * > (this))
      {

         m_pviewcreator->on_show_view();

      }

      ::user::impact * pview = NULL;

      if (m_pviewdata->m_pwnd != NULL)
      {

         pview = dynamic_cast <::user::impact *> (m_pviewdata->m_pwnd);

      }

      if (pview == NULL)
      {

         pview = this;

      }

      GetParentFrame()->SetActiveView(pview);

      Application.on_show_view(this);

   }


   ::index tab_view::create_tab_by_id(id id)
   {

      if (get_impact(id, get_data()->m_rectTabClient) == NULL)
      {

         return -1;

      }

      index iTab = id_tab(id);

      if (iTab < 0)
      {

         return -1;

      }

      return iTab;

   }

   ::user::view_creator_data * tab_view::get_impact(id id, bool bCallOnCreateView)
   {

      return get_impact(id, get_data()->m_rectTabClient, bCallOnCreateView);

   }

   ::user::view_creator_data * tab_view::get_impact(id id, LPCRECT lpcrectCreate, bool bCallOnCreateView)
   {

      ::user::view_creator_data * pdata = get(id);

      if (pdata != NULL)
      {

         return pdata;

      }

      if (!bCallOnCreateView)
      {

         pdata = allocate_creator_data(id, lpcrectCreate);

         return pdata;

      }

      pdata = create_impact(id, lpcrectCreate);

      if (pdata == NULL)
      {

         return NULL;

      }

      return pdata;

   }


   ::user::view_creator_data * tab_view::create_impact(id id)
   {

      return create_impact(id,get_data()->m_rectTabClient);

   }


   ::user::view_creator_data * tab_view::create_impact(id id, LPCRECT lpcrectCreate)
   {

      if(m_pviewcreator == NULL)
         return NULL;

      ::user::view_creator_data * pcreatordata = m_pviewcreator->::user::view_creator::create_impact(id,lpcrectCreate);

      if(pcreatordata != NULL)
      {

         id = pcreatordata->m_id;

         if (id_pane(id) == -1)
         {

            ::user::tab::add_tab("", id, true, false, pcreatordata->m_pholder);

         }

         ::user::tab_pane * ppane = get_pane_by_id(id);

         if(ppane != NULL)
         {

            if(pcreatordata->m_strCreatorDataTitle.has_char() && ppane->m_id == pcreatordata->m_id)
            {

               ppane->m_istrTitleEx = pcreatordata->m_strCreatorDataTitle;

            }

            if(ppane != NULL)
            {

               ppane->m_pholder = pcreatordata->m_pholder;

            }

         }

         //on_change_pane_count();

      }

      return pcreatordata;

   }

   ::user::view_creator_data * tab_view::create_impact(id id, ::user::frame_window * pframewindow)
   {

      return create_impact(id, get_data()->m_rectTabClient, pframewindow);

   }

   ::user::view_creator_data * tab_view::create_impact(id id, LPCRECT lpcrectCreate, ::user::frame_window * pframewindow)
   {

      if (m_pviewcreator == NULL)
         return NULL;

      ::user::view_creator_data * pcreatordata = m_pviewcreator->::user::view_creator::create_impact(id, lpcrectCreate);

      if (pcreatordata != NULL)
      {

         if (id_pane(id) == -1)
         {

            ::user::tab::add_tab("", id);

         }

         ::user::tab_pane * ppane = get_pane_by_id(id);

         if (ppane != NULL)
         {

            if (pcreatordata->m_strCreatorDataTitle.has_char() && ppane->m_id == pcreatordata->m_id)
            {

               ppane->m_istrTitleEx = pcreatordata->m_strCreatorDataTitle;

            }

            if (ppane != NULL)
            {

               ppane->m_pholder = pcreatordata->m_pholder;

            }

         }

         on_change_pane_count();

      }

      return pcreatordata;

   }

   id tab_view::get_view_id()
   {

      if (m_pviewdata == NULL)
      {

         return ::aura::system::idEmpty;

      }

      return m_pviewdata->m_id;

   }


   ::user::view_creator_data * tab_view::get_view_creator_data()
   {

      return m_pviewdata;

   }


   ::user::interaction * tab_view::get_view_uie()
   {

      ::user::view_creator_data * pcreatordata = get_view_creator_data();

      if (pcreatordata == NULL)
      {

         return NULL;

      }

      if (pcreatordata->m_pwnd != NULL)
      {

         return pcreatordata->m_pwnd;

      }

      if (pcreatordata->m_pholder != NULL && pcreatordata->m_pholder->m_uiptraChild.get_count() == 1)
      {

         return pcreatordata->m_pholder->m_uiptraChild[0];

      }

      return NULL;

   }

   ::user::document * tab_view::get_view_document()
   {
      if(m_pviewdata == NULL)
         return NULL;
      return m_pviewdata->m_pdoc;
   }

   void tab_view::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      defer_check_layout();

      defer_check_zorder();

      if(!is_this_visible())
         return;

      if (m_bDrawTabAtBackground)
      {

         _001DrawThis(pgraphics);

         _001DrawChildren(pgraphics);

      }
      else
      {

         _001DrawChildren(pgraphics);

         _001DrawThis(pgraphics);

      }

   }

   
   void tab_view::_001OnCmdMsg(::user::command * pcommand)
   {

      if (!handle(pcommand))
      {

         return;

      }

      if (get_view_uie() != NULL)
      {

         get_view_uie()->_001OnCmdMsg(pcommand);

         if (pcommand->m_bRet)
         {

            return;

         }

      }

      return impact::_001OnCmdMsg(pcommand);

   }

   void tab_view::set_view_creator(::user::view_creator * pviewcreator)
   {

      m_pviewcreator = pviewcreator;

      if(m_pviewcreator != NULL)
      {
         m_pviewcreator->m_pviewcontainer = this;
      }

   }

   ::user::view_creator * tab_view::get_view_creator()
   {
      return m_pviewcreator;
   }

   tab_drop_target_window::tab_drop_target_window(::user::tab * ptab, int32_t iPane) :
      ::object(ptab->get_app())
   {
      m_ptab            = ptab;
      m_iPane           = iPane;
   }

   tab_drop_target_window::~tab_drop_target_window()
   {
   }

   void tab_drop_target_window::install_message_routing(::message::sender * pinterface)
   {
      ::user::interaction::install_message_routing(pinterface);
      IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &tab_drop_target_window::_001OnLButtonUp);
   }


   void tab_drop_target_window::_001OnDraw(::draw2d::graphics * pgraphics)
   {



//      class imaging & imaging = System.visual().imaging();

      COLORREF crBorder = ARGB(84, 127, 127, 80);
      COLORREF crBorderSel = ARGB(84, 40, 40, 20);

      rect rectClient;
      GetClientRect(rectClient);
      pgraphics->FillSolidRect(rectClient, ARGB(84, 255,255,255));

      point ptCursor;
      Session.get_cursor_pos(&ptCursor);
      ScreenToClient(&ptCursor);
      e_position epositionDrag = m_ptab->DragHitTest(ptCursor);
      rect rectTop;
      m_ptab->GetDragRect(rectTop, position_top);
      COLORREF crTop;
      int iAlpha = 84 + 77;
      if(rectTop.contains(ptCursor))
      {
         crTop = ARGB(iAlpha, 255, 230, 155);
      }
      else
      {
         crTop = ARGB(iAlpha, 150,200,255);
      }
      rect rectBottom;
      m_ptab->GetDragRect(rectBottom, position_bottom);
      COLORREF crBottom;
      if(rectBottom.contains(ptCursor))
      {
         crBottom = ARGB(iAlpha, 255,230,155);
      }
      else
      {
         crBottom = ARGB(iAlpha, 150,200,255);
      }
      rect rectRight;
      m_ptab->GetDragRect(rectRight, position_right);
      COLORREF crRight;
      if(rectRight.contains(ptCursor))
      {
         crRight = ARGB(iAlpha, 255,230,155);
      }
      else
      {
         crRight = ARGB(iAlpha, 150,200,255);
      }
      rect rectLeft;
      m_ptab->GetDragRect(rectLeft, position_left);
      COLORREF crLeft;
      if(rectLeft.contains(ptCursor))
      {
         crLeft = ARGB(iAlpha, 255,230,155);
      }
      else
      {
         crLeft = ARGB(iAlpha, 150,200,255);
      }
      pgraphics->FillSolidRect(rectTop,crTop);
      pgraphics->FillSolidRect(rectLeft,crLeft);
      pgraphics->FillSolidRect(rectRight,crRight);
      pgraphics->FillSolidRect(rectBottom,crBottom);
      pgraphics->Draw3dRect(rectTop,crBorder,crBorder);
      pgraphics->Draw3dRect(rectLeft, crBorder, crBorder);
      pgraphics->Draw3dRect(rectRight, crBorder, crBorder);
      pgraphics->Draw3dRect(rectBottom, crBorder, crBorder);

      if(epositionDrag != position_none)
      {
         rect rectSel;
         m_ptab->GetDragRect(rectSel, epositionDrag);
         pgraphics->Draw3dRect(rectSel, crBorderSel, crBorderSel);
      }

   }

   void tab_drop_target_window::_001OnLButtonUp(::message::message * pobj)
   {
      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt(pmouse->m_pt);

      ScreenToClient(&pt);

      e_position eposition = m_ptab->DragHitTest(pt);

      m_ptab->_001OnDropTab(m_iPane, eposition);

      ShowWindow(SW_HIDE);

      DestroyWindow();

      //delete this;

      pobj->m_bRet = true;

   }





} // namespace user
