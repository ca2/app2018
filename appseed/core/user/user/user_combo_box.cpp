#include "framework.h"


namespace user
{


   combo_box::combo_box()
   {

      user_combo_box_common_construct();

   }


   combo_box::combo_box(::aura::application * papp) :
      object(papp),
      ::user::plain_edit(papp)
   {

      user_combo_box_common_construct();

   }


   void combo_box::user_combo_box_common_construct()
   {

      m_iSel                  = -1;
      m_bCaseSensitiveMatch   = false;
      m_typeComboList         = System.type_info < ::user::combo_list > ();
      m_estyle                = style_simply;
      m_bEdit                 = true;
      m_edatamode             = data_mode_opaque;

   }


   combo_box::~combo_box()
   {

      if(m_plist.is_set())
      {

         if(m_plist->IsWindow())
         {

            m_plist->DestroyWindow();

         }

      }

   }


   void combo_box::install_message_routing(::message::sender * psender)
   {

      if(m_bEdit)
      {

         ::user::plain_edit::install_message_routing(psender);

      }
      else
      {

         ::user::control::install_message_routing(psender);

      }

      IGUI_MSG_LINK(WM_LBUTTONDBLCLK, psender, this, &combo_box::_001OnLButtonDblClk);
      IGUI_MSG_LINK(WM_LBUTTONDOWN, psender, this, &combo_box::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_LBUTTONUP, psender, this, &combo_box::_001OnLButtonUp);
      IGUI_MSG_LINK(WM_KEYDOWN,psender,this,&combo_box::_001OnKeyDown);
      IGUI_MSG_LINK(WM_KEYUP,psender,this,&combo_box::_001OnKeyUp);
      IGUI_MSG_LINK(WM_SETFOCUS,psender,this,&combo_box::_001OnSetFocus);
      IGUI_MSG_LINK(WM_KILLFOCUS, psender, this, &combo_box::_001OnKillFocus);
      IGUI_MSG_LINK(WM_MOUSEMOVE, psender, this, &combo_box::_001OnMouseMove);
      IGUI_MSG_LINK(WM_MOUSELEAVE, psender, this, &combo_box::_001OnMouseLeave);
      IGUI_MSG_LINK(WM_SHOWWINDOW, psender, this, &combo_box::_001OnShowWindow);
      IGUI_MSG_LINK(WM_MOVE, psender, this, &combo_box::_001OnMove);

   }


   void combo_box::_001OnDrawStaticText(::draw2d::graphics * pgraphics)
   {

      string strText;

      if (m_bEdit)
      {

         _001GetText(strText);

      }
      else
      {

         _001GetListText(_001GetCurSel(), strText);

      }

      rect rectClient;

      GetClientRect(rectClient);
      ::user::e_color colorText = color_text;

      if (m_pdrawcontext != NULL)
      {

         if (m_pdrawcontext->is_control_selected())
         {

            if (m_pdrawcontext->is_control_hover())
            {

               colorText = color_text_selected_highlight;

            }
            else
            {

               colorText = color_text_selected;

            }

         }
         else
         {

            if (m_pdrawcontext->is_control_hover())
            {

               colorText = color_text_hover;

            }
            else
            {

               colorText = color_text;

            }

         }

      }

      if(!select_text_color(pgraphics, colorText))
      {

         pgraphics->set_text_color(ARGB(255,0,0,0));

      }

      rect rectText;

      get_element_rect(rectText, element_text);

      select_font(pgraphics, font_plain_edit);

      int iDrawTextFlags = _001GetInt(m_eintDrawTextFlags);

      pgraphics->draw_text(strText, rectText, iDrawTextFlags);

   }


   void combo_box::_001OnDrawCombo(::draw2d::graphics * pgraphics)
   {

      rect rectClient;

      GetClientRect(rectClient);

      ::draw2d::brush_sp br(allocer());

      if(m_bEdit)
      {

         ::user::plain_edit::_001OnDraw(pgraphics);

      }
      else
      {

         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

         _001OnDrawStaticText(pgraphics);

      }

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      rect rectDropDown;

      get_element_rect(rectDropDown, element_drop_down);

      rect rectDropIn(rectDropDown);

      ::user::e_color colorDropDown = color_button_background_disabled;

      if (m_pdrawcontext != NULL)
      {

         if (m_pdrawcontext->is_control_selected())
         {

            if (m_pdrawcontext->is_control_hover())
            {

               colorDropDown = color_action_hover_border_color;

            }
            else
            {

               colorDropDown = color_action_hover_border_color;

            }

         }
         else
         {

            if (m_pdrawcontext->is_control_hover())
            {

               colorDropDown = color_action_hover_border_color;

            }
            else
            {

               colorDropDown = color_button_background_disabled;

            }

         }

      }
      else
      {

         if (Session.get_focus_ui() == this)
         {

            if (m_iHover >= 0)
            {

               colorDropDown = color_action_hover_border_color;

            }
            else
            {

               colorDropDown = color_action_hover_border_color;

            }

         }
         else
         {

            if (m_iHover >= 0)
            {

               colorDropDown = color_action_hover_border_color;

            }
            else
            {

               colorDropDown = color_button_background_disabled;

            }

         }

      }

      color c(_001GetColor(colorDropDown, ARGB(210, 230, 230, 230)));

      c.hls_rate(0.0, 0.5, 0.1);

      br->create_solid(c);

      pgraphics->SelectObject(br);

      pgraphics->fill_rect(rectDropIn);

      ::draw2d::path_sp path(allocer());

      point_array pointa;

      get_simple_drop_down_open_arrow_polygon(pointa);

      br->create_solid(ARGB(210, 0, 0, 0));

      pgraphics->SelectObject(br);

      pgraphics->fill_polygon(pointa);

   }


   void combo_box::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      ::user::plain_edit::_000OnDraw(pgraphics);

   }


   void combo_box::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      ::user::control::_001OnDraw(pgraphics);

      _001OnDrawCombo(pgraphics);

   }


   void combo_box::_001GetText(string & str) const
   {

      if(m_bEdit)
      {

         if(m_iSel < 0)
         {

            ::user::plain_edit::_001GetText(str);

         }
         else
         {

            _001GetListText(m_iSel, str);

         }

      }
      else
      {

         if(m_iSel < 0)
         {

            str = m_strText;

         }
         else
         {

            _001GetListText(m_iSel, str);

         }

      }

   }


   void combo_box::_001SetText(const string & str, ::action::context actioncontext)
   {

      if(m_bEdit)
      {

         ::user::plain_edit::_001SetText(str, actioncontext);

      }
      else
      {

         m_strText = str;

         _001OnAfterChangeText(actioncontext);

      }

   }


   index combo_box::_001GetListCount() const
   {

      return m_straList.get_count();

   }


   bool combo_box::get_element_rect(LPRECT lprect, e_element eelement)
   {

      if(eelement == element_text)
      {

         eelement = element_combo_edit;

      }

      return control::get_element_rect(lprect, eelement);

   }


   //void combo_box::get_simple_drop_down_open_arrow_polygon(point_array & pointa)
   //{

   //   rect rectDropDown;

   //   get_element_rect(rectDropDown, element_drop_down);

   //   int32_t cx = rectDropDown.width() / 3;

   //   int32_t cy = cx * 2 / 3;

   //   point ptCenter = rectDropDown.center();

   //   pointa.add(ptCenter.x - cx / 2, ptCenter.y - cy / 2);

   //   pointa.add(ptCenter.x + cx / 2, ptCenter.y - cy / 2);

   //   pointa.add(ptCenter.x, ptCenter.y + cy / 2);

   //}


   e_element combo_box::hit_test(point pt)
   {

      rect rectElement;

      if(get_element_rect(rectElement, element_drop_down))
      {

         if (rectElement.contains(pt))
         {

            return element_drop_down;

         }

      }

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.contains(pt))
      {

         return element_text;

      }

      /*

            if(get_element_rect(rectElement, element_text))
            {

               if (rectElement.contains(pt))
               {

                  return element_text;

               }

            }

      */

      return element_none;

   }


   void combo_box::_001OnShowWindow(::message::message * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      if (!pshowwindow->m_bShow)
      {

         if (m_plist.is_set())
         {

            m_plist->post_message(WM_CLOSE);

         }

      }

   }

   void combo_box::_001OnMove(::message::message * pobj)
   {

      if (is_drop_down())
      {

         rect rectWindow;

         GetWindowRect(rectWindow);

         m_plist->on_drop_down(rectWindow, m_sizeFull);

      }

   }


   void combo_box::_001OnKeyDown(::message::message * pobj)
   {

      SCAST_PTR(::message::key,pkey,pobj);

      if(pkey->m_ekey == ::user::key_down)
      {

         if(!is_drop_down())
         {

            _001ShowDropDown();

         }

      }

   }


   void combo_box::_001OnKeyUp(::message::message * pobj)
   {

      //SCAST_PTR(::message::key,pkey,pobj);

      UNREFERENCED_PARAMETER(pobj);

   }


   void combo_box::_001OnLButtonDown(::message::message * pobj)
   {

      //output_debug_string("\nCOMBO_BOX: LBUTTONDOWN\n");

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      if(!m_bEdit || hit_test(pt) == element_drop_down)
      {

         _001ToggleDropDown();

         pmouse->m_bRet = true;

      }

   }


   void combo_box::_001OnMouseMove(::message::message * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      if (m_iHover < 0)
      {

         m_iHover = 0;

         track_mouse_leave();

         set_need_redraw();

      }

      ::user::e_element eelementHover = hit_test(pt);

      if (!m_bEdit || eelementHover == element_drop_down)
      {

         pmouse->m_ecursor = ::visual::cursor_arrow;

         pmouse->m_bRet = true;

      }

      if (m_eelementHover != eelementHover) // on purpose
      {

         m_eelementHover = eelementHover; // on purpose;

         set_need_redraw();

      }

   }


   void combo_box::_001OnMouseLeave(::message::message * pmessage)
   {

      //SCAST_PTR(::message::mouse, pmouse, pmessage);

      UNREFERENCED_PARAMETER(pmessage);

      m_iHover = -1;

      set_need_redraw();

   }


   void combo_box::_001OnLButtonUp(::message::message * pobj)
   {


      //output_debug_string("\nCOMBO_BOX: LBUTTON UP\n");
      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      if (!m_bEdit || hit_test(pt) == element_drop_down)
      {

         pmouse->m_bRet = true;


      }


   }

   void combo_box::_001OnSetFocus(::message::message * pobj)
   {

//      SCAST_PTR(::message::set_focus, pfocus, pobj);
      //GetTopLevelFrame()->ActivateFrame();

   }


   void combo_box::_001OnKillFocus(::message::message * pobj)
   {

      SCAST_PTR(::message::kill_focus, pkillfocus, pobj);

      if (m_plist != NULL)
      {

         if (m_plist->get_safe_handle() != pkillfocus->m_oswindowNew)
         {

            m_plist->post_message(WM_CLOSE);

         }

      }


   }


   void combo_box::_001ToggleDropDown()
   {

      defer_create_combo_list();

      _001ShowDropDown(!m_plist->IsWindowVisible());

   }


   void combo_box::_001ShowDropDown(bool bShow)
   {

      if(bShow)
      {

         if (m_plist.is_set())
         {

            if (tick::now() - m_plist->m_tickLastHide < 300)
            {

               m_plist->m_tickLastHide.m_ui -= 300;

               output_debug_string("asking to show too soon!! not going to show!!");

               return;

            }

         }

         defer_create_combo_list();

         Session.on_show_user_input_popup(m_plist);

         ::draw2d::font_sp spfont = _001GetFont(font_plain_edit);

         m_plist->query_full_size(m_sizeFull);

         rect rectWindow;

         GetWindowRect(rectWindow);

         m_plist->on_drop_down(rectWindow, m_sizeFull);

      }
      else
      {

         if(m_plist.is_set())
         {

            m_plist->ShowWindow(SW_HIDE);

         }

      }

   }


   void combo_box::defer_create_combo_list()
   {

      if(m_plist == NULL)
      {

         sp(object) pca = Application.alloc(m_typeComboList);

         m_plist =  (pca);

         if(m_plist == NULL)
         {
            pca.release();
            _throw(resource_exception(get_app()));
         }

         m_plist->m_pcombo = this;

      }

      if (!m_plist->IsWindow())
      {

         ::user::create_struct cs(0, NULL, "combo_list");

         if (!m_plist->create_window_ex(cs))
         {

            m_plist.release();

            _throw(resource_exception(get_app()));

         }

         m_plist->SetOwner(this);

      }

      rect rectClient;

      GetClientRect(rectClient);

      m_plist->m_iItemHeight = MIN(24, rectClient.height());

   }


   void combo_box::_001SetCurSel(index iSel, ::action::context actioncontext)
   {

      if (m_iSel != iSel)
      {

         m_iSel = iSel;

         ::user::control_event ev;

         ev.m_puie = this;

         ev.m_id = m_id;

         ev.m_eevent = ::user::event_after_change_cur_sel;

         ev.m_iItem = iSel;

         ev.m_actioncontext = actioncontext;

         on_control_event(&ev);

         set_need_redraw();

      }

      if (!IsWindow())
      {

         return;

      }

      string strItem;

      _001GetListText(iSel,strItem);

      if (m_bEdit)
      {

         _001SetText(strItem, actioncontext);

      }
      else
      {

         _001SetText(strItem, ::action::source_sync);

      }

   }


   index combo_box::_001GetCurSel()
   {

      return m_iSel;

   }

   void combo_box::on_layout()
   {

      if(m_bEdit)
      {

         ::user::plain_edit::on_layout();

      }


      /*      ::draw2d::font_sp fontxyz(allocer());

            rect rectClient;

            GetClientRect(rectClient);

            fontxyz->m_dFontSize = rectClient.height() * 0.4;
            fontxyz->m_eunitFontSize = ::draw2d::unit_pixel;
            fontxyz->m_bUpdated = false;

            SetFont(fontxyz);*/



   }

   void combo_box::_001OnAfterChangeText(::action::context actioncontext)
   {

      string str;

      if(m_bEdit)
      {

         ::user::plain_edit::_001GetText(str);

      }
      else
      {

         str = m_strText;

      }

      m_iSel = _001FindListText(str);

      if (m_plist.is_set())
      {

         m_plist->on_change_combo_sel(m_iSel);

      }

   }


   void combo_box::_001OnInitialUpdate()
   {

      if(m_bEdit)
      {

         ::user::plain_edit::_001OnInitialUpdate();

      }
      //else
      //{

      //   ::user::control::_001OnInitialUpdate();

      //}


   }




   bool combo_box::create_window(const RECT & rect, sp(::user::interaction) pParentWnd, id id)
   {

      if (!::user::plain_edit::create_window(rect, pParentWnd, id))
         return false;

      return true;

   }


#ifdef WINDOWSEX
   //Derived class is responsible for implementing these handlers
   //for owner/self draw controls (except for the optional DeleteItem)
   void combo_box::DrawItem(LPDRAWITEMSTRUCT)
   { ASSERT(FALSE); }
   void combo_box::MeasureItem(LPMEASUREITEMSTRUCT)
   { ASSERT(FALSE); }
   int32_t combo_box::CompareItem(LPCOMPAREITEMSTRUCT)
   { ASSERT(FALSE); return 0; }
   void combo_box::DeleteItem(LPDELETEITEMSTRUCT)
   { /* default to nothing */ }
#endif

   bool combo_box::OnChildNotify(::message::base * pbase)
   {

      switch (pbase->m_id)
      {
      case WM_DRAWITEM:
#ifdef WINODWSEX
         DrawItem((LPDRAWITEMSTRUCT)pbase->m_lparam);
#else
         _throw(todo(get_app()));
#endif
         break;
      case WM_MEASUREITEM:
#ifdef WINODWSEX
         MeasureItem((LPMEASUREITEMSTRUCT)pbase->m_lparam);
#else
         _throw(todo(get_app()));
#endif
         break;
      case WM_COMPAREITEM:
#ifdef WINODWSEX
         *pResult = CompareItem((LPCOMPAREITEMSTRUCT)pbase->m_lparam);
#else
         _throw(todo(get_app()));
#endif
         break;
      case WM_DELETEITEM:
#ifdef WINODWSEX
         DeleteItem((LPDELETEITEMSTRUCT)pbase->m_lparam);
#else
         _throw(todo(get_app()));
#endif
         break;
      default:
         return ::user::interaction::OnChildNotify(pbase);
      }
      return TRUE;
   }

   void combo_box::GetLBText(index nIndex, string & rString)
   {
      ASSERT(IsWindow());
      GetLBText(nIndex, rString.GetBufferSetLength(GetLBTextLen(nIndex)));
      rString.ReleaseBuffer();
   }

   //::count combo_box::get_count()
   // { ASSERT(IsWindow()); return (count)send_message( CB_GETCOUNT, 0, 0); }
   //index combo_box::get_cur_sel()
   // { ASSERT(IsWindow()); return (index)send_message( CB_GETCURSEL, 0, 0); }
   //index combo_box::set_cur_sel(index nSelect)
   // { ASSERT(IsWindow()); return (index)send_message( CB_SETCURSEL, nSelect, 0); }
   //IA64: Assuming retval of CB_GETEDITSEL won't be expanded
   bool combo_box::GetEditSel(strsize & nStartChar, strsize & nEndChar)
   {
      /*      ASSERT(IsWindow()); uint32_t dw = uint32_t(send_message( CB_GETEDITSEL, 0, 0));
            nStartChar = LOWORD(dw);
            nEndChar = LOWORD(dw);*/
      return TRUE;
   }
   bool combo_box::LimitText(strsize nMaxChars)
   {
      //ASSERT(IsWindow());
      //return send_message( CB_LIMITTEXT, nMaxChars, 0) != FALSE;
      return true;
   }

   bool combo_box::SetEditSel(strsize nStartChar, strsize nEndChar)
   {
      //ASSERT(IsWindow());
      //return send_message( CB_SETEDITSEL, 0, MAKELONG(nStartChar, nEndChar)) != FALSE;
      return true;
   }

   uint_ptr combo_box::GetItemData(index nIndex)
   {

      //ASSERT(IsWindow());

      //return send_message( CB_GETITEMDATA, nIndex, 0);
      return 0;

   }

   index combo_box::SetItemData(index nIndex, uint_ptr dwItemData)
   {

      //ASSERT(IsWindow());

      //return (index)send_message( CB_SETITEMDATA, nIndex, (LPARAM)dwItemData);

      return -1;

   }

   void * combo_box::GetItemDataPtr(index nIndex)
   {

      //ASSERT(IsWindow()); return (LPVOID)GetItemData(nIndex);

      return NULL;

   }

   index combo_box::SetItemDataPtr(index nIndex, void * pData)
   {

      //ASSERT(IsWindow());

      //return SetItemData(nIndex, (uint_ptr)(LPVOID)pData);

      return -1;

   }

   index combo_box::GetLBText(index nIndex, LPSTR lpszText)
   {

      //ASSERT(IsWindow());

      //return (index)send_message( CB_GETLBTEXT, nIndex, (LPARAM)lpszText);

      return -1;

   }

   strsize combo_box::GetLBTextLen(index nIndex)
   {

      //ASSERT(IsWindow());

      //return (strsize)send_message( CB_GETLBTEXTLEN, nIndex, 0);

      return -1;

   }


   void combo_box::ShowDropDown(bool bShowIt)
   {

      _001ShowDropDown(bShowIt);

   }


   index combo_box::DeleteString(index nIndex)
   {

      //ASSERT(IsWindow());

      //return (index) send_message( CB_DELETESTRING, nIndex, 0);

      return -1;

   }

   index combo_box::InsertString(index nIndex, const char * lpszString)
   {

      //ASSERT(IsWindow());

      //return (int32_t)send_message( CB_INSERTSTRING, nIndex, (LPARAM)lpszString);

      return -1;

   }


   void combo_box::ResetContent()
   {

      synch_lock sl(m_pmutex);

      m_straList.remove_all();

      m_straValue.remove_all();

      m_iSel = -1;
      m_iHover = -1;

   }


   index combo_box::Dir(index attr, const char * lpszWildCard)
   {

//      ASSERT(IsWindow());

//      return (index)send_message( CB_DIR, attr, (LPARAM)lpszWildCard);

      return -1;

   }


   //index combo_box::FindString(index nStartAfter, const char * lpszString)
   //{

   //   for (index i = nStartAfter++; i < m_straList.get_count(); i++)
   //   {

   //      if (m_straList[i].compare(lpszString) == 0)
   //      {

   //         return i;

   //      }

   //   }

   //   return -1;

   //}


   //index combo_box::SelectString(index nStartAfter, const char * lpszString, ::action::context context)
   //{

   //   index iSel = FindString(nStartAfter, lpszString);

   //   if (iSel >= 0)
   //   {

   //      _001SetCurSel(iSel, context);

   //   }

   //   return -1;

   //}

   //
   void combo_box::clear()
   {

      //ASSERT(IsWindow());

      //send_message(WM_CLEAR, 0, 0);

   }


   void combo_box::copy()
   {

      //ASSERT(IsWindow());

      //send_message(WM_COPY, 0, 0);

   }

   void combo_box::Cut()
   {

      //ASSERT(IsWindow());

      //send_message( WM_CUT, 0, 0);

   }

   void combo_box::Paste()
   {

      //ASSERT(IsWindow());

      //send_message( WM_PASTE, 0, 0);

   }

   int32_t combo_box::SetItemHeight(index nIndex, UINT cyItemHeight)
   {

      //ASSERT(IsWindow());

      //return (int32_t)send_message( CB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));

      return -1;

   }

   int32_t combo_box::GetItemHeight(index nIndex)
   {

      //ASSERT(IsWindow());

      //return (int32_t)send_message( CB_GETITEMHEIGHT, nIndex, 0L);

      return -1;

   }

   index combo_box::FindStringExact(index nIndexStart, const char * lpszFind)
   {

      //ASSERT(IsWindow());

      //return (index)send_message( CB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);

      return -1;

   }

   int32_t combo_box::SetExtendedUI(bool bExtended)
   {

      //ASSERT(IsWindow());

      //return (int32_t)send_message( CB_SETEXTENDEDUI, bExtended, 0L);

      return -1;

   }

   bool combo_box::GetExtendedUI()
   {

      //ASSERT(IsWindow());

      //return send_message( CB_GETEXTENDEDUI, 0, 0L) != 0;

      return false;

   }

   void combo_box::GetDroppedControlRect(LPRECT lprect)
   {

      //ASSERT(IsWindow());

      //send_message( CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)lprect);

   }

   bool combo_box::GetDroppedState()
   {

      //ASSERT(IsWindow());

      //return send_message( CB_GETDROPPEDSTATE, 0, 0L) != 0;

      return false;

   }

   LCID combo_box::GetLocale()
   {

      //ASSERT(IsWindow());

      //return (LCID)send_message( CB_GETLOCALE, 0, 0);

      return -1; // everywhere

   }

   LCID combo_box::SetLocale(LCID nNewLocale)
   {

      //ASSERT(IsWindow());

      //return (LCID)send_message( CB_SETLOCALE, (WPARAM)nNewLocale, 0);

      return nNewLocale; // set where it is

   }

   index combo_box::GetTopIndex()
   {

      //ASSERT(IsWindow());

      //return (index)send_message( CB_GETTOPINDEX, 0, 0);

      return -1;

   }

   index combo_box::SetTopIndex(index nIndex)
   {

      //ASSERT(IsWindow());

      //return (index)send_message( CB_SETTOPINDEX, nIndex, 0);

      return -1;

   }

   ::count combo_box::InitStorage(::count nItems, UINT nBytes)
   {

      //ASSERT(IsWindow());

      //return (count)send_message( CB_INITSTORAGE, (WPARAM)nItems, nBytes);

      return -1;

   }

   void combo_box::SetHorizontalExtent(UINT nExtent)
   {

      //ASSERT(IsWindow());

      //send_message( CB_SETHORIZONTALEXTENT, nExtent, 0);

   }

   UINT combo_box::GetHorizontalExtent()
   {

      //ASSERT(IsWindow());

      //return (UINT)send_message( CB_GETHORIZONTALEXTENT, 0, 0);

      return 0;

   }

   int32_t combo_box::SetDroppedWidth(UINT nWidth)
   {

//      ASSERT(IsWindow());

//      return (int32_t)send_message( CB_SETDROPPEDWIDTH, nWidth, 0);

      return -1;

   }

   int32_t combo_box::GetDroppedWidth()
   {

//      ASSERT(IsWindow());

//      return (int32_t)send_message( CB_GETDROPPEDWIDTH, 0, 0);

      return -1;

   }





   bool combo_box::_001GetListText(index iSel,string & str) const
   {

      str.Empty();

      if (iSel < 0)
         return false;

      if (iSel >= m_straList.get_count())
         return false;

      str = m_straList[iSel];

      return true;

   }


   index combo_box::_001FindListText(const string & str) const
   {


      //index combo_box::_001FindListText(const string & str) const
      //{

      //   ::count ca = _001GetListCount();

      //   string strItem;

      //   for(index i = 0; i < ca; i++)
      //   {

      //      _001GetListText(i,strItem);

      //      if(m_bCaseSensitiveMatch)
      //      {

      //         if(str.compare(strItem) == 0)
      //            return i;

      //      }
      //      else
      //      {

      //         if(str.compare_ci(strItem) == 0)
      //            return i;

      //      }

      //   }

      //   return -1;

      //}


      if(m_bCaseSensitiveMatch)
      {
         return m_straList.find_first(str);
      }
      else
      {
         return m_straList.find_first_ci(str);
      }

   }







   index combo_box::AddString(const char * lpszString,uint_ptr dwItemData)
   {

      synch_lock sl(m_pmutex);

      ASSERT(m_edatamode == data_mode_opaque);

      if(m_edatamode != data_mode_opaque)
         return -1;

      m_straList.add(lpszString);

      m_straValue.add(::str::from(dwItemData));

      return m_straList.get_upper_bound();

   }


   index combo_box::AddString(const char * lpszString,const string & strValue)
   {

      ASSERT(m_edatamode == data_mode_string);

      if(m_edatamode != data_mode_string)
         return -1;

      m_straList.add(lpszString);

      m_straValue.add(strValue);

      return m_straList.get_upper_bound();

   }


   bool combo_box::has_action_hover()
   {

      return ::user::plain_edit::has_action_hover() || is_drop_down();

   }

   bool combo_box::has_text_input()
   {

      return m_bEdit && ::user::plain_edit::has_text_input();

   }


   COLORREF combo_box::get_action_hover_border_color()
   {

      //return ARGB(184,120,255,190);

      return ::user::plain_edit::get_action_hover_border_color();

   }


   void combo_box::_001SetCurSelByStringValue(const string & strValue,::action::context actioncontext)
   {

      index iSel = m_straValue.find_first(strValue);

      if(iSel < 0)
         return;

      _001SetCurSel(iSel,actioncontext);

   }


   void combo_box::_001SetCurSelByData(uint_ptr ui,::action::context actioncontext)
   {

      index iSel = m_straValue.find_first(::str::from(ui));

      if(iSel < 0)
         return;

      _001SetCurSel(iSel,actioncontext);

   }


   string combo_box::_001GetCurSelStringValue()
   {

      index iSel = _001GetCurSel();

      if(iSel < 0)
         return "";

      string str = m_straValue[iSel];

      return str;

   }


   bool combo_box::keyboard_focus_is_focusable()
   {

      return true;

   }


   bool combo_box::is_drop_down()
   {

      return m_plist != NULL && m_plist->IsWindowVisible();

   }


   bool combo_box::create_control(class control_descriptor * pdescriptor)
   {

      ASSERT(pdescriptor->get_control_type() == control_type_combo_box);

      if (!::user::control::create_control(pdescriptor))
      {

         TRACE("Failed to create control");

         return false;

      }

      ShowWindow(SW_HIDE);

      m_bMultiLine = false;

      return true;

   }


   void combo_box::_001OnLButtonDblClk(::message::message * pobj)
   {

      //output_debug_string("\nCOMBO_BOX: DOUBLE CLICK\n");

      _001OnLButtonDown(pobj);

      //pobj->m_bRet = true;

   }


   void combo_box::on_control_event(::user::control_event * pevent)
   {

      if(pevent->m_eevent == ::user::event_after_change_cur_sel)
      {

         if(pevent->m_puie == m_plist)
         {

            if (pevent->m_iItem >= 0)
            {

               _001SetCurSel(pevent->m_iItem, ::action::source_user);

            }

            _001ShowDropDown(false);

            pevent->Ret();

            set_need_redraw();

            keyboard_set_focus_next();

            return;

         }

      }

      ::user::plain_edit::on_control_event(pevent);

   }


} // namespace user



