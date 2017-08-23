#include "framework.h"

static ::user::notify_icon * g_pnotifyiconLast = NULL;


#ifdef LINUX

   #include <dlfcn.h>
   BEGIN_EXTERN_C
   typedef void * BASECORE_APP_INDICATOR_NEW(const char *, const char *, const char *, i_close_quit *);
   typedef void BASECORE_APP_INDICATOR_TERM(void *);
   typedef void BASECORE_APP_INDICATOR_STEP(void *);
   END_EXTERN_C
   extern void * g_pbasecore;

#endif


namespace user
{


   notify_icon::notify_icon(::aura::application * papp) :
      object(papp)
   {

      g_pnotifyiconLast = this;

#ifdef WINDOWSEX

      m_nid.cbSize = sizeof(m_nid);

#elif defined(LINUX)

      m_pindicator = NULL;

#endif

      m_bCreated = false;

   }


   notify_icon::~notify_icon()
   {

      Destroy();

      if(g_pnotifyiconLast== this)
      {

         g_pnotifyiconLast = NULL;

      }

   }


   void notify_icon::install_message_routing(::message::sender * pinterface)
   {

#ifdef WINDOWSEX

      IGUI_WIN_MSG_LINK(MessageNotifyIcon, pinterface, this, &notify_icon::_001OnNotifyIconMessage);

#endif

   }


   bool notify_icon::create_notify_icon(UINT uiId, notify_icon_listener * plistener, ::visual::icon * pvisualicon)
   {

      if(m_bCreated)
      {

         return false;

      }

      m_strId.Format("user::notify_icon - %d", uiId);

      m_strId = "ca2-" + pvisualicon->m_strAppTrayIcon + "-" + m_strId;

#ifdef WINDOWSEX

      if(!create_message_queue(m_strId))
      {

         return false;

      }

#endif

      m_uiId                     = uiId;

#ifdef WINDOWSEX

      m_nid.hWnd                 = get_safe_handle();
      m_nid.uID                  = uiId;
      m_nid.hIcon                = *pvisualicon;
      m_nid.uFlags               = NIF_ICON | NIF_MESSAGE;
      m_nid.uCallbackMessage     = MessageNotifyIcon;

#elif defined(LINUX)

#elif defined(MACOS)

#elif defined(METROWIN)

#elif defined(VSNORD)

#elif defined(APPLE_IOS)

#else
      throw todo(get_app());

#endif

      m_plistener                = plistener;

#ifdef WINDOWSEX

      if(!Shell_NotifyIcon(NIM_ADD, &m_nid))
      {

         m_plistener = NULL;

         DestroyWindow();

         return false;

      }

#elif defined(LINUX)

      {

         BASECORE_APP_INDICATOR_NEW* f = (BASECORE_APP_INDICATOR_NEW *) dlsym(g_pbasecore, "basecore_app_indicator_new");

         string strFolder(System.dir().element());

         strFolder += "/";

         string str1 = hicon->m_strAppTrayIcon;

         str1.replace("-", "_");

         str1.replace("/", "_");

         str1.replace("\\", "_");

         string str(str1);

         if(::str::begins_eat_ci(str, "app_veriwell_"))
         {

            strFolder+="app-veriwell";

         }
         else if(::str::begins_eat_ci(str, "app_core_"))
         {

            strFolder+="app-core";

         }
         else
         {

            strFolder+="app";

         }

         strFolder+= "/appmatter/" + str;

         strFolder += "/_std/_std/main/";

         m_pindicator = (*f)(m_strId,str1+"_128", strFolder, this);

      }

      if(m_pindicator == NULL)
      {

         m_plistener = NULL;

         //DestroyWindow();

         return false;

      }

#elif defined(MACOS)

      string strFolder;

      string str1 = hicon->m_strAppTrayIcon;

      str1.replace("-", "_");

      str1.replace("/", "_");

      str1.replace("\\", "_");

      string str(str1);

      if(::str::begins_eat_ci(str, "app_veriwell_"))
      {

         strFolder+="app-veriwell";

      }
      else if(::str::begins_eat_ci(str, "app_core_"))
      {

         strFolder+="app-core";

      }
      else
      {

         strFolder+="app";

      }

      strFolder+= "/appmatter/" + str;

      strFolder += "/_std/_std/main/";

      string strFile = "menubar-icon-22.png";

      string strUrl = "https://server.ca2.cc/matter/" + strFolder + strFile;

      strFile = Application.dir().userappdata() / strFolder / strFile;

      int iRetry = 3;

      while(iRetry >= 0 && (!Application.file().exists(strFile) || Application.file().length(strFile) <= 0))
      {

         ::property_set set;

         set["raw_http"] = true;
         set["disable_common_name_cert_check"] = true;

         Application.http().download(strUrl, strFile, set);

         iRetry--;

      }

      notify_icon_init(strFile);

#else

#endif

      m_bCreated = true;

      return true;

   }


   bool notify_icon::ModifyIcon(sp(::visual::icon) hicon, bool bForce)
   {

      if(!m_bCreated)
      {

         return false;

      }

      if (!bForce && hicon == m_piconCurrent)
      {

         return true;

      }

#ifdef WINDOWSEX

      m_nid.hIcon       = (HICON) *hicon;

      m_nid.uFlags      = NIF_ICON;


      if(!Shell_NotifyIcon(NIM_MODIFY, &m_nid))
      {

         return false;

      }

#else

      throw todo(get_app());

#endif

      m_piconCurrent = hicon;

      return true;

   }


   void notify_icon::AddHiddenWindow(sp(::user::interaction) pwnd)
   {

      m_wndptraHidden.add_unique(pwnd);

   }


   bool notify_icon::Destroy()
   {

      if(!m_bCreated)
      {

         return false;

      }

#ifdef WINDOWSEX

      m_nid.uFlags = 0;

      if(!Shell_NotifyIcon(NIM_DELETE, &m_nid))
      {

         return false;

      }

      DestroyWindow();

#elif defined(LINUX)

      {

         BASECORE_APP_INDICATOR_TERM * f =  (BASECORE_APP_INDICATOR_TERM *) dlsym(g_pbasecore, "basecore_app_indicator_term");

         (*f)(m_pindicator);

      }

#else

      throw todo(get_app());

#endif

      m_bCreated = false;

      return true;

   }


   void notify_icon::_001OnNotifyIconMessage(::message::message * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      if (pbase->m_lparam == WM_LBUTTONDOWN)
      {

         while (m_wndptraHidden.get_size() > 0)
         {

            try
            {

               sp(::user::interaction) pframe = (m_wndptraHidden.element_at(0));

               if (pframe != NULL)
               {

                  pframe->WfiRestore();

               }
               else
               {

                  m_wndptraHidden.element_at(0)->ShowWindow(SW_SHOW);

               }

            }
            catch (...)
            {

            }

            m_wndptraHidden.remove_at(0);

         }

      }

      m_plistener->OnNotifyIconMessage(m_uiId, (UINT) pbase->m_lparam);

   }


   void notify_icon::notify_icon_play(const char * action)
   {

      string strAction(action);

      if(strAction == "close")
      {

         __close();

      }
      else if(strAction== "quite")
      {

         __quit();

      }

   }


   void notify_icon::__close()
   {

      m_plistener->OnNotifyIconMessage(m_uiId, WM_CLOSE);

   }


   void notify_icon::__quit()
   {

      m_plistener->OnNotifyIconMessage(m_uiId, WM_QUIT);

   }


   bool notify_icon::__close_is_closed()
   {

      return m_plistener->__close_is_closed();

   }


   bool notify_icon::notify_icon_frame_is_opened()
   {

      return m_plistener->notify_icon_frame_is_opened();

   }


   void notify_icon::step()
   {

#if defined(LINUX)

      if(m_pindicator != NULL)
      {

         BASECORE_APP_INDICATOR_STEP* f = (BASECORE_APP_INDICATOR_STEP *) dlsym(g_pbasecore, "basecore_step");

         (*f)(m_pindicator);

      }

#endif

   }


} // namespace user





