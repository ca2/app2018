﻿#include "framework.h" // from "aura/net/net_sockets.h"
#include "aura/net/net_sockets.h"
#include "aura/net/sockets/bsd/basic/sockets_ssl_context.h"
#include <time.h>
#include <stdio.h>


namespace http
{


   system::system(::aura::application * papp) :
      ::object(papp)
   {

//      m_phandler = new ::sockets::socket_handler(get_app());
      //    m_phandler->EnablePool();
      oprop("dw") = ::get_tick_count();

      m_pmutexPac = NULL;
      m_pmutexProxy = NULL;
      m_pmutexDownload = NULL;







   }


   system::~system()
   {

//      ::aura::del(m_phandler);

   }

   int32_t system::auto_config_proxy_count()
   {

      return 4;

   }


   void system::auto_config_proxy(int32_t i)
   {

      return;
//      if(i == 0)
//      {
//      }
//      else if(i == 1)
//      {
//         // telmico: no proxy
//         string str = Application.file().as_string(System.dir().appdata() / "machine/proxy.xml");
//         if(str.has_char() && str.find("<") >= 0 && str.find(">") > 0)
//         {
//            Application.file().copy(System.dir().appdata()/ "proxy_original.xml", System.dir().install()/ "proxy.xml", false);
//         }
//         if(Application.file().exists(System.dir().appdata()/ "proxy.xml"))
//         {
//            try
//            {
//               Application.file().del(System.dir().appdata()/ "proxy.xml");
//            }
//            catch(...)
//            {
//            }
//         }
//      }
//      else if(i == 2)
//      {
//         // telmico: original proxy configuration
//         if(Application.file().exists(System.dir().appdata()/ "proxy_original.xml"))
//         {
//            Application.file().copy(System.dir().appdata()/ "proxy.xml", System.dir().appdata()/"proxy_original.xml", false);
//         }
//      }
//      else
//      {
//         // telmico: simple default proxy configuration : hostname=>proxy - try etc/hosts port=>80  - assume HTTP proxy
//         string str = Application.file().as_string(System.dir().appdata()/"proxy.xml");
//         if(str.has_char() && str.find("<") >= 0 && str.find(">") > 0)
//         {
//            Application.file().copy(System.dir().appdata()/"proxy_original.xml", System.dir().appdata()/"proxy.xml", false);
//         }
//         Application.file().put_contents(System.dir().appdata()/"proxy.xml", "proxy");
//      }
   }

   void system::defer_auto_initialize_proxy_configuration()
   {
      /*
      if(!strcmp(System.get_module_name(), "netnodeapp")
         || !strcmp(System.get_module_name(), "netnodecfgapp")
         || System.get_license_id() == "simpledbcfg"
         || System.get_license_id() == "app-core/netnodelite"
         || System.handler()->m_varTopicQuery["app"] == "simpledbcfg")
      {
         return;
      }
      else if(System.handler()->m_varTopicQuery["app"] == "mydns"
         || System.handler()->m_varTopicQuery.has_property("install")
         || System.handler()->m_varTopicQuery.has_property("uninstall"))
      {
         return;
      }
      else if(!strcmp(System.get_module_name(), "productionapp")
         || !strcmp(System.get_module_name(), "productionbasisapp")
         || !strcmp(System.get_module_name(), "backupapp")
         || System.handler()->m_varTopicQuery["app"] == "production"
         || System.get_license_id() == "winservice_filesystemsize")
      {
         return;
      }
      */


      string strHost = Application.file().as_string(System.dir().appdata()/"database\\text\\last_good_known_fontopus_com.txt");
      stringa straRequestingServer;
      straRequestingServer.add("account.ca2.cc");
      //straRequestingServer.add("eu-account.ca2.cc");
      //straRequestingServer.add("asia-account.ca2.cc");
      if(!straRequestingServer.contains_ci(strHost))
      {
         strHost = "account.ca2.cc";
      }

      straRequestingServer.remove(strHost);
      straRequestingServer.insert_at(0, strHost);

      bool bOk = false;
      /*for(int32_t i = 0; i < straRequestingServer.get_count() && !bOk; i++)
      {
         string strUrl = "https://" + straRequestingServer[i] + "/hello";
         for(int32_t j = 0; j < auto_config_proxy_count(); j++)
         {
            auto_config_proxy(j);
            string str = get(strUrl);
            if(str == "Hello!!")
            {
               bOk = true;
               break;
            }
         }
      }*/

      if(bOk)
         return;

      auto_config_proxy(0);

   }

   system::pac::pac(::aura::application * papp) :
      ::object(papp)
   {

      m_pjs = new tinyjs(papp);

   }

   system::pac::~pac()
   {

      if(m_pjs == NULL)
      {

         delete m_pjs;

      }

   }


   system::pac * system::get_pac(const char * pszUrl)
   {

      single_lock sl(m_pmutexPac, true);

      auto ppair = m_mapPac.PLookup(pszUrl);

      if(ppair == NULL || (::get_tick_count() - ppair->m_element2->m_dwLastChecked) > (84 * 1000))
      {
         if(ppair != NULL)
         {
//            delete ppair->m_element2;
            m_mapPac.remove_key(pszUrl);
         }

         class pac * ppac = canew(class pac(get_app()));

         ppac->m_dwLastChecked = get_tick_count();

         ppac->m_strUrl = pszUrl;

         var varQuery;

         varQuery["disable_ca2_sessid"] = true;
         varQuery["no_proxy_config"] = true;

         ppac->m_strAutoConfigScript = Application.file().as_string(ppac->m_strUrl, varQuery);


         m_mapPac.set_at(pszUrl, ppac);

         if(ppac->m_strAutoConfigScript.is_empty())
         {
            return NULL;
         }

         registerFunctions(ppac->m_pjs);
         registerJavascriptFunctions(ppac->m_pjs);
         ppac->m_pjs->execute(ppac->m_strAutoConfigScript);

         ppair = m_mapPac.PLookup(pszUrl);

         if(ppair == NULL)
            return NULL;
      }

      if(ppair->m_element2->m_strAutoConfigScript.is_empty())
         return NULL;

      return ppair->m_element2;

   }


   system::proxy::proxy(::aura::application * papp) :
      ::object(papp)
   {
   }



   ::http::system::proxy * system::get_proxy(const char * pszUrl)
   {

      single_lock sl(m_pmutexProxy, true);

      auto ppair = m_mapProxy.PLookup(pszUrl);

      if(ppair == NULL || (::get_tick_count() - ppair->m_element2->m_dwLastChecked) > (84 * 1000))
      {
         if(ppair != NULL)
         {
//            delete ppair->m_element2;
            m_mapPac.remove_key(pszUrl);
         }

         class ::http::system::proxy * pproxy = canew(class ::http::system::proxy(get_app()));

         pproxy->m_dwLastChecked = get_tick_count();

         pproxy->m_strUrl = pszUrl;

         config_proxy(pszUrl, pproxy);

         m_mapProxy.set_at(pszUrl, pproxy);

         return pproxy;

      }

      return ppair->m_element2;

   }


   bool system::try_pac_script(const char * pszScriptUrl, const char * pszUrl, proxy * pproxy)
   {

      single_lock sl(m_pmutexPac, true);

      string strProxyServer;

      string strUrl(pszScriptUrl);

      if(::str::begins(pszUrl, strUrl))
      {
         pproxy->m_bDirect = true;
         return true;
      }

      class pac * ppac = get_pac(pszScriptUrl);

      if(ppac == NULL)
         return false;

      string strHost;

      strHost = System.url().get_server(pszUrl);
      int32_t port = System.url().get_port(pszUrl);

      /*         ipaddr_t l;
            if (!Session.sockets().net().u2ip(strHost,l))
            {
               return false;
            }*/
      ::net::address ad(strHost, port);

      strHost = ad.get_display_number();

      string var;
      try
      {
         var = ppac->m_pjs->evaluate("FindProxyForURL('" + string(pszUrl) + "', '" +strHost +"');");
      }
      catch(...)
      {
         return false;
      }

      if(var.compare_ci("DIRECT") == 0)
      {
         pproxy->m_bDirect = true;
      }
      else if(::str::begins_eat_ci(var, "PROXY"))
      {
         var.trim();
         stringa stra;
         stra.explode(":", var);
         pproxy->m_bDirect = false;
         pproxy->m_strProxy = stra[0];
         pproxy->m_iPort = stra.get_size() > 1 ? ::atoi(stra[1]) : 80;
      }
      else
      {
         pproxy->m_bDirect = true;
      }

      return true;

   }

   void system::config_proxy(const char * pszUrl, ::sockets::http_tunnel * psocket)
   {

#ifdef METROWIN

      psocket->m_bDirect = true;

#else

      ::http::system::proxy * pproxy = get_proxy(pszUrl);

      if(pproxy == NULL)
         return;

      if(pproxy->m_bDirect)
      {
         psocket->m_bDirect      = true;
      }
      else
      {
         psocket->m_bDirect      = false;
         psocket->m_strProxy     = pproxy->m_strProxy;
         psocket->m_iProxyPort   = pproxy->m_iPort;
      }

#endif

   }


   void system::config_proxy(const char * pszUrl, ::http::system::proxy * pproxy)
   {

      xml::document doc(get_app());
      ::file::path pathProxyXml = System.dir().appdata()/"proxy.xml";

      if(!Application.file().exists(pathProxyXml))
      {

         pproxy->m_bDirect = true;

         return;

      }

      string str = Application.file().as_string(pathProxyXml);


      if(str.has_char() && str.find("<") < 0 && str.find(">") < 0)
      {
         stringa stra;
         stra.explode(":", str);
         if(stra.get_size() > 0 && stra[0].has_char())
         {
            pproxy->m_bDirect = false;
            pproxy->m_strProxy = stra[0];
            if(stra.get_size() >= 2)
            {
               pproxy->m_iPort = atoi(stra[1]);
            }
            else
            {

               pproxy->m_iPort = 80;

            }

            return;

         }

      }

      if(!doc.load(str))
      {

         pproxy->m_bDirect = true;

         return;

      }

      bool bOk = true;

      string strHost = System.url().get_server(pszUrl);
      int32_t iHostPort = System.url().get_port(pszUrl);
      ::net::address ipHost(strHost, iHostPort);
      for(int32_t iNode = 0; iNode < doc.get_root()->get_children_count(); iNode++)
      {
         sp(::xml::node) pnode = doc.get_root()->child_at(iNode);
         if(pnode->get_name() == "proxy")
         {
            ::net::address ipAddress(pnode->attr("address").get_string(), 0);
            ::net::address ipMask(pnode->attr("mask").get_string(), 0);
            if(ipHost.is_in_same_net(ipAddress, ipMask))
            {
               if(pnode->attr("server") == "DIRECT")
               {
                  pproxy->m_bDirect = true;
                  return;
               }
               else
               {
                  pproxy->m_bDirect = false;
                  pproxy->m_strProxy = pnode->attr("server");
                  pproxy->m_iPort = pnode->attr("port");
                  TRACE("Select Proxy : address %s mask %s server %s port %d",pnode->attr("address").get_string(),
                        pnode->attr("mask").get_string(), pproxy->m_strProxy, pproxy->m_iPort);
                  return;
               }
            }
         }
      }
      if(doc.attr("server") == "DIRECT")
      {
         pproxy->m_bDirect = true;
         return;
      }
      else
      {
         pproxy->m_bDirect = false;
         pproxy->m_strProxy = doc.get_root()->attr("server");
         pproxy->m_iPort = doc.get_root()->attr("port");
         return;
      }


//      if(!bOk)
//      {
//
//         //bool bAutoDetect = System.os().connection_settings_get_auto_detect();
//
//         //if(bAutoDetect)
//         //{
//
//         //   TRACE("proxy auto_detect true");
//
//         //   string strUrl = System.os().connection_settings_get_auto_config_url();
//
//         //   if(strUrl.has_char())
//         //   {
//
//         //      TRACE("get_auto_config_url : %s",strUrl);
//
//         //      if(try_pac_script(strUrl,pszUrl,pproxy))
//         //         return;
//
//         //   }
//
//         //}
//         //else
//         //{
//
//         //   TRACE("proxy auto_detect false");
//
//         //   string strUrl = System.os().connection_settings_get_auto_config_url();
//
//         //   if(strUrl.has_char())
//         //   {
//
//         //      TRACE("get_auto_config_url : %s",strUrl);
//
//         //      if(try_pac_script(strUrl,pszUrl,pproxy))
//         //         return;
//
//         //   }
//
//         //   if(try_pac_script("http://wpad/wpad.dat",pszUrl,pproxy))
//         //      return;
//
//         //}
//
//
//         pproxy->m_bDirect = true;
//
//      }



   }

















   bool system::open(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession, const char * pszHost, const char * pszProtocol, property_set & set, ::fontopus::user * puser, const char * pszVersion)
   {

      uint32_t dwTimeProfile1 = get_tick_count();

      UNREFERENCED_PARAMETER(pszVersion);

      string strServer = pszHost;

      string strProtocol = pszProtocol;

      sp(::aura::application) papp = set["app"].cast < application >();

      int32_t iPort;

      if(strProtocol == "https")
      {

         iPort = 443;

      }
      else
      {

         iPort = 80;

      }

      if(pszVersion == NULL || pszVersion[0] == '\0')
      {

         pszVersion = "HTTP/1.1";

      }

      string strUrl(strProtocol + "://" + strServer);

      // Format of script name example "system://server.com/the rain.mp3" => "system://server.com/the%20rain.mp3"
      {

         string strScript = System.url().url_encode(System.url().url_decode(System.url().get_script(strUrl)));

         strScript.replace("+", "%20");

         strScript.replace("%2F", "/");

         strUrl = System.url().set_script(strUrl, strScript);

      }

      property_set setQuery(get_app());

      setQuery.parse_url_query(System.url().get_query(strUrl));

      string strIp;

      string strSessId;

      psession = canew(::sockets::http_session(handler, strProtocol, pszHost));

      if(!(bool)set["disable_ca2_sessid"] && !setQuery.has_property("authnone"))
      {

         if((bool)set["optional_ca2_sessid"])
         {

            if(papp != NULL)
            {

               string strFontopusServer = Session.fontopus()->get_server(strUrl, 8);

               url_domain domainFontopus;

               domainFontopus.create(strFontopusServer);

               if(domainFontopus.m_strRadix == "ca2")
               {

                  puser = &AppUser(papp);

                  if(puser != NULL && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
                        if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
                  {

                     System.url().string_set(strUrl, "sessid", strSessId);

                  }

               }

            }

         }

         if(puser != NULL && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
               if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
         {

            System.url().string_set(strUrl, "sessid", strSessId);

            string strFontopus = Session.fontopus()->get_server(string("http://") + pszHost, 8);

            ::net::address ad(strFontopus);

            strIp = ad.get_display_number();;

            psession->inheader(__id(host)) = pszHost;

         }
         else if(if_then(set.has_property("optional_ca2_login"), (bool)set["optional_ca2_login"]))
         {

         }
         else
         {

            System.url().string_set(strUrl, "authnone", 1);

         }

      }

      if(strProtocol == "https")
      {

         psession->EnableSSL();

      }

      uint32_t dw1 = ::get_tick_count();

      bool bConfigProxy = !set.has_property("no_proxy_config") || !(bool)set["no_proxy_config"];

      if (strIp.has_char())
      {

         psession->m_strConnectHost = strIp;

      }

      uint32_t dwTimeProfileMid = get_tick_count();

      TRACE("just before open open http_session ::http::system::open %d, %d, %d",dwTimeProfile1,dwTimeProfileMid, dwTimeProfileMid - dwTimeProfile1);

      if(!psession->open(bConfigProxy))
      {

         uint32_t dwTimeProfile2 = get_tick_count();

         TRACE("Not Opened/Connected Result Total time ::http::system::get(\"%s\")", strUrl.Left(MIN(255,strUrl.get_length())), dwTimeProfile2 - dwTimeProfile1);

         return NULL;

      }

      uint32_t dw2 = ::get_tick_count();

      TRACE("system::get open time %d\n", dw2 - dw1);

      return true;

   }











































   bool system::request(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession, const char * pszRequest, property_set & set)
   {

      TRACE("http system request : %s",pszRequest);

      uint32_t dw1;
      uint32_t dw2;
      bool bSeemsOk;

      int32_t iTry = 0;

retry:

      iTry++;

      bSeemsOk = true;

      try
      {

         if (psession == NULL)
         {

            bSeemsOk = false;

         }
         else if(!psession->is_valid())
         {

            handler.remove(psession);

            bSeemsOk = false;

         }

      }
      catch(...)
      {

         bSeemsOk = false;

      }

      if(!bSeemsOk)
      {
         try
         {

            DWORD dwBeg = ::get_tick_count();

            if (!open(handler, psession, System.url().get_server(pszRequest), System.url().get_protocol(pszRequest), set, set["user"].cast < ::fontopus::user >(), set["http_protocol_version"]))
            {

               return false;

            }

            DWORD dwEnd = ::get_tick_count();

            TRACE("opening system::request time(%d) = %d, %d, %d\n",0,dwEnd,dwBeg,dwEnd - dwBeg);

         }
         catch(...)
         {
            return NULL;
         }
      }

      DWORD dwBegA = ::get_tick_count();

      try
      {

         uint32_t dwTimeProfile1 = get_tick_count();

         ::aura::application * papp = psession->get_app();

         string strRequest = System.url().get_object(pszRequest);

         string strServer = System.url().get_server(pszRequest);

         string strUrl = psession->m_strProtocol + "://" + strServer + strRequest;

         // Format of script name example "system://server.com/the rain.mp3" => "system://server.com/the%20rain.mp3"
         {
            string strScript = System.url().url_encode(System.url().url_decode(System.url().get_script(strUrl)));
            strScript.replace("+", "%20");
            strScript.replace("%2F", "/");
            strUrl = System.url().set_script(strUrl, strScript);
         }



         property_set setQuery(get_app());

         setQuery.parse_url_query(System.url().get_query(strUrl));



         string strSessId;
         if(!(bool)set["disable_ca2_sessid"] && !setQuery.has_property("authnone"))
         {
            if((bool)set["optional_ca2_sessid"])
            {


               if(papp != NULL)
               {

                  string strFontopusServer = Session.fontopus()->get_server(strUrl, 8);

                  url_domain domainFontopus;

                  domainFontopus.create(strFontopusServer);

                  if(domainFontopus.m_strRadix == "ca2")
                  {
                     set["user"] = &AppUser(papp);
                     if (set["user"].cast < ::fontopus::user >() != NULL && (strSessId = set["user"].cast < ::fontopus::user >()->get_sessid(strUrl, !set["interactive_user"].is_new() &&
                           (bool)set["interactive_user"])).has_char() &&
                           if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
                     {
                        System.url().string_set(strUrl, "sessid", strSessId);
                     }
                  }

               }

            }
            if (set["user"].cast < ::fontopus::user >() != NULL &&
                  (strSessId = set["user"].cast < ::fontopus::user >()->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
                  if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
            {
               System.url().string_set(strUrl, "sessid", strSessId);
            }
            else if(if_then(set.has_property("optional_ca2_login"), (bool)set["optional_ca2_login"]))
            {
            }
            else
            {
               System.url().string_set(strUrl, "authnone", 1);
            }
         }

         strRequest = System.url().get_object(strUrl);

         psession->inheaders().clear();
         psession->outheaders().clear();
         psession->inattrs().clear();
         psession->outattrs().clear();
         psession->m_memoryfile.Truncate(0);


         psession->inheaders() = set["headers"].propset();

         if(set.has_property("minimal_headers") && (bool)set["minimal_headers"])
         {
            psession->m_request.attrs()["minimal_headers"] = true;
         }
         if(set.has_property("file"))
         {
            psession->m_pfile = set["file"].cast < ::file::file >();
         }
         if(set.has_property("int_scalar_source_listener"))
         {
            psession->::int_scalar_source::m_plistener = set["int_scalar_source_listener"].cast < int_scalar_source::listener >();
         }
         else
         {
            psession->::int_scalar_source::m_plistener = NULL;
         }
         if (set["cookies"].cast < ::http::cookies >() != NULL && set["cookies"].cast < ::http::cookies >()->get_size() > 0)
         {
            psession->request().header(__id(cookie)) = set["cookies"].cast < ::http::cookies >()->get_cookie_header();
         }
         if (set["user"].cast < ::fontopus::user >() != NULL && set["user"].cast < ::fontopus::user >()->m_phttpcookies != NULL && !(bool)set["disable_ca2_user_cookies"])
         {
            psession->request().header(__id(cookie)) = set["user"].cast < ::fontopus::user >()->m_phttpcookies->get_cookie_header();
         }
         if(set.has_property(__id(cookie)) && set[__id(cookie)].get_string().has_char())
         {
            psession->request().header(__id(cookie)) = set[__id(cookie)];
         }

         psession->m_host  =System.url().get_server(pszRequest);
         psession->m_strHost  =System.url().get_server(pszRequest);
         psession->m_request.m_propertysetHeader[__id(host)] = psession->m_host;

         set["http_body_size_downloaded"] = &psession->m_body_size_downloaded;

         psession->m_scalarsourceDownloadedRate.m_plistener = set["http_downloaded_rate_listener"].cast < ::double_scalar_source::listener >();

         psession->m_scalarsourceDownloadedRate.m_id = set["http_downloaded_rate_id"].get_id();

         psession->m_scalarsourceDownloaded.m_plistener = set["http_downloaded_listener"].cast < ::int_scalar_source::listener >();

         psession->m_scalarsourceDownloaded.m_id = set["http_downloaded_id"].get_id();

         bool bPost;
         bool bPut;
         if (set["put"].cast < ::file::binary_file >() != NULL || set.lookup(__id(http_method)) == "PUT")
         {
            bPost = false;
            bPut = true;
            psession.cast < ::sockets::http_put_socket>()->m_file = set["put"].cast < ::file::binary_file >();
            psession->request("PUT",strRequest);
         }
         else if (set["post"].propset().get_count() > 0 || set.lookup(__id(http_method)) == "POST")
         {
            bPost = true;
            bPut = false;
            psession.cast < ::sockets::http_post_socket>()->m_fields = set["post"].propset();
            if (set.has_property("multipart"))
            {
               psession.cast < ::sockets::http_post_socket>()->m_pmultipart = set["multipart"].cast < ::sockets::multipart > ();
            }
            psession->request("POST",strRequest);
         }
         else
         {
            bPost = false;
            bPut = false;
            psession->request("GET", strRequest);
         }


         handler.add(psession);

         int32_t iIteration = 0;
         ::aura::live_signal keeplive;

         if(papp != NULL)
         {
            keeplive.keep(papp);
            keeplive.keep(&Sess(papp));
            keeplive.keep(&Sys(papp));
         }

         DWORD dwEndA = ::get_tick_count();

         TRACE("opening preparation system::request time(%d) = %d, %d, %d\n",0,dwEndA,dwBegA,dwEndA - dwBegA);


         oprop("dw").get_value().set_type(var::type_uint32);
         dw1 = oprop("dw");
         dw2 = ::get_tick_count();

         TRACE("Higher Level Diagnosis : iNTERTIMe system::request time(%d) = %d, %d, %d\n", iIteration, dw1, dw2, dw2 - dw1);

         while((handler.get_count() > 0 && !psession->m_bRequestComplete) && (::get_thread() == NULL || ::get_thread_run()))
            //while(psession->m_phandler->get_count() > 0 && !psession->m_bRequestComplete) // should only exit in case of process exit signal
         {

            dw1 = ::get_tick_count();

            handler.select(240, 0);

            keeplive.keep_alive();

            set["http_content_length"] = psession->m_content_length;

            double dRateDownloaded = 0.0;

            int64_t iContentLength = set["http_content_length"].i64();

            int64_t iBodySizeDownloaded = set["http_body_size_downloaded"].i64();

            if (iContentLength > 0)
            {

               if (iBodySizeDownloaded > 0.0)
               {

                  dRateDownloaded = (double) iBodySizeDownloaded / (double) iContentLength;

               }

            }

            set["http_rate_downloaded"] = dRateDownloaded;

            psession->m_scalarsourceDownloadedRate.set_scalar(::scalar_none, dRateDownloaded);

            psession->m_scalarsourceDownloaded.set_scalar(::scalar_none, iBodySizeDownloaded);

            if(psession->m_estatus == ::sockets::socket::status_connection_timed_out
                  || psession->m_estatus == ::sockets::socket::status_timed_out)
            {

               break;

            }
//            if(set["file_out"].cast < ::file::timeout_file >() != NULL)
//            {
//               if(psession->m_iFinalSize != -1 && set["file_out"].cast < ::file::timeout_file >()->m_uiExpectedSize != psession->m_iFinalSize)
//               {
//                  set["file_out"].cast < ::file::timeout_file >()->m_uiExpectedSize = psession->m_iFinalSize;
//               }
//            }
            dw2 = ::get_tick_count();
            TRACE("system::request time(%d) = %d, %d, %d\n", iIteration, dw1, dw2, dw2 - dw1);
            iIteration++;
         }


//         if(set["file_out"].cast < ::file::timeout_file >() != NULL)
//         {
//            if(psession->m_iFinalSize != -1 && set["file_out"].cast < ::file::timeout_file >()->m_uiExpectedSize != psession->m_iFinalSize)
//            {
//               set["file_out"].cast < ::file::timeout_file >()->m_uiExpectedSize = psession->m_iFinalSize;
//            }
//         }

         keeplive.keep_alive();

         oprop("dw") = ::get_tick_count();



         set["get_headers"] = psession->outheaders();
         set["get_attrs"] = psession->outattrs();

//            string strSessId;

//#ifdef BSD_STYLE_SOCKETS
//
//         if(psession->IsSSL())
//         {
//            strSessId = psession->m_response.m_cookies["sessid"];
//            if(strSessId.has_char())
//            {
//               Session.sockets().m_clientcontextmap[System.url().get_server(strUrl) + "?sessid=" + strSessId] = psession->m_spsslclientcontext;
//            }
//         }
//
//#endif

         string strCookie = psession->response().cookies().get_cookie_header();
         set[__id(cookie)] = strCookie;

         int32_t iStatusCode = psession->outattr("http_status_code");

         if(iStatusCode == 0)
         {

#if defined(BSD_STYLE_SOCKETS)

            if(psession->m_psslcontext->m_ssl_ctx != NULL && psession->m_psslcontext->m_iSslCtxRetry == 1 && iTry < 8)
            {

               goto retry;

            }

#endif

         }
         else if(iStatusCode != 200)
         {

            if(iTry < 8)
            {

               psession = NULL;

               goto retry;

            }

         }

         e_status estatus = status_ok;

         if (psession->m_estatus == ::sockets::socket::status_connection_timed_out)
         {
            estatus = status_connection_timed_out;
         }
         else if (psession->m_estatus == ::sockets::socket::status_timed_out)
         {
            estatus = status_timed_out;
         }
         else if((iStatusCode >= 200 && iStatusCode <= 299) || (psession != NULL && psession->outattr("http_status_code").is_empty()))
         {
            estatus = status_ok;
         }
         else if(iStatusCode >= 300 && iStatusCode <= 399)
         {
            if(psession != NULL)
            {

               string strCa2Realm = psession->outheader("ca2realm-x");

               if(::str::begins_ci(strCa2Realm,"not licensed: "))
               {

                  uint32_t dwTimeProfile2 = get_tick_count();

                  TRACE("Not Licensed Result Total time ::http::system::get(\"%s\") %d", strUrl.Left(MIN(255,strUrl.get_length())), dwTimeProfile2 - dwTimeProfile1);

                  string strLocation = psession->outheader("Location");

                  psession.release();

                  _throw(not_licensed(get_app(),strCa2Realm,strLocation));

                  return NULL;

               }

            }

         }
         else
         {

            estatus = status_fail;

         }

         set["get_status"] = (int64_t)estatus;

         uint32_t dwTimeProfile2 = get_tick_count();

         TRACE("Total time ::http::system::get(\"%s\") %d", strUrl.Left(MIN(255,strUrl.get_length())), dwTimeProfile2 - dwTimeProfile1);

      }
      catch(...)
      {
         if(iTry > 8)
            return NULL;
         goto retry;
      }

      if(psession != NULL)
      {

         if(set.has_property("get_response"))
         {

            set["get_response"] = string((const char *)psession->GetDataPtr(),psession->GetContentLength());

         }

         if(set.has_property("get_memory"))
         {

            ::primitive::memory_base * paxis = set.cast < ::primitive::memory_base >("get_memory");

            if(paxis != NULL)
            {

               paxis->assign(memory(psession->GetDataPtr(),psession->GetContentLength()));

            }
            else
            {

               set["get_memory"] = canew(memory(psession->GetDataPtr(),psession->GetContentLength()));

            }

         }

      }


      return true;

   }









































































































   bool system::get(::http::session & session, const char * pszUrl, string & str, property_set & set)
   {

      bool bOk = get(session.m_handler, session.m_psocket, pszUrl, set);

      if (bOk)
      {

         const char * pszData = (const char *)session.m_psocket->GetDataPtr();

         strsize iSize = session.m_psocket->GetContentLength();

         str = string(pszData, iSize);

      }

      return bOk;

   }

   string system::get(::http::session & session, const char * pszUrl, property_set & set)
   {

      string str;

      if (!get(session, pszUrl, str, set))
      {

         return "";

      }

      return str;

   }


   bool system::get(::sockets::socket_handler & handler, sp(::sockets::http_client_socket) & psocket, const char * pszUrl, property_set & set)
   {

      TRACE("http system get : %s", pszUrl);
      DWORD dwStart = get_tick_count();
      int iTry = 0;
#ifdef BSD_STYLE_SOCKETS
retry:
#endif



      uint32_t dwTimeProfile1 = get_tick_count();

      string strServer = System.url().get_root(pszUrl);
      string strProtocol = System.url().get_protocol(pszUrl);
      string strObject = System.url().get_object(pszUrl);
      sp(::aura::application) papp = set["app"].cast < ::aura::application >();
      sp(::aura::application) pappAgent = papp;
      int32_t iPort;
      if(strProtocol == "https")
      {
         iPort = 443;
      }
      else
      {
         iPort = 80;
      }


      string strVersion = set["http_protocol_version"];
      if (strVersion.is_empty())
      {
         strVersion = "HTTP/1.1";
      }


      string strUrl(pszUrl);
      /*      if (Session.fontopus()->m_strBestApiServer.has_char() && strUrl.find_ci("://api.ca2.cc/") > 0)
            {

               strUrl.replace("://api.ca2.cc/", "://" + Session.fontopus()->m_strBestApiServer + "/");

            }*/

      single_lock slFontopus(Session.fontopus() ? Session.fontopus()->m_pmutex : NULL);

      string strIp;

      string strSessId;

      int iRetrySession = 0;

retry_session:

      {

         property_set setQuery(get_app());

         setQuery.parse_url_query(System.url().get_query(strUrl));

         if ((bool)set["raw_http"])
         {

         }
         else
         {

            if (!(bool)set["disable_ca2_sessid"] && !setQuery.has_property("authnone"))
            {

               if ((bool)set["optional_ca2_sessid"])
               {

                  if (papp != NULL)
                  {

                     string strFontopusServer = Session.fontopus()->get_server(strUrl, 8);

                     url_domain domainFontopus;

                     domainFontopus.create(strFontopusServer);

                     if (domainFontopus.m_strRadix == "ca2")
                     {

                        set["user"] = &AppUser(papp);

                        if (set["user"].cast < ::fontopus::user >() != NULL && (strSessId = set["user"].cast < ::fontopus::user >()->get_sessid(strUrl, !set["interactive_user"].is_new()
                              && (bool)set["interactive_user"])).has_char() &&
                              if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
                        {

                           System.url().string_set(strUrl, "sessid", strSessId);

                        }

                     }

                  }

               }
               else if (set["user"].cast < ::fontopus::user >() != NULL &&
                        (strSessId = set["user"].cast < ::fontopus::user >()->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
                        if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
               {

                  System.url().string_set(strUrl, "sessid", strSessId);

                  string strHost;

                  string strFontopus = Session.fontopus()->get_server(pszUrl, 8);

                  ::net::address ad1(strFontopus);

                  strIp = ad1.get_display_number();

                  string str1 = strFontopus;

                  slFontopus.lock();

                  if (::str::ends_eat_ci(str1, "-account.ca2.cc") && strIp.has_char())
                  {

                     str1 = str1 + ".account.ca2.cc";

                     ::net::address ad1(str1);

                     string strIp2 = ad1.get_display_number();

                     Session.fontopus()->m_strBestFontopusServerIp = strIp2;

                  }

                  strHost = System.url().get_server(pszUrl);

                  if (strHost.has_char())
                  {

                     if (strUrl.find_ci("://api.ca2.cc/") > 0)
                     {

                        if (::str::ends_eat_ci(strFontopus, "-account.ca2.cc"))
                        {

                           strHost = strFontopus + "-api.ca2.cc";

                           strUrl.replace("://api.ca2.cc/", "://" + strHost + "/");

                           Session.fontopus()->m_strBestApiServer = strHost;

                        }

                     }

                     set["headers"]["host"] = strHost;

                  }

                  slFontopus.unlock();

               }
               else if (if_then(set.has_property("optional_ca2_login"), (bool)set["optional_ca2_login"]))
               {

               }
               else
               {

                  System.url().string_set(strUrl, "authnone", 1);

               }

            }
            else
            {

               System.url().string_set(strUrl, "authnone", 1);

            }

         }

      }

      // Format of script name example "system://server.com/the rain.mp3" => "system://server.com/the%20rain.mp3"
      {

         string strScript = System.url().url_encode(System.url().url_decode(System.url().get_script(strUrl)));

         strScript.replace("+", "%20");

         strScript.replace("%2F", "/");

         strUrl = System.url().set_script(strUrl, strScript);

      }

      slFontopus.lock();

      if (strUrl.find_ci("://account.ca2.cc/") > 0 && Session.fontopus()->m_strBestFontopusServerIp.has_char())
      {

         strIp = Session.fontopus()->m_strBestFontopusServerIp;

      }

      slFontopus.unlock();

      bool bPost;

      bool bPut;

      if (set["put"].cast < ::file::file >() != NULL || set.lookup(__id(http_method)) == "PUT")
      {
         bPost = false;
         bPut = true;
         psocket = canew(::sockets::http_put_socket(handler, strUrl));
         dynamic_cast < ::sockets::http_put_socket * > (psocket.m_p)->m_file = set["put"].cast < ::file::file >();
         psocket->m_emethod = ::sockets::http_method_put;
      }
      else if (set["post"].propset().get_count() > 0)
      {
         bPost = true;
         bPut = false;
         psocket = canew(::sockets::http_post_socket(handler, strUrl));
         dynamic_cast < ::sockets::http_post_socket * > (psocket.m_p)->m_fields = set["post"].propset();
         if (set.has_property("multipart"))
         {
            psocket.cast < ::sockets::http_post_socket>()->m_pmultipart = set["multipart"].cast < ::sockets::multipart >();
         }
         psocket->m_emethod = ::sockets::http_method_post;
      }
      else
      {
         bPost = false;
         bPut = false;
         psocket = canew(::http::get_socket(handler, strUrl));
         psocket->m_emethod = ::sockets::string_http_method(set.lookup(__id(http_method), "GET"));
      }

      //psocket->m_output_length = 1; // at least one to trigger write and in case it is not set (in case of reuse currently);

      if (pappAgent.is_set())
      {

         psocket->set_app(pappAgent);

      }


      psocket->m_bEnablePool = handler.PoolEnabled();

      if((bool)set["disable_common_name_cert_check"])
      {
         psocket->enable_cert_common_name_check(false);
      }

      if (strProtocol == "https")
      {
         psocket->m_strTlsHostName = System.url().get_server(strUrl);
      }

      if (set["http_listener"].cast < ::http::listener >() != NULL)
      {
         psocket->::sockets::http_socket::m_plistener = set["http_listener"].cast < ::http::listener >();
      }
      psocket->inheaders().add(set["headers"].propset());
      if (set.has_property("progress_listener"))
      {
         psocket->m_progress.m_plistener = set["progress_listener"].cast < progress_listener >();
      }
      if (set.has_property("int_scalar_source_listener"))
      {
         psocket->::int_scalar_source::m_plistener = set["int_scalar_source_listener"].cast < int_scalar_source::listener >();
      }
      if (set.has_property("minimal_headers") && (bool)set["minimal_headers"])
      {
         psocket->m_request.attrs()["minimal_headers"] = true;
      }
      if(set.has_property("only_headers") && (bool)set["only_headers"])
      {
         psocket->m_bOnlyHeaders = true;
      }
      if(set.has_property("noclose") && !(bool)set["noclose"])
      {
         psocket->m_bNoClose = false;
      }
      if(set.has_property("file"))
      {
         psocket->m_pfile = set["file"].cast < ::file::file >();
      }
      if (set["cookies"].cast < ::http::cookies >() != NULL && set["cookies"].cast < ::http::cookies >()->get_size() > 0)
      {
         if (set["cookies"].cast < ::http::cookies >()->find_cookie("sessid") >= 0)
         {
            set["cookies"].cast < ::http::cookies >()->set_cookie("sessid", strSessId);
         }
         psocket->request().header(__id(cookie)) = set["cookies"].cast < ::http::cookies >()->get_cookie_header();
      }
      if (set["user"].cast < ::fontopus::user >() != NULL && set["user"].cast < ::fontopus::user >()->m_phttpcookies != NULL && !(bool)set["disable_ca2_user_cookies"])
      {
         if (set["user"].cast < ::fontopus::user >()->m_phttpcookies->find_cookie("sessid") >= 0)
         {
            set["user"].cast < ::fontopus::user >()->m_phttpcookies->set_cookie("sessid", strSessId);
         }
         psocket->request().header(__id(cookie)) = set["user"].cast < ::fontopus::user >()->m_phttpcookies->get_cookie_header();
      }
      if(set.has_property(__id(cookie)) && set[__id(cookie)].get_string().has_char())
      {
         psocket->request().header(__id(cookie)) = set[__id(cookie)];
      }
      if(!psocket->m_bNoClose)
      {
         psocket->SetCloseOnComplete();
      }
      if(strProtocol == "https")
      {
#ifdef BSD_STYLE_SOCKETS
//         ::sockets::ssl_client_context * pcontext = set["ssl_client_context"].cast < ::sockets::ssl_client_context > ();
//         if(pcontext != NULL)
//         {
//            psocket->m_spsslclientcontext = pcontext;
//         }
//         else
//         {
//            if(strSessId.has_char() && strSessId != "not_auth")
//            {
//               psocket->m_strInitSSLClientContext = System.url().get_server(strUrl) + "?sessid=" + strSessId;
//            }
//         }
#endif
         psocket->EnableSSL();
      }
      uint32_t dw1 = ::get_tick_count();

      bool bConfigProxy = !set.has_property("no_proxy_config") || !(bool)set["no_proxy_config"];

      int32_t iTimeoutTotalMs = set["timeout"];

      int32_t iTimeout = 0;

      int32_t iTimeoutMs = 0;

      set["http_body_size_downloaded"] = &psocket->m_body_size_downloaded;

      psocket->m_scalarsourceDownloadedRate.m_plistener = set["http_downloaded_rate_listener"].cast < ::double_scalar_source::listener >();

      psocket->m_scalarsourceDownloadedRate.m_id = set["http_downloaded_rate_id"].get_id();

      psocket->m_scalarsourceDownloaded.m_plistener = set["http_downloaded_listener"].cast < ::int_scalar_source::listener >();

      psocket->m_scalarsourceDownloaded.m_id = set["http_downloaded_id"].get_id();

      if (iTimeoutTotalMs == 0)
      {

         iTimeout = 23;
         iTimeoutMs = 0;

      }
      else
      {

         iTimeout = iTimeoutTotalMs / 1000;
         iTimeoutMs = iTimeoutTotalMs % 1000;

      }

      if (strIp.has_char())
      {

         psocket->m_strConnectHost = strIp;

      }

      stringa straProxy;

      if (set.has_property("proxy"))
      {

         straProxy.explode(":", set["proxy"].get_string());

         if (straProxy.get_count() != 2 || !psocket->proxy_open(straProxy[0], atoi(straProxy[1])))
         {

            set["get_status"] = (int64_t)status_failed;

            uint32_t dwTimeProfile2 = get_tick_count();

            TRACE("Not Opened/Connected Result Total time ::http::system::get(\"%s\") %d", strUrl.Left(MIN(255, strUrl.get_length())), dwTimeProfile2 - dwTimeProfile1);

            return NULL;

         }

      }
      else if(!psocket->open(bConfigProxy))
      {

         set["get_status"] = (int64_t) status_failed;

         uint32_t dwTimeProfile2 = get_tick_count();

         TRACE("Not Opened/Connected Result Total time ::http::system::get(\"%s\") %d", strUrl.Left(MIN(255,strUrl.get_length())), dwTimeProfile2 - dwTimeProfile1);

         return NULL;

      }

      uint32_t dw2 = ::get_tick_count();

      TRACE("system::get open time %d\n", dw2 - dw1);

      handler.add(psocket);

      int32_t iIteration = 0;

      ::aura::live_signal keeplive;

      if((bool)set["noloop"])
         return true;

      if(papp != NULL)
      {
         keeplive.keep(papp);
         keeplive.keep(&Sess(papp));
         keeplive.keep(&Sys(papp));
      }

      psocket->m_bEnablePool = handler.PoolEnabled();
      if (handler.PoolEnabled())
      {
         psocket->SetRetain();

      }

//      ::file::timeout_file * ptimeoutbuffer = set["file_out"].cast < ::file::timeout_file >();

      while(handler.get_count() > 0 && (::get_thread() == NULL || ::get_thread_run()))
      {

         dw1 = ::get_tick_count();

         handler.select(iTimeout, iTimeoutMs * 1000);

         set["http_content_length"] = psocket->m_content_length;

         double dRateDownloaded = 0.0;

         int64_t iContentLength = set["http_content_length"].i64();

         int64_t iBodySizeDownloaded = set["http_body_size_downloaded"].i64();

         if (iContentLength > 0)
         {

            if (iBodySizeDownloaded > 0.0)
            {

               dRateDownloaded = (double)iBodySizeDownloaded / (double)iContentLength;

            }

         }

         set["http_rate_downloaded"] = dRateDownloaded;

         psocket->m_scalarsourceDownloadedRate.set_scalar(::scalar_none, dRateDownloaded);

         psocket->m_scalarsourceDownloaded.set_scalar(::scalar_none, iBodySizeDownloaded);

         keeplive.keep_alive();

         if(psocket->m_estatus == ::sockets::socket::status_connection_timed_out
               || psocket->m_estatus == ::sockets::socket::status_timed_out)
         {

            break;

         }

         if (psocket->m_b_complete)
         {

            break;

         }
//         if (ptimeoutbuffer != NULL)
//         {
//            if (psocket->m_iFinalSize != -1 && ptimeoutbuffer->m_uiExpectedSize != psocket->m_iFinalSize)
//            {
//               ptimeoutbuffer->m_uiExpectedSize = psocket->m_iFinalSize;
//            }
//         }
         dw2 = ::get_tick_count();
         TRACE("system::get time(%d) = %d, %d, %d\n", iIteration, dw1, dw2, dw2 - dw1);
         iIteration++;
      }
      keeplive.keep_alive();

      set["get_headers"] = psocket->outheaders();
      set["get_attrs"] = psocket->outattrs();

//#ifdef BSD_STYLE_SOCKETS
//      if(psocket->IsSSL())
//      {
//         strSessId = psocket->m_response.m_cookies["sessid"];
//         if(strSessId.has_char())
//         {
//            Session.sockets().m_clientcontextmap[System.url().get_server(strUrl) + "?sessid=" + strSessId] = psocket->m_spsslclientcontext;
//         }
//      }
//#endif

      string strCookie = psocket->response().cookies().get_cookie_header();
      set[__id(cookie)] = strCookie;

      e_status estatus = status_fail;

      int32_t iStatusCode = psocket->outattr("http_status_code");

      set["http_status_code"] = psocket->outattr("http_status_code");
      iTry++;
#ifdef BSD_STYLE_SOCKETS
      if(iStatusCode == 0)
      {
         DWORD dwDelta = get_tick_count() - dwStart;
         if (iTry <= 1 &&  dwDelta < iTimeoutTotalMs)
         {
            Sleep(300);
            goto retry;
         }
      }
#endif

      if (psocket->m_estatus == ::sockets::socket::status_connection_timed_out)
      {
         estatus = status_connection_timed_out;
      }
      else if (psocket->m_estatus == ::sockets::socket::status_timed_out)
      {
         estatus = status_timed_out;
      }
      else if((iStatusCode >= 200 && iStatusCode <= 299 ) || psocket->outattr("http_status_code").is_empty())
      {
         estatus = status_ok;
      }
      else if(iStatusCode >= 300 && iStatusCode <= 399)
      {
         string strCa2Realm = psocket->outheader("ca2realm-x");
         if(::str::begins_ci(strCa2Realm, "not licensed: "))
         {
            uint32_t dwTimeProfile2 = get_tick_count();
            TRACE("Not Licensed Result Total time ::http::system::get(\"%s\") %d", strUrl.Left(MIN(255,strUrl.get_length())), dwTimeProfile2 - dwTimeProfile1);
            string strLocation = psocket->outheader("Location");
//            delete psocket;
            _throw(not_licensed(get_app(), strCa2Realm, strLocation));
            return NULL;
         }
         string strLocation = psocket->outheader("location");

         if (set.has_property("redirect_location"))
         {

            set["redirect_location"] = strLocation;

            estatus = status_ok;

         }
         else if(strLocation.has_char())
         {
            if(strLocation.find_ci("http://") == 0
                  || strLocation.find_ci("https://") == 0)
            {
               return get(handler,psocket, strLocation,set);
            }
            else
            {
               strLocation = System.url().get_protocol(pszUrl) + System.url().get_server(pszUrl) + System.url().get_object(strLocation);
               return get(handler,psocket,strLocation,set);
            }
         }
      }
      else if (iStatusCode == 401)
      {


         if (strSessId.has_char() && set["user"].cast < ::fontopus::user >() != NULL && iRetrySession < 3)
         {

            slFontopus.lock();

            Session.fontopus()->m_mapFontopusSessId[Session.fontopus()->m_strFirstFontopusServer].Empty();

            Session.fontopus()->m_authmap.remove_key(set["user"].cast < ::fontopus::user >()->m_strLogin);

            set["user"].cast < ::fontopus::user >()->m_sessionidmap[System.url().get_server(pszUrl)].Empty();

            set["user"].cast < ::fontopus::user >()->m_sessionidmap[System.url().get_server(strUrl)].Empty();

            Session.fontopus()->m_mapFontopusServer.remove_key(System.url().get_server(pszUrl));

            Session.fontopus()->m_mapFontopusServer.remove_key(System.url().get_server(strUrl));

            Session.fontopus()->m_strFirstAccountServer.Empty();

            string strFontopusServer = Session.fontopus()->get_server("account.ca2.cc", 8);

            url_domain domainFontopus;

            domainFontopus.create(strFontopusServer);

            if (domainFontopus.m_strRadix == "ca2")
            {

               set["user"] = (::object *) NULL;

               Session.fontopus()->m_puser = NULL;

               // set["user"] = Session.fontopus()->get_user();

            }


            iRetrySession++;

            slFontopus.unlock();

            goto retry_session;

         }

         estatus = status_fail;

      }
      else
      {
         estatus = status_fail;
      }

      set["get_status"] = (int64_t)estatus;

      if (set["http_listener"].cast < ::http::listener >() != NULL)
      {

         set["http_listener"].cast < ::http::listener >()->on_http_complete(psocket, estatus);

      }

      if (set.has_property("get_response"))
      {

         const char * pszData = (const char *)psocket->GetDataPtr();

         strsize iSize = psocket->GetContentLength();

         string strResponse(pszData, iSize);

         set["get_response"] = strResponse;

      }

      if (set.has_property("get_memory"))
      {

         ::primitive::memory_base * paxis = set.cast < ::primitive::memory_base >("get_memory");

         if (paxis != NULL)
         {

            paxis->assign(psocket->GetDataPtr(), psocket->GetContentLength());

         }
         else
         {

            set["get_memory"] = canew(memory(psocket->GetDataPtr(), psocket->GetContentLength()));

         }

      }

//      if (ptimeoutbuffer != NULL && ptimeoutbuffer->m_uiExpectedSize == (uint_ptr)-1)
//      {
//
//         ptimeoutbuffer->m_uiExpectedSize = 0;
//
//      }

      uint32_t dwTimeProfile2 = get_tick_count();
      TRACE("Total time ::http::system::get(\"%s\") %d", strUrl.Left(MIN(255,strUrl.get_length())), dwTimeProfile2 - dwTimeProfile1);

      return true;

   }





















   void system::get(::message::message * pobj)
   {

      SCAST_PTR(message, pmessage, pobj);

      if(pmessage == NULL)
      {
         return;
      }

      ::sockets::socket_handler handler(get_app());

      property_set set;

      set = pmessage->m_set;

      if (pmessage->m_setPost.get_count() > 0)
      {

         set["post"] = pmessage->m_setPost;

      }

      if (pmessage->m_setHeaders.get_count() > 0)
      {

         set["headers"] = pmessage->m_setHeaders;

      }

      if (pmessage->m_pcookies != NULL && pmessage->m_pcookies->get_count() > 0)
      {

         set["cookies"] = pmessage->m_pcookies;

      }

      if (pmessage->m_puser != NULL)
      {

         set["user"] = pmessage->m_puser;

      }

      if (pmessage->m_strVersion.has_char())
      {

         set["http_protocol_version"] = pmessage->m_strVersion;

      }

      sp(::sockets::http_client_socket) psocket;

      if(!get(handler, psocket, pmessage->m_strUrl, set))
      {

         pmessage->m_estatusRet = (::http::e_status) set["get_status"].i64();

         pmessage->m_bRet = false;

         return;

      }

      pmessage->m_estatusRet = (::http::e_status) set["get_status"].i64();


      if(psocket->GetDataPtr() != NULL && psocket->GetContentLength() > 0)
      {

         pmessage->m_memoryRet.set_data((void *) psocket->GetDataPtr(), psocket->GetContentLength());

      }
      else
      {

         pmessage->m_memoryRet.allocate(0);

      }

      pmessage->m_setHeaders = psocket->outheaders();

      int32_t iStatusCode = psocket->outattr("http_status_code");

      pmessage->m_bRet = iStatusCode == 200;

      return;

   }


   bool system::download(sockets::socket_handler & handler, sp(::sockets::http_session) & psession,const char * pszRequest,var varFile,property_set & set)
   {

      ::file::file_sp spfile = set.cast < ::aura::application >("app",get_app())->m_paurasession->file().get_file(varFile,
                               ::file::type_binary | ::file::mode_create | ::file::mode_read_write | ::file::defer_create_directory);

      set["file"] = spfile;

      bool bOk = request(handler, psession, pszRequest,set);

      set["file"].null();

      return bOk;

   }


   bool system::download(const char * pszUrl, var varFile, property_set & set)
   {

      ::sockets::socket_handler handler(get_app());

      sp(::sockets::http_client_socket) psocket;

      bool bOk = false;

      {

         ::file::file_sp spfile = set.cast < ::aura::application >("app", get_app())->m_paurasession->file().get_file(varFile,
                                  ::file::type_binary | ::file::mode_create | ::file::mode_read_write | ::file::defer_create_directory);

         set["file"] = spfile;

         bOk = get(handler, psocket, pszUrl, set);

         set["file"].null();

      }

      if(!bOk)
         return false;


      /*      if(spfile.is_null())
            {
               return false;
            }

            spfile->write(psocket->GetDataPtr(), psocket->GetContentLength());*/

      return true;

   }


   bool system::exists(const char * pszUrl, ::property_set & set)
   {

      single_lock sl(m_pmutexDownload, true);

      while (m_straExists.contains(pszUrl))
      {
         sl.unlock();
         Sleep(100);
         sl.lock();
      }

      m_straExists.add(pszUrl);

      sl.unlock();

      ::sockets::socket_handler handler(get_app());

      set["only_headers"] = true;

      ::url_domain domain;

      domain.create(System.url().get_server(pszUrl));

      if (::str::begins(System.url().get_object(pszUrl), astr.strMatterUri))
      {

         set["raw_http"] = true;

      }

      sp(::sockets::http_client_socket) psocket;

      if (!get(handler, psocket, pszUrl, set))
      {
         sl.lock();
         m_straExists.remove(pszUrl);
         return false;
      }

      int32_t iStatusCode = psocket->outattr("http_status_code");

      sl.lock();
      m_straExists.remove(pszUrl);

      return iStatusCode == 200;
   }


   var system::length(const char * pszUrl, ::property_set & set)
   {

      ::sockets::socket_handler handler(get_app());

      set["only_headers"] = true;
      set["minimal_headers"] = true;

      ::url_domain domain;

      domain.create(System.url().get_server(pszUrl));

      if (::str::begins(System.url().get_object(pszUrl), astr.strMatterUri))
      {

         set["disable_ca2_sessid"] = true;

      }

      sp(::sockets::http_client_socket) psocket;

      if (get(handler, psocket, pszUrl, set))
         return false;

      int32_t iStatusCode = psocket->outattr("http_status_code");

      if (iStatusCode == 200)
      {

         return psocket->outheader(__id(content_length));

      }
      else
      {

         return var(var::type_null);

      }


   }


   string system::gmdate(time_t t)
   {
      if(t == 0)
         t = time(NULL);

      //time_t t = time((time_t *)&iExpire);
      struct tm tp;
#ifdef _WIN32
      memcpy(&tp, gmtime(&t), sizeof(tp));
#else
      gmtime_r(&t, &tp);
#endif
      const char *days[7] = {"Sunday", "Monday",
                             "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
                            };
      const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May",
                                "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                               };
      char strDateTime[1024];

      sprintf(strDateTime, "%s, %02d-%s-%04d %02d:%02d:%02d GMT",
              days[tp.tm_wday],
              tp.tm_mday,
              months[tp.tm_mon],
              tp.tm_year + 1900,
              tp.tm_hour,
              tp.tm_min,
              tp.tm_sec);

      return strDateTime;
   }

   void system::set_proxy_auth(::fontopus::user * puser, ::sockets::http_client_socket * psocket)
   {
      string strUserName;
      string strPassword;
      string strUserNameFile;
      string strPasswordFile;
      string strSection;
      strSection.Format("proxy_auth\\%s.%s", puser->m_strLogin.c_str(), "proxy_auth");
      strUserNameFile = System.dir().appdata() / strSection + "_1";
      strPasswordFile = System.dir().appdata() / strSection + "_2";
      bool bOk = true;
      if(!System.crypto().file_get(strUserNameFile, strUserName, NULL, get_app())
            || strUserName.is_empty())
      {
         bOk = false;
      }
      if(!System.crypto().file_get(strPasswordFile, strPassword, NULL, get_app())
            || strPassword.is_empty())
      {
         bOk = false;
      }
      if(bOk)
      {
         psocket->m_strUserNameFile = strUserNameFile;
         psocket->m_strPasswordFile = strPasswordFile;
      }
      else
      {
         //SPECIFY AUTHENTICATION USERNAME
         if(Session.get_auth("system\\::fontopus::user\\proxy_authenticate.xhtml", strUserName, strPassword))
         {
            System.crypto().file_set(strUserNameFile, strUserName, NULL, get_app());
            System.crypto().file_set(strPasswordFile, strPassword, NULL, get_app());
            psocket->m_strUserNameFile = strUserNameFile;
            psocket->m_strPasswordFile = strPasswordFile;
         }
      }
   }

   void system::clean_proxy_auth(::fontopus::user * puser)
   {
      string strSection;
      strSection.Format("proxy_auth\\%s.%s", puser->m_strLogin.c_str(), "proxy_auth");
      Application.file().del(System.dir().appdata() / strSection + "_1");
      Application.file().del(System.dir().appdata() / strSection + "_2");
   }


   bool system::put(const char * pszUrl, primitive::memory_base & memory, property_set & set)
   {

      ::memory_file file(get_app(), &memory);

      return put(pszUrl, &file, set);

   }


   bool system::put(const char * pszUrl, ::file::file_sp  pfile, property_set & set)
   {

      set["put"] = pfile;

      set["noclose"] = false;

      return System.http().get(pszUrl, set);

   }

   bool system::get(const char * pszUrl, property_set & set)
   {

      sockets::socket_handler sockethandler(get_app());

      //bool bMember;

      //if (::str::ends_ci(System.url().get_server(pszUrl), "ca2.cc"))
      //{

      //   p = m_phandler;

      //   bMember = true;

      //}
      //else
      //{

      //   p = new ::sockets::socket_handler(get_app());

      //   bMember = false;

      //}


      sp(::sockets::http_client_socket) psocket;

      if (!System.http().get(sockethandler, psocket, pszUrl, set))
      {

         return false;

      }

      //if (!bMember)
      //{

      //   ::aura::del(p);

      //}

      return status_succeeded(set["get_status"]);

   }

   bool system::request(const char * pszMethod, const char * pszUrl, property_set & set)
   {

      set[__id(http_method)] = pszMethod;

      return System.http().get(pszUrl, set);

   }

} // namespace http

