#include "framework.h" // from "aura/net/net_sockets.h"
#include "aura/net/net_sockets.h"


namespace http
{


   application::application()
   {

      m_setHttp["max_http_post"] = 5 * 1024 * 1024; // 5MB;

   }


   application::~application()
   {

   }


   property_set & application::process_set(property_set & set, const char * pszUrl)
   {

      set["app"] = get_app();


      if(Application.is_serviceable())
         return set;

      if ((bool)set["raw_http"] || (bool)set["disable_ca2_sessid"]
            || ::str::find_ci("/matter.ca2.cc/", pszUrl) >= 0 || ::str::find_ci("-matter.ca2.cc/", pszUrl) >= 0
            || ::str::find_ci("/get_fontopus_login",pszUrl) >= 0
            || ::str::find_ci("sessid=noauth", pszUrl) >= 0
            || ::str::find_ci("/matter/",System.url().get_script(pszUrl)) == 0
            || set["user"].cast < ::fontopus::user >() != NULL
            ||
            !(::str::find_wwci("ca2",pszUrl) >= 0
              || ::str::find_wwci("veriwell",pszUrl) >= 0))
         return set;

      string strWorkUrl;

      if (get_thread() != NULL)
      {

         strWorkUrl = get_thread()->m_strWorkUrl;

      }

      keep < string > keepWorkUrl(get_thread() == NULL ? &strWorkUrl : &get_thread()->m_strWorkUrl, pszUrl, strWorkUrl, true);

      if ((bool)set["optional_ca2_sessid"])
      {
         if (Session.fontopus()->m_puser != NULL)
            set["user"] = get_app()->m_psession->fontopus()->get_user(true,pszUrl);
      }
      else if (get_app() == NULL ||
               get_app()->m_psession == NULL
               || get_app()->m_psession->fontopus() == NULL)
      {

         return set;
      }
      else
      {
         set["user"] = get_app()->m_psession->fontopus()->get_user(true,pszUrl);
      }

      return set;

   }


   bool application::get(::sockets::socket_handler & handler, sp(::sockets::http_client_socket) & psession, const char * pszUrl,property_set & set)
   {

      return System.http().get(handler, psession, pszUrl, process_set(set, pszUrl));

   }


   void application::get(::message::message * pobj)
   {

      SCAST_PTR(message, pmessage, pobj);

      ::url_domain domain;

      domain.create(System.url().get_server(pmessage->m_strUrl));

      if(domain.m_strRadix == "ca2" && ::str::begins(System.url().get_object(pmessage->m_strUrl),astr.strMatterUri))
      {

         string strUrl(pmessage->m_strUrl);

         property_set set(get_app());

         set = pmessage->m_set;

         single_lock sl(System.http().m_pmutexDownload, true);

         if (!(System.http().m_straDownloading.contains(strUrl)) && !exists(pmessage->m_strUrl, set))
         {

            sl.unlock();

            pmessage->m_estatusRet = status_fail;

            pmessage->m_bRet = false;

            return;

         }

      }

      pmessage->m_set = process_set(pmessage->m_set, pmessage->m_strUrl);

      System.http().get(pobj);

   }


   bool application::get(const char * pszUrl, primitive::memory_base & memory, property_set & set)
   {

      set["get_memory"] = &memory;

      bool bOk = System.http().get(pszUrl, process_set(set, pszUrl));

      set.remove_by_name("get_memory");

      return bOk;

   }

   bool application::get(const char * pszUrl, string & str, property_set & set)
   {

      set["get_response"].get_value() = &str;

      return System.http().get(pszUrl, process_set(set, pszUrl));

   }


   string application::api_get(const char * pszUrl, property_set & set)
   {

      ::file::path url("https://api.ca2.cc/");

      url /= pszUrl;

      return get(url, set);

   }


   string application::api_get(const char * pszUrl)
   {

      property_set set;

      set["disable_common_name_cert_check"] = true;

      return api_get(pszUrl, set);

   }


   string application::get(const char * pszUrl, property_set & set)
   {

      set["get_response"] = "";

      set["bool_result"] = System.http().get(pszUrl,process_set(set,pszUrl));

      return set["get_response"].get_string();

   }


   bool application::exists(const char * pszUrl, property_set & set)
   {

      ::file::e_type etype = ::file::type_none;

      bool bExists = is_file_or_dir(pszUrl, NULL, &etype, set);

      return bExists && etype != ::file::type_none;

   }


   bool application::is_file_or_dir(const char * pszUrl, var * pvarQuery, ::file::e_type * petype, property_set & set)
   {

      string strUrl(pszUrl);

      string strFile(strUrl);

      strFile.replace(":", "_");
      strFile.replace("//", "/");
      strFile.replace("?", "%19");
      strFile = System.dir().cache() / strFile + ".meta_information";

      string strCache;

      if (!set["nocache"].get_bool())
      {

         Application.file().as_string(strFile);

         if (strCache.has_char())
         {

            if (strCache == "file")
            {

               if (is_set(petype))
               {

                  *petype = ::file::type_file;

               }

               return true;

            }
            else if (strCache == "folder")
            {

               if (is_set(petype))
               {

                  *petype = ::file::type_folder;

               }

               return true;

            }
            else if (strCache == "element")
            {

               if (is_set(petype))
               {

                  *petype = ::file::type_element;

               }

               return true;

            }
            else if (strCache == "itdoesntexist")
            {

               if (is_set(petype))
               {

                  *petype = ::file::type_none;

               }

               return false;

            }

         }

      }

      if(::str::find_wwci("ca2", System.url().get_server(pszUrl)) < 0 && System.url().get_object(pszUrl).find_ci("/matter/") < 0)
      {

         set["raw_http"] = true;

      }

      ::file::e_type etype = ::file::type_none;

      bool bExists = System.http().is_file_or_dir(strUrl, process_set(set, pszUrl), &etype);

      if (bExists)
      {

         if (etype == ::file::type_folder)
         {

            strCache = "folder";

         }
         else if (etype == ::file::type_file)
         {

            strCache = "file";

         }
         else if (etype == ::file::type_element)
         {

            strCache = "element";

         }
         else
         {

            strCache = "itdoesntexist";

         }

      }
      else
      {

         strCache = "itdoesntexist";

      }

      Application.file().put_contents(strFile, strCache);

      if (is_set(petype))
      {

         *petype = etype;

      }

      return bExists;

   }


   var application::length(const char * pszUrl, property_set & set)
   {

      return length(pszUrl, NULL, set);

   }


   var application::length(const char * pszUrl, var * pvarQuery, property_set & set)
   {

      string strUrl(pszUrl);

      string strFile(strUrl);

      strFile.replace(":", "_");
      strFile.replace("//", "/");
      strFile.replace("?", "%19");
      strFile = System.dir().cache() / strFile + ".length_question";

      bool bNoCache = set["nocache"].get_bool();

      string strCache;

      if (!bNoCache)
      {

         strCache = Application.file().as_string(strFile);

         if (strCache.has_char())
         {

            if (strCache == "-1")
            {

               return -1;

            }
            else if (strCache == "no")
            {

               return ::str::to_int64(strCache);

            }

         }

      }

      var len = System.http().length(strUrl, process_set(set, pszUrl));

      if (len.is_empty())
      {

         strCache = "-1";

      }
      else
      {

         strCache = ::str::from(len.i64());

      }

      Application.file().put_contents(strFile, strCache);

      return len;

   }


   bool application::request(const char * pszRequest, const char * pszUrl, property_set & set)
   {

      return System.http().request(pszRequest, pszUrl, process_set(set, pszUrl));

   }


   bool application::download(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession,const char * pszUrl,var varFile,property_set & set)
   {

      return System.http().download(handler, psession, pszUrl,varFile,process_set(set,pszUrl));

   }


   bool application::download(const char * pszUrl, var varFile, property_set & set)
   {
      return System.http().download(pszUrl, varFile, process_set(set, pszUrl));
   }

   bool application::put(const char * pszUrl, memory & memory, property_set & set)
   {

      return System.http().put(pszUrl, memory, process_set(set, pszUrl));
   }

   bool application::put(const char * pszUrl, ::file::file_sp  pfile, property_set & set)
   {
      return System.http().put(pszUrl, pfile, process_set(set, pszUrl));
   }



   string application::locale_schema_url(const char * pszUrl, const char * pszLocale, const char * pszSchema)
   {

      string str;

//      int32_t iAttempt = 0;

      string strUrl(pszUrl);

      if (strUrl.find("?") >= 0)
      {

         strUrl += "&";

      }
      else
      {

         strUrl += "?";

      }

      strUrl += "lang=" + string(pszLocale) + "&styl=" + string(pszSchema);

      return strUrl;

   }


   string application::get_locale_schema(const char * pszUrl,const char * pszLocale,const char * pszSchema)
   {

      string strUrl = locale_schema_url(pszUrl, pszLocale, pszSchema);

      string str;

      int iAttempt = 0;

      property_set set(get_app());

      sp(::sockets::http_session) psession;

      string strFontopusServer;

      if(atoi(System.url().get_param(pszUrl, "authnone")) == 1
            || System.url().get_param(pszUrl,"sessid").compare_ci("noauth") == 0)
      {

         strFontopusServer = pszUrl;

      }
      else
      {

         strFontopusServer = Session.fontopus()->get_fontopus_server(pszUrl);

      }

      while(::get_thread() == NULL || get_thread_run())
      {

         {

            set["get_response"] = "";

            str = Application.http().get(strUrl, set);

            if(str.has_char())
               return str;

         }

         iAttempt++;

         if(iAttempt > 11)
            return "";

         Sleep(iAttempt * 840);

      }

      return str;

   }


} // namespace http


