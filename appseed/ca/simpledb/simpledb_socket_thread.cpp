#include "framework.h"


namespace simpledb
{


   socket_thread::socket_thread(::ca::application * papp) :
      ca(papp),
      thread(papp),
      ::sockets::socket_handler(papp),
      m_evCanInit(papp),
      m_evInitialized(papp)
   {
      m_psocket = ::null();
      m_evCanInit.ResetEvent();
      m_evInitialized.ResetEvent();
      m_bInitialized = false;

   }

   socket_thread::~socket_thread()
   {
   }

   void socket_thread::install_message_handling(::ca::message::dispatch * pinterface)
   {
      IGUI_WIN_MSG_LINK(WM_APP, pinterface, this, &socket_thread::OnApp);
   }


   bool socket_thread::initialize_instance()
   {
      //Application.::simpledb::application::m_pmanager->LoadEnv();


      /*   STARTUPINFO startupinfo;
      memset(&startupinfo, 0, sizeof(startupinfo));
      if(!::CreateProcess(::null(), "netnoderewrite.exe",
      ::null(),
      ::null(),
      TRUE,
      0,
      ::null(),
      ".",
      &startupinfo,
      &m_pi))
      {
      return FALSE;
      }


      WaitForInputIdle(m_pi.hProcess, INFINITE);


      //   m_hChildThreadId = ::OpenThread(THREAD_ALL_ACCESS, TRUE, m_pi.dwThreadId);

      ::post_thread_message(m_pi.dwThreadId, WM_APP, 0, m_nThreadID);

      post_thread_message(WM_APP, 0, 0);*/




      /*   m_plistensocket = new netnodeListenSocket(this);
      //Sleep(15000);
      if(!m_plistensocket->create(80, SOCK_STREAM))
      {
      uint32_t dw = ::GetLastError();
      TRACE("error %u", dw);
      return FALSE;
      }
      if(!m_plistensocket->Listen())
      {
      uint32_t dw = ::GetLastError();
      TRACE("error %u", dw);
      return FALSE;
      }*/
      m_evInitialized.SetEvent();
      return true;
   }


   void socket_thread::OnApp(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::base, pbase, pobj);
      if(pbase->m_wparam == 0)
      {

         while(m_pservice->get_run())
         {
            try
            {

               EnablePool();
               ::sockets::listen_socket < socket > ll(*this);
               ll.m_bDetach = true;
               while(true)
               {
                  //m_strIp = "127.0.0.1";
                  if(m_iPort == 443)
                  {
                     ll.EnableSSL();
                  }
                  if (ll.Bind(m_strIp, (port_t) m_iPort))
                  {
                     string strMessage;
                     strMessage.Format("could not bind to address %s %d", m_strIp, m_iPort);
                     TRACE(strMessage);
                     //System.simple_message_box(::null(), strMessage);
                     return;
                  }
                  ::sockets::socket_handler::add(&ll);
                  while (true)
                  {
                     Select(8,0);
                     if(!m_pservice-get_run())
                        break;
                  }
                  if(!m_pservice->get_run())
                     break;
               }
            }
            catch(...)
            {
            }
         }
      }
      if(pbase->m_wparam == 0)
      {
         m_bInitialized = true;
         m_evInitialized.SetEvent();
      }
      else if(pbase->m_wparam == 1)
      {

#ifdef WINDOWSEX

         ::PostThreadMessageA(m_pi.dwThreadId, WM_APP, 1, 0);

#else

         throw not_implemented(get_app());

#endif

      }

   }


} // namespace simpledb




