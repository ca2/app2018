#include "StdAfx.h"


 // BEGIN_MESSAGE_MAP(netnodeSocketThread, thread)
   //ON_THREAD_MESSAGE(WM_APP, OnApp)
 // END_MESSAGE_MAP()



netnodeSocketThread::netnodeSocketThread(::ca::application * papp)
: thread(papp), ca(papp)
{
   m_psocket = NULL;
   m_evCanInit.ResetEvent();
   m_evInitialized.ResetEvent();
   m_bInitialized = false;

}

netnodeSocketThread::~netnodeSocketThread(void)
{
}

void netnodeSocketThread::_001InstallMessageHandling(::user::win::message::dispatch * pinterface)
{
   IGUI_WIN_MSG_LINK(WM_APP, pinterface, this, &netnodeSocketThread::OnApp);
}


bool netnodeSocketThread::initialize_instance()
{
   Application.get_script_manager()->LoadEnv();


/*   STARTUPINFO startupinfo;
   memset(&startupinfo, 0, sizeof(startupinfo));
   if(!::CreateProcess(NULL, "netnoderewrite.exe", 
      NULL,
      NULL,
      TRUE,
      0, 
      NULL,
      ".",
      &startupinfo,
      &m_pi))
   {
      return FALSE;
   }

   
   WaitForInputIdle(m_pi.hProcess, INFINITE);


//   m_hChildThreadId = ::OpenThread(THREAD_ALL_ACCESS, TRUE, m_pi.dwThreadId);

   ::PostThreadMessage(m_pi.dwThreadId, WM_APP, 0, m_nThreadID);

   PostThreadMessage(WM_APP, 0, 0);*/

   


/*   m_plistensocket = new netnodeListenSocket(this);
   //Sleep(15000);
   if(!m_plistensocket->create(80, SOCK_STREAM))
   {
      DWORD dw = ::GetLastError();
      TRACE("error %u", dw);
      return FALSE;
   }
   if(!m_plistensocket->Listen())
   {
      DWORD dw = ::GetLastError();
      TRACE("error %u", dw);
      return FALSE;
   }*/
   m_evInitialized.SetEvent();
   return true;
}


void netnodeSocketThread::OnApp(gen::signal_object * pobj)
{
   SCAST_PTR(::user::win::message::base, pbase, pobj);
   if(pbase->m_wparam == 0)
   {
      // o Telmo me perguntou hoje no almo�o se meu fim-de-semana foi while-true - recursivo - pensamento, apesar de while-true ser iterativo
      // que lindinho
      // precisa de um while quase true aqui, in�cion de 2011
      while(!m_pservice->m_stopping)
      {
         try
         {
            ::sockets::socket_handler h(get_app());
            h.EnablePool();
            ::sockets::listen_socket<netnodeSocket> ll(h);
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
                  //System.simple_message_box(NULL, strMessage);
                  return;
               }
               h.add(&ll);
               while (true)
               {
                  h.Select(8,0);
                  if(m_pservice->m_stopping)
                     break;
               }
               if(m_pservice->m_stopping)
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
      ::PostThreadMessage(m_pi.dwThreadId, WM_APP, 1, 0);
   }
}

