#include "StdAfx.h"
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/err.h>


typedef string ( *SALT)(::ca::application *, const char * , stringa &);

namespace fontopus
{

   validate::validate(::ca::application * papp, const char * pszForm, bool bVotagusAuth, bool bInteractive) :
      ca(papp),
      m_loginthread(papp),
      m_netcfg(papp)
   {
      m_bInteractive    = bInteractive;
      m_bVotagusAuth    = bVotagusAuth;
      m_strForm         = pszForm;
      m_puser           = NULL;
      ::radix::application * pradixapp = dynamic_cast < ::radix::application * > (papp);
      if(pradixapp != NULL)
      {
         try
         {
            pradixapp->keep_alive();
         }
         catch(...)
         {
         }
      }
      //Sleep(15 * 1000);
      m_pauth           = NULL;
   }

   validate::~validate()
   {
      ::WaitForSingleObject(m_loginthread.get_os_data(), INFINITE);
   }

   ::fontopus::user * validate::get_user(const char * pszRequestingParty, const char * pszSessId)
   {
      m_loginthread.m_strSalt.Empty();
      if(command_thread().property("app") == "simpledbcfg"
      || command_thread().property("app") == "core_netnode"
      || command_thread().property("app") == "veievserver"
      || command_thread().property("app") == "simpledbcfg"
      //|| command_thread().property("app") == "veriwell_mplite"      // churrasco 2011 m_strLicense
      || command_thread().property("app") == "netnodecfg")
      {
         m_puser = Application.allocate_user();
         m_puser->m_strPathPrefix = "system" + gen::str::has_char(Application.command().m_varTopicQuery["systemid"], "-");
         m_puser->m_strLogin = carlosgustavocecynlundgren;
         return m_puser;
      }
      else if(command_thread().property("app") == "mydns"
           || command_thread().has_property("install")
           || command_thread().has_property("uninstall"))
      {
         m_puser = Application.allocate_user();
         m_puser->m_strPathPrefix = "system" + gen::str::has_char(Application.command().m_varTopicQuery["systemid"], "-");
         m_puser->m_strLogin = carlosgustavocecynlundgren;
         return m_puser;
      }
      //else if(!strcmp(System.get_module_name(), "productionapp")
      //   || !strcmp(System.get_module_name(), "productionbasisapp")
      //   || !strcmp(System.get_module_name(), "backupapp")
      //   || System.command().m_varTopicQuery["app"] == "production"
      //   || System.get_license_id() == "winservice_filesystemsize")
      else if(command_thread().property("app") == "backup"
           || command_thread().property("app") == "winservice_filesystemsize")
      {
         m_puser = Application.allocate_user();
         m_puser->m_strPathPrefix = "system" + gen::str::has_char(Application.command().m_varTopicQuery["systemid"], "-");
         m_puser->m_strLogin = camilosasuketsumanuma;
         return m_puser;
      }
      string strDir;
      string strUsername;
      string strPasshash;
      System.crypt().file_get(Application.dir().usersystemappdata(Application.dir().default_os_user_path_prefix(), "license_auth", "00001.data"), strUsername, "", get_app());
      m_loginthread.m_strUsername = strUsername;
      string strHost(pszRequestingParty);

      stringa straRequestingServer;
      straRequestingServer.add("fontopus.com");
      straRequestingServer.add("fontopus.eu");
      straRequestingServer.add("fontopus.asia");
      if(strHost.is_empty())
      {
         if(::ca::get_thread() != NULL && ::ca::get_thread()->m_strWorkUrl.has_char())
         {
            strHost = ::ca::get_thread()->m_strWorkUrl;
         }
         else
         {
            strHost = Application.file().as_string(System.dir().appdata("database\\text\\last_good_known_fontopus_com.txt"));
            if(!straRequestingServer.contains_ci(strHost))
            {
               strHost = "https://fontopus.com/";
            }
         }
      }
      if(System.url().get_server(strHost).has_char())
      {
         strHost = System.url().get_server(strHost);
      }

      if(straRequestingServer.contains(Application.command_thread().m_varTopicQuery["fontopus"].get_string())
         && Application.command_thread().m_varTopicQuery["sessid"].get_string().get_length() > 16)
      {
         m_loginthread.m_puser = Application.allocate_user();
         m_loginthread.m_puser->m_sessionidmap[Application.command_thread().m_varTopicQuery["fontopus"].get_string()] = Application.command_thread().m_varTopicQuery["sessid"].get_string();
         m_loginthread.m_puser->m_sessionidmap[strHost] = Application.command_thread().m_varTopicQuery["sessid"].get_string();
         m_loginthread.m_puser->m_strFontopusServerSessId = Application.command_thread().m_varTopicQuery["sessid"].get_string();
         m_loginthread.m_puser->m_strRequestingServer = strHost;
         xml::document documentBasicInfo(get_app());
         if(documentBasicInfo.load(Application.http().get("https://"+Application.command_thread().m_varTopicQuery["fontopus"].get_string()+"/ca2api/account/get_basic_info", m_loginthread.m_puser)))
         {
            string strLogin;
            if(documentBasicInfo.get_root()->get_attr("login", strLogin) && strLogin.find("@") > 0)
            {
               m_loginthread.m_puser->m_strLogin = strLogin;
               return  m_loginthread.m_puser;
            }
         }
      }

      m_loginthread.m_puser = Application.allocate_user();

      if(pszSessId != NULL && string(pszSessId).get_length() > 16)
      {
         m_loginthread.m_puser->m_sessionidmap[strHost] = pszSessId;
      }

      m_loginthread.m_strRequestingServer = strHost;
      System.crypt().file_get(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/00002.data"), strPasshash, calc_key_hash(), get_app());
      if(strUsername.has_char() && strPasshash.has_char())
      {

         m_loginthread.m_strPassword.Empty();
         m_loginthread.m_strPasshash = strPasshash;
         m_loginthread.m_pcallback = this;
         m_loginthread.run();

         close_all();


         return m_puser;
      }
      else if(m_bInteractive)
      {

         m_loginthread.m_strModHash.Empty();
         m_loginthread.m_strKeyHash.Empty();
         m_loginthread.m_strCa2Hash.Empty();
         ensure_main_document();
         page1(NULL);
         show_and_request_auth();
         return m_puser;

      }
      else
      {

         return NULL;

      }

   }

   void validate::ensure_main_document()
   {
/*      if(m_pdoc != NULL)
         return;*/
      ::ca::create_context_sp createcontext(get_app());
      createcontext->m_bMakeVisible = false;
      createcontext->m_puiParent = Sys(get_app()).oprop("top_parent").ca2 < ::user::interaction > ();
      createcontext->m_bOuterPopupAlertLike = true;
      //m_pdoc = dynamic_cast < form_document * > (m_ptemplatePane->open_document_file(createcontext));
      //userex::pane_tab_view * pview = m_pdoc->get_typed_view < userex::pane_tab_view >();
      //pview->set_view_creator(this);
      //m_ptabview = pview;
/*      pview->set_tab("ca2open", 1);
      pview->set_tab("network", 2);
      pview->set_image_tab("", Application.dir().matter("image/keyboard-h21.png"), 3);
      pview->set_cur_tab_by_id(1);*/
   }

   bool validate::get_license(const char * psz)
   {
      string strLicense(psz);
      if(strLicense == "netnodeapp"
         || strLicense == "netnodecfgapp"
         || strLicense == "simpledbcfg"
         || strLicense == "core_netnode"
         || strLicense == "veievserver"
         //|| strLicense == "veriwell_mplite"      // churrasco 2011 m_strLicense
         || strLicense == "mydns"
         || Application.command().m_varTopicQuery.has_property("install")
         || Application.command().m_varTopicQuery.has_property("uninstall")
         || strLicense == "netnodecfg")
      {
         return true;
      }
      else if(strLicense == "flag"
         || strLicense == "alarm"
         || strLicense == "biteditor"
         || strLicense == "md5"
         || strLicense == "vmp"
         || strLicense == "veiev"
         || strLicense == "netshareservercfg"
         || strLicense == "netshareserver"
         || strLicense == "veriedit"
         || strLicense == "netshareclient"
         || strLicense == "verisimplevideo"
         || strLicense == "eluce"
         || strLicense == "whiteboard"
         || strLicense == "bergedge"
         || strLicense == "app.sysutils.igd"
         || strLicense == "projection"
         || strLicense == "querydb"
         )
      {
         return true;
      }
      //else if(!strcmp(System.get_module_name(), "productionapp")
      //   || !strcmp(System.get_module_name(), "productionbasisapp")
      //   || !strcmp(System.get_module_name(), "backupapp")
      //   || System.get_license_id() == "winservice_filesystemsize")
      else if(strLicense == "backupapp"
         || strLicense == "winservice_filesystemsize"
         || strLicense == "production")
      {
         return true;
      }
      else if(strLicense == "i2com")
      {
         return true;
      }
      m_strLicense = psz;
      m_loginthread.m_strLicense = m_strLicense;
      string strHost = Application.file().as_string(System.dir().appdata("database\\text\\last_good_known_fontopus_com.txt"));
      stringa straRequestingServer;
      straRequestingServer.add("fontopus.com");
      straRequestingServer.add("fontopus.eu");
      straRequestingServer.add("fontopus.asia");
      if(!straRequestingServer.contains_ci(strHost))
      {
         strHost = "fontopus.com";
      }

      if(straRequestingServer.contains(Application.command_thread().m_varTopicQuery["fontopus"].get_string())
         && Application.command_thread().m_varTopicQuery["sessid"].get_string().get_length() > 16)
      {
         strHost = Application.command_thread().m_varTopicQuery["fontopus"].get_string();
      }

      string strAuthUrl("https://"+ strHost +"/ca2api/account/auth");

      gen::property_set post;
      gen::property_set headers;
      gen::property_set set;

      string strAuth;
      post["entered_license"] = m_strLicense;
      //m_puser->set_sessid(ApplicationUser.m_str, strAuthUrl);
//      DWORD dwTimeTelmo1 = GetTickCount();

      post["entered_license"] = m_strLicense;

      Application.http().get(strAuthUrl, strAuth, post, headers, set);

      m_loginthread.m_strFontopusServer = strHost;

      xml::document doc(get_app());
      doc.load(strAuth);
      if(doc.get_root()->get_name() == "response")
      {
         if(doc.get_root()->attr("id") == "auth")
         {
            return true;
         }
      }
      if(m_bInteractive)
      {
         m_loginthread.m_strModHash.Empty();
         m_loginthread.m_strKeyHash.Empty();
         m_loginthread.m_strCa2Hash.Empty();
         ensure_main_document();
         page1(NULL);
         show_and_request_auth();
         return m_bLicense;
      }
      return false;
      string strDir;
      string strUsername;
      string strPasshash;
      System.crypt().file_get(Application.dir().usersystemappdata(Application.dir().default_os_user_path_prefix(), "license_auth", "00001.data"), strUsername, "", get_app());
      m_loginthread.m_strUsername = strUsername;
      System.crypt().file_get(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/00002.data"), strPasshash, calc_key_hash(), get_app());
      if(strUsername.has_char() && strPasshash.has_char())
      {

         m_loginthread.m_strPassword.Empty();
         m_loginthread.m_strPasshash = strPasshash;
         m_loginthread.m_pcallback = this;
         m_loginthread.run();

         close_all();

         return m_bLicense;
      }
      else if(m_bInteractive)
      {
         m_loginthread.m_strModHash.Empty();
         m_loginthread.m_strKeyHash.Empty();
         m_loginthread.m_strCa2Hash.Empty();
         ensure_main_document();
         page1(NULL);
         show_and_request_auth();
         return m_bLicense;
      }
      else
      {
         return false;
      }
   }

   void validate::page1(const char * pszMatter)
   {
//      m_pdocAuth->get_html_data()->m_puser = m_loginthread.m_puser;
      //string strUrl;
      //strUrl = "http://spaignition.api.laborserver.net/query?node=install_application&id=";
      //string strAppName;
      //if(System.m_strAppName == "winactionarea")
      //{
        // strAppName = "_set_windesk";
      //}
      //else
      //{
        // strAppName = System.m_strAppName;
      //}
      //strUrl += strAppName;
      //strUrl += "&key=name";
      //m_pdocAuth->get_html_data()->m_propertyset["application_name"] = System.http().get(strUrl);
      //if(m_strLicense.is_empty())
      //{
        // m_pdocAuth->get_html_data()->m_propertyset["reason"] = "Authenticating";
      //}
      //else
      //{
        // m_pdocAuth->get_html_data()->m_propertyset["reason"] = "Licensing";
      //}
/*      if(!m_pdocAuth->on_open_document(Application.dir().matter(m_strForm)))
      {
         authentication_failed(0, "Cannot open form for authentication!!");
         return;
      }*/
      display_main_frame();
      //::user::interaction * pguie = m_pviewAuth->GetChildByName("user");
//      text_interface * ptext = dynamic_cast < text_interface * > (pguie);
//      ptext->_001SetText(m_loginthread.m_strUsername);
//      if(m_loginthread.m_strUsername.is_empty())
  //       Application.set_keyboard_focus(pguie);
    //  else
      //{
        // pguie = m_pviewAuth->GetChildByName("password");
         //Application.set_keyboard_focus(pguie);
      //}
   }

   void validate::display_main_frame()
   {
      rect rectOpen;
/*      if(m_ptabview->GetParentFrame()->GetParent() == NULL)
      {

         System.get_screen_rect(rectOpen);
      }
      else
      {
         m_ptabview->GetParentFrame()->GetParent()->GetClientRect(rectOpen);
      }
      int iWidth = rectOpen.width();
      int iHeight = rectOpen.height();
      rectOpen.deflate(iWidth / 5, iHeight / 5);
      simple_frame_window * pframe = dynamic_cast < simple_frame_window * > (m_pviewAuth->GetTopLevelParent());
      if(pframe != NULL)
      {
         pframe->m_bblur_Background = true;
      }
/*      if(System.m_puiInitialPlaceHolderContainer != NULL)
      {
         System.m_puiInitialPlaceHolderContainer->GetTopLevelParent()->ShowWindow(SW_SHOW);
      }
      else*/
      /*{
         m_ptabview->GetTopLevelFrame()->ShowWindow(SW_SHOW);
      }
      m_ptabview->GetTopLevelFrame()->SetWindowPos(
         ZORDER_TOP,
         rectOpen.left, rectOpen.top,
         rectOpen.width(), rectOpen.height(),
         SWP_SHOWWINDOW);
      if(m_ptabview->GetTopLevelFrame()->GetParent() != NULL)
      {
         try
         {
            m_ptabview->GetTopLevelFrame()->GetParent()->layout();
         }
         catch(...)
         {
         }
      }
      else
      {
      }*/
      /*UINT ui1 = GetCurrentThreadId();
      UINT ui2 = m_ptabview->GetTopLevelFrame()->m_pthread->get_os_int();
      if(::AttachThreadInput(ui1, ui2, TRUE)) 
      {
         TRACE("AttachedThreadInput");
      }*/
/*      if(m_ptabview->GetTopLevelFrame()->SetForegroundWindow())
      {
         TRACE("fontopus_validate tab_view top_level_frame set_foreground_window OK");
         if(m_ptabview->GetTopLevelFrame()->BringWindowToTop())
         {
            TRACE("fontopus_validate tab_view top_level_frame bring_window_to_top OK");
         }
      }
      */
      

      /*
      m_ptabview->GetTopLevelFrame()->ActivateFrame();
      m_ptabview->GetTopLevelFrame()->SetFocus();*/
      

      //m_pviewAuth->GetTopLevelParent()->SetForegroundWindow();
      //m_pviewAuth->GetTopLevelParent()->BringWindowToTop();
   }

   void validate::pageMessage(const char * pszMatter, gen::property_set & set)
   {
      ensure_main_document();
/*      m_pdocAuth->get_html_data()->m_propertyset = set;
      m_pdocAuth->on_open_document(Application.dir().matter(pszMatter));
      display_main_frame();
      ::ca::live_signal livesignal;
      livesignal.keep(get_app());
      m_ptabview->get_wnd()->RunModalLoop(MLF_NOIDLEMSG | MLF_NOKICKIDLE, &livesignal);
      m_ptabview->get_wnd()->EndAllModalLoops(IDOK);*/
   }

/*   void validate::on_create_view(::user::view_creator_data * pcreatordata)
   {
      switch(pcreatordata->m_id)
      {
      case 1:
         {
            m_pdocAuth = Cube.create_child_form(this, pcreatordata->m_pholder);
            if(m_pdocAuth != NULL)
            {
               m_pviewAuth = m_pdocAuth->get_typed_view < form_view > ();
               m_pviewAuth->m_pcallback = this;
               pcreatordata->m_pdoc = m_pdocAuth;
               pcreatordata->m_pwnd = m_pviewAuth->GetParentFrame();
            }
         }
         break;
      case 2:
         {
            if(m_netcfg.initialize_child(pcreatordata->m_pholder))
            {
               pcreatordata->m_pdoc = m_netcfg.m_pdoc;
               pcreatordata->m_pwnd = m_netcfg.m_pview->GetParentFrame();
            }

         }
         break;
      case 3:
         {

            m_pkeyboardlayout = m_ptabview->create_view < ::user::keyboard_layout > ();
            m_pkeyboardlayout->CreateViews();
            pcreatordata->m_pdoc = m_pkeyboardlayout->m_pdoc;
            pcreatordata->m_pwnd = m_pkeyboardlayout;
         }
         break;
      }
      if(pcreatordata->m_pwnd != NULL)
      {
         pcreatordata->m_eflag.signalize(::user::view_creator_data::flag_hide_all_others_on_show);
      }

   }*/

   void validate::on_show_view()
   {
/*      switch(m_ptabview->get_view_id())
      {
      case 1:
         {
         }
         break;
      case 2:
         {
            m_netcfg.on_show();
         }
         break;
      }*/
   }






   login_thread::login_thread(::ca::application * papp) :
   ca(papp),
      thread(papp),
      simple_thread(papp)
   {
      m_bOk                = false;
      m_puser              = NULL;
      m_bFontopusServer    = false;
   }

   login_thread::~login_thread()
   {
   }

   void validate::get_mod(stringa & straHash, stringa & straSource)
   {
      straHash.remove_all();
      straSource.remove_all();
/*#if !core_level_1 && !core_level_2
      SetDllDirectory(NULL);
#endif
      ::LoadLibraryA("salt.dll");
      DWORD dwNeeded;
      if(!EnumProcessModules(::GetCurrentProcess(),  NULL,  0,  &dwNeeded))
      {
         return;
      }
      DWORD dwAlloc = (dwNeeded + sizeof(HMODULE)) * 2;
      HMODULE * pmodulea = new HMODULE[dwAlloc / sizeof(HMODULE)];
      if(pmodulea == NULL)
         return;
      if(!EnumProcessModules(::GetCurrentProcess(),  pmodulea,  dwAlloc,  &dwNeeded))
      {
         delete pmodulea;
         return;
      }
      string strModule;
      ex1::file_system_sp fs(get_app());
      string strModuleFolder(System.get_ca2_module_folder());
      fs->FullPath(strModuleFolder, strModuleFolder);
      for(DWORD dw = 0; dw < dwNeeded / (sizeof(HMODULE)); dw++)
      {
         strModule.Empty();
         GetModuleFileName(pmodulea[dw], strModule.GetBufferSetLength(4096), 4096);
         strModule.ReleaseBuffer();
         fs->FullPath(strModule, strModule);
         if(gen::str::begins_ci(strModule, strModuleFolder))
         {
            straSource.add(strModule);
            straHash.add(System.file36().md5(strModule));
         }
      }
      straHash.QuickSort();
      delete pmodulea;*/
   }

   string validate::calc_mod_hash()
   {
      if(m_loginthread.m_strModHash.has_char())
         return m_loginthread.m_strModHash;
      stringa straHash;
      stringa straSource;
      get_mod(straHash, straSource);
      m_loginthread.m_strModHash = System.crypt().md5(straHash.implode(";"));
      return m_loginthread.m_strModHash;
   }

   string validate::calc_key_hash()
   {
      if(m_loginthread.m_strKeyHash.has_char())
         return m_loginthread.m_strKeyHash;
/*#if !core_level_1 && !core_level_2
      ::SetDllDirectoryA(System.get_ca2_module_folder());
#endif
      HMODULE hmoduleSalt = ::LoadLibraryA("salt.dll");
      SALT salt = (SALT) ::GetProcAddress(hmoduleSalt, "salt");
      stringa straSource;
      if(m_loginthread.m_strUsername.has_char())
      {
         m_loginthread.m_strKeyHash = salt(get_app(), m_loginthread.m_strUsername, straSource);
         return m_loginthread.m_strKeyHash;
      }
      else */
      {
         m_loginthread.m_strKeyHash = "ca2t12n";
         return "ca2t12n";
      }
   }

   string validate::calc_ca2_hash()
   {
      if(m_loginthread.m_strCa2Hash.has_char())
         return m_loginthread.m_strCa2Hash;
      /*stringa straHash;
      stringa straSource;
      get_mod(straHash, straSource);
      straHash.insert_at(0, m_loginthread.m_strUsername);
#if !core_level_1 && !core_level_2
      ::SetDllDirectoryA(System.get_ca2_module_folder());
#endif*/
      /*HMODULE hmoduleSalt = ::LoadLibraryA("salt.dll");
      SALT salt = (SALT) ::GetProcAddress(hmoduleSalt, "salt");
      m_loginthread.m_strCa2Hash = salt(get_app(), straHash.implode(";"), straSource);*/
      m_loginthread.m_strCa2Hash = "ca2t12n";
      return m_loginthread.m_strCa2Hash;
   }


   // return hash and check if hash is valid
   bool validate::check_ca2_hash()
   {
      string strUrl("https://fontopus.com/ca2api/account/check_hash");
      gen::property_set post;
      gen::property_set headers;
      gen::property_set set;
      string strResponse;
      stringa straHash;
      stringa straSource;
      get_mod(straHash, straSource);
      straHash.insert_at(0, m_loginthread.m_strPasshash);
      straHash.insert_at(0, m_loginthread.m_strUsername);

      post["hash"] = straHash.implode(";");
      post["source"] = straHash.implode(";");
      for(int i = 0; i < 3; i++)
      {
         if(Application.http().get(strUrl, strResponse, post, headers, set))
            break;
      }
      if(strResponse == "OK")
         return true;
      else
         return false;
   }

   void validate::on_login_thread_response(int iAuth, const char * pszResponse)
   {
      if(iAuth == 1)
      {
         authentication_succeeded();
      }
      else
      {
         authentication_failed(iAuth, pszResponse);
      }
   }

   void login_thread_callback::on_login_thread_response(int iAuth, const char * pszResponse)
   {
      UNREFERENCED_PARAMETER(iAuth);
      UNREFERENCED_PARAMETER(pszResponse);
   }

   string login_thread_callback::calc_mod_hash()
   {
      return "";
   }

   string login_thread_callback::calc_key_hash()
   {
      return "";
   }

   string login_thread_callback::calc_ca2_hash()
   {
      return "";
   }

   BOOL login_thread::run()
   {
      ca4::http::e_status estatus;
      string strResponse = Login(&estatus);
      int iAuth;
      xml::document doc(get_app());
      doc.load(strResponse);
      if(doc.get_root()->get_name() == "response")
      {
         if(doc.get_root()->attr("id") == "auth" && doc.get_root()->attr("passhash").has_char() && doc.get_root()->attr("secureuserid").has_char())
         {
            System.m_authmap[m_strUsername].m_mapServer[m_strRequestingServer] = strResponse;
            System.m_authmap[m_strUsername].m_mapFontopus[m_strFontopusServer] = strResponse;
            m_puser->m_strLogin = m_strUsername;
            m_puser->m_strFontopusServerSessId = doc.get_root()->attr("sessid");
            m_puser->m_strRequestingServer = m_strRequestingServer;
            m_puser->m_strFunUserId = doc.get_root()->attr("secureuserid");
            m_strPasshash = doc.get_root()->attr("passhash");
            iAuth = 1;
            if(m_bFontopusServer)
            {
               Application.file().put_contents(System.dir().appdata("database\\text\\last_good_known_fontopus_com.txt"), m_strRequestingServer);
            }
            execute();
            if(m_strLicense.has_char())
            {
               m_strValidUntil = doc.get_root()->attr("valid_until");
            }
         }
         else if(doc.get_root()->attr("id") == "registration_deferred")
         {
            delete m_puser;
            iAuth = 5;
         }
         else
         {
            delete m_puser;
            iAuth = -1;
         }
      }
      else
      {
         delete m_puser;
         if(estatus == ca4::http::status_connection_timed_out)
         {
            iAuth = -2;
         }
         else
         {
            iAuth = 0;
         }
      }
      //      char * psz = NULL;
      //    *psz = '2';
      m_pcallback->on_login_thread_response(iAuth, strResponse);
      return TRUE;
   }

   string login_thread::Login(ca4::http::e_status * pestatus)
   {
      if(m_straRequestingServer.get_count() <= 0)
      {
         stringa straRequestingServer;
         straRequestingServer.add("fontopus.com");
         straRequestingServer.add("fontopus.eu");
         straRequestingServer.add("fontopus.asia");
         if(straRequestingServer.contains_ci(m_strRequestingServer))
         {
            m_bFontopusServer = true;
            straRequestingServer.remove(m_strRequestingServer);
            straRequestingServer.insert_at(0, m_strRequestingServer);
            m_straRequestingServer = straRequestingServer;
         }
      }
      if(m_straRequestingServer.get_count() > 0)
      {
         string strLogin;
         for(int i = 0; i < m_straRequestingServer.get_count(); i++)
         {
            m_strRequestingServer  = m_straRequestingServer[i];
            strLogin = NetLogin(pestatus);
            if(strLogin.has_char())
               return strLogin;
         }
         return "";
      }
      else
      {
         return NetLogin(pestatus);
      }
      // entering offline validation
/*      if(m_strLicense.is_empty()) // user auth
      {
         if(m_strPasshash.has_char()) // validated some time before
         {
            return "<response id=\"auth\" sessid=\"\" secureuserid=\"\">";
         }
         else
         {
            // check against last password saved in this computer
            if(m_strPassword.has_char())
            {
               string strSalt;
               System.crypt().file_get(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), m_strUsername, "license_auth/00005.data"), strSalt, m_pcallback->calc_key_hash());
               if(strSalt.is_empty())
                  return ""; // at least one time connected and validated to ca2open ca2anima
               string strPasshash2 = System.crypt().v5_get_password_hash(strSalt, m_strPassword);
               string strPasshash5;
               System.crypt().file_get(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), m_strUsername, "license_auth/00010.data"), strPasshash5, m_pcallback->calc_key_hash());
               if(strPasshash5.is_empty() || strPasshash2.is_empty())
                  return ""; // at least one time connected and validated to ca2open ca2anima
               if(strPasshash5 != strPasshash2)
                  return ""; // at least one time connected and validated to ca2open ca2anima
               return "<response id=\"auth\" sessid=\"\" secureuserid=\"\">";
            }
            else
            {
               return "";  // no password, no validation
            }
         }
      }
      else // license
      {
         // check against last password saved in this computer
         if(m_strPassword.has_char())
         {
            string strLicense;
            System.crypt().file_get(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), m_strUsername, "license_auth/" + m_strLicense + ".data"), strLicense, m_pcallback->calc_ca2_hash());
            if(strLicense.is_empty())
               return ""; // at least one time connected and validated to ca2open ca2anima
            stringa straLicense;
            straLicense.explode(";", strLicense);
            if(straLicense.get_count() != 2)
               return ""; // license validation
            int iPathCount = 0;
            time_t timeLicense = System.datetime().strtotime(NULL, straLicense[0], 0, iPathCount);
            // TODO: take with timeNow, it should be always be greater than before.
            time_t timeNow = time(NULL);
            time_t timeLast = System.datetime().strtotime(NULL, straLicense[1], 0, iPathCount);
            // should license at least 5 seconds after last licensing
            if(timeLicense > timeNow && timeNow > (timeLast + 5))
            {
               return "<response id=\"auth\" sessid=\"\" secureuserid=\"\">";
            }
            else
            {
               return ""; // license expired
            }
         }
         else
         {
            return "";  // no password, no validation, at least one time connected to ca2open ca2anima
         }
      }*/
   }


   string login_thread::NetLogin(ca4::http::e_status * pestatus)
   {

      if(System.m_authmap[m_strUsername].m_mapServer[m_strRequestingServer].get_length() > 32)
      {
         return System.m_authmap[m_strUsername].m_mapServer[m_strRequestingServer];
      }

      string strGetFontopus("http://" + m_strRequestingServer + "/get_fontopus");

      m_strFontopusServer.Empty();

      ::ca::application * papp = get_app();

      url_domain domainFontopus;

      m_strFontopusServer = System.get_fontopus_server(strGetFontopus, papp, 8);

      domainFontopus.create(m_strFontopusServer);

      if(domainFontopus.m_strRadix != "fontopus")
         return "";

      if(System.m_authmap[m_strUsername].m_mapFontopus[m_strFontopusServer].get_length() > 32)
      {
         return System.m_authmap[m_strUsername].m_mapFontopus[m_strFontopusServer];
      }




      string strLogin;

      string strLoginUrl("https://"+ m_strFontopusServer +"/ca2api/account/login");

      xml::document doc(get_app());

      string strSessId;

      string strRsaModulus;

      for(int iRetry = 0; iRetry <= 8; iRetry++)
      {

         if(iRetry > 0)
         {
            Sleep(iRetry * 584);
         }

         try
         {


            gen::property_set post;
            gen::property_set headers;
            gen::property_set set;

            if(m_puser->m_sessionidmap[m_strRequestingServer].get_length() > 16)
            {
               m_puser->set_sessid(m_puser->m_sessionidmap[m_strRequestingServer], strLoginUrl);
            }
            else
            {
               m_puser->set_sessid("not_auth", strLoginUrl);
            }
            set["app"] = papp;

            Application.http().get(strLoginUrl, strLogin, post, headers, set, m_puser->m_phttpcookies, m_puser, NULL, pestatus);

         }
         catch(...)
         {
         }

         strLogin.trim();

         if(strLogin.is_empty())
            continue;

         if(!doc.load(strLogin))
            continue;

         if(doc.get_root()->get_name() != "login")
            continue;

         strSessId = doc.get_root()->attr("sessid");

         if(strSessId.is_empty())
            continue;

         strRsaModulus = doc.get_root()->attr("rsa_modulus");

         if(strRsaModulus.has_char())
            break;

      }

      if(strRsaModulus.is_empty())
         return "";

      string strPass;
      if(m_strPasshash.is_empty())
      {
         strPass = System.crypt().nessie(m_strPassword);
      }
      else
      {
         strPass = m_strPasshash;
      }

      RSA * rsa = RSA_new();

      BN_hex2bn(&rsa->n, strRsaModulus);
      BN_hex2bn(&rsa->e, "10001");


      primitive::memory memory;
      primitive::memory memIn;

      Application.hex_to_memory(memIn, strPass);

      memory.allocate(2048);

      int i = RSA_public_encrypt((int) memIn.get_size(), (const unsigned char * ) (const char *) memIn.get_data(), memory.get_data(), rsa, RSA_PKCS1_PADDING);

      const char * psz = ERR_error_string(ERR_get_error(), NULL);

      TRACE(psz);

      memory.allocate(i);

      string strHex;
      Application.memory_to_hex(strHex, memory);

      RSA_free(rsa);

      m_puser->m_strLogin = m_strUsername;

      string strAuth;

      {

         string strAuthUrl("https://"+ m_strFontopusServer +"/ca2api/account/auth?" + m_pcallback->oprop("defer_registration").get_string() 
            +"&ruri=" + System.url(m_pcallback->oprop("ruri").get_string()).url_encode());

         gen::property_set post;
         gen::property_set headers;
         gen::property_set set;

         if(m_strPasshash.is_empty())
         {
            post["entered_password"] = strHex;
         }
         else
         {
            post["entered_passhash"] = strHex;
         }
         string strCrypt;

         post["entered_login"] = m_strUsername;
         if(m_strLicense.has_char())
         {
            post["entered_license"] = m_strLicense;
         }

         m_puser->set_sessid(strSessId, strAuthUrl);
         set["app"] = papp;
         DWORD dwTimeTelmo1 = GetTickCount();
         Application.http().get(strAuthUrl, strAuth, post, headers, set, m_puser->m_phttpcookies, m_puser, NULL, pestatus);
         DWORD dwTimeTelmo2 = GetTickCount();

         TRACE("Total time Application.http().get(\"" + strAuthUrl + "\") : %d", (dwTimeTelmo2 - dwTimeTelmo1));

      }

      return strAuth;
   }

   void login_thread::execute()
   {
      string strFilename;
      string strResponse;
      gen::property_set set;
      for(int i = 0; i < m_httpexecutea.get_size(); i++)
      {
         strFilename = System.file().time_square(get_app());
         System.http().download(
            m_httpexecutea[i].m_strUrl,
            strFilename,
            m_httpexecutea[i].m_propertysetPost,
            m_httpexecutea[i].m_propertysetHeaders,
            set,
            m_puser->m_phttpcookies,
            m_puser);
         strResponse = Application.file().as_string(strFilename);
         TRACE0(strResponse);
         m_httpexecutea[i].m_strResponse = strResponse;
      }
   }

/*   bool validate::BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent)
   {
      UNREFERENCED_PARAMETER(pview);
      if(pevent->m_eevent == ::user::event_button_clicked
         || pevent->m_eevent == ::user::event_enter_key)
      {
         if(pevent->m_puie->m_id == "submit" ||
            pevent->m_eevent == ::user::event_enter_key)
         {
            if(m_loginthread.get_os_data() != NULL)
               return true;
            m_pviewAuth->KillTimer(1984);
            m_loginthread.oprop("defer_registration") = oprop("defer_registration");
            if(m_bVotagusAuth)
            {
               m_ptabview->GetParentFrame()->ShowWindow(SW_HIDE);
               ::user::interaction * pguie = m_pviewAuth->GetChildByName("user");
               text_interface * ptext = dynamic_cast < text_interface * > (pguie);
               //m_loginthread.m_puser = dynamic_cast < ::fontopus::user * > (System.allocate_user());
               ptext->_001GetText(m_loginthread.m_strUsername);
               pguie = m_pviewAuth->GetChildByName("password");
               ptext = dynamic_cast < text_interface * > (pguie);
               ptext->_001GetText(m_loginthread.m_strPassword);
               m_loginthread.m_pcallback = this;
               m_loginthread.Begin();
            }
            else
            {
               m_pauth = new auth;
               ::user::interaction * pguie = m_pviewAuth->GetChildByName("user");
               text_interface * ptext = dynamic_cast < text_interface * > (pguie);
               ptext->_001GetText(m_pauth->m_strUsername);
               pguie = m_pviewAuth->GetChildByName("password");
               ptext = dynamic_cast < text_interface * > (pguie);
               ptext->_001GetText(m_pauth->m_strPassword);
               m_ptabview->get_wnd()->EndModalLoop(IDOK);
               m_ptabview->GetParentFrame()->ShowWindow(SW_HIDE);
            }
            return true;
         }
      }
      else if(pevent->m_eevent == ::user::event_timer
         && pevent->m_uiEvent == 1984)
      {

         string strDir;
         string strUsername;
         string strPasshash;

         string strPathUsername = Application.dir().usersystemappdata(Application.dir().default_os_user_path_prefix(), "license_auth", "00001.data");
         string strPathPasshash = Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/00002.data");

         if(!System.file().exists(strPathUsername) || !System.file().exists(strPathPasshash))
            return true;

         System.crypt().file_get(strPathUsername, strUsername, "", get_app());
         m_loginthread.m_strUsername = strUsername;

         System.crypt().file_get(strPathPasshash, strPasshash, calc_key_hash(), get_app());

         if(strUsername.is_empty() || strPasshash.is_empty())
            return true;

         m_pviewAuth->KillTimer(1984);
         m_loginthread.m_strPassword.Empty();
         m_loginthread.m_strPasshash = strPasshash;
         m_loginthread.m_pcallback = this;
         m_loginthread.Begin();

         return true;
      }
      return false;
   }*/

   void validate::authentication_failed(int iAuth, const char * pszResponse)
   {
      UNREFERENCED_PARAMETER(pszResponse);
      gen::property_set propertyset;
      string strUsername = m_loginthread.m_strUsername;
      m_bLicense = false;
      m_puser = NULL;
/*      if(m_pdocAuth != NULL)
      {
         m_pdocAuth->get_html_data()->m_puser = NULL;
      }
      if(m_strLicense.has_char())
      {
         if(m_bInteractive)
         {
            string strUrl;
            strUrl = "http://spaignition.api.laborserver.net/query?node=install_application&id=";
            strUrl += m_strLicense;
            strUrl += "&key=launch_name";
            string strName = Application.http().get(strUrl);
            if(strName.is_empty())
               strName = m_strLicense;
            propertyset["project"] = strName;
            strUrl = "ext://https://"+ m_loginthread.m_strFontopusServer + "/license?id="+ m_strLicense + "&lang=" + System.get_locale() + "&sessid=" + ApplicationUser.get_sessid(m_loginthread.m_strFontopusServer);
            propertyset["contribute_link"] = strUrl;
            pageMessage("err\\user\\authentication\\not_licensed.xhtml", propertyset);
         }
      }
      else if(iAuth == -1)
      {
         propertyset["register_link"] = "ext://http://"+ m_loginthread.m_strFontopusServer + "/register?email="+ System.url().url_encode(m_loginthread.m_strUsername);
         pageMessage("err\\user\\authentication\\wrong_fontopus_login.xhtml", propertyset);
         try
         {
            System.file().del(Application.dir().usersystemappdata(Application.dir().default_os_user_path_prefix(), "license_auth", "00001.data"));
         }
         catch(...)
         {
         }
         try
         {
            System.file().del(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/00002.data"));
         }
         catch(...)
         {
         }
         m_loginthread.m_strModHash.Empty();
         m_loginthread.m_strKeyHash.Empty();
         m_loginthread.m_strCa2Hash.Empty();
      }
      else if(iAuth == -2)
      {
         if(m_bInteractive)
         {
            propertyset["server"] = "fontopus.com";
            pageMessage("err\\user\\network\\connection_timed_out.xhtml", propertyset);
         }
      }
      else if(iAuth == 5)
      {
         if(m_bInteractive)
         {
            propertyset["server"] = "fontopus.com";
            propertyset["email"] = strUsername;
            pageMessage("err\\user\\authentication\\registration_deferred.xhtml", propertyset);
         }
      }
      else
      {
         if(m_bInteractive)
         {
            pageMessage("err\\user\\authentication\\failed.xhtml", propertyset);
         }
      }
      delete m_pauth;*/
   }

   void validate::authentication_succeeded()
   {
      TRACE0("The authentication has succeeded.");
      string strUsername = m_loginthread.m_strUsername;
      string strPasshash = m_loginthread.m_strPasshash;
      string strPassword = m_loginthread.m_strPassword;

      string strUsernamePrevious;
      string strPasshashPrevious;
      System.crypt().file_get(Application.dir().usersystemappdata(Application.dir().default_os_user_path_prefix(), "license_auth", "00001.data"), strUsernamePrevious, "", get_app());
      System.crypt().file_get(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsernamePrevious, "license_auth/00002.data"), strPasshashPrevious, calc_key_hash(), get_app());

      if((strUsername.has_char() && strPasshash.has_char())
      && (strUsernamePrevious != strUsername || strPasshashPrevious != strPasshash))
      {
         System.crypt().file_set(Application.dir().usersystemappdata(Application.dir().default_os_user_path_prefix(), "license_auth", "00001.data"), strUsername, "", get_app());
         System.crypt().file_set(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/00002.data"), strPasshash, calc_key_hash(), get_app());
         if(strPassword.has_char())
         {
            string strSalt = System.crypt().v5_get_password_salt();
            System.crypt().file_set(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/00005.data"), strSalt, calc_key_hash(), get_app());
            string strPasshash2 = System.crypt().v5_get_password_hash(strSalt, strPassword);
            System.crypt().file_set(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/00010.data"), strPasshash2, calc_key_hash(), get_app());
         }
      }
      if(m_loginthread.m_strLicense.has_char())
      {
         stringa straLicense;
         straLicense.add(m_loginthread.m_strValidUntil);
         straLicense.add(System.datetime().international().get_gmt_date_time());
         System.crypt().file_set(Application.dir().default_userappdata(Application.dir().default_os_user_path_prefix(), strUsername, "license_auth/" + m_loginthread.m_strLicense + ".data"), straLicense.implode(";"), calc_ca2_hash(), get_app());
      }
      m_bLicense = true;
      m_puser = m_loginthread.m_puser;
   }

   validate::auth * validate::get_auth()
   {
/*      ::ca::create_context_sp createcontext(get_app());
      createcontext->m_bMakeVisible = true;
      form_document * pdoc = dynamic_cast < form_document * > (m_ptemplatePane->open_document_file(createcontext));
      userex::pane_tab_view * pview = pdoc->get_typed_view < userex::pane_tab_view > ();
      pview->set_view_creator(this);
      rect rectOpen;
      System.get_screen_rect(rectOpen);
      int iWidth = rectOpen.width();
      int iHeight = rectOpen.width();
      rectOpen.deflate(iWidth / 5, iHeight / 5);
      m_ptabview->GetParentFrame()->SetWindowPos(ZORDER_TOP, rectOpen.left,
         rectOpen.top,
         rectOpen.width(), rectOpen.height(), SWP_SHOWWINDOW);
      m_ptabview = pview;
      pview->add_tab("login", 1);
      pview->set_cur_tab_by_id(1);
      pview->GetParentFrame()->RedrawWindow();
         ::ca::live_signal livesignal;
         livesignal.keep(get_app());
      pview->GetTopLevelFrame()->RunModalLoop(MLF_NOIDLEMSG | MLF_NOKICKIDLE, &livesignal);
      return m_pauth;*/
      return NULL;
   }

   void validate::close_all()
   {
   }

   void validate::show_and_request_auth()
   {
   }

} // namespace fontopus

