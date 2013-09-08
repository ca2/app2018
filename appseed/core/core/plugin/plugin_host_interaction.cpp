#include "framework.h"


#ifdef Application
#undef Application
#endif

#define Application System



namespace plugin
{

   host_interaction::host_interaction(sp(base_application) papp) :
      element(papp)
   {
      m_pframe = NULL;
   }

   host_interaction::~host_interaction()
   {
      if(m_papp != NULL)
      {
         try
         {
            m_papp.release();
         }
         catch(...)
         {
         }
      }
   }

   void host_interaction::install_message_handling(::message::dispatch * pinterface)
   {

      ::user::interaction::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE         , pinterface, this, &host_interaction::_001OnMouseMove);
      IGUI_WIN_MSG_LINK(message_check        , pinterface, this, &host_interaction::_001OnCheck);
      IGUI_WIN_MSG_LINK(WM_CREATE            , pinterface, this, &host_interaction::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_TIMER             , pinterface, this, &host_interaction::_001OnTimer);

      /*
      IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGED  , pinterface, this, &host_interaction::on_ignore_message);
      IGUI_WIN_MSG_LINK(WM_SIZE              , pinterface, this, &host_interaction::on_ignore_message);
      IGUI_WIN_MSG_LINK(WM_MOVE              , pinterface, this, &host_interaction::on_ignore_message);
      IGUI_WIN_MSG_LINK(WM_TIMER             , pinterface, this, &host_interaction::on_ignore_message);
      IGUI_WIN_MSG_LINK(WM_IME_SETCONTEXT    , pinterface, this, &host_interaction::on_ignore_message);
      IGUI_WIN_MSG_LINK(WM_WINDOWPOSCHANGING , pinterface, this, &host_interaction::on_ignore_message);
      IGUI_WIN_MSG_LINK(WM_CHILDACTIVATE     , pinterface, this, &host_interaction::on_ignore_message);
      */

   }

   void host_interaction::_001OnMouseMove(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void host_interaction::_001OnCreate(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      Application.m_bSessionSynchronizedCursor = false;
   }

   void host_interaction::_001OnCheck(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_wparam == 0)
      {
         if(pbase->m_lparam != 0)
         {
            m_pplugin->ready_on_main_thread();
         }
      }
      else if(pbase->m_wparam == 2)
      {
         string * pstrLink = (string *) pbase->m_lparam.m_lparam;
         string strLink(*pstrLink);
         property_set setQuery(get_app());
         //setQuery.parse_url_query(strLink);
         if(setQuery.has_property("karfile_url"))
         {
            System.url().string_set(strLink, "karfile_url", (const char *) m_pplugin->get_host_location_url());
         }
         // do the following if desired on calling System open linnk, will try to create plugin::system::get_host_location_url
         // else if(setQuery["ruri"].get_string().get_length() <= 0)
         //{
            //System.url().string_set(strLink, "ruri", (const char *) m_pplugin->get_host_location_url());
         //}
         m_pplugin->open_url(strLink);
         delete pstrLink;
      }
   }

   void host_interaction::_001OnTimer(signal_details * pobj)
   {
      SCAST_PTR(::message::timer, ptimer, pobj);
      if(ptimer->m_nIDEvent == 19841115)
      {
         KillTimer(19841115);
         m_pplugin->ready_on_main_thread();
      }
   }



   void host_interaction::_000OnDraw(::draw2d::graphics * pdc)
   {

      //rect rectWindow;
      //m_pguie->GetWindowRect(rectWindow);
      //point ptPreviousViewportOrg = pdc->GetViewportOrg();
      //pdc->SetViewportOrg(rectWindow.top_left());

      //rect rectClient;
      //GetClientRect(rectClient);

      _001DrawChildren(pdc);

      //pdc->SetViewportOrg(ptPreviousViewportOrg);
      //pdc->SelectClipRgn(NULL);
      point ptCursor;
      Session.get_cursor_pos(&ptCursor);
      ScreenToClient(&ptCursor);
      ::visual::cursor * pcursor = Session.get_cursor();
      if(pcursor != NULL)
      {
         pcursor->to(pdc, ptCursor);
      }

   }

   bool host_interaction::defer_check(e_check echeck)
   {

      if(!m_pplugin->is_ok())
         return false;

      return send_message((UINT)message_check, 0, (LPARAM) echeck) != 0;

   }


   void host_interaction::layout()
   {
      if(m_pframe != NULL)
      {
         class rect rect;
         GetClientRect(rect);
         m_pframe->SetWindowPos(0, 0, 0, rect.width(), rect.height(), SWP_SHOWWINDOW);
      }
      else
      {
         sp(::user::interaction) pui = get_top_child();
         if(pui != NULL)
         {
            class rect rectClient;
            GetClientRect(rectClient);
            class rect rectWindow;
            pui->GetWindowRect(rectWindow);
            ScreenToClient(rectWindow);
            if(rectWindow != rectClient)
            {
               pui->SetWindowPos(0, 0, 0, rectClient.width(), rectClient.height(), SWP_SHOWWINDOW);
            }
            else
            {
               pui->ShowWindow(SW_SHOW);
               pui->layout();
            }
         }
      }
   }

   bool host_interaction::IsWindowVisible()
   {
      // it is a regular operational system invisible window whose visibility should be controlled by plugin.
      // So, do not let children be invisible because this invisible host is invisible...
      return true;
   }

   void host_interaction::GetWindowRect(__rect64 * lprect)
   {
      RECT rect;
      m_pplugin->get_window_rect(&rect);
      ::copy(lprect, &rect);
   }


   bool host_interaction::RedrawWindow(LPCRECT lpRectUpdate, ::draw2d::region* prgnUpdate, UINT flags)
   {

      UNREFERENCED_PARAMETER(lpRectUpdate);
      UNREFERENCED_PARAMETER(prgnUpdate);
      UNREFERENCED_PARAMETER(flags);

      return TRUE;

   }

   sp(::user::interaction) host_interaction::get_os_focus_uie()
   {
      return Application.window_from_os_data(m_pplugin->get_host_window());
   }

   void host_interaction::_user_message_handler(signal_details * pobj)
   {
      ::user::interaction::_user_message_handler(pobj);
      pobj->m_bRet = true;
   }

   void host_interaction::_on_start_user_message_handler()
   {
      ::user::interaction::_on_start_user_message_handler();
      m_pfnDispatchWindowProc = reinterpret_cast < void (::message::dispatch::*)(signal_details * pobj) > (&host_interaction::_user_message_handler);
   }


   void host_interaction::on_ignore_message(signal_details * pobj)
   {
      // avoid host interaction call DefWindowProc for certain Windows messages
//      SCAST_PTR(::message::base, pbase, pobj);

      pobj->m_bRet = true;
   }


   void host_interaction::_000OnMouse(message::mouse * pmouse)
   {
      if(&Session != NULL)
      {
         Session.m_ptCursor = pmouse->m_pt;
      }
      else
      {
         if(m_uiptraChild.get_size() > 0 && m_uiptraChild[0].m_papp != NULL && m_uiptraChild[0].m_pbaseapp->m_pplaneapp->m_psession != NULL)
         {
            set_app(m_uiptraChild[0].m_papp);
         }
      }
      ::user::interaction::_000OnMouse(pmouse);
   }

   LRESULT host_interaction::DefWindowProc(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {

      if(uiMessage == WM_NCCREATE
      || uiMessage == WM_CREATE)
/*      || uiMessage == WM_SIZE
      || uiMessage == WM_MOVE
      || uiMessage == WM_WINDOWPOSCHANGING
      || uiMessage == WM_WINDOWPOSCHANGED)*/
      {

         return ::user::interaction::DefWindowProc(uiMessage, wparam, lparam);

      }
      else
      {

         return 0;

      }

   }

} // namespace plugin
