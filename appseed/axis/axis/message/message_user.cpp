//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"



#if defined(LINUX)
// Ubuntu apt-get install libx11-dev
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>

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


   void create::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_lpcreatestruct = reinterpret_cast<::user::create_struct *>(lparam);
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
      TRACE0(lpcszErrorMessage);
      System.log().print(lpcszErrorMessage);
   }

   void timer::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nIDEvent = static_cast<UINT>(wparam);
   }

   activate::activate(::aura::application * papp):
      
      ::message::base(papp)
   {
      }

   void activate::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      
      m_nState = (UINT)(LOWORD(wparam));
      
      if(lparam == 0)
      {
       
         m_pWndOther = NULL;

      }
      else
      {
         
         m_pWndOther = System.window_from_os_data((void *)lparam);

      }
      
      m_bMinimized = HIWORD(wparam) != FALSE;

   }



   erase_bkgnd::erase_bkgnd(::aura::application * papp):
      
      ::message::base(papp)
   {
      }

   void erase_bkgnd::set_result(bool bResult)
   {
      set_lresult(bResult);
   }


   key::key(::aura::application * papp):
      
      ::message::base(papp)
   {

   }


   key::key(const key & key) :
      ::message::base(key)
   {

      operator = (key);

   }


   void key::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {

      base::set(pwnd,uiMessage,wparam,lparam,lresult);

      m_nChar = static_cast<UINT>(wparam);

      m_nRepCnt = LOWORD(lparam);

      m_nFlags = HIWORD(lparam);

      m_nScanCode = ((lparam >> 16) & 0xff);

      m_bExt = (lparam & (1 << 24)) != 0;

      Session.user()->keyboard().translate_os_key_message(this);

   }


   key & key::operator = (const key & key)
   {

      if(this != &key)
      {

         m_nChar = key.m_nChar;
         m_nScanCode = key.m_nScanCode;
         m_nRepCnt = key.m_nRepCnt;
         m_nFlags = key.m_nFlags;
         m_bExt = key.m_bExt;
         m_ekey = key.m_ekey;
         m_strText = key.m_strText;

      }

      return *this;

   }


   nc_activate::nc_activate(::aura::application * papp):
      
      ::message::base(papp)
   {
      }

   void nc_activate::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_bActive = wparam != FALSE;
   }

   void size::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nType     = static_cast < UINT > (wparam);
      m_size      = ::size(LOWORD(lparam),HIWORD(lparam));
   }

   mouse::mouse(::aura::application * papp):
      
      ::message::base(papp),
      m_ecursor(::visual::cursor_unmodified)
   {
      m_pcursor = NULL;
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

   void mouse::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
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

   void mouse_wheel::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
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

   ::window_sp context_menu::GetWindow()
   {
      ::exception::throw_not_implemented(get_app());
      return NULL;
      //            return interaction_impl::from_handle_dup(reinterpret_cast<oswindow>(m_wparam));
   }

   point context_menu::GetPoint()
   {
      return point(m_lparam);
   }


   void scroll::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nSBCode = (int16_t)LOWORD(wparam);
      m_nPos = (int16_t)HIWORD(wparam);
      m_pScrollBar = (::aura::interaction *) lparam;
   }

   void show_window::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_bShow = wparam != FALSE;
      m_nStatus = static_cast<UINT>(lparam);
   }

   void set_focus::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      //m_pwnd = System.window_from_os_data(reinterpret_cast<oswindow>(wparam));
      m_pwnd = NULL;
   }

   void window_pos::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_pwindowpos = reinterpret_cast<WINDOWPOS*>(lparam);
   }

   void nc_calc_size::set(::aura::interaction * pwnd,UINT uiMessage,WPARAM wparam,LPARAM lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_pparams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);
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

   int32_t notify::get_ctrl_id()
   {
      return (int32_t)m_wparam;
   }

#endif




} // namespace message







