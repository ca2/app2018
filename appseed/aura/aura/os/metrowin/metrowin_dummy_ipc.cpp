#include "framework.h"


//
//
///*
//extern LPFN_ChangeWindowMessageFilter g_pfnChangeWindowMessageFilter;
//
//
//namespace aura
//{
//
//
//   namespace ipc
//   {
//
//
//      base::base(::aura::application * papp) :
//         object(papp)
//      {
//
//         m_iSerial = 1;
//
//      }
//
//
//      base::~base()
//      {
//
//         //if(m_oswindow != NULL)
//         //{
//
//         //   ::DestroyWindow(m_oswindow);
//
//         //   m_oswindow = NULL;
//
//         //}
//
//      }
//
//
//      tx::tx(::aura::application * papp):
//         object(papp),
//         base(papp)
//      {
//
//
//      }
//
//
//      tx::~tx()
//      {
//
//
//      }
//
//
//      bool tx::open(const char * pszKey,launcher * plauncher)
//      {
//
//         m_strBaseChannel = "core\\appdata\\ipi\\" + string(pszKey);
//
//         if(m_strBaseChannel.is_empty())
//         {
//
//            return false;
//
//         }
//
//         return true;
//
//      }
//
//
//      bool tx::close()
//      {
//
//         //if(m_oswindow == NULL)
//         //   return true;
//
//         //m_oswindow = NULL;
//
//         //m_strBaseChannel = "";
//
//         return true;
//
//      }
//
//
//      bool tx::send(const char * pszMessage,unsigned int dwTimeout)
//      {
//
//         ::Windows::Storage::StorageFolder ^ folder = ::Windows::Storage::KnownFolders::DocumentsLibrary;
//         
//         ::Windows::Storage::StorageFolder ^ folderTopic = ::wait(folder->CreateFolderAsync(wstring(m_strBaseChannel),::Windows::Storage::CreationCollisionOption::OpenIfExists));
//
//         ::Windows::Storage::StorageFile ^ file = ::wait(folderTopic->CreateFileAsync(wstring("core.topic"),::Windows::Storage::CreationCollisionOption::OpenIfExists));
//
//         string strMessage;
//
//         strMessage = pszMessage;
//
//         strMessage += "\n";
//
//         ::wait(::Windows::Storage::FileIO::AppendTextAsync(file,wstring(strMessage)));
//         
//         return true;
//
//      }
//
//
//      bool tx::send(int message,void * pdata,int len,unsigned int dwTimeout)
//      {
//
//
//         return true;
//
//      }
//
//
//
//      bool tx::is_tx_ok()
//      {
//
////         return ::IsWindow(m_oswindow) != FALSE;
//         return m_strBaseChannel.has_char();
//
//      }
//
//
//      class rx_private
//      {
//      public:
//
//         
//         ::Windows::Storage::StorageFolder ^                      folder;
//         ::Windows::Storage::StorageFolder ^                      folderTopic;
//         ::Windows::Storage::Search::StorageFileQueryResult ^     result;
//
//
//      };
//
//      
//
//
//
//
//      rx::rx(::aura::application * papp) :
//         object(papp),
//         base(papp)
//      {
//
//         m_preceiver    = NULL;
//
//
//      }
//
//
//      rx::~rx()
//      {
//
//      }
//
//
//      bool rx::create(const char * pszKey)
//      {
//
//
//         if(g_pfnChangeWindowMessageFilter != NULL)
//         {
//            g_pfnChangeWindowMessageFilter(WM_COPYDATA,MSGFLT_ADD);
//         }
//
//         m_strBaseChannel = "core\\appdata\\ipi\\" + string(pszKey);
//
//         if(m_strBaseChannel.is_empty())
//         {
//
//            return false;
//
//         }
//
//         m_pp = new rx_private;
//
//         try
//         {
//
//            m_pp->folder = ::Windows::Storage::KnownFolders::DocumentsLibrary;
//
//         }
//         catch (...)
//         {
//
//            return false;
//
//         }
//
//         m_pp->folderTopic = ::wait(m_pp->folder->CreateFolderAsync(wstring(m_strBaseChannel),::Windows::Storage::CreationCollisionOption::OpenIfExists));
//
//         ::Windows::Storage::StorageFile ^ f = ::wait(m_pp->folderTopic->GetFileAsync(wstring("core.topic")));
//
//         if(f != nullptr)
//         {
//
//            ::wait(f->DeleteAsync());
//
//         }
//
//
//         //::Windows::Storage::Search::QueryOptions ^ options = ref new ::Windows::Storage::Search::QueryOptions();
//
//         //options->FileTypeFilter->Append(wstring(".topic"));
//         
//         //::Windows::Storage::Search::StorageFileQueryResult  ^ result = m_pp->folderTopic->CreateFileQueryWithOptions(options);
//
//         m_pp->result = m_pp->folderTopic->CreateFileQuery();
//
//         m_pp->result->ContentsChanged += ref new TypedEventHandler < ::Windows::Storage::Search::IStorageQueryResultBase ^,Object ^ >([this](::Windows::Storage::Search::IStorageQueryResultBase ^ result,Object ^ o)
//         {
//
//            ::Windows::Storage::StorageFile ^ f = ::wait(result->Folder->GetFileAsync(wstring("core.topic")));
//
//            if(f != nullptr)
//            {
//
//               string str(::wait(::Windows::Storage::FileIO::ReadTextAsync(f)));
//
//               ::wait(f->DeleteAsync());
//
//               stringa stra;
//
//               stra.add_lines(str);
//
//               for(auto & strLine : stra)
//               {
//
//                  ::fork(get_app(),[=]()
//                  {
//
//                     on_receive(this,strLine);
//
//                  });
//
//               }
//
//            }
//
//         });
//         
//         ::wait(m_pp->result->GetFilesAsync());
//
//         //HINSTANCE hinstance = ::GetModuleHandleA(pszWindowProcModule);
//
//         //ATOM atom = register_class(hinstance);
//
//         //m_oswindow = ::CreateWindowExA(0,"small_ipc_rx_channel_message_queue_class",pszKey,0,0,0,0,0,HWND_MESSAGE,NULL,hinstance,NULL);
//
//         //if(m_oswindow == NULL)
//         //{
//         //   unsigned int dwLastError = ::GetLastError();
//         //   return false;
//         //}
//
//         //SetTimer(m_oswindow,888888,84,NULL);
//
//         //SetWindowLongPtr(m_oswindow,GWLP_USERDATA,(LONG_PTR) this);
//
//         //m_strWindowProcModule = pszWindowProcModule;
//
//
//
//         return true;
//      }
//
//
//      bool rx::destroy()
//      {
//
//         ::aura::del(m_pp);
//
//         return true;
//
//      }
//
//
//      void rx::receiver::on_receive(rx * prx,const char * pszMessage)
//      {
//      }
//
//      void rx::receiver::on_receive(rx * prx,int message,void * pdata,memory_size_t len)
//      {
//      }
//
//      void rx::receiver::on_post(rx * prx,long long int a,long long int b)
//      {
//      }
//
//      void * rx::on_receive(rx * prx,const char * pszMessage)
//      {
//
//         if(m_preceiver != NULL)
//         {
//            m_preceiver->on_receive(prx,pszMessage);
//         }
//
//         // ODOW - on date of writing : return ignored by this windows implementation
//
//         return NULL;
//
//      }
//
//      void * rx::on_receive(rx * prx,int message,void * pdata,memory_size_t len)
//      {
//
//         if(m_preceiver != NULL)
//         {
//            m_preceiver->on_receive(prx,message,pdata,len);
//         }
//
//         // ODOW - on date of writing : return ignored by this windows implementation
//
//         return NULL;
//
//      }
//
//
//      void * rx::on_post(rx * prx,long long int a,long long int b)
//      {
//
//         if(m_preceiver != NULL)
//         {
//            m_preceiver->on_post(prx,a,b);
//         }
//
//         // ODOW - on date of writing : return ignored by this windows implementation
//
//         return NULL;
//
//      }
//
//
//      //LRESULT CALLBACK rx::s_message_queue_proc(oswindow oswindow,UINT message,WPARAM wparam,LPARAM lparam)
//      //{
//
//      //   //int iRet = 0;
//
//      //   //rx * pchannel = (rx *)GetWindowLongPtr(oswindow,GWLP_USERDATA);
//
//      //   //if(pchannel == NULL)
//      //   //{
//
//      //   //   return ::DefWindowProcA(oswindow,message,wparam,lparam);
//
//      //   //}
//      //   //else
//      //   //{
//
//      //   //   return pchannel->message_queue_proc(message,wparam,lparam);
//
//      //   //}
//
//      //   return 0;
//
//      //}
//
//
//
//      //ATOM rx::register_class(HINSTANCE hInstance)
//      //{
//      //   //WNDCLASSEX wcex;
//
//      //   //wcex.cbSize = sizeof(WNDCLASSEX);
//
//      //   //wcex.style			   = 0;
//      //   //wcex.lpfnWndProc	   = &rx::s_message_queue_proc;
//      //   //wcex.cbClsExtra	   = 0;
//      //   //wcex.cbWndExtra	   = 0;
//      //   //wcex.hInstance		   = hInstance;
//      //   //wcex.hIcon			   = NULL;
//      //   ////wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
//      //   //wcex.hCursor		   = NULL;
//      //   //wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
//      //   //wcex.lpszMenuName	   = NULL;
//      //   //wcex.lpszClassName	= "small_ipc_rx_channel_message_queue_class";
//      //   //wcex.hIconSm		   = NULL;
//
//      //   //return RegisterClassEx(&wcex);
//      //   return true;
//      //}
//
//
//      //LRESULT rx::message_queue_proc(UINT message,WPARAM wparam,LPARAM lparam)
//      //{
//
//      //   //if(message == WM_USER + 100)
//      //   //{
//
//      //   //   on_post(this,wparam,lparam);
//
//      //   //}
//      //   //else if(message == WM_COPYDATA)
//      //   //{
//
//      //   //   COPYDATASTRUCT * pcds = (COPYDATASTRUCT *)lparam;
//
//      //   //   if(pcds == NULL)
//      //   //   {
//
//      //   //      return 0;
//
//      //   //   }
//      //   //   else if(pcds->dwData == 0x80000000)
//      //   //   {
//
//      //   //      string strMessage((const char *)pcds->lpData,pcds->cbData);
//
//      //   //      on_receive(this,strMessage.c_str());
//
//      //   //   }
//      //   //   else
//      //   //   {
//
//      //   //      on_receive(this,(int)pcds->dwData,pcds->lpData,pcds->cbData);
//
//      //   //   }
//
//      //   //}
//      //   //else if(message >= WM_APP)
//      //   //{
//
//      //   //   on_receive(this,message,(void *)wparam,lparam);
//
//      //   //}
//      //   //else
//      //   //{
//
//      //   //   return ::DefWindowProcA(m_oswindow,message,wparam,lparam);
//
//      //   //}
//
//      //   return 0;
//
//      //}
//
//
//
//      bool rx::on_idle()
//      {
//
//         return false;
//
//      }
//
//      bool rx::is_rx_ok()
//      {
//
//         //return ::IsWindow(m_oswindow) != FALSE;
//         return true;
//
//      }
//
//      ipc::ipc(::aura::application * papp):
//         object(papp),
//         base(papp),
//         tx(papp),
//         m_rx(papp)
//      {
//
//         m_dwTimeout = (5000) * 11;
//
//      }
//
//
//      ipc::~ipc()
//      {
//
//
//      }
//
//      bool ipc::open_ab(const char * pszKey,const char * pszModule,launcher * plauncher)
//      {
//
//         /*m_strChannel = pszKey;
//
//         m_rx.m_preceiver = this;
//
//         string strChannelRx = m_strChannel + "-a";
//         string strChannelTx = m_strChannel + "-b";
//
//         if(!::IsWindow(m_rx.m_oswindow))
//         {
//
//            if(!m_rx.create(strChannelRx.c_str(),pszModule))
//            {
//
//               return false;
//
//            }
//
//         }
//
//         if(!tx::open(strChannelTx.c_str(),plauncher))
//         {
//            return false;
//         }
//*/
//         return true;
//
//      }
//
//      bool ipc::open_ba(const char * pszKey,const char * pszModule,launcher * plauncher)
//      {
//
//         //m_strChannel = pszKey;
//
//         //m_rx.m_preceiver = this;
//
//         //string strChannelRx = m_strChannel + "-b";
//         //string strChannelTx = m_strChannel + "-a";
//
//
//         //if(!::IsWindow(m_rx.m_oswindow))
//         //{
//
//         //   if(!m_rx.create(strChannelRx.c_str(),pszModule))
//         //   {
//
//         //      return false;
//
//         //   }
//
//         //}
//
//         //if(!tx::open(strChannelTx.c_str(),plauncher))
//         //{
//         //   return false;
//         //}
//
//         return true;
//
//      }
//
//
//      bool ipc::is_rx_tx_ok()
//      {
//
//         return m_rx.is_rx_ok() && is_tx_ok();
//
//      }
//
//
//   } // namespace ipc
//
//
//} // namespace aura
//
//
//
//
//
//
