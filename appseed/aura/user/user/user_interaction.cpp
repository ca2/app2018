#include "framework.h"


::user::interaction * get_system_window_interaction(::os_system_window * psystemwindow);


#ifdef WINDOWSEX
#include "aura/os/windows/windows_windowing.h"
#define MESSAGE_WINDOW_PARENT HWND_MESSAGE
#include "aura/aura/os/windows/windows_system_interaction_impl.h"

#elif defined(METROWIN)

#include "aura/aura/os/metrowin/metrowin_user_impl.h"

#endif // METROWIN


namespace user
{


#ifdef METROWIN


   Agile<Windows::UI::Core::CoreWindow>(*interaction::s_get_os_window)(interaction * pui) = &interaction::get_os_window_default;


#endif


   interaction::interaction()
   {

      user_interaction_common_construct();

   }

   interaction::interaction(::aura::application * papp) :
      ::object(papp),
      ::user::interaction_base(papp)
   {

      user_interaction_common_construct();

   }


   void interaction::user_interaction_common_construct()
   {

      m_puiThis = this;

      m_bDrawable = true;

      m_bProDevian = false;

      m_ecolorBackground = color_background;

      m_econtroltype = control_type_none;

      m_iItemHeight = -1;
      m_flagNonClient.signalize(non_client_background);
      m_flagNonClient.signalize(non_client_focus_rect);

      m_bMouseHover = false;
      m_bRedraw = false;

      m_bRedraw = false;

      m_bRedrawOnVisible = false;

      m_bNeedLayout = false;

      m_bLockWindowUpdate = false;

      m_bRedrawing = false;
      m_pparent = NULL;
      m_bWorkspaceFullScreen = false;
      m_bHideOnTransparentFrame = false;
      m_puiThis = this;

      m_eappearance = appearance_normal;
      m_eappearanceRequest = appearance_none;
      m_bCursorInside = false;
      m_nFlags = 0;
      m_puiOwner = NULL;
      //m_pimpl                    = NULL;
      m_ecursor = ::visual::cursor_default;
      m_bModal = false;
      m_bRectOk = false;
      m_bLayoutEnable = true;
      //m_psession                 = NULL;
      m_bMessageWindow = false;

      m_bVoidPaint = false;
      //m_pparent                  = NULL;
      m_bBackgroundBypass = false;
      m_bEnableSaveWindowRect = false;

      m_bLockWindowUpdate = false;


      m_bDefaultWalkPreTranslateParentTree = false;

      m_bMoving = false;
      m_bMoveWindow = false;

      m_bSizeMove = false;

      m_ptScrollPassword1.x = 0;
      m_ptScrollPassword1.y = 0;
      m_palphasource = NULL;

      m_pform = NULL;
      m_pformParent = NULL;

   }


   interaction::~interaction()
   {

      //TRACE("::user::interaction::~interaction interaction=0x%016x %s", this, typeid(*this).name());

   }


   tooltip *    interaction::get_tooltip()
   {

      return m_ptooltip.cast<tooltip>();

   }


   bool interaction::defer_check_layout()
   {

      synch_lock sl(m_pmutex);

      if (m_pimpl.is_null() || m_pimpl->m_pui == NULL)
      {

         return false;

      }

      if(!m_bLayoutEnable)
      {

         return false;

      }

      if (!check_need_layout())
      {

         return defer_check_translation();

      }

      layout();

      translate();

      do_show_flags();

      clear_need_layout();

      clear_need_translation();

      clear_show_flags();

      return true;

   }


   bool interaction::check_need_layout()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_need_layout();

   }


   void interaction::clear_need_layout()
   {

      if (m_pimpl.is_null() || m_pimpl->m_pui == NULL)
      {

         return;

      }

      return m_pimpl->clear_need_layout();

   }


   void interaction::set_need_layout(bool bAscendants)
   {

      m_bNeedLayout = true;

      if (bAscendants && m_pparent != NULL)
      {

         m_pparent->set_need_layout(true);

      }

   }


   void interaction::set_need_redraw(bool bAscendants)
   {

      m_bRedraw = true;

      if (bAscendants && m_pparent != NULL)
      {

         m_pparent->set_need_redraw(true);

      }

      if (m_pimpl.is_set())
      {

         m_pimpl->set_need_redraw();

      }

   }


   bool interaction::defer_check_translation()
   {

      if(!m_bLayoutEnable)
      {

         return false;

      }

      if (!check_need_translation())
      {

         return defer_check_show_flags();

      }

      translate();

      do_show_flags();

      clear_need_translation();

      clear_show_flags();

      return true;

   }


   bool interaction::check_need_translation()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_need_translation();

   }


   void interaction::clear_need_translation()
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      return m_pimpl->clear_need_translation();

   }


   bool interaction::defer_check_show_flags()
   {

      if(!m_bLayoutEnable)
      {

         return false;

      }

      if (!check_show_flags())
      {

         return false;

      }

      do_show_flags();

      clear_show_flags();

      return true;

   }


   bool interaction::check_show_flags()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_show_flags();

   }


   void interaction::clear_show_flags()
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      return m_pimpl->clear_show_flags();

   }


   bool interaction::defer_check_zorder()
   {

      if(!m_bLayoutEnable)
      {

         return false;

      }

      if (!check_need_zorder())
      {

         return false;

      }

      do_zorder();

      clear_need_zorder();

      return true;

   }


   bool interaction::check_need_zorder()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->check_need_zorder();

   }


   void interaction::clear_need_zorder()
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      return m_pimpl->clear_need_zorder();

   }


   ::user::interaction * interaction::GetTopWindow() const
   {

      if (m_pimpl != NULL)
      {

         return m_pimpl->GetTopWindow();

      }

      sp(interaction) pui = get_wnd();

      if (pui.is_null())
         return NULL;

      return pui->GetTopWindow();

   }

   ::user::form * interaction::get_form()
   {

      return m_pform;

   }

   ::user::form * interaction::get_parent_form()
   {

      if (m_pformParent == NULL)
      {

         return get_form();

      }

      return m_pformParent;

   }


   ::user::interaction * interaction::get_bind_ui()
   {

      return Session.get_bound_ui(m_strView);

   }


   elemental * interaction::get_parent() const
   {

      return GetParent();

   }


   interaction * interaction::GetParent() const
   {

#if defined(METROWIN) || defined(ANDROID) || defined(APPLE_IOS)

      if (m_pparent == System.m_possystemwindow->m_pui)
      {

         return NULL;

      }

#elif !defined(WINDOWSEX) && !defined(MACOS) && !defined(LINUX)

#error "error"

#endif

      return m_pparent;

   }


   oswindow interaction::GetParentHandle() const
   {

      interaction * puiParent = GetParent();

      if (puiParent == NULL)
         return NULL;

      return puiParent->get_safe_handle();

   }


   sp(place_holder) interaction::place(::user::interaction * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return NULL;

   }



   bool interaction::on_before_set_parent(::user::interaction * puiParent)
   {

      return true;

   }



   ::user::interaction * interaction::SetParent(::user::interaction * puiParent)
   {

      if (puiParent != NULL && (puiParent == this || puiParent == GetParent() || IsDescendant(puiParent)))
      {

         return GetParent();

      }

      if (!on_before_set_parent(puiParent))
      {

         return GetParent();

      }

      if (m_pthreadUserInteraction.is_set()
            && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
      {

         m_pthreadUserInteraction->m_puiptraThread->remove(this);

      }

      m_pthreadUserInteraction.release();

      sp(::user::interaction_impl_base) pimplOld = m_pimpl;

      sp(interaction) pparentOld = GetParent();

      if (pparentOld != NULL)
      {

         if (puiParent == NULL)
         {

            {

restart:

               sp(::user::interaction) pui;

               while (get_impl()->m_guieptraMouseHover.get_child(pui))
               {

                  if (IsDescendant(pui) || pui == this)
                  {

                     try
                     {

                        pui->send_message(WM_MOUSELEAVE);

                     }
                     catch (...)
                     {

                     }

                     get_impl()->m_guieptraMouseHover.remove(pui);

                     goto restart;

                  }

               }

            }

            sp(::user::interaction_impl) pimplNew = Application.alloc(System.type_info < ::user::interaction_impl >());

            string strName;

            get_window_text(strName);

//            string strClassName;
//
//            GetClassName(strClassName);

            const char * lpszClassName = NULL;

            rect rectWindow;

            GetWindowRect(rectWindow);

            ::user::create_struct cs(
            get_window_long(GWL_EXSTYLE),
            lpszClassName,
            strName,
            get_window_long(GWL_STYLE),
            rectWindow
            );

            cs.style &= ~WS_CHILD;

            m_pthreadUserInteraction = ::get_thread();

            if (m_pthreadUserInteraction.is_set())
            {

               m_pthreadUserInteraction->uiptra().add(this);

            }

            if(!pre_create_window(cs))
            {

               pimplNew.release();

               pimplNew = NULL;

               m_pimpl = pimplOld;

            }
            else if (!pimplNew->create_window_ex(this, cs, NULL, GetDlgCtrlId()))
            {

               pimplNew.release();

               if (m_pthreadUserInteraction.is_set()
                     && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
               {

                  m_pthreadUserInteraction->m_puiptraThread->remove(this);

               }

               m_pthreadUserInteraction.release();

               pimplNew = NULL;

               m_pimpl = pimplOld;

            }
            else
            {

               {

                  //{

                  //   single_lock sl(m_pmutex);

                  //   pimplNew->m_pui = this;

                  //   m_pimpl = pimplNew;

                  //   pimplNew->install_message_routing(pimplNew);

                  //}

                  on_set_parent(puiParent);

               }

               if (pimplOld != NULL)
               {

                  try
                  {

                     pimplOld->remove_all_routes();

                     pimplOld->m_pui = NULL;

                     pimplOld->DestroyWindow();

                     pimplOld.release();

                  }
                  catch (...)
                  {

                  }

               }

            }

         }
         else
         {

            on_set_parent(puiParent);

         }

      }
      else
      {

         if (puiParent != NULL)
         {

            ::multithreading::post_quit(m_pimpl.cast < ::user::interaction_impl>()->m_pthreadProDevian);

            sp(::user::interaction_child) pimplNew = canew(::user::interaction_child(get_app()));

            pimplNew->m_pui = this;

            System.remove_frame(this); // no more a top level frame if it were one

            m_pimpl = pimplNew;

            string strName;

            int32_t iStyle = get_window_long(GWL_STYLE);

            iStyle |= WS_CHILD;

            rect rectWindow;

            GetWindowRect(rectWindow);

            puiParent->ScreenToClient(rectWindow);

            if (!pimplNew->create_window(this, NULL, strName, iStyle, rectWindow, puiParent, GetDlgCtrlId()))
            {

               m_pimpl = pimplOld;

               System.add_frame(this);

               pimplNew.release();

            }
            else
            {

               if (pimplOld != NULL)
               {

                  ::user::interaction_impl * pimpl = pimplOld.cast < ::user::interaction_impl >();

                  if (pimpl != NULL)
                  {

                     try
                     {

                        oswindow oswindow = oswindow_remove(pimpl);

                        {

                           synch_lock slSystem(System.m_pmutex);

                           System.m_pwindowmap->m_map.remove_key(oswindow);

                        }

                        pimpl->m_pui = NULL;

                        if (oswindow != NULL)
                        {

                           ::destroy_window(oswindow);

                        }
                        try
                        {

                           //pimpl->release();

                        }
                        catch (...)
                        {


                        }


                     }
                     catch (...)
                     {

                     }

                  }
                  else
                  {

                     pimplOld.release();

                  }

               }

               on_set_parent(puiParent);

            }

         }
         else
         {

         }

      }

      return pparentOld;

   }


   void interaction::set_timer(smart_pointer_array < ::aura::timer_item > timera)
   {

      for (int32_t i = 0; i < timera.get_count(); i++)
      {

         SetTimer(timera[i]->m_uiId, timera[i]->m_uiElapse, NULL);

      }

   }


   bool interaction::GetClientRect(LPRECT lprect)
   {

      RECT64 rect;

      if (!GetClientRect(&rect))
      {

         return false;

      }

      lprect->left = (LONG)rect.left;
      lprect->top = (LONG)rect.top;
      lprect->right = (LONG)rect.right;
      lprect->bottom = (LONG)rect.bottom;

      return true;

   }


   bool interaction::GetWindowPos(LPRECT lprect)
   {

      if (!GetWindowRect(lprect))
      {

         return false;

      }

      ::user::interaction * puiParent = GetParent();

      if (puiParent != NULL)
      {

         puiParent->ScreenToClient(lprect);

      }

      return true;

   }


   bool interaction::GetWindowRect(LPRECT lprect)
   {

      rect64 rectWindow;

      if (!GetWindowRect(rectWindow))
      {

         return false;

      }

      ::copy(lprect, rectWindow);

      return true;

   }


   bool interaction::GetWindowRect(RECT64 * lprect)
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      if (!m_pimpl->GetWindowRect(lprect))
      {

         return false;
      }

      return true;

   }


   bool interaction::SetPlacement(const RECT & lpcrect, UINT nFlags)
   {

      return RepositionWindow(lpcrect.left, lpcrect.top, ::width(lpcrect), ::height(lpcrect), nFlags);

   }

   bool interaction::RepositionWindow(const RECT & lpcrect, UINT nFlags)
   {

      return SetPlacement(lpcrect, nFlags);

   }

   bool interaction::RepositionWindow(int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags)
   {

      return SetWindowPos(0, x, y, cx, cy, nFlags);

   }

   bool interaction::MoveWindow(int32_t x, int32_t y, UINT nFlags)
   {

      return SetWindowPos(0, x, y, 0, 0, nFlags | SWP_NOSIZE | SWP_NOZORDER);

   }

   bool interaction::MoveWindow(POINT pt, UINT nFlags)
   {

      return MoveWindow(pt.x, pt.y, nFlags);

   }

   bool interaction::SizeWindow(int32_t cx, int32_t cy, UINT nFlags)
   {

      return SetWindowPos(0, 0, 0, cx, cy, nFlags | SWP_NOMOVE | SWP_NOZORDER);

   }

   bool interaction::SizeWindow(SIZE sz, UINT nFlags)
   {

      return SizeWindow(sz.cx, sz.cy, nFlags);

   }

   bool interaction::ResizeWindow(int32_t cx, int32_t cy, UINT nFlags)
   {

      return SetWindowPos(0, 0, 0, cx, cy, nFlags | SWP_NOMOVE | SWP_NOZORDER);

   }

   bool interaction::ResizeWindow(SIZE sz, UINT nFlags)
   {

      return ResizeWindow(sz.cx, sz.cy, nFlags);

   }

   bool interaction::SetWindowPos(int_ptr z, const RECT & rect, UINT nFlags)
   {

      return SetWindowPos(z, rect.left, rect.top, ::width(rect), ::height(rect), nFlags);

   }

   bool interaction::defer_set_window_pos(int_ptr z, const RECT & rect, UINT nFlags)
   {

      return defer_set_window_pos(z, rect.left, rect.top, ::width(rect), ::height(rect), nFlags);

   }

   bool interaction::defer_set_window_pos(int_ptr z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags) // only set_windows_pos if GetParent()->ScreenToClient(get_window_rect) different of rect(x, y, cx, cy)
   {

      rect rectWindow;

      GetWindowRect(rectWindow);

      if (GetParent() != NULL)
      {

         GetParent()->ScreenToClient(rectWindow);

      }

      if (!(nFlags & SWP_NOMOVE))
      {

         if (rectWindow.left == x && rectWindow.top == y)
         {
            nFlags |= SWP_NOMOVE;

         }

      }

      if (!(nFlags & SWP_NOSIZE))
      {

         if (rectWindow.width() == cx && rectWindow.height() == cy)
         {

            nFlags |= SWP_NOSIZE;

         }

      }

      if ((nFlags & (SWP_NOSIZE | SWP_NOMOVE)) == (SWP_NOSIZE | SWP_NOMOVE))
      {

         if (nFlags & SWP_SHOWWINDOW)
         {

            if (is_this_visible())
            {

               return true;

            }

         }
         else if (nFlags & SWP_SHOWWINDOW)
         {

            if (!is_this_visible())
            {

               return true;

            }

         }
         else
         {

            return true;

         }

      }

      bool bOk = SetWindowPos(z, x, y, cx, cy, nFlags);

      return bOk;

   }




   void interaction::install_message_routing(::message::sender * pinterface)
   {

      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &interaction::_001OnCreate);

      if (m_bMessageWindow)
      {

         //IGUI_MSG_LINK(WM_DESTROY              , pinterface, this, &interaction::_001OnDestroyMessageWindow);

      }
      else
      {

         IGUI_MSG_LINK(WM_CLOSE, pinterface, this, &interaction::_001OnClose);
         IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &interaction::_001OnDestroy);
         IGUI_MSG_LINK(WM_SIZE, pinterface, this, &interaction::_001OnSize);
         IGUI_MSG_LINK(WM_MOVE, pinterface, this, &interaction::_001OnMove);
         IGUI_MSG_LINK(WM_NCCALCSIZE, pinterface, this, &interaction::_001OnNcCalcSize);
         IGUI_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &interaction::_001OnShowWindow);
         IGUI_MSG_LINK(WM_SETFOCUS, pinterface, this, &interaction::_001OnSetFocus);
      }  IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &interaction::_001OnLButtonDown);


      IGUI_MSG_LINK(WM_COMMAND, pinterface, this, &interaction::_001OnCommand);
      IGUI_MSG_LINK(message_simple_command, pinterface, this, &interaction::_001OnSimpleCommand);

   }


   void interaction::_001OnNcCalcSize(::message::message * pobj)
   {

      pobj->m_bRet = true; // avoid any Microsoft-Window-concept-of-non-client-area

   }


   void interaction::_001OnDestroy(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      user_interaction_on_destroy();

      pobj->previous();

   }


   void interaction::user_interaction_on_hide()
   {


      //if(System.get_active_ui() == this)
      //{
      //   // TODO : to be possibly done by camilo : instead of setting active guie to null
      //   // you could relinquish (I don't know what it is it) the active window status
      //   // to another window by using a function like kill_focus or relinquish_focus.
      //   // But, if you set active guie to NULL, Session could ignore the NULL assignment,
      //   // and use as a tip to set to a proper alive windows/guie.
      //   System.set_active_guie(NULL);
      //}

      //if(Session.get_focus_ui() == this)
      //{

      //   System.set_focus_guie(NULL);

      //}


      if(get_wnd()->get_focus_elemental()== this)
      {

         get_wnd()->set_focus_elemental(NULL);

      }

      try
      {

         for (index i = 0; i < m_uiptraChild.get_size(); i++)
         {

            try
            {

               if (m_uiptraChild[i].is_set())
               {

                  m_uiptraChild[i]->send_message(WM_SHOWWINDOW, 0, 1);

               }

            }
            catch (...)
            {

            }

         }

      }
      catch (...)
      {

      }

      try
      {

         for (index i = 0; i < m_uiptraOwned.get_size(); i++)
         {

            try
            {

               if (::is_set(m_uiptraOwned[i]))
               {

                  m_uiptraOwned[i]->ShowWindow(SW_HIDE);

               }

            }
            catch (...)
            {

            }

         }

      }
      catch (...)
      {

      }


      try
      {

         if (m_papp != NULL && m_papp->m_psession != NULL)
         {

            ::user::interaction * puiCapture = m_papp->m_psession->GetCapture();

            if (puiCapture != NULL && puiCapture == this)
            {

               ReleaseCapture();

            }

         }

      }
      catch (...)
      {

      }


      try
      {

         mouse_hover_remove(this);

      }
      catch (...)
      {

      }

      try
      {

         if (m_papp != NULL && m_papp->m_psession != NULL && has_focus())
         {

            if (GetParent() == NULL || !IsWindowVisible())
            {

               m_papp->m_psession->set_keyboard_focus(NULL);

            }
            else
            {

               keyboard_set_focus_next();

            }

         }

      }
      catch (...)
      {

      }

      if (GetCapture() == this)
      {

         SetCapture();

      }


      if (m_papp != NULL && &System != NULL && System.get_active_ui() == this)
      {

         ::set_active_window(NULL);

      }


      if (Application.m_puiActive == this)
      {

         Application.m_puiActive = NULL;

      }


      ::user::interaction * puiParent = GetParent();

      if (puiParent != NULL)
      {

         try
         {

            puiParent->on_hide_child(this);

         }
         catch (...)
         {

         }

      }

      {

         synch_lock sl(Session.m_pmutex);

         try
         {

            if (Session.m_puiLastUserInputPopup == this)
            {

               Session.m_puiLastUserInputPopup = NULL;

            }

         }
         catch (...)
         {

         }

      }

   }


   void interaction::_001GetXScrollInfo(scroll_info & info)
   {

      size sizeTotal = get_total_size();
      size sizePage = get_page_size();
      point ptOffset = get_viewport_offset();

      info.nMin = 0;
      info.nMax = sizeTotal.cx;
      info.nPage = sizePage.cx;
      info.nPos = ptOffset.x;
      info.nTrackPos = ptOffset.x;
   }

   void interaction::_001GetYScrollInfo(scroll_info & info)
   {
      size sizeTotal = get_total_size();
      size sizePage = get_page_size();
      point ptOffset = get_viewport_offset();

      info.nMin = 0;
      info.nMax = sizeTotal.cy;
      info.nPage = sizePage.cy;
      info.nPos = ptOffset.y;
      info.nTrackPos = ptOffset.y;
   }


   void interaction::layout_scroll_bar()
   {

   }


   void interaction::on_remove_child(::user::interaction * pui)
   {

   }


   void interaction::on_remove_place_holder_child(::user::interaction * pui)
   {

   }


   void interaction::on_hide_child(::user::interaction * pui)
   {

   }


   void interaction::on_hide_place_holder_child(::user::interaction * pui)
   {

   }


   void interaction::user_interaction_on_destroy()
   {

      user_interaction_on_hide();

      if (Application.m_puiMain == this)
      {

         Application.m_puiMain = NULL;

      }

      m_bUserElementalOk = false;

      post_quit();

      single_lock slDraw(get_wnd() == NULL || get_wnd()->m_pimpl.is_null()
                         || get_wnd()->m_pimpl.cast < ::user::interaction_impl >() == NULL ? NULL : get_wnd()->m_pimpl.cast < ::user::interaction_impl >()->draw_mutex(), true);

      try
      {

         if (m_pimpl != NULL)
         {

            m_pimpl->m_ptimerarray.release();

         }

      }
      catch (...)
      {

      }

      try
      {

         ShowWindow(SW_HIDE);

      }
      catch (...)
      {

      }

      pointer_array <  ::user::interaction * > uiptra;

      {

         synch_lock sl(m_pmutex);

         m_uiptraChild.slice(uiptra);

      }

      for (int32_t i = 0; i < uiptra.get_count(); i++)
      {

         ::user::interaction * pui = uiptra[i];

         try
         {

            pui->DestroyWindow();

         }
         catch (...)
         {

         }

      }

      children_post_quit_and_wait(one_minute());

      release_parents();

      if (m_papp == NULL)
      {

         return;

      }


   }


   void interaction::_001OnSize(::message::message * pobj)
   {

      SCAST_PTR(::message::size, psize, pobj);

      pobj->previous();

      if (psize->m_nType == SIZE_MINIMIZED)
      {

         TRACE("::user::interaction::_001OnSize SIZE_MINIMIZED - ignoring event");

      }
      else if (m_pimpl->m_bIgnoreSizeEvent)
      {

         TRACE("::user::interaction::_001OnSize instructed to m_bIgnoreSizeEvent");

      }

   }


   void interaction::layout()
   {

      single_lock sl(m_pmutex, true);

      if (m_pimpl->m_pui == NULL)
      {

         return;

      }

      m_pimpl->on_layout();

      on_layout();

   }


   bool interaction::is_frame_window()
   {

      return false;

   }


   void interaction::translate()
   {

      m_pimpl->on_translate();

      single_lock sl(m_pmutex, true);

      on_translate();

   }


   void interaction::do_show_flags()
   {

      //output_debug_string("\ninteraction::do_show_flags " + string(typeid(*this).name()));

      m_pimpl->on_do_show_flags();

      single_lock sl(m_pmutex, true);

      on_do_show_flags();

   }

   void interaction::zorder(int iZorder, int nFlags)
   {

      SetWindowPos(iZorder, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | nFlags);

   }

   void interaction::do_zorder()
   {

      if (GetParent() == NULL)
      {

         m_pimpl->on_zorder();

         return;

      }

      single_lock sl(GetParent()->m_pmutex);

      int_ptr z = m_pimpl->m_iZ;

      if (z == ZORDER_TOP || z == ZORDER_TOPMOST)
      {

         if (GetParent()->m_uiptraChild.find_first(this) != GetParent()->m_uiptraChild.get_upper_bound())
         {

            try
            {

               GetParent()->m_uiptraChild.remove(this);

               GetParent()->m_uiptraChild.add_unique(this);

            }
            catch (...)
            {

            }

         }

      }
      else if (z == ZORDER_BOTTOM)
      {

         if (GetParent()->m_uiptraChild.find_first(this) != 0)
         {

            try
            {

               GetParent()->m_uiptraChild.remove(this);

               GetParent()->m_uiptraChild.insert_at(0, this);

            }
            catch (...)
            {

            }

         }

      }

   }


   void interaction::_001OnMove(::message::message * pobj)
   {

      pobj->previous();

      //      layout_tooltip();



      //synch_lock sl(m_pmutex);

      //for(index i = 0; i < m_uiptraChild.get_size(); i++)
      //{
      //
      //   sp(::user::interaction) pui = m_uiptraChild[i];
      //
      //   sl.unlock();

      //   pui->send_message(WM_MOVE);
      //
      //   sl.lock();

      //}

   }


   void interaction::set_viewport_org(::draw2d::graphics * pgraphics)
   {

      if (m_pimpl == NULL)
         return;

      m_pimpl->set_viewport_org(pgraphics);

      /*      rect64 rectWindow;
      GetWindowRect(rectWindow);
      get_wnd()->ScreenToClient(rectWindow);
      pgraphics->SetViewportOrg(point(rectWindow.top_left()));
      pgraphics->SelectClipRgn(NULL);
      */

   }


   void interaction::_001OnClip(::draw2d::graphics * pgraphics)
   {

      //return;



      try
      {

         rect rectClip;

         ::aura::draw_context * pdrawcontext = pgraphics->::core::simple_chain < ::aura::draw_context >::get_last();

         rect rectClient;

         bool bFirst = true;

         if (pdrawcontext != NULL)
         {

            rectClient = pdrawcontext->m_rectWindow;

            ScreenToClient(rectClient);

            rectClient.bottom++;
            rectClient.right++;

            rectClip = rectClient;

            bFirst = false;

         }

         ::user::interaction * pui = this;

         ::rect rectFocus;

         ::rect rectIntersect;

         index i = 0;

         while (pui != NULL)
         {

            pui->GetClientRect(rectFocus);

            pui->ClientToScreen(rectFocus);

            ScreenToClient(rectFocus);

            //rectFocus.bottom++;
            //rectFocus.right++;

            if (i == 0)
            {

               rectIntersect = rectFocus;

            }
            else
            {

               rectIntersect.intersect(rectFocus);

            }

            i++;

            pui = pui->GetParent();

         }

         pgraphics->IntersectClipRect(rectIntersect);

      }
      catch (...)
      {

         _throw(simple_exception(get_app(), "no more a window"));

      }

   }


   void interaction::_001DrawThis(::draw2d::graphics * pgraphics)
   {

      if (pgraphics == NULL)
      {

         _throw(invalid_argument_exception(get_app()));

      }

      ::draw2d::savedc k(pgraphics);

      pgraphics->m_pui = this;

      try
      {

         if (!is_custom_draw() && pgraphics != NULL && pgraphics->m_pnext == NULL)
         {

            set_viewport_org(pgraphics);

         }

         pgraphics->m_dFontFactor = 1.0;

         //try
         //{

         //   pgraphics->SelectClipRgn(NULL);

         //}
         //catch(...)
         //{

         //   _throw(simple_exception(get_app(), "no more a window"));

         //}

         draw_select dl(this, pgraphics);

         {

            synch_lock sl(m_pmutex);

            _001OnNcDraw(pgraphics);

         }

         _001OnClip(pgraphics);

         _001CallOnDraw(pgraphics);


      }
      catch (...)
      {

         TRACE("Exception: interaction::_001DrawThis %s", typeid(*this).name());

      }

      if (m_pparent != NULL)
      {

         on_after_graphical_update();

      }

   }

   void interaction::_001CallOnDraw(::draw2d::graphics * pgraphics)
   {

      on_viewport_offset(pgraphics);

      synch_lock sl(m_pmutex);

      {

         draw_select ds(this, pgraphics);

         _001OnDraw(pgraphics);

      }

   }


   void interaction::_008CallOnDraw(::draw2d::graphics * pgraphics)
   {

      ::draw2d::savedc k(pgraphics);

      try
      {

         set_viewport_org(pgraphics);

         synch_lock sl(m_pmutex);

         _008OnDraw(pgraphics);

      }
      catch (...)
      {

      }

   }


   void interaction::on_viewport_offset(::draw2d::graphics * pgraphics)
   {

      point ptViewportOffset = get_viewport_offset();

      pgraphics->OffsetViewportOrg(-ptViewportOffset.x, -ptViewportOffset.y);

   }


   void interaction::_001DrawChildren(::draw2d::graphics * pgraphics)
   {

      ::draw2d::savedc k(pgraphics);

      // while drawing layout can occur and change z-order.
      // keep this past z-order
      interaction_spa uia = m_uiptraChild;

      for (auto & pui : uia)
      {

         try
         {

            ::draw2d::savedc keep(pgraphics);

            if (pui.is_set() && !pui->is_custom_draw())
            {

               pui->_000OnDraw(pgraphics);

            }

         }
         catch (...)
         {

            TRACE("\n\nException thrown while drawing user::interaction\n\n");

         }

      }

   }


   void interaction::_001Print(::draw2d::graphics * pgraphics)
   {

      windowing_output_debug_string("\n_001Print A");

      //pgraphics->fill_solid_rect_dim(100, 100, 400, 120, ARGB(255, 255, 255, 0));

      {

         ::draw2d::savedc k(pgraphics);

         try
         {

            _001OnDeferPaintLayeredWindowBackground(pgraphics);

         }
         catch (...)
         {

         }

      }

      windowing_output_debug_string("\n_001Print B");

      {

         ::draw2d::savedc k(pgraphics);

         try
         {

            _000OnDraw(pgraphics);

         }
         catch (...)
         {

         }

      }

      windowing_output_debug_string("\n_001Print B");

      if (&Session != NULL && Session.m_bDrawCursor)
      {

         {

            ::draw2d::savedc k(pgraphics);

            try
            {

               point ptCursor;

               Session.get_cursor_pos(&ptCursor);

               ScreenToClient(&ptCursor);

               ::visual::cursor * pcursor = Session.get_cursor();

               if (pcursor != NULL && pgraphics != NULL)
               {

                  pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

                  pcursor->to(pgraphics, ptCursor);

               }

            }
            catch (...)
            {

            }

         }

      }

   }


   bool interaction::is_this_visible()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      if (!m_bUserElementalOk)
      {

         return false;

      }

      if ((m_pimpl->m_bShowFlags && m_pimpl->m_iShowFlags & SWP_HIDEWINDOW)
            || (m_pimpl->m_bShowWindow && m_pimpl->m_iShowWindow == SW_HIDE))
      {

         //set_need_redraw();

         return false;

      }

      if ((m_pimpl->m_bShowFlags && m_pimpl->m_iShowFlags & SWP_SHOWWINDOW)
            || (m_pimpl->m_bShowWindow && m_pimpl->m_iShowWindow != SW_HIDE))
      {

         //set_need_redraw();

         return true;

      }

      return ::user::interaction_base::is_this_visible();

   }


   string interaction::calc_window_class()
   {

#ifdef WINDOWSEX

      uint32_t uiStyle = get_window_default_style();

      string strIcon = get_window_icon_matter();

      if (strIcon.has_char())
      {

         return windows_calc_icon_window_class(this, uiStyle, strIcon);

      }
      else
      {

         return windows_get_user_interaction_window_class(this);

      }

#else

      return "";

#endif

   }



   void interaction::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      defer_check_layout();

      defer_check_zorder();

      if (!is_this_visible())
         return;

      try
      {

         _001DrawThis(pgraphics);

      }
      catch (...)
      {

         TRACE("Exception: interaction::_000OnDraw _001DrawThis %s", typeid(*this).name());

      }

      if (m_uiptraChild.has_elements())
      {

         try
         {

            _001DrawChildren(pgraphics);

         }
         catch (...)
         {

            TRACE("Exception: interaction::_000OnDraw _001DrawChildren %s", typeid(*this).name());

         }

      }

      try
      {

         _008CallOnDraw(pgraphics);

      }
      catch (...)
      {

         TRACE("Exception: interaction::_000OnDraw _008CallOnDraw %s", typeid(*this).name());

      }

   }


   void interaction::_001OnNcDraw(::draw2d::graphics * pgraphics)
   {

      if (m_flagNonClient.is_signalized(non_client_background))
      {

         draw_control_background(pgraphics);

      }

   }


   void interaction::_001OnDraw(::draw2d::graphics * pgraphics)
   {


   }

   void interaction::_008OnDraw(::draw2d::graphics * pgraphics)
   {

   }


   void interaction::draw_control_background(::draw2d::graphics * pgraphics)
   {

      ::aura::draw_context * pdrawcontext = pgraphics->::core::simple_chain < ::aura::draw_context >::get_last();

      rect rectClient;

      if (pdrawcontext != NULL)
      {

         rectClient = pdrawcontext->m_rectClient;

      }
      else
      {

         GetClientRect(rectClient);

      }

      if (_001IsBackgroundBypass())
      {

      }
      else if (_001IsTranslucent())
      {

         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

         COLORREF cr = _001GetColor(::user::color_background, ARGB(128, 255, 255, 255));

         if (cr != 0)
         {

            pgraphics->fill_solid_rect(rectClient, cr);

         }

      }
      else
      {

         pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

         pgraphics->fill_solid_rect(rectClient, _001GetColor(m_ecolorBackground, ARGB(200, 255, 255,255)));

      }

   }

   bool interaction::is_custom_draw()
   {

      return false;

   }


   bool interaction::set_prodevian(bool bSet)
   {

      m_bProDevian = bSet;

      return true;

   }


   void interaction::defer_start_prodevian()
   {

#if defined(WINDOWSEX) //|| defined(MACOS) //|| defined(LINUX)

      if (this == System.m_psystemwindow)
      {

         return;

      }

#endif

      if (m_pimpl.is_set())
      {

         m_pimpl->defer_start_prodevian();

      }

   }


   void interaction::_001OnCreate(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      m_bUserElementalOk = true;

      if (m_papp == NULL)
      {

         _throw(simple_exception(get_app(), "m_papp cannot be null"));

      }


      {

         //m_papp->add(this);
         ::user::interaction * puiSystem = NULL;

#if !defined(LINUX) && !defined(METROWIN) && !defined(APPLEOS) && !defined(VSNORD)
         puiSystem = dynamic_cast < ::user::interaction * > (System.m_psystemwindow);
#endif

         if ((GetParent() == NULL
#if defined(METROWIN) || defined(VSNORD)
               || GetParent() == System.m_possystemwindow->m_pui
#endif
             ) && !is_message_only_window() &&  puiSystem != this
            )
         {

            synch_lock sl(m_pmutex);

            if (m_papp->m_psystem != NULL)
            {

               System.add_frame(this);

            }

            if (m_papp->m_psession != NULL)
            {

               Session.add_frame(this);

            }

            if (m_papp != NULL)
            {

               Application.add_frame(this);

            }

         }

      }

      defer_create_mutex();

      try
      {
         if ((GetParent() != NULL
#if defined(METROWIN) || defined(VSNORD)
               && GetParent() != System.m_possystemwindow->m_pui
#endif
             )
               && !is_message_only_window()
#if !defined(LINUX) && !defined(METROWIN) && !defined(APPLEOS) && !defined(VSNORD)
               && (::user::interaction *) System.m_psystemwindow != this
#endif
            )
         {

            sp(place_holder) pholder = GetParent();

            if (pholder.is_set())
            {

               // A Copy Paste error (the commented out code below)?
               //single_lock sl(puiParent->m_pmutex,TRUE);
               //single_lock sl2(m_pmutex,TRUE);

               if (!pholder->is_holding(this))
               {

                  pholder->hold(this);

               }

            }

         }

      }
      catch (...)
      {

      }

   }


   void interaction::_000OnDrag(::message::drag_and_drop * pdrag)
   {

#ifdef WINDOWS

      if (pdrag->m_id != MESSAGE_OLE_DRAGLEAVE)
      {

         try
         {

            if (!IsWindowVisible())
            {

               return;

            }

            if (!_001IsPointInside(point(pdrag->pt.x, pdrag->pt.y)))
            {

               return;

            }

         }
         catch (...)
         {
            return;
         }

      }

      // these try catchs are needed for multi threading : multi threaded windows: the hell
      // Now I understand why Microsoft (TM) Windows (R) windows are single threaded.

      sp(::user::interaction) pui = top_child();

      //      int32_t iSize;

      try
      {

         while (pui != NULL)
         {

            try
            {

               if (pui->IsWindowVisible() && (pdrag->m_id == MESSAGE_OLE_DRAGLEAVE || pui->_001IsPointInside(point(pdrag->pt.x, pdrag->pt.y))))
               {

                  try
                  {

                     pui->_000OnDrag(pdrag);

                     if (pdrag->m_bRet)
                     {

                        return;

                     }

                  }
                  catch (...)
                  {

                  }

               }

               pui = pui->under_sibling();

            }
            catch (...)
            {

            }

         }

      }
      catch (...)
      {

      }

      try
      {

         if (m_pimpl == NULL)
         {

            return;

         }

         route_message(pdrag);

         if (pdrag->m_bRet)
         {

            return;

         }

      }
      catch (...)
      {

      }

#endif

   }

   double interaction::get_rotate()
   {

      return 0.0;

   }


   void interaction::_000OnMouse(::message::mouse * pmouse)
   {

      bool bThisCapture = false;

      try
      {

         if (!is_this_visible()) // assume parent visibility already checked
         {

            return;

         }

         bThisCapture = Session.GetCapture() == this;

         if (!bThisCapture && !is_descendant(Session.GetCapture()) && !_001IsPointInside(pmouse->m_pt))
         {

            return;

         }

      }
      catch (...)
      {

         return;

      }

      if (bThisCapture)
      {

         _000OnThisMouse(pmouse);

         if (pmouse->m_bRet)
         {

            return;

         }

         _000OnChildrenMouse(pmouse);

      }
      else
      {

         _000OnChildrenMouse(pmouse);

         if (pmouse->m_bRet)
         {

            return;

         }

         _000OnThisMouse(pmouse);

      }

   }


   void interaction::_000OnThisMouse(::message::mouse * pmouse)
   {

      try
      {

         if (m_pimpl == NULL)
         {

            return;

         }

         m_pimpl->route_message(pmouse);

         if (pmouse->get_lresult() != 0)
         {

            return;

         }

      }
      catch (...)
      {

      }

   }


   void interaction::_000OnChildrenMouse(::message::mouse * pmouse)
   {

      // these try catchs are needed for multi threading : multi threaded windows: the hell
      // Now I understand why many OSes windows are single threaded.
      sp(::user::interaction) pui;

      try
      {
         while (rget_child(pui))
         {
            try
            {
               if (pui.is_null())
               {
                  continue;
               }
               if (pui->IsWindowVisible() && pui->_001IsPointInside(pmouse->m_pt))
               {
                  try
                  {
                     pui->_000OnMouse(pmouse);
                     if (pmouse->m_bRet)
                        return;
                  }
                  catch (...)
                  {
                  }
               }
            }
            catch (...)
            {
            }
         }
      }
      catch (...)
      {
      }

   }


   void interaction::_000OnKey(::message::key * pkey)
   {

      if (!(pkey->m_uiMessageFlags & 1)) // message already pre translated
      {

         pre_translate_message(pkey);

         if(pkey->m_bRet)
         {

            return;

         }

         pkey->m_uiMessageFlags |= 1;

      }

      point ptCursor;

      Session.get_cursor_pos(&ptCursor);

      if (!pkey->m_bRet)
      {

         // these try catchs are needed for multi threading : multi threaded windows: the hell
         // Now I understand why Microsoft (TM) Windows (R) windows are single threaded.

         ::user::interaction * pui = top_child();

         //         int32_t iSize;

         try
         {

            while (pui != NULL)
            {

               try
               {

                  if (pui->IsWindowVisible())
                  {

                     try
                     {

                        pui->_000OnKey(pkey);

                        if (pkey->m_bRet)
                        {

                           return;

                        }

                     }
                     catch (...)
                     {

                     }

                  }

                  pui = pui->under_sibling();

               }
               catch (...)
               {

               }

            }

         }
         catch (...)
         {

         }

         try
         {

            m_pimpl->route_message(pkey);

            if (pkey->get_lresult() != 0)
            {

               return;

            }

         }
         catch (...)
         {

         }

      }

   }



   ::user::interaction * interaction::_001FromPoint(point64 pt, bool bTestedIfParentVisible)
   {

      if (bTestedIfParentVisible)
      {

         if (!is_this_visible() || !_001IsPointInside(pt)) // inline version - do not need pointer to the function
         {

            return NULL;

         }

      }
      else
      {

         if (!IsWindowVisible() || !_001IsPointInside(pt)) // inline version - do not need pointer to the function
         {

            return NULL;

         }

      }

      sp(interaction) pui = top_child();

      while (pui != NULL)
      {

         sp(interaction) puie = pui->_001FromPoint(pt, true);

         if (puie != NULL)
         {

            return puie;

         }

         pui = pui->under_sibling();

      }

      return this;

   }


   bool interaction::_001IsPointInside(point64 pt)
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      if (m_pimpl != get_wnd())
      {

         if (get_wnd()->WfiIsIconic() || !IsWindowVisible())
         {

            return false;

         }

      }

      rect64 rect;

      GetWindowRect(rect);

      //int iLeft = rect.left;
      //int iTop = rect.top;
      //int iRight = rect.right;
      //int iBottom = rect.bottom;

      return rect.contains(pt);

   }


   void interaction::_001OnKeyDown(::message::message * pobj)
   {
      //if(Session.get_keyboard_focus() != this
      //   && Session.get_keyboard_focus() != NULL)
      //{
      // Session.get_keyboard_focus()->keyboard_focus_OnKeyDown(pobj);
      //}
   }

   void interaction::_001OnKeyUp(::message::message * pobj)
   {
      //if(Session.get_keyboard_focus() != this
      ///&& Session.get_keyboard_focus() != NULL)
      //{
      // Session.get_keyboard_focus()->keyboard_focus_OnKeyUp(pobj);
      //}
   }

   void interaction::_001OnChar(::message::message * pobj)
   {
      //if(Session.get_keyboard_focus() != this
      // && Session.get_keyboard_focus() != NULL)
      //{
      // Session.get_keyboard_focus()->keyboard_focus_OnChar(pobj);
      //}
   }


   void interaction::_001OnTimer(::timer * ptimer)
   {

   }


   bool interaction::on_timer(::timer * ptimer)
   {

      _001OnTimer(ptimer);

      return true;

   }


   ::user::interaction * interaction::get_child_by_name(const char * pszName, int32_t iLevel)
   {
      sp(interaction) pui = top_child();
      while (pui != NULL)
      {
         if (pui->m_strName == pszName)
         {
            return pui;
         }
         pui = pui->under_sibling();
      }
      sp(interaction) pchild;
      if (iLevel > 0 || iLevel == -1)
      {
         if (iLevel > 0)
         {
            iLevel--;
         }
         sp(interaction) pui = top_child();
         while (pui != NULL)
         {
            pchild = pui->get_child_by_name(pszName, iLevel);
            if (pchild != NULL)
               return pchild;
            pui = pui->under_sibling();
         }
      }
      return NULL;
   }


   ::user::interaction * interaction::get_child_by_id(id id, int32_t iLevel)
   {
      interaction * pui = top_child();
      while (pui != NULL)
      {
         if (pui->m_id == id)
         {
            return pui;
         }
         pui = pui->under_sibling();
      }
      interaction * pchild;
      if (iLevel > 0 || iLevel == -1)
      {
         if (iLevel > 0)
         {
            iLevel--;
         }
         interaction * pui = top_child();
         while (pui != NULL)
         {
            pchild = pui->get_child_by_id(id, iLevel);
            if (pchild != NULL)
               return pchild;
            pui = pui->under_sibling();
         }
      }
      return NULL;
   }



   void interaction::_002OnLButtonDown(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnLButtonUp(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnMouseMove(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnMouseEnter(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnMouseLeave(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnKeyDown(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnKeyUp(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_002OnTimer(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   // pbase object should be allocated with new in
   // base or derived object and will be delete after
   // handling
   LRESULT interaction::send(::message::base * pbase)
   {

      if(m_pimpl.is_set())
      {

         m_pimpl->message_handler(pbase);

      }

      return pbase->get_lresult();

   }


   // pbase object should be allocated with new in
   // base or derived object and will be delete after
   // handling
   bool interaction::post(::message::base * pbase)
   {

      if(pbase->m_id == WM_KEYDOWN)
      {

         output_debug_string("::user::interaction::post WM_KEYDOWN");

      }

      return post_message(message_post_user, 1, pbase);

   }


   LRESULT interaction::send_message(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      if (m_pimpl == NULL)
      {

         return 0;

      }

      return m_pimpl->send_message(uiMessage, wparam, lparam);

   }


   LRESULT interaction::message_call(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      if (m_pimpl == NULL)
      {

         return 0;

      }

      return m_pimpl->message_call(uiMessage, wparam, lparam);

   }


#ifdef LINUX

   LRESULT interaction::send_x11_event(void * pevent)
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->send_x11_event(pevent);
   }

#endif


   bool interaction::IsWindowVisible()
   {

      if (is_null(this))
      {

         return false;

      }

      if (!IsWindow())
      {

         return false;

      }

      if (m_bHideOnTransparentFrame)
      {

         ::user::interaction * puiTopLevel = GetTopLevel();

         if (puiTopLevel != NULL)
         {

            if (puiTopLevel->frame_is_transparent())
            {

               return false;

            }

         }

      }

      if (GetParent() != NULL)
      {

         if (!GetParent()->IsWindowVisible())
         {

            return false;

         }

      }

      if(WfiIsIconic())
      {

         return false;

      }

      return m_pimpl->IsWindowVisible();

   }


   bool interaction::enable_window(bool bEnable)
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->enable_window(bEnable);

   }


   bool interaction::ShowWindow(int32_t nCmdShow)
   {

      if (m_pimpl.is_set())
      {

         m_pimpl->ShowWindow(nCmdShow);

      }
      else
      {

         set_need_redraw();

      }

      return IsWindowVisible();

   }


   bool interaction::is_window_enabled()
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->is_window_enabled();

   }


   void interaction::send_message_to_descendants(UINT message, WPARAM wparam, lparam lparam, bool bDeep, bool bOnlyPerm)
   {

      if (m_pimpl == NULL)
      {

         return;

      }

      return m_pimpl->send_message_to_descendants(message, wparam, lparam, bDeep, bOnlyPerm);

   }


   void interaction::route_message_to_descendants(::message::message * pmessage)
   {

      if (m_pimpl == NULL)
      {

         return;

      }

      interaction_spa uiptraChild;

      {

         synch_lock sl(m_pmutex);

         uiptraChild = m_uiptraChild;

      }

      for (auto & puiChild : uiptraChild)
      {

         try
         {

            puiChild->route_message_to_descendants(pmessage);

         }
         catch (...)
         {

         }

      }

   }


   void interaction::pre_translate_message(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      //SCAST_PTR(::message::base, pbase, pobj);

      //if(pbase->m_id == WM_KEYDOWN)
      //{

      //   SCAST_PTR(::message::key,pkey,pobj);

      //   if(pkey->m_ekey == ::user::key_tab)
      //   {

      //      ::user::control_event ev;

      //      ev.m_puie         = this;

      //      ev.m_eevent       = ::user::event_tab_key;

      //      ev.m_actioncontext        = ::action::source_user;

      //      if(!on_control_event(&ev))
      //      {

      //         sp(::user::interaction) pui = Session.get_keyboard_focus();

      //         pui =  pui.is_set() ? pui->keyboard_get_next_focusable() : keyboard_get_next_focusable();

      //         if(pui != NULL)
      //         {

      //            if (!pui->does_consume_key(::user::key_tab))
      //            {

      //               pui->keyboard_set_focus();

      //               pkey->m_bRet = true;

      //            }

      //         }

      //      }

      //   }

      //}


   }


   oswindow interaction::get_handle() const
   {

#if defined(METROWIN)

      sp(::user::interaction) pwnd = NULL;

      try
      {

         pwnd = get_wnd();

         if (pwnd == NULL)
            return NULL;

         return oswindow_get(pwnd->m_pimpl.cast < ::user::interaction_impl >());

      }
      catch (...)
      {

      }

      return NULL;

#else

      ::user::interaction * pui;

      ::user::interaction_impl * pwnd;

      try
      {

         pui = get_wnd();

         if (pui == NULL)
            return NULL;

         if (pui->m_pimpl == NULL)
            return NULL;

         pwnd = pui->m_pimpl->get_user_interaction_impl();

         if (pwnd == NULL)
            return NULL;

         return pwnd->get_handle();

      }
      catch (...)
      {

      }

      return NULL;

#endif


   }



   bool interaction::subclass_window(oswindow posdata)
   {

      synch_lock sl(m_pmutex);

      if (IsWindow())
      {

         DestroyWindow();

      }

      remove_all_routes();

      sp(interaction_impl_base) pimplOld = m_pimpl;

      ::window_sp pimplNew = NULL;

      pimplNew = (Application.alloc(System.type_info < interaction_impl >()));

      pimplNew->m_pui = this;

      if (!pimplNew->subclass_window(posdata))
      {

         pimplNew.release();

      }

      if (pimplNew != NULL)
      {

         if (pimplOld != NULL)
         {

            pimplOld->m_pui = NULL;

            pimplOld->remove_all_routes();

            ::window_sp pwindowOld = pimplOld;

            if (pwindowOld != NULL)
            {

               pwindowOld->install_message_routing(pimplOld);

            }

            pimplOld.release();

         }

         m_pimpl = pimplNew;

         return true;

      }
      else
      {

         return false;

      }

   }

   oswindow interaction::unsubclass_window()
   {

      ::window_sp pwindow = m_pimpl;

      if (pwindow != NULL)
      {

         return pwindow->unsubclass_window();
      }
      return NULL;
   }

#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)

   bool interaction::initialize_native_window(::user::native_window_initialize * pinitialize)
   {

      if (IsWindow())
      {

         DestroyWindow();

      }

      //      m_signalptra.remove_all();

      m_pimpl = (Application.alloc(System.type_info < interaction_impl >()));

      m_pimpl->m_pui = this;

      if (!m_pimpl->initialize_native_window(pinitialize))
      {

         m_pimpl.release();

         return false;

      }

      return true;

   }


#endif


   bool interaction::create_window(const RECT & rect, ::user::interaction * puiParent, id id)
   {

      if (!create_window(
            NULL,
            NULL,
            WS_VISIBLE | WS_CHILD,
            rect,
            puiParent,
            id))
      {

         TRACE("Failed to create control");

         return false;

      }

      return true;

   }


   bool interaction::create_window(const char * lpszClassName, const char * lpszWindowName, uint32_t dwStyle, const RECT & rectParam, ::user::interaction * pParentWnd, id id, ::create * pcreate)
   {

      if (IsWindow())
      {

         DestroyWindow();

      }

      m_pthreadUserInteraction = ::get_thread();

      if (m_pthreadUserInteraction.is_set())
      {

         m_pthreadUserInteraction->uiptra().add(this);

      }

      try
      {

         m_bUserElementalOk = true;

         remove_all_routes();

         sp(interaction_impl_base) pimplNew;

         ::rect rect(rectParam);

         if (rect.area() <= 0)
         {

            if (pParentWnd != NULL && dynamic_cast <::user::place_holder *> (pParentWnd) != NULL)
            {

               pParentWnd->GetClientRect(rect);

            }

         }

#if defined(APPLE_IOS) || defined(VSNORD) || defined(METROWIN)

         if (pParentWnd == NULL || pParentWnd == System.m_possystemwindow->m_pui)
         {

            pParentWnd = System.m_possystemwindow->m_pui;

         }

#endif

#if defined(WINDOWSEX) || defined(LINUX)

         if (pParentWnd == NULL || pParentWnd->get_safe_handle() == (oswindow)MESSAGE_WINDOW_PARENT)

#else

         if (pParentWnd == NULL)

#endif
         {

            pimplNew = (Application.alloc(System.type_info < interaction_impl >()));

            if (!pimplNew->create_window(this, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, id, pcreate))
            {

               m_pimpl.release();

               pimplNew.release();

            }

         }
         else
         {

            pimplNew = canew(::user::interaction_child(get_app()));

            if (!pimplNew->create_window(this, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, id, pcreate))
            {

               pimplNew.release();

               m_bUserElementalOk = false;

               if (m_pthreadUserInteraction.is_set()
                     && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
               {

                  m_pthreadUserInteraction->m_puiptraThread->remove(this);

               }

               m_pthreadUserInteraction.release();

               return false;

            }

         }

      }
      catch (...)
      {

         m_bUserElementalOk = false;

         if (m_pthreadUserInteraction.is_set()
               && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
         {

            m_pthreadUserInteraction->m_puiptraThread->remove(this);

         }

         m_pthreadUserInteraction.release();

         return false;

      }

      return true;

   }


   bool interaction::create_window_ex(::user::create_struct & cs, ::user::interaction * puiParent, id id)
   {

      m_pthreadUserInteraction = ::get_thread();

      if (m_pthreadUserInteraction.is_set())
      {

         m_pthreadUserInteraction->uiptra().add(this);

      }

      cs.hMenu = NULL;

      if(puiParent != NULL)
      {

         cs.hwndParent = puiParent->get_safe_handle();

      }

      cs.hInstance = System.m_hinstance;

      if (!pre_create_window(cs))
      {

         PostNcDestroy();

         if (m_pthreadUserInteraction.is_set()
               && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
         {

            m_pthreadUserInteraction->m_puiptraThread->remove(this);

         }

         m_pthreadUserInteraction.release();

         return false;

      }

      if (IsWindow())
      {

         DestroyWindow();

      }

      try
      {

         m_bUserElementalOk = true;

         remove_all_routes();

         //::thread * pthread = ::get_thread();

         //if(pthread != NULL)
         //{

         //   m_threadptra.add(pthread);

         //}

#if defined(APPLE_IOS) || defined(VSNORD) || defined(METROWIN)

         if (puiParent == NULL || puiParent == System.m_possystemwindow->m_pui)
         {

            puiParent = System.m_possystemwindow->m_pui;

            //         if(!Application.defer_initialize_twf())
            //         {
            //
            ////            m_threadptra.remove_all();
            //
            //            return false;
            //
            //         }

         }

#endif

         //if(m_threadptra.get_count() <= 0)
         //{

         //   m_threadptra.add(get_app());

         //}
#if !defined(METROWIN) && !defined(VSNORD) && !defined(APPLE_IOS)

         if ((WS_CHILD & cs.style) == 0)
         {

            m_pimpl = Application.alloc(System.type_info < interaction_impl >());

            //dwStyle &= ~WS_CHILD;

            if (!m_pimpl->create_window_ex(this, cs, puiParent, id))
            {

               m_bUserElementalOk = false;

               m_pimpl.release();

               if (m_pthreadUserInteraction.is_set()
                     && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
               {

                  m_pthreadUserInteraction->m_puiptraThread->remove(this);

               }

               m_pthreadUserInteraction.release();

               return false;

            }

            return true;

         }
         else
#endif
         {
#if defined(METROWIN) || defined(APPLE_IOS) || defined(VSNORD)

            if (puiParent == NULL)
            {

               puiParent = System.m_possystemwindow->m_pui;

            }

#endif

            ::rect rect;

            cs.get_rect(rect);

            ::rect rectFrame(0, 0, 0, 0);

            if (IsRectEmpty(&rect))
            {

               if (rectFrame.is_null() && puiParent->is_place_holder())
               {

                  puiParent->GetClientRect(rectFrame);

               }

            }
            else
            {

               rectFrame = rect;

            }

            //synch_lock sl(pParentWnd == NULL ? NULL : pParentWnd->m_pmutex);

            m_pimpl = canew(::user::interaction_child(get_app()));

            m_pimpl->m_pui = this;

            if (!m_pimpl->create_window_ex(this, cs, puiParent, id))
            {

               m_bUserElementalOk = false;

               m_pimpl.release();

               if (m_pthreadUserInteraction.is_set()
                     && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
               {

                  m_pthreadUserInteraction->m_puiptraThread->remove(this);

               }

               m_pthreadUserInteraction.release();

               return false;

            }

            //install_message_routing(this);

            return true;

         }

      }
      catch (...)
      {

         m_bUserElementalOk = false;

         if (m_pthreadUserInteraction.is_set()
               && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
         {

            m_pthreadUserInteraction->m_puiptraThread->remove(this);

         }

         m_pthreadUserInteraction.release();

         return false;

      }

      return true;

   }


   void interaction::BringToTop(int nCmdShow)
   {

      if (m_pimpl != NULL)
         return m_pimpl->BringToTop(nCmdShow);

   }


   bool interaction::BringWindowToTop()
   {

      if (m_pimpl == NULL)
         return false;
      else
         return m_pimpl->BringWindowToTop();

   }


   bool interaction::IsWindow() const
   {

      if (is_null(this))
      {

         return false;

      }

      if (m_pimpl.is_null())
      {

         return false;

      }

      if (GetParent() != NULL)
      {

         if (!GetParent()->IsWindow())
         {

            return false;

         }

      }

      return m_pimpl->IsWindow();

   }


   LONG interaction::get_window_long(int32_t nIndex) const
   {
      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->get_window_long(nIndex);
   }

   LONG interaction::set_window_long(int32_t nIndex, LONG lValue)
   {
      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->set_window_long(nIndex, lValue);
   }

   LONG_PTR interaction::get_window_long_ptr(int32_t nIndex) const
   {
      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->get_window_long_ptr(nIndex);
   }

   LONG_PTR interaction::set_window_long_ptr(int32_t nIndex, LONG_PTR lValue)
   {
      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->set_window_long_ptr(nIndex, lValue);
   }


   bool interaction::RedrawWindow(LPCRECT lpRectUpdate, ::draw2d::region* prgnUpdate, UINT flags)
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->RedrawWindow(lpRectUpdate, prgnUpdate, flags);

   }


   ::user::interaction * interaction::ChildWindowFromPoint(POINT point)
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->ChildWindowFromPoint(point);
   }

   ::user::interaction * interaction::ChildWindowFromPoint(POINT point, UINT nFlags)
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->ChildWindowFromPoint(point, nFlags);
   }

   ::user::interaction * interaction::get_next_window(UINT nFlag)
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->get_next_window(nFlag);
   }

   ::user::interaction * interaction::get_next(bool bIgnoreChildren, int32_t * piLevel)
   {

      if (!bIgnoreChildren)
      {
         if (m_uiptraChild.has_elements())
         {
            if (piLevel != NULL)
               (*piLevel)++;
            return m_uiptraChild[0];
         }
      }
      if (GetParent() == NULL)
      {
         // todo, reached desktop or similar very top system interaction_impl
         return NULL;
      }

      synch_lock slParent(GetParent()->m_pmutex);
      synch_lock sl(m_pmutex);


      index iFind = GetParent()->m_uiptraChild.find_first(this);

      if (iFind < 0)
         _throw(simple_exception(get_app(), "not expected situation"));

      if (iFind < GetParent()->m_uiptraChild.get_upper_bound())
      {
         return GetParent()->m_uiptraChild[iFind + 1];
      }

      if (GetParent()->GetParent() == NULL)
      {
         // todo, reached desktop or similar very top system interaction_impl
         return NULL;
      }

      if (piLevel != NULL)
         (*piLevel)--;

      return GetParent()->GetParent()->get_next(true, piLevel);

   }

   bool interaction::is_message_only_window() const
   {

      return m_bMessageWindow;

   }

   ::user::interaction * interaction::GetTopWindow()
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetTopWindow();
   }

   ::user::interaction * interaction::get_wnd(UINT nCmd) const
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->get_wnd(nCmd);
   }

   ::user::interaction * interaction::GetActiveWindow()
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetActiveWindow();
   }


   bool interaction::SetFocus()
   {

      if (m_pimpl == NULL)
      {

         return false;

      }
      else
      {

         return m_pimpl->SetFocus();

      }

   }


   ::user::interaction * interaction::GetFocus()
   {

      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetFocus();

   }



   ::user::interaction * interaction::SetActiveWindow()
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->SetActiveWindow();
   }

   bool interaction::SetForegroundWindow()
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->SetForegroundWindow();
   }

   ::user::interaction * interaction::GetLastActivePopup()
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetLastActivePopup();
   }


   void interaction::set_window_text(const char * lpszString)
   {

      m_strWindowText = lpszString;

      if (m_pimpl != NULL)
      {

         m_pimpl->set_window_text(lpszString);

      }

      set_need_redraw();

   }


   strsize interaction::get_window_text(char * lpszStringBuf, strsize nMaxCount)
   {

      strsize n = MIN(nMaxCount, m_strWindowText.get_length());

      strncpy(lpszStringBuf, m_strWindowText, n);

      return n;

   }


   string interaction::get_window_text()
   {

      string str;

      get_window_text(str);

      return str;

   }


   void interaction::get_window_text(string & rString)
   {

      synch_lock sl(m_pmutex);

      rString = m_strWindowText;

   }


   strsize interaction::get_window_text_length()
   {

      synch_lock sl(m_pmutex);

      if (m_pimpl == NULL)
      {

         return 0;

      }

      return m_pimpl->get_window_text_length();

   }


//   void interaction::_001SetText(const string & str, ::action::context actioncontext)
//   {
//
//      UNREFERENCED_PARAMETER(actioncontext);
//
//      set_window_text(str);
//
//   }


   ::user::interaction * interaction::GetParentTopLevel() const
   {

      sp(::user::interaction) puiParent = GetParent();

      sp(::user::interaction) puiParentTopLevel;

      if (puiParent.is_null())
         return NULL;

      do
      {

         puiParentTopLevel = puiParent;

         puiParent = puiParent->GetParent();

      }
      while (puiParent.is_set());

      return puiParentTopLevel;

   }


   ::user::interaction * interaction::EnsureParentTopLevel()
   {

      sp(interaction) pwindow = GetParentTopLevel();

      ENSURE_VALID(pwindow);

      return pwindow;

   }


   ::user::interaction * interaction::GetTopLevel() const
   {

      ::user::interaction * puiParent = (::user::interaction *) this;

      ::user::interaction * puiTopLevelParent;

      if (puiParent == NULL)
         return NULL;

      do
      {

         puiTopLevelParent = puiParent;

         puiParent = puiParent->GetParent();

      }
      while (puiParent != NULL);

      return puiTopLevelParent;

   }


   ::user::interaction * interaction::EnsureTopLevel()
   {

      sp(interaction) pwindow = GetTopLevel();

      ENSURE_VALID(pwindow);

      return pwindow;

   }

   ::user::frame * interaction::GetFrame() const
   {

      ::user::interaction * pui = (::user::interaction *) this;

      if (pui == NULL)
      {

         return NULL;

      }

      ::user::frame * pframeParent;

      do
      {

         pframeParent = dynamic_cast <::user::frame *> (pui);

         if (pframeParent != NULL)
            return pframeParent;

         pui = pui->GetParent();

      }
      while (pui != NULL);

      return NULL;

   }


   ::user::frame * interaction::GetParentFrame() const
   {

      ::user::interaction * pui = GetParent();

      if (pui == NULL)
         return NULL;

      return pui->GetFrame();

   }


   ::user::frame * interaction::GetTopLevelFrame() const
   {

      sp(::user::frame) pframe = GetFrame();

      if (pframe.is_null())
         return NULL;

      sp(::user::frame) pframeTopLevel;

      do
      {

         pframeTopLevel = pframe;

         pframe = pframe->GetParentFrame();

      }
      while (pframe.is_set());

      return pframeTopLevel;

   }


   ::user::frame * interaction::GetParentTopLevelFrame() const
   {

      sp(::user::frame) pframe = GetParentFrame();

      if (pframe.is_null())
         return NULL;

      return pframe->GetTopLevelFrame();

   }


   ::user::frame * interaction::EnsureParentFrame()
   {

      ::user::frame * pFrameWnd = GetParentFrame();

      ENSURE_VALID(pFrameWnd);

      return pFrameWnd;

   }


   LRESULT interaction::Default()
   {

      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->Default();

   }


   uint32_t interaction::GetStyle() const
   {

      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->GetStyle();

   }


   uint32_t interaction::GetExStyle() const
   {

      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->GetExStyle();

   }


   bool interaction::ModifyStyle(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {

      if (m_pimpl == NULL)
         return false;

      return m_pimpl->ModifyStyle(dwRemove, dwAdd, nFlags);

   }


   bool interaction::ModifyStyleEx(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags)
   {

      if (m_pimpl == NULL)
         return false;

      return m_pimpl->ModifyStyleEx(dwRemove, dwAdd, nFlags);

   }


   bool interaction::DestroyWindow()
   {

      m_bUserElementalOk = false;

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->DestroyWindow();

   }


   void interaction::post_quit()
   {

//      try
//      {
//
//         DestroyWindow();
//
//      }
//      catch (...)
//      {
//
//      }

      ::user::interaction_base::post_quit();

   }


   // for custom cleanup after WM_NCDESTROY
   void interaction::PostNcDestroy()
   {

      sp(::user::interaction) pui = this;

      sp(::user::interaction_impl_base) pimpl = m_pimpl;

      {

         synch_lock sl(m_pmutex);

         try
         {

            remove_all_routes();

         }
         catch (...)
         {

         }

      }

      ::user::interaction * puiParent = GetParent();

      if (puiParent != NULL)
      {

         try
         {

            single_lock sl(puiParent->m_pmutex, true);

            if (puiParent->m_bUserElementalOk)
            {

               puiParent->on_remove_child(this);

               puiParent->m_uiptraChild.remove(this);

            }
            else
            {

               TRACE("Parent being destroyed");

            }

         }
         catch (...)
         {

         }

      }
      else if (!is_message_only_window())
      {

         if (get_app() != NULL)
         {

            try
            {

               Application.remove_frame(this); // guess this may be a frame, it doesn't hurt to remove if this is not there

            }
            catch (...)
            {

            }

            if (get_app()->m_psession != NULL)
            {

               try
               {

                  Session.remove_frame(this); // guess this may be a frame, it doesn't hurt to remove if this is not there

               }
               catch (...)
               {

               }

            }

            if (get_app()->m_psystem != NULL)
            {

               try
               {

                  System.remove_frame(this); // guess this may be a frame, it doesn't hurt to remove if this is not there

               }
               catch (...)
               {

               }

            }

         }

      }

      pimpl.release();

   }


   void interaction::CalcWindowRect(LPRECT lprect, UINT nAdjustType)
   {
      if (m_pimpl == NULL)
         return;
      else
         return m_pimpl->CalcWindowRect(lprect, nAdjustType);
   }


   void interaction::RepositionBars(UINT nIDFirst, UINT nIDLast, id nIDLeftOver,
                                    UINT nFlag, LPRECT lpRectParam,
                                    LPCRECT lpRectClient, bool bStretch)
   {
      if (m_pimpl == NULL)
         return;
      else
         return m_pimpl->RepositionBars(nIDFirst, nIDLast, nIDLeftOver, nFlag, lpRectParam, lpRectClient, bStretch);
   }


   ::user::interaction * interaction::SetOwner(::user::interaction * pui)
   {

      if (m_pimpl == NULL)
         return NULL;

      if (m_puiOwner != NULL)
      {

         try
         {

            m_puiOwner->m_uiptraOwned.remove(this);

         }
         catch (...)
         {

         }

      }

      m_puiOwner = pui;

      pui->m_uiptraOwned.add(this);

      ::user::interaction * puiRet = m_pimpl->SetOwner(pui);

      return puiRet;

   }


   ::user::interaction * interaction::GetOwner() const
   {

      if (m_puiOwner != NULL)
      {

         return m_puiOwner;

      }

      if (m_pimpl == NULL)
         return NULL;

      return m_pimpl->GetOwner();

   }

   ::user::frame * interaction::GetOwnerFrame() const
   {

      ::user::interaction * pui = GetOwner();

      if (pui == NULL)
      {

         return NULL;

      }

      ::user::frame * pframe = dynamic_cast < ::user::frame * > (pui);

      while (pui != NULL)
      {

         if (pframe != NULL)
         {

            return pframe;

         }

         pframe = pui->GetParentFrame();

         if (pframe != NULL)
         {

            return pframe;

         }

         pui = pui->GetOwner();

         if (pui == NULL)
         {

            break;

         }

         pframe = dynamic_cast < ::user::frame * > (pui);

      }

      return NULL;

   }


   ::user::interaction * interaction::GetParentOwner() const
   {

      sp(::user::interaction) puiParent = GetParent();

      if (puiParent.is_null())
      {

         return NULL;

      }

      return puiParent->GetOwner();

   }


   ::user::interaction * interaction::GetParentOrOwner() const
   {

      sp(::user::interaction) puiParent = GetParent();

      if (puiParent.is_set())
      {

         return puiParent;

      }

      return GetOwner();

   }


   ::user::interaction * interaction::GetTopLevelOwner() const
   {

      sp(::user::interaction) puiOwner = GetOwner();

      sp(::user::interaction) puiTopLevelOwner;

      if (puiOwner.is_null())
         return NULL;

      do
      {

         puiTopLevelOwner = puiOwner;

         puiOwner = puiOwner->GetParent();

      }
      while (puiOwner.is_set());

      return puiTopLevelOwner;

   }




   interaction * interaction::GetDescendantWindow(id iId) const
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->GetDescendantWindow(iId);
   }

   void interaction::viewport_client_to_screen(POINT * ppt)
   {

      m_pimpl->viewport_client_to_screen(ppt);

   }

   bool interaction::IsNotifyIconEnabled()
   {

      return false;

   }

   void interaction::viewport_screen_to_client(POINT * ppt)
   {

      if (m_pimpl.is_null())
         return;

      m_pimpl->viewport_screen_to_client(ppt);

   }


   void interaction::viewport_client_to_screen(RECT * prect)
   {

      viewport_client_to_screen((POINT *)&prect->left);
      viewport_client_to_screen((POINT *)&prect->right);

   }


   void interaction::viewport_screen_to_client(RECT * prect)
   {

      viewport_screen_to_client((POINT *)&prect->left);
      viewport_screen_to_client((POINT *)&prect->right);

   }




   //int32_t interaction::SetWindowRgn(HRGN hRgn,bool bRedraw)
   //{

   //   if(m_pimpl == NULL)
   //      return 0;
   //   else
   //      return m_pimpl->SetWindowRgn(hRgn,bRedraw);

   //}


   //int32_t interaction::GetWindowRgn(HRGN hRgn)
   //{
   //   if(m_pimpl == NULL)
   //      return 0;
   //   else
   //      return m_pimpl->GetWindowRgn(hRgn);
   //}


   bool interaction::WfiIsZoomed()
   {
      if (m_pimpl == NULL)
         return false;
      else
         return m_pimpl->WfiIsZoomed();
   }


   bool interaction::WfiIsFullScreen()
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->WfiIsFullScreen();

   }


   bool interaction::WfiIsIconic()
   {
      if (m_pimpl == NULL)
         return false;
      else
         return m_pimpl->WfiIsIconic();
   }


   bool interaction::CheckAutoCenter()
   {
      if (m_pimpl == NULL)
         return TRUE;
      else
         return m_pimpl->CheckAutoCenter();
   }

   void interaction::CenterWindow(::user::interaction * pAlternateOwner)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->CenterWindow(pAlternateOwner);
   }


   LRESULT interaction::DefWindowProc(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->DefWindowProc(uiMessage, wparam, lparam);

   }


   void interaction::message_handler(::message::base * pbase)
   {

      auto psystemwindow = System.m_possystemwindow;

      if (psystemwindow == NULL || ::get_system_window_interaction(psystemwindow) != this)
      {

         if (!(pbase->m_uiMessageFlags & 2)) // message already pre translated
         {

            Application.pre_translate_message(pbase);

            if (pbase->m_bRet)
            {

               return;

            }

         }

         if (!(pbase->m_uiMessageFlags & 1)) // message already pre translated
         {

            pre_translate_message(pbase);

            if (pbase->m_bRet)
            {

               return;

            }

            pbase->m_uiMessageFlags |= 1;

         }

      }


      if (m_pimpl == NULL)
      {

         return;

      }

      interaction_impl_base * pimpl = m_pimpl;

      pimpl->message_handler(pbase);

   }


   LRESULT interaction::message_handler(LPMESSAGE lpmessage)
   {

      return send_message(lpmessage->message, lpmessage->wParam, lpmessage->lParam);

   }

#ifdef WINDOWSEX

   bool interaction::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->GetWindowPlacement(lpwndpl);
   }

   bool interaction::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->SetWindowPlacement(lpwndpl);
   }

#endif


   bool interaction::pre_create_window(::user::create_struct& cs)
   {

      return true;

   }


   bool interaction::IsTopParentActive()
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->IsTopParentActive();
   }

   void interaction::ActivateTopParent()
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->ActivateTopParent();
   }


   void interaction::update_dialog_controls(command_target * pTarget)
   {

      if (m_pimpl.is_null())
      {

         return;

      }

      m_pimpl->update_dialog_controls(pTarget);

   }


   void interaction::UpdateWindow()
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->UpdateWindow();
   }

   void interaction::register_drop_target()
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->register_drop_target();
   }

   void interaction::SetRedraw(bool bRedraw)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->SetRedraw(bRedraw);
   }

   bool interaction::GetUpdateRect(LPRECT lpRect, bool bErase)
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->GetUpdateRect(lpRect, bErase);
   }

   int32_t interaction::GetUpdateRgn(::draw2d::region* pRgn, bool bErase)
   {
      if (m_pimpl == NULL)
         return 0;
      else
         return m_pimpl->GetUpdateRgn(pRgn, bErase);

   }

   void interaction::Invalidate(bool bErase)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->Invalidate(bErase);
   }

   void interaction::InvalidateRect(LPCRECT lpRect, bool bErase)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->InvalidateRect(lpRect, bErase);
   }

   void interaction::InvalidateRgn(::draw2d::region* pRgn, bool bErase)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->InvalidateRgn(pRgn, bErase);
   }

   void interaction::ValidateRect(LPCRECT lpRect)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->ValidateRect(lpRect);
   }
   void interaction::ValidateRgn(::draw2d::region* pRgn)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->ValidateRgn(pRgn);
   }


   void interaction::on_layout()
   {

      layout_tooltip();

      on_change_view_size();

   }


   void interaction::on_set_window_pos()
   {

   }


   void interaction::on_translate()
   {

      layout_tooltip();

   }


   void interaction::on_do_show_flags()
   {

      layout_tooltip();

   }



   void interaction::ShowOwnedPopups(bool bShow)
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->ShowOwnedPopups(bShow);
   }

   void * interaction::get_os_data() const
   {

      return get_handle();

   }

   bool interaction::attach(oswindow oswindow_New)
   {
      if (m_pimpl == NULL)
         return FALSE;
      else
         return m_pimpl->attach(oswindow_New);
   }

   oswindow interaction::detach()
   {
      if (m_pimpl == NULL)
         return NULL;
      else
         return m_pimpl->detach();
   }

   void interaction::pre_subclass_window()
   {
      if (m_pimpl == NULL)
         return;
      else
         m_pimpl->pre_subclass_window();
   }


   void interaction::_001Emphasize(int cx, int cy)
   {

      ::rect rectMainMonitor;

      System.get_main_monitor(rectMainMonitor);

      int x = (rectMainMonitor.width() - cx) / 2;

      int y = (rectMainMonitor.height() - cy) / 3;

      SetWindowPos(ZORDER_TOP, x, y, cx, cy, SWP_SHOWWINDOW);

      BringToTop(SW_SHOW);

   }


   id interaction::run_modal_loop(::user::interaction * pui, uint32_t dwFlags)
   {

      return pui->_001RunModalLoop(dwFlags);

   }


   id interaction::RunModalLoop(uint32_t dwFlags)
   {

      set_need_redraw();

      return _001RunModalLoop(dwFlags);

   }


   id interaction::_001RunModalLoop(uint32_t dwFlags)
   {

      ASSERT(!m_bModal);

      if(m_bModal)
      {

         return ::id();

      }

      keep < bool > keepModal(&m_bModal, true, false, true);

      keep < thread * > keepThreadModal(&m_pthreadModal, ::get_thread(), NULL, true);

      while(true)
      {

         if (!ContinueModal())
         {

            break;

         }


         if(::is_null(get_thread()))
         {

            break;

         }

         if(!::get_thread()->defer_pump_message())
         {

            break;

         }

      }

      //System.post_to_all_threads(WM_KICKIDLE, 0, 0);

      return m_idModalResult;

   }


   bool interaction::ContinueModal()
   {

      return m_bModal && ::get_thread_run();

   }


   void interaction::EndModalLoop(id nResult)
   {

      ASSERT(IsWindow());

      // this result will be returned from interaction_impl::RunModalLoop
      m_idModalResult = nResult;

      // make sure a message goes through to exit the modal loop
      m_bModal = false;

      if(::is_set(m_pthreadModal))
      {

         m_pthreadModal->kick_thread();

      }

   }


   void interaction::on_control_event(control_event * pevent)
   {

      ::user::interaction * pui = get_bind_ui();

      if (pui != NULL)
      {

         pui->on_control_event(pevent);

         if (pevent->m_bRet)
         {

            return;

         }

      }

      ::user::interaction * puiParent = GetParent();

      if (puiParent != NULL)
      {

         puiParent->on_control_event(pevent);

      }
      else
      {

         ::user::interaction * puiOwner = GetOwner();

         if (puiOwner != NULL)
         {

            puiOwner->on_control_event(pevent);

         }
         else
         {

            if (m_papp != NULL)
            {

               Application.on_control_event(pevent);

               if (pevent->m_bRet)
               {

                  return;

               }

            }

         }

      }

   }


   bool interaction::post_message(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      if(uiMessage == WM_KEYDOWN)
      {

         output_debug_string("::user::interaction::post_message WM_KEYDOWN");

      }

      return m_pimpl->post_message(uiMessage, wparam, lparam);

   }


   bool interaction::post_object(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      bool bIsWindow = m_pimpl.is_set() && IsWindow();

      if (uiMessage == WM_QUIT || !bIsWindow)
      {

         {

            // discards object
            sp(::object) spo(lparam);

         }

         if (!bIsWindow)
         {

            return false;

         }

         return m_pimpl->post_message(uiMessage);

      }

      return m_pimpl->post_message(uiMessage, wparam, lparam);

   }


   // timer Functions
   bool interaction::SetTimer(uint_ptr nIDEvent, UINT nElapse, PFN_TIMER pfnTimer)
   {

      if (m_pimpl == NULL)
         return false;

      return m_pimpl->SetTimer(nIDEvent, nElapse, pfnTimer);

   }


   bool interaction::KillTimer(uint_ptr nIDEvent)
   {

      if (m_pimpl == NULL)
         return false;

      return m_pimpl->KillTimer(nIDEvent);

   }


   bool interaction::has_focus()
   {

      if(m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->has_focus();

   }


   bool interaction::is_active()
   {

      if(m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->is_active();

   }


   bool interaction::SetCapture(::user::interaction * pui)
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      if (pui == NULL)
      {

         pui = this;

      }

      return m_pimpl->SetCapture(pui);

   }


   ::user::interaction * interaction::GetCapture()
   {

      if (m_pimpl != NULL)
      {

         return m_pimpl->GetCapture();

      }

      ::user::interaction * pwnd = get_wnd();

      if (pwnd != NULL)
      {

         return pwnd->GetCapture();

      }

      return NULL;

   }


   bool interaction::ReleaseCapture()
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->ReleaseCapture();

   }


   void interaction::track_mouse_leave()
   {

      track_mouse_hover();

   }


   void interaction::track_mouse_hover()
   {

      ::user::interaction * pui = GetTopLevel();

      if (pui == NULL)
         return;

      if (pui->get_wnd() == NULL)
         return;

      pui->get_wnd()->mouse_hover_add(this);

   }


   void interaction::_001UpdateWindow(bool bUpdateBuffer)
   {

      if (m_bLockWindowUpdate)
         return;

      if (m_pimpl == NULL)
         return;

      m_pimpl->_001UpdateWindow(bUpdateBuffer);

      on_after_graphical_update();

   }


   void interaction::_001WindowMinimize(bool bNoActivate)
   {

      m_pimpl->_001WindowMinimize(bNoActivate);

   }


   void interaction::_001WindowMaximize()
   {

      m_pimpl->_001WindowMaximize();

   }


   void interaction::_001WindowFullScreen(LPCRECT lpcrectHint)
   {

      m_pimpl->_001WindowFullScreen(lpcrectHint);

   }


   void interaction::_001WindowRestore()
   {

      m_pimpl->_001WindowRestore();

   }


   void interaction::_001WindowDock(::user::e_appearance eappearance)
   {

      m_pimpl->_001WindowDock(eappearance);

   }


   void interaction::GuieProc(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction::_001DeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics)
   {
      if (m_pimpl != NULL)
      {
         //         m_pimpl->_001DeferPaintLayeredWindowBackground(pgraphics);
      }
   }

   void interaction::_001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pgraphics)
   {

      _001DeferPaintLayeredWindowBackground(pgraphics);

   }







   void interaction::OnLinkClick(const char * psz, const char * pszTarget)
   {

      System.open_link(psz, "", pszTarget);

   }


   void interaction::on_set_parent(::user::interaction * puiParent)
   {

      if (m_pparent == puiParent)
      {

         return;

      }

      try
      {

         if (m_pparent != NULL)
         {

            single_lock sl(m_pparent->m_pmutex, TRUE);

            single_lock sl2(m_pmutex, TRUE);

            m_pparent->m_uiptraChild.remove(this);

         }

      }
      catch (...)
      {
      }

      m_pparent = puiParent;

      try
      {

         if (puiParent != NULL)
         {

            if (m_pthreadUserInteraction.is_set()
                  && ::is_set(m_pthreadUserInteraction->m_puiptraThread))
            {

               m_pthreadUserInteraction->m_puiptraThread->remove(this);

            }

            m_pthreadUserInteraction.release();

            single_lock sl(puiParent->m_pmutex, TRUE);

            single_lock sl2(m_pmutex, TRUE);

            puiParent->m_uiptraChild.add_unique(this);

         }

      }
      catch (...)
      {

      }

   }


   //bool interaction::SetTimer(uint_ptr nIDEvent,UINT nEllapse, PFN_TIMER pfnTimer)
   //{
   //
   //   return ::aura::timer_array::SetTimer(nIDEvent,nEllapse, pfnTimer);

   //}


   //bool interaction::KillTimer(uint_ptr nIDEvent)
   //{

   //   return ::aura::timer_array::KillTimer(nIDEvent);

   //}



   bool interaction::create_message_queue(const char * pszName)
   {

      if (IsWindow())
      {

         DestroyWindow();

      }

      try
      {

         m_bUserElementalOk = true;

         //m_signalptra.remove_all();

         remove_all_routes();

         m_pimpl = Application.alloc(System.type_info < interaction_impl >());

         if (m_pimpl == NULL)
         {

            m_bUserElementalOk = false;

            return false;

         }

         m_bMessageWindow = true;

         m_pimpl->m_pui = this;

         //::thread * pthread = ::get_thread();

         //if(pthread != NULL)
         //{

         //   m_threadptra.add(pthread);

         //}

         //if(m_threadptra.get_count() <= 0)
         //{

         //   m_threadptra.add(get_app());

         //}

         if (!m_pimpl->create_message_queue(this, pszName))
         {

            //m_threadptra.remove_all();

            m_bUserElementalOk = false;

            m_pimpl.release();

            return false;

         }

      }
      catch (...)
      {

         m_bUserElementalOk = false;

         return false;

      }

      return true;

   }



   void interaction::walk_pre_translate_tree(::message::message * pobj, ::user::interaction * puiStop)
   {

      if (puiStop == this)
      {

         return;

      }

      try
      {

         pre_translate_message(pobj);

      }
      catch (...)
      {

      }

      if (!m_bDefaultWalkPreTranslateParentTree)
         return;

      if (pobj->m_bRet)
         return;

      ::user::interaction * puiParent = GetParent();

      if (puiParent != NULL)
         return;

      if (puiParent == puiStop)
         return;

      try
      {

         puiParent->pre_translate_message(pobj);

      }
      catch (...)
      {

      }



   }

   void interaction::on_select()
   {
   }

   bool interaction::is_place_holder()
   {
      return false;
   }


   ::visual::e_cursor interaction::get_cursor()
   {

      return m_ecursor;

   }


   bool interaction::set_cursor(::visual::e_cursor ecursor)
   {

      m_ecursor = ecursor;

      return Session.set_cursor(this, m_ecursor);

   }


   void interaction::_001OnMouseEnter(::message::message * pobj)
   {

      pobj->m_bRet = false;

   }


   void interaction::_001OnMouseMove(::message::message * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      pmouse->m_ecursor = get_cursor();

   }


   void interaction::_001OnMouseLeave(::message::message * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      sp(interaction) pui = top_child();

      while (pui != NULL)
      {

         if (pui->m_bCursorInside)
         {

            pui->m_bCursorInside = false;

            pui->message_handler(pbase);

            pui->message_handler(pbase);

         }

         pui = pui->under_sibling();

      }

      pobj->m_bRet = false;

   }


   bool interaction::can_merge(::user::interaction * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return false;

   }


   bool interaction::merge(::user::interaction * pui)
   {

      UNREFERENCED_PARAMETER(pui);

      return false;

   }


   ::user::elemental * interaction::first_child_elemental()
   {

      return first_child();

   }


   ::user::elemental * interaction::top_elemental()
   {

      return interaction_base::top_elemental();

   }


   ::user::elemental * interaction::under_elemental()
   {

      return under_sibling();

   }


   ::user::elemental * interaction::above_elemental()
   {

      return above_sibling();

   }


   ::user::elemental * interaction::next_elemental()
   {

      return next_sibling();

   }


   ::user::elemental * interaction::previous_elemental()
   {

      return previous_sibling();

   }



   ::user::interaction * interaction::first_child()
   {

      try
      {
         if (m_uiptraChild.get_count() <= 0)
            return NULL;
         else
            return m_uiptraChild.first_sp();

      }
      catch (...)
      {
      }
      return NULL;
   }

   ::user::interaction * interaction::last_child()
   {
      try
      {
         if (m_uiptraChild.get_count() <= 0)
            return NULL;
         else
            return m_uiptraChild.last_sp();
      }
      catch (...)
      {
      }
      return NULL;
   }



   ::user::interaction * interaction::first_sibling()
   {
      try
      {

         if (GetParent() == NULL)
            return NULL;

         return GetParent()->first_child();

      }
      catch (...)
      {
      }
      return NULL;

   }


   ::user::interaction * interaction::next_sibling()
   {
      try
      {

         ::user::interaction * pui = NULL;
         try
         {
            pui = GetParent();
         }
         catch (...)
         {
            return NULL;
         }
         if (pui == NULL)
            return NULL;
         index i = pui->m_uiptraChild.find_first(this);
         if (i < 0)
            return NULL;
         i++;
         if (i >= pui->m_uiptraChild.get_count())
            return NULL;
         else
            return pui->m_uiptraChild[i];

      }
      catch (...)
      {
      }
      return NULL;


   }

   ::user::interaction * interaction::next_sibling(::user::interaction * pui)
   {
      synch_lock sl(m_pmutex);
      try
      {

         index i = m_uiptraChild.find_first(pui);
         if (i < 0)
            return NULL;
         i++;
restart:
         if (i >= m_uiptraChild.get_count())
            return NULL;
         else
         {
            try
            {
               return m_uiptraChild[i];
            }
            catch (...)
            {
               goto restart;
            }
         }
      }
      catch (...)
      {

      }

      return NULL;

   }

   ::user::interaction * interaction::previous_sibling()
   {

      try
      {

         ::user::interaction * pui = NULL;
         try
         {
            pui = GetParent();
         }
         catch (...)
         {
            return NULL;
         }
         if (pui == NULL)
            return NULL;
         index i = pui->m_uiptraChild.find_first(this);
         if (i < 0)
            return NULL;
         i--;
         if (i < 0)
            return NULL;
         else
            return pui->m_uiptraChild[i];

      }
      catch (...)
      {

      }

      return NULL;

   }

   ::user::interaction * interaction::last_sibling()
   {

      try
      {

         if (GetParent() == NULL)
            return NULL;

         return GetParent()->last_child();

      }
      catch (...)
      {

      }

      return NULL;


   }


   void interaction::mouse_hover_add(::user::interaction * pinterface)
   {

      if (m_pimpl == NULL)
         return;

      m_pimpl->mouse_hover_add(pinterface);

   }

   void interaction::mouse_hover_remove(::user::interaction * pinterface)
   {

      if (m_pimpl == NULL)
         return;

      m_pimpl->mouse_hover_remove(pinterface);

   }


   ::user::interaction * interaction::previous_sibling(::user::interaction * pui)
   {
      single_lock sl(m_pmutex, TRUE);
      index i = m_uiptraChild.find_first(pui);
      if (i < 0)
         return NULL;
restart:
      i--;
      if (i < 0)
         return NULL;
      else
      {
         try
         {
            return m_uiptraChild[i];
         }
         catch (...)
         {
            goto restart;
         }
      }
   }



   bool interaction::on_keyboard_focus(::user::elemental * pfocus)
   {

      if (m_pimpl == NULL)
         return true;


      return m_pimpl->on_keyboard_focus(pfocus);

   }


   bool interaction::keyboard_focus_OnKillFocus(oswindow oswindowNew)
   {

      send_message(WM_KILLFOCUS, (WPARAM) oswindowNew);

      if (m_pimpl == NULL)
      {

         return true;

      }

      return m_pimpl->keyboard_focus_OnKillFocus(oswindowNew);

   }


   bool interaction::keyboard_focus_OnChildKillFocus()
   {

      if (m_pimpl == NULL)
         return true;


      return m_pimpl->keyboard_focus_OnChildKillFocus();

   }


   ::user::interaction * interaction::get_os_focus_uie()
   {

      return NULL;

   }


   bool interaction::IsAscendant(const interaction * puiIsAscendant) const
   {

      return m_pimpl->IsAscendant(puiIsAscendant);

   }


   bool interaction::IsParent(const interaction * puiIsParent) const
   {

      return m_pimpl->IsParent(puiIsParent);

   }


   bool interaction::IsChild(const interaction * puiIsChild) const
   {

      return m_pimpl->IsChild(puiIsChild);

   }


   bool interaction::IsDescendant(const interaction * puiIsDescendant) const
   {

      return m_pimpl->IsDescendant(puiIsDescendant);

   }


   ::user::elemental * interaction::get_wnd_elemental() const
   {

      return get_wnd();

   }


   ::user::interaction * interaction::get_wnd() const
   {

      if (m_pimpl != NULL)
      {

         ::user::interaction_impl * pwnd = m_pimpl->get_user_interaction_impl();

         if (pwnd != NULL)
            return (::user::interaction *) this;

      }

      if (m_pparent == NULL)
         return NULL;

      return m_pparent->get_wnd();

   }


   // returns -1 if not descendant
   int32_t interaction::get_descendant_level(::user::interaction * pui)
   {
      int32_t iLevel = 0;
      while (pui != NULL)
      {
         if (pui == this)
            return iLevel;
         pui = pui->GetParent();
         iLevel++;
      }
      return -1;
   }

   bool interaction::is_descendant(::user::interaction * pui, bool bIncludeSelf)
   {
      if (bIncludeSelf)
      {
         return get_descendant_level(pui) >= 0;
      }
      else
      {
         return get_descendant_level(pui) > 0;
      }
   }

   ::user::interaction * interaction::get_focusable_descendant(::user::interaction * pui)
   {
      int32_t iLevel = 0;
      if (pui == NULL)
      {
         pui = this;
      }
      else if (pui != this)
      {
         iLevel = get_descendant_level(pui);
         if (iLevel < 0)
         {
            return NULL;
         }
      }
      sp(::user::interaction) puiFocusable = NULL;
      int32_t iPreviousLevel = iLevel;
      while (true)
      {
         iPreviousLevel = iLevel;
         pui = pui->get_next(false, &iLevel);
         if (iLevel == 0)
            break;
         if (iLevel <= iPreviousLevel && puiFocusable != NULL)
            break;
         if (pui == NULL)
            break;
         if (pui->keyboard_focus_is_focusable())
            puiFocusable = pui;

      }
      return puiFocusable;
   }


   void interaction::_001OnTriggerMouseInside()
   {

      if (m_pimpl != NULL)
      {

         m_pimpl->_001OnTriggerMouseInside();

      }

   }


   bool interaction::update_data(bool bSaveAndValidate)
   {

      return true;

   }


   void interaction::_001OnClose(::message::message * pobj)
   {

      pobj->m_bRet = true;

      ShowWindow(SW_HIDE);

      DestroyWindow();

   }


   /*
   #ifdef METROWIN

   #ifdef METROWIN

   Agile<Windows::UI::Core::CoreWindow> interaction::get_os_window()
   {
   if (m_pimpl == NULL)
   {
   return System.m_window;
   }
   else
   return m_pimpl->get_os_window();
   }

   #endif

   #endif
   */


   string interaction::get_window_default_matter()
   {

      return "";

   }

   string interaction::get_window_icon_matter()
   {

      return get_window_default_matter();

   }

   uint32_t interaction::get_window_default_style()
   {
      return 0;
   }

   ::user::interaction::e_type interaction::get_window_type()
   {
      return type_window;
   }


   //bool interaction::post_simple_command(e_simple_command ecommand,lparam lparam)
   //{

   //   post_message(message_simple_command,(WPARAM)ecommand,lparam);

   //   return true;

   //}

   void interaction::_001OnCommand(::message::message * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      LRESULT lresult = 0;

      pbase->m_bRet = OnCommand(pbase);

      pbase->set_lresult(lresult);

   }


   void interaction::_001OnSimpleCommand(::message::message * pmessage)
   {

      SCAST_MSG(simple_command);

      on_simple_command(psimple_command);

   }


   /*

      bool interaction::_001SetSchema(::user::style * pschema)
      {

         m_pschema = pschema;

         return true ;

      }


      bool interaction::_008SetSchema(::user::style * pschema)
      {

         send_message_to_descendants(message_set_schema,0,pschema,true,true);

         return true;

      }


      bool interaction::_009SetSchema(::user::style * pschema)
      {

         _001SetSchema(pschema);

         _008SetSchema(pschema);

         return true;

      }


   */


   /*

      void interaction::_001OnSetSchema(::message::message * pobj)
      {

         SCAST_PTR(::message::base,pbase,pobj);

         LRESULT lresult = 0;

         _001SetSchema((::user::style *) pbase->m_lparam);

         pbase->set_lresult(lresult);

      }

   */


   bool interaction::OnCommand(::message::base * pbase)
   {

      if (m_pimpl != NULL)
         return m_pimpl->OnCommand(pbase);

      return false;

   }

   bool interaction::OnNotify(::message::base * pbase)
   {

      if (m_pimpl != NULL)
         return m_pimpl->OnNotify(pbase);

      return false;

   }


   bool interaction::OnChildNotify(::message::base * pbase)
   {

      if (m_pimpl != NULL)
         return m_pimpl->OnChildNotify(pbase);

      return false;

   }


   void interaction::on_simple_command(::message::simple_command * psimplecommand)
   {

      switch (psimplecommand->m_esimplecommand)
      {
      case simple_command_layout:
      {

         on_layout();

         psimplecommand->m_bRet = true;

      }
      break;
      case simple_command_defer_start_prodevian:
      {

         m_pimpl->_defer_start_prodevian();


      }
      break;
      default:

         break;

      }

      if (psimplecommand->m_bRet)
      {

         return;

      }

      ::user::interaction_base::on_simple_command(psimplecommand);

   }


   void interaction::on_command(::user::command * pcommand)
   {

      ::user::interaction_base::on_command(pcommand);

   }


   bool interaction::is_selected(::data::item * pitem)
   {

      UNREFERENCED_PARAMETER(pitem);

      return false;

   }


   bool interaction::has_command_handler(::user::command * pcommand)
   {

      if (command_target::has_command_handler(pcommand))
      {

         return true;

      }

      return false;

   }


   bool interaction::track_popup_menu(::user::menu_item * pitem, int32_t iFlags)
   {

      point pt;

      Session.get_cursor_pos(&pt);

      return track_popup_menu(pitem, iFlags, pt);

   }


   bool interaction::track_popup_menu(::xml::node * lpnode, int32_t iFlags)
   {

      point pt;

      Session.get_cursor_pos(&pt);

      return track_popup_menu(lpnode, iFlags, pt);

   }


   bool interaction::track_popup_xml_matter_menu(const char * pszMatter, int32_t iFlags)
   {

      point pt;

      Session.get_cursor_pos(&pt);

      return track_popup_xml_matter_menu(pszMatter, iFlags, pt);

   }



   bool interaction::track_popup_menu(::user::menu_item * pitem, int32_t iFlags, ::message::message * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      return track_popup_menu(pitem, iFlags, pt);

   }


   bool interaction::track_popup_menu(::xml::node * lpnode, int32_t iFlags, ::message::message * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      ScreenToClient(&pt);

      return track_popup_menu(lpnode, iFlags, pt);

   }


   bool interaction::track_popup_xml_matter_menu(const char * pszMatter, int32_t iFlags, ::message::message * pobj)
   {

      SCAST_PTR(::message::mouse, pmouse, pobj);

      point pt = pmouse->m_pt;

      return track_popup_xml_matter_menu(pszMatter, iFlags, pt);

   }


   bool interaction::track_popup_menu(::user::menu_item * pitem, int32_t iFlags, POINT pt)
   {

      sp(::user::menu) pmenu = Application.alloc(System.type_info < ::user::menu >());

      pmenu->m_pitem = pitem;

      if (!pmenu->track_popup_menu(this, this))
      {

         pmenu.release();

         return false;

      }

      return true;

   }


   sp(::user::menu) interaction::track_popup_menu(::xml::node * lpnode, int32_t iFlags, POINT pt, size sizeMinimum)
   {

      sp(::user::menu) pmenu = Application.alloc(System.type_info < ::user::menu >());

      pmenu->m_sizeMinimum = sizeMinimum;

      if (!pmenu->load_menu(lpnode))
      {

         pmenu.release();

         return pmenu;

      }

      pmenu->hints(iFlags, pt);

      if (!pmenu->track_popup_menu(this, this))
      {

         pmenu.release();

         return pmenu;

      }

      return pmenu;


   }


   bool interaction::track_popup_xml_matter_menu(const char * pszMatter, int32_t iFlags, POINT pt)
   {

      if (get_thread()->m_bTemporary)
      {

         string strMatter(pszMatter);

         Application.post_pred([=]()
         {

            track_popup_xml_matter_menu(strMatter, iFlags, pt);

         });

         return true;

      }

      sp(::user::menu) pmenu = Application.alloc(System.type_info < ::user::menu >());

      string strMatter(pszMatter);

      if (!strMatter.begins_ci("matter://"))
      {

         strMatter = "matter://" + strMatter;

      }

      if (!pmenu->load_xml_menu(strMatter))
      {

         pmenu.release();

         return false;

      }

      pmenu->hints(iFlags, pt);

      if (!pmenu->track_popup_menu(this))
      {

         pmenu.release();

         return false;

      }

      return true;

   }


   sp(::user::menu) interaction::track_popup_xml_string_menu(const char * pszString, int32_t iFlags, POINT pt, size sizeMinimum)
   {

      sp(::user::menu) pmenu = Application.alloc(System.type_info < ::user::menu >());

      pmenu->m_sizeMinimum = sizeMinimum;

      if (!pmenu->load_xml_string_menu(pszString))
      {

         pmenu.release();

         return pmenu;

      }

      pmenu->hints(iFlags, pt);

      if (!pmenu->track_popup_menu(this))
      {

         pmenu.release();

         return pmenu;

      }

      return pmenu;

   }


   void interaction::WfiEnableFullScreen(bool bEnable)
   {

   }


   bool interaction::WfiIsFullScreenEnabled()
   {

      return false;

   }


   bool interaction::WfiClose()
   {

      return false;

   }


   bool interaction::Wfi(e_appearance eapperance)
   {

      return false;

   }


   bool interaction::WfiDock(e_appearance eapperance)
   {

      return false;

   }

   bool interaction::WfiRestore(bool bForceNormal)
   {

      UNREFERENCED_PARAMETER(bForceNormal);

      return false;

   }


   bool interaction::WfiMinimize()
   {

      return false;

   }


   bool interaction::WfiMaximize()
   {

      return false;

   }


   bool interaction::WfiFullScreen(LPCRECT lpcrectHint)
   {

      if(m_pimpl.is_null())
      {

         return false;

      }

      m_pimpl->_001WindowFullScreen(lpcrectHint);

      return true;

   }


   bool interaction::WfiUp()
   {

      return false;

   }


   bool interaction::WfiDown()
   {

      return false;

   }


   bool interaction::WfiNotifyIcon()
   {

      return false;

   }


   bool interaction::WfiIsMoving()
   {

      return false;

   }


   bool interaction::WfiIsSizing()
   {

      return false;

   }



   e_appearance interaction::get_appearance()
   {

      return m_eappearance;

   }


   bool interaction::set_appearance(e_appearance eappearance)
   {

      if (get_appearance() != get_appearance_before())
      {

         set_appearance_before(get_appearance());

      }

      m_eappearance = eappearance;

      if(eappearance == m_eappearanceRequest)
      {

         m_eappearanceRequest = ::user::appearance_none;

      }

      return true;

   }


   bool interaction::set_appearance_before(e_appearance eappearance)
   {

      m_eappearanceBefore = eappearance;

      return true;

   }


   e_appearance interaction::get_appearance_before()
   {

      return m_eappearanceBefore;

   }


   void interaction::show_keyboard(bool bShow)
   {

      if (m_pimpl == NULL)
         return;

      m_pimpl->show_keyboard(bShow);

   }


   LRESULT interaction::call_message_handler(UINT message, WPARAM wparam, lparam lparam)
   {

      smart_pointer < ::message::base > spbase;

      spbase = get_message_base(message, wparam, lparam);

      //__trace_message("window_procedure",spbase);

      try
      {

         message_handler(spbase);

      }
      catch (exit_exception * pexception)
      {

         esp671 esp(pexception);

         pexception->post_quit();

      }
      catch (::exception::exception * pexception)
      {

         esp671 esp(pexception);

         if (!Application.on_run_exception(esp))
         {

            ::multithreading::post_quit(&System);

            return -1;

         }

         return -1;

      }
      catch (...)
      {

      }

      try
      {
         LRESULT lresult = spbase->get_lresult();

         return lresult;

      }
      catch (...)
      {

         return 0;

      }

   }


   void interaction::keep_alive(::object * pliveobject)
   {

      m_papp->keep_alive();

      if (::get_thread() != NULL)
      {

         ::get_thread()->keep_alive();

      }

      if (pliveobject != NULL)
      {

         pliveobject->keep_alive();

      }

   }


   ::user::interaction * interaction::best_top_level_parent(LPRECT lprect)
   {

      sp(::user::interaction) pui = GetTopLevel();

      if (pui.is_null() || pui == this)
      {

         best_monitor(lprect);

      }
      else
      {

         pui->GetWindowRect(lprect);

      }

      return pui;

   }


   index interaction::best_monitor(LPRECT lprect, const RECT & rect, bool bSet, UINT uiSwpFlags, int_ptr iZOrder)
   {

      ::rect rectWindow;

      if (!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = -1;

      if (GetParent() != NULL)
      {

         GetParent()->GetClientRect(rectNew);

         iMatchingMonitor = 0;

      }
      else
      {

         iMatchingMonitor = Session.get_best_monitor(rectNew, rectWindow);

      }

      if (bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {

#ifdef WINDOWSEX

         if (m_bWorkspaceFullScreen)
         {

            if (iMatchingMonitor >= 0 && rectNew.bottom > 2)
            {

               rectNew.bottom -= 2;

            }

         }


         /*synch_lock slUserMutex(m_pmutex);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate, true, false, true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent, true, false, true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent, true, false, true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

            ::ShowWindow(get_handle(), SW_RESTORE);

         }*/

         //ShowWindow(SW_RESTORE);

         SetWindowPos(iZOrder, rectNew, uiSwpFlags);

#elif defined WINDOWSEX

         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         if (::IsZoomed(get_handle()) || ::IsIconic(get_handle()))
         {

            wp.showCmd = SW_RESTORE;

         }
         else
         {

            wp.showCmd = SW_SHOW;

         }

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rect;

         SetWindowPlacement(&wp);

#else

         output_debug_string("\ninteraction::bestmonitor TRUE " + string(typeid(*this).name()) + string(" rectNewArea") + ::str::from(rectNew.area()) + string(" 0x") + ::hex::upper_from(uiSwpFlags));

         SetWindowPos(iZOrder, rectNew, uiSwpFlags);

#endif

      }

      if (lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::best_wkspace(LPRECT lprect, const RECT & rect, bool bSet, UINT uiSwpFlags, int_ptr iZOrder)
   {

      ::rect rectWindow;

      if (!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_best_monitor(rectNew, rectWindow);

      ::rect rectWkspace;

      Session.get_wkspace_rect(iMatchingMonitor, rectWkspace);

      if (bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX

         if (iMatchingMonitor >= 0 && rectNew.bottom > 0)
         {

            if (rectWkspace.bottom > rectNew.bottom - 2)
            {

               rectWkspace.bottom = rectNew.bottom - 2;

            }

         }


         synch_lock slUserMutex(m_pmutex);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate, true, false, true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent, true, false, true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent, true, false, true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

            slUserMutex.unlock();

            ::ShowWindow(get_handle(), SW_MAXIMIZE);

            SetWindowPos(iZOrder, rectWkspace, uiSwpFlags);

         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);

#elif defined WINDOWSEX

         ::rect rectWkspace;

         Session.get_wkspace_rect(iMatchingMonitor, rectWkspace);

         if (!::IsRectEmpty(&rect))
         {

            rect.intersect(lpcrect, rectWkspace);

         }
         else
         {

            rect = rectWkspace;

         }


         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         wp.showCmd = SW_MAXIMIZE;

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rectWkspace;

         //wp.ptMaxPosition = rectWkspace.top_left();

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder, rectWkspace, uiSwpFlags);

#endif

      }

      if (lprect != NULL)
      {

         *lprect = rectWkspace;

      }

      return iMatchingMonitor;

   }


   index interaction::make_zoneing(LPRECT lprect, const RECT & rect, bool bSet, ::user::e_appearance * peappearance, UINT uiSwpFlags, int_ptr iZOrder)
   {

      if (peappearance == NULL || !::user::is_docking_appearance(*peappearance))
      {

         return best_zoneing(lprect, rect, bSet, peappearance, uiSwpFlags, iZOrder);

      }

      ::rect rectWindow;

      if (IsRectEmpty(&rect))
      {

         GetWindowRect(rectWindow);

      }
      else
      {

         rectWindow = rect;

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_zoneing(rectNew, rectWindow, *peappearance);

      if (bSet & (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX


         synch_lock slUserMutex(m_pmutex);

         set_appearance(*peappearance);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate, true, false, true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent, true, false, true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent, true, false, true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

            ::ShowWindow(get_handle(), SW_RESTORE);

            SetWindowPos(iZOrder, rectNew, uiSwpFlags);

         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);

#elif defined WINDOWSEX

         ::rect rectWkspace;

         Session.get_wkspace_rect(iMatchingMonitor, rectWkspace);

         if (!::IsRectEmpty(&rect))
         {

            rect.intersect(lpcrect, rectWkspace);

         }
         else
         {

            rect = rectWkspace;

         }


         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         wp.showCmd = SW_MAXIMIZE;

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rectWkspace;

         //wp.ptMaxPosition = rectWkspace.top_left();

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder, rectNew, uiSwpFlags);

#endif

      }

      if (lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }

   index interaction::best_zoneing(LPRECT lprect, const RECT & rect, bool bSet, ::user::e_appearance * peappearance, UINT uiSwpFlags, int_ptr iZOrder)
   {

      ::user::e_appearance eappearance;

      if (peappearance == NULL)
      {

         peappearance = &eappearance;

      }

      *peappearance = get_appearance();

      ::rect rectWindow;

      if (!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_best_zoneing(peappearance, rectNew, rectWindow);

      if (bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX


         synch_lock slUserMutex(m_pmutex);

         set_appearance(*peappearance);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate, true, false, true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent, true, false, true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent, true, false, true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

            ::ShowWindow(get_handle(), SW_RESTORE);

            SetWindowPos(iZOrder, rectNew, uiSwpFlags);

         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);

#elif defined WINDOWSEX

         ::rect rectWkspace;

         Session.get_wkspace_rect(iMatchingMonitor, rectWkspace);

         if (!::IsRectEmpty(&rect))
         {

            rect.intersect(lpcrect, rectWkspace);

         }
         else
         {

            rect = rectWkspace;

         }


         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         wp.showCmd = SW_MAXIMIZE;

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rectWkspace;

         //wp.ptMaxPosition = rectWkspace.top_left();

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder, rectNew, uiSwpFlags);

#endif

      }

      if (lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::good_restore(LPRECT lprect, const RECT & rect, bool bSet, UINT uiSwpFlags, int_ptr iZOrder)
   {

      ::rect rectWindow;

      if (!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_good_restore(rectNew, rectWindow, this);

      if (bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {
#ifdef WINDOWSEX

         //synch_lock slUserMutex(m_pmutex);

         {

            if (GetExStyle() & WS_EX_LAYERED)
            {

               m_pimpl->m_bShowWindow = true;

               m_pimpl->m_iShowWindow = SW_RESTORE;

               SetWindowPos(iZOrder, rectNew, uiSwpFlags);

            }
            else
            {

               keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate, true, false, true);

               keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent, true, false, true);

               keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent, true, false, true);

               keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

               ::ShowWindow(get_handle(), SW_RESTORE);

               SetWindowPos(iZOrder, rectNew, uiSwpFlags);

            }


         }

         //send_message(WM_SIZE);

         //send_message(WM_MOVE);


#elif defined WINDOWSEX

         WINDOWPLACEMENT wp;

         GetWindowPlacement(&wp);

         if (::IsZoomed(get_handle()) || ::IsIconic(get_handle()))
         {

            wp.showCmd = SW_RESTORE;

         }
         else
         {

            wp.showCmd = SW_SHOW;

         }

         wp.flags = 0;

         Session.monitor_to_wkspace(rect);

         wp.rcNormalPosition = rect;

         SetWindowPlacement(&wp);

#else


         SetWindowPos(iZOrder, rectNew, uiSwpFlags);

#endif

      }

      if (lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::good_iconify(LPRECT lprect, const RECT & rect, bool bSet, UINT uiSwpFlags, int_ptr iZOrder)
   {

      ::rect rectWindow;

      if (!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_good_iconify(&rectNew, rectWindow);

      if (bSet && (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0))
      {

#ifdef WINDOWSEX

         synch_lock slUserMutex(m_pmutex);

         {

            keep < bool > keepLockWindowUpdate(&m_bLockWindowUpdate, true, false, true);

            keep < bool > keepIgnoreSizeEvent(&m_pimpl->m_bIgnoreSizeEvent, true, false, true);

            keep < bool > keepIgnoreMoveEvent(&m_pimpl->m_bIgnoreMoveEvent, true, false, true);

            keep < bool > keepDisableSaveWindowRect(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

            if (GetExStyle() & WS_EX_LAYERED)
            {

               m_pimpl->m_bShowWindow = true;

               m_pimpl->m_iShowWindow = SW_MINIMIZE;

               set_need_redraw();

            }
            else
            {

               ::ShowWindow(get_handle(), SW_MINIMIZE);

            }

         }

#elif defined(LINUX)

         ::show_window(get_handle(), SW_MINIMIZE);

#else

         _throw(todo(get_app()));

         SetWindowPos(iZOrder, rectNew, uiSwpFlags);

#endif

      }

      if (lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   index interaction::good_move(LPRECT lprect, const RECT & rect, UINT uiSwpFlags, int_ptr iZOrder)
   {

      ::rect rectWindow;

      if (!::IsRectEmpty(&rect))
      {

         rectWindow = rect;

      }
      else
      {

         GetWindowRect(rectWindow);

      }

      ::rect rectNew;

      index iMatchingMonitor = Session.get_good_move(rectNew, rectWindow, this);

      if (!::IsRectEmpty(&rect) || iMatchingMonitor >= 0)
      {

         SetWindowPos(iZOrder, rectNew, uiSwpFlags);

      }

      if (lprect != NULL)
      {

         *lprect = rectNew;

      }

      return iMatchingMonitor;

   }


   bool interaction::SetWindowPos(int_ptr z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags)
   {

      if (m_pimpl.is_null())
      {

         return false;

      }

      return m_pimpl->SetWindowPos(z, x, y, cx, cy, nFlags);

   }


   pointd interaction::client_to_screen()
   {

      if (m_pimpl.is_set())
      {

         return m_pimpl->client_to_screen();

      }

      return ::user::interaction_base::client_to_screen();

   }


   //bool interaction::ClientToScreen(LPRECT lprect)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ClientToScreen(lprect))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool interaction::ClientToScreen(LPRECTD lprect)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ClientToScreen(lprect))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool interaction::ClientToScreen(LPPOINT lppoint)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ClientToScreen(lppoint))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool interaction::ClientToScreen(LPPOINTD lppoint)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ClientToScreen(lppoint))
   //   {

   //      return false;

   //   }

   //   return true;

   //}

   //bool interaction::ClientToScreen(RECT64 * lprect)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ClientToScreen(lprect))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool interaction::ClientToScreen(POINT64 * lppoint)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ClientToScreen(lppoint))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool interaction::ScreenToClient(LPRECT lprect)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ScreenToClient(lprect))
   //   {

   //      return false;

   //   }

   //   return true;

   //}

   //bool interaction::ScreenToClient(LPRECTD lprect)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ScreenToClient(lprect))
   //   {

   //      return false;

   //   }

   //   return true;

   //}

   //bool interaction::ScreenToClient(LPPOINT lppoint)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ScreenToClient(lppoint))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool interaction::ScreenToClient(LPPOINTD lppoint)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ScreenToClient(lppoint))
   //   {

   //      return false;

   //   }

   //   return true;

   //}

   //bool interaction::ScreenToClient(RECT64 * lprect)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ScreenToClient(lprect))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool interaction::ScreenToClient(POINT64 * lppoint)
   //{

   //   if (m_pimpl == NULL)
   //   {

   //      return false;

   //   }

   //   if (!m_pimpl->ScreenToClient(lppoint))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   bool interaction::GetClientRect(RECT64 * lprect)
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      if (!m_pimpl->GetClientRect(lprect))
      {

         return false;

      }

      return true;

   }


   id interaction::GetDlgCtrlId() const
   {

      return m_id;

   }


   id interaction::SetDlgCtrlId(id id)
   {

      m_id = id;

      return m_id;

   }


   void interaction::_001OnBaseWndGetProperty(::message::message * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      pbase->set_lresult(_001BaseWndGetProperty((EProperty)pbase->m_wparam, pbase->m_lparam));
   }


   LRESULT interaction::_001BaseWndGetProperty(EProperty eprop, LPARAM lparam)
   {
      switch (eprop)
      {
      case PropertyBaseWndInterface:
      case PropertyDrawBaseWndInterface:
      {
         const interaction_base ** ppinterface = (const interaction_base **)lparam;
         *ppinterface = this;
      }
      return 1;
      default:
         break;
      }
      return 0;
   }



   bool interaction::get_rect_normal(LPRECT lprect)
   {

      return m_pimpl->get_rect_normal(lprect);

   }

   void interaction::offset_viewport_offset(int x, int y)
   {

      point ptOffset = get_viewport_offset();

      set_viewport_offset(ptOffset.x + x, ptOffset.y + y);

   }


   void interaction::offset_viewport_offset_x(int x)
   {

      offset_viewport_offset(x, 0);

   }


   void interaction::offset_viewport_offset_y(int y)
   {

      offset_viewport_offset(0, y);

   }


   void interaction::set_viewport_offset(int x, int y)
   {

      point ptOffset(x, y);

      if(!validate_viewport_offset(ptOffset))
      {

         return;

      }

      m_ptScrollPassword1 = ptOffset;

      on_change_viewport_offset();

   }


   bool interaction::validate_viewport_offset(point & p)
   {

      if (p == m_ptScrollPassword1)
      {

         return false;

      }

      return true;

   }


   void interaction::set_viewport_offset_x(int x)
   {

      set_viewport_offset(x, get_viewport_offset().y);

   }


   void interaction::set_viewport_offset_y(int y)
   {

      set_viewport_offset(get_viewport_offset().x, y);

   }


   void interaction::on_change_viewport_offset()
   {

      set_need_redraw();

   }


   pointd interaction::get_viewport_offset()
   {

      return m_ptScrollPassword1;

   }


   size interaction::get_total_size()
   {

      ::rect rectClient;

      GetClientRect(rectClient);

      return rectClient.get_size();

   }


   void interaction::on_change_view_size()
   {

   }


   size interaction::get_page_size()
   {

      ::rect rectClient;

      GetClientRect(rectClient);

      return rectClient.get_size();

   }


   pointd interaction::get_ascendant_viewport_offset()
   {

      sp(::user::interaction) puser = GetParent();

      pointd pt(0.0, 0.0);

      while (puser.is_set())
      {

         pt += puser->get_viewport_offset();

         puser = puser->GetParent();

      }

      return pt;

   }


   void interaction::get_margin_rect(LPRECT lprectMargin)
   {

      lprectMargin->left = 0;
      lprectMargin->top = 0;
      lprectMargin->right = 0;
      lprectMargin->bottom = 0;

   }

   int interaction::get_final_x_scroll_bar_width()
   {

      return 0;

   }

   int interaction::get_final_y_scroll_bar_width()
   {

      return 0;

   }


   pointd interaction::get_parent_viewport_offset()
   {

      ::user::interaction * puser = GetParent();

      if (puser == NULL)
      {

         return point(0, 0);

      }

      return puser->get_viewport_offset();

   }


   //   bool interaction::select_font(::draw2d::graphics * pgraphics, e_font efont)
   //   {
   //
   //      return ::user::style::select_font(pgraphics, efont, this);
   //
   //   }


   //::user::style * interaction::parent_userstyle()
   //{

   //   ::user::interaction * puiParent = GetParent();

   //   if (puiParent == NULL)
   //      return NULL;

   //   return puiParent->userstyle();

   //}

   void interaction::_001OnShowWindow(::message::message * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);

      if (!pshowwindow->m_bShow)
      {

         user_interaction_on_hide();

      }

      set_need_redraw();

   }

   void interaction::_001OnSetFocus(::message::message * pmessage)
   {

      SCAST_PTR(::message::set_focus, psetfocus, pmessage);

      on_reset_focus_start_tick();

      // get_keyboard_focus will return the control with focus


      // return true to set focus to this control
      Application.keyboard_focus_OnSetFocus(this);


   }




   void interaction::_001OnLButtonDown(::message::message * pobj)
   {

      //      SCAST_PTR(::message::mouse,pmouse,pobj);

      pobj->previous();

      try
      {

         if (keyboard_focus_is_focusable())
         {

            Session.set_keyboard_focus(this);


         }
         else
         {

            output_debug_string("::user::interaction::_001OnLButtonDown Not Focusable\n");

            Session.set_keyboard_focus(NULL);

            //Session.user()->set_mouse_focus_LButtonDown(NULL);

         }


      }
      catch (...)
      {

      }

   }



   //   void interaction::transfer_from(::aura::timer_array & ta, interaction * pui)
   //   {
   //
   //
   //      single_lock sl(&ta.m_mutex,TRUE);
   //
   //      smart_pointer_array < ::aura::timer_item > timera;
   //
   ////      ta.detach(timera,this);
   //
   //      set_timer(timera);
   //
   //   }

   /*

      sp(::message::base) interaction::peek_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {

         if(!::PeekMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax,wRemoveMsg))
            return NULL;

         return get_base(lpmsg,pwnd);

      }


      sp(::message::base) interaction::get_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {

         if(!::GetMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax))
            return NULL;

         return get_base(lpmsg,pwnd);

      }


      sp(::message::base) interaction::peek_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {
         MESSAGE msg;
         return peek_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax,wRemoveMsg);
      }


      sp(::message::base) interaction::get_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {
         MESSAGE msg;
         return get_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax);
      }


      sp(::message::base) interaction::peek_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {
         if(!::PeekMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax,wRemoveMsg))
            return NULL;
         return get_base(lpmsg,pwnd);
      }

      sp(::message::base) interaction::get_message(LPMESSAGE lpmsg,::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {
         if(!::GetMessage(lpmsg,pwnd->get_safe_handle(),wMsgFilterMin,wMsgFilterMax))
            return NULL;
         return get_base(lpmsg,pwnd);
      }

      sp(::message::base) interaction::peek_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
      {
         MESSAGE msg;
         return peek_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax,wRemoveMsg);
      }


      sp(::message::base) interaction::get_message(::user::interaction * pwnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
      {
         MESSAGE msg;
         return get_message(&msg,pwnd,wMsgFilterMin,wMsgFilterMax);
      }

      */

   sp(::message::base) interaction::get_message_base(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      sp(::message::base) pbase;

      ::message::e_prototype eprototype = ::message::PrototypeNone;

      {

         eprototype = ::message::sender::GetMessagePrototype(uiMessage, 0);

      }

      switch (eprototype)
      {
      case ::message::PrototypeNone:
      {

         pbase = canew(::message::base(get_app()));

      }
      break;
      case ::message::PrototypeCreate:
      {
         pbase = canew(::message::create(get_app()));
      }
      break;
      case ::message::PrototypeEnable:
      {
         pbase = canew(::message::enable(get_app()));
      }
      break;
      case ::message::PrototypeNcActivate:
      {
         pbase = canew(::message::nc_activate(get_app()));
      }
      break;
      case ::message::PrototypeKey:
      {
         pbase = canew(::message::key(get_app()));
      }
      break;
      case ::message::PrototypeTimer:
      {
         _throw(simple_exception(get_app(), "do not use WM_TIMER or Windows SetTimer/KillTimer"));
         //           pbase = canew(::message::timer(get_app()));
      }
      break;
      case ::message::PrototypeShowWindow:
      {
         pbase = canew(::message::show_window(get_app()));
      }
      break;
      case ::message::PrototypeSetCursor:
      {
         pbase = canew(::message::set_cursor(get_app()));
      }
      break;
      case ::message::PrototypeNcHitTest:
      {
         pbase = canew(::message::nchittest(get_app()));
      }
      break;
      case ::message::PrototypeMove:
      {
         pbase = canew(::message::move(get_app()));
      }
      break;
      case ::message::PrototypeEraseBkgnd:
      {
         pbase = canew(::message::erase_bkgnd(get_app()));
      }
      break;
      case ::message::PrototypeScroll:
      {
         pbase = canew(::message::scroll(get_app()));
      }
      break;
      case ::message::PrototypeSetFocus:
      {
         pbase = canew(::message::set_focus(get_app()));
      }
      break;
      case ::message::PrototypeKillFocus:
      {
         pbase = canew(::message::kill_focus(get_app()));
      }
      break;
#if !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS)
      case ::message::PrototypeWindowPos:
      {
         pbase = canew(::message::window_pos(get_app()));
      }
      break;
      case ::message::PrototypeNcCalcSize:
      {
         pbase = canew(::message::nc_calc_size(get_app()));
      }
      break;
#endif
      case ::message::PrototypeMouse:
      {
         pbase = canew(::message::mouse(get_app()));
      }
      break;
      case ::message::PrototypeMouseWheel:
      {
         pbase = canew(::message::mouse_wheel(get_app()));
      }
      break;
      case ::message::PrototypeSize:
      {
         pbase = canew(::message::size(get_app()));
      }
      break;
      case ::message::PrototypeActivate:
      {
         pbase = canew(::message::activate(get_app()));
      }
      break;
      case ::message::PrototypeMouseActivate:
      {
         pbase = canew(::message::mouse_activate(get_app()));
      }
      break;
      case ::message::PrototypeSimpleCommand:
      {
         pbase = canew(::message::simple_command(get_app()));
      }
      break;
      default:
      {
         pbase = canew(::message::base(get_app()));
      }
      break;
      }

      if (pbase.is_null())
      {

         return NULL;

      }

      pbase->set(this, uiMessage, wparam, lparam);

      return pbase;

   }


   window_graphics * interaction::get_window_graphics()
   {

      if (m_pimpl == NULL)
         return ::user::interaction_base::get_window_graphics();

      return m_pimpl->get_window_graphics();

   }


   bool interaction::is_composite()
   {

      if (m_pimpl == NULL)
         return true; // optimistic response, assume always true alpha blendable

      return m_pimpl->is_composite();

   }

   size interaction::get_window_minimum_size()
   {

      return size(0, 0);

   }

   // up down target

   //UpDownTarget::UpDownTarget()
   //{
   //   m_eupdowntargettype = type_normal_frame;
   //}

   void interaction::UpDownTargetAttach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      bool bAttached = false;

      pupdown->m_eupdown = updown_down;

      try
      {

         if (OnUpDownTargetAttach(pupdown))
         {

            bAttached = true;

         }

      }
      catch (...)
      {

      }

      if (!bAttached)
      {

         pupdown->m_eupdown = updown_none;

      }

   }


   void interaction::UpDownTargetDetach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      bool bDetached = false;

      pupdown->m_eupdown = updown_up;

      try
      {

         if (OnUpDownTargetDetach(pupdown))
         {

            bDetached = true;

         }

      }
      catch (...)
      {

      }

      if (!bDetached)
      {

         pupdown->m_eupdown = updown_none;

      }

   }


   bool interaction::OnUpDownTargetAttach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      return false;

   }


   bool interaction::OnUpDownTargetDetach(::user::wndfrm::frame::WorkSetUpDownInterface * pupdown)
   {

      return false;

   }


   void interaction::on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint)
   {

      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(pHint);

   }


   void interaction::keyboard_focus_OnKeyDown(::message::message * pobj)
   {

      SCAST_PTR(::message::key, pkey, pobj);

      if (pkey->m_ekey == ::user::key_tab)
      {

         control_event ev;

         ev.m_puie = dynamic_cast <::user::interaction *> (this);
         ev.m_eevent = ::user::event_tab_key;
         ev.m_actioncontext = ::action::source_user;

         GetParent()->on_control_event(&ev);

         on_control_event(&ev);

      }

   }



   bool interaction::get_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      return m_uiptraChild.get_child(pui);

   }

   bool interaction::rget_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      return m_uiptraChild.rget_child(pui);

   }


   ::user::elemental * interaction::get_focus_elemental()
   {

      if (m_pimpl == NULL)
      {

         return NULL;

      }

      return m_pimpl->get_focus_elemental();

   }


   bool interaction::set_focus_elemental(::user::elemental * pelemental)
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->set_focus_elemental(pelemental);

   }


   bool interaction::is_ascendant_of(::user::interaction * puiDescendantCandidate, bool bIncludeSelf)
   {

      if (puiDescendantCandidate == NULL)
      {

         return false;

      }

      return puiDescendantCandidate->is_descendant_of(this, bIncludeSelf);

   }


   bool interaction::is_descendant_of(::user::interaction * puiAscendantCandidate, bool bIncludeSelf)
   {

      ::user::interaction * pui = bIncludeSelf ? this : GetParent();

      while (pui != NULL)
      {

         if (pui == puiAscendantCandidate)
            return true;

         pui = pui->GetParent();

      }

      return false;

   }


   bool interaction::is_ascendant_or_owner_of(::user::interaction * puiDescendantCandidate, bool bIncludeSelf)
   {

      if (puiDescendantCandidate == NULL)
      {

         return false;

      }

      return puiDescendantCandidate->is_descendant_of_or_owned_by(this, bIncludeSelf);

   }


   bool interaction::is_descendant_of_or_owned_by(::user::interaction * puiAscendantCandidate, bool bIncludeSelf)
   {

      ::user::interaction * pui = bIncludeSelf ? this : GetParentOrOwner();

      while (pui != NULL)
      {

         if (pui == puiAscendantCandidate)
            return true;

         pui = pui->GetParentOrOwner();

      }

      return false;

   }


   bool interaction::kick_queue()
   {

      post_message(::message::message_null);

      return true;

   }


   bool interaction::has_action_hover()
   {

      return false;

   }


   bool interaction::has_text_input()
   {

      return false;

   }


   COLORREF interaction::get_action_hover_border_color()
   {

      return 0;

   }


   bool interaction::show_tooltip(const string & str, bool bError)
   {

      if (m_ptooltip.is_null())
      {

         m_ptooltip = canew(tooltip(get_app()));

         ::user::create_struct cs(WS_EX_LAYERED | WS_EX_TOOLWINDOW, NULL, "tooltip", 0, ::null_rect());

         m_ptooltip->create_window_ex(cs);

      }

      get_tooltip()->m_str = str;

      get_tooltip()->m_bError = bError;

      get_tooltip()->CalcSize();

      if (!layout_tooltip(true))
      {

         return false;

      }

      return true;

   }

   void interaction::ExitHelpMode()
   {

   }


   void interaction::on_control_event(::message::base * pbase)
   {

      auto pevent = pbase->m_lparam.cast < ::user::control_event >();

      on_control_event(pevent);

   }


   bool interaction::layout_tooltip(bool bForceShow)
   {

      if (m_ptooltip.is_null())
      {

         return true;

      }

      if (m_ptooltip->IsWindowVisible() && !get_wnd()->is_active())
      {

         hide_tooltip();

         return false;

      }

      if (!bForceShow && !m_ptooltip->IsWindowVisible())
      {

         return true;

      }

      ::size sizeTooltip = get_tooltip()->m_size;

      ::rect rectThisWindow;

      GetWindowRect(rectThisWindow);

      ::rect rectWindow;

      ::rect rectMonitor;

      Session.get_best_monitor(rectMonitor, rectThisWindow);

      rectWindow = rectThisWindow;

      rectWindow.SetBottomRightSize(sizeTooltip);

      rectWindow.offset(0, -sizeTooltip.cy);

      if (rectWindow.top < rectMonitor.top)
      {

         rectWindow.move_to_y(rectThisWindow.bottom);

      }

      if (rectWindow.right > rectMonitor.right)
      {

         rectWindow.move_to_x(rectMonitor.right - sizeTooltip.cx - rectThisWindow.height());

      }

      m_ptooltip->SetWindowPos(ZORDER_TOPMOST, rectWindow, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOACTIVATE);

      return true;

   }


   bool interaction::hide_tooltip()
   {

      if (m_ptooltip.is_null())
      {

         return true;

      }

      m_ptooltip->ShowWindow(SW_HIDE);

      return true;

   }


   bool interaction::frame_is_transparent()
   {

      return false;

   }

   bool interaction::WfiToggleTransparentFrame()
   {

      return false;

   }


   double interaction::get_alpha()
   {

      if (m_palphasource == NULL)
      {

         return 1.0;

      }
      else
      {

         return m_palphasource->get_alpha(this);

      }

   }


   bool interaction::has_pending_graphical_update()
   {

      if (m_bProDevian)
      {

         return true;

      }

#ifdef APPLE_IOS

      {

         synch_lock sl(m_pmutex);

         for (index i = 0; i < m_uiptraChild.get_size(); i++)
         {

            ::user::interaction * pui = m_uiptraChild[i];

            sl.unlock();

            if (pui->has_pending_graphical_update())
            {

               return true;

            }

            sl.lock();


         }

      }

#endif

      if(IsWindowVisible())
      {

         if (m_bRedrawOnVisible)
         {

            return true;

         }

         if (m_bRedraw)
         {

            return true;

         }

      }

      if (m_pimpl->has_pending_graphical_update())
      {

         return true;

      }

      return false;

   }


   bool interaction::enable_transparent_mouse_events(bool bEnable)
   {

      ::user::interaction * puiTop = get_wnd();

      if (puiTop == NULL)
      {

         return false;

      }

      synch_lock sl(puiTop->m_pmutex);

      sp(::user::interaction_impl) pimpl = puiTop->m_pimpl;

      if (pimpl.is_null())
      {

         return false;

      }

      pimpl->m_bTransparentMouseEvents = bEnable;

      check_transparent_mouse_events();

      return true;

   }


   void interaction::check_transparent_mouse_events()
   {

      ::user::interaction * puiTop = get_wnd();

      bool bStart = true;

      if (puiTop == NULL)
      {

         bStart = false;

      }

      synch_lock sl(bStart ? puiTop->m_pmutex : NULL);

      sp(::user::interaction_impl) pimpl = puiTop->m_pimpl;

      if (bStart)
      {

         synch_lock sl(puiTop->m_pmutex);

         if (pimpl.is_null())
         {

            bStart = false;

         }

         if (bStart)
         {

            bStart = pimpl->m_bTransparentMouseEvents && puiTop->IsWindowVisible();

         }

      }

      if (bStart)
      {

         if (pimpl->m_pthreadTransparentMouseEvents.is_null())
         {

            pimpl->m_pthreadTransparentMouseEvents = fork([pimpl]()
            {

               try
               {

                  pimpl->_thread_transparent_mouse_events();

               }
               catch (...)
               {

               }

               pimpl->m_pthreadTransparentMouseEvents.release();

            });

         }

      }
      else
      {

         if (pimpl.is_set() && pimpl->m_pthreadTransparentMouseEvents.is_set())
         {

            ::multithreading::post_quit(pimpl->m_pthreadTransparentMouseEvents);

         }

      }

   }


   void interaction::defer_notify_mouse_move(bool & bPointInside, point & ptLast)
   {

      if (Session.get_capture() != NULL)
      {

         return;

      }

      point ptCurrent;

      Session.get_cursor_pos(ptCurrent);

      if (ptCurrent != ptLast)
      {

         ptLast = ptCurrent;

         bPointInside = _001IsPointInside(ptCurrent);

         if (bPointInside || m_bMouseHover)
         {

#if !defined(LINUX)

            get_wnd()->ScreenToClient(ptCurrent);

#endif

            if (bPointInside)
            {

               m_bMouseHover = true;

               get_wnd()->message_call(WM_MOUSEMOVE, 0, ptCurrent);

            }
            else
            {

               m_bMouseHover = false;

               get_impl()->mouse_hover_remove(this);

               send_message(WM_MOUSELEAVE);

               set_need_redraw();

            }

         }

      }

   }


   void interaction::destruct()
   {

      // If this is a "top level" or "operating system" window,
      // the operating system may keep reference to this object,
      // preventing it from being fully released.
      // If the window DestroyWindow member is called in interaction::~interaction destructor,
      // derived classes from interaction::~interaction may have deleted object resources
      // that could be used to correctly destroy window, as DestroyWindow generally require
      // that the object be a full valid object before being disposed.

      DestroyWindow();

      ::user::interaction_base::destruct();

   }


   void interaction::defer_notify_mouse_move()
   {

      if (Session.get_capture() != NULL)
      {

         return;

      }

      point ptCurrent;

      Session.get_cursor_pos(ptCurrent);

#if !defined(LINUX)

      get_wnd()->ScreenToClient(ptCurrent);

#endif

      get_wnd()->send_message(WM_MOUSEMOVE, 0, ptCurrent);

   }


   double alpha_source::get_alpha(::user::interaction * puiTarget)
   {

      return 1.0;

   }


   void alpha_source::on_alpha_target_initial_frame_position()
   {


   }


   void interaction::on_after_graphical_update()
   {

      ::user::interaction_base::on_after_graphical_update();

      if(m_pimpl.is_set())
      {

         m_pimpl->on_after_graphical_update();

      }

   }


   void interaction::_001OnDeiconify(::user::e_appearance eappearance)
   {

      WfiRestore();

   }


   void interaction::on_setting_changed(::aura::e_setting esetting)
   {


   }


   bool interaction::_001OnClick(uint_ptr nFlag, point point)
   {

      return false;

   }


   bool interaction::_001OnRightClick(uint_ptr nFlag, point point)
   {

      return false;

   }


   ::sized interaction::get_size()
   {

      rect rectWindow;

      GetWindowRect(rectWindow);

      return rectWindow.get_size();

   }


   ::sized interaction::get_client_size()
   {

      rect rectClient;

      GetClientRect(rectClient);

      return rectClient.get_size();

   }


   int interaction::width()
   {

      rect rectWindow;

      GetWindowRect(rectWindow);

      return rectWindow.width();

   }


   int interaction::height()
   {

      rect rectWindow;

      GetWindowRect(rectWindow);

      return rectWindow.height();

   }


   int interaction::client_width()
   {

      rect rectClient;

      GetClientRect(rectClient);

      return rectClient.width();

   }


   int interaction::client_height()
   {

      rect rectClient;

      GetClientRect(rectClient);

      return rectClient.height();

   }

   void interaction::resize_to_fit()
   {

      // this default implementation doesn't need to be called by derived
      // classes and it has the side effect of warning at debug output log
      // the default resize_to_fit implementation is being called.

      ::output_debug_string("default resize_to_fit doesn't do anything\n");


   }


   void interaction::wait_redraw()
   {

      while (has_pending_redraw_flags())
      {

         do_events();

         Sleep(5);

      }


   }


   bool interaction::has_pending_redraw_flags()
   {

      if (m_pimpl == NULL)
      {

         return false;

      }

      return m_pimpl->has_pending_redraw_flags();

   }


   //void interaction::on_select_user_style()
   //{

   //   if (get_app()->m_psession == NULL)
   //   {

   //      return;

   //   }

   //   if (m_puserstyle == NULL)
   //   {

   //      ::user::interaction * pui = GetOwner();

   //      while (pui != NULL)
   //      {

   //         m_puserstyle = pui->m_puserstyle;

   //         if (m_puserstyle != NULL)
   //         {

   //            break;

   //         }

   //         pui = pui->GetOwner();

   //      }

   //   }


   //   if (m_puserstyle == NULL)
   //   {

   //      ::user::interaction * pui = GetParent();

   //      while (pui != NULL)
   //      {

   //         m_puserstyle = pui->m_puserstyle;

   //         if (m_puserstyle != NULL)
   //         {

   //            break;

   //         }

   //         pui = pui->GetParent();

   //      }

   //   }


   //   sp(::user::style) puserstyle = m_puserstyle;

   //   if (puserstyle.is_null())
   //   {

   //      puserstyle = Session.m_puserstyle;

   //      if (puserstyle == NULL)
   //      {

   //         puserstyle = Application.m_puserstyle;

   //         if (puserstyle == NULL)
   //         {

   //            puserstyle = Session.m_puserstyle;

   //            if (puserstyle == NULL)
   //            {

   //               Session.m_puserstyle = Session.get_user_style("", get_app());

   //               m_puserstyle = Session.m_puserstyle;

   //            }

   //         }

   //      }

   //   }


   //   //if (m_puserstyle == NULL)
   //   {

   //      ::user::style::on_select_user_style();

   //   }

   //}

   //bool interaction::get_color(COLORREF & cr, e_color ecolor)
   //{

   //   return get_color(cr, ecolor);

   //}


   //bool interaction::get_font(::draw2d::font_sp & sp, e_font efont)
   //{

   //   return get_font(sp, efont, this);

   //}


   //bool interaction::get_translucency(e_translucency & etranslucency, e_element eelement)
   //{

   //   return get_translucency(etranslucency, eelement, this);

   //}


   //bool interaction::has_flag(::user::e_flag eflag)
   //{

   //   return has_flag(eflag, this);

   //}


   //rect interaction::get_rect(::user::e_rect erect)
   //{

   //   return get_rect(erect, this);

   //}


   //int interaction::get_int(::user::e_int eint)
   //{

   //   return get_int(eint, this);

   //}


   bool interaction::select_text_color(::draw2d::graphics * pgraphics, e_color ecolor)
   {

      return select_text_color(this, pgraphics, ecolor);

   }


   bool interaction::select_solid_brush(::draw2d::graphics * pgraphics, e_color ecolor)
   {

      return select_solid_brush(this, pgraphics, ecolor);

   }


   bool interaction::select_font(::draw2d::graphics * pgraphics, e_font efont)
   {

      return select_font(this, pgraphics, efont);

   }


   bool interaction::select_text_color(e_color ecolor)
   {

      return select_text_color(this, ecolor);

   }


   bool interaction::select_solid_brush(e_color ecolor)
   {

      return select_solid_brush(this, ecolor);

   }


   bool interaction::select_font(e_font efont)
   {

      return select_font(this, efont);

   }


   void interaction::set_stock_icon(e_stock_icon eicon)
   {

      _throw(interface_only_exception(get_app()));

   }


   e_stock_icon interaction::get_stock_icon()
   {

      return stock_icon_none;

   }


   void interaction::set_ipc_copy(bool bSet)
   {

      ::user::interaction * pwnd = get_wnd();

      if (pwnd == NULL)
      {

         _throw(invalid_argument_exception(get_app()));

      }

      if (pwnd == this)
      {

         m_pimpl->set_ipc_copy(bSet);

      }
      else
      {

         pwnd->set_ipc_copy(bSet);

      }

   }


   bool interaction::scroll_bar_get_client_rect(LPRECT lprect)
   {

      if (!GetClientRect(lprect))
      {

         return false;

      }

      lprect->right += get_final_y_scroll_bar_width();
      lprect->bottom += get_final_x_scroll_bar_width();

      return true;

   }


   void interaction::on_calc_size(calc_size * psize)
   {

      rect rectWindow;

      GetWindowRect(rectWindow);

      psize->m_size = rectWindow.get_size();

   }


   e_control_type interaction::get_control_type()
   {

      return m_econtroltype;

   }


   void interaction::nextstyle(style_context * pcontext)
   {

      if (GetParent() == NULL)
      {

         if (GetOwner() == NULL)
         {

            Session.userstyle(pcontext);

         }
         else
         {

            pcontext->m_pstyle = GetOwner();

         }

      }
      else
      {

         pcontext->m_pstyle = GetParent();

      }

   }

   void interaction::defer_show_software_keyboard(::user::elemental * pelemental, bool bShow, string str, strsize iBeg, strsize iEnd)
   {

      synch_lock sl(m_pmutex);

      m_iSoftwareKeyboardEventId++;

      index iEventId = m_iSoftwareKeyboardEventId;

      m_pelementalSoftwareKeyboard = pelemental;

      fork([=]
      {

         Sleep(400);

         synch_lock sl(m_pmutex);

         if(iEventId == m_iSoftwareKeyboardEventId)
         {

            ASSERT(pelemental == m_pelementalSoftwareKeyboard);

            sl.unlock();

            show_software_keyboard(bShow, str, iBeg, iEnd);

         }

      });

   }


   void interaction::show_software_keyboard(bool bShow, string str, strsize iBeg, strsize iEnd)
   {

      if(m_pimpl.is_null())
      {

         return;

      }

      m_pimpl->show_software_keyboard(bShow, str, iBeg, iEnd);

   }


   void interaction::post_runnable(::object * pobjectRunnable)
   {

      ::thread * pthread = get_wnd() == NULL ? NULL : get_wnd()->m_pthreadUserInteraction;

      ::thread * pthreadCurrent = ::get_thread();

      if (pthread == NULL || pthread == pthreadCurrent)
      {

         one_shot_run(pobjectRunnable);

      }
      else
      {

         pthread->post_runnable(pobjectRunnable);

      }

   }


   void interaction::send_runnable(::object * pobjectRunnable, ::duration durationTimeout)
   {

      ::thread * pthread = get_wnd() == NULL ? NULL : get_wnd()->m_pthreadUserInteraction;

      ::thread * pthreadCurrent = ::get_thread();

      if (pthread == NULL || pthread == pthreadCurrent)
      {

         one_shot_run(pobjectRunnable);

      }
      else
      {

         pthread->send_runnable(pobjectRunnable, durationTimeout);

      }

   }


} // namespace user



