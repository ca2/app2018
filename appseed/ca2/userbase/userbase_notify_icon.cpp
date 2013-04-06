#include "framework.h"


namespace userbase
{

   notify_icon::notify_icon(sp(::ca::application) papp) :
      ca(papp)
   {
#ifdef WINDOWSEX      
      m_nid.cbSize = sizeof(m_nid);
#endif
      m_bCreated = false;

   }

   notify_icon::~notify_icon()
   {
      Destroy();
   }

   void notify_icon::install_message_handling(::ca::message::dispatch * pinterface)
   {
      IGUI_WIN_MSG_LINK(MessageNotifyIcon, pinterface, this, &notify_icon::_001OnNotifyIconMessage);
   }

   bool notify_icon::create(UINT uiId, notify_icon_listener * plistener, HICON hicon)
   {

      if(m_bCreated)
         return false;

      string strNotifyIcon;

      strNotifyIcon.Format("userbase::notify_icon - %d", uiId);

      if(!create_message_window(strNotifyIcon))
         return false;

      m_uiId                     = uiId;
#ifdef WINDOWSEX
      m_nid.hWnd                 = get_safe_handle();
      m_nid.uID                  = uiId;
      m_nid.hIcon                = hicon;
      m_nid.uFlags               = NIF_ICON | NIF_MESSAGE;
      m_nid.uCallbackMessage     = MessageNotifyIcon;
#else
      throw todo(get_app());
#endif
      m_plistener                = plistener;

#ifdef WINDOWSEX
      if(!Shell_NotifyIcon(NIM_ADD, &m_nid))
      {
         DestroyWindow();
         return false;
      }
#else
      throw todo(get_app());
#endif

      m_bCreated = true;

      return true;

   }

   bool notify_icon::ModifyIcon(HICON hicon)
   {

      if(!m_bCreated)
         return false;

#ifdef WINDOWSEX
      m_nid.hIcon       = hicon;
      m_nid.uFlags      = NIF_ICON;


      if(!Shell_NotifyIcon(NIM_MODIFY, &m_nid))
      {
         return false;
      }

#else
      throw todo(get_app());
#endif

      return true;

   }


   bool notify_icon::Destroy()
   {
      
      if(!m_bCreated)
         return false;

#ifdef WINDOWSEX
      m_nid.uFlags = 0;

      if(!Shell_NotifyIcon(NIM_DELETE, &m_nid))
      {
         return false;
      }
#else
      throw todo(get_app());
#endif

      m_bCreated = false;

      DestroyWindow();

      return true;

   }

   void notify_icon::_001OnNotifyIconMessage(::ca::signal_object * pobj)
   {
      
      SCAST_PTR(::ca::message::base, pbase, pobj);

      m_plistener->OnNotifyIconMessage(m_uiId, (UINT) pbase->m_lparam);

   }


} // namespace userbase

