#include "framework.h" // from "axis/net/net_sockets.h"
//#include "axis/net/net_sockets.h"


#ifdef BSD_STYLE_SOCKETS

//#include <openssl/rsa.h>
//#include <openssl/engine.h>
//#include <openssl/bn.h>
//#include <openssl/err.h>

#endif

//string chunk_split(const string & body,int32_t chunklen = 76,const string & end = "\r\n");


string chunk_split(const string & body,int32_t chunklen,const string & end)
{

   strsize pos = 0;
   string strRet;
   strsize iRead;
   while(pos < body.get_length())
   {
      iRead = MIN(chunklen,body.get_length() - pos);
      strRet += body.Mid(pos,iRead);
      strRet += end;
      pos += iRead;
   }

   return strRet;

}

typedef string(*SALT)(sp(::aura::application),const char *,stringa &);


namespace fontopus
{


   validate::validate(::aura::application * papp,const char * pszForm,bool bAuth,bool bInteractive):
      ::object(papp),
      m_loginthread(papp)
   {

      m_bInteractive          = bInteractive;

      if(bInteractive)
      {

         oprop("defer_registration") = "defer_registration";

      }

      m_bAuth                 = bAuth;
      m_strForm               = pszForm;
      m_bDeferRegistration    = false;
      sp(::aura::application) pgenapp = (papp);

      if(pgenapp != NULL)
      {

         try
         {

            pgenapp->keep_alive();

         }
         catch(...)
         {

         }

      }

      m_pauth           = NULL;
      m_pvOldWindow     = NULL;
      m_bInteractive    = bInteractive;
      m_bAuth           = bAuth;
      m_strForm         = pszForm;
      m_puser           = NULL;
      m_pauth           = NULL;

   }


   validate::~validate()
   {
      
   }


   ::fontopus::user * validate::get_user(const char * pszRequestingParty,const char * pszSessId)
   {

      m_loginthread.m_strSalt.Empty();

      string strApp = handler()->property("app");

      if(strApp == "simpledbcfg"
         || (strApp.begins_ci("app-core/netnode") && handler()->property("root_handler") == "webserver")
         || (strApp.begins_ci("app-core/netnode"))
         || strApp == "veievserver"
         || strApp == "simpledbcfg"
         //|| handler()->property("app") == "veriwell_mplite"      // churrasco 2011 m_strLicense
         || strApp == "app-core/netnodecfg"
         || strApp == "app-core/netnode_dynamic_web_server"
         || strApp == "app-core/netnode_dynamic_web_server_cfg"
         || strApp == "app-gtech/sensible_netnode"
         || strApp == "app-gtech/sensible_service")
      {
         m_puser = Session.fontopus()->allocate_user();
         m_puser->m_strPathPrefix = "system" + ::str::has_char(Application.handler()->m_varTopicQuery["systemid"],"-");
         m_puser->m_strLogin = system_user_1;
         return m_puser;
      }
      else if(handler()->property("app") == "app-core/mydns"
         || handler()->has_property("install")
         || handler()->has_property("uninstall"))
      {
         m_puser = Session.fontopus()->allocate_user();
         m_puser->m_strPathPrefix = "system" + ::str::has_char(Application.handler()->m_varTopicQuery["systemid"],"-");
         m_puser->m_strLogin = system_user_1;
         return m_puser;
      }
      //else if(!strcmp(System.get_module_name(), "productionapp")
      //   || !strcmp(System.get_module_name(), "productionbasisapp")
      //   || !strcmp(System.get_module_name(), "backupapp")
      //   || System.handler()->m_varTopicQuery["app"] == "production"
      //   || System.get_license_id() == "winservice_filesystemsize")
      else if(handler()->property("app") == "backup"
         || handler()->property("app") == "winservice_filesystemsize")
      {
         m_puser = Session.fontopus()->allocate_user();
         m_puser->m_strPathPrefix = "system" + ::str::has_char(Application.handler()->m_varTopicQuery["systemid"],"-");
         m_puser->m_strLogin = system_user_2;
         return m_puser;
      }

      string strDir;
      string strUsername;
      string strPasshash;
      string strHost(pszRequestingParty);

      stringa straRequestingServer;

      string strIgnitionServer = file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt");

      if(::str::ends_ci(strIgnitionServer,".ca2.cc"))
      {

         straRequestingServer.add(strIgnitionServer);

      }
      else
      {

         straRequestingServer.add("account.ca2.cc");

      }

      //straRequestingServer.add("eu-account.ca2.cc");
      ///straRequestingServer.add("asia-account.ca2.cc");
      if(strHost.is_empty())
      {

         if(::get_thread() != NULL && ::get_thread()->m_strWorkUrl.has_char())
         {

            strHost = ::get_thread()->m_strWorkUrl;

         }
         else
         {
         
            strHost = Application.file().as_string(System.dir().appdata() / "database\\text\\last_good_known_fontopus_com.txt");
            
            if(!straRequestingServer.contains_ci(strHost))
            {
            
               if(straRequestingServer.has_elements())
               {

                  strHost = straRequestingServer[0];

               }

            }

         }

      }
      if(System.url().get_server(strHost).has_char())
      {
         strHost = System.url().get_server(strHost);
      }

      if(straRequestingServer.contains(Application.handler()->m_varTopicQuery["fontopus"].get_string())
         && Application.handler()->m_varTopicQuery["sessid"].get_string().get_length() > 16)
      {
         m_loginthread.m_puser = Session.fontopus()->allocate_user();
         m_loginthread.m_puser->m_sessionidmap[Application.handler()->m_varTopicQuery["fontopus"].get_string()] = Application.handler()->m_varTopicQuery["sessid"].get_string();
         m_loginthread.m_puser->m_sessionidmap[strHost] = Application.handler()->m_varTopicQuery["sessid"].get_string();
         m_loginthread.m_puser->m_strFontopusServerSessId = Application.handler()->m_varTopicQuery["sessid"].get_string();
         m_loginthread.m_puser->m_strRequestingServer = strHost;
         xml::document documentBasicInfo(get_app());

         string strApiServer;

         //strApiServer = Application.handler()->m_varTopicQuery["fontopus"];

         //strApiServer.replace("account","api");

         strApiServer = "account.ca2.cc/api";

         property_set set(get_app());

         set["user"] = m_loginthread.m_puser;

         if(documentBasicInfo.load(Application.http().get("https://" + strApiServer + "/account/get_basic_info",set)))
         {
            string strLogin;
            if(documentBasicInfo.get_root()->get_attr("login",strLogin) && strLogin.find("@") > 0)
            {
               m_loginthread.m_puser->m_strLogin = strLogin;
               return  m_loginthread.m_puser;
            }
         }
      }

      m_loginthread.m_puser = Session.fontopus()->allocate_user();

      if(pszSessId != NULL && string(pszSessId).get_length() > 16)
      {
         m_loginthread.m_puser->m_sessionidmap[strHost] = pszSessId;
      }

      m_loginthread.m_strRequestingServer = strHost;

      string strPassword;

      string strResult = Application.fontopus_get_cred(get_app(), pszRequestingParty, null_rect(),strUsername,strPassword,"ca2","ca2",m_bInteractive);

      if(strResult != "ok")
      {

         ::aura::del(m_puser);
         
         if(strResult == "cancel")
         {

            throw new cancel_exception(get_app(),"User canceled login.");

         }

         return NULL;

      }

      if(strUsername.has_char() && strPassword.has_char())
      {

         m_loginthread.m_strUsername = strUsername;
         m_loginthread.m_strPassword = strPassword;
         m_loginthread.m_strPasshash.Empty();
         m_loginthread.m_pcallback = this;
         m_loginthread.run();

         if(m_puser != NULL)
         {

            ::fontopus::set_cred(get_app(),"ca2",strUsername,strPassword);
            ::fontopus::set_cred_ok(get_app(),"ca2",true);

         }
         else
         {
            ::fontopus::set_cred(get_app(),"ca2","","");
            ::fontopus::set_cred_ok(get_app(),"ca2",false);
         }

         return m_puser;

      }
      else
      {

         return NULL;

      }

   }


   bool validate::get_license(const char * psz)
   {
      string strLicense(psz);
      if(strLicense == "netnodeapp"
         || strLicense == "netnodecfgapp"
         || strLicense == "simpledbcfg"
         || (strLicense.begins_ci("app-core/netnode") && handler()->property("root_handler") == "webserver")
         || (strLicense.begins_ci("app-core/netnode"))
         || strLicense == "veievserver"
         //|| strLicense == "veriwell_mplite"      // churrasco 2011 m_strLicense
         || strLicense == "mydns"
         || Application.handler()->m_varTopicQuery.has_property("install")
         || Application.handler()->m_varTopicQuery.has_property("uninstall")
         || strLicense == "netnode"
         || strLicense == "netnode_dynamic_web_server"
         || strLicense == "app-core/netnodecfg"
         || strLicense == "app-core/netnode_dynamic_web_server"
         || strLicense == "app-core/netnode_dynamic_web_server_cfg"
         || strLicense == "app-gtech/sensible_netnode")
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
      string strHost = Application.file().as_string(System.dir().appdata()/"database\\text\\last_good_known_fontopus_com.txt");
      stringa straRequestingServer;

      string strFirstFontopusServer = Session.fontopus()->m_strFirstFontopusServer;

      if(strFirstFontopusServer.has_char())
      {
         
         straRequestingServer.add(strFirstFontopusServer);

         strHost = strFirstFontopusServer;

      }
      else
      {
         straRequestingServer.add("account.ca2.cc");

         //straRequestingServer.add("eu-account.ca2.cc");
         //straRequestingServer.add("asia-account.ca2.cc");
         if(System.url().get_server(strHost).has_char())
         {
            strHost = System.url().get_server(strHost);
         }
         else
         {
            strHost = "account.ca2.cc";
         }

         strHost = Session.fontopus()->get_server(strHost);

      }

      if(straRequestingServer.contains(Application.handler()->m_varTopicQuery["fontopus"].get_string())
         && Application.handler()->m_varTopicQuery["sessid"].get_string().get_length() > 16)
      {
         strHost = Application.handler()->m_varTopicQuery["fontopus"].get_string();
      }

      string strAuthUrl;

      string strApiHost = strHost;

      //strApiHost.replace("account","api");

      string strReqSessId = Session.fontopus()->m_mapFontopusSessId[strHost];

      strAuthUrl = "https://" + System.dir().get_api_cc() + "/account/license3?sessid=" + strReqSessId;

      property_set set;

      
      set["post"]["entered_license"] = m_strLicense;
      //m_puser->set_sessid(ApplicationUser.m_str, strAuthUrl);
      //      uint32_t dwTimeProfile1 = get_tick_count();

      set["post"]["entered_license"] = m_strLicense;

      string strAuth = Application.http().get(strAuthUrl, set);

      m_loginthread.m_strFontopusServer = strHost;

      xml::document doc(get_app());
      doc.load(strAuth);
      if(doc.get_root()->get_name() == "response")
      {

         string strDecrypt;

         string strId = doc.get_root()->attr("id");

         string strLicense = doc.get_root()->attr("license");

         string strResSessId = doc.get_root()->attr("sessid");

         if(strId.has_char() 
            && strId == "licensed"
            && m_strLicense == strLicense
            && strReqSessId == strResSessId)
         {

            string strLogStats = doc.get_root()->attr("stats");

            return true;

         }

      }

      if(m_bInteractive)
      {

#ifdef WINDOWSEX

         if(get_splash() != NULL)
         {

            ::KillTimer(get_splash(),123);

         }

         {

            ::PostMessage(get_splash(),WM_CLOSE,0,0);

         }

#endif // WINDOWS


         ::simple_message_box(NULL,"No license for using application/feature \"" + string(psz) + "\" by user \"" + ApplicationUser.m_strLogin + "\".\n\nPlease, contact your system administrator.",psz,MB_ICONEXCLAMATION);


         return false;


//         m_loginthread.m_strModHash.Empty();
//         m_loginthread.m_strKeyHash.Empty();
//         m_loginthread.m_strCa2Hash.Empty();
//         return m_bLicense;

      }

      return false;

   }







   login_thread::login_thread(::aura::application * papp):
      ::object(papp),
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

   void validate::get_mod(stringa & straHash,stringa & straSource)
   {
      straHash.remove_all();
      straSource.remove_all();
   }


   string validate::calc_mod_hash()
   {

      if(m_loginthread.m_strModHash.has_char())
         return m_loginthread.m_strModHash;

      stringa straHash;

      stringa straSource;

      get_mod(straHash,straSource);

      m_loginthread.m_strModHash = System.crypto().md5(straHash.implode(";"));

      return m_loginthread.m_strModHash;

   }


   string validate::calc_key_hash()
   {
      if(m_loginthread.m_strKeyHash.has_char())
         return m_loginthread.m_strKeyHash;
      {
         m_loginthread.m_strKeyHash = "ca2_12n";
         return "ca2_12n";
      }
   }

   // return hash and check if hash is valid
   bool validate::check_ca2_hash()
   {
      string strUrl("https://" + System.dir().get_api_cc() + "/account/check_hash");
      property_set set;
      string strResponse;
      stringa straHash;
      stringa straSource;
      get_mod(straHash,straSource);
      straHash.insert_at(0,m_loginthread.m_strPasshash);
      straHash.insert_at(0,m_loginthread.m_strUsername);

      set["post"]["hash"] = straHash.implode(";");
      set["post"]["source"] = straHash.implode(";");
      for(int32_t i = 0; i < 3; i++)
      {
         if(Application.http().get(strUrl,strResponse,set))
            break;
      }
      if(strResponse == "OK")
         return true;
      else
         return false;
   }

   void validate::on_login_response(e_result iAuth,const char * pszResponse)
   {
      if(iAuth == result_auth)
      {
         authentication_succeeded();
      }
      else
      {
         authentication_failed(iAuth,pszResponse);
      }
   }

   void login_callback::on_login_response(e_result iAuth,const char * pszResponse)
   {
      UNREFERENCED_PARAMETER(iAuth);
      UNREFERENCED_PARAMETER(pszResponse);
   }

   string login_callback::calc_mod_hash()
   {
      return "";
   }

   string login_callback::calc_key_hash()
   {
      return "";
   }

   string login_callback::calc_ca2_hash()
   {
      return "";
   }


   bool login_callback::get_defer_registration()
   {
      return true;
   }

   
   void login_thread::run()
   {

      int iRetryLogin = 0;
      
      ::http::e_status estatus;

//      RetryLogin:
      
      string strResponse = Login(&estatus);

      if(m_pcallback == NULL)
         return 2;

      e_result iAuth = result_fail;
      xml::document doc(get_app());
      doc.load(strResponse);
      if(doc.get_root()->get_name() == "response")
      {

         string strDecrypt;

         string strId = doc.get_root()->attr("id");

         if(strId.has_char() && strId == "auth")
         {

               Session.fontopus()->m_authmap[m_strUsername].m_mapServer[m_strRequestingServer] = strResponse;
               Session.fontopus()->m_authmap[m_strUsername].m_mapFontopus[m_strFontopusServer] = strResponse;
               m_puser->m_strLogin = m_strUsername;
               m_puser->m_strFontopusServerSessId = doc.get_root()->attr("sessid");
               m_puser->set_sessid(m_puser->m_strFontopusServerSessId,m_strLoginUrl);
               m_puser->m_strRequestingServer = m_strRequestingServer;
               m_puser->set_sessid(m_puser->m_strFontopusServerSessId,m_strRequestingServer);
               m_puser->m_strFunUserId = doc.get_root()->attr("secureuserid");
               m_puser->m_strLoginStats = doc.get_root()->attr("stats");
               m_strPasshash = doc.get_root()->attr("passhash");
               iAuth = result_auth;
               if(m_bFontopusServer)
               {
                  Application.file().put_contents(System.dir().appdata()/"database\\text\\last_good_known_fontopus_com.txt",m_strFontopusServer);
               }
               execute();
               if(m_strLicense.has_char())
               {
                  m_strValidUntil = doc.get_root()->attr("valid_until");
               }

            //string strRsaModulus;

            //strRsaModulus = Session.fontopus()->m_mapFontopusRsa[m_strFontopusServer];

            //if(strRsaModulus.has_char())
            //{

            //   strDecrypt = System.crypto().spa_auth_decrypt(strId,strRsaModulus);

            //}

         }

         else if(strDecrypt.has_char() && doc.get_root()->attr("passhash").has_char() && doc.get_root()->attr("secureuserid").has_char())
         {

            //string strHash = System.crypto().sha1("\"authorization granted(71d0e601ae5e787ea898774115b2aa2a)\":" + m_puser->get_session_secret());

            //if(strDecrypt.compare_ci(strHash) != 0)
            //{

            //   if(iRetryLogin > 3)
            //   {

            //      delete m_puser;

            //      iAuth = result_fail;

            //   }
            //   else
            //   {

            //      iRetryLogin++;

            //      goto RetryLogin;

               //}

            //}
            //else
            //{

            //   Session.fontopus()->m_authmap[m_strUsername].m_mapServer[m_strRequestingServer] = strResponse;
            //   Session.fontopus()->m_authmap[m_strUsername].m_mapFontopus[m_strFontopusServer] = strResponse;
            //   m_puser->m_strLogin = m_strUsername;
            //   m_puser->m_strFontopusServerSessId = doc.get_root()->attr("sessid");
            //   m_puser->set_sessid(m_puser->m_strFontopusServerSessId,m_strLoginUrl);
            //   m_puser->m_strRequestingServer = m_strRequestingServer;
            //   m_puser->set_sessid(m_puser->m_strFontopusServerSessId,m_strRequestingServer);
            //   m_puser->m_strFunUserId = doc.get_root()->attr("secureuserid");
            //   m_puser->m_strLoginStats = doc.get_root()->attr("stats");
            //   m_strPasshash = doc.get_root()->attr("passhash");
            //   iAuth = result_auth;
            //   if(m_bFontopusServer)
            //   {
            //      Application.file().put_contents(System.dir().appdata()/"database\\text\\last_good_known_fontopus_com.txt",m_strFontopusServer);
            //   }
            //   execute();
            //   if(m_strLicense.has_char())
            //   {
            //      m_strValidUntil = doc.get_root()->attr("valid_until");
            //   }

            //}

         }
         else if(doc.get_root()->attr("id") == "registration_deferred")
         {
            
            m_puser.release();
            
            iAuth = result_registration_deferred;
            
         }
         else if(doc.get_root()->attr("id") == "not_auth")
         {

            if(doc.get_root()->attr("detail") == "please_finish_registration")
            {

               iAuth = result_please_finish_registration;

            }
            else if(doc.get_root()->attr("detail") == "wrong_password_or_login")
            {

               iAuth = result_wrong_password_or_login;

            }
            else if(doc.get_root()->attr("detail") == "no_login")
            {

               iAuth = result_no_login;

            }
            else if(doc.get_root()->attr("detail") == "no_password")
            {

               iAuth = result_no_password;

            }
            else
            {

               iAuth = result_fail;

            }

            m_puser.release();

         }
         else
         {
            
            m_puser.release();
            
            iAuth = result_fail;
            
         }

      }
      else
      {

         m_puser.release();

         if(estatus == ::http::status_connection_timed_out)
         {

            iAuth = result_time_out;

         }
         else
         {

            iAuth = result_fail;

         }

      }
      //      char * psz = NULL;
      //    *psz = '2';
      m_pcallback->on_login_response(iAuth,strResponse);
      
      return 0;

   }

   string login_thread::Login(::http::e_status * pestatus)
   {

      if(m_straRequestingServer.get_count() <= 0)
      {

         stringa straRequestingServer;

         string strIgnitionServer = file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt");

         if(::str::ends_ci(strIgnitionServer,".ca2.cc"))
         {
            
            straRequestingServer.add(strIgnitionServer);

         }
         else
         {

            straRequestingServer.add("account.ca2.cc");

         }

         //straRequestingServer.add("eu-account.ca2.cc");
         //straRequestingServer.add("asia-account.ca2.cc");
         if(straRequestingServer.contains_ci(m_strRequestingServer))
         {
            m_bFontopusServer = true;
            straRequestingServer.remove(m_strRequestingServer);
            straRequestingServer.insert_at(0,m_strRequestingServer);
            m_straRequestingServer = straRequestingServer;
         }
      }
      if(m_straRequestingServer.get_count() > 0)
      {
         string strLogin;
         for(int32_t i = 0; i < m_straRequestingServer.get_count(); i++)
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

   }


   string login_thread::NetLogin(::http::e_status * pestatus)
   {

      if(Session.fontopus()->m_authmap[m_strUsername].m_mapServer[m_strRequestingServer].get_length() > 32)
      {
         return Session.fontopus()->m_authmap[m_strUsername].m_mapServer[m_strRequestingServer];
      }

      //m_strRequestingServer = "account.ca2.cc";

      string strFontopusServer = Session.fontopus()->get_fontopus_server(m_strRequestingServer);

      if(strFontopusServer.is_empty())
         return "";

      m_strFontopusServer = strFontopusServer;

      if(Session.fontopus()->m_strFirstFontopusServer.is_empty())
      {

         Session.fontopus()->m_strFirstFontopusServer = m_strFontopusServer;
         Session.fontopus()->m_strFirstAccountServer = Session.fontopus()->m_mapSomeBrothersAndSisters[m_strFontopusServer][0];

      }

      if(Session.fontopus()->m_authmap[m_strUsername].m_mapFontopus[m_strFontopusServer].get_length() > 32)
      {
         return Session.fontopus()->m_authmap[m_strUsername].m_mapFontopus[m_strFontopusServer];
      }



      string strLogin;

      string strApiServer;

      if(m_strRequestingServer == "account.ca2.cc")
      {

         strApiServer = "account.ca2.cc";

      }
      else
      {

         strApiServer = m_strFontopusServer;

      }
//     strApiServer = "account.ca2.cc";

      m_strFontopusServer = strApiServer;

      m_strLoginUrl = "https://" + strApiServer + "/api/account/login";

      xml::document doc(get_app());

      string strSessId;

      string strRsaModulus;

      DWORD dwGetLoginBeg = ::get_tick_count();
      property_set set;


      set["app"] = get_app();

      strSessId = Session.fontopus()->m_mapFontopusSessId[m_strFontopusServer];

      strRsaModulus = Session.fontopus()->m_mapFontopusRsa[m_strFontopusServer];

      if(strSessId.is_empty())
         return "";

      if(strRsaModulus.is_empty())
         return "";

      DWORD dwGetLoginEnd = ::get_tick_count();

      TRACE("NetLogin: Get Login Millis = %d",dwGetLoginEnd - dwGetLoginBeg);

      string strPass;
      if(m_strPasshash.is_empty())
      {
         strPass = m_strPassword;
      }
      else
      {
         strPass = m_strPasshash;
      }

//      string strHex = System.crypto().spa_login_crypt(strPass,strRsaModulus);

//      string strSec = System.crypto().spa_login_crypt(m_puser == NULL ? string() : m_puser->m_strSessionSecret,strRsaModulus);

      if(m_puser != NULL)
      {

         m_puser->m_strLogin = m_strUsername;

      }

      string strAuth;

      DWORD dwAuthBeg = ::get_tick_count();
      {

         string strAuthUrl("https://" + strApiServer + "/api/account/auth3?" + (m_pcallback == NULL ? string() : m_pcallback->oprop("defer_registration").get_string())
            + (m_pcallback == NULL ? string() : "&ruri=" + System.url().url_encode((m_pcallback->oprop("ruri").get_string()))));

         property_set set;

         //if(m_strPasshash.is_empty())
         {
         //   set["post"]["entered_password"] = strPass;
         }
         //else
         //{
         set["post"]["entered_passhash"] = strPass;
         //}
         string strCrypt;

         //set["post"]["session_secret"] = strSec;

         string strUsername = m_strUsername;

         set["post"]["entered_login"] = m_strUsername;
         if(m_strLicense.has_char())
         {
            set["post"]["entered_license"] = m_strLicense;
         }

         if(m_puser != NULL)
         {

            m_puser->set_sessid(strSessId,strAuthUrl);
            m_puser->set_sessid(strSessId,strFontopusServer);

         }

         set["app"] = get_app();
         set["user"] = m_puser;
         
         if(m_puser != NULL)
         {

            set["cookies"] = m_puser->m_phttpcookies;

         }
         
         uint32_t dwTimeProfile1 = get_tick_count();

         strAuth = Application.http().get(strAuthUrl, set);
         

         *pestatus = (::http::e_status) set["get_status"].int64();
         uint32_t dwTimeProfile2 = get_tick_count();

         TRACE("login_thread::NetLogin Total time Application.http().get(\"%s\") : %d", strAuthUrl, dwTimeProfile2 - dwTimeProfile1);

      }
      DWORD dwAuthEnd = ::get_tick_count();

      TRACE("NetLogin: Authentication Millis = %d",dwAuthEnd - dwAuthBeg);

      return strAuth;

   }


   void login_thread::execute()
   {
      string strFilename;
      string strResponse;
      property_set set;
      for(int32_t i = 0; i < m_httpexecutea.get_size(); i++)
      {

         strFilename = System.file().time_square(get_app());

         set["post"] = m_httpexecutea[i]->m_propertysetPost;
         set["headers"] = m_httpexecutea[i]->m_propertysetHeaders;
         set["cookies"] = m_puser->m_phttpcookies;
         set["user"] = m_puser;

         System.http().download(m_httpexecutea[i]->m_strUrl,strFilename,set);

         strResponse = Application.file().as_string(strFilename);
         TRACE("%s", strResponse);
         m_httpexecutea[i]->m_strResponse = strResponse;
      }
   }



   void validate::save_authentication_info()
   {
      ::net::address address(m_loginthread.m_strFontopusServer);
      m_loginthread.m_puser->m_strFontopusServerInfo = m_loginthread.m_strFontopusServer + " : " + address.get_display_number() ;
      TRACE("The authentication has succeeded (%s[%s]).", m_loginthread.m_strFontopusServer, address.get_display_number());
      TRACE("Fontopus Server info = %s", m_loginthread.m_puser->m_strFontopusServerInfo);

      

      string strUsername = m_loginthread.m_strUsername;
      string strPasshash = m_loginthread.m_strPasshash;
      string strPassword = m_loginthread.m_strPassword;

      string strUsernamePrevious;
      string strPasshashPrevious;
      Application.file().crypto_get(::dir::userappdata() / "license_auth/00001.data",strUsernamePrevious,"");
      Application.file().crypto_get(::dir::userappdata() / "license_auth/00002.data",strPasshashPrevious,calc_key_hash());

      if((strUsername.has_char() && strPasshash.has_char())
         && (strUsernamePrevious != strUsername || strPasshashPrevious != strPasshash))
      {
         Application.file().crypto_set(::dir::userappdata() / "license_auth/00001.data",strUsername,"");
         Application.file().crypto_set(::dir::userappdata() / "license_auth/00002.data",strPasshash,calc_key_hash());
         if(strPassword.has_char())
         {
            string strSalt = System.crypto().v5_get_password_salt();
            Application.file().crypto_set(::dir::userappdata() / "license_auth/00005.data",strSalt,calc_key_hash());
            string strPasshash2 = System.crypto().v5_get_password_hash(strSalt,strPassword);
            Application.file().crypto_set(::dir::userappdata() / "license_auth/00010.data",strPasshash2,calc_key_hash());
         }
      }
      if(m_loginthread.m_strLicense.has_char())
      {
         stringa straLicense;
         straLicense.add(m_loginthread.m_strValidUntil);
         straLicense.add(System.datetime().international().get_gmt_date_time());
         Application.file().crypto_set(::dir::userappdata() / "license_auth" / m_loginthread.m_strLicense + ".data",straLicense.implode(";"),calc_ca2_hash());
      }
      m_bLicense = true;
      m_puser = m_loginthread.m_puser;
   }




   bool validate::get_defer_registration()
   {

      return m_bDeferRegistration;

   }




   void validate::authentication_failed(::fontopus::e_result iAuth,const char * pszResponse)
   {

      UNREFERENCED_PARAMETER(pszResponse);

      property_set propertyset;

      string strUsername = m_loginthread.m_strUsername;

      m_bLicense = false;
      m_puser = NULL;

      if(m_strLicense.has_char())
      {
         if(m_bInteractive)
         {
            string strUrl;
            strUrl = "http://api.ca2.cc/spaignition/query?node=install_application&id=";
            strUrl += m_strLicense;
            strUrl += "&key=launch_name";

            property_set set(get_app());

            string strName = Application.http().get(strUrl,set);

            if(strName.is_empty())
               strName = m_strLicense;
            propertyset["project"] = strName;
            strUrl = "ext://https://" + m_loginthread.m_strFontopusServer + "/license?id=" + m_strLicense + "&lang=" + Session.get_locale() + "&sessid=" + ApplicationUser.get_sessid(m_loginthread.m_strFontopusServer);
            propertyset["contribute_link"] = strUrl;
            pageMessage({"err\\user\\authentication\\not_licensed.xhtml"},propertyset);
         }
      }
      else if(iAuth == ::fontopus::result_please_finish_registration)
      {
         if(m_bInteractive)
         {
            stringa stra;
            stra.add("err\\user\\authentication\\please_finish_registration.html");
            stra.add("err\\user\\authentication\\registration_deferred.html");
            pageMessage(stra,propertyset);
         }
      }
      else if(iAuth == ::fontopus::result_wrong_password_or_login)
      {
         if(m_bInteractive)
         {
            propertyset["register_link"] = "ext://http://" + m_loginthread.m_strFontopusServer + "/register?email=" + System.url().url_encode(m_loginthread.m_strUsername);
            pageMessage({"err\\user\\authentication\\wrong_fontopus_login.html"},propertyset);
            try
            {
               Application.file().del(::dir::userappdata() / "license_auth/00001.data");
            }
            catch(...)
            {
            }
            try
            {
               Application.file().del(::dir::userappdata() / "license_auth/00002.data");
            }
            catch(...)
            {
            }
            m_loginthread.m_strModHash.Empty();
            m_loginthread.m_strKeyHash.Empty();
            m_loginthread.m_strCa2Hash.Empty();
         }
      }
      else if(iAuth == ::fontopus::result_time_out)
      {
         if(m_bInteractive)
         {
            propertyset["server"] = "account.ca2.cc";
            pageMessage({"err\\user\\network\\connection_timed_out.html"},propertyset);
         }
      }
      else if(iAuth == ::fontopus::result_registration_deferred)
      {
         if(m_bInteractive)
         {
            propertyset["server"] = "account.ca2.cc";
            propertyset["email"] = strUsername;
            pageMessage({"err\\user\\authentication\\registration_deferred.html"},propertyset);
         }
      }
      else if(iAuth == ::fontopus::result_no_login)
      {
         if(m_bInteractive)
         {
            pageMessage({"err\\user\\authentication\\no_login.html"},propertyset);
         }
      }
      else if(iAuth == ::fontopus::result_no_password)
      {
         if(m_bInteractive)
         {
            pageMessage({"err\\user\\authentication\\no_password.html"},propertyset);
         }
      }
      else
      {
         if(m_bInteractive)
         {
            pageMessage({"err\\user\\authentication\\failed.html"},propertyset);
         }
      }

      delete m_pauth;

   }

   void validate::authentication_succeeded()
   {

      save_authentication_info();

   }

   validate::auth * validate::get_auth()
   {
      sp(::create) createcontext(allocer());
      createcontext->m_bMakeVisible = true;
      return NULL;
   }


   string validate::calc_ca2_hash()
   {
      if(m_loginthread.m_strCa2Hash.has_char())
         return m_loginthread.m_strCa2Hash;
      m_loginthread.m_strCa2Hash = "ca2_12n";
      return m_loginthread.m_strCa2Hash;
   }

   void validate::pageMessage(stringa straMessage,::property_set set)
   {

      UNREFERENCED_PARAMETER(straMessage);
      UNREFERENCED_PARAMETER(set);


   }


} // namespace fontopus





