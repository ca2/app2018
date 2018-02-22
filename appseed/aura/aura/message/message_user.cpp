﻿#include "framework.h"




#if defined(LINUX)
// Ubuntu apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#endif

#undef new

struct myfx_CTLCOLOR
{
   ::oswindow oswindow;
   HDC hDC;
   UINT nCtlType;
};


namespace message
{


   void create::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_lpcreatestruct = reinterpret_cast<::user::create_struct *>(lparam.m_lparam);
   }

   void create::set_lresult(LRESULT lresult)
   {
      base::set_lresult(lresult);
      if(get_lresult() == -1)
         m_bRet = true;
      else
         m_bRet = false;
   }

   void create::failed(const char * lpcszErrorMessage)
   {
      error(lpcszErrorMessage);
   }

   void create::error(const char * lpcszErrorMessage)
   {
      set_lresult(-1);
      ::aura::application * papp = ::aura::system::g_p;
      APPTRACE("%s", lpcszErrorMessage);
#ifdef DEBUG
      Sys(::aura::system::g_p).log().print(lpcszErrorMessage);
#endif
   }


   activate::activate(::aura::application * papp):
      ::object(papp),
      ::message::base(papp)
   {
   }

   void activate::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {

      base::set(pwnd,uiMessage,wparam,lparam,lresult);

      m_nState = (UINT)(LOWORD(wparam));

      if(lparam == 0)
      {

         m_pWndOther = NULL;

      }
      else
      {

         m_pWndOther = System.ui_from_handle((void *)lparam);

      }

      m_bMinimized = HIWORD(wparam) != FALSE;

   }



   erase_bkgnd::erase_bkgnd(::aura::application * papp):
      ::object(papp),
      ::message::base(papp)
   {
   }

   void erase_bkgnd::set_result(bool bResult)
   {
      set_lresult(bResult);
   }


   key::key(::aura::application * papp):
      ::object(papp),
      ::user::key(papp)
   {
      m_ekey = ::user::key_none;
      m_nScanCode = 0;
      m_nChar = 0;
      m_nRepCnt = 0;
      m_iCode = 0;
      m_bExt = false;
      m_nFlags = 0;
   }


   key::key(const key & key):
      ::object(key),
      ::user::key(key)
   {

      operator = (key);

   }


   void key::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {

      base::set(pwnd,uiMessage,wparam,lparam,lresult);

      m_nChar = static_cast<UINT>(wparam);

      m_nRepCnt = LOWORD(lparam);

      m_nFlags = HIWORD(lparam);

      m_nScanCode = ((lparam >> 16) & 0xff);

      m_bExt = (lparam & (1 << 24)) != 0;

      Session.translate_os_key_message(this);

   }


   key & key::operator = (const key & key)
   {

      if(this != &key)
      {

         ::message::message::copy_this(key);

         ::message::base::copy_this(key);

         ::user::key::copy_this(key);

      }

      return *this;

   }




   nc_activate::nc_activate(::aura::application * papp):
      ::object(papp),
      ::message::base(papp)
   {
   }

   void nc_activate::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_bActive = wparam != FALSE;
   }

   void size::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nType     = static_cast < UINT > (wparam);
      m_size      = ::size(LOWORD(lparam),HIWORD(lparam));
   }

   mouse::mouse(::aura::application * papp):
      ::object(papp),
      ::message::base(papp)
   {
      m_ecursor = ::visual::cursor_unmodified;
      m_pcursor = NULL;
      m_bTranslated = false;
   }

   mouse::~mouse()
   {

      try
      {

         if(m_pcursor != NULL && m_pauraapp != NULL && m_pauraapp->m_pbasesession != NULL)
         {

            Session.set_cursor(m_pcursor);

         }
         else if(m_ecursor != ::visual::cursor_unmodified && m_pauraapp != NULL && m_pauraapp->m_pbasesession != NULL)
         {

            Session.set_cursor(m_ecursor);

         }

      }
      catch(...)
      {
      }
   }

   void mouse::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nFlags    = wparam;
      m_pt        = point(lparam);
#ifdef LINUX
      m_bTranslated = true;  // in root coordinates
#elif defined(WINDOWS)
      m_bTranslated = false; // not in root coordinates
#else
      m_bTranslated = false; // not in root coordinates
#endif
   }

   void mouse_wheel::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nFlags    = wparam;
      m_pt        = point(lparam);
      m_bTranslated = true;
   }

   ::user::interaction * mouse_activate::GetDesktopWindow()
   {
      ::exception::throw_not_implemented(get_app());
      //      return interaction_impl::from_handle_dup(reinterpret_cast<oswindow>(m_wparam));
      return NULL;
   }

   UINT mouse_activate::GetHitTest()
   {
      return LOWORD(m_lparam);
   }

   UINT mouse_activate::get_message()
   {
      return HIWORD(m_lparam);
   }

   //::window_sp context_menu::GetWindow()
   //{
   //   ::exception::throw_not_implemented(get_app());
   //   return NULL;
   //   //            return interaction_impl::from_handle_dup(reinterpret_cast<oswindow>(m_wparam));
   //}

   point context_menu::GetPoint()
   {
      return point(m_lparam);
   }


   void scroll::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nSBCode = (int16_t)LOWORD(wparam);
      m_nPos = (int16_t)HIWORD(wparam);
      m_pScrollBar = (::user::primitive *) lparam;
   }

   void show_window::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_bShow = wparam != FALSE;
      m_nStatus = static_cast<UINT>(lparam);
   }

   void set_focus::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      //m_pwnd = System.ui_from_handle(reinterpret_cast<oswindow>(wparam));
      m_pwnd = NULL;
   }

   void window_pos::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_pwindowpos = reinterpret_cast<WINDOWPOS*>(lparam.m_lparam);
   }

   void nc_calc_size::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_pparams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam.m_lparam);
   }

   bool nc_calc_size::GetCalcValidRects()
   {
      return m_wparam != FALSE;
   }


   bool enable::get_enable()
   {
      return m_wparam != 0;
   }





   UINT mouse_wheel::GetFlags()
   {
      return LOWORD(m_wparam);
   }

   int16_t mouse_wheel::GetDelta()
   {
      return (int16_t)HIWORD(m_wparam);
   }

   point mouse_wheel::GetPoint()
   {
      return point(GET_X_LPARAM(m_lparam),GET_Y_LPARAM(m_lparam));
   }

   UINT command::GetNotifyCode()
   {
      return HIWORD(m_wparam);
   }

   UINT command::GetId()
   {
      return LOWORD(m_wparam);
   }

#ifdef WINDOWS

   oswindow command::get_oswindow()
   {
      return (oswindow)m_lparam.m_lparam;
   }

#endif

#ifdef WINDOWSEX

   LPNMHDR notify::get_lpnmhdr()
   {

      return (LPNMHDR)m_lparam.m_lparam;

   }

#endif

   int32_t notify::get_ctrl_id()
   {

      return (int32_t)m_wparam;

   }


} // namespace message







