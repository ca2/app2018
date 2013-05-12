#include "framework.h"


namespace ca
{


   namespace http
   {


      system::system(sp(::ca::application) papp) :
         ca(papp),
         m_mutexPac(papp),
         m_mutexProxy(papp)
      {
      }

      system::~system()
      {
      }

      int32_t system::auto_config_proxy_count()
      {
         return 4;
      }

      void system::auto_config_proxy(int32_t i)
      {

         return;
         if(i == 0)
         {
         }
         else if(i == 1)
         {
            // telmico: no proxy
            string str = System.file_as_string(&System, System.dir().appdata("machine/proxy.xml"));
            if(str.has_char() && str.find("<") >= 0 && str.find(">") > 0)
            {
               Application.file().copy(System.dir().appdata("proxy_original.xml"), System.dir().ca2("proxy.xml"), false);
            }
            if(Application.file().exists(System.dir().appdata("proxy.xml")))
            {
               try
               {
                  System.file().del(System.dir().appdata("proxy.xml"));
               }
               catch(...)
               {
               }
            }
         }
         else if(i == 2)
         {
            // telmico: original proxy configuration
            if(Application.file().exists(System.dir().appdata("proxy_original.xml")))
            {
               Application.file().copy(System.dir().appdata("proxy.xml"), System.dir().appdata("proxy_original.xml"), false);
            }
         }
         else
         {
            // telmico: simple default proxy configuration : hostname=>proxy - try etc/hosts port=>80  - assume HTTP proxy
            string str = System.file_as_string(&System, System.dir().appdata("proxy.xml"));
            if(str.has_char() && str.find("<") >= 0 && str.find(">") > 0)
            {
               Application.file().copy(System.dir().appdata("proxy_original.xml"), System.dir().appdata("proxy.xml"), false);
            }
            Application.file().put_contents(System.dir().appdata("proxy.xml"), "proxy");
         }
      }

      void system::defer_auto_initialize_proxy_configuration()
      {
         /*
         if(!strcmp(System.get_module_name(), "netnodeapp")
            || !strcmp(System.get_module_name(), "netnodecfgapp")
            || System.get_license_id() == "simpledbcfg"
            || System.get_license_id() == "core_netnodelite"
            || System.command()->m_varTopicQuery["app"] == "simpledbcfg")
         {
            return;
         }
         else if(System.command()->m_varTopicQuery["app"] == "mydns"
            || System.command()->m_varTopicQuery.has_property("install")
            || System.command()->m_varTopicQuery.has_property("uninstall"))
         {
            return;
         }
         else if(!strcmp(System.get_module_name(), "productionapp")
            || !strcmp(System.get_module_name(), "productionbasisapp")
            || !strcmp(System.get_module_name(), "backupapp")
            || System.command()->m_varTopicQuery["app"] == "production"
            || System.get_license_id() == "winservice_filesystemsize")
         {
            return;
         }
         */


         string strHost = Application.file().as_string(System.dir().appdata("database\\text\\last_good_known_fontopus_com.txt"));
         stringa straRequestingServer;
         straRequestingServer.add("account.ca2.cc");
         straRequestingServer.add("eu-account.ca2.cc");
         straRequestingServer.add("asia-account.ca2.cc");
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

      system::pac::pac(sp(::ca::application) papp) :
         ca(papp),
         m_js(papp)
      {
      }

      system::pac * system::get_pac(const char * pszUrl)
      {

         single_lock sl(&m_mutexPac, true);

         string_map < pac * >::pair * ppair = m_mapPac.PLookup(pszUrl);

         if(ppair == ::null() || (::get_tick_count() - ppair->m_element2->m_dwLastChecked) > (84 * 1000))
         {
            if(ppair != ::null())
            {
               delete ppair->m_element2;
               m_mapPac.remove_key(pszUrl);
            }

            class pac * ppac = new class pac(get_app());

            ppac->m_dwLastChecked = get_tick_count();

            ppac->m_strUrl = pszUrl;

            var varQuery;

            varQuery["disable_ca2_sessid"] = true;
            varQuery["no_proxy_config"] = true;

            ppac->m_strAutoConfigScript = Application.file().as_string(ppac->m_strUrl, varQuery);


            m_mapPac.set_at(pszUrl, ppac);

            if(ppac->m_strAutoConfigScript.is_empty())
            {
               return ::null();
            }

            registerFunctions(&ppac->m_js);
            registerJavascriptFunctions(&ppac->m_js);
            ppac->m_js.execute(ppac->m_strAutoConfigScript);

            ppair = m_mapPac.PLookup(pszUrl);

            if(ppair == ::null())
               return ::null();
         }

         if(ppair->m_element2->m_strAutoConfigScript.is_empty())
            return ::null();

         return ppair->m_element2;

      }


      system::proxy::proxy(sp(::ca::application) papp) :
         ca(papp)
      {
      }



      ::ca::http::system::proxy * system::get_proxy(const char * pszUrl)
      {

         single_lock sl(&m_mutexProxy, true);

         string_map < ::ca::http::system::proxy * >::pair * ppair = m_mapProxy.PLookup(pszUrl);

         if(ppair == ::null() || (::get_tick_count() - ppair->m_element2->m_dwLastChecked) > (84 * 1000))
         {
            if(ppair != ::null())
            {
               delete ppair->m_element2;
               m_mapPac.remove_key(pszUrl);
            }

            class ::ca::http::system::proxy * pproxy = new class ::ca::http::system::proxy(get_app());

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

         single_lock sl(&m_mutexPac, true);

         string strProxyServer;

         string strUrl(pszScriptUrl);

         if(::ca::str::begins(pszUrl, strUrl))
         {
            pproxy->m_bDirect = true;
            return true;
         }

         class pac * ppac = get_pac(pszScriptUrl);

         if(ppac == ::null())
            return false;

         string strHost;

         strHost = System.url().get_server(pszUrl);
         int32_t port = System.url().get_port(pszUrl);

/*         ipaddr_t l;
         if (!System.net().u2ip(strHost,l))
         {
            return false;
         }*/
         ::sockets::address ad(get_app(), strHost, port);

         strHost = ad.get_display_number();

         string var;
         try
         {
            var = ppac->m_js.evaluate("FindProxyForURL('" + string(pszUrl) + "', '" +strHost +"');");
         }
         catch(...)
         {
            return false;
         }

         if(var.CompareNoCase("DIRECT") == 0)
         {
            pproxy->m_bDirect = true;
         }
         else if(::ca::str::begins_eat_ci(var, "PROXY"))
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

         ::ca::http::system::proxy * pproxy = get_proxy(pszUrl);

         if(pproxy == ::null())
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


      void system::config_proxy(const char * pszUrl, ::ca::http::system::proxy * pproxy)
      {

         xml::document doc(get_app());
         string str = System.file_as_string(&System, System.dir().appdata("proxy.xml"));
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
         bool bOk = false;
         if(!doc.load(str))
         {
            pproxy->m_bDirect = true;
         }
         else
         {
            bOk = true;
            string strHost = System.url().get_server(pszUrl);
            int32_t iHostPort = System.url().get_port(pszUrl);
            ::sockets::address ipHost(get_app(), strHost, iHostPort);
            for(int32_t iNode = 0; iNode < doc.get_root()->get_children_count(); iNode++)
            {
               sp(::xml::node) pnode = doc.get_root()->child_at(iNode);
               if(pnode->get_name() == "proxy")
               {
                  ::sockets::address ipAddress(get_app(), pnode->attr("address"), 0);
                  ::sockets::address ipMask(get_app(), pnode->attr("mask"), 0);
                  if(ipHost.is_in_net(ipAddress, ipMask))
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
         }

         if(!bOk)
         {

            bool bAutoDetect = System.os().connection_settings_get_auto_detect();

            if(bAutoDetect)
            {
               if(try_pac_script("http://wpad/wpad.dat", pszUrl, pproxy))
                  return;
            }

            string strUrl = System.os().connection_settings_get_auto_config_url();

            if(strUrl.has_char())
            {
               if(try_pac_script(strUrl, pszUrl, pproxy))
                  return;
            }

            pproxy->m_bDirect = true;
         }

      }

















      ::sockets::http_session * system::open(
                     ::sockets::socket_handler & handler,
                     const char * pszHost,
                     const char * pszProtocol,
                     ::ca::property_set & set,
                     ::fontopus::user * puser,
                     const char * pszVersion)
      {

         uint32_t dwTimeProfile1 = get_tick_count();

         UNREFERENCED_PARAMETER(pszVersion);
         string strServer = pszHost;
         string strProtocol = pszProtocol;
         sp(::ca::application) papp = set["app"].ca < ::ca::application >();
         int32_t iPort;
         if(strProtocol == "https")
         {
            iPort = 443;
         }
         else
         {
            iPort = 80;
         }

         if(pszVersion == ::null() || pszVersion[0] == '\0')
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

         ::ca::property_set setQuery(get_app());

         setQuery.parse_url_query(System.url().get_query(strUrl));



         string strSessId;
         if(!(bool)set["disable_ca2_sessid"] && !setQuery.has_property("authnone"))
         {
            if((bool)set["optional_ca2_sessid"])
            {


               if(papp != ::null())
               {

                  string strFontopusServer = Sys(papp).get_fontopus_server(strUrl, papp);

                  url_domain domainFontopus;

                  domainFontopus.create(strFontopusServer);

                  if(domainFontopus.m_strRadix == "ca2")
                  {
                     puser = &AppUser(papp);
                     if(puser != ::null() && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
                        if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
                     {
                        System.url().string_set(strUrl, "sessid", strSessId);
                     }
                  }

               }

            }
            if(puser != ::null() && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
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

         ::sockets::http_session * psession;


         psession = new ::sockets::http_session(handler, strProtocol, pszHost);

         if(strProtocol == "https")
         {
#ifndef METROWIN
            ::sockets::ssl_client_context * pcontext = set["ssl_client_context"].ca < ::sockets::ssl_client_context > ();
            if(pcontext != ::null())
            {
               psession->m_spsslclientcontext = pcontext;
            }
            else
            {
               if(strSessId.has_char() && strSessId != "not_auth")
               {
                  psession->m_strInitSSLClientContext = System.url().get_server(strUrl) + "?sessid=" + strSessId;
               }
            }
#endif
            psession->EnableSSL();
         }
         uint32_t dw1 = ::get_tick_count();
         bool bConfigProxy = !set.has_property("no_proxy_config") || !(bool)set["no_proxy_config"];
         if(!psession->open(bConfigProxy))
         {
/*            if(pestatus != ::null())
            {
               *pestatus = status_failed;
            }*/
            delete psession;
            uint32_t dwTimeProfile2 = get_tick_count();
            TRACE0("Not Opened/Connected Result Total time ::ca::http::system::get(\"" + strUrl.Left(min(255,strUrl.get_length())) + "\")  " + ::ca::str::from(dwTimeProfile2 - dwTimeProfile1));
            return ::null();
         }
         uint32_t dw2 = ::get_tick_count();
         TRACE("system::get open time %d\n", dw2 - dw1);

         return psession;


      }











































      ::sockets::http_session * system::request(
                     ::sockets::socket_handler & handler,
                     ::sockets::http_session * psession,
                     const char * pszRequest,
                     ::ca::property_set & post,
                     ::ca::property_set & headers,
                     ::ca::property_set & set,
                     ::http::cookies * pcookies,
                     ::fontopus::user * puser,
                     const char * pszVersion,
                     e_status * pestatus)
      {

         uint32_t dw1;
         uint32_t dw2;
         bool bSeemsOk;

         int32_t iTry = 0;

retry:

         iTry++;

         bSeemsOk = true;

         try
         {

            if(psession == ::null() || !psession->is_valid())
            {

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
               psession = open(handler, System.url().get_server(pszRequest), System.url().get_protocol(pszRequest),
                  set, puser, pszVersion);
               if(psession == ::null())
                  return ::null();
            }
            catch(...)
            {
               return ::null();
            }
         }


         try
         {

            uint32_t dwTimeProfile1 = get_tick_count();

            sp(::ca::application) papp = handler.get_app();


            string strRequest = System.url().get_object(pszRequest);





            string strUrl = psession->m_strProtocol + "://" + psession->m_strHost + strRequest;



         // Format of script name example "system://server.com/the rain.mp3" => "system://server.com/the%20rain.mp3"
         {
            string strScript = System.url().url_encode(System.url().url_decode(System.url().get_script(strUrl)));
            strScript.replace("+", "%20");
            strScript.replace("%2F", "/");
            strUrl = System.url().set_script(strUrl, strScript);
         }



         ::ca::property_set setQuery(get_app());

         setQuery.parse_url_query(System.url().get_query(strUrl));



         string strSessId;
         if(!(bool)set["disable_ca2_sessid"] && !setQuery.has_property("authnone"))
         {
            if((bool)set["optional_ca2_sessid"])
            {


               if(papp != ::null())
               {

                  string strFontopusServer = Sys(papp).get_fontopus_server(strUrl, papp);

                  url_domain domainFontopus;

                  domainFontopus.create(strFontopusServer);

                  if(domainFontopus.m_strRadix == "ca2")
                  {
                     puser = &AppUser(papp);
                     if(puser != ::null() && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
                        if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
                     {
                        System.url().string_set(strUrl, "sessid", strSessId);
                     }
                  }

               }

            }
            if(puser != ::null() && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
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
            psession->m_memoryfile.Truncate(0);


            psession->inheaders().add(headers);
            if(set.has_property("minimal_headers") && (bool)set["minimal_headers"])
            {
               psession->m_request.attrs()["minimal_headers"] = true;
            }
            if(set.has_property("file"))
            {
               psession->m_pfile = set["file"].ca < ::ca::file >();
            }
            if(pcookies != ::null() && pcookies->get_size() > 0)
            {
               psession->request().header(__id(cookie)) = pcookies->get_cookie_header();
            }
            if(puser != ::null() && puser->m_phttpcookies != ::null() && !(bool)set["disable_ca2_user_cookies"])
            {
               psession->request().header(__id(cookie)) = puser->m_phttpcookies->get_cookie_header();
            }
            if(set.has_property(__id(cookie)) && set[__id(cookie)].get_string().has_char())
            {
               psession->request().header(__id(cookie)) = set[__id(cookie)];
            }

            bool bPost;
            bool bPut;
            if(set["put"].ca < ::ca::file >() != ::null() || set["http_request"] == "PUT")
            {
               bPost = false;
               bPut = true;
               psession->request("PUT", strRequest);
               dynamic_cast < ::sockets::http_put_socket * > (psession)->m_file = set["put"].ca < ::ca::file >();
            }
            else if(post.m_propertya.get_count() > 0 || set["http_request"] == "POST")
            {
               bPost = true;
               bPut = false;
               psession->request("POST", strRequest);
               dynamic_cast < ::sockets::http_post_socket * > (psession)->m_fields = post;
            }
            else
            {
               bPost = false;
               bPut = false;
               psession->request("GET", strRequest);
            }

            handler.add(psession);

            int32_t iIteration = 0;
            ::ca::live_signal keeplive;

            if(papp != ::null())
            {
               keeplive.keep(papp);
               keeplive.keep(&Sess(papp));
               keeplive.keep(&Sys(papp));
            }
            oprop("dw").get_value().set_type(var::type_uint32);
            dw1 = oprop("dw");
            dw2 = ::get_tick_count();

            TRACE("intertime system::request time(%d) = %d, %d, %d\n", iIteration, dw1, dw2, dw2 - dw1);

            while(handler.get_count() > 0 && !psession->m_bRequestComplete)
            {
               dw1 = ::get_tick_count();
               handler.Select(240, 0);
               keeplive.keep_alive();
               if(psession->m_estatus == ::sockets::socket::status_connection_timed_out)
               {
                  break;
               }
               if(set["file_out"].ca < ::ca::timeout_file >() != ::null())
               {
                  if(psession->m_content_length != ((size_t) -1) && set["file_out"].ca < ::ca::timeout_file >()->m_uiExpectedSize != psession->m_content_length)
                  {
                     set["file_out"].ca < ::ca::timeout_file >()->m_uiExpectedSize = psession->m_content_length;
                  }
               }
               dw2 = ::get_tick_count();
               TRACE("system::request time(%d) = %d, %d, %d\n", iIteration, dw1, dw2, dw2 - dw1);
               iIteration++;
            }

            if(set["file_out"].ca < ::ca::timeout_file >() != ::null())
            {
               if(psession->m_content_length != ((size_t) -1) && set["file_out"].ca < ::ca::timeout_file >()->m_uiExpectedSize != psession->m_content_length)
               {
                  set["file_out"].ca < ::ca::timeout_file >()->m_uiExpectedSize = psession->m_content_length;
               }
            }

            keeplive.keep_alive();

            oprop("dw") = ::get_tick_count();


            headers = psession->outheaders();

//            string strSessId;

#ifdef BSD_STYLE_SOCKETS

            if(psession->IsSSL())
            {
               strSessId = psession->m_response.m_cookies["sessid"];
               if(strSessId.has_char())
               {
                  System.sockets().m_clientcontextmap[System.url().get_server(strUrl) + "?sessid=" + strSessId] = psession->m_spsslclientcontext;
               }
            }

#endif

            string strCookie = psession->response().cookies().get_cookie_header();
            set[__id(cookie)] = strCookie;

            int32_t iStatusCode = psession->outattr("http_status_code");

            if(iStatusCode != 200)
            {
               if(iTry < 8)
               {
                  goto retry;
               }
            }

            if(pestatus != ::null())
            {
               if(iStatusCode == 0)
               {
#if defined(BSD_STYLE_SOCKETS)
                  if(psession->m_spsslclientcontext.is_set() && psession->m_spsslclientcontext->m_iRetry == 1)
                  {
                     goto retry;
                  }
#endif
               }
               if(iStatusCode == 200 || psession->outattr("http_status_code").is_empty())
               {
                  *pestatus = status_ok;
               }
               else if(psession->m_estatus == ::sockets::socket::status_connection_timed_out)
               {
                  *pestatus = status_connection_timed_out;
               }
               else if(iStatusCode >= 300 && iStatusCode <= 399)
               {
                  string strCa2Realm = psession->outheader("ca2realm-x");
                  if(::ca::str::begins_ci(strCa2Realm, "not licensed: "))
                  {
                     uint32_t dwTimeProfile2 = get_tick_count();
                     TRACE0("Not Licensed Result Total time ::ca::http::system::get(\"" + strUrl.Left(min(255,strUrl.get_length())) + "\") " + ::ca::str::from(dwTimeProfile2 - dwTimeProfile1));
                     string strLocation = psession->outheader("Location");
                     delete psession;
                     throw not_licensed(get_app(), strCa2Realm, strLocation);
                     return ::null();
                  }
               }
               else
               {
                  *pestatus = status_failed;
               }
            }

            uint32_t dwTimeProfile2 = get_tick_count();
            TRACE0("Total time ::ca::http::system::get(\"" + strUrl.Left(min(255,strUrl.get_length())) + "\") " + ::ca::str::from(dwTimeProfile2 - dwTimeProfile1));

         }
         catch(...)
         {
            if(iTry > 8)
               return ::null();
            goto retry;
         }

         return psession;

      }






      ::sockets::http_session * system::get(::sockets::socket_handler & handler, ::sockets::http_session * psession, const char * pszRequest, primitive::memory_base & memory, ::fontopus::user * puser)
      {

         ::ca::property_set post(get_app());

         ::ca::property_set headers(get_app());

         ::ca::property_set set(get_app());

         psession = request(handler, psession, pszRequest, post, headers, set, ::null(), puser, ::null(), ::null());

         if(psession == ::null())
            return ::null();

         memory.allocate(psession->GetDataLength());

         memcpy(memory.get_data(), psession->GetDataPtr(), memory.get_size());

         return psession;

      }







































































































      ::sockets::http_client_socket * system::get(
                     ::sockets::socket_handler & handler,
                     const char * pszUrl,
                     ::ca::property_set & post,
                     ::ca::property_set & headers,
                     ::ca::property_set & set,
                     ::http::cookies * pcookies,
                     ::fontopus::user * puser,
                     const char * pszVersion,
                     e_status * pestatus)
      {
#ifdef BSD_STYLE_SOCKETS
retry:
#endif

         uint32_t dwTimeProfile1 = get_tick_count();

         UNREFERENCED_PARAMETER(pszVersion);
         string strServer = System.url().get_root(pszUrl);
         string strProtocol = System.url().get_protocol(pszUrl);
         string strObject = System.url().get_object(pszUrl);
         sp(::ca::application) papp = set["app"].ca < ::ca::application >();
         int32_t iPort;
         if(strProtocol == "https")
         {
            iPort = 443;
         }
         else
         {
            iPort = 80;
         }

         if(pszVersion == ::null() || pszVersion[0] == '\0')
         {
            pszVersion = "HTTP/1.1";
         }

         string strUrl(pszUrl);

         // Format of script name example "system://server.com/the rain.mp3" => "system://server.com/the%20rain.mp3"
         {
            string strScript = System.url().url_encode(System.url().url_decode(System.url().get_script(strUrl)));
            strScript.replace("+", "%20");
            strScript.replace("%2F", "/");
            strUrl = System.url().set_script(strUrl, strScript);
         }

         ::ca::property_set setQuery(get_app());

         setQuery.parse_url_query(System.url().get_query(strUrl));



         string strSessId;
         if(!(bool)set["disable_ca2_sessid"] && !setQuery.has_property("authnone"))
         {
            if((bool)set["optional_ca2_sessid"])
            {


               if(papp != ::null())
               {

                  string strFontopusServer = Sys(papp).get_fontopus_server(strUrl, papp, 8);

                  url_domain domainFontopus;

                  domainFontopus.create(strFontopusServer);

                  if(domainFontopus.m_strRadix == "ca")
                  {
                     puser = &AppUser(papp);
                     if(puser != ::null() && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
                        if_then(set.has_property("optional_ca2_login"), !(bool)set["optional_ca2_login"]))
                     {
                        System.url().string_set(strUrl, "sessid", strSessId);
                     }
                  }

               }

            }
            if(puser != ::null() && (strSessId = puser->get_sessid(strUrl, !set["interactive_user"].is_new() && (bool)set["interactive_user"])).has_char() &&
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

         ::sockets::http_client_socket * psocket;

         bool bPost;
         bool bPut;
         if(set["put"].ca < ::ca::file >() != ::null() || set["http_request"] == "PUT")
         {
            bPost = false;
            bPut = true;
            psocket = new ::sockets::http_put_socket(handler, strUrl);
            dynamic_cast < ::sockets::http_put_socket * > (psocket)->m_file = set["put"].ca < ::ca::file >();
         }
         else if(post.get_count() > 0 || set["http_request"] == "POST")
         {
            bPost = true;
            bPut = false;
            psocket = new ::sockets::http_post_socket(handler, strUrl);
            dynamic_cast < ::sockets::http_post_socket * > (psocket)->m_fields = post;
         }
         else
         {
            bPost = false;
            bPut = false;
            psocket = new http_get_socket(handler, strUrl);
         }
         psocket->inheaders().add(headers);
         if(set.has_property("minimal_headers") && (bool)set["minimal_headers"])
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
            psocket->m_pfile = set["file"].ca < ::ca::file >();
         }
         if(pcookies != ::null() && pcookies->get_size() > 0)
         {
            psocket->request().header(__id(cookie)) = pcookies->get_cookie_header();
         }
         if(puser != ::null() && puser->m_phttpcookies != ::null() && !(bool)set["disable_ca2_user_cookies"])
         {
            psocket->request().header(__id(cookie)) = puser->m_phttpcookies->get_cookie_header();
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
            ::sockets::ssl_client_context * pcontext = set["ssl_client_context"].ca < ::sockets::ssl_client_context > ();
            if(pcontext != ::null())
            {
               psocket->m_spsslclientcontext = pcontext;
            }
            else
            {
               if(strSessId.has_char() && strSessId != "not_auth")
               {
                  psocket->m_strInitSSLClientContext = System.url().get_server(strUrl) + "?sessid=" + strSessId;
               }
            }
#endif
            psocket->EnableSSL();
         }
         uint32_t dw1 = ::get_tick_count();
         bool bConfigProxy = !set.has_property("no_proxy_config") || !(bool)set["no_proxy_config"];
         int32_t iTimeout = set["timeout"];
         if(iTimeout == 0)
            iTimeout = 23;
         else if(iTimeout < 1000)
            iTimeout = 1;
         else
            iTimeout = iTimeout / 1000;


         if(!psocket->open(bConfigProxy))
         {
            if(pestatus != ::null())
            {
               *pestatus = status_failed;
            }
            delete psocket;
            uint32_t dwTimeProfile2 = get_tick_count();
            TRACE0("Not Opened/Connected Result Total time ::ca::http::system::get(\"" + strUrl.Left(min(255,strUrl.get_length())) + "\")  " + ::ca::str::from(dwTimeProfile2 - dwTimeProfile1));
            return ::null();
         }
         uint32_t dw2 = ::get_tick_count();
         TRACE("system::get open time %d\n", dw2 - dw1);
         handler.add(psocket);

         int32_t iIteration = 0;
         ::ca::live_signal keeplive;

         if((bool)set["noloop"])
            return psocket;

         if(papp != ::null())
         {
            keeplive.keep(papp);
            keeplive.keep(&Sess(papp));
            keeplive.keep(&Sys(papp));
         }
         while(handler.get_count() > 0)
         {
            dw1 = ::get_tick_count();
            handler.Select(iTimeout, 0);
            keeplive.keep_alive();
            if(psocket->m_estatus == ::sockets::socket::status_connection_timed_out)
            {
               break;
            }
            if(set["file_out"].ca < ::ca::timeout_file >() != ::null())
            {
               if(psocket->m_content_length != ((size_t) -1) && set["file_out"].ca < ::ca::timeout_file >()->m_uiExpectedSize != psocket->m_content_length)
               {
                  set["file_out"].ca < ::ca::timeout_file >()->m_uiExpectedSize = psocket->m_content_length;
               }
            }
            dw2 = ::get_tick_count();
            TRACE("system::get time(%d) = %d, %d, %d\n", iIteration, dw1, dw2, dw2 - dw1);
            iIteration++;
         }
         keeplive.keep_alive();

         headers = psocket->outheaders();

#ifdef BSD_STYLE_SOCKETS
         if(psocket->IsSSL())
         {
            strSessId = psocket->m_response.m_cookies["sessid"];
            if(strSessId.has_char())
            {
               System.sockets().m_clientcontextmap[System.url().get_server(strUrl) + "?sessid=" + strSessId] = psocket->m_spsslclientcontext;
            }
         }
#endif

         string strCookie = psocket->response().cookies().get_cookie_header();
         set[__id(cookie)] = strCookie;

         if(pestatus != ::null())
         {
            int32_t iStatusCode = psocket->outattr("http_status_code");
#ifdef BSD_STYLE_SOCKETS
            if(iStatusCode == 0)
            {
               if(psocket->m_spsslclientcontext.is_set() && psocket->m_spsslclientcontext->m_iRetry == 1)
               {
                  goto retry;
               }
            }
#endif
            if(iStatusCode == 200 || psocket->outattr("http_status_code").is_empty())
            {
               *pestatus = status_ok;
            }
            else if(psocket->m_estatus == ::sockets::socket::status_connection_timed_out)
            {
               *pestatus = status_connection_timed_out;
            }
            else if(iStatusCode >= 300 && iStatusCode <= 399)
            {
               string strCa2Realm = psocket->outheader("ca2realm-x");
               if(::ca::str::begins_ci(strCa2Realm, "not licensed: "))
               {
                  uint32_t dwTimeProfile2 = get_tick_count();
                  TRACE0("Not Licensed Result Total time ::ca::http::system::get(\"" + strUrl.Left(min(255,strUrl.get_length())) + "\") " + ::ca::str::from(dwTimeProfile2 - dwTimeProfile1));
                  string strLocation = psocket->outheader("Location");
                  delete psocket;
                  throw not_licensed(get_app(), strCa2Realm, strLocation);
                  return ::null();
               }
            }
            else
            {
               *pestatus = status_failed;
            }
         }

         uint32_t dwTimeProfile2 = get_tick_count();
         TRACE0("Total time ::ca::http::system::get(\"" + strUrl.Left(min(255,strUrl.get_length())) + "\") " + ::ca::str::from(dwTimeProfile2 - dwTimeProfile1));

         return psocket;

      }





















      void system::get(::ca::signal_object * pobj)
      {
         SCAST_PTR(signal, psignal, pobj);
         if(psignal == ::null())
         {
            return;
         }
         ::sockets::socket_handler handler(get_app());
         sp(::sockets::http_client_socket) psocket = get(handler, psignal->m_strUrl, psignal->m_setPost, psignal->m_setHeaders, psignal->m_set, psignal->m_pcookies, psignal->m_puser, psignal->m_strVersion, &psignal->m_estatusRet);
         if(psocket == ::null())
         {
            psignal->m_bRet = false;
            return;
         }
         if(psocket->GetDataPtr() != ::null() && psocket->GetContentLength() > 0)
         {
            psignal->m_memoryRet.set_data((void *) psocket->GetDataPtr(), psocket->GetContentLength());
         }
         else
         {
            psignal->m_memoryRet.allocate(0);
         }
         psignal->m_setHeaders = psocket->outheaders();
         psignal->m_bRet = true;
         return;
      }


      bool system::download(
                     const char * pszUrl,
                     const char * pszFile,
                     ::ca::property_set & post,
                     ::ca::property_set & headers,
                     ::ca::property_set & set,
                     ::http::cookies * pcookies,
                     ::fontopus::user * puser,
                     const char * pszVersion)
      {
         ::sockets::socket_handler handler(get_app());
         sp(::sockets::http_client_socket) psocket = get(handler, pszUrl, post, headers, set, pcookies, puser, pszVersion);
         if(psocket == ::null())
            return false;

         ::ca::filesp spfile(allocer());
         if(!spfile->open(pszFile, ::ca::file::type_binary | ::ca::file::mode_create | ::ca::file::mode_read_write
            | ::ca::file::defer_create_directory))
         {
            return false;
         }
         spfile->write(psocket->GetDataPtr(), psocket->GetContentLength());
         headers = psocket->outheaders();
         return true;
      }

      bool system::download(
                     const char * pszUrl,
                     const char * pszFile,
                     const char * pszPost,
                     ::ca::property_set & headers,
                     ::http::cookies * pcookies,
                     ::fontopus::user * puser,
                     const char * pszVersion)
      {
         ::ca::property_set post;
         post.parse_url_query(pszPost);
         ::ca::property_set set;
         if(!download(pszUrl, pszFile, post, headers, set, pcookies, puser, pszVersion))
            return false;
         return true;
      }

      bool system::download(const char * pszUrl, const char * pszFile, ::fontopus::user * puser)
      {
         ::ca::property_set post;
         ::ca::property_set headers;
         ::ca::property_set set;

         if(puser == ::null())
         {
            if(!download(pszUrl, pszFile, post, headers, set))
               return false;
         }
         else
         {
            if(!download(pszUrl, pszFile, post, headers, set, puser->m_phttpcookies, puser))
               return false;
         }

         return true;
      }
      bool system::get(
                     const char * pszUrl,
                     primitive::memory_base & memory,
                     ::ca::property_set & post,
                     ::ca::property_set & headers,
                     ::ca::property_set & set,
                     ::http::cookies * pcookies,
                     ::fontopus::user * puser,
                     const char * pszVersion,
                     e_status * pestatus)
      {
         ::sockets::socket_handler handler(get_app());
         sp(::sockets::http_client_socket) psocket = get(handler, pszUrl, post, headers, set, pcookies, puser, pszVersion, pestatus);
         if(psocket == ::null())
            return false;
         memory.allocate(psocket->GetContentLength());
         memcpy(memory.get_data(), psocket->GetDataPtr(), memory.get_size());
         headers = psocket->outheaders();
         return true;
      }

      bool system::get(
                     const char * pszUrl,
                     string & str,
                     ::ca::property_set & post,
                     ::ca::property_set & headers,
                     ::ca::property_set & set,
                     ::http::cookies * pcookies,
                     ::fontopus::user * puser,
                     const char * pszVersion,
                     e_status * pestatus)
      {
         ::sockets::socket_handler handler(get_app());
         sp(::sockets::http_client_socket) psocket = get(handler, pszUrl, post, headers, set, pcookies, puser, pszVersion, pestatus);
         if(psocket == ::null())
            return false;
         str = string((const char *) psocket->GetDataPtr(), psocket->GetDataLength());
         headers = psocket->outheaders();
         return true;
      }

      bool system::get(const char * pszUrl, primitive::memory_base & storage, ::fontopus::user * puser)
      {
         class signal signal;
         signal.m_strUrl = pszUrl;
         signal.m_puser = puser;
         get(&signal);
         storage = signal.m_memoryRet;
         return signal.m_bRet;
      }


      bool system::get(const char * pszUrl, string & str, ::fontopus::user * puser)
      {
         ::ca::property_set setPost;
         ::ca::property_set setHeaders;
         ::ca::property_set set;
         return get(pszUrl, str, setPost, setHeaders, set, ::null(), puser);
      }

      string system::get(const char * pszUrl, ::fontopus::user * puser)
      {
         string str;
         if(!get(pszUrl, str, puser))
            str.Empty();
         return str;
      }

      bool system::exists(const char * pszUrl, ::fontopus::user * puser)
      {
         ::sockets::socket_handler handler(get_app());
         ::ca::property_set post;
         ::ca::property_set headers;
         ::ca::property_set set;
         set["only_headers"] = true;
         sp(::sockets::http_client_socket) psocket = get(handler, pszUrl, post, headers, set, ::null(), puser);
         if(psocket == ::null())
            return false;
         int32_t iStatusCode = psocket->outattr("http_status_code");
         return iStatusCode == 200;
      }


      bool system::request(
                     const char * pszRequest,
                     const char * pszUrl,
                     string & str,
                     ::ca::property_set & post,
                     ::ca::property_set & headers,
                     ::ca::property_set & set,
                     ::http::cookies * pcookies,
                     ::fontopus::user * puser,
                     const char * pszVersion,
                     e_status * pestatus)
      {
         ::sockets::socket_handler handler(get_app());
         set["http_request"] = pszRequest;
         sp(::sockets::http_client_socket) psocket = get(handler, pszUrl, post, headers, set, pcookies, puser, pszVersion, pestatus);
         if(psocket == ::null())
            return false;
         str = string((const char *) psocket->GetDataPtr(), psocket->GetContentLength());
         headers = psocket->outheaders();
         return true;
      }

      bool system::request(const char * pszRequest, const char * pszUrl, primitive::memory_base & storage, ::fontopus::user * puser)
      {
         class signal signal;
         signal.m_strUrl = pszUrl;
         signal.m_puser = puser;
         signal.m_set["http_request"] = pszRequest;
         get(&signal);
         storage = signal.m_memoryRet;
         return signal.m_bRet;
      }


      bool system::request(const char * pszRequest, const char * pszUrl, string & str, ::fontopus::user * puser)
      {
         ::ca::property_set setPost;
         ::ca::property_set setHeaders;
         ::ca::property_set set;
         return request(pszRequest, pszUrl, str, setPost, setHeaders, set, ::null(), puser);
      }

      string system::request(const char * pszRequest, const char * pszUrl, ::fontopus::user * puser)
      {
         string str;
         if(!request(pszRequest, pszUrl, str, puser))
            str.Empty();
         return str;
      }


      string system::gmdate(time_t t)
      {
         if(t == 0)
            t = time(::null());

         //time_t t = time((time_t *)&iExpire);
         struct tm tp;
      #ifdef _WIN32
         memcpy(&tp, gmtime(&t), sizeof(tp));
      #else
         gmtime_r(&t, &tp);
      #endif
         const char *days[7] = {"Sunday", "Monday",
          "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
         const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May",
          "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
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
         strSection.Format("proxy_auth\\%s.%s", puser->m_strLogin, "proxy_auth");
         strUserNameFile = System.dir().appdata(strSection + "_1");
         strPasswordFile = System.dir().appdata(strSection + "_2");
         bool bOk = true;
         if(!System.crypt().file_get(strUserNameFile, strUserName, ::null(), get_app())
         || strUserName.is_empty())
         {
            bOk = false;
         }
         if(!System.crypt().file_get(strPasswordFile, strPassword, ::null(), get_app())
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
            if(System.get_auth("system\\::fontopus::user\\proxy_authenticate.xhtml", strUserName, strPassword))
            {
               System.crypt().file_set(strUserNameFile, strUserName, ::null(), get_app());
               System.crypt().file_set(strPasswordFile, strPassword, ::null(), get_app());
               psocket->m_strUserNameFile = strUserNameFile;
               psocket->m_strPasswordFile = strPasswordFile;
            }
         }
      }

      void system::clean_proxy_auth(::fontopus::user * puser)
      {
         string strSection;
         strSection.Format("proxy_auth\\%s.%s", puser->m_strLogin, "proxy_auth");
         System.file().del(System.dir().appdata(strSection + "_1"));
         System.file().del(System.dir().appdata(strSection + "_2"));
      }

      bool system::put(const char * pszUrl, primitive::memory_base & memory, ::fontopus::user * puser)
      {
         ::primitive::memory_file file(get_app(), &memory);
         return put(pszUrl, &file, puser);
      }

      bool system::put(const char * pszUrl, sp(::ca::file) pfile, ::fontopus::user * puser)
      {
         if(puser == ::null())
         {
            puser = &ApplicationUser;
         }
         string str;
         ::ca::property_set post;
         ::ca::property_set headers;
         ::ca::property_set set;
         set["put"] = pfile;
         set["noclose"] = false;
         return System.http().get(pszUrl, str, post, headers, set, ::null(), puser);
      }


      bool system::put(string & strResponse, const char * pszUrl, primitive::memory_base & memory, ::fontopus::user * puser)
      {

         ::primitive::memory_file file(get_app(), &memory);

         return put(strResponse, pszUrl, &file, puser);

      }


      bool system::put(string & strResponse, const char * pszUrl, sp(::ca::file) pfile, ::fontopus::user * puser)
      {

         if(puser == ::null())
         {
            puser = &ApplicationUser;
         }

         ::ca::property_set post;
         ::ca::property_set headers;
         ::ca::property_set set;
         set["put"] = pfile;

         return get(pszUrl, strResponse, post, headers, set, ::null(), puser);

      }

   } // namespace system


} // namespace ca

