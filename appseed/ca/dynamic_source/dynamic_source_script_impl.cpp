﻿#include "StdAfx.h"
#include "include/FreeImage.h"
#include <math.h>


namespace dynamic_source
{


   exit_exception::exit_exception(int iCode) :
      m_iCode(iCode)
   {
   }

   script_impl::script_impl(script * pscript) :
   ca(pscript->get_app()),
      m_memoryfileaPrint(pscript->get_app()),
      m_memoryfileFallback(pscript->get_app())
   {
      //m_memfileBody << "<html>\n";
      //m_memfileBody << "<head>\n";
      //m_memfileBody << "</head>\n";
      //m_memfileBody << "<body>\n";
      //m_memfileBody << "<h1>Ti Amo Thomas Borregaard Sørensen de 15 de novembro de 1984!</h1>\n";
      //m_memfileBody << "<h1>Assinado Camilo Sasuke Tsumanuma de 2 de abril de 1977!</h1>\n";
      //m_memfileBody << "<h1>Demais!</h1>\n";
      //m_memfileBody << "</body>\n";
      //m_memfileBody.Truncate(0);

      set_app(pscript->get_app());
      m_pinstanceMain         = NULL;
      m_pnetnodesocket        = NULL;
      m_pscript               = pscript;
      m_iDebug                = 0;
      m_bSession              = false;
      m_pfun                  = NULL;
      m_pmusicdb              = NULL;
      m_pmusicsedb            = NULL;
      m_padmindb              = NULL;
      m_pnetnodedb            = NULL;
      m_psecure               = NULL;
      m_pmusic                = NULL;
      m_pcyncedb              = NULL;
      m_pcynce                = NULL;
      m_pui                   = NULL;
      m_puistr                = NULL;
      m_pcheckimage           = NULL;
      m_pprofiler             = NULL;
      m_pwb                   = NULL;
      m_ppropertysetVar       = NULL;
      m_ppropertysetSession   = NULL;
      m_pmutexSession         = NULL;
      m_pgeoiprecordClient    = NULL;
      m_bOnTopicInclude       = false;
   }

   script_impl::~script_impl()
   {
   }

   void script_impl::run()
   {
      /*   print("<html>\n");
      print("<head>\n");
      print("</head>\n");
      print("<body>\n");
      print("<h1>Ti Amo Thomas Borregaard Sørensen de 15 de novembro de 1984!</h1>\n");
      print("<h1>Assinado Camilo Sasuke Tsumanuma de 2 de abril de 1977!</h1>\n");
      print("<h1>Demais!</h1>\n");
      print("</body>\n");*/
   }


   void script_impl::script_start()
   {
      m_pinstanceMain->m_str2aScript.add_new();
      m_pinstanceMain->ob_start();
   }

   void script_impl::script_step()
   {
      string str;
      m_pinstanceMain->ob_get_clean().to_string(str);
      m_pinstanceMain->m_str2aScript.last_element().add(str);
      m_pinstanceMain->ob_start();
   }

   void script_impl::script_end()
   {
      string str;
      m_pinstanceMain->ob_get_clean().to_string(str);
      m_pinstanceMain->m_str2aScript.last_element().add(str);
      if(!gprop("g_script_left_to_right").is_set() || (bool)(gprop("g_script_left_to_right")))
      {
         str = m_pinstanceMain->m_str2aScript.last_element().implode();
      }
      else
      {
         str = m_pinstanceMain->m_str2aScript.last_element().reverse_implode();
      }
      m_pinstanceMain->m_str2aScript.remove_last();
      if(m_pinstanceMain->m_str2aScript.get_count() > 0)
      {
         m_pinstanceMain->m_str2aScript.last_element().add(str);
      }
      else
      {
         print(str);
      }
   }





   gen::property & script_impl::gprop(const char * pszKey)
   {
      return (*m_pinstanceMain->m_ppropertysetVar)[pszKey];
   }














   void script_impl::echo(const char * lpcsz)
   {
      print(lpcsz);
   }

   void script_impl::print_r(var var)
   {
      print(var.to_r_string());
   }

   void script_impl::print(const char * lpcsz)
   {
      string str(lpcsz);

      strsize iStart = 0;
      while(true)
      {
         iStart = str.find("$", iStart);
         if(iStart < 0)
            break;
         iStart++;
         if(iStart >= str.get_length())
            break;
         if(isalpha(str[iStart]))
         {
            strsize iEnd = iStart;
            iEnd++;
            while(iEnd < str.get_length()
               && (     isalpha(str[iEnd])
               || isdigit(str[iEnd])
               || str[iEnd] == '_'))
            {
               iEnd++;
            }
            string strValue = gprop(str.Mid(iStart, iEnd - iStart));
            str = str.Left(iStart - 1) + strValue + str.Mid(iEnd);
         }
      }
      raw_print(str);
   }

   void script_impl::raw_print(const char * lpcsz)
   {
      string str(lpcsz);

      if(m_pinstanceMain != NULL)
      {
         str = m_pinstanceMain->web_map(str);
      }

      output_file() << str;

   }

   void script_impl::ds_print(const char * lpcsz)
   {
      raw_print(lpcsz);
   }

   void script_impl::printf(const char * lpcsz, ...)
   {
      va_list arguments;                     // A place to store the list of arguments
      string str;
      va_start ( arguments, lpcsz );
      str.FormatV(lpcsz, arguments);
      va_end ( arguments );
      print(str);
   }

   void script_impl::printstr(const char * lpcsz, ...)
   {
      string strIndex;
      string str = lpcsz;
      va_list arguments;
      int num;
      va_start ( arguments, num );
      for ( int i = 0; true; i++ )
      {
         strIndex.Format("{$%d}", i);
         if(str.find(strIndex) < 0)
            break;
         str.replace(strIndex, va_arg ( arguments, const char * ));
      }
      va_end ( arguments );
      print(str);
   }


   void script_impl::print_url(const char * pszUrl)
   {
      print(a_go1(pszUrl));
   }


   bool script_impl::read_file(const char * lpcsz, array_ptr_alloc < int_array > * prangea, const char * pszContentType)
   {
      return m_pnetnodesocket->read_file(lpcsz, prangea, pszContentType);
   }

   string script_impl::real_path(const char * psz)
   {

      if(Application.file().exists(psz))
         return psz;

      string strRealPath;

      strRealPath = real_path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site").get_string(), string("netseed/ds/ca2")), psz);
      if(strRealPath.has_char())
         return strRealPath;

      if(m_pinstanceMain != NULL && m_pinstanceMain->m_straUserDir.get_count() > 0)
      {
         for(int i = 0; i < m_pinstanceMain->m_straUserDir.get_count(); i++)
         {
            strRealPath = real_path(m_pinstanceMain->m_straUserDir[i], psz);
            if(strRealPath.has_char())
               return strRealPath;
         }
      }

      strRealPath = real_path(System.dir().name(m_pscript->m_strSourcePath), psz);
      if(strRealPath.has_char())
         return strRealPath;

      return real_path(System.dir().path(get_manager()->m_strNetseedPath, "ds/ca2"), psz);

   }

   string script_impl::real_path(const char * pszBase, const char * psz)
   {
      return get_manager()->real_path(pszBase, psz);
   }

   string script_impl::system_path(const char * psz)
   {
      return real_path(psz);
   }

   bool script_impl::defer_expand_file(const char * lpcszPath)
   {
      
      raw_print(defer_expanded_file_string(lpcszPath));

      return true;

   }

   string script_impl::defer_expanded_file_string(const char * lpcszPath)
   {

      string strMainPath(lpcszPath);
      
      string strFile = Application.file().as_string(lpcszPath);

      string strExtension = System.file().extension(lpcszPath);

      string strRet;

      if(strExtension == "css")
      {
         strRet += ("// including file inline : \"" );
         gen::str::begins_eat_ci(strMainPath, get_manager()->m_strNetnodePath);
         strRet += (strMainPath);
         strRet += ("\"\r\n");
         int iFind;
         int iStart = 0;
         int i;
         int j;
         while((iFind = strFile.find("@import", iStart)) >= 0)
         {
            strRet += (strFile.Mid(iStart, iFind - iStart));
            i = iFind + 7; // length of @import
            while(isspace(strFile[i]) && i < strFile.get_length())
               i++;
            j = i;
            while(isalpha(strFile[j]) && j < strFile.get_length())
               j++;
            if(strFile.Mid(i, j - i) == "url")
            {
               while((isspace(strFile[j]) || strFile[j] == '(') && strFile[j] != '\r' && strFile[j] != '\n' && j < strFile.get_length())
                  j++;
               while((isspace(strFile[j]) || strFile[j] == '\"') && strFile[j] != '\r' && strFile[j] != '\n'  && j < strFile.get_length())
                  j++;
               i = j + 1;
               while(strFile[i] != '\"'  && strFile[i] != '\r' && strFile[i] != '\n'  && i < strFile.get_length())
                  i++;

               string strScript = strFile.Mid(j, i - j);

               i++; // skip quote

               while((isspace(strFile[i]) || strFile[i] == ')') && strFile[i] != '\r' && strFile[i] != '\n'  && i < strFile.get_length())
                  i++;
               while((isspace(strFile[i]) || strFile[i] == ';') && strFile[i] != '\r' && strFile[i] != '\n'  && i < strFile.get_length())
                  i++;
               string strPath;
               if(strScript.Left(1) == "/")
               {
                  
                  if(strlen(gprop("param_site")) > 0)
                  {
                     strPath = System.dir().path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), strScript);
                  }
                  if(strPath.is_empty() || !Application.file().exists(strPath))
                  {
                     strPath = System.dir().path(get_manager()->m_strNetseedPath, "ds\\ca2", strScript);
                  }
               }
               else
               {
                  strPath = System.dir().path(System.dir().name(lpcszPath), strScript);
               }
               m_pinstanceMain->ob_start();
               include(strPath);
               string str;
               m_pinstanceMain->ob_get_clean().to_string(str);
               strRet += str;
            }
            iStart = i;
         }
         strRet += (strFile.Mid(iStart));
      }
      else if(strExtension == "js")
      {
         string strTitle = System.file().title_(strMainPath);
         strRet += ("// including file inline : \"" );
         gen::str::begins_eat_ci(strMainPath, get_manager()->m_strNetnodePath);
         strRet += (strMainPath);
         strRet += ("\"\r\n");
         int iFind;
         int iStart = 0;
         int i;
         int j;
         if(strTitle == "c" && (iFind = strFile.find("___include", iStart)) >= 0)
         {
            strRet += (strFile.Mid(iStart, iFind - iStart + 10));
            iStart = iFind + 10;
         }
         while((iFind = strFile.find("___include", iStart)) >= 0)
         {
            strRet += (strFile.Mid(iStart, iFind - iStart));
            i = iFind + 10; // length of ___include
            while(isspace(strFile[i]) && i < strFile.get_length())
               i++;
            if(strFile[i] != '(')
            {
               iStart = i + 1;
               continue;
            }
            i++;
            while(isspace(strFile[i]) && strFile[i] != '\r' && strFile[i] != '\n' && i < strFile.get_length())
               i++;
            char chQuote = strFile[i];
            if(i + 1 < strFile.get_length())
               i++;
            j = i;
            while(strFile[j] != chQuote  && strFile[j] != '\r' && strFile[j] != '\n'  && j < strFile.get_length())
               j++;

            string strScript = strFile.Mid(i, j - i);

            if(j + 1 < strFile.get_length())
               j++; // skip quote


            while((isspace(strFile[j]) || strFile[j] == ')') && strFile[j] != '\r' && strFile[j] != '\n'  && j < strFile.get_length())
               j++;
            while((isspace(strFile[j]) || strFile[j] == ';') && strFile[j] != '\r' && strFile[j] != '\n'  && j < strFile.get_length())
               j++;
            string strPath;
            if(strScript.Left(1) == "/")
            {
                  
               if(strlen(gprop("param_site")) > 0)
               {
                  strPath = System.dir().path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), strScript);
               }
               if(strPath.is_empty() || !Application.file().exists(strPath))
               {
                  strPath = System.dir().path(get_manager()->m_strNetseedPath, "ds\\ca2", strScript);
               }
            }
            else
            {
               strPath = System.dir().path(System.dir().name(lpcszPath), strScript);
            }
            m_pinstanceMain->ob_start();
            include(strPath);
            string str;
            m_pinstanceMain->ob_get_clean().to_string(str);
            strRet += str;
            iStart = j;
         }
         strRet += (strFile.Mid(iStart));
      }
      else if(strExtension == "php")
      {
         strRet += ("<!-- including file inline " );
         gen::str::begins_eat_ci(strMainPath, get_manager()->m_strNetnodePath);
         strRet += (strMainPath);
         strRet += (" -->\r\n");
         strRet += (strFile);
      }
      else
      {
         strRet += (strFile);
      }


      return strRet;
   }

   var script_impl::include(const char * lpcsz, bool bRoot)
   {
      string strInclude(lpcsz);
      string strExtension = System.file().extension(strInclude);
      string strExternalMd5;
      if(strExtension == "js"
      || strExtension == "css")
      {
         string strName = System.file().name_(strInclude);
         int iEnd = strName.reverse_find('.');
         if(iEnd >= 0)
         {
            int iStart = strName.reverse_find('.', iEnd - 1);
            if(iStart >= 0)
            {
               strExternalMd5 = strName.Mid(iStart + 1, iEnd - iStart - 1);
               iEnd = strInclude.reverse_find('.');
               iStart = strInclude.reverse_find('.', iEnd - 1);
               strInclude = strInclude.Left(iStart) + "." + strExtension;
            }
         }
      }
      else if(strExtension != "ds"
         && strExtension != "php")
      {
         strInclude += ".ds";
      }
      strInclude = real_path(strInclude);
      if(m_bOnTopicInclude)
      {
         m_bOnTopicInclude = false;
         if(!get_manager()->include_matches_file_exists(strInclude))
         {
            outattr("http_status_code") = 404;
            outattr("http_status") = "Not Found";
            return include(sys_get_include_path("ds", "internal", "http_error/404"));
         }
      }

      if(!get_manager()->include_has_script(strInclude))
      {
         if(bRoot)
         {
            if(strExternalMd5.has_char())
            {
               outheader("Cache-control") = "public, max-age=3153600";
               outheader("Pragma") = "public";

               m_pnetnodesocket->outheaders().remove_by_name("Set-Cookie");

               m_pnetnodesocket->response().cookies().remove_all();
               outheader("Expires") = System.http().gmdate(strtotime("+365 day"));

            }
            else
            {
               outheader("Cache-control") = "public, max-age=0";
               outheader("Pragma") = "public";

               m_pnetnodesocket->outheaders().remove_by_name("Set-Cookie");

               m_pnetnodesocket->response().cookies().remove_all();
               outheader("Expires") = System.http().gmdate(::time(NULL));

            }

            outheader("Vary") = "Accept-Encoding";

            string strMd5 = get_manager()->include_expand_md5(strInclude);

            if(strMd5.has_char() && inheader("If-None-Match").compare_value_ci(strMd5) == 0)
            {
               
               outheader("ETag") = strMd5;
               outattr("http_status_code") = 304;
               outattr("http_status") = "Not Modified";

            }
            else
            {

               string strExpanded = defer_expanded_file_string(strInclude);

               strMd5 = System.crypt().md5(strExpanded);

               get_manager()->set_include_expand_md5(strInclude, strMd5);

               outheader("ETag") = strMd5;

               if(inheader("If-None-Match") == outheader("ETag"))
               {

                  outattr("http_status_code") = 304;
                  outattr("http_status") = "Not Modified";

               }
               else
               {
               
                  raw_print(strExpanded);

               }
            }
               
            return true;

         }

         return defer_expand_file(strInclude);

      }
      

      

      netnode::application * papp = m_papp->m_pappThis;
      script_manager * pmanager = papp->get_script_manager();
      string strHead;
      http::memory_file memfile(get_app());

      script_instance * pinstance = NULL;
      try
      {
         pinstance = pmanager->get_output_internal(this, strInclude);
      }
      catch(const exit_exception & e)
      {
         throw e;
      }
      catch(...)
      {
         pinstance = NULL;
      }
      if(pinstance == NULL)
         return var();
      else
      {
         var var = pinstance->m_varRet;
         pinstance->destroy();
         return var;
      }
   }

   void script_impl::launch(const char * lpcsz)
   {
      netnode::application * papp = m_papp->m_pappThis;
      string strVotagusFolder;
      papp->get_Votagus_folder(strVotagusFolder);
      string strCmd;
   #ifdef _DEBUG
      strCmd.Format("\""+ strVotagusFolder + "stage\\ca2\\fontopus\\app\\main\\front\\basis\\netnodeapp.exe\" launch=%s", lpcsz);
   #else
      strCmd.Format("\""+ strVotagusFolder + "stage\\ca2\\fontopus\\app\\main\\front\\Release\\netnodeapp.exe\" launch=%s", lpcsz);
   #endif

      STARTUPINFO si;
      PROCESS_INFORMATION pi;
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_SHOW;
      ::CreateProcess(NULL, (LPTSTR) (const char *) strCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
   }

   gen::property & script_impl::get(const char * pszKey)
   {
      return m_pnetnodesocket->request().form().get()[pszKey];
   }
   gen::property & script_impl::post(const char * pszKey)
   {
      return m_pnetnodesocket->request().form().post()[pszKey];
   }
   http::cookie & script_impl::cookie(const char * pszKey)
   {
      return m_pnetnodesocket->response().cookies().cookie(pszKey);
   }
   gen::property & script_impl::request(const char * pszKey)
   {
      return m_pnetnodesocket->request().form().request()[pszKey];
   }
/*   gen::property & script_impl::session(const char * pszKey)
   {
      m_pinstanceMain->session_ensure();
      return (*m_pinstanceMain->m_ppropertysetSession)[pszKey];
   }*/
   gen::relation_set & script_impl::geta()
   {
      return m_pnetnodesocket->request().form().get();
   }
   gen::relation_set & script_impl::posta()
   {
      return m_pnetnodesocket->request().form().post();
   }
   http::cookies & script_impl::cookies()
   {
      return m_pnetnodesocket->request().cookies();
   }
   gen::relation_set & script_impl::requesta()
   {
      return m_pnetnodesocket->request().form().request();
   }

   void script_impl::set_session_value(const char * psz, var value)
   {
      m_pinstanceMain->session_ensure();
      single_lock sl(m_pinstanceMain->m_pmutexSession, true);
      m_pinstanceMain->m_ppropertysetSession->operator[](psz) = value;
   }

   var script_impl::get_session_value(const char * psz)
   {
      m_pinstanceMain->session_ensure();
      single_lock sl(m_pinstanceMain->m_pmutexSession, true);
      return m_pinstanceMain->m_ppropertysetSession->operator[](psz);
   }

/*
   gen::property_set & script_impl::sessiona()
   {
      m_pinstanceMain->session_ensure();
      return *m_pinstanceMain->m_ppropertysetSession;
   }*/

   gen::property_set & script_impl::inattra()
   {
      return m_pnetnodesocket->inattrs();
   }

   string script_impl::this_url()
   {
      bool bSsl = inattr("https");
      int iPort = m_pnetnodesocket->m_iBindPort;
      string strUrl;
      if(iPort == 80)
      {
         if(bSsl)
         {
            strUrl = "https://" + inattr("http_host") + ":80";
         }
         else
         {
            strUrl = "http://" + inattr("http_host");
         }
      }
      else if(iPort == 443)
      {
         if(bSsl)
         {
            strUrl = "https://" + inattr("http_host");
         }
         else
         {
            strUrl = "http://" + inattr("http_host") + ":443";
         }
      }
      else
      {
         string strPort;
         strPort.Format("%d", iPort);
         if(bSsl)
         {
            strUrl = "https://" + inattr("http_host") + ":" + strPort;
         }
         else
         {
            strUrl = "http://" + inattr("http_host") + ":" + strPort;
         }
      }
      return strUrl + inattr("request_uri");
   }


   string script_impl::session_id(const char * pszId)
   {
      if(get("fun_session").is_set() && get("fun_session") == 0)
         return "";
      if(pszId != NULL)
      {
         set_cookie("sessid", pszId, 0);
         m_pinstanceMain->session_ensure();
         single_lock sl(m_pinstanceMain->m_pmutexSession, true);
         session_read(cookie("sessid").m_varValue.get_string(), *m_pinstanceMain->m_ppropertysetSession);
      }
      else
      {
         m_pinstanceMain->session_ensure();
      }
      return cookie("sessid").m_varValue.get_string();
   }

   /*
   void script_impl::edgify(int iEdge, ::userbase::document_template * ptemplate, bool bMakeVisible)
   {
      UNREFERENCED_PARAMETER(iEdge);
      UNREFERENCED_PARAMETER(ptemplate);
      UNREFERENCED_PARAMETER(bMakeVisible);
      //System.edgify(iEdge, ptemplate, bMakeVisible);
   }

   void script_impl::edgify_app(int iEdge, ::ca::type_info prtcApp, bool bMakeVisible)
   {
      UNREFERENCED_PARAMETER(iEdge);
      UNREFERENCED_PARAMETER(prtcApp);
      UNREFERENCED_PARAMETER(bMakeVisible);
      //System.edgify_app(iEdge, prtcApp, bMakeVisible);
   }
   */

   void script_impl::hand_root()
   {
      if(gen::str::begins(inattr("request_uri"), "/passthrough/"))
      {
         outheader("Cache-control") = "public";
         outheader("Pragma") = "public";
         outheader("Expires") = System.http().gmdate(strtotime("+1 day"));
         simple_file_server(System.dir().path(get_manager()->m_strNetnodePath, "net-img\\img\\ds\\common"));
         return;
      }

      string strSuffix = gprop("g_domain_name");
      stringa straDomainSuffix;
      if(gprop("param_root") == "cc/ca2v")
      {
         string str = inattr("http_host");
         gen::str::begins_eat(str, "ca2v.");
         gen::str::ends_eat(str, ".laborserver.net");
         straDomainSuffix.add(str);
      }
      else
      {
         straDomainSuffix.explode(".", strSuffix);
      }

      dprint("<h3>script_impl::hand_root()</h3>");
      until_here();

      dprint("<h3>system</h3>");
      dprint("param_root=$param_root<br/>");
      dprint("param_locale=$param_locale<br/>");
      dprint("param_style=$param_style<br/>");

      if(ca2_fetch_mode() != 1)
      {
         include("system/carlosgustavocecynlundgren/handle_locale");
         dprint("<h3>system/carlosgustavocecynlundgren/handle_locale</h3>");
      }
      if(gprop("param_locale").get_string().is_empty())
      {
         gprop("param_locale") = "_std";
      }
      if(gprop("param_style").get_string().is_empty())
      {
         gprop("param_style") = "_std";
      }
      if(gprop("param_style").get_string().CompareNoCase("ca2open") == 0)
      {
         if(gprop("g_base_param_style").get_string().CompareNoCase("ca2open") == 0)
         {
            if(!gen::str::ends(inattr("http_host"), ".fontopus.com") && inattr("http_host") != "fontopus.com")
            {
               outheader("Location") = "https://fontopus.com" + inattr("request_uri");
               exit(0);
            }
         }
         else
         {
            gprop("param_style") = gprop("g_base_param_style");
         }
      }
      dprint("param_root=$param_root<br/>");
      dprint("param_locale=$param_locale<br/>");
      dprint("param_style=$param_style<br/>");

      if(m_pinstanceMain->m_pnetnodesocket->IsSSL())
      {
         if(locale_is_south_america(netnode().m_strLocale))
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://amdesur.fontopus.com/";
            m_pinstanceMain->m_strImageWebSite           = "https://amdesur.fontopus.com/image/";
            m_pinstanceMain->m_strCssWebSite             = "https://amdesur.fontopus.com/css/";
            m_pinstanceMain->m_strFileWebSite            = "https://amdesur.fontopus.com/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "https://amdesur.fontopus.com/";
         }
         else if(locale_is_eu(netnode().m_strLocale))
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://fontopus.eu/";
            m_pinstanceMain->m_strImageWebSite           = "https://fontopus.eu/image/";
            m_pinstanceMain->m_strCssWebSite             = "https://fontopus.eu/css/";
            m_pinstanceMain->m_strFileWebSite            = "https://fontopus.eu/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "https://fontopus.eu/";
         }
         else if(locale_is_asia(netnode().m_strLocale))
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://fontopus.asia/";
            m_pinstanceMain->m_strImageWebSite           = "https://fontopus.asia/image/";
            m_pinstanceMain->m_strCssWebSite             = "https://fontopus.asia/css/";
            m_pinstanceMain->m_strFileWebSite            = "https://fontopus.asia/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "https://fontopus.asia/";
         }
         else
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://fontopus.com/";
            m_pinstanceMain->m_strImageWebSite           = "https://fontopus.com/image/";
            m_pinstanceMain->m_strCssWebSite             = "https://fontopus.com/css/";
            m_pinstanceMain->m_strFileWebSite            = "https://fontopus.com/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "https://fontopus.com/";
         }
      }
      else
      {
         if(locale_is_south_america(netnode().m_strLocale))
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://amdesur.fontopus.com/";
            m_pinstanceMain->m_strImageWebSite           = "http://amdesur.laborserver.net/image/";
            m_pinstanceMain->m_strCssWebSite             = "http://amdesur.laborserver.net/css/";
            m_pinstanceMain->m_strFileWebSite            = "http://amdesur.laborserver.net/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "http://amdesur.laborserver.net/";
         }
         else if(locale_is_eu(netnode().m_strLocale))
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://fontopus.eu/";
            m_pinstanceMain->m_strImageWebSite           = "http://eu.laborserver.net/image/";
            m_pinstanceMain->m_strCssWebSite             = "http://eu.laborserver.net/css/";
            m_pinstanceMain->m_strFileWebSite            = "http://eu.laborserver.net/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "http://eu.laborserver.net/";
         }
         else if(locale_is_asia(netnode().m_strLocale))
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://fontopus.asia/";
            m_pinstanceMain->m_strImageWebSite           = "http://asia.laborserver.net/image/";
            m_pinstanceMain->m_strCssWebSite             = "http://asia.laborserver.net/css/";
            m_pinstanceMain->m_strFileWebSite            = "http://asia.laborserver.net/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "http://asia.laborserver.net/";
         }
         else
         {
            m_pinstanceMain->m_strCa2OpenWebSite         = "https://fontopus.com/";
            m_pinstanceMain->m_strImageWebSite           = "http://us.laborserver.net/image/";
            m_pinstanceMain->m_strCssWebSite             = "http://us.laborserver.net/css/";
            m_pinstanceMain->m_strFileWebSite            = "http://us.laborserver.net/file/";
            m_pinstanceMain->m_strCa2VFallbackWebSite    = "http://us.laborserver.net/";
         }
      }
      gprop("g_image_site")            = m_pinstanceMain->m_strImageWebSite;
      gprop("g_css_site")              = m_pinstanceMain->m_strCssWebSite;
      gprop("g_file_site")             = m_pinstanceMain->m_strFileWebSite;
      gprop("g_ca2vfallback_site")     = m_pinstanceMain->m_strCa2VFallbackWebSite;



      gprop("request") = request_uri();
      gen::str::begins_eat(gprop("request"), gprop("g_base_request"));

      root_init();


      string strParamRoot = gstr("param_root");
      string strOverrideParamLocale = gstr("override_param_locale");
      string strOverrideParamStyle = gstr("override_param_style");
      string strParamLocale = gstr("param_locale");
      string strParamStyle = gstr("g_langdir_modifier_ex");


      dprint("param_root=$param_root<br/>");
      dprint("param_locale=$param_locale<br/>");
      dprint("param_style=$param_style<br/>");


      string strInclude;
      string strUri = gprop("request");
      string strObject = strUri;
      string strScript = System.url().object_get_script(strObject);
      string strQuery = System.url().object_get_query(strObject);
      inattr("script_extension") = System.file().extension(strScript);
      if(strScript.is_empty())
      {
         strScript = "/";
      }
      else if(strScript.Right(1) == "/")
      {
         strScript += "index";
         strObject = System.url().object(strScript, strQuery);
      }
      bool bPhp = gen::str::ends_ci(strScript, ".php");
      if(gen::str::begins(strUri, "/image/"))
      {
         if(inattr("script_extension") == "png")
         {
            outheader("Content-type") = "image/png";
         }
         else if(inattr("script_extension") == "gif")
         {
            outheader("Content-type") = "image/gif";
         }
         else if(inattr("script_extension") == "jpg")
         {
            outheader("Content-type") = "image/jpeg";
         }
         outheader("Cache-control") = "public, max-age=7257600";
         outheader("Pragma") = "public";

         m_pnetnodesocket->outheaders().remove_by_name("Set-Cookie");

         m_pnetnodesocket->response().cookies().remove_all();
         
         outheader("Expires") = System.http().gmdate(strtotime("+84 day"));
         string strPath;
         if(strlen(gprop("param_site")) > 0)
         {
            strPath = System.dir().path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), string("netseed/ds/ca2")), strScript);
         }
         if(strPath.is_empty() || !get_manager()->include_matches_file_exists(strPath))
         {
            strPath = System.dir().path(get_manager()->m_strNetseedPath, "ds\\ca2", strScript);
         }
         
         if(get_manager()->include_matches_file_exists(strPath))
         {

            outheader("ETag") = System.file36().md5(strPath);

            if(inheader("If-None-Match") == outheader("ETag"))
            {

               outattr("http_status_code") = 304;
               outattr("http_status") = "Not Modified";

            }
            else
            {
             
               read_file(strPath);

            }

         }
      }
      else if(gen::str::begins(strUri, "/css/"))
      {

         if(System.file().extension(strScript).Right(3) == "css")
         {
            outheader("Content-type") = "text/css";
         }

         string strPath;
         
         if(strlen(gprop("param_site")) > 0)
         {
            strPath = System.dir().path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), strScript);
         }
         
         if(strPath.is_empty() || !Application.file().exists(strPath))
         {
            strPath = System.dir().path(get_manager()->m_strNetseedPath, "ds\\ca2", strScript);
         }

         include(strPath, true);

      }
      else if(gen::str::begins(strUri, "/js/"))
      {
         
         if(System.file().extension(strScript).Right(3) == "css")
         {
            outheader("Content-type") = "application/javascript";
         }

         string strPath;
         
         if(strlen(gprop("param_site")) > 0)
         {
            strPath = System.dir().path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), strScript);
         }
         
         if(strPath.is_empty() || !Application.file().exists(strPath))
         {
            strPath = System.dir().path(get_manager()->m_strNetseedPath, "ds\\ca2", strScript);
         }

         include(strPath, true);

      }
      else if(strScript == "/sec" &&
         (
         gen::str::ends(inattr("http_host"), ".fontopus.com")
         || gen::str::ends(inattr("http_host"), "." + Application.m_strFontopusServer)
         || inattr("http_host") == "fontopus.com"
         || inattr("http_host") == Application.m_strFontopusServer
         || get("action") == "logout"
         || get("action") == "checkoptional")
         )
      {
         hand_sec();
      }
      else if(strScript == "/e")
      {
         hand_err();
      }
      else if(strScript == "/img")
      {
         hand_img();
      }
      else if(strScript == "/krftimg")
      {
         hand_krftimg();
      }
      else if(strScript.find(".") >= 0 && !bPhp)
      {
         hand_extension();
      }
      else
      {
         if(ca2_fetch_mode() != 1)
         {
            load_stringtable("datetime");
            load_stringtable("main");
         }
         gen::str::begins_eat(strScript, "/");
         if(bPhp)
         {
            strInclude = sys_get_include_path("php", "php", strScript);
         }
         else
         {
            strInclude = sys_get_include_path("ds", "doc", strScript);
            if(strInclude.is_empty())
            {
               strInclude = sys_get_include_path("ds", "doc", System.url().path(strScript, "index"));
               if(strInclude.has_char())
               {
                  outheader("Location") = this_url() + "/";
                  exit(0);
               }
            }
         }
         dprint("strInclude=");
         dprint(strInclude);
         if(strUri.find("thesecret") >= 0)
         {
            //AfxDebugBreak();
         }
         if(strInclude.has_char())
         {
            if(ca2_fetch_mode() != 1)
            {
               dprint("<h4>script_impl::_on_before_include() root</h4>");
               until_here();
               include("system/carlosgustavocecynlundgren/on_before_include");
               dprint("<h3>system/carlosgustavocecynlundgren/on_before_include</h3>");
               include(eval("site/$param_root/standard/_on_before_include.ds"));
               if(gprop("param_subdomain").get_string().has_char())
               {
                  dprint("<h4>script_impl::_on_before_include() subdomain</h4>");
                  until_here();
                  include(eval("site/$param_root/subdomain/$param_subdomain/secure/_on_before_include.ds"));
               }
            }
            dprint("<h3>script_impl::hand_root() Include</h3>");
            until_here();
            m_bOnTopicInclude = true;
            include(strInclude);
         }
         else
         {
            outattr("http_status_code") = 404;
            outattr("http_status") = "Not Found";
            include(sys_get_include_path("ds", "internal", "http_error/404"));
         }
      }

   }

   ///////////////////////////////////////////////
   // request_uri: /sec
   // http_host: fontopus.com

   void script_impl::hand_sec()
   {
      prof_enter("script_impl::hand_sec()");
      include(eval("site/$param_root/secure/include.ds"));
      if(gprop("param_subdomain").get_string().has_char())
      {
         include(eval("site/$param_root/subdomain/$param_subdomain/secure/include.ds"));
      }

      if(get("action") == "login")
      {
         gprop("login_request_uri") = request("ruri");

         dprint("login_request_uri=$login_request_uri");
         //      dprint_r($_REQUEST);
         //      dprint_r($_POST);

         //$local_parama = base_array();
         //$local_parama[] =  "r=login";
         //$local_parama[] =  "ruri=".$param_uri;
         //$local_parama[] =  "loginerror=Login%20Failed";

         //$ruri             = $param_ruri;
         //   $loginerror       =  "Login%20Failed";

         gprop("loginerror")   =  "Login%20Failed";

         gprop("login_request") = true;
         gprop("minUserLevel") = 30;

         secure().login();
         return;
      }
      else if(get("action") == "logout")
      {
          secure().logout();
      }
      else
      {
          secure().ensure();
         if((bool) get_session_value("auth") && isset(request("ruri")))
         {
            outheader("Location") =  urldecode(request("ruri"));
         }
      }
      prof_leave("script_impl::hand_sec()");
   }

   void script_impl::hand_err()
   {
      //   import_request_variables("gp", "param_");

      //   $param_e = parse_req_view();


      /*   if($param_err == 'song')
      {
      include sys_get_include_path('ds', 'handerr', 'song');
      }
      else if($param_err == 'songr')
      {
      include sys_get_include_path('ds', 'handerr', 'song');
      }
      else if($param_err == 'ci')
      {
      require_once 'sharedapi/image/checkimage.ds';
      checkimage_generateNewImage();
      }
      else if($param_err == 'http')
      {
      include "err/http.ds";
      }
      else
      {
      include "err/default.ds";
      }
      */

      if(get("ci").is_set())
      {
         session_ensure();
         single_lock sl(m_pinstanceMain->m_pmutexSession, TRUE);
          checkimage().generate_new_image();
      }
   }

   void script_impl::hand_karaoke()
   {
   }

   void script_impl::hand_cifras()
   {
   }

   void script_impl::hand_letras()
   {
   }


   void script_impl::ui_nav_printOut5(stringa & straLink, stringa & straTitle)
   {
      print("<span class=\"topnav5\">");
      ::count iCount = straTitle.get_size() - 1;
      script_start();
      index i;
      for(i = 0; i < iCount; i++)
      {
         if(i > 0)
         {
            print("&nbsp;");
            if(script_right())
               print("&lt;");
            else
               print("&gt;");
            print("&nbsp;");
            script_step();
         }
         print("<a class=\"topnav5-a\" href=\"" + straLink[i] + "\">" + straTitle[i] + "</a>");
         script_step();
      }
      if(iCount > 0)
      {
         print("&nbsp;");
         if(script_right())
            print("&lt;");
         else
            print("&gt;");
         print("&nbsp;");
         script_step();
      }
      print(straTitle[i]);
      script_end();
      print("</span>");
   }

   void script_impl::ui_nav_printOut3(stringa & straLink, stringa & straTitle)
   {
      ::count iCount = straTitle.get_size() - 1;
      // link array count should be 1 less than title array
      if(straLink.get_count() < iCount)
         return;
      script_start();
      int i;
      for(i = 0; i < iCount; i++)
      {
         if(i > 0)
         {
            print("&nbsp;");
            print("::");
            print("&nbsp;");
            script_step();
         }
         print("<a href=\"" + straLink[i] + "\">" + straTitle[i] + "</a>");
         script_step();
      }
      if(iCount > 0)
      {
         print("&nbsp;");
         print("::");
         print("&nbsp;");
         script_step();
      }
      if(iCount >= 0)
      {
         print(straTitle[i]);
      }
      script_end();
   }


   void script_impl::ui_nav_printOut2(stringa & straLink, stringa & straTitle)
   {
      ::count iCount = straTitle.get_size() - 1;
      int i;
      for(i = 0; i < iCount; i++)
      {
         if(i > 0)
         {
            print("&nbsp;");
            print("::");
            print("&nbsp;");
         }
         print("<a href=\"" + straLink[i] + "\">" + straTitle[i] + "</a>");
      }
      if(iCount > 0)
      {
         print ("&nbsp;");
         print ("::");
         print ("&nbsp;");
      }
      if(iCount >= 0)
      {
         print(straTitle[i]);
      }
   }


   void script_impl::ui_nav_printOut1(stringa & straLink, stringa & straTitle)
   {
      print("<div class=\"divtopnavpeta\">");
      print("<div class=\"divtopnavtera\">");
      print("<div class=\"divtopnavnear\">");
      print("<div class=\"divtopnav\">");
      if(straTitle.get_size() > 0 &&
         ((straLink.get_size() + 1) == straTitle.get_size())
         )
      {
         ::count iCount = straLink.get_size();
         int i;
         for(i = 0; i < iCount; i++)
         {
            print("<a href=\"" + straLink[i] + "\" class=\"divtopnav-a\">" + straTitle[i] + "</a>");
            if(gstr("g_langdir_modifier_ex") == "mb")
            {
               print(":&nbsp;");
            }
            else
            {
               print(" &gt; ");
            }
         }
         print("<span class=\"divtopnavsel-span\">" + straTitle[i] + "</span>");
      }
      print("</div></div></div></div>");
   }

   bool script_impl::script_right()
   {
      return false;
   }



   void script_impl::dprint(const char * psz)
   {
      if(m_pinstanceMain->m_iDebug > 0)
      {
         print(psz);
      }
   }

   void script_impl::dprint_r(var var)
   {
      if(m_pinstanceMain->m_iDebug > 0)
      {
         print_r(var);
      }
   }

   void script_impl::dinit()
   {
      //global $g_profiler;
      if(get("debug").is_set())
      {
         if(get("debug") == 4)
         {
            m_iDebug = 4;
            //      error_reporting(E_ALL);
         }
         else
         {
            m_iDebug = 1;
            //      ini_set('display_errors', 'On');
            //      error_reporting(E_ALL & ~E_NOTICE);
         }
      }
      else
      {
         m_iDebug = 0;
         //error_reporting(0);
      }
      if(isset(get("profile")))
      {
         m_pprofiler = new dynamic_source::profiler();
      }
   }

   __int64 script_impl::get_first_time()
   {
      return m_pnetnodesocket->m_iFirstTime;
   }

   void script_impl::prof_enter(const char * pszName)
   {
      if(m_pinstanceMain->m_pprofiler != NULL)
      {
         m_pinstanceMain->m_pprofiler->enter(pszName);
      }
   }

   void script_impl::prof_leave(const char * pszName)
   {
      if(m_pinstanceMain->m_pprofiler != NULL)
      {
         m_pinstanceMain->m_pprofiler->leave(pszName);
      }
   }

   void script_impl::prof_display()
   {
      if(m_pinstanceMain->m_pprofiler != NULL)
      {
         m_pinstanceMain->m_pprofiler->display();
      }
   }

   void script_impl::until_here()
   {
      __int64 iTime;
      {
         __int64 count;
         __int64 freq;
         if(QueryPerformanceCounter((LARGE_INTEGER *) &count)
            && QueryPerformanceFrequency((LARGE_INTEGER *) &freq))
         {
            iTime = count * 1000 * 1000 / freq;
         }
         else
         {
            iTime = ::GetTickCount();
         }
      }
      iTime -= get_first_time();
      string str;
      str.Format("<h4>until here %f</h4>", iTime / (1000.0 * 1000.0));
      dprint(str);
   }

   class dynamic_source::profiler * script_impl::profiler()
   {
      return m_pinstanceMain->m_pprofiler;
   }

   void script_impl::dfinish()
   {
      //global $g_debug;
      //global $g_profiler;
   }

   /*string script_impl::defer_explain_mysqli(&$mysqli, $sql)
   {
   global $g_debug;
   if($g_debug > 0 && isset($_GET['explain']))
   {
   print "AAA:Explain";
   print $sql;
   if(substr($sql, 0, 6) == "SELECT")
   {
   $result = $mysqli->query("EXPLAIN ".$sql);
   print "<table border=2>";

   print "<tr>";

   print "<td>";
   print "id";
   print "</td>";

   print "<td>";
   print "select_type";
   print "</td>";

   print "<td>";
   print "table";
   print "</td>";

   print "<td>";
   print "type";
   print "</td>";

   print "<td>";
   print "possible_keys";
   print "</td>";

   print "<td>";
   print "key";
   print "</td>";

   print "<td>";
   print "key_len";
   print "</td>";

   print "<td>";
   print "ref";
   print "</td>";

   print "<td>";
   print "rows";
   print "</td>";

   print "<td>";
   print "Extra";
   print "</td>";

   print "</tr>";
   while($row = fonto_fetch_row($result))
   {
   print "<tr>";
   foreach($row as $cell)
   {
   print "<td>";
   print $cell;
   print "</td>";
   }
   print "</tr>";
   }
   print "</table>";
   }
   else
   {
   print mysqli_error($mysqli);
   }
   }
   }
   */



   void script_impl::root_init()
   {

      dprint("<br>INIT PATH");

      comparable_array < id > & propLocaleEx = str_context()->m_plocaleschema->m_idaLocale;
      comparable_array < id > & propSchemaEx = str_context()->m_plocaleschema->m_idaSchema;
      gen::property & propLangDirModifierEx = gprop("g_langdir_modifier_ex");

      if(gprop("enable_userdir"))
      {
         if(isset(gprop("g_override_userdir")))
         {
            gprop("g_userdir_modifier") = gstr("param_root") + "/" + gstr("g_override_userdir");
         }
         else
         {
            gprop("g_userdir_modifier") = gstr("g_subdomain_modifier");
         }
      }

      if(gprop("enable_userdir"))
      {
         propLangDirModifierEx.propset().add(NULL, "/");
      }


      if(localeschema().m_bSchemaOnly)
      {
         gstr("g_langdir_modifier") = gstr("param_locale") + "/" + gstr("override_param_style") + "/";
         for(int i = 0; i < propLocaleEx.get_count(); i++)
         {
            gprop("locale") = propLocaleEx[i];
            propLangDirModifierEx.stra().add(gstr("locale") +"/" + gstr("override_param_style") +"/");
         }

         propLangDirModifierEx.stra().add("_std/" + gstr("override_param_style") +"/");
         propLangDirModifierEx.stra().add("en/" + gstr("override_param_style") +"/");
         propLangDirModifierEx.stra().add("root/_std/" + gstr("override_param_style") +"/");
         propLangDirModifierEx.stra().add("root/" + gstr("param_locale") +"/" + gstr("override_param_style") +"/");
         propLangDirModifierEx.stra().add("root/en/" + gstr("override_param_style") +"/");
         
      }
      else if(gstr("param_locale") == "k2")
      {
         gstr("g_langdir_modifier") = "en/en/";
         propLangDirModifierEx.stra().add("k2/k2/");
         propLangDirModifierEx.stra().add("en/en/");
      }
      else if(gstr("g_langdir_modifier_ex") == "mp")
      {
         gstr("g_langdir_modifier") = gstr("param_locale") + "/mp/";
         propLangDirModifierEx.stra().add(gstr("param_locale") + "/mp/");
         propLangDirModifierEx.stra().add(gstr("param_locale") + "/" + gstr("param_locale") + "/");
         propLangDirModifierEx.stra().add("_std/mp/");
      }
      else
      {
         gstr("g_langdir_modifier") = gstr("param_locale") + "/" + gstr("param_style") + "/";
         for(int i = 0; i < propLocaleEx.get_count(); i++)
         {
            gprop("locale") = propLocaleEx[i];
            gprop("style") = propSchemaEx[i];
            propLangDirModifierEx.stra().add(gstr("locale") +"/" + gstr("style") +"/");
         }

         propLangDirModifierEx.stra().add("_std/" + gstr("param_style") +"/");
         propLangDirModifierEx.stra().add("en/" + gstr("param_style") +"/");
         propLangDirModifierEx.stra().add("en/en/");
         propLangDirModifierEx.stra().add("_std/" + gstr("param_locale") +"/");
         propLangDirModifierEx.stra().add("root/_std/" + gstr("param_style") +"/");
         propLangDirModifierEx.stra().add("root/" + gstr("param_locale") +"/" + gstr("param_style") +"/");
         propLangDirModifierEx.stra().add("root/en/" + gstr("param_style") +"/");
         propLangDirModifierEx.stra().add("" + gstr("param_locale") +"/" + gstr("param_locale") +"/");
         propLangDirModifierEx.stra().add("root/" + gstr("param_locale") +"/" + gstr("param_locale") +"/");
         propLangDirModifierEx.stra().add("root/_std/" + gstr("param_locale") +"/");
         propLangDirModifierEx.stra().add("root/en/en/");
         propLangDirModifierEx.stra().add("_std/_std/");
      }

      if(gstr("param_root").is_empty())
      {
         gstr("site_modifier") = "veriwell/";
      }
      else
      {
         gstr("site_modifier") = gstr("param_root") + "/";
      }

      dprint("site_modifier=$site_modifier");

      if(gstr("param_subdomain").is_empty())
      {
         gstr("g_subdomain_modifier") = gstr("site_modifier");
      }
      else
      {
         gstr("g_subdomain_modifier") = gstr("site_modifier") + gstr("param_subdomain") + "/";
         gprop("g_subdomain_modifier_ex").stra().add(gstr("site_modifier"));
      }

      gprop("g_subdomain_modifier_ex").stra().add("_std/");

      if(gprop("enable_userdir"))
      {
         string strCandidate = eval("{$g_userdir_modifier}config");
         dprint("user dir init config.ds candidate: " + strCandidate);
         include(gstr("userdir") + strCandidate);
      }

      gprop("g_css_modifier_prefix") = use_style_calc_initial_css_modifier_prefix();

      include("site/" + gstr("site_modifier") + "standard/_root");

   }





   string script_impl::ui_redir(const char * pszLang, const char * pszStyle, const char * pszExUri, const char * pszRoot)
   {
      single_lock sl(&m_pmanager->m_csUiRedir, TRUE);
      string strRoot(pszRoot);
      string strStyle(pszStyle);
      string strLang(pszLang);
      if(strRoot.is_empty())
         strRoot = gstr("param_root");
      if(strStyle.is_empty())
         strStyle = gstr("param_style");
      if(strLang.is_empty())
         strLang = gstr("param_locale");
       return uiredir().solve(strRoot, strLang, strStyle, pszExUri);
      return "";
   }

   void script_impl::ui_redir_add(const char * pszRoot, const char * pszLang, const char * pszStyle, const char * pszTarget)
   {
      single_lock sl(&m_pmanager->m_csUiRedir, TRUE);
       return uiredir().add(pszRoot, pszLang, pszStyle, pszTarget);
      return;
   }

   string script_impl::a_go1(var var)
   {
      string strUrl = var.get_string();
      gen::property_set set(get_app());
      set.parse_url_query(System.url().get_query(var));
      if(!set["lang"].is_set())
      {
         System.url().set(strUrl, "lang", gprop("param_locale"));
      }
      if(!set["styl"].is_set())
      {
         System.url().set(strUrl, "styl", gprop("param_style"));
      }
      return strUrl;
   }

   string script_impl::ui_exuri()
   {
      return "";
   }

   webserver::ui_redir & script_impl::uiredir()
   {
      return *m_pscript->m_pmanager->m_puiredir;
   }

   string script_impl::sys_get_include_path(const char * pszExt, const char * pszType, const char * pszDoc, stringa * pstraAccept)
   {

      if(strlen(gprop("param_site")) > 0)
      {
         string str = named_sys_get_include_path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), pszExt, pszType, pszDoc, pstraAccept);
         if(str.has_char())
         {
            if(Application.file().exists(str))
               return str;
         }
      }

      for(index i = 0; i < m_pinstanceMain->m_straUserDir.get_count(); i++)
      {
         string str = named_sys_get_include_path(m_pinstanceMain->m_straUserDir[i], pszExt, pszType, pszDoc, pstraAccept);
         if(str.has_char())
         {
            if(Application.file().exists(str))
               return str;
            else
               return System.dir().path(m_pinstanceMain->m_straUserDir[i], str);
         }
      }

      return named_sys_get_include_path(System.dir().path(get_manager()->m_strNetseedPath, "ds/ca2"), pszExt, pszType, pszDoc, pstraAccept);

   }


   string script_impl::named_sys_get_include_path(const char * pszSystemPath, const char * pszExt, const char * pszType, const char * pszDoc, stringa * pstraAccept)
   {
      string strDoc(pszDoc);
      if(gen::str::ends_eat_ci(strDoc, string(".") + string(pszExt)))
      {
         string str = sys_get_include_path(pszExt, pszType, strDoc, pstraAccept);
         if(str.has_char())
            return str;
      }
      dynamic_source::profiler::scope scopeProfiler(profiler(), "script_impl::sys_get_include_path", true);
      /*   if($_SERVER['HTTP_USER_AGENT'] == "ccvotagus_ca2_fontopus")
      {
      $base_modifier = "ca2/front/";
      $path = sys_get_base_path($ext, $type, $doc, $base_modifier, $accept_table);
      if(file_exists($path))
      return $path;

      }*/
      stringa straBaseModifier;
      if(inheader("User-Agent") == "ca2 votagus veriview:1")
      {
         straBaseModifier.add("ca2votagusveriview/");
         straBaseModifier.add("front/");
      }
      else
      {
         straBaseModifier.add("front/");
      }
      string strPath;
      for(int i = 0; i < straBaseModifier.get_size(); i++)
      {
         strPath = named_sys_get_base_path(pszSystemPath, pszExt, pszType, pszDoc, straBaseModifier[i], pstraAccept);
         if(strPath.has_char())
            break;
      }
      return strPath;
   }

   string script_impl::sys_get_base_path(const char * pszExt, const char * pszType, const char * pszDoc, const char *pszBase, stringa * pstraAccept)
   {
      if(strlen(gprop("param_site")) > 0)
      {
         string str = named_sys_get_base_path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), pszExt, pszType, pszDoc, pszBase, pstraAccept);
         if(str.has_char())
         {
            if(Application.file().exists(str))
               return str;
         }
      }

      for(index i = 0; i < m_pinstanceMain->m_straUserDir.get_count(); i++)
      {
         string str = named_sys_get_base_path(m_pinstanceMain->m_straUserDir[i], pszExt, pszType, pszDoc, pszBase, pstraAccept);
         if(str.has_char())
         {
            if(Application.file().exists(str))
               return str;
            else
               return System.dir().path(m_pinstanceMain->m_straUserDir[i], str);
         }
      }

      return named_sys_get_base_path(System.dir().path(get_manager()->m_strNetseedPath, "ds/ca2"), pszExt, pszType, pszDoc, pszBase, pstraAccept);
   }

   string script_impl::named_sys_get_base_path(const char * pszSystemPath, const char * pszExt, const char * pszType, const char * pszDoc, const char *pszBase, stringa * pstraAccept)
   {
      stringa straSubdomain;
      if(gprop("enable_userdir"))
      {
         straSubdomain.add(gstr("g_userdir_modifier"));
      }
      //$subdomain_modifiers[] = $g_subdomain_modifier;
      straSubdomain.add(gstr("g_subdomain_modifier"));
      //dprint('g_subdomain_modifier_ex <br />');
      //dprint_r($g_subdomain_modifier_ex);
      //if(is_array($g_subdomain_modifier_ex))
      //   $subdomain_modifiers = array_merge($subdomain_modifiers, $g_subdomain_modifier_ex);
      straSubdomain.add(gprop("g_subdomain_modifier_ex").stra());
      //dprint('subdomain modifiers <br />');
      //dprint_r($subdomain_modifiers);
      //foreach($subdomain_modifiers as $subdomain_modifier)
      string strPath;
      for(int i = 0; i < straSubdomain.get_size(); i++)
      {
         strPath = named_sys_get_subdomain_path(pszSystemPath, pszExt, pszType, pszDoc, pszBase, straSubdomain[i], pstraAccept);
         if(!strPath.is_empty())
            break;
      }
      return strPath;
   }

   string script_impl::sys_get_subdomain_path(const char * pszExt, const char * pszType, const char * pszDoc, const char *pszBase, const char * pszSubdomain, stringa * pstraAccept)
   {
      if(strlen(gprop("param_site")) > 0)
      {
         string str = named_sys_get_subdomain_path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), pszExt, pszType, pszDoc, pszBase, pszSubdomain, pstraAccept);
         if(str.has_char())
         {
            if(Application.file().exists(str))
               return str;
         }
      }

      for(index i = 0; i < m_pinstanceMain->m_straUserDir.get_count(); i++)
      {
         string str = named_sys_get_subdomain_path(m_pinstanceMain->m_straUserDir[i], pszExt, pszType, pszDoc, pszBase, pszSubdomain, pstraAccept);
         if(str.has_char())
         {
            if(Application.file().exists(str))
               return str;
            else
               return System.dir().path(m_pinstanceMain->m_straUserDir[i], str);
         }
      }

      return named_sys_get_subdomain_path(System.dir().path(get_manager()->m_strNetseedPath, "ds/ca2"), pszExt, pszType, pszDoc, pszBase, pszSubdomain, pstraAccept);
   }


   string script_impl::named_sys_get_subdomain_path(const char * pszSystemPath, const char * pszExt, const char * pszType, const char * pszDoc, const char * pszBase, const char * pszSubdomain, stringa * pstraAccept)
   {
      gen::property & propLangDirModifierEx = gprop("g_langdir_modifier_ex");
      string strBase(pszBase);
      string strSubdomain(pszSubdomain);
      string strExt;
      if(strlen(pszExt) == 0)
      {
         strExt = "";
      }
      else
      {
         strExt = ".";
         strExt += pszExt;
      }

      string strType(pszType);
      if(strType.get_length() > 0)
      {
         strType += "/";
      }


      //      print $g_langdir_modifier;
      string strDoc(pszDoc);
      bool bAccept;
      if(pstraAccept == NULL)
      {
         bAccept = true;
      }
      else
      {
         /*         $accept_table = base_array();

         $accept_table[] = "midi";
         $accept_table[] = "information";
         $accept_table[] = "contribute";
         $accept_table[] = "contact";
         $accept_table[] = "KaraokeHub";
         $accept_table[] = "index";
         $accept_table[] = "chord-InsertWizard-QuickGuide";
         $accept_table[] = "category";
         $accept_table[] = "attfileformat";*/

         bAccept = !strDoc.is_empty() && pstraAccept->find_first(strDoc) >= 0;
      }

      if(bAccept)
      {
         // create an base_array that will hold all the possibles modifiers
         // for the path of the desired document.
         stringa straMod;
         //$local_modifiers = base_array();
         // if $g_langdir_modifier is not is_empty, add to the base_array that will be
         // use to search for a document
         //if(!is_empty($g_langdir_modifier))
         //   $local_modifiers[] = $g_langdir_modifier;
         //if(!gstr("g_langdir_modifier").is_empty())
           // straMod.add(gstr("g_langdir_modifier"));

         // if $"g_langdir_modifier_ex" is set and is an base_array, it has
         // higher priority over $g_langdir_modifier and it will
         // hold the modifiers in order of priority. Higher priority first.
         //straMod.add(propLangDirModifierEx.stra());

         gen::international::locale_schema & ls = localeschema();

         if(gprop("enable_userdir"))
         {
            for(int i = 0; i < ls.m_idaSchema.get_size(); i++)
            {
               string strCandidate = strSubdomain + strBase + string(ls.m_idaLocale[i]) + "/" + string(ls.m_idaSchema[i]) + "/" + strType + strDoc + strExt;
               dprint("candidate1=" + strCandidate);
               //$path = util_file_realpath($candidate);
               string strPath = System.dir().path(pszSystemPath, strCandidate);
               dprint("path=" + strPath);
               if(get_manager()->include_matches_file_exists(strPath))
               {
                  return strPath;
               }
               dprint("<br />");
            }
         }
         string strCandidate;
         string strPath;
         string strPrefix = System.dir().path(pszSystemPath, strBase + strSubdomain);
         string strSuffix = "/" + strType + strDoc + strExt;
         if(get_manager()->include_matches_is_dir(strPrefix))
         {
            for(int i = 0; i < ls.m_idaSchema.get_size(); i++)
            {

               strPath = strPrefix + string(ls.m_idaLocale[i]) + "/" + string(ls.m_idaSchema[i]) + strSuffix;
               dprint("path=" + strPath);
               if(get_manager()->include_matches_file_exists(strPath))
               {
                  return strPath;
               }
               dprint("<br />");
            }
         }
         dprint("<br />");

         if(!localeschema().m_bSchemaOnly)
         {

            strCandidate = strBase + strSubdomain + "_std/" + gstr("param_style") + strType + strDoc + strExt;
            dprint("candidate2=" + strCandidate);
            dprint("<br />");
            strPath = System.dir().path(pszSystemPath, strCandidate);
            dprint("path=" + strPath);
            if(get_manager()->include_matches_file_exists(strPath))
            {
               return strPath;
            }
            dprint("<br />");
            strCandidate = strBase + strSubdomain + "_std/_std/" + strType + strDoc + strExt;
            dprint("candidate3=" + strCandidate);
            strPath = System.dir().path(pszSystemPath, strCandidate);
            dprint("path=" + strPath);
            if(get_manager()->include_matches_file_exists(strPath))
            {
               return strPath;
            }
            dprint("<br />");
            dprint("<br />");

         }

      }

      /*if(strlen($subdomain_modifier) > 0)
      {
      foreach($local_modifiers as $local_modifier)
      {
      $candidate = "{$base_modifier}{$local_modifier}{$type}{$doc}{$l_ext}";
      dprint("candidate4=".$candidate);
      $path = util_file_realpath($candidate);
      if(!is_empty($path))
      {
      return $path;
      }
      }
      if($doc == "index")
      return false;
      $candidate = "{$base_modifier}_std/{$"param_style"}/{$type}{$doc}{$l_ext}";
      dprint("candidate5=".$candidate);
      $path = util_file_realpath($candidate);
      if(!is_empty($path))
      {
      return $path;
      }
      $candidate = "{$base_modifier}_std/_std/{$type}{$doc}{$l_ext}";
      dprint("candidate6=".$candidate);
      $path = util_file_realpath($candidate);
      if(!is_empty($path))
      {
      return $path;
      }
      }*/
      return "";
   }


   /*void script_impl::use_style_get_acceptmap(string_to_string_map & strmap)
   {
   if(gstr("param_style") == "veriwell" || gstr("param_root") == "veriwell")
   {
   strmap.set_at("2008-04-02", "2008-04-02/");
   strmap.set_at("2007-11-15", "2007-11-15/");
   strmap.set_at("2007-10-19", "2007-10-19/");
   strmap.set_at("2007_I", "2007-10/");
   }
   }

   string script_impl::use_style_get_default()
   {
   if(gstr("param_style") == "veriwell" || gstr("param_root") == "veriwell")
   {
   return "2008-04-02";
   }
   return "";
   }

   string script_impl::use_style_calc_initial_css_modifier_prefix()
   {

   /*global $musicdb;
   global $secureuserid;*/
   /*    string_to_string_map strmap;
   use_style_get_acceptmap(strmap);
   if(strmap.get_size() == 0)
   return "";
   string strUseStyle;
   //if(!is_object($musicdb) || !$musicdb->_fun_get_user_data($secureuserid, "use_style", $use_style))
   {
   strUseStyle = use_style_get_default();
   }
   return strmap[strUseStyle];
   }
   */
   /*string script_impl::use_style_set_style($id)
   {
   global $musicdb;
   global $secureuserid;
   $accepta = use_style_get_accepta();

   dprint("accept user style");
   dprint_r($accepta);

   if(array_key_exists($id, $accepta))
   {
   $musicdb->_fun_set_user_data($secureuserid, "use_style", $id);
   }
   }

   string script_impl::use_style_get_path_prefix($id)
   {
   $accepta = use_style_get_accepta();
   if(count($accepta) == 0)
   return '';
   return $accepta[$use_style];
   }*/





   // two level base_array
   void script_impl::menuleft_printVert001(webserver::ui_menu * pmenu, int iPanelIndex)
   {
      
      if(::ca::is_null(pmenu))
         return;
      webserver::ui_menu & menu = *pmenu;
       if(menu.get_size() > 0)
      {
         printf("<div class=\"menuleft%d-panel\">", iPanelIndex);
      }
         int iMenuIndex = gprop("menuindex");
      gprop("panelindex") = iPanelIndex;
       for(int i = 0; i < menu.get_size(); i++)
      {
         if(!menu[i]->m_strTitle.is_empty())
         {
            printf("<div class=\"menuleft%d-group\">", iPanelIndex);
            printf("<div class=\"menuleft%d-groupheader\">", iPanelIndex);
            print(menu[i]->m_strTitle);
            print("</div>\n");
         }
         for(int j = 0; j < menu[i]->get_size(); j++)
         {
            if(!menu[i]->element_at(j)->m_strTitle.is_empty())
            {
               /*            print "<a href=\"".$item['link']."\" id=\"mli$menuindex-a\" class=\"menuleft$panelindex-item-a\"   onmouseover=\"mliMOv($panelindex, $menuindex);\" onmouseout=\"mliMOu($panelindex, $menuindex);\"
               onmousedown=\"mliMDn($panelindex, $menuindex);\" onmouseup=\"mliMUp($panelindex, $menuindex);\">";*/
               printf("<a href=\"" + menu[i]->element_at(j)->m_strLink + "\" id=\"mli$menuindex-a\" class=\"menuleft$panelindex-item-a\"   onmouseover=\"mliMOv($panelindex, $menuindex);\" onmouseout=\"mliMOu($panelindex, $menuindex);\" \
                                                                         onmousedown=\"mliMDn($panelindex, $menuindex);\" >");
               printf("<div id=\"mli$menuindex-div3\" class=\"menuleft$panelindex-item-div3\">", iPanelIndex);
               printf("<div id=\"mli$menuindex-div2\" class=\"menuleft$panelindex-item-div2\">", iPanelIndex);
               printf("<div id=\"mli$menuindex-div1\" class=\"menuleft$panelindex-item-div1\">", iPanelIndex);
               print(menu[i]->element_at(j)->m_strTitle);
               print("</div>");
               print("</div>");
               print("</div>");
               print("</a>\n");
               gprop("menuindex")++;
            }
         }
         print("</div>");
      }
      if(menu.get_size() > 0)
      {
         print("</div>");
      }
   }
   /*
   string script_impl::menu_printOutNav1($linka, $titlea)
   {
   require_once "left.inc.ds";
   ?>
   <div class="divtopnavpeta">
   <div class="divtopnavtera">
   <div class="divtopnavnear">
   <div class="divtopnav">
   <?php
   $count = count($linka);
   for($i = 0; $i < $count; $i++)
   {
   print "<a href=\"".$linka[$i]."\" class=\"divtopnav-a\">".$titlea[$i]."</a>";
   print " &gt; ";
   }
   print "<span class=\"divtopnavsel-span\">".$titlea[$i]."</span>";
   ?>
   </div>
   </div>
   </div>
   </div>
   <?php
   }

   /*
   string script_impl::menu_printOutNav2($linka, $titlea)
   {
   require_once "left.inc.ds";
   ?>
   <div class="dtn3 divtopnavpeta">
   <div class="dtn2 divtopnavtera">
   <div class="dtn1 divtopnavnear">
   <div class="dtn0 divtopnav">
   <?php
   $count = count($linka);
   for($i = 0; $i < $count; $i++)
   {
   print "<a href=\"".$linka[$i]."\" class=\"divtopnav-a\">".$titlea[$i]."</a>";
   print " &gt; ";
   }
   print "<span class=\"divtopnavsel-span\">".$titlea[$i]."</span>";
   ?>
   </div>
   </div>
   </div>
   </div>
   <?php
   print "<span class=\"main-level1\">".$titlea[$i]."</span>";
   }



   */


   gen::property & script_impl::inheader(const char * psz)
   {
      return m_pnetnodesocket->inheader(psz);
   }

   gen::property & script_impl::outheader(const char * psz)
   {
      return m_pnetnodesocket->outheader(psz);
   }


   gen::property & script_impl::inattr(const char * psz)
   {
      return m_pnetnodesocket->inattr(psz);
   }

   gen::property & script_impl::outattr(const char * psz)
   {
      return m_pnetnodesocket->outattr(psz);
   }

   netnode::socket & script_impl::netnode()
   {
      return *m_pnetnodesocket;
   }

   gen::property & script_impl::request_uri()
   {
      return netnode().request().attr("request_uri");
   }


   string script_impl::langstr(const char * pszKey)
   {
      return pstr(pszKey);
      return gprop("langstr").propset()[pszKey].get_string();
   }

   string script_impl::langstr2(const char * pszKey)
   {
      return pstr(pszKey);
      return gprop("langstr").propset()[pszKey].get_string();
   }

   void script_impl::print_langstr(const char * pszKey)
   {
      print(langstr(pszKey));
   }

   void script_impl::set_langstr(const char * pszKey, const char * pszValue)
   {
      gprop("langstr").propset()[pszKey].get_value().set_string(pszValue);
   }

   string script_impl::current_url()
   {
      return inattr("http_protocol") + "://" + inattr("http_host") + inattr("request_uri");
   }


   string script_impl::urlencode(const char * psz)
   {
      return System.url().url_encode(psz);
   }

   string script_impl::urldecode(const char * psz)
   {
      return System.url().url_decode(psz);
   }

   string script_impl::htmlspecialchars(const char * psz)
   {
      return System.html().special_chars(psz);
   }

   string script_impl::htmlentities(const char * psz)
   {
      return System.html().entities(psz);
   }

   script_manager * script_impl::get_manager()
   {
      return m_pmanager;
   }


   void script_impl::load_stringtable(const char * pszLoad)
   {
      //   prof_enter('load string table');
      // Parameter: $load_stringtable
      string strLoad(pszLoad);
      string strSubdomainModifier;
      string strParamLocale = gprop("param_locale");
      string strParamStyle = gprop("param_style");
      comparable_array < id > & propLocaleEx = str_context()->m_plocaleschema->m_idaLocale;
      comparable_array < id > & propSchemaEx = str_context()->m_plocaleschema->m_idaSchema;

      strLoad += ".ds";

      dprint("<h2>LOAD STRING TABLE Start: " + strLoad + "</h2)");
      until_here();


      /*   $lang = sys_get_base_path('ds', '', $load_stringtable, 'lang/');
      if($lang)
      {
      require_once $lang;
      }*/

      stringa straSubdomainModifier;
      straSubdomainModifier.add(gprop("g_subdomain_modifier"));
      dprint("g_subdomain_modifier=$g_subdomain_modfier");
      if(gprop("load_stringtable_extra_domains").get_value().get_type() == var::type_stra)
      {
         gprop("load_stringtable_extra_domains").stra().add(gprop("load_stringtable_extra_domains").stra());
      }

      string strPath;
      string strCandidate;

      dprint("<br>");
      strCandidate = "lang/_std/en/en/" + strLoad;
      dprint("(standard english)candidate=" + strCandidate + "<br>");

      strPath = real_path(strCandidate);
      if(strPath.has_char())
      {
         dprint("loaded<br>");
         include(strPath);
      }

      for(int i = 0; i < straSubdomainModifier.get_count(); i++)
      {
         string strSubdomainModifier = straSubdomainModifier[i];
         dprint("<br>");
         strCandidate = "lang/";
         strCandidate += strSubdomainModifier;
         strCandidate += "en/en/";
         strCandidate += strLoad;
         strPath = real_path(strCandidate);
         dprint("(1)" + strSubdomainModifier + "en/en=" + strCandidate);
         if(strPath.has_char())
         {
            dprint("loaded<br>");
            include(strPath);
         }
      }


      dprint("<br>");
      strCandidate = "lang/_std/" + strParamLocale + "/" + strParamStyle + "/" + strLoad;
      dprint("(2.1)candidate=" + strCandidate + "<br>");
      strPath = real_path(strCandidate);
      if(strPath.has_char())
      {
         dprint("loaded<br>");
         include(strPath);
      }


      if(gprop("g_langdir_modifier") != (strParamLocale + "/" + strParamLocale + "/"))
      {
      for(int i = 0; i < straSubdomainModifier.get_count(); i++)
      {
      strSubdomainModifier = straSubdomainModifier[i];
      strCandidate = "lang/";
      strCandidate += strSubdomainModifier;
      strCandidate += strParamLocale;
      strCandidate += "/";
      strCandidate += strParamLocale;
      strCandidate += "/";
      strCandidate += strLoad;
      dprint("(3)lang/{\\$l_subdomain_modifier}\\$param_locale/\\$param_locale/\\$load_stringtable=" + strCandidate + "<br>");
      strPath = real_path(strCandidate);
      if(strPath.has_char())
      {
      dprint("loaded<br>");
      include(strPath);
      }
      }
      }
      dprint("step 3 end<br>");

      if(gprop("g_langdir_modifier") != (strParamLocale + "/" + strParamStyle + "/"))
      {
      for(int i = 0; i < straSubdomainModifier.get_count(); i++)
      {
      strSubdomainModifier = straSubdomainModifier[i];
      strCandidate = "lang/";
      strCandidate += strSubdomainModifier;
      strCandidate += strParamLocale;
      strCandidate += "/";
      strCandidate += strParamStyle;
      strCandidate += "/";
      strCandidate += strLoad;
      dprint("(4)lang/{\\$l_subdomain_modifier}\\$param_locale/\\$param_style/\\$load_stringtable="+ strCandidate + "<br>");
      strPath = real_path(strCandidate);
      if(strPath.has_char())
      {
      dprint("loaded<br>");
      include(strPath);
      }
      }
      }

      for(int i = 0; i < straSubdomainModifier.get_count(); i++)
      {
         strSubdomainModifier = straSubdomainModifier[i];
         for(int i = 0; i < propLocaleEx.get_count(); i++)
         {
            strCandidate = "lang/";
            strCandidate += strSubdomainModifier;
            strCandidate += propLocaleEx[i];
            strCandidate += "/";
            strCandidate += propSchemaEx[i];
            strCandidate += "/";
            strCandidate += strLoad;
            dprint("(5.1)candidate=" + strCandidate + "<br>");
            strPath = real_path(strCandidate);
            if(strPath.has_char())
            {
               dprint("loading...<br>");
               include(strPath);
            }
         }

         strCandidate = "lang/"+strSubdomainModifier+ gstr("g_langdir_modifier") + strLoad;
         dprint("(5)candidate=" + strCandidate + "<br>");
         strPath = real_path(strCandidate);
         if(strPath.has_char())
         {
            dprint("loading...<br>");
            include(strPath);
         }
      }
      until_here();
      dprint("<h2>LOAD STRING TABLE End: " + strLoad + "</h2>");
   }

   string script_impl::eval(const char * pszText)
   {
      string str(pszText);
      strsize iStart = 0;
      bool bBracket;
      while(true)
      {
         bBracket = false;
         iStart = str.find("$", iStart);
         if(iStart < 0)
            break;
         if(iStart > 0)
         {
            if(str[iStart - 1] == '{')
            {
               bBracket = true;
            }
         }
         iStart++;
         strsize iEnd = iStart;
         while(iEnd < str.get_length()
            && (isalpha(str[iEnd]) || isdigit(str[iEnd]) || str[iEnd] == '_'))
         {
            iEnd++;
         }
         string strValue = gprop(str.Mid(iStart, iEnd - iStart));
         if(bBracket)
         {
            str = str.Left(iStart - 2) + strValue + str.Mid(iEnd + 1);

         }
         else
         {
            str = str.Left(iStart - 1) + strValue + str.Mid(iEnd);
         }
      }
      return str;
   }

   //   prof_leave('load string table');



   ::webserver::fun::fun & script_impl::fun()
   {
      if(m_pinstanceMain->m_pfun == NULL)
      {
          m_pinstanceMain->m_pfun = new ::webserver::fun::fun();
          m_pinstanceMain->keep(m_pinstanceMain->m_pfun);
      }
      return *m_pinstanceMain->m_pfun;
   }

   ::webserver::fontopus_database & script_impl::musicdb()
   {
      if(m_pinstanceMain->m_pmusicdb == NULL)
      {
          m_pinstanceMain->m_pmusicdb = get_manager()->new_musicdb(0);
          m_pinstanceMain->m_pmusicdb->m_pinterface = m_pinstanceMain;
          m_pinstanceMain->m_pmusicdb->m_pmusicdatabasecache->m_pinterface = m_pinstanceMain;
      }
      return *m_pinstanceMain->m_pmusicdb;
   }

   ::webserver::fontopus_database & script_impl::musicdb_se()
   {
      if(m_pinstanceMain->m_pmusicsedb == NULL)
      {
          m_pinstanceMain->m_pmusicsedb = get_manager()->new_musicdb(1);
          m_pinstanceMain->m_pmusicsedb->m_pinterface = m_pinstanceMain;
          m_pinstanceMain->m_pmusicsedb->m_pmusicdatabasecache->m_pinterface = m_pinstanceMain;
      }
      return *m_pinstanceMain->m_pmusicsedb;
   }

   ::webserver::fontopus_database & script_impl::admindb()
   {
      if(m_pinstanceMain->m_padmindb == NULL)
      {
          m_pinstanceMain->m_padmindb = new ::webserver::fontopus_database(get_app());
          m_pinstanceMain->m_padmindb->m_pinterface = m_pinstanceMain;
          m_pinstanceMain->m_padmindb->m_pmusicdatabasecache->m_pinterface = m_pinstanceMain;
          m_pinstanceMain->m_padmindb->initialize(2);
      }
      return *m_pinstanceMain->m_padmindb;
   }

   ::webserver::fontopus_database & script_impl::netnodedb()
   {
      if(m_pinstanceMain->m_pnetnodedb == NULL)
      {
          m_pinstanceMain->m_pnetnodedb = new ::webserver::fontopus_database(get_app());
          m_pinstanceMain->keep(m_pinstanceMain->m_pnetnodedb);
          m_pinstanceMain->m_pnetnodedb->initialize(1);
      }
      return *m_pinstanceMain->m_pnetnodedb;
   }

   ::webserver::secure & script_impl::secure()
   {
      if(m_pinstanceMain->m_psecure == NULL)
      {
          m_pinstanceMain->m_psecure = new class ::webserver::secure();
      }
      return *m_pinstanceMain->m_psecure;
   }

   ::webserver::music::music & script_impl::music()
   {
      if(m_pinstanceMain->m_pmusic == NULL)
      {
         if(m_pinstanceMain->m_pmusic == NULL)
         {
            m_pinstanceMain->m_pmusic = new ::webserver::music::music();
         }
      }
      return *m_pinstanceMain->m_pmusic;
   }

   ::webserver::cynce_database & script_impl::cyncedb()
   {
      if(m_pinstanceMain->m_pcyncedb == NULL)
      {
         m_pinstanceMain->m_pcyncedb = get_manager()->new_cyncedb();
         m_pinstanceMain->m_pcyncedb->m_pinterface = m_pinstanceMain;
      }
      return *m_pinstanceMain->m_pcyncedb;
   }

   ::webserver::cynce & script_impl::cynce()
   {
      if(m_pinstanceMain->m_pcynce == NULL)
      {
                   m_pinstanceMain->m_pcynce = new class ::webserver::cynce();
      }
      return *m_pinstanceMain->m_pcynce;
   }

   ::webserver::user & script_impl::user()
   {
      if(m_pinstanceMain->m_pui == NULL)
      {
                   m_pinstanceMain->m_pui = new class ::webserver::user();
      }
      return *m_pinstanceMain->m_pui;
   }

   ::webserver::wb & script_impl::wb()
   {
      if(m_pinstanceMain->m_pwb == NULL)
      {
                   m_pinstanceMain->m_pwb = new class ::webserver::wb();
      }
      return *m_pinstanceMain->m_pwb;
   }

   void script_impl::keep(ptr * pptr)
   {
      pptr->m_pinterface = this;
      pptr->m_ulFlags &= ~ca::flag_auto_clean;
      m_pinstanceMain->m_ptra.add_unique(pptr);
   }

   void script_impl::unkeep(ptr * pptr)
   {
      m_pinstanceMain->m_ptra.remove(pptr);
   }

   int script_impl::exit(int iExitCode)
   {
      throw exit_exception(iExitCode);
   }


   bool script_impl::session_open(const char * pszSavePath, const char * pszSessionName)
   {
      gprop("sessid_save_path") = pszSavePath;
      gprop("sessid_session_name") = pszSessionName;
      return true;
   }

   bool script_impl::session_close()
   {
      return true;
   }

   bool script_impl::session_read(const char * pszId, gen::property_set & set)
   {
      UNREFERENCED_PARAMETER(pszId);
      UNREFERENCED_PARAMETER(set);
      if(get("fun_session").is_set() && get("fun_session") == 0)
         return false;
      //return netnodedb().get_session(pszId, set);
      //return musicdb().get_session(pszId, set);
      //      set = get_manager()->get_session(pszId)->m_set;
      return true;
   }

   bool script_impl::session_write(const char * pszId, gen::property_set & set)
   {
      UNREFERENCED_PARAMETER(pszId);
      UNREFERENCED_PARAMETER(set);
      if(get("fun_session").is_set() && get("fun_session") == 0)
         return false;
      //return netnodedb().set_session(pszId, set);
      //return musicdb().set_session(pszId, set);
      //get_manager()->get_session(pszId)->m_set = set;
      return true;
   }

   bool script_impl::session_destroy(const char * pszId)
   {
      UNREFERENCED_PARAMETER(pszId);
      //return delete(sessid);
      return true;
   }

   /*********************************************
   * WARNING - You will need to implement some *
   * sort of garbage collection routine here.  *
   *********************************************/
   bool script_impl::session_gc(int maxlifetime)
   {
      UNREFERENCED_PARAMETER(maxlifetime);
      return true;
   }


   bool script_impl::main_initialize()
   {
      if(this != m_pinstanceMain)
         throw 0;

      m_ppropertysetVar = new gen::property_set(get_app());
      m_puistrcontext = new ::user::str_context(get_app());
      m_puistrcontext->m_pstr = &System.str();

      return true;
   }

   bool script_impl::main_finalize()
   {
      if(this != m_pinstanceMain)
         throw 0;
      if(cookies().find_cookie("sessid") >= 0 && m_pnetnodesocket->response().cookies().find_cookie("sessid") >= 0 && m_ppropertysetSession != NULL)
      {
         session_write(cookie("sessid").m_varValue, *m_ppropertysetSession);
      }
      dprint(unitext("<h2>つづく</h2>"));
      until_here();
      prof_display();

      if(m_pmusicdb != NULL)
      {
                   m_ptra.remove(dynamic_cast < ptr * > (m_pmusicdb));
                   get_manager()->free_musicdb(m_pmusicdb);
      }
      if(m_pmusicsedb != NULL)
      {
                   m_ptra.remove(dynamic_cast < ptr * > (m_pmusicsedb));
                   get_manager()->free_musicdb(m_pmusicsedb);
      }
      if(m_pcyncedb != NULL)
      {
                   m_ptra.remove(dynamic_cast < ptr * > (m_pcyncedb));
                   get_manager()->free_cyncedb(m_pcyncedb);
      }


      if(m_pgeoiprecordClient != NULL)
      {
         GeoIPRecord_delete(m_pgeoiprecordClient);
      }

      for(int i = 0; i < m_ppropertysetVar->get_count(); i++)
      {
         if(m_ppropertysetVar->m_propertya[i].get_value().get_type() == var::type_ca2)
         {
            ptr * p = dynamic_cast < ptr * >
               (m_ppropertysetVar->m_propertya[i].get_value().m_pca2);
            if(p != NULL)
            {
               m_ptra.remove(p);
            }
         }
      }

      delete m_ppropertysetVar;
      if(m_ppropertysetSession != NULL)
      {
         //delete m_ppropertysetSession;
      }

      __int64 countSafe = m_ptra.get_count();
      while(countSafe > 0)
      {
         // deleting a "ptr" derived object unkeep's it -
         // removing from m_ptra
         try
         {
            if(m_ptra.last_element()->m_bUnkeep && m_ptra.last_element()->::ca::ca::is_set_ca_flag(::ca::ca::flag_auto_clean))
            {
               delete m_ptra.last_element();
            }
         }
         catch(...)
         {
         }
         countSafe--;
         // if it fails to remove from m_ptra,
         // remove it anyway
         if(countSafe > m_ptra.get_count())
         {
            m_ptra.remove_at(m_ptra.get_upper_bound());
         }
      }
      try
      {
         if(m_puistrcontext != NULL)
         {
            delete m_puistrcontext;
         }
      }
      catch(...)
      {
      }

      return true;
   }


   void script_impl::session_ensure()
   {
      if(get("fun_session").is_set() && get("fun_session") == 0)
         return;

      if(!m_bSession)
      {
         m_bSession = true;
         if(cookie("sessid").m_varValue.get_string().get_length() > 10) // heuristical check
         {
            session_read(cookie("sessid").m_varValue, *m_pinstanceMain->m_ppropertysetSession);
         }
         else
         {
                      set_cookie("sessid", fun().key1_generate_sessid());
         }
         if(m_ppropertysetSession == NULL)
         {
            m_ppropertysetSession = &get_manager()->get_session(m_pmutexSession, session_id())->m_set;
            m_pmutexSession = &get_manager()->get_session(m_pmutexSession, session_id())->m_mutex;
         }
      }
   }

   string script_impl::uri_set_var(const char * pszQuery, const char * pszKey, var var)
   {
      return System.url().set_key(pszQuery, pszKey, var);
   }

   string script_impl::get_current_url()
   {
      //return inattr("http_protocol")//" + inattr("http_host").get_string()); print (inattr("request_uri"));
      return inattr("http_protocol") + "://" + inattr("http_host").get_string() + inattr("request_uri");
   }


   int script_impl::ca2_fetch_mode(int iMode)
   {
      if(iMode >= 0)
      {
         gprop("g_ca2_fetch_mode") = iMode;
      }
      return gprop("g_ca2_fetch_mode");
   }



   string script_impl::set_cookie(
      const char * pszKey,
      var varValue,
      int iExpire,
      const char * pszPath,
      const char * pszDomain,
      bool bSecure)
   {
      return m_pnetnodesocket->set_cookie(
         pszKey,
         varValue,
         iExpire,
         pszPath,
         pszDomain,
         bSecure);
   }




   void script_impl::use_style_get_accepta(gen::property_set & set)
   {
      if(gprop("g_base_param_style") == "veriwell" || gprop("param_root") == "veriwell")
      {
         set["2009-01-21"] = "2009-01-21/";
         set["2008-11-15"] = "2008-11-15/";
         set["2008-04-02"] = "2008-04-02/";
         set["2007-11-15"] = "2007-11-15/";
         set["2007-10-19"] = "2007-10-19/";
         set["2007_I"] = "2007-10/";
      }
   }

   string script_impl::use_style_get_default()
   {
      if(gprop("g_base_param_style") == "veriwell" || gprop("param_root") == "veriwell")
      {
         return "2009-01-21";
      }
      return "";
   }



   string script_impl::use_style_calc_initial_css_modifier_prefix()
   {
      if(ca2_fetch_mode() == 1)
         return "";
      if(gen::str::ends(inattr("http_host"), ".fontopus.com") || inattr("http_host") == "fontopus.com")
         return "";
      string strRequestUri = inattr("request_uri");
      if(gen::str::begins(strRequestUri, "/image/"))
         return "";
      if(gen::str::begins(strRequestUri, "/css/"))
         return "";
      if(gen::str::begins(strRequestUri, "/js/"))
         return "";

      
      if(!isset(get("secureuserid")) || get("secure"))
      {
      var varOptional = gprop("secure_optional");
      gprop("secure_optional") = true;
      include("site/cc/.Votagus/secure/secure");
      gprop("secure_optional") = varOptional;
      }
      

      gen::property_set accepta;
      use_style_get_accepta(accepta);
      if(accepta.m_propertya.get_count() == 0)
         return "";
      var strUseStyle;
      if(!gprop("secureuserid").is_set())
      {
         dprint("there is no secureuserid");
         strUseStyle = use_style_get_default();
      }
 
       else if(!musicdb()._fun_get_user_data(gprop("secureuserid"), "use_style", strUseStyle))
      {
         dprint("could not get data");
         strUseStyle = use_style_get_default();
      }
      return accepta[strUseStyle];
   }

   void script_impl::use_style_set_style(const char * pszId)
   {
      gprop("secure_optional") = false;
      gprop("minUserLevel") = 30;
      include("site/cc/.Votagus/secure/secure");
      gen::property_set accepta;
      use_style_get_accepta(accepta);

      dprint("accept user style");
      //dprint_r(accepta);

      if(accepta.has_property(pszId) && gprop("secureuserid").is_set())
      {
         dprint("ACCEPTED->$id");
                   musicdb()._fun_set_user_data(gprop("secureuserid"), "use_style", pszId);
      }
   }

   string script_impl::use_style_get_path_prefix(const char * pszId)
   {
      gen::property_set accepta;
      use_style_get_accepta(accepta);
      if(accepta.m_propertya.get_count() == 0)
         return "";
      return accepta[pszId];
   }

   string  script_impl::use_style_get1()
   {
      if((gprop("g_css_modifier_prefix") == "2008-11-15/" || gprop("g_css_modifier_prefix") ==  "2008-04-02/"
         || gprop("g_css_modifier_prefix") == "2007-11-15/"  || gprop("g_css_modifier_prefix") == "2007-10-19/"
         || gprop("g_css_modifier_prefix") == "2007-10/") && gprop("param_root") == "veriwell")
      {
         return "1";
      }
      return "_std";
   }

   webserver::checkimage & script_impl::checkimage()
   {
                if(m_pcheckimage == NULL)
                   m_pcheckimage = new class webserver::checkimage();
      return *m_pcheckimage;
   }

   void script_impl::header(const char * pszHeader)
   {
      string strHeader(pszHeader);
      strsize iFind = strHeader.find(":");
      if(iFind < 0)
      {
         TRACE("header(x) : x should contain :");
         dprint("header(x) : x should contain :");
         return;
      }
      string strKey = strHeader.Left(iFind);
      string strValue = strHeader.Mid(iFind + 1);
      strKey.trim();
      strValue.trim_left();
      strValue = web_map(strValue);
      outheader(strKey) = strValue;
   }


   void script_impl::simple_file_server(const char * psz, const char * pszRelative)
   {

      string strRelative;
      if(pszRelative != NULL)
      {
         strRelative = string(pszRelative);
      }
      else
      {
         strRelative = System.url().url_decode(System.url().get_script(inattr("request_uri")));
      }
      
      string strPath;
      
      strPath = System.dir().path(psz, strRelative);

      string strCandidate;
      if(Application.dir().is(strPath))
      {
         strCandidate = System.dir().path(strPath, "index.ds");
         if(Application.file().exists(strCandidate))
         {
            include(strCandidate);
            return;
         }
         strCandidate = System.dir().path(strPath, "index.html");
         if(Application.file().exists(strCandidate))
         {
            strPath = strCandidate;
            goto ok1;
         }
         strCandidate = System.dir().path(strPath, "index.htm");
         if(Application.file().exists(strCandidate))
         {
            strPath = strCandidate;
            goto ok1;
         }
         return;
      }
      else if(gen::str::ends_ci(strPath, ".ds"))
      {
         include(strPath);
         return;
      }
      else
      {
         strCandidate = strPath + ".ds";
         if(Application.file().exists(strCandidate))
         {
            include(strCandidate);
            return;
         }
      }

ok1:

      m_pnetnodesocket->simple_file_server(strPath);

   }

   bool script_impl::isset(var & var)
   {
      return var.is_set();
   }

   bool script_impl::isset(gen::property & prop)
   {
      return prop.is_set();
   }

   bool script_impl::is_null(var & var)
   {
      return var.is_null();
   }

   bool script_impl::is_null(gen::property & prop)
   {
      return prop.is_null();
   }

   string script_impl::pstr(id pszTopic, gen::property_set & set, id pszLocale, id pszStyle)
   {
      string str = pstr(pszTopic, pszLocale, pszStyle);
      set.replace_ex1(str);
      return str;
   }

   string script_impl::pstr(id pszTopic, id pszLocale, id pszStyle)
   {
      single_lock sl(&m_pmanager->m_csPersistentStr, TRUE);
      if(pszLocale.is_empty())
      {
         pszLocale = gprop("param_locale");
      }
      if(pszStyle.is_empty())
      {
         pszStyle = gprop("param_style");
      }
      string strTopic(pszTopic);
      string str = System.str().get(str_context(), pszTopic, pszLocale, pszStyle);
      if(str.has_char())
         return str;
      str = System.str().get(str_context(), pszTopic, pszLocale, pszStyle);
      if(str.has_char())
         return str;
      if(strTopic == "ca2plugin.install_ca2_rem")
      {
         TRACE("teste");
      }
      return System.str().get(str_context(), pszTopic, pszLocale, pszStyle);
   }

   void script_impl::pstr_set(id pszTopic, id pszLocale, id pszStyle, const char * psz)
   {
      single_lock sl(&m_pmanager->m_csPersistentStr, TRUE);
      System.str().set(pszTopic, pszLocale, pszStyle, psz);
   }




   string script_impl::ui_url(const char * pszTopic, const char * pszStyle)
   {
      return ui_redir(NULL, pszStyle, pszTopic);
   }

   void script_impl::cla_sel1(const char * pszSection, const char * pszItem)
   {
      if(gprop("g_cla")[pszSection] == pszItem)
      {
         print("cl-emp");
      }
      else
      {
         print("topitem");
      }
   }


   string script_impl::ca2_get_build(const char * pszVersion)
   {
      string strPath;
      strPath = "C:\\home\\ccvotagus\\ca2_spa\\stage\\app\\build.txt";
      string str = Application.file().as_string(strPath);
      str.trim();
      return str;
   }

   string script_impl::ca2_get_subversion_revision(const char * pszVersion)
   {
      string strUrl;
      strUrl = "http://ca2.se/votagus_ca2_get_subversion_revision?build=" + urlencode(ca2_get_build(pszVersion)) + "&secure=0";
      return Application.http().get(strUrl);
   }

   string decimal_a(int i)
   {
      switch(i)
      {
      case 0:
         return "a";
      case 1:
         return "b";
      case 2:
         return "c";
      case 3:
         return "d";
      case 4:
         return "e";
      case 5:
         return "f";
      case 6:
         return "g";
      case 7:
         return "h";
      case 8:
         return "i";
      case 9:
         return "j";
      }
      return "";
   }

   string script_impl::ca2_format_build(const char * pszVersion)
   {
      string src = ca2_get_build(pszVersion);
      src.trim();
      return src;
      gen::property_set set;
      System.datetime().international().parse_str(src, set);
      string dec = decimal_a(atoi(src.Mid(14, 1)));
      if(set["year"].get_integer() < 1900)
      return "";
      class time time(set["year"], set["month"], set["day"], 0, 0, 0);
      int w = atoi(time.Format("%w"));
      class time time_now = time::get_current_time();
      int curj = atoi(time_now.Format("%j"));
      int curz = atoi(time_now.Format("%U"));
      int cury = atoi(time_now.Format("%Y"));
      int j = atoi(time.Format("%j"));
      int z = atoi(time.Format("%U"));
      class time time_t(set["year"], 12, 31, 0, 0, 0);
      int jt = atoi(time_t.Format("%j"));
      int zt = atoi(time_t.Format("%U"));
      int year = set["year"];
      string str;
      if((curj - j < 7 && cury == year) || ((cury == year + 1) && curj + jt - j < 7))
      {
      str += datetime_get_weekday_str(w) + " ";
      if((curj - j < 1 && cury == year) || ((cury == year + 1) && curj + jt - j < 1))
      {
      str += set["hour"];
      dec.make_upper();
      str += dec;
      }
      }
      else if(cury == year)
      {
      str += datetime_get_month_str(set["month"]) + " ";
      str += set["day"] + " ";
      if(curj - j < 21)
      {
      str += datetime_get_weekday_str(w) + " ";
      }
      }
      else
      {
      str = set["year"] + " ";
      str += datetime_get_month_str(set["month"]) + " ";
      str += set["day"] + " ";
      }

      str.trim();

      return str;
   }


   string script_impl::datetime_get_month_str(int idx, const char * pszLocale, const char * pszStyle)
   {
      string str;
      str.Format("datetimestr_month[%d]", idx - 1);
      return pstr(str, pszLocale, pszStyle);
   }

   string script_impl::datetime_get_month_short_str(int idx, const char * pszLocale, const char * pszStyle)
   {
      string str;
      str.Format("datetimestr_month_short[%d]", idx - 1);
      return pstr(str, pszLocale, pszStyle);
   }

   string script_impl::datetime_get_weekday_short_str(int idx, const char * pszLocale, const char * pszStyle)
   {
      string str;
      str.Format("datetimestr_weekday_short[%d]", idx);
      return pstr(str, pszLocale, pszStyle);
   }

   string script_impl::datetime_get_weekday_str(int idx, const char * pszLocale, const char * pszStyle)
   {
      string str;
      str.Format("datetimestr_weekday_long[%d]", idx);
      return pstr(str, pszLocale, pszStyle);
   }


   //string script_impl::datetime_get_month_day_count(int year, int month)


   //}
   /*
   string script_impl::datetime_getGMTDateTime()
   {
   return datetime_formatDateTime(datetime_getTimeStamp());
   }
   */
   /*
   string script_impl::datetime_formatDateTime($timestamp)
   {
   return gmdate("Y-m-d H:i:s", $timestamp);
   }
   */

   /*int64_t script_impl::datetime_get_timestamp()
   {
   return time(NULL);
   }*/

   /*
   string script_impl::datetime_InternationalToUnixMillis($date)
   {
   return mktime(substr($date, 11, 2), substr($date, 14, 2), substr($date, 17, 2),
   substr($date, 5, 2),  substr($date, 8, 2), substr($date, 0, 4)) * 1000;
   }
   */

   /*string script_impl::datetime_getToday()
   {
   return gmdate("Y-m-d 00:00:00", time());
   }

   string script_impl::datetime_strtotime($str, $time = FALSE)
   {
   if($time === FALSE)
   $time = time();
   return gmdate("Y-m-d 00:00:00", strtotime($str, $time));
   }
   */

   int64_t script_impl::strtotime(const char * psz, time_t time)
   {
      int iPathCount;
      return System.datetime().strtotime(NULL, psz, time, 0, iPathCount);
   }


   int64_t script_impl::strtotime(const char * psz)
   {
      int iPathCount;
      return System.datetime().strtotime(NULL, psz, 0, iPathCount);
   }

   int64_t script_impl::str_context_strtotime(const char * psz, time_t time)
   {
      int iPathCount;
      return System.datetime().strtotime(str_context(), psz, time, 0, iPathCount);
   }


   int64_t script_impl::str_context_strtotime(const char * psz)
   {
      int iPathCount;
      return System.datetime().strtotime(str_context(), psz, 0, iPathCount);
   }

   string script_impl::strftime(const char * psz, time_t timeParam)
   {
      return System.datetime().strftime(psz, timeParam);
   }

   string script_impl::strftime(const char * psz)
   {
      string str;
      class time time;
      time = time::get_current_time();
      time.FormatGmt(str, psz);
      return str;
   }

   var script_impl::rand(var start, var end)
   {
      return System.math().RandRange(start.get_integer(), start.get_integer());
   }


   void script_impl::echo_spa(const char * psz, const char * pszBuild)
   {
      print(spa_url(psz, pszBuild));
   }


   string script_impl::spa_url(const char * psz, const char * pszBuild)
   {
      string strFormatBuild;
      if(pszBuild == NULL)
         strFormatBuild = ca2_get_build(pszBuild);
      else
         strFormatBuild = pszBuild;
      if(strFormatBuild.has_char())
      {
         strFormatBuild.replace(" ", "_");
         string strUrl;
         strUrl.Format(
            "%s_%s.spaboot",
            strFormatBuild,
            psz);
         return strUrl;
      }
      else
      {
         return "http://veriaxs.com/building?ruri=" + this_url();
      }
   }

   void script_impl::hand_krftimg()
   {
      var size;
      var bk;
      if(get("mb").get_string().has_char())
      {
         string strText = urldecode(get("mb"));
         if(strText == "gustavo" ||
            strText == "camilo." ||
            strText == "muitobem" ||
            strText == "cantar." ||
            strText == "tocar.musica." ||
            strText == "curtir.letras." ||
            strText == "tocar.musica.muitobem" ||
            strText == unitext("início") ||
            strText == unitext("início ") ||
            strText == "chopinzinho." ||
            strText == "curitiba." ||
            strText == "saojoao." ||
            strText == "santos.")
         {
            //if(image_cache_start('krftimage', image_cache_get_path1($_GET['mb'])))
            //   return true;
            //require_once "mbimage.ds";
            size    = strlen(get("s")) == 0 ? "b" : get("s");
            bk      = strlen(get("bk")) == 0 ? "mb" : get("bk");
            mbimage_outputImage(strText, size, bk);
            //image_cache_end('krftimage', image_cache_get_path1($_GET['mb']));
            //      break;
         }
         else if(strText == "bk-h40")
         {
            //if(image_cache_start('krftimage', image_cache_get_path1($_GET['mb'])))
            //   return true;
            //require_once "mbimage.ds";
            mbimage_rectImage(10, 40);
            //image_cache_end('krftimage', image_cache_get_path1($_GET['mb']));
         }
         //default:
         //   break;
         //}
      }
      else if(get("bellmusic").get_string().has_char())
      {
         string strText = urldecode(get("bellmusic"));
         if(strText == "ackorden veribell" ||
            strText == unitext("sångtexter veribell") ||
            strText == "lyrics veribell" ||
            strText == "letras veribell" ||
            strText == "liedtexte veribell" ||
            strText == "it/lyrics veribell" ||
            strText == "paroles veribell" ||
            strText == "liricas veribell" ||
            strText == "tekstypiosenek veribell" ||
            strText == "kantotekstoj veribell" ||
            strText == unitext("лирику veribell") ||
            strText == "songteksten veribell" ||
            strText == "akkorde veribell" ||
            strText == "chords veribell" ||
            strText == "cifras veribell" ||
            strText == "acordes veribell" ||
            strText == "kordoj veribell" ||
            strText == "accords veribell" ||
            strText == "accordi veribell" ||
            strText == "akkoorden veribell" ||
            strText == "struna veribell" ||
            strText == unitext("аккорды veribell"))
         {
            //if(image_cache_start('krftimage', image_cache_get_path1($_GET['bellmusic'])))
            //   return true;
            //require_once "bellmusic_image.ds";
            size    = strlen(get("s")) == 0 ? "b" : get("s");
            bk      = strlen(get("bk")) == 0 ? "w" : get("bk");
            //bellmusic_image_outputImage(strText, size, bk);
            //image_cache_end('krftimage', image_cache_get_path1($_GET['bellmusic']));
            //         break;
         }
         else if(strText == unitext("歌詞 veribell") ||
            strText == unitext("コード veribell"))
         {
            //if(image_cache_start('krftimage', image_cache_get_path1($_GET['bellmusic'])))
            //   return true;
            //require_once "bellmusic_image.ds";
            size    = strlen(get("s")) == 0 ? "b" : get("s");
            bk      = strlen(get("bk")) == 0 ? "w" : get("bk");
            //         bellmusic_image_outputImage($text, $size, $bk, 'ja');
            //image_cache_end('krftimage', image_cache_get_path1($_GET['bellmusic']));
            //         break;
         }
         else if(strText == unitext("歌词 veribell") ||
            strText == unitext("调和 veribell"))
         {
            //         if(image_cache_start('krftimage', image_cache_get_path1($_GET['bellmusic'])))
            //            return true;
            //         require_once "bellmusic_image.ds";
            size    = strlen(get("s")) == 0 ? "b" : get("s");
            bk      = strlen(get("bk")) == 0 ? "w" : get("bk");
            //         bellmusic_image_outputImage($text, $size, $bk, 'cn');
            //         image_cache_end('krftimage', image_cache_get_path1($_GET['bellmusic']));
            //         break;
         }
         else if(strText ==unitext("가사 veribell") ||
            strText == unitext("화음 veribell"))
         {
            //if(image_cache_start('krftimage', image_cache_get_path1($_GET['bellmusic'])))
            //   return true;
            //require_once "bellmusic_image.ds";
            size    = strlen(get("s")) == 0 ? "b" : get("s");
            bk      = strlen(get("bk")) == 0 ? "w" : get("bk");
            //         bellmusic_image_outputImage($text, $size, $bk, 'kr');
            //image_cache_end('krftimage', image_cache_get_path1($_GET['bellmusic']));
            //break;
         }
         else if(strText == "bk-h40")
         {
            //if(image_cache_start('krftimage', image_cache_get_path1($_GET['bellmusic'])))
            //   return true;
            //require_once "mbimage.ds";
            //mbimage_rectImage(10, 40);
            //image_cache_end('krftimage', image_cache_get_path1($_GET['bellmusic']));
         }
      }
      //return true;
   }

   void script_impl::mbimage_outputImage(
      var text,
      var size,
      var bk)
   {
      ///global $g_ds_common_root;
      header("Cache-control: max-age=1, no-store, no-cache, must-revalidate");
      header ("Content-type: image/png");
      //$fontpath = realpath("$g_ds_common_root/internal/font/HUM777K.TTF");
      var fontsize;
      if(size == "ss")
      {
         fontsize = 12;
      }
      else if(size == "s")
      {
         fontsize = 18;
      }
      else
      {
         fontsize = 24;
      }
      ::ca::font_sp font(get_app());
      LOGFONT lf;
      memset(&lf, 0, sizeof(lf));

      strcpy(lf.lfFaceName, "Humnst777 Blk BT"); // Humanist 777 Black BT
      lf.lfHeight = 16 * fontsize.get_integer() / 11;
      lf.lfWeight = FW_HEAVY;
      lf.lfPitchAndFamily = FF_SWISS;
      font->CreateFontIndirectA(&lf);

      ::ca::graphics_sp spgraphics(get_app());
      spgraphics->CreateCompatibleDC(NULL);
      spgraphics->SelectObject(font);
      class size sizeText = spgraphics->GetTextExtent(text.get_string());
      ::ca::dib_sp spdib(get_app());
      spdib->create(sizeText.cx + 1, sizeText.cy + 1);

      //$box = imageftbbox($fontsize, 0, $fontpath, $text);
      //dprint('box=');
      //dprint_r($box);
      //$w = $box[4] + $box[0] + 1;
      //$h = 40;
      //      $im = @imagecreatetruecolor($w, $h)
      //        or die("Cannot Initialize new GD image stream");
      //      $maxy = $h - 1;
      //      $maxx = $w - 1;
      COLORREF back_color;
      if(bk == "mb")
      {
         back_color = RGB(255, 255, 204);
      }
      else
      {
         //ca = color_array($bk);
         //dprint_r($ca);
         back_color = color(bk); //imagecolorallocate($im, $ca[0], $ca[1], $ca[2]);
      }
      //imagefilledrectangle($im, 0, 0, $w, $h, $back_color);
      spdib->get_graphics()->FillSolidRect(0, 0, sizeText.cx+1, sizeText.cy+1, back_color);

      COLORREF color = RGB(125, 226, 120);

      spdib->get_graphics()->SetTextColor(color);
      spdib->get_graphics()->SetBkMode(TRANSPARENT);

      // add some shadow to the text
      //imagefttext($im, $fontsize, 0, $box[0], 31, $color, $fontpath, $text);
      spdib->get_graphics()->TextOut(0, 0, text.get_string());

      // add the text
      //      $font = imageloadfont('/home/veriwell/php/common/karaoke/almosnow.gdf');
      //imagestring($im, $font, 5, 5,  $code, $text_color);
      image_png(spdib->get_bitmap());
      //imagedestroy($im);
   }

   void script_impl::mbimage_rectImage(var w, var h)
   {
      header("Cache-control: max-age=1, no-store, no-cache, must-revalidate");
      header("Content-type: image/png");

      ::ca::dib_sp spdib(get_app());
      spdib->create(w, h);

      spdib->get_graphics()->FillSolidRect(0, 0, w, h, RGB(255, 255, 204));

      image_png(spdib->get_bitmap());
   }

   COLORREF script_impl::color(var c)
   {
      if(c == "w")
      {
         return RGB(255, 255, 255);
      }
      else
      {
         TRACE("script_impl::color unknwon color code");
      }
      return 0;
   }

   void script_impl::image_png(::ca::bitmap_sp pbitmap)
   {
      outheader("Content-type") = "image/png";
      FIMEMORY * pfm1 = FreeImage_OpenMemory();
      FIBITMAP * pfi7 = System.imaging().HBITMAPtoFI(pbitmap);
      FIBITMAP * pfi8 = FreeImage_ConvertTo32Bits(pfi7);
      FreeImage_SaveToMemory(FIF_PNG, pfi8, pfm1 , PNG_DEFAULT);
      BYTE * pbData;
      DWORD dwSize;
      FreeImage_AcquireMemory(pfm1, &pbData, &dwSize);
      netnode().response().file().write(pbData, dwSize);

      FreeImage_CloseMemory(pfm1);
      FreeImage_Unload(pfi8);
      FreeImage_Unload(pfi7);
   }

   void script_impl::hand_extension()
   {
      string strFile;
      string strUri = gprop("request");
      strsize iFind = strUri.find("?");
      string strObject;
      if(iFind >= 0)
         strObject = strUri.Left(iFind);
      else
         strObject = strUri;
      gen::str::begins_eat(strObject, "/");
      string strExtension = System.file().extension(strObject);
      gen::str::ends_eat_ci(strObject, "." + strExtension);
      strFile = sys_get_include_path(strExtension, strExtension, strObject);
      if(strFile.is_empty())
      {
         strFile = sys_get_include_path(strExtension, "doc", strObject);
      }
      if(strFile.has_char())
      {
         dprint("strExtension=");
         dprint(strExtension);

         bool bCacheable = false;

         if(!strcasecmp(strExtension, "ico"))
         {
            header("Content-type: image/x-icon");
            bCacheable = true;
         }
         else if(!strcasecmp(strExtension, "ogv"))
         {
            header("Content-type: video/ogg");
            bCacheable = true;
         }
         else if(!strcasecmp(strExtension, "png"))
         {
            header("Content-type: image/png");
            bCacheable = true;
         }
         else if(!strcasecmp(strExtension, "jpg"))
         {
            header("Content-type: image/jpeg");
            bCacheable = true;
         }
         else if(!strcasecmp(strExtension, "jpeg"))
         {
            header("Content-type: image/jpeg");
            bCacheable = true;
         }
         else if(!strcasecmp(strExtension, "gif"))
         {
            header("Content-type: image/gif");
            bCacheable = true;
         }
         else if(!strcasecmp(strExtension, "xpi"))
         {
            header("Content-type: application/x-xpinstall");
         }
         else if(!strcasecmp(strExtension, "rdf"))
         {
            header("Content-type: text/xml");
         }

         array_ptr_alloc < int_array > rangea;
         if(strlen(inheader("range")) > 0)
         {
            stringa straItem;
            straItem.explode("=", inheader("range"));
            if(straItem.get_count() == 2)
            {
               string strUnit = straItem[0];
               stringa stra;
               stra.explode(",", straItem[1]);
               for(int i = 0; i < stra.get_count(); i++)
               {
                  stringa straRange;
                  straRange.explode("-", stra[i]);
                  if(straRange.get_count() == 2)
                  {
                     rangea.add(int_array());
                     rangea.last_element().add(atoi(straRange[0]));
                     straRange[1].trim();
                     if(strlen(straRange[1]) == 0)
                        rangea.last_element().add(-1);
                     else
                        rangea.last_element().add(atoi(straRange[1]));
                  }
               }
            }
         }

         if(bCacheable)
         {
            
            outheader("Cache-control") = "public, max-age=7257600";
            
            outheader("Pragma") = "public";

            m_pnetnodesocket->outheaders().remove_by_name("Set-Cookie");

            m_pnetnodesocket->response().cookies().remove_all();
         
            outheader("Expires") = System.http().gmdate(strtotime("+84 day"));

            outheader("ETag") = System.file36().md5(strFile);

            if(inheader("If-None-Match") == outheader("ETag"))
            {

               outattr("http_status_code") = 304;
               outattr("http_status") = "Not Modified";

            }
            else
            {
               
               read_file(strFile, &rangea);

            }


         }
         else
         {

            read_file(strFile, &rangea);

         }

      }
   }

   void script_impl::ensure_ssl()
   {
      if(!m_pinstanceMain->m_pnetnodesocket->IsSSL())
      {
         header("Location: https://" + inheader("host").get_string() + inattr("request_uri"));
         exit(0);
      }
   }

   bool script_impl::is_common_dir(const char * psz)
   {
      string str(psz);
      if(str == "css")
         return true;
      else if(str == "image")
         return true;
      return false;
   }

   string script_impl::locale_from_request_v1(const char * psz)
   {
      string strRequest;
      if(psz != NULL && strlen(psz) > 0)
      {
         strRequest = psz;
      }
      else
      {
         strRequest = inattr("request_uri");
      }
      strsize iFindQuestion = strRequest.find("?");
      strsize iFindSlash = strRequest.find("/", 1);
      string strCandidate;
      if(iFindQuestion > 0)
      {
         if(iFindSlash > 0 && iFindSlash < iFindQuestion)
         {
            strCandidate = strRequest.Mid(1, iFindSlash - 1);
         }
      }
      else
      {
         if(iFindSlash > 0)
         {
            strCandidate = strRequest.Mid(1, iFindSlash - 1);
         }
      }
      if(strCandidate.has_char() && !is_common_dir(strCandidate))
      {
         return strCandidate;
      }
      return "_std";
   }



   bool script_impl::locale_is_eu(const char * pszLocale)
   {
      return System.url().locale_is_eu(pszLocale);
   }

   bool script_impl::locale_is_asia(const char * pszLocale)
   {
      return System.url().locale_is_asia(pszLocale);
   }

   bool script_impl::locale_is_south_america(const char * pszLocale)
   {
      return System.url().locale_is_south_america(pszLocale);
   }

   bool script_impl::locale_is_north_america(const char * pszLocale)
   {
      return System.url().locale_is_north_america(pszLocale);
   }

   bool script_impl::locale_is_central_america(const char * pszLocale)
   {
      return System.url().locale_is_central_america(pszLocale);
   }

   bool script_impl::locale_is_latin_america(const char * pszLocale)
   {
      return System.url().locale_is_latin_america(pszLocale);
   }

   bool script_impl::locale_is_de(const char * pszLocale)
   {
      string strLocale(pszLocale);
      if(strLocale == "be"
         || strLocale == "ch"
         || strLocale == "at"
         || strLocale == "de"
         || strLocale == "lu"
         || strLocale == "at")
      {
         return true;
      }
      return false;
   }
   bool script_impl::locale_is_es(const char * pszLocale)
   {
      string strLocale(pszLocale);
      if(strLocale == "es"
         || strLocale == "mx"
         || strLocale == "co"
         || strLocale == "ar"
         || strLocale == "cl"
         || strLocale == "ve"
         || strLocale == "bo"
         || strLocale == "cu"
         || strLocale == "ec"
         || strLocale == "pa"
         || strLocale == "uy"
         || strLocale == "pe")
      {
         return true;
      }
      return false;
   }


   var script_impl::explode(const char * pszDelimiter, const char * psz)
   {
      var var;
      var.stra().explode(pszDelimiter, psz);
      return var;
   }

   string script_impl::implode(const char * pszGlue, const var & var)
   {
      return var.implode(pszGlue);
   }

   string script_impl::implode(const char * pszGlue, const stringa & stra)
   {
      return stra.implode(pszGlue);
   }

   bool script_impl::in_array(const var & varElement, const var & varArray)
   {
      return varArray.array_contains(varElement.get_string());
   }

   void script_impl::array_unshift(var & varArray, const var varElement)
   {
      varArray.vara().insert_at(0, varElement);
   }

   var script_impl::array(
      var var01,
      var var02,
      var var03,
      var var04,
      var var05,
      var var06,
      var var07,
      var var08,
      var var09,
      var var10,
      var var11,
      var var12,
      var var13,
      var var14,
      var var15,
      var var16,
      var var17,
      var var18,
      var var19,
      var var20,
      var var21,
      var var22,
      var var23,
      var var24,
      var var25)
   {
      var var;
      var.vara().add(var01);
      var.vara().add(var02);
      var.vara().add(var03);
      var.vara().add(var04);
      var.vara().add(var05);
      var.vara().add(var06);
      var.vara().add(var07);
      var.vara().add(var08);
      var.vara().add(var09);
      var.vara().add(var10);
      var.vara().add(var11);
      var.vara().add(var12);
      var.vara().add(var13);
      var.vara().add(var14);
      var.vara().add(var15);
      var.vara().add(var16);
      var.vara().add(var17);
      var.vara().add(var18);
      var.vara().add(var19);
      var.vara().add(var20);
      var.vara().add(var21);
      var.vara().add(var22);
      var.vara().add(var23);
      var.vara().add(var24);
      var.vara().add(var25);
      return var;
   }


   bool script_impl::is_array(var var)
   {
      return var.is_array();
   }

   var script_impl::array_merge(
      var var01,
      var var02,
      var var03,
      var var04,
      var var05,
      var var06,
      var var07,
      var var08,
      var var09,
      var var10,
      var var11,
      var var12,
      var var13,
      var var14,
      var var15,
      var var16,
      var var17,
      var var18,
      var var19,
      var var20,
      var var21,
      var var22,
      var var23,
      var var24,
      var var25)
   {
      var var;
      var.vara().add(var01.vara());
      var.vara().add(var02.vara());
      var.vara().add(var03.vara());
      var.vara().add(var04.vara());
      var.vara().add(var05.vara());
      var.vara().add(var06.vara());
      var.vara().add(var07.vara());
      var.vara().add(var08.vara());
      var.vara().add(var09.vara());
      var.vara().add(var10.vara());
      var.vara().add(var11.vara());
      var.vara().add(var12.vara());
      var.vara().add(var13.vara());
      var.vara().add(var14.vara());
      var.vara().add(var15.vara());
      var.vara().add(var16.vara());
      var.vara().add(var17.vara());
      var.vara().add(var18.vara());
      var.vara().add(var19.vara());
      var.vara().add(var20.vara());
      var.vara().add(var21.vara());
      var.vara().add(var22.vara());
      var.vara().add(var23.vara());
      var.vara().add(var24.vara());
      var.vara().add(var25.vara());
      return var;
   }

   var script_impl::http_accept_language(const char * psz)
   {
      string str(psz);
      if(str.is_empty())
      {
         str = inheader("Accept-Language");
      }
      str.make_lower();
      dprint(str);
      stringa stra;
      stra.explode(",", str);
      var a;
      for(int i = 0; i < stra.get_count(); i++)
      {
         string item = stra[i];
         stringa itema;
         itema.explode(";", item);
         if(itema.get_count() == 1)
         {
            gen::property_set b;
            b["lang"] = itema[0];
            b["q"] =1.0;
            a.vara().add((var)b);
         }
         else
         {
            stringa qa;
            qa.explode("=", itema[1]);
            string q = qa[1];
            gen::property_set b;
            b["lang"] = itema[0];
            b["q"] = atof(q);
            a.vara().add((var) b);
         }
      }
      return a;
   }

   string script_impl::normalize_locale(const char * psz)
   {
      string str(psz);
      strsize iPos = str.reverse_find("-");
      if(iPos >= 0)
         str = str.Mid(iPos + 1);
      str.make_lower();
      if(str == "sv")
      {
         str = "se";
      }
      return str;
   }


   ::count script_impl::count(var var)
   {
      
      if(var.is_array())
         return var.array_get_count();
      else
         return 0;

   }


   var script_impl::emphasize_text(var textArg, var emphasizeArg)
   {
      string text = textArg;
      string emphasize = emphasizeArg;
      string emphasizeL(emphasize);
      emphasizeL.make_lower();

      strsize pos = 0;
      strsize empLen = emphasize.get_length();
      while(true)
      {
         string textL(text);
         textL.make_lower();
         if((pos = textL.find(emphasizeL, pos)) < 0)
            break;
         string newtext = text.Left(pos);
         string emp = "<strong>" + text.Mid(pos, empLen) + "</strong>";
         newtext += emp;
         newtext += text.Mid(pos + empLen);
         text = newtext;
         pos = pos + empLen;
      }
      return text;
   }



   double sqr(double d)
   {
      return d*d;
   }

   string script_impl::spa_calc_best_host()
   {
      string strVersion;
      strVersion = get("version");
      string strHost;
      if(strVersion.CompareNoCase("basis") == 0)
      {
         strHost = Application.file().as_string(System.dir().path(get_manager()->m_strNetnodePath, "net/system/basis_spa_host.txt"));
      }
      else
      {
         strHost = Application.file().as_string(System.dir().path(get_manager()->m_strNetnodePath, "net/system/stage_spa_host.txt"));
      }
      dprint("<pre>host:\n");
      dprint(strHost);
      dprint("</pre>");
      stringa straLastHost;
      if(isset(request("last_host")))
      {
         straLastHost.explode(";", request("last_host"));
      }
      stringa straHost;
      straHost.explode("\r\n", strHost);
      double dMinDistance = 10000000000000.0;
      GeoIP * pgeoip = GeoIP_open(System.dir().path(get_manager()->m_strNetnodePath, "net/system/geoip/GeoLiteCity.dat"), GEOIP_STANDARD);
      string best_host;
      for(int i = 0; i < straHost.get_count(); i++)
      {
         string h = straHost[i];
         h.trim();
         if(h.is_empty())
            continue;
         stringa stra;
         stra.explode("=", h);
         strHost = stra[0];
         string strIp;
         strIp = stra[1];
         //$ca = explode(".", $host);
         //$c = $ca[count($ca) - 1];

         if(straLastHost.contains(strHost))
            continue;

         GeoIPRecord * pr = GeoIP_record_by_addr(pgeoip, strIp);
         GeoIPRecord * prClient = get_client_geopip_record();
         if(pr != NULL && prClient != NULL)
         {
            double distance = sqrt(sqr(prClient->latitude - pr->latitude) + sqr(prClient->longitude - pr->longitude));
            //dprint ("<br>ip=$ip<br>host=$host<br>$distance<br>geoip_latitude={$geoip_record->latitude}<br>geoip_longitude={$geoip_record->longitude}<br>host_latitude={$r->latitude}<br>host_longitude={$r->longitude}<br>");
            if(distance < dMinDistance)
            {
               dMinDistance = distance;
               best_host = strHost;
            }
         }
         else if(best_host.is_empty())
         {
            best_host = strHost;
         }
      }
      GeoIP_delete(pgeoip);
      return best_host;
   }


   GeoIPRecord * script_impl::get_client_geopip_record()
   {
      if(m_pinstanceMain->m_pgeoiprecordClient != NULL)
         return m_pinstanceMain->m_pgeoiprecordClient;
      GeoIP * pgeoip = GeoIP_open(System.dir().path(get_manager()->m_strNetnodePath, "net/geoip/GeoLiteCity.dat"), GEOIP_STANDARD);
      if(pgeoip == NULL)
         return NULL;
      m_pinstanceMain->m_pgeoiprecordClient = GeoIP_record_by_addr(pgeoip, inattra()["remote_addr"]);
      GeoIP_delete(pgeoip);
      return m_pinstanceMain->m_pgeoiprecordClient;
   }


   void script_impl::echo_simage(var param_view)
   {
      echo(simage(param_view));
   }

   string script_impl::simage(var param_view)
   {
      string base_url = "http://laborserver.net/";
      return base_url + simage_relative(param_view);
   }

   void script_impl::simage_add_accepta(webserver::simage_accepta * paccepta)
   {
      get_manager()->m_psimageptra->add(paccepta);
   }

   string script_impl::simage_relative(var param_view)
   {
      webserver::simage_accepta_ptr_array * psimageptra = get_manager()->m_psimageptra;
      string candidate;
      string path;
      for(index i = 0; i < psimageptra->get_count(); i++)
      {
         webserver::simage_accepta * paccepta = psimageptra->element_at(i);
         
         
         strsize iFind = paccepta->m_propsetAccept.find_value_ci(param_view);
         gprop("root") = paccepta->m_root;
         if(isset(param_view) && iFind >= 0)
         {
            gprop("title") = paccepta->m_propsetAccept.m_propertya[iFind].name();
            candidate = eval("image/{$root}/{$g_langdir_modifier}{$title}");
            path = real_path(candidate);
            if(path.is_empty())

               candidate = eval("image/{$root}/{$param_style}/{$title}");
            path = real_path(candidate);
            if(path.is_empty())
            {
               candidate = eval("image/{$root}/{$title}");
               path = real_path(candidate);
               if(path.is_empty())
               {
                  candidate = eval("image/{$g_langdir_modifier}{$title}");
                  path = real_path(candidate);
                  if(path.is_empty())
                  {
                     candidate =  "image/{$title}";
                     path = real_path(candidate);
                  }
               }
            }
         }
         if(candidate.has_char())
            break;
            
      }
      return candidate;
   }




   bool script_impl::net_hostname_is_spider(const char * hostname, const char * useragent)
   {
      single_lock sl(&get_manager()->m_mutexSpider, TRUE);
      webserver::match_host_array * pptra = get_manager()->m_pmatchhostaSpider;
      for(int i = 0; i < pptra->get_count(); i++)
      {

/*         if(pptra->element_at(i).matches(hostname, useragent))
            return true;*/
      }
      return false;
   }


   void script_impl::add_spider_match_host(const char * hostname, const char * useragent)
   {
      single_lock sl(&get_manager()->m_mutexSpider, TRUE);
      webserver::match_host_array * pptra = get_manager()->m_pmatchhostaSpider;
      pptra->add(new webserver::match_host(hostname, useragent));

   }

   string script_impl::ca2_html_fluidcode(const char * pszFreeText)
   {
      stringa stra;
      string strQuery(pszFreeText);
      if(cregexp_util::match(stra, strQuery, "/(^|[^a-zA-Z]+)de\\s*((\\d+)\\s*/\\s*(\\d+)\\s*a\\s*(\\d+)\\s*/\\s*(\\d+))(((\\d)?!)|$)/", true, 8) > 0)
      {
         class time time1;
         time1 = time::get_current_time();
         class time time2;
         time2 = time::get_current_time();
         int i1 = atoi(stra[3]);
         int i2 = atoi(stra[4]);
         int i3 = atoi(stra[5]);
         int i4 = atoi(stra[6]);
         int iCount = 0;
         bool bFirst = false;
         if(i1 != i2
            && i1 >= 1 && i1 <= 12
            && i2 >= 1 && i2 <=
            System.datetime().get_month_day_count(time1.GetYear(), i1)
            && i3 != i4
            && i3 >= 1 && i3 <= 12
            && i4 >= 1 && i4 <=
            System.datetime().get_month_day_count(time2.GetYear(), i3))
         {
            iCount++;
            bFirst = true;
         }
         if(i2 >= 1 && i2 <= 12
            && i1 >= 1 && i1 <=
            System.datetime().get_month_day_count(time1.GetYear(), i2)
            &&   i4 >= 1 && i4 <= 12
            && i3 >= 1 && i3 <=
            System.datetime().get_month_day_count(time2.GetYear(), i4))
         {
            iCount++;
         }
         string strLink;
         if(iCount == 1)
         {
            int iDay1;
            int iMonth1;
            int iDay2;
            int iMonth2;
            if(bFirst)
            {
               iDay1 = i2;
               iMonth1 = i1;
               iDay2 = i4;
               iMonth2 = i3;
            }
            else
            {
               iDay1 = i1;
               iMonth1 = i2;
               iDay2 = i3;
               iMonth2 = i4;
            }
            time1 = System.datetime().from_gmt_date_time(time1.GetGmtYear(), iMonth1, iDay1,
               time1.GetGmtHour(), time1.GetGmtMinute(), time1.GetGmtSecond());
            time2 = System.datetime().from_gmt_date_time(time2.GetGmtYear(), iMonth2, iDay2,
               time2.GetGmtHour(), time2.GetGmtMinute(), time2.GetGmtSecond());
            string strValue1 = System.datetime().international().get_local_date_time(time1);
            string strValue2 = System.datetime().international().get_local_date_time(time2);
            string strHref = "http://ca2.cc/calendar?query=" + urlencode(strValue1) + "&queryEnd=" + urlencode(strValue2);
            strLink = "<a href=\"" + a_go1(strHref) + "\">";
            strLink += stra[0];
            strLink += "</a>";
         }
         else if(iCount == 2)
         {
            string strHref = "http://ca2.cc/query?query=" + urlencode(stra[0]);
            strLink = "<a href=\"" + a_go1(strHref) + "\">";
            strLink += stra[0];
            strLink += "</a>";
         }
         if(strLink.has_char())
         {
            strQuery.replace(stra[0], strLink);
         }
      }
      return strQuery;
   }

   string script_impl::query_ca2(const char * pszQuery)
   {
      stringa straResp;
      string strResp;
      {
         var varRet = include("system/carlosgustavocecynlundgren/query_ca2_relay");
         if(varRet.get_string().get_length() > 0)
            return varRet.get_string();
      }
      {
         var varRet = include("system/carlosgustavocecynlundgren/query_ca2_include_pre");
         if(varRet.get_string().get_length() > 0)
            straResp.add(varRet.get_string());
      }

      string strQuery(pszQuery);
      bool bOk = false;
      strResp.Empty();
      try
      {
         calculator::parser parser(get_app());
         calculator::element * pelement = parser.parse(strQuery);
         string strValue = pelement->get_value().to_string();
         strResp = pelement->get_expression() + " = " + strValue  + "\n";
         bOk = true;
      }
      catch(const char * psz)
      {
         TRACE("script_impl::query_ca2 string exception 1 %s", psz);
         bOk = false;
      }
      try
      {
         stringa stra;
         if(cregexp_util::match(stra, strQuery, "/\\s*(((\\s?#1)de\\s+)?(\\d+)\\s*/\\s*(\\d+)\\s*a\\s*(\\d+)\\s*/\\s*(\\d+))(((\\d$)?!)|$)/", true, 8) > 0)
         {
            class time time1;
            time1 = time::get_current_time();
            class time time2;
            time2 = time::get_current_time();
            int i1 = atoi(stra[3]);
            int i2 = atoi(stra[4]);
            int i3 = atoi(stra[5]);
            int i4 = atoi(stra[6]);
            if(i1 != i2
               && i1 >= 1 && i1 <= 12
               && i2 >= 1 && i2 <=
               System.datetime().get_month_day_count(time1.GetYear(), i1)
               && i3 != i4
               && i3 >= 1 && i3 <= 12
               && i4 >= 1 && i4 <=
               System.datetime().get_month_day_count(time2.GetYear(), i3))
            {
               int iDay1 = i2;
               int iMonth1 = i1;
               int iDay2 = i4;
               int iMonth2 = i3;
               time1 = System.datetime().from_gmt_date_time(time1.GetGmtYear(), iMonth1, iDay1,
                  time1.GetGmtHour(), time1.GetGmtMinute(), time1.GetGmtSecond());
               time2 = System.datetime().from_gmt_date_time(time2.GetGmtYear(), iMonth2, iDay2,
                  time2.GetGmtHour(), time2.GetGmtMinute(), time2.GetGmtSecond());
               string strValue1 = System.datetime().international().get_gmt_date_time(time1);
               string strValue2 = System.datetime().international().get_gmt_date_time(time2);
               strResp = "<a href=\"calendar?query=" + urlencode(strValue1) + "&queryEnd=" + urlencode(strValue2) + "\">";
               strResp += stra[0];
               strResp += "</a>";
               straResp.add(strResp);
               strResp.Empty();
            }
            if(i2 >= 1 && i2 <= 12
               && i1 >= 1 && i1 <=
               System.datetime().get_month_day_count(time1.GetYear(), i2)
               &&   i4 >= 1 && 42 <= 12
               && i3 >= 1 && i3 <=
               System.datetime().get_month_day_count(time2.GetYear(), i4))
            {
               int iDay1 = i1;
               int iMonth1 = i2;
               int iDay2 = i3;
               int iMonth2 = i4;
               time1 = System.datetime().from_gmt_date_time(time1.GetGmtYear(), iMonth1, iDay1,
                  time1.GetGmtHour(), time1.GetGmtMinute(), time1.GetGmtSecond());
               time2 = System.datetime().from_gmt_date_time(time2.GetGmtYear(), iMonth2, iDay2,
                  time2.GetGmtHour(), time2.GetGmtMinute(), time2.GetGmtSecond());
               string strValue1 = System.datetime().international().get_gmt_date_time(time1);
               string strValue2 = System.datetime().international().get_gmt_date_time(time2);
               strResp = "<a href=\"calendar?query=" + urlencode(strValue1) + "&queryEnd=" + urlencode(strValue2) + "\">";
               strResp += stra[0];
               strResp += "</a>";
               straResp.add(strResp);
               strResp.Empty();
            }
         }
      }
      catch(...)
      {
      }

      if(strResp.get_length() > 0)
         straResp.add(strResp);
      strResp.Empty();
      try
      {
         stringa stra;
         if(cregexp_util::match(str_context(), stra, strQuery, "/(.+)\\s*%1/", "calendar:seconds"))
         {

            calculator::parser parser(get_app());
            calculator::element * pelement = parser.parse(stra[1]);
            string strValue;
            strValue.Format("%f", pelement->get_value());
            string strMinFrac;
            strMinFrac.Format("%f", pelement->get_value().mod() / 60.0);
            string strMin;
            strMin.Format("%d", ((int)pelement->get_value().mod() / 60));
            string strMinSec;
            strMinSec.Format("%f", fmod(pelement->get_value().mod(), 60.0));
            strResp = pelement->get_expression() + " " + str_context()->get("calendar:seconds");
            strResp += " = " + strValue + " " + str_context()->get("calendar:seconds");
            strResp += " = " + strMinFrac + " " + str_context()->get("calendar:minutes");
            strResp += " = " + strMin + " " + str_context()->get("calendar:minutes") + " and " + strMinSec + " " + str_context()->get("calendar:seconds") + "\n";

            bOk = true;
         }
      }
      catch(const char * psz)
      {
         TRACE("script_impl::query_ca2 string exception 2 %s", psz);
         bOk = false;
      }
      if(strResp.get_length() > 0)
         straResp.add(strResp);
      strResp.Empty();
      try
      {
         stringa stra;
         if(cregexp_util::match(str_context(), stra, strQuery, "/(.+)\\s*%1/", "calendar:days"))
         {

            calculator::parser parser(get_app());
            calculator::element * pelement = parser.parse(stra[1]);
            string strSignal;
            if(pelement->get_value().mod() >= 0)
               strSignal = "+";
            else
               strSignal = "-";
            int iPathCount;
            class time time = System.datetime().strtotime(
               str_context(),
               strSignal + " " + gen::str::itoa((int) pelement->get_value().mod()) + " days",
               0, iPathCount);
            string strValue = System.datetime().international().get_gmt_date_time(time);
            strResp = "<a href=\"calendar?query=" + urlencode(strSignal + " " + gen::str::itoa((int) pelement->get_value().mod())) + "+days"+"\">";
            strResp += pelement->get_expression() + " dias = " + strValue;
            strResp += "</a>";
            straResp.add(strResp);
            strResp.Empty();
            ca2_query_date(straResp, time, strQuery, strQuery);
            bOk = true;
         }
      }
      catch(const char * psz)
      {
         TRACE("script_impl::query_ca2 string exception 3 %s", psz);
         bOk = false;
      }
      if(strResp.get_length() > 0)
      straResp.add(strResp);
      strResp.Empty();
      try
      {
         stringa stra;
         if(cregexp_util::match(stra, strQuery, "/\\s*((\\d+)\\s*/\\s*(\\d+))(((\\d)?!)|$)/", true, 4) > 0)
         {
            class time time;
            time = time::get_current_time();
            int i1 = atoi(stra[2]);
            int i2 = atoi(stra[3]);
            if(i1 != i2
               && i1 >= 1 && i1 <= 12
               && i2 >= 1 && i2 <=
               System.datetime().get_month_day_count(time.GetYear(), i1))
            {
               int iDay = i2;
               int iMonth = i1;
               time = System.datetime().from_gmt_date_time(time.GetGmtYear(), iMonth, iDay,
                  time.GetGmtHour(), time.GetGmtMinute(), time.GetGmtSecond());
               string strValue = System.datetime().international().get_gmt_date_time(time);
               strResp = "<a href=\"calendar?query=" + urlencode(strValue) + "\">";
               strResp += strQuery + "=" + gen::str::itoa(iDay) + ", " + pstr("datetimestr_month["+gen::str::itoa(iMonth - 1)+"]");
               strResp += "</a>";
               straResp.add(strResp);
               strResp.Empty();
               ca2_query_date(straResp, time, stra[1], strQuery);
            }
            if(i2 >= 1 && i2 <= 12
               && i1 >= 1 && i1 <=
               System.datetime().get_month_day_count(time.GetYear(), i2))
            {
               int iDay = i1;
               int iMonth = i2;
               time = System.datetime().from_gmt_date_time(time.GetGmtYear(), iMonth, iDay,
                  time.GetGmtHour(), time.GetGmtMinute(), time.GetGmtSecond());
               string strValue = System.datetime().international().get_gmt_date_time(time);
               strResp = "<a href=\"calendar?query=" + urlencode(strValue) + "\">";
               strResp += strQuery + "=" + gen::str::itoa(iDay) + ", " + pstr("datetimestr_month["+gen::str::itoa(iMonth - 1)+"]");
               strResp += "</a>";
               straResp.add(strResp);
               strResp.Empty();
               ca2_query_date(straResp, time, stra[1], strQuery);
            }
         }
      }
      catch(const char * psz)
      {
         TRACE("script_impl::query_ca2 string exception 4 %s", psz);
         bOk = false;
      }
      if(strResp.get_length() > 0)
         straResp.add(strResp);
      try
      {
         int iPath = 0;
         int iPathCount = 0;
         do
         {
            class time t(System.datetime().strtotime(str_context(), strQuery, iPath, iPathCount));
            string strValue = System.datetime().international().get_gmt_date_time(t);
            strResp = "<a href=\"calendar?query=" + urlencode(strValue) + "\">";
            strResp += strValue;
            strResp += "</a>";
            straResp.add(strResp);
            strResp.Empty();
            iPath++;
         } while (iPath < iPathCount);

      }
      catch(const char * psz)
      {
         TRACE("script_impl::query_ca2 string exception 5 %s", psz);
      }
      catch(...)
      {
      }



      try
      {
         int iPath = 0;
         int iPathCount;
         int i;
         do
         {


            datetime::parser parser(get_app(), str_context());
            datetime::element * pelement = parser.parse(strQuery);
            string strValue;
            iPathCount = 1;
            i = iPath;
            strValue.Format("%s", pelement->get_value(get_app(), str_context(), i = iPath, iPathCount).to_string(get_app(), str_context()));
            iPathCount = 1;
            i = iPath;
            strResp = pelement->get_expression(get_app(),  str_context(), i, iPathCount) + " = " + strValue  + "\n";
            straResp.add(strResp);
            iPath++;
         } while (iPath < iPathCount);

      }
      catch(const char * psz)
      {
         TRACE("script_impl::query_ca2 string exception 6 %s", psz);
      }
      catch(...)
      {
      }
      strResp.Empty();
      
      if(straResp.get_count() == 0)
      {
         strResp = "ca2 computing could not answer you by this mean. Don't know! Expect and hope to say finally too: yet!!";
      }
      else
      {
         strResp = straResp.implode("<br><br>");
      }
      return strResp;
   }


   void script_impl::ca2_query_date(stringa & straResp, class time & time, const char * pszDateTime, const char * pszQuery)
   {
      string strDateTime(pszDateTime);
      string strValue = System.datetime().international().get_gmt_date_time(time);
      string strQuery(pszQuery);
      string strRemark;
      string strTopic;
      string strResp;

      int iDay = time.GetDay();
      int iMonth = time.GetMonth();


      stringa straCandidate;
      str_context()->get(straCandidate, "calendar_event:birthday");
      for(int i = 0; i < straCandidate.get_count(); i++)
      {
         strTopic = pszQuery;
         strTopic = gen::str::replace_ci(straCandidate[i], "", strTopic);
         strRemark = pszQuery;
         strResp = "<a href=\"calendar?query=" + urlencode(strValue) + "&appointment=birthday&topic=" + strTopic + "&remark=" + strRemark + "\">";
         strResp += straCandidate[i] + gen::str::itoa(iDay) + ", " + pstr("datetimestr_month["+gen::str::itoa(iMonth - 1)+"]");
         strResp += ", ";
         strResp += htmlentities(strTopic);
         strResp += "</a>";
         straResp.add(strResp);
      }

      strTopic = pszQuery;
      strTopic = gen::str::replace_ci(pszDateTime, "", strTopic);
      strRemark = pszQuery;
      strResp = "<a href=\"calendar?query=" + urlencode(strValue) + "&appointment=event&topic=" + strTopic + "&remark=" + strRemark + "\">";
      strResp += unitext("Evento: ") + gen::str::itoa(iDay) + ", " + pstr("datetimestr_month["+gen::str::itoa(iMonth - 1)+"]");
      strResp += ", ";
      strResp += htmlentities(strTopic);
      strResp += "</a>";
      straResp.add(strResp);
   }


   script_interface & script_impl::main()
   {
      return *m_pinstanceMain;
   }


   ::user::str_context * script_impl::str_context()
   {
      if(this == m_pinstanceMain)
         return m_puistrcontext;
      else
         return m_pinstanceMain->str_context();
   }


   void script_impl::print_email_body(::mail::pop3 * ppop3)
   {

      gen::property_set & set = ppop3->m_setHeaders;
      if(set["content-type"].get_string().find("text/plain") >= 0 || set["content-type"].get_string().is_empty())
      {
         print_plain_text_email_body(ppop3);
      }
      else
      {
         print(ppop3->m_strBody);
      }
   }

   void script_impl::print_plain_text_email_body(::mail::pop3 * ppop3)
   {

      string str(ppop3->m_strBody);
      strsize iFind = 0;
      strsize iStart;
      while(true)
      {
         strsize iFindHttp = str.find("http://", iFind);
         strsize iFindHttps = str.find("https://", iFind);
         strsize iFindWww = str.find("www.", iFind);
         if(iFindHttp == -1)
            iFindHttp = str.get_length();
         if(iFindHttps == -1)
            iFindHttps = str.get_length();
         if(iFindWww == -1)
            iFindWww = str.get_length();
         iFind = min(iFindWww, min(iFindHttp, iFindHttps));
         if(iFind == str.get_length())
            break;
         if(iFind > 0 && !isspace(str[iFind - 1]))
         {
            iFind++;
            continue;
         }
         iStart = iFind;
         while(!isspace(str[iFind]) && iFind < str.get_length())
         {
            iFind++;
         }
         string strLink = str.Mid(iStart, iFind - iStart);
         string strA;
         if(iFindWww == iFind)
            strA = "<a href=\"http://" + strLink + "\" >" + strLink + "</a>";
         else
            strA = "<a href=\"" + strLink + "\" >" + strLink + "</a>";
         str = str.Left(iStart) + strA + str.Mid(iFind);
         iFind = iStart + strA.get_length();
      }
      str.replace("\r\n", "<br>");
      str.replace("\n\r", "<br>");
      str.replace("\r", "<br>");
      str.replace("\n", "<br>");
      print(str);
   }


   void script_impl::ob_start()
   {
      m_pinstanceMain->m_memoryfileaPrint.add_new();
   }

   primitive::memory script_impl::ob_get_clean()
   {

      class primitive::memory memory = *m_pinstanceMain->m_memoryfileaPrint.last_element().get_primitive_memory();

      m_pinstanceMain->m_memoryfileaPrint.remove_at(m_pinstanceMain->m_memoryfileaPrint.get_upper_bound());

      return memory;

   }


   ::http::file & script_impl::output_file()
   {
      if(m_pinstanceMain != NULL && m_pinstanceMain->m_memoryfileaPrint.get_count() > 0)
      {
         return m_pinstanceMain->m_memoryfileaPrint.last_element();
      }
      else if(m_pnetnodesocket != NULL)
      {
         return m_pnetnodesocket->response().file();
      }
      else
      {
         return m_memoryfileFallback;
      }
   }


   void script_impl::filter_user_presence_status(const char * pszUserId)
   {

      string strSql;
      strSql.Format("SELECT `datetime` FROM cyncecc.user_presence_status WHERE `user` = '%s' ORDER BY `datetime` DESC LIMIT 1",
         pszUserId
         );

      
      var varDateTime = musicdb().query_item(strSql);

      if(varDateTime.is_empty())
      {
         string strUserTag = cynce().user_tag(pszUserId);
         // for telmico:
         // when status is set to predefined empty - 10000...0004 - or predefined offline - 10000...0025
         // it should delete other stati, currently. Because online status for example, support e.g., an
         // existing or to be created "in Miami beach" tag as coexisting tag_tag3 for user presence status
         strSql.Format("DELETE FROM tag_tag3 WHERE tag = '%s' AND tag2 = '1000000024'", strUserTag);
         cyncedb().query(strSql);
         if(is_licensed("user_presence", pszUserId))
         {
            cynce().tag_tag3_set(strUserTag, "1000000024", "1000000025");
         }
         else
         {
            cynce().tag_tag3_set(strUserTag, "1000000024", "1000000004");
         }
         return;
      }

      class time timeLastReportedActivity = strtotime(varDateTime + " UTC");
      class time timeNow = time::get_current_time();


      __int64 iTimeOut = 90;

      __int64 iDiff = timeNow.get_time() - timeLastReportedActivity.get_time();
      if(iDiff > iTimeOut)
      {
         string strUserTag = cynce().user_tag(pszUserId);
         // for telmico:
         // when status is set to predefined empty - 10000...0004 - or predefined offline - 10000...0025
         // it should delete other status, currently. Because online status for example, support e.g., an
         // existing or to be created "in Miami beach" tag as coexisting tag_tag3 for user presence status
         strSql.Format("DELETE FROM tag_tag3 WHERE tag = '%s' AND tag2 = '1000000024'", strUserTag);
         cyncedb().query(strSql);
         if(is_licensed("user_presence", pszUserId))
         {
            cynce().tag_tag3_set(strUserTag, "1000000024", "1000000025");
         }
         else
         {
            cynce().tag_tag3_set(strUserTag, "1000000024", "1000000004");
         }
         return;
      }
      else
      {
         string strUserTag = cynce().user_tag(pszUserId);
         strSql.Format("DELETE FROM tag_tag3 WHERE tag = '%s' AND tag2 = '1000000024' AND tag3 = '1000000025'", strUserTag);
         cyncedb().query(strSql);
         strSql.Format("DELETE FROM tag_tag3 WHERE tag = '%s' AND tag2 = '1000000024' AND tag3 = '1000000004'", strUserTag);
         cyncedb().query(strSql);
         cynce().tag_tag3_set(strUserTag, "1000000024", "1000000006");
      }

   }

   bool script_impl::is_licensed(const char * pszLicense, const char * pszUserId)
   {

      string strDateTimeNow = System.datetime().international().get_gmt_date_time();

      string strSql;
      strSql.Format("SELECT COUNT(*) FROM fun_license as l INNER JOIN fun_user_license as u ON u.user = '%s' AND '%s' > u.startdatetime AND IF(ISNULL(u.enddatetime), 1, '%s' < u.enddatetime) INNER JOIN fun_product as p ON p.id = u.product AND p.license = l.id WHERE l.strid = '%s'",
         pszUserId,
         strDateTimeNow,
         strDateTimeNow,
         pszLicense);

      var varCount = musicdb().query_item(strSql);

      return varCount.get_integer() >= 1;
   }

   string script_impl::web_map(const char * psz)
   {
      if(m_pinstanceMain == NULL || m_pinstanceMain == this)
      {
         string str(psz);
         str.replace("http://laborserver.net/image/", m_strImageWebSite);

         stringa straExtension;
         straExtension.add("css");
         straExtension.add("js");

         for(int i = 0; i < straExtension.get_count(); i++)
         {

            string strExtension = straExtension[i];

            int iFind;
            int iStart = 0;
            string strRet;
            string strScript;
            string strPath;
            string strFind = "http://laborserver.net/" + strExtension +"/";
            int iExtensionLen = strExtension.length();
            int iFindLen = strFind.length();
            while((iFind = str.find(strFind, iStart)) >= 0)
            {
               strRet += str.Mid(iStart, iFind - iStart);
               strRet += m_strCa2VFallbackWebSite + strExtension + "/";
               iStart = iFind + iFindLen;
               iFind = str.find("." + strExtension, iStart);
               if(iFind >= 0)
               {
                  iFind += iExtensionLen + 1;
                  strScript = "/" + strExtension + "/" + str.Mid(iStart, iFind - iStart);
                  if(strlen(gprop("param_site")) > 0)
                  {
                     strPath = System.dir().path(System.dir().path(get_manager()->m_strNetnodePath, "net-" + gprop("param_site"), "netseed/ds/ca2"), strScript);
                  }
                  if(strPath.is_empty() || !Application.file().exists(strPath))
                  {
                     strPath = System.dir().path(get_manager()->m_strNetseedPath, "ds\\ca2", strScript);
                  }
                  if(get_manager()->include_has_script(strPath))
                  {
                     // not cacheable.
                     // write it as it is.
                     strRet += str.Mid(iStart, iFind - iStart);
                  }
                  else
                  {
                     // cacheable.
                     // write it with md5.
                     string strMd5 = get_manager()->include_expand_md5(strPath);
                     bool bUpdate = false;
                     if(strMd5.is_empty())
                     {
                        strMd5 = System.crypt().md5(defer_expanded_file_string(strPath));
                        bUpdate = true;
                     }
                     strRet += str.Mid(iStart, iFind - iStart - iExtensionLen - 1);
                     strRet += ".";
                     strRet += strMd5;
                     strRet += "." + strExtension;
                     if(bUpdate)
                        get_manager()->set_include_expand_md5(strPath, strMd5);
                  }
               }
               else
               {
                  break;
               }
               iStart = iFind;
            }
            strRet += str.Mid(iStart);
            str = strRet;
         }

         str.replace("http://laborserver.net/file/", m_strFileWebSite);
         str.replace("http://laborserver.net/", m_strCa2VFallbackWebSite);
         str.replace("https://fontopus.com/", m_strCa2OpenWebSite);
         str.replace("http://fontopus.com/", m_strCa2OpenWebSite);

         stringa straReplacement;
         straReplacement.add("ca2inside");
         straReplacement.add("ca2based");

         stringa straSearch;
         straSearch.add("ca2inside");
         straSearch.add("ca2based");

         str = gen::str::random_replace(get_app(), straReplacement, straSearch, str);

         return str;
      }
      else
      {
         return m_pinstanceMain->web_map(psz);
      }
   }

   bool script_impl::is_site_user(var varSite, var varUser)
   {

      return musicdb().query_item("SELECT COUNT(*) FROM hi5.site_user WHERE site = " + varSite + " AND user = " + varUser) == 1;

   }

   bool script_impl::is_site_user(var varSite)
   {

      return is_site_user(varSite, gprop("secureuserid"));

   }


   bool script_impl::low_fs_read(const char * user, __int64 iFolder, const char * pszName)
   {
      string strSql;
      strSql.Format("SELECT `hash`, `key`, `size`, `mimetype`, `extension` FROM `fs`.`user_folder_item` WHERE `user` = '%s' AND `folder` = '%I64d' AND `name` = '%s'", user, iFolder, pszName);
      var row = musicdb().query_row(strSql);

      if(row.array_get_count() != 5)
         return false;

      if(get("extension").is_set() && get("extension").get_string().Mid(1).CompareNoCase(row.at(4)) != 0)
         return false;

      return low_fs_read((const char *) row.at(0),  (__int64) row.at(1), (__int64) row.at(2), (const char *) row.at(3), (const char *) row.at(4));
      return false;
   }

   bool script_impl::low_fs_write(const char * user, __int64 iFolder, const char * pszName)
   {
      return low_fs_write(user, iFolder, pszName, &m_pnetnodesocket->request().file(), inheader("Content-Type"));
   }

   bool script_impl::low_fs_write(const char * user, __int64 iFolder, const char * pszName, ::ex1::file * pfile, const char * pszContentType)
   {
      string strHash;
      pfile->seek_to_begin();
      strHash = System.file36().nessie(pfile);
      __int64 iKey = 1;
      __int64 iSize = pfile->get_length();
      string strMimeType = pszContentType;
      if(strMimeType.is_empty())
      {
         strMimeType = "unknown";
      }
      string strExtension = System.file().extension(pszName);
      if(strExtension.is_empty())
      {
         strExtension = "unknown";
      }

      string strName(pszName);

      string strPath = low_fs_file_path(strHash, iKey, iSize, strMimeType, strExtension);

      if(strPath.is_empty())
         return false;

      if(!Application.file().exists(strPath))
      {
         pfile->seek_to_begin();
         primitive::memory mem;
         mem.FullLoad(*pfile);
         Application.file().put_contents(strPath, mem);
      }

      if(!Application.file().exists(strPath))
         return false;


      low_fs_map_file(strPath); // ignore return collision

      return low_fs_add_user_file(user, iFolder, strHash, iKey, iSize, strMimeType, strExtension, strName, true);
   }

   string script_impl::low_fs_file_path(const char * hash, __int64 key, __int64 size, const char * mimetype, const char * extension, gen::property_set set)
   {


      string strSql;
      strSql.Format("SELECT `path` FROM `fs`.`item` WHERE `hash` = '%s' AND `key` = %I64d AND `size`=%I64d AND `mimetype` = '%s' AND `extension` = '%s'", hash, key, size, mimetype, extension);
      string strPath = musicdb().query_item(strSql);
      if(strPath.has_char())
         return strPath;

      if(!set["kar"].is_set())
      {
         strSql.Format("SELECT karfile.id FROM karfile WHERE nessie = '%s' AND `key` = '%d' AND `size` = '%I64d' AND mimetype = '%s' AND extension = '%s' LIMIT 1", hash, key, size, mimetype, extension);
         var row = musicdb().query_row(strSql);
         if(!row.is_empty())
         {
   
            ::webserver::music::song * psong = NULL;

   		   string fn = music().file_get_file_by_kar_file_id(row[0], &psong);
         
            var karfileid = psong->m_propertyset["topic"];
	
	         if(!musicdb().is_karfile_licensed(karfileid))
	         {
		
		         outattr("http_status_code") = "307";
		         outattr("http_status") ="Temporary Redirect";
		         outheader("ca2realm-x") = "not licensed: " + psong->m_propertyset["filename"];
		
		         string strLocation("https://fontopus.com/license_karfile");
		         System.url().set(strLocation, "id", karfileid);
		         System.url().set(strLocation, "karfile_url", urlencode(this_url()));
		         outheader("Location") = strLocation;
		
		         exit(0);
		
	         }

            if(strlen(fn) > 0)
      	   {
               return fn;
            }
         }
	   /*	header("Content-disposition: attachment; filename=\"" + $song->filename + "\"");
		   header("Content-type: application/download");
		   header("Pragma: no-cache");
		   header("Expires: 0");
		   if(!System.compress().ungz(m_pnetnodesocket->response().file(), $fn))
		   {
			   read_file($fn);
		   }
		   dprint("xxx");
		   $address 		= $_SERVER['REMOTE_ADDR'];
		   $http_referer 	= $_SERVER['HTTP_REFERER'];
		   if(isset($param_refr))
		   {
			   if($param_refr == "vmplight")
			   {
				   $refr = "vmplight-0.1.1.0";
			   }
			   else
			   {
				   $refr	= urldecode($param_refr);		
			   }
		   }
		   else
		   {
			   $refr = "unk";
		   }
		   music().register_song_click($song->topic, $address, $http_referer, $param_locale, $param_style, $refr, "100");
		   exit(0);
	   }
	   else
	   {
	   }
	   $filepath    	= $song->get_file_path();
	   music().report_error("FileSave: song file not found: \""+$filepath+"\"", true);
   //	require_once "sharedapi/util/getinclude.ds";	
	   $path = sys_get_include_path("ds", "handerr", "song-notFound");
	   dprint("downloadpath2=" + $path);

	   if(strlen($path) > 0)
	   {
		   include $path;
	   }
	   else
	   {
		   print "Sorry! File Not Found.";
	   }
	   exit(0);*/
      }
   
      string strHash(hash);
      string strMime(mimetype);
      strMime.replace("/", "\\");
      if(strHash.get_length() < 5)
         return "";
      return string("C:\\fs\\") + strMime + "\\" + strHash[0] + "\\" + strHash[1] + "\\" + strHash[2] + "\\" + strHash[3] + "\\" + strHash[4] + "\\" + strHash + "." + string(extension);
      return "";

   }


   bool script_impl::low_fs_read(const char * hash, __int64 key, __int64 size, const char * mimetype, const char * extension)
   {
      return read_file(low_fs_file_path(hash, key, size, mimetype, extension), NULL, mimetype);

   }

   __int64 script_impl::low_fs_map(const char * pszDir)
   {
      stringa stra;
      Application.dir().rls(pszDir, &stra);
      int iCount = 0;
      for(int i = 0; i < stra.get_size(); i++)
      {
         if(!Application.dir().is(stra[i]))
         {
            if(low_fs_map_file(stra[i]))
               iCount++;
         }
      }
      return iCount;
   }

   bool script_impl::low_fs_map_file(const char * pszFile)
   {
      
      string strSql;



      string strHash = System.file36().nessie(pszFile);
      __int64 len = System.file().length(pszFile);
      string strMimeType = "unknown";
      string strExtension = System.file().extension(pszFile);

      strSql.Format("SELECT `key`, `path` FROM `fs`.`item` WHERE `hash` = '%s' AND `size`=%I64d AND `mimetype` = '%s' AND `extension` = '%s'", strHash, len, strMimeType, strExtension);

      var varKeys = musicdb().query_rows(strSql);
      if(varKeys.array_get_count() <= 0)
      {

         strSql.Format( "INSERT INTO `fs`.`item`(`hash`, `key`, `size`, `mimetype`, `extension`, `path`) VALUES ('%s', '%I64d', '%I64d', '%s', '%s', '%s')",
            strHash, (__int64) 1, len, strMimeType, strExtension, musicdb().real_escape_string(pszFile));
         return musicdb().query(strSql) != NULL;
      }
      else
      {
         // nessie collision or file already exists in the repository, should solve
         return false;
      }

      return false;

   }

   bool script_impl::low_fs_license(const char * user, const char * hash, __int64 key, __int64 size, const char * mimetype, const char * extension)
   {
      string strName;
      strName.Format("%s-%I64d-%I64d-%s.%s", hash, key, size, mimetype, extension);
      return low_fs_add_user_file(user, 11, hash, key, size, mimetype, extension, strName, true);
   }

   bool script_impl::low_fs_is_licensed(const char * user, const char * hash, __int64 key, __int64 size, const char * mimetype, const char * extension)
   {
      
      string strSql;
      strSql.Format("SELECT COUNT(*) FROM `fs`.`user_folder_item` WHERE `user` = '%s' AND `hash` = '%s' AND `key` = '%I64d' AND `size`= '%I64d' AND `mimetype` = '%s' AND extension = '%s'",
         user, hash, key, size, mimetype, extension);
      return musicdb().query_item(strSql).get_integer() >= 1;

   }

   bool script_impl::low_fs_add_user_file(const char * user, __int64 iFolder, const char * hash, __int64 key, __int64 size, const char * mimetype, const char * extension, const char * pszName, bool bOverwrite)
   {
      
      string strSql;
      string strSqlPrefix;

      if(bOverwrite)
         strSqlPrefix = "REPLACE INTO ";
      else
         strSqlPrefix = "INSERT INTO ";

      strSql.Format(strSqlPrefix + " `fs`.`user_folder_item`(`user`,  `folder`, `hash`, `key`, `size`, `mimetype`, `extension`, `name`) VALUES ('%s', '%I64d', '%s', '%I64d', '%I64d', '%s', '%s', '%s')",
         user, iFolder, musicdb().real_escape_string(hash), key, size, musicdb().real_escape_string(mimetype), musicdb().real_escape_string(extension), musicdb().real_escape_string(pszName));

      mysql::result * presult = musicdb().query(strSql);

      return presult != NULL;

   }

   __int64 script_impl::low_fs_add_user_dir(const char * user, __int64 iFolder, const char * pszName)
   {
      
      string strSql;

      strSql.Format("SELECT `id` FROM `fs`.`user_folder` WHERE `parent` = %I64d AND `user` = %s AND `name` = '%s'", iFolder, user, pszName);

      var varId = musicdb().query_item(strSql);
      if(!varId.is_empty())
         return varId;

      strSql.Format("SELECT MAX(`id`) + 1 FROM `fs`.`system_folder`", iFolder, user);

      var varMaxSystem = musicdb().query_item(strSql);

      int iRetry = 0;
      while(iRetry < 100)
      {
         strSql.Format("SELECT MAX(`id`) + 1 FROM `fs`.`user_folder` WHERE `user` = %s", user);

         var varId = musicdb().query_item(strSql);

         if((__int64) varId < (__int64)varMaxSystem)
         {
            varId = varMaxSystem;
         }



         strSql.Format("INSERT INTO `fs`.`user_folder`(`user`, `parent`, `id`, `name`) VALUES('%s', '%I64d', '%I64d', '%s')", user,  iFolder, (__int64) varId, pszName);

         mysql::result * presult = musicdb().query(strSql);
         if(presult != NULL)
            return varId;

         iRetry++;
         Sleep(584);
      }

      return gen::g_emptyconst;

   }

   void script_impl::low_fs_ls_dir(const char * user, __int64 iFolder, int64_array & ia, stringa & stra)
   {
      
      string strSql;

      strSql.Format("SELECT `id`, `name` FROM `fs`.`user_folder` WHERE `parent` = %I64d AND `user` = %s", iFolder, user);

      var rowa = musicdb().query_rows(strSql);

      for(int i = 0; i < rowa.array_get_count(); i++)
      {
         var row = rowa.at(i);
         ia.add((__int64) row.at(0));
         stra.add((const char *)row.at(1));
      }

   }

   void script_impl::low_fs_ls_file(const char * user,  __int64 iFolder, stringa & straHash, int64_array & iaKey, int64_array & iaSize, stringa & straMime, stringa & straExt, stringa & straName)
   {

      string strSql;

      strSql.Format("SELECT `hash`, `key`, `size`, `mimetype`, `extension`, `name` FROM `fs`.`user_folder_item` WHERE  `folder` = %I64d AND `user` = %s", iFolder, user);

      var rowa = musicdb().query_rows(strSql);

      for(int i = 0; i < rowa.array_get_count(); i++)
      {
         var row = rowa.at(i);
         straHash.add((const char *) row.at(0));
         iaKey.add(row.at(1));
         iaSize.add(row.at(2));
         straMime.add((const char *) row.at(3));
         straExt.add((const char *) row.at(4));
         straName.add((const char *) row.at(5));
      }

   }

   bool script_impl::low_folder(string & strUser, __int64 & iFolder, const char * pszFolder)
   {
      
      string strPath(pszFolder);
      if(gen::str::begins_eat(strPath, "ifs://"))
      {
         strsize iFind = strPath.find('/');
         if(iFind <= 0)
            return false;
         strUser = musicdb().query_item("SELECT id FROM fun_user WHERE email = '" + strPath.Left(iFind) + "'");
         if(!strUser.has_char())
            return false;
         strPath = strPath.Mid(iFind + 1);
      }
      else if(gen::str::begins_eat(strPath, "uifs://"))
      {
         strUser = gprop("secureuserid");
      }
      else
      {
         return false;
      }
      if(strPath.is_empty())
      {
         iFolder = 1;
         return true;
      }
      strsize iStart = 0;
      strsize iEnd;
      string strName;
      strsize iFind;
      string strSql;
      iFolder = 1;
      while(true)
      {
         iFind = strPath.find('/', iStart);
         if(iFind == iStart)
         {
            // skip consecutive slashes as single slash;
            iStart++;
            continue;
         }
         else if(iFind > iStart)
         {
            iEnd = iFind;
         }
         else
         {
            iEnd = strPath.get_length();
         }
         strName = strPath.Mid(iStart, iEnd - iStart);
         if(strName.is_empty())
            break;
         strSql.Format("SELECT `id` FROM `fs`.`user_folder` WHERE `parent` = %I64d AND `user` = %s AND `name` = '%s'", iFolder, strUser, musicdb().real_escape_string(strName));
         iFolder = musicdb().query_item(strSql);
         if(iFolder <= 0)
            return false;
         if(iFind < 0)
            break;
         iStart = iFind + 1;
      }
      return true;
   }

   bool      script_impl::fs_dir_mk               (const char * pszFolder)
   {
      
      string strPath(pszFolder);
      string strUser;
      __int64 iFolder;
      if(gen::str::begins_eat(strPath, "ifs://"))
      {
         strsize iFind = strPath.find('/');
         if(iFind <= 0)
            return false;
         strUser = musicdb().query_item("SELECT id FROM fun_user WHERE email = '" + strPath.Left(iFind) + "'");
         if(!strUser.has_char())

            return false;
         strPath = strPath.Mid(iFind + 1);
      }
      else if(gen::str::begins_eat(strPath, "uifs://"))
      {
         strUser = gprop("secureuserid");
      }
      else
      {
         return false;
      }
      //basic permission check
      if(strUser != gprop("secureuserid").get_string() || strUser.is_empty())
         return false;
      if(strPath.is_empty())
      {
         iFolder = 1;
         return true;
      }
      strsize iStart = 0;
      strsize iEnd;
      string strName;
      strsize iFind;
      string strSql;
      __int64 iParentFolder = 1;
      while(true)
      {
         iFind = strPath.find('/', iStart);
         if(iFind == iStart)
         {
            // skip consecutive slashes as single slash;
            iStart++;
            continue;
         }
         else if(iFind > iStart)
         {
            iEnd = iFind;
         }
         else
         {
            iEnd = strPath.get_length();
         }
         strName = strPath.Mid(iStart, iEnd - iStart);
         if(strName.is_empty())
            break;
         strSql.Format("SELECT `id` FROM `fs`.`user_folder` WHERE `parent` = %I64d AND `user` = %s AND `name` = '%s'", iParentFolder, strUser, musicdb().real_escape_string(strName));
         iFolder = musicdb().query_item(strSql);
         if(iFolder <= 0)
         {
            if(!low_fs_add_user_dir(strUser, iParentFolder, strName))
               return false;
            strSql.Format("SELECT `id` FROM `fs`.`user_folder` WHERE `parent` = %I64d AND `user` = %s AND `name` = '%s'", iParentFolder, strUser, musicdb().real_escape_string(strName));
            iFolder = musicdb().query_item(strSql);
            if(iFolder <= 0)
               return false;
         }
         if(iFind < 0)
            break;
         iStart = iFind + 1;
         iParentFolder = iFolder;
      }
      return true;
   }

   int script_impl::datetime_get_month_day_count(int year, int month)
   {
      switch(month)
      {
      case 1:
         return 31;
      case 2:
         if((year % 4)== 0)
            return 29;
         else
            return 28;
      case 3:
         return 31;
      case 4:
         return 30;
      case 5:
         return 31;
      case 6:
         return 30;
      case 7:
         return 31;
      case 8:
         return 31;
      case 9:
         return 30;
      case 10:
         return 31;
      case 11:
         return 30;
      case 12:
         return 31;
      }
      return -1;
   }

   // 0 is Sunday
   string script_impl::datetime_get_weekday(int year, int month, int day)
   {
      return strftime("%w", datetime_get_timestamp(year, month, day).get_time());
   }

   class time script_impl::datetime_get_timestamp(int year, int month, int day)
   {
      string international_date;
      international_date.Format("%d-%02d-%02d", year, month, day);
      //	print "international_date=$international_date";
      return strtotime(international_date);
   }

   string script_impl::datetime_getGMTDateTime()
   {
      return datetime_formatDateTime(datetime_getTimestamp());
   }

   string script_impl::datetime_formatDateTime(class time t)
   {
      return t.FormatGmt("%Y-%m-%d %H:%M:%S");
   }

   class time script_impl::datetime_getTimestamp()
   {
      return time::get_current_time();
   }

   __int64 script_impl::datetime_InternationalToUnixMillis(const char * pszDatetime)
   {
      string date(pszDatetime);
      return class time(atoi(date.Mid(11, 2)), atoi(date.Mid(14, 2)), atoi(date.Mid(17, 2)),
         atoi(date.Mid(5, 2)),  atoi(date.Mid(8, 2)), atoi(date.Mid(0, 4))).get_time() * 1000;
   }


   string script_impl::datetime_getToday()
   {
      return time::get_current_time().FormatGmt("%Y-%m-%d 00:00:00");
   }

   class time script_impl::datetime_strtotime(const char * psz)
   {
      return datetime_strtotime(psz, time::get_current_time());
   }

   class time script_impl::datetime_strtotime(const char * psz, class time t)
   {
      class time datetime(strtotime(psz, t.get_time()));
      return class time(datetime.GetGmtYear(), datetime.GetGmtMonth(), datetime.GetGmtDay(), 12, 0, 0);
   }





   void script_impl::hand_img()
   {
      
      stringa stra;
      stra.explode(".", inattr("query_string"));
      if(stra.get_size() >= 5)
      {

         int iImage = atoi(stra[4]);
         int iHeight = -1;
         if(stra.get_size() >= 6)
         {
            iHeight = atoi(stra[5]);
         }
         string strSql = "SELECT image FROM image WHERE id = \'"+gen::str::itoa(iImage)+"\'";
         primitive::memory_file memfile(get_app());
         bool bGet = false;
         if(musicdb().query_blob(*memfile.get_memory(), strSql))
         {
            ::visual::dib_sp dib(get_app());
            var varFile;
            varFile["file"] = &memfile;
            if(dib.load_from_file(varFile))
            {
               if(iHeight > 0)
               {
                  ::visual::dib_sp dibOut(get_app());
                  dibOut->create(max(1, dib->width() * iHeight / dib->height()), iHeight);
                  dibOut->get_graphics()->SetStretchBltMode(HALFTONE);
                  dibOut->get_graphics()->StretchBlt(
                     0,
                     0,
                     dibOut->width(),
                     dibOut->height(),
                     dib->get_graphics(),
                     0,
                     0,
                     dib->width(),
                     dib->height(),
                     SRCCOPY);
                  dibOut->fill_channel(200, visual::rgba::channel_alpha);
                  //stretch_dib(dib);
                  image_png(dibOut->get_bitmap());
                  bGet = true;
               }
               else
               {
                  dib->fill_channel(200, visual::rgba::channel_alpha);
                  image_png(dib->get_bitmap());
                  bGet = true;
               }
            }
         }
         if(!bGet)
         {
            ::visual::dib_sp dib(get_app());
            if(iHeight <= 0)
               iHeight = 80;
            dib->create(iHeight, iHeight);
            ::ca::pen_sp pen(get_app());
            pen->CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            ::ca::brush_sp brush(get_app());
            brush->CreateSolidBrush(RGB(255, 255, 240));
            dib->get_graphics()->SelectObject(pen);
            dib->get_graphics()->SelectObject(brush);
            dib->get_graphics()->Rectangle(0, 0, iHeight, iHeight);
            dib->get_graphics()->SetBkMode(TRANSPARENT);
            string str("No Photo");
            dib->get_graphics()->SetTextColor(RGB(155, 100, 155));
            dib->get_graphics()->TextOutA(5, iHeight - 20, str);
            dib->fill_channel(200, visual::rgba::channel_alpha);
            image_png(dib->get_bitmap());
         }

      }


   }


   void script_impl::fontopus_ds_url_update_named()
   {

      
	   string email = "www.ca2.cc.";

	   string strSql;
	   string rr;
	   string strHost;
	   string id;
	   string subdomain;
	   string cname;

	   string type = "master";

	   string vhost_file_path = "/etc/named/_trunk";
	   string zone_dir = "/var/named/data";
	
	   ::collection::int_to_string ns1a;
	   ::collection::int_to_string ns2a;
	   ::collection::int_to_string ns5a;
	   ::collection::int_to_string ns7a;
	   ::collection::int_to_string ns8a;
	
	   string us_ns = "1.ns.laborserver.net"; // cgcl
	   string uk_ns = "2.ns.laborserver.net"; // cst	
	   string de_ns = "5.ns.laborserver.net"; 
	   string br_ns = "7.ns.laborserver.net"; 
	   string jp_ns = "8.ns.laborserver.net"; 
	
	   ns1a[0] = us_ns;
	   ns2a[0] = uk_ns;
	   ns5a[0] = de_ns;
	   ns7a[0] = br_ns;
	   ns8a[0] = jp_ns;

	   ns1a[1] = de_ns;
	   ns2a[1] = uk_ns;
	   ns5a[1] = us_ns;
	   ns7a[1] = jp_ns;
	   ns8a[1] = br_ns;

	   ns1a[2] = "ns1.discreetserver.net";
	   ns2a[2] = "ns2.discreetserver.net";
	   ns5a[2] = "ns5.discreetserver.net";
	   ns7a[2] = "ns7.discreetserver.net";
	   ns8a[2] = "ns8.discreetserver.net";
	

	   ns1a[3] = br_ns;
	   ns2a[3] = us_ns;
	   ns5a[3] = de_ns;
	   ns7a[3] = jp_ns;	
	   ns8a[3] = uk_ns;	

	   ns1a[4] = us_ns;
	   ns2a[4] = uk_ns;
	   ns5a[4] = de_ns;
	   ns7a[4] = br_ns;	
	   ns8a[4] = "";	

	   ns1a[1000] = us_ns;
	   ns2a[1000] = uk_ns;
	   ns5a[1000] = de_ns;
	   ns7a[1000] = br_ns;
	   ns8a[1000] = jp_ns;

	   ns1a[1984] = "ns1.carlosgustavocecynlundgren.info";
	   ns2a[1984] = "ns2.carlosgustavocecynlundgren.info";
	   ns5a[1984] = "ns5.carlosgustavocecynlundgren.info";
	   ns7a[1984] = "ns7.carlosgustavocecynlundgren.info";
	   ns8a[1984] = "ns8.carlosgustavocecynlundgren.info";

	   ns1a[4000] = us_ns;
	   ns2a[4000] = uk_ns;
	   ns5a[4000] = de_ns;
	   ns7a[4000] = jp_ns;
	   ns8a[4000] = "";
      string log;

	   string default_iphost = "50.30.32.100";
   //	$default_eu_iphost = "188.138.0.172";
   //	$default_eu_iphost = $default_iphost;
	   //string default_weu_iphost = "46.105.148.219";
	   string default_eu_iphost = "188.138.107.136";
      string default_weu_iphost = default_eu_iphost;
   //	$default_asia_iphost = "27.112.107.224";
   //	$default_br_iphost = "187.84.226.245";
	   //string default_br_iphost = default_iphost;
      //string default_br_iphost = "177.67.80.221";
      string default_br_iphost = default_iphost;
	   string  default_iphost_africa = default_weu_iphost;	
	
   //	$default_eu_iphost = "173.224.125.231";
   //	$default_asia_iphost = $default_eu_iphost;
	   string default_asia_iphost = "163.43.33.45";
	
	   string cgcl_ttl = "1984";
	
	
	   string debugserver1 = "188.138.107.136";

	   stringa straCmd;
	
	   straCmd.set_size(0, 1000);

	   var rra = musicdb().query_rows("SELECT name, nameserver, host, mx0, mx10 FROM gustavocc.domain WHERE active = 1");
	
	   strSql = "DELETE FROM gustavocc.rr";
	   straCmd.add(strSql);
	   strSql = "DELETE FROM gustavocc.soa";
	   straCmd.add(strSql);	
	
	
	   string rev = strftime("%Y%m%d") + rand((int)0, 9).get_string() + rand((int)0, 9).get_string();

	   int iSoa = 1;
	   int iRr = 1;
	
	   for(int i = 0; i < rra.array_get_count(); i++)
	   {
		   var rr = rra.at(i);
		   strHost = rr[0];
         TRACE("mydns: updating host " + strHost);
		   string ns1 = ns1a[rr[1]];
		   string ns2 = ns2a[rr[1]];
		   string ns5 = ns5a[rr[1]];
		   string ns7 = ns7a[rr[1]];
		   string ns8 = ns8a[rr[1]];
		   string iphost = rr[2];
		   if(iphost.is_empty())
		   {
			   url_domain domain;
			   domain.create(strHost);
			   string str = domain.m_strTopLevel;
			   if(System.url().locale_is_middle_east(str))
			   {
				   iphost = default_eu_iphost;
			   }
			   else if(System.url().locale_is_africa(str) 
			   || gen::str::find_wwci(strHost, "africa") >= 0)
			   {
				   iphost = default_iphost_africa;
			   }
			   else if(System.url().locale_is_eu(str)) // by the time, locale_is_eu comes first, the asia - japanese -  host is far from eu/asia countries
			   {
				   if(str == "fr"
				   || str == "uk"
				   || str == "pt"
				   || str == "es"
				   || str == "be"
				   || str == "ie"
				   || str == "nl"
				   || str == "it"
				   || str == "ch"
				   || str == "li"
				   || str == "lu")
				   {
					   iphost = default_weu_iphost;
				   }
				   else
				   {
					   iphost = default_eu_iphost;
				   }
			   }
			   else if(System.url().locale_is_asia(str))
			   {
				   iphost = default_asia_iphost;
			   }
			   else if(System.url().locale_is_south_america(str))
			   {
				   iphost = default_br_iphost;
			   }
			   else if(System.url().locale_is_oceania(str))
			   {
				   iphost = default_asia_iphost;
			   }
			   else
			   {
				   string str = domain.m_strSuffix;
				   if(str == "uk.com")
				   {
					   iphost = default_weu_iphost;
				   }
				   else if(str == "eu.com"
				   || str == "de.com")
				   {
					   iphost = default_eu_iphost;
				   }
				   else if(str == "asia.com")
				   {
					   iphost = default_asia_iphost;
				   }
				   else
				   {
					   iphost = default_iphost;
				   }
			   }
		   }

         log += "<h3 style=\"margin-bottom: 0px; 	font-family: Geneva, Verdana, sans-serif;\">";
		   log += strHost;
		   log += "</h3>";
		   log += iphost;
		   log += "<br />";
		
		   string mx0 = rr[3];
		   string mx10 = rr[4];		

		   string zone_file = strHost;

		
		   string id = gen::str::itoa(iSoa);
		   iSoa++;
		   strSql = "INSERT INTO gustavocc.soa(`id`, `origin`, `ns`, `mbox`, `serial`, `refresh`, `retry`, `expire`, `minimum`, `ttl`) VALUES ('" + id + "', '" + strHost + ".', '" + ns1 + ".', '" + email + "', '" + rev + "', '18000', '9000', '1209600', '" + cgcl_ttl + "', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'NS', '" + ns1 + ".', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);		
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'NS', '" + ns2 + ".', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'NS', '" + ns5 + ".', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'NS', '" + ns7 + ".', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   if(strlen(ns8) > 0)
		   {
			   rr = gen::str::itoa(iRr);
			   iRr++;
			   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'NS', '" + ns8 + ".', '0', '" + cgcl_ttl + "')";
            straCmd.add(strSql);
		   }
		
		
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'A', '" + iphost + "', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   if(strlen(mx0) > 0)
		   {
			   
            log +=("mx0=" + mx0 + "<br>");
			
			   rr = gen::str::itoa(iRr);
			   iRr++;
			   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'MX', '" + mx0 + ".', '0', '" + cgcl_ttl + "')";
            straCmd.add(strSql);
			
		   }

         if(strlen(mx10) > 0 && mx0.CompareNoCase(mx10) != 0 && (mx10.CompareNoCase("mail.ca2.cc") != 0 || mx0.CompareNoCase("mail.ca2.cc") != 0))
         {

            log +=("mx10="+mx10 + "<br>");
			
			   rr = gen::str::itoa(iRr);
			   iRr++;
			   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'MX', '" + mx10 + ".', '10', '" + cgcl_ttl + "')";
            straCmd.add(strSql);

         }
		   else if(!mx0.has_char() && !mx10.has_char())
		   {

			   log +=("mx10=mail.ca2.cc.<br>");
		
			   rr = gen::str::itoa(iRr);
			   iRr++;
			   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '" + strHost + ".', 'MX', 'mail.ca2.cc.', '0', '" + cgcl_ttl + "')";
            //musicdb().query(strSql);
            straCmd.add(strSql);
			
		   }
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', 'localhost', 'A', '127.0.0.1', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', 'www." + strHost + ".', 'A', '" + iphost + "', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', 'mail." + strHost + ".', 'A', '" + iphost + "', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '*." + strHost + ".', 'A', '" + iphost + "', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '1server." + strHost + ".', 'A', '" + debugserver1 + "', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '*.1server." + strHost + ".', 'A', '" + debugserver1 + "', '0', '" + cgcl_ttl + "')";
		   straCmd.add(strSql);
		
	   }

  	   strSql = straCmd.implode(";\r\n");
      straCmd.remove_all();
      Application.file().put_contents("C:\\ca2\\mydns1.sql", strSql);
      System.process().synch("C:\\ca2\\mydns1.bat", 0);
      
	   var cnamea = musicdb().query_rows("SELECT gustavocc.cname.subdomain, gustavocc.cname.cname, gustavocc.cname.domain, gustavocc.soa.id FROM gustavocc.cname INNER JOIN gustavocc.soa ON gustavocc.soa.origin = CONCAT(gustavocc.cname.domain, '.')");
	
	   for(int j = 0; j < cnamea.array_get_count(); j++)
	   {
		   var row  = cnamea.at(j);
		   strHost = row[2];
		   cname = row[1];
		   subdomain = row[0];		
		   log += row[0] + "." +  strHost + " IN CNAME " + cname;

		   log += "<br />";
		   id = row[3];
         TRACE("mydns: updating cname " + cname);
		   rr = gen::str::itoa(iRr);
		   iRr++;
		   strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '"+subdomain +"." + strHost + ".', 'CNAME', '"+cname +"', '0', '" + cgcl_ttl + "')";
         straCmd.add(strSql);
		
	   }
		
      var treea = musicdb().query_rows("SELECT gustavocc.subdomain_address.subdomain, gustavocc.subdomain_address.address, gustavocc.soa.origin, gustavocc.soa.id FROM gustavocc.subdomain_address INNER JOIN gustavocc.soa ON CONCAT(gustavocc.subdomain_address.subdomain, '.') LIKE CONCAT('%.', gustavocc.soa.origin)");

		for(int j = 0; j < treea.array_get_count(); j++)
		{
			var tree  = treea.at(j);
			strHost = tree[2];
			strHost.trim_right(".");
			id = tree[3];
			subdomain = tree[0];
			subdomain = subdomain.Mid(0, subdomain.find(strHost) - 1);
         TRACE("mydns: updating subdomain address " + subdomain);


			rr = gen::str::itoa(iRr);
			iRr++;
			strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '"+subdomain+"."+strHost+".', 'A', '"+tree[1].get_string()+"', '0', '60')";
         straCmd.add(strSql);

			rr = gen::str::itoa(iRr);
			iRr++;
			strSql = "INSERT INTO gustavocc.rr(`id`, `zone`, `name`, `type`, `data`, `aux`, `ttl`) VALUES('" + rr + "', '" + id + "', '*."+subdomain+"."+strHost+".', 'A', '"+tree[1].get_string()+"', '0', '60')";
         straCmd.add(strSql);
			
			log += subdomain + "." + strHost +  " IN A " + tree[1];
			log += "<br />";
		}
	


  	   strSql = straCmd.implode(";\r\n");
      straCmd.remove_all();
      Application.file().put_contents("C:\\ca2\\mydns2.sql", strSql);
      System.process().synch("C:\\ca2\\mydns2.bat", 0);

	
      Application.file().put_contents("C:\\ca2\\mydns.log", log);

	
      print(log);
   }

} // namespace dynamic_source


