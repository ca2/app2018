//#include "framework.h" // from "axis/net/net_sockets.h"
//#include "axis/net/net_sockets.h"



namespace fontopus
{


   fontopus::fontopus(::aura::application * papp) :
      ::object(papp),
      ::aura::department(papp),
      m_mutex(papp)
   {

      m_puser                       = NULL;

      m_pthreadCreatingUser      = NULL;

   }


   fontopus::~fontopus()
   {

   }


   bool fontopus::initialize_instance()
   {

      return true;

   }


   int32_t fontopus::exit_instance()
   {

      int32_t iExitCode = 0;

      return iExitCode;

   }


   user * fontopus::create_current_user(const char * pszRequestUrl)
   {

      property_set set(get_app());

      if(m_puser == NULL)
      {

         if(pszRequestUrl != NULL)
         {

            set["ruri"] = pszRequestUrl;

         }

         int32_t iRetry = 3;
         ::fontopus::user * puser = NULL;
         while(iRetry > 0 && (::get_thread() == NULL || ::get_thread()->m_bRun))
         {
            try
            {
               puser = login(set);
            }
            catch(const cancel_exception &)
            {

               return NULL;

            }
            if(puser != NULL)
               break;
            //bool bOk = false;
            //string strSection;
            //strSection.Format("license_auth");
            //string strDir;
            //strDir = Application.dir().usersystemappdata(puser->m_strPathPrefix, strSection);
            //::DeleteFile(strDir / "00001"));
            //::DeleteFile(strDir / "00002"));
            iRetry--;
         }
         if(puser == NULL)
         {
            //System.simple_message_box(NULL, "<h1>You have not logged in!</h1><h2>Exiting</h2>");
            TRACE("<error>You have not logged in! Exiting!</error>");
            //throw string("You have not logged in! Exiting!");
            //debug_box("You have not logged in!","Debug Message",MB_OK);
            return NULL;
         }
         m_puser = create_user(puser);
         System.userset().add(m_puser);


         if(m_puser != NULL
            && !::str::begins(m_puser->m_strLogin, astr.strSystem)
            && m_pauraapp->m_strAppId != "app-core/deepfish"
            && !::str::begins(m_pauraapp->m_strAppName, astr.strAppCoreDeepfish)
            && !m_pauraapp->is_serviceable())
         {

            on_user_login(m_puser);

         }



      }
      return m_puser;
   }

   user * fontopus::login(property_set & set)
   {

      user * puser = NULL;

      class validate authuser(get_app(), "system\\user\\authenticate.xhtml", true);

      authuser.m_bDeferRegistration = true;

      authuser.oprop_set().merge(set);

      if(set.has_property("ruri"))
      {

         puser = authuser.get_user(set["ruri"]);

      }
      else
      {

         puser = authuser.get_user();

      }

      return puser;

   }

   bool fontopus::get_auth(const char * psz, string & strUsername, string & strPassword)
   {
          /*::draw2d::fontopus * papp;
      if(m_puiInitialPlaceHolderContainer != NULL)
      {
      papp = m_puiInitialPlaceHolderContainer->m_pauraapp;
      }
      else if(System.m_puiInitialPlaceHolderContainer != NULL)
      {
      papp = System.m_puiInitialPlaceHolderContainer->m_pauraapp;
      }
      else
      {
      papp = this;
      }*/
      //class validate authuser(papp, psz);
      class validate authuser(get_app(), psz);
      validate::auth * pauth = authuser.get_auth();
      if(pauth == NULL)
         return false;
      else
      {
         strUsername = pauth->m_strUsername;
         strPassword = pauth->m_strPassword;
         delete pauth;
         return true;
      }

   }

   void fontopus::logout()
   {
      
      if(m_puser != NULL)
      {

         try
         {
            
            Application.file().del(::dir::userappdata()/"license_auth/00001.data");

         }
         catch(...)
         {

         }

         try
         {

            Application.file().del(::dir::userappdata()/"license_auth/00002.data");

         }
         catch(...)
         {

         }

         delete m_puser;

      }

      m_puser = NULL;
   }



   user * fontopus::allocate_user()
   {
      return new class user(m_pauraapp);
   }

   user * fontopus::create_user(::fontopus::user * puser)
   {
      if(puser->m_strPathPrefix.is_empty())
      {
         puser->m_strPathPrefix = Application.dir().default_os_user_path_prefix();
      }
      puser->m_strPath = Application.dir().default_userfolder(puser->m_strPathPrefix, puser->m_strLogin);
      Sess(m_pauraapp).dir().mk(puser->m_strPath);
      puser->m_strDataPath = Application.dir().default_userdata(puser->m_strPathPrefix, puser->m_strLogin);
      Sess(m_pauraapp).dir().mk(puser->m_strDataPath);
      puser->m_strAppDataPath = Application.dir().default_userappdata(puser->m_strPathPrefix, puser->m_strLogin);
      Sess(m_pauraapp).dir().mk(puser->m_strAppDataPath);
      puser->create_ifs();
      return puser;
   }


   user * fontopus::create_system_user(const string & strSystemUserName)
   {

      m_puser                 = new ::fontopus::user(get_app());

      m_puser->m_strLogin     = strSystemUserName;

      user * puserNew = create_user(m_puser);

      if(puserNew == NULL)
      {

         delete m_puser;

         m_puser = NULL;

      }

      return m_puser;

   }



   user * fontopus::get_user(bool bSynch, const char * pszRequestUrl)
   {

      if(m_puser == NULL)
      {

         if(m_pthreadCreatingUser == ::get_thread())
            return NULL;

         if(m_pthreadCreatingUser != NULL)
         {

            if(!bSynch)
               return m_puser;

            while(m_pthreadCreatingUser != NULL && m_pthreadCreatingUser->m_bRun && ::get_thread()->m_bRun)
            {

               do_events(millis(84));

               if(m_pthreadCreatingUser == NULL)
                  break;

               m_pthreadCreatingUser->m_evReady.wait(millis(0));

            }

            if(m_puser != NULL)
               return m_puser;

            return NULL;

         }

         m_pthreadCreatingUser = new create_user_thread(get_app());

         if(pszRequestUrl != NULL)
         {

            m_pthreadCreatingUser->m_strRequestUrl = pszRequestUrl;

         }

         m_pthreadCreatingUser->m_bAutoDelete = true;

         m_pthreadCreatingUser->begin();

         if(!bSynch)
            return m_puser;

         while(m_pthreadCreatingUser != NULL && m_pthreadCreatingUser->m_bRun && ::get_thread()->m_bRun)
         {

            do_events(millis(84));

            if(m_pthreadCreatingUser == NULL)
               break;

            try
            {

               m_pthreadCreatingUser->m_evReady.wait(millis(0));

            }
            catch(...)
            {

               break;

            }

         }

         if(m_puser != NULL)
            return m_puser;

         return NULL;

      }

      if(m_puser != NULL)
      {

         if(m_puser->m_pifs == NULL)
         {

            m_puser->create_ifs();

         }

      }

      return m_puser;

   }

   void fontopus::set_user(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void fontopus::set_user(user * puser)
   {
      m_puser = puser;
   }

   bool fontopus::check_license(const char * pszId, bool bInteractive)
   {

      class validate authuser(get_app(), "err\\user\\authentication\\not_licensed.xhtml", true, bInteractive);
      return authuser.get_license(pszId);


   }

   void fontopus::on_user_login(user * puser)
   {

      Session.on_user_login(puser);


#ifdef METROWIN

      //if(!System.directrix()->m_varTopicQuery.has_property("install")
      //   && !System.directrix()->m_varTopicQuery.has_property("uninstall"))
      //{

      //   sp(::create) spcreatecontext(allocer());

      //   ::aura::application * papp = Session.start_application("application", "app-core/deepfish", spcreatecontext);

      //   if(papp == NULL)
      //   {
      //      Application.simple_message_box("deepfish subsystem - responsible for running background applications - could not be started", MB_ICONEXCLAMATION);
      //   }

      //}

#endif

   }







   void fontopus::on_request(sp(::create) pcreatecontext)
   {

      if(pcreatecontext->m_spCommandLine.is_set()
         && pcreatecontext->m_spCommandLine->m_varFile == "ca2login")
      {
         //Sleep(15 * 1000);
         property_set setLogin(get_app());
         if(pcreatecontext->m_spCommandLine->m_varQuery["ruri"].is_set())
         {
            setLogin["ruri"] = pcreatecontext->m_spCommandLine->m_varQuery["ruri"];
         }
         ::fontopus::user * puser = NULL;
         while(true)
         {
            if((puser = login(setLogin)) != NULL)
               break;
         }
         if(pcreatecontext->m_spCommandLine->m_varQuery["ruri"].is_set())
         {
            string strUri = pcreatecontext->m_spCommandLine->m_varQuery["ruri"];
            System.url().string_set(strUri, "sessid", puser->m_strFontopusServerSessId);
            System.open_link(strUri);
         }
      }

   }

   string fontopus::get_server(const char * pszUrl,int32_t iRetry)
   {

      //return "account.ca2.cc";



      string strHost(System.url().get_server(pszUrl));

      if(strHost.is_empty())
      {

         strHost = System.url().get_server(pszUrl);

      }

      synch_lock sl(&m_mutex);

      string strFontopusServer;

      if(strHost == "account.ca2.cc")
      {
         strFontopusServer = "account.ca2.cc";
      }
      else
      {

         if(m_mapFontopusServer.Lookup(strHost,strFontopusServer) && strFontopusServer.has_char())
         {
            return strFontopusServer;
         }

      }

      sl.unlock();

      ::sockets::socket_handler h(get_app());

   retry:

      if(iRetry < 0)
         return ""; // should not retry or lookup is valid and strFontopusServer is really empty

      iRetry--;

      string strGetFontopus;

      if(::str::ends(strHost,".ca2.cc"))
      {

         if (m_strFirstAccountServer.has_char())
         {

            return m_strFirstAccountServer;

         }

         strGetFontopus = "https://" + strHost + "/get_fontopus_login";

      }
      else
      {
         strGetFontopus = "http://" + strHost + "/get_fontopus_login";
      }

      System.url().set_param(strGetFontopus,strGetFontopus,"lang",Session.get_locale());
      System.url().set_param(strGetFontopus,strGetFontopus,"styl",Session.get_schema());

      sp(::sockets::http_session) psession;

      string strNode;


      try
      {

         DWORD dwBeg = ::get_tick_count();

         ::property_set set(get_app());

         set["raw_http"] = true;

         set["get_response"] = "";

         psession = System.http().request(psession,strGetFontopus,set);

         strNode = set["get_response"];

         DWORD dwEnd = ::get_tick_count();

         TRACE("get_fontopus_login HTTP GET time = %dms",dwEnd - dwBeg);

      }
      catch(...)
      {
      }

      strNode.trim();

      if(strNode.is_empty())
         goto retry;

      ::xml::document doc(get_app());

      if(!doc.load(strNode))
         goto retry;

      if(doc.get_root()->get_name() != "login")
         goto retry;

      string strSessId = doc.get_root()->attr("sessid");

      if(strSessId.is_empty())
         goto retry;

      if(strHost != "account.ca2.cc")
      {

         strFontopusServer = doc.get_root()->attr("fontopus_server");

      }

      string strIgnitionServer = file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt");

      if(::str::ends_ci(strIgnitionServer,".ca2.cc"))
      {

         ::net::address addr1(strFontopusServer);

         ::net::address addr2(strIgnitionServer);

         if(addr1 == addr2)
         {

            strFontopusServer = strIgnitionServer;

         }

      }


      if(strFontopusServer.is_empty())
         goto retry;

      string strRsaModulus;

      strRsaModulus = doc.get_root()->attr("rsa_modulus");

      if(strRsaModulus.length() < 32)
         goto retry;

      string strSomeBrothersAndSisters;

      strSomeBrothersAndSisters = doc.get_root()->attr("some_brothers_and_sisters");

      //::sockets::net::dns_cache_item item = Session.sockets().net().m_mapCache[strHost];

      sl.lock();

      if(strSomeBrothersAndSisters.has_char())
      {

         stringa straSomeBrothersAndSisters;

         straSomeBrothersAndSisters.explode(";",strSomeBrothersAndSisters);

         for (index i = 0; i < straSomeBrothersAndSisters.get_count(); i++)
         {

            string strX = straSomeBrothersAndSisters[i];

            m_mapFontopusServer.set_at(straSomeBrothersAndSisters[i], strFontopusServer);

            strX.replace("-api.ca2.cc", "-account.ca2.cc");

            m_mapSomeBrothersAndSisters[strFontopusServer].add_unique(strX);

            //Session.sockets().net().m_mapCache.set_at(straSomeBrothersAndSisters[i],item);

         }

      }

      ::xml::node * pnodeForm = doc.get_root()->get_child("form");

      if(pnodeForm != NULL)
      {

         m_mapLabelUser[strFontopusServer] = pnodeForm->attr("email");
         m_mapLabelPass[strFontopusServer] = pnodeForm->attr("senha");
         m_mapLabelOpen[strFontopusServer] = pnodeForm->attr("abrir");

      }

      m_mapFontopusSession.set_at(strFontopusServer,psession);
      

      if(m_mapFontopusSessId[strFontopusServer].is_empty())
      {

         m_mapFontopusSessId.set_at(strFontopusServer,strSessId);

         m_mapFontopusRsa.set_at(strFontopusServer,strRsaModulus);

         m_mapFontopusServer.set_at(strFontopusServer,strFontopusServer);

//         Session.sockets().net().m_mapCache.set_at(strFontopusServer,item);

      }

      m_mapFontopusServer.set_at(strHost, strFontopusServer);

      //Session.sockets().net().m_mapCache.set_at(strHost,item);

      return strFontopusServer;

   }

   string fontopus::get_fontopus_server(const char * pszRequestingServerOrUrl,int32_t iRetry)
   {

      string strRequestingServer = System.url().get_server(pszRequestingServerOrUrl);

      DWORD dwGetFontopusBeg = ::get_tick_count();

      string strGetFontopus;

      if(::str::ends(strRequestingServer,".ca2.cc"))
      {
         strGetFontopus = "https://" + strRequestingServer + "/get_fontopus_login";
      }
      else
      {
         strGetFontopus = "https://" + strRequestingServer + "/get_fontopus_login";
      }

//      ::aura::application * papp = get_app();

      url_domain domainFontopus;

      string strFontopusServer = Session.fontopus()->get_server(strGetFontopus,8);

      domainFontopus.create(strFontopusServer);

      if(domainFontopus.m_strRadix != "ca2" && domainFontopus.m_strRadix != "fontopus")
         return "";

      if(strRequestingServer == "account.ca2.cc")
         return "account.ca2.cc";

      DWORD dwGetFontopusEnd = ::get_tick_count();

      TRACE("NetLogin: Get Fontopus Millis = %d",dwGetFontopusEnd - dwGetFontopusBeg);

      return strFontopusServer;

   }


   create_user_thread::create_user_thread(::aura::application * papp) :
      ::object(papp),
      ::thread(papp),
      m_evReady(papp)
   {

      m_evReady.ResetEvent();

   }


   create_user_thread::~create_user_thread()
   {


   }

   int32_t create_user_thread::run()
   {


#ifdef WINDOWSEX

      ::attach_thread_input_to_main_thread(false);

#endif

      ::fontopus::user * puser = Session.fontopus()->create_current_user(m_strRequestUrl);

      if(puser != NULL)
      {

         if(!puser->initialize())
         {

            delete puser;

            Session.fontopus()->m_puser = NULL;

         }
         else
         {

            Session.fontopus()->m_puser = puser;

         }

      }
      else
      {

         Session.fontopus()->m_puser = NULL;

      }

      m_evReady.SetEvent();

      Session.fontopus()->m_pthreadCreatingUser = NULL;

      return 0;

   }


} // namespace fontopus



