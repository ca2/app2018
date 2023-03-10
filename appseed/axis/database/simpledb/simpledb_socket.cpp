#include "framework.h"
#include "simpledb_socket.h"
#include "simpledb_socket_thread.h"





namespace simpledb
{


   socket::socket(::sockets::base_socket_handler & h) :
      object(h.get_app()),
      base_socket(h),
      ::sockets::socket(h),
      stream_socket(h),
      tcp_socket(h),
#ifdef BSD_STYLE_SOCKETS
      http_base_socket(h),
      http_socket(h),
#endif
      httpd_socket(h)
   {
      /*m_strHead += "HTTP/1.1 200 OK\n";
      m_strHead += "Server: netnode/1.0 (Windows)\n";
      m_strHead += "Accept-Ranges: bytes\n";
      m_strHead += "Date: Mon, 23 May 2005 22:38:34 GMT\n";
      m_strHead += "Server: netnode/1.0 (Windows)\n";
      m_strHead += "Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\n";
      //m_memfileSend += "Etag: "3f80f-1b6-3e1cb03b"";
      m_strHead += "Accept-Ranges: bytes\n";
      // m_memfileSend += "Content-Length: 438";
      //m_strHead += "Connection: close\n";*/

      /*   m_memfileBody << "<html>\n";
         m_memfileBody << "<head>\n";
         m_memfileBody << "</head>\n";
         m_memfileBody << "<body>\n";
         m_memfileBody << "</body>\n";
         m_memfileBody.Truncate(0);*/
   }

   socket::~socket()
   {
   }


   void socket::send_response()
   {

      if(::str::begins(inattr("request_uri"),astr.strPassthroughUri))
      {
      }
      else
      {

         http::response res(get_app());

         string strUri(inattr("request_uri"));
         string strHost(inheader("host"));
         string strScript = System.url().object_get_script(strUri);
         if(strUri.find("thesecret") >= 0)
         {
            //      debug_break();
         }
         if(strHost == "core.am")
         {
            TRACE("simpledb::socket::send_response core.am");
//            uint32_t dw = ::get_tick_count();
            //  debug_break();
         }
         if(strScript == "songs")
         {
            TRACE("simpledb::socket::send_response songs");
            //uint32_t dw = ::get_tick_count();
         }
         manager().handle(this);
      }
      if(!outheaders().has_property("content-type") && response().file().get_length() > 0)
      {
         outheader(__id(content_type)) = "text/html; charset=UTF-8";
      }
      int32_t iStatusCode;
      string strStatus;
      if(outattr("http_status_code").is_new()
            || outattr("http_status").is_new())
      {
         if(outheader("location").is_set())
         {
            iStatusCode = 303; // 303 (See Other Location)
            strStatus = "See Other";
         }
         else
         {
            iStatusCode = 200;
            strStatus = "OK";
         }
         outattr("http_status_code") = iStatusCode;
         outattr("http_status") = strStatus;
      }
      outattr("http_version") = "HTTP/1.1";
      Respond();
   }



   class manager & socket::manager()
   {
      return *dynamic_cast < ::simpledb::socket_thread & > (Handler()).m_pservice->m_pmanager;
   }


   void socket::OnExecute()
   {
      send_response();
   }

   void socket::OnResponseComplete()
   {
   }

   /*
   bool socket::http_filter_response_header(id key, string & strValue)
   {
      if(strKey.compare_ci("Location") == 0)
      {
         url_domain domain;
         domain.create(System.url().get_server(strValue));
         if(domain.m_strRadix == "ca2open")
         {
            strValue = "https://" + Application.m_strFontopusServer + System.url().get_object(strValue);
         }
         else if(domain.m_strName == "account.ca2.cc" && System.url().get_protocol(strValue) == "https")
         {
            strValue = "https://" + Application.m_strFontopusServer + System.url().get_object(strValue);
         }
      }
      strValue.trim();
      if(strValue.is_empty())
         return false;
      return true;
   }
   */

   void socket::OnSSLAccept()
   {
      m_strCat = System.m_psimpledb->db().data_load(NULL, "netnodec." + GetLocalAddress().get_display_number());
      ::sockets::httpd_socket::OnSSLAccept();
   }

   void socket::simple_file_server(const ::file::path & psz,const char * pszRelative)
   {
      smart_pointer_array < int_array > rangea;
      if(strlen(inheader("range")) > 0)
      {
         stringa straItem;
         straItem.explode("=", inheader("range"));
         if(straItem.get_count() == 2)
         {
            string strUnit = straItem[0];
            stringa stra;
            stra.explode(",", straItem[1]);
            for(int32_t i = 0; i < stra.get_count(); i++)
            {
               stringa straRange;
               straRange.explode("-", stra[i]);
               if(straRange.get_count() == 2)
               {
                  rangea.add(new int_array());
                  rangea.last_sp()->add(atoi(straRange[0]));
                  straRange[1].trim();
                  if(strlen(straRange[1]) == 0)
                     rangea.last_sp()->add(-1);
                  else
                     rangea.last_sp()->add(atoi(straRange[1]));
               }
            }
         }
      }
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
      strPath = psz / strRelative;
      read_file(strPath, &rangea);
   }


   bool socket::read_file(const ::file::path & lpcsz,smart_pointer_array < int_array > * prangea,const char * pszContentType)
   {
      string strExtension = lpcsz.extension();
      string str = strExtension;
      str.make_lower();
      string strContentType(pszContentType);
      if(strContentType.has_char() && strContentType.compare_ci("unknown") != 0)
      {
         outheader(__id(content_type)) = strContentType;
      }
      else if(str == "mp3")
      {
         outheader(__id(content_type)) = "audio/mpeg";
      }
      else if(str == "exe")
      {
         outheader(__id(content_type)) = "application/x-msdownload";
      }
      else if(str == "spa")
      {
         outheader(__id(content_type)) = "application/x-spa";
      }
      else if(str == "mid")
      {
         outheader(__id(content_type)) = "audio/midi";
      }
      else if(str == "js")
      {
         outheader(__id(content_type)) = "text/javascript";
      }
      else if(str == "xpi")
      {
         outheader(__id(content_type)) = "application/x-xpinstall";
      }
      else if(str == "ttf")
      {
         outheader(__id(content_type)) = "font/ttf";
      }
      else if(str == "ogv")
      {
         outheader(__id(content_type)) = "video/ogg";
      }
      else if(str == "zip")
      {
         outheader(__id(content_type)) = "application/x-zip-compressed";
      }
      else if(str == "jar")
      {
         outheader(__id(content_type)) = "application/x-jar";
      }
      ::file::file_sp spfile(allocer());
      if(spfile->open(lpcsz, ::file::type_binary | ::file::mode_read | ::file::share_deny_none).failed())
      {
         return false;
      }
      if(prangea == NULL || prangea->get_count() == 0)
      {
         serialize is(spfile);
         serialize os(&response().file());
         os.transfer_from(is);
      }
      else
      {
         int32_t iLen = Application.file().length(lpcsz);
         if(prangea->get_count() > 1)
         {
            memory_size_t uiTotal = 0;
            memory mem;
            mem.allocate(128 * 1024 * 1024);
            for(int32_t i = 0; i < prangea->get_count(); i++)
            {
               memory_position_t  iStart = prangea->element_at(i)->element_at(0);
               memory_position_t  iEnd = prangea->element_at(i)->element_at(1);
               if(iStart >= natural(iLen))
                  continue;
               // iEnd > iLen is not verified because file may be growing
               spfile->seek(iStart, ::file::seek_begin);
               memory_size_t uiRead;
               ::memory_file memfile(get_app());
               memory_position_t iPos = iStart;
               if(iEnd >= iStart)
               {
               }
               else if(iStart > 0)
               {
                  iEnd = iLen - 1;
               }
               else
               {
                  continue;
               }
               response().ostream() << "--THIS_STRING_SEPARATES\r\n\r\n";
               response().ostream() << "Content-range: bytes " + ::str::from(iStart) + "-" + ::str::from(iEnd) + "/" + ::str::from(iLen) + "\r\n";
               response().ostream() << "Content-Transfer-Encoding: base64";
               response().ostream() << "\r\n";
               while(true)
               {
                  if(iEnd >= iStart)
                  {
                     uiRead = MIN(mem.get_size(), (memory_size_t) (iEnd - iPos + 1));
                  }
                  else
                  {
                     uiRead = mem.get_size();
                  }
                  uiRead = spfile->read(mem.get_data(), uiRead);
                  uiTotal += uiRead;
                  if(uiRead == 0)
                     break;
                  memfile.write(mem.get_data(), uiRead);
                  iPos += uiRead;
                  if(iPos >= spfile->get_length())
                     break;
               }
               response().ostream() << System.base64().encode(*memfile.get_memory());
            }
            response().ostream() << "--THIS_STRING_SEPARATES--\r\n\r\n";
            outheader(__id(content_type)) = "multipart/x-byteranges; boundary=THIS_STRING_SEPARATES";
         }
         else
         {
            memory_size_t uiTotal = 0;
            memory mem;
            mem.allocate(128 * 1024 * 1024);
            memory_position_t iStart = prangea->element_at(0)->element_at(0);
            memory_position_t iEnd = prangea->element_at(0)->element_at(1);
            if(iStart < natural(iLen))
            {
               // iEnd > iLen is not verified because file may be growing
               spfile->seek(iStart, ::file::seek_begin);
               memory_size_t uiRead;
               ::memory_file memfile(get_app());
               memory_position_t iPos = iStart;
               if(iEnd >= iStart)
               {
               }
               else if(iStart > 0)
               {
                  iEnd = iLen - 1;
               }
               while(true)
               {
                  if(iEnd != -1 && iEnd >= iStart)
                  {
                     uiRead = MIN(mem.get_size(), (iEnd - iPos + 1));
                  }
                  else
                  {
                     uiRead = mem.get_size();
                  }
                  uiRead = spfile->read(mem.get_data(), uiRead);
                  uiTotal += uiRead;
                  if(uiRead == 0)
                     break;
                  response().file().write(mem.get_data(), uiRead);
                  iPos += uiRead;
                  if(iPos >= spfile->get_length())
                     break;
               }
            }
            outattr("http_status_code") = 206;
            outattr("http_status") = "Partial Content";
            if(iEnd == -1)
            {
               outheader("Content-range") = "bytes " + ::str::from(iStart) + "-" + ::str::from(iEnd) + "/*";
            }
            else
            {
               outheader("Content-range") = "bytes " + ::str::from(iStart) + "-" + ::str::from(iEnd) + "/" + ::str::from(iLen);
            }

         }
//         brk1:;
      }
      return true;
   }


} // namespace simpledb






