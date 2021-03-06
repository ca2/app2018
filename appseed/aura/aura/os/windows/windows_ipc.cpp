#include "framework.h"


typedef int (WINAPI * LPFN_ChangeWindowMessageFilter)(UINT message, DWORD dwFlag);



extern LPFN_ChangeWindowMessageFilter g_pfnChangeWindowMessageFilter;

namespace aura
{


   namespace ipc
   {


      ATOM rx_register_class(HINSTANCE hInstance);
      LRESULT CALLBACK s_rx_message_queue_proc(oswindow oswindow,UINT message,WPARAM wparam,LPARAM lparam);


      base::base(::aura::application * papp) :
         object(papp)
      {

         m_oswindow = NULL;

      }


      base::~base()
      {

         if(m_oswindow != NULL)
         {

            ::DestroyWindow(m_oswindow);

            m_oswindow = NULL;

         }

      }


      tx::tx(::aura::application * papp):
         object(papp),
         base(papp)
      {

      }


      tx::~tx()
      {

      }


      bool tx::open(const char * pszKey,launcher * plauncher)
      {

         if(m_oswindow != NULL)
            close();


         int jCount = 23;
         int iCount;

         if(plauncher != NULL)
            iCount = plauncher->m_iStart + 1;
         else
            iCount = 2;

         m_oswindow = NULL;

         for(int i = 0; i < iCount; i++)
         {
            for(int j = 0; j < jCount; j++)
            {
               m_oswindow = ::FindWindow(NULL,pszKey);
               if(m_oswindow != NULL)
                  break;
               //         m_oswindow = FindDesktopWindow(pszKey);
               //       if(m_oswindow != NULL)
               //        break;
               if(i <= 0)
               {
                  break;
               }
               int k = 40;
               while (k > 0)
               {
                  if (!get_thread_run())
                  {
                     return false;
                  }
                  Sleep(25);
                  k--;
               }
            }
            if(m_oswindow != NULL)
               break;
            if(plauncher != NULL)
            {
               if(plauncher->m_iStart <= 0)
                  return false;
               plauncher->start();
               plauncher->m_iStart--;
            }
         }
         m_strBaseChannel = pszKey;
         return true;

      }

      bool tx::close()
      {

         if(m_oswindow == NULL)
            return true;

         m_oswindow = NULL;

         m_strBaseChannel = "";

         return true;

      }


      bool tx::send(const char * pszMessage, duration durationTimeout)
      {

         if(!is_tx_ok())
            return false;

         COPYDATASTRUCT cds;

         cds.dwData = 0x80000000;
         cds.cbData = (unsigned int)strlen(pszMessage);
         cds.lpData = (void *)pszMessage;

         if(durationTimeout.is_pos_infinity())
         {

            SendMessage(m_oswindow,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds);

         }
         else
         {

            DWORD_PTR dwptr;

            if(!::SendMessageTimeout(m_oswindow,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds, SMTO_ABORTIFHUNG, (UINT)(durationTimeout.get_total_milliseconds()),&dwptr))
               return false;

            unsigned int dwError = ::get_last_error();

            if(dwError == ERROR_TIMEOUT)
               return false;

         }

         return true;
      }

      bool tx::send(int message, void * pdata, int len, duration durationTimeout)
      {

         if(message == 0x80000000)
            return false;

         if(!is_tx_ok())
            return false;

         COPYDATASTRUCT cds;

         cds.dwData = (unsigned int)message;
         cds.cbData = (unsigned int)MAX(0,len);
         cds.lpData = (void *)pdata;

         if(durationTimeout.is_pos_infinity())
         {

            if(message >= WM_APP)
            {

               SendMessage(m_oswindow,message,0,0);

            }
            else
            {

               SendMessage(m_oswindow,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds);

            }

         }
         else
         {

            DWORD_PTR dwptr;

            if(!::SendMessageTimeout(m_oswindow,WM_COPYDATA,(WPARAM)0,(LPARAM)&cds,SMTO_BLOCK, (UINT)(durationTimeout.get_total_milliseconds()), &dwptr))
               return false;

            unsigned int dwError = ::get_last_error();

            if(dwError == ERROR_TIMEOUT)
               return false;

         }

         return true;

      }



      bool tx::is_tx_ok()
      {

         return ::IsWindow(m_oswindow) != FALSE;

      }




      rx::rx(::aura::application * papp) :
         object(papp),
         base(papp)
      {

         m_preceiver    = NULL;

      }


      rx::~rx()
      {

      }


      bool rx::create(const char * pszKey)
      {


         if(g_pfnChangeWindowMessageFilter != NULL)
         {
            g_pfnChangeWindowMessageFilter(WM_COPYDATA,MSGFLT_ADD);
         }

         HINSTANCE hinstance = ::GetModuleHandleA("aura.dll");

         if(hinstance == NULL)
         {

            hinstance = ::GetModuleHandleA(NULL);

         }

         ATOM atom = rx_register_class(hinstance);

         m_oswindow = ::CreateWindowExA(0,"small_ipc_rx_channel_message_queue_class",pszKey,0,0,0,0,0,HWND_MESSAGE,NULL,hinstance,NULL);

         if(m_oswindow == NULL)
         {
            unsigned int dwLastError = ::get_last_error();
            return false;
         }

         SetTimer(m_oswindow,888888,84,NULL);

         SetWindowLongPtr(m_oswindow,GWLP_USERDATA,(LONG_PTR) this);

         //m_strWindowProcModule = pszWindowProcModule;

         return true;

      }


      bool rx::destroy()
      {

         if(m_oswindow != NULL)
         {
            ::DestroyWindow(m_oswindow);
            m_oswindow = NULL;
         }

         return true;

      }

      void rx::receiver::on_receive(rx * prx,const char * pszMessage)
      {
      }

      void rx::receiver::on_receive(rx * prx,int message,void * pdata,memory_size_t len)
      {
      }

      void rx::receiver::on_post(rx * prx,long long int a,long long int b)
      {
      }

      void * rx::on_receive(rx * prx,const char * pszMessage)
      {

         string strMessage(pszMessage);

         if (::str::begins_ci(strMessage, "synch_"))
         {

            if (m_preceiver != NULL)
            {

               m_preceiver->on_receive(prx, strMessage);

            }

         }
         else
         {

            ::fork(get_app(), [=]()
            {

               if (m_preceiver != NULL)
               {

                  m_preceiver->on_receive(prx, strMessage);

               }

            });

         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }

      void * rx::on_receive(rx * prx,int message,void * pdata,memory_size_t len)
      {

         if(m_preceiver != NULL)
         {
            m_preceiver->on_receive(prx,message,pdata,len);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }


      void * rx::on_post(rx * prx,long long int a,long long int b)
      {

         if(m_preceiver != NULL)
         {
            m_preceiver->on_post(prx,a,b);
         }

         // ODOW - on date of writing : return ignored by this windows implementation

         return NULL;

      }


      LRESULT CALLBACK s_rx_message_queue_proc(oswindow oswindow,UINT message,WPARAM wparam,LPARAM lparam)
      {

         int iRet = 0;

         rx * pchannel = (rx *)GetWindowLongPtr(oswindow,GWLP_USERDATA);

         if(pchannel == NULL)
         {

            return ::DefWindowProcA(oswindow,message,wparam,lparam);

         }
         else
         {

            return pchannel->message_queue_proc(message,wparam,lparam);

         }

      }



      ATOM rx_register_class(HINSTANCE hInstance)
      {
         WNDCLASSEX wcex;

         wcex.cbSize = sizeof(WNDCLASSEX);

         wcex.style           = 0;
         wcex.lpfnWndProc     = &s_rx_message_queue_proc;
         wcex.cbClsExtra      = 0;
         wcex.cbWndExtra      = 0;
         wcex.hInstance       = hInstance;
         wcex.hIcon           = NULL;
         //wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
         wcex.hCursor         = NULL;
         wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW + 1);
         wcex.lpszMenuName    = NULL;
         wcex.lpszClassName   = "small_ipc_rx_channel_message_queue_class";
         wcex.hIconSm         = NULL;

         return RegisterClassEx(&wcex);
      }


      LRESULT rx::message_queue_proc(UINT message,WPARAM wparam,LPARAM lparam)
      {

         if(message == WM_USER + 100)
         {

            on_post(this,wparam,lparam);

         }
         else if(message == WM_COPYDATA)
         {

            COPYDATASTRUCT * pcds = (COPYDATASTRUCT *)lparam;

            if(pcds == NULL)
            {

               return 0;

            }
            else if(pcds->dwData == 0x80000000)
            {

               string strMessage((const char *)pcds->lpData,pcds->cbData);

               on_receive(this,strMessage.c_str());

            }
            else
            {

               on_receive(this,(int)pcds->dwData,pcds->lpData,pcds->cbData);

            }

         }
         else if(message >= WM_APP)
         {

            on_receive(this,message,(void *)wparam,lparam);

         }
         else
         {

            return ::DefWindowProcA(m_oswindow,message,wparam,lparam);

         }

         return 0;

      }



      bool rx::on_idle()
      {

         return false;

      }

      bool rx::is_rx_ok()
      {

         return ::IsWindow(m_oswindow) != FALSE;

      }


      ipc::ipc(::aura::application * papp):
         object(papp),
         base(papp),
         tx(papp),
         m_rx(papp)
      {

         m_dwTimeout = (5000) * 11;

      }


      ipc::~ipc()
      {


      }


      bool ipc::open_ab(const char * pszKey,const char * pszModule,launcher * plauncher)
      {

         m_strChannel = pszKey;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel + "-a";
         string strChannelTx = m_strChannel + "-b";

         if(!::IsWindow(m_rx.m_oswindow))
         {

            if(!m_rx.create(strChannelRx.c_str()))
            {

               return false;

            }

         }

         if(!tx::open(strChannelTx.c_str(),plauncher))
         {

            return false;

         }

         return true;

      }

      bool ipc::open_ba(const char * pszKey,const char * pszModule,launcher * plauncher)
      {

         m_strChannel = pszKey;

         m_rx.m_preceiver = this;

         string strChannelRx = m_strChannel + "-b";
         string strChannelTx = m_strChannel + "-a";


         if(!::IsWindow(m_rx.m_oswindow))
         {

            if(!m_rx.create(strChannelRx.c_str()))
            {

               return false;

            }

         }

         if(!tx::open(strChannelTx.c_str(),plauncher))
         {
            return false;
         }

         return true;

      }


      bool ipc::is_rx_tx_ok()
      {

         return m_rx.is_rx_ok() && is_tx_ok();

      }


   } // namespace ipc


} // namespace aura






