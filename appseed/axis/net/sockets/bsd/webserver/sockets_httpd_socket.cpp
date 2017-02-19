/** \file httpd_socket.cpp
*/
/*
Copyright (C) 2001-2007  Anders Hedstrom (grymse@alhem.net)

This library is made available under the terms of the GNU GPL.

If you would like to use this library in a closed-source application,
a separate license agreement is available. For information about
the closed-source license agreement for the C++ sockets library,
please visit http://www.alhem.net/Sockets/license.html and/or
email license@alhem.net.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "framework.h" // #include "axis/net/sockets/bsd/sockets.h"
#include "axis/net/net_sockets.h"
#include "axis/axis/compress/compress_compress.h"
#include <openssl/ssl.h>


namespace sockets
{


   #define TMPSIZE 10000


   httpd_socket::httpd_socket(base_socket_handler& h) :
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_base_socket(h),
      http_socket(h),
      m_received(0)
   {

      m_http_date = datetime2httpdate(GetDate());

   }


   httpd_socket::httpd_socket(const httpd_socket& s) :
      object(s.get_app()),
      base_socket(s),
      socket(s),
      stream_socket(s),
      tcp_socket(s),
      http_base_socket(s),
      http_socket(s)
   {

   }


   httpd_socket::~httpd_socket()
   {

   }


   void httpd_socket::Send64(const string & str64, const string & type)
   {
   //   Base64 bb;

   /*   if (request().headers()["if-modified-since"].get_string().CompareNoCase(m_start) == 0)
      {
         m_response.attr("http_status_code") = 304;
         m_response.attr("http_status") = "Not Modified";
         SendResponse();
      }
      else*/
      {
         memory mem;
         System.base64().decode(mem, str64);
         m_response.attr("http_status_code") = 200;
         m_response.attr("http_status") = "OK";

         m_response.header("Content-length") = (int64_t) mem.get_size();
         m_response.header(__id(content_type)) = type;
   //      m_response.header("Last-modified") = m_start;
         SendResponse();

         write((char *)mem.get_data(), (int32_t) mem.get_size());
      }
   }


   string httpd_socket::datetime2httpdate(const string & dt)
   {
      struct tm tp;
      time_t t;
      const char *days[] = { "Sun","Mon","Tue","Wed","Thu","Fri","Sat" };
      const char *months[] = { "Jan","Feb","Mar","Apr","May","Jun",
                         "Jul","Aug","Sep","Oct","Nov","Dec" };
      int32_t i;
      char s[40];

   /* 1997-12-16 09:50:40 */

      if (dt.get_length() == 19)
      {
         tp.tm_year = atoi(dt.Mid(0,4)) - 1900;
         i = atoi(dt.Mid(5,2)) - 1;
         tp.tm_mon = i >= 0 ? i : 0;
         tp.tm_mday = atoi(dt.Mid(8,2));
         tp.tm_hour = atoi(dt.Mid(11,2));
         tp.tm_min = atoi(dt.Mid(14,2));
         tp.tm_sec = atoi(dt.Mid(17,2));
         tp.tm_wday = 0;
         tp.tm_yday = 0;
         tp.tm_isdst = 0;
         t = mktime(&tp);
         if (t == -1)
         {
            log("datetime2httpdate", 0, "mktime() failed");
         }

         sprintf(s,"%s, %02d %s %d %02d:%02d:%02d GMT",
          days[tp.tm_wday],
          tp.tm_mday,
          months[tp.tm_mon],
          tp.tm_year + 1900,
          tp.tm_hour,tp.tm_min,tp.tm_sec);
      }
      else
      {
         *s = 0;
      }
      return s;
   }


   string httpd_socket::GetDate()
   {
      time_t t = time(NULL);
      struct tm tp;
   #ifdef _WIN32
      memcpy(&tp, localtime(&t), sizeof(tp));
   #else
      localtime_r(&t, &tp);
   #endif
      char slask[40]; // yyyy-mm-dd hh:mm:ss
      sprintf(slask,"%d-%02d-%02d %02d:%02d:%02d",
         tp.tm_year + 1900,
         tp.tm_mon + 1,
         tp.tm_mday,
         tp.tm_hour,tp.tm_min,tp.tm_sec);
      return slask;
   }


   void httpd_socket::Reset()
   {
      http_socket::Reset();
      m_received = 0;
   }


   map < int, int, DH *, DH * > g_dh;

   DH * tmp_dh_callback(SSL *ssl, int is_export, int keylength)
   {
      switch(keylength)
      {
      case 512:
      case 1024:
      case 2048:
      case 4096:
         return g_dh[keylength];
         break;
      }
      return NULL;
   }


   void httpd_socket::InitSSLServer()
   {
      // here's the server.pem file we just created above
      // %! remember to change the password to the one you used for your server key
      //InitializeContext(m_strCat, m_strCat, "", SSLv23_server_method());
	  InitializeContext(m_strCat, m_strCat, "", TLS_server_method());


	 // SSL_CTX_set_min_proto_version(m_ssl_ctx, TLS1_VERSION);

      //synch_lock sl(m_pmutexSslCtx);

      string strCipherList = m_strCipherList;

      if (strCipherList.is_empty())
      {

         strCipherList = "ECDHE-RSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-RSA-AES128-SHA:ECDHE-RSA-RC4-SHA:DHE-RSA-AES256-SHA:DHE-RSA-AES128-SHA:RSA:SHA:3DES:!aNULL:!eNULL:!EXP:!LOW:!MD5:@STRENGTH";

      }

      if (m_strCipherList.find("DH") >= 0)
      {

         int_array ia;

         ia.add(512);
         ia.add(1024);
         ia.add(2048);
         ia.add(4096);

         for (index i = 0; i < ia.get_count(); i++)
         {

            int keylength = ia[i];

            if (g_dh[keylength] == NULL)
            {

               string strTitle = m_strCat.name();

               if (strTitle.find_ci(".") >= 0)
               {

                  strTitle = strTitle.Left(strTitle.reverse_find("."));

               }


               string strFile = m_strCat.sibling(strTitle) + ".dh" + ::str::from(keylength) + ".pem";

               FILE * paramfile = fopen(strFile, "r");

               if (paramfile)
               {

                  DH * pdh = PEM_read_DHparams(paramfile, NULL, NULL, NULL);

                  g_dh[keylength] = pdh;

                  fclose(paramfile);

               }

            }

         }

         SSL_CTX_set_tmp_dh_callback(m_ssl_ctx, &tmp_dh_callback);

      }


      //int nid = OBJ_sn2nid(ECDHE_CURVE);

      if (m_strCipherList.find("ECDH") >= 0)
      {

         EC_KEY *ecdh = EC_KEY_new_by_curve_name(NID_secp384r1);

         SSL_CTX_set_tmp_ecdh(m_ssl_ctx, ecdh);

      }

      if (m_strCipherList.find("DH") >= 0)
      {

         SSL_CTX_set_options(m_ssl_ctx, SSL_CTX_get_options(m_ssl_ctx) | SSL_OP_SINGLE_DH_USE | SSL_OP_CIPHER_SERVER_PREFERENCE);

      }
      else
      {

         SSL_CTX_set_options(m_ssl_ctx, SSL_CTX_get_options(m_ssl_ctx) | SSL_OP_CIPHER_SERVER_PREFERENCE);

      }

      SSL_CTX_set_cipher_list(m_ssl_ctx, strCipherList);

   }



   bool httpd_socket::read_file(const ::file::path & lpcsz,smart_pointer_array < int_array > * prangea,const char * pszContentType)
   {
      string strExtension = lpcsz.extension();
      string str = strExtension;
      str.make_lower();
      string strContentType(pszContentType);

      string strName(lpcsz);

      if(str::ends_ci(strName,"03 Coisa De Acender - Se..... - Djavan.mp3"))
      {
         debug_print("%s", strName.c_str());
      }

      if(outheader(__id(content_type)).get_string().has_char())
      {
      }
      else if(strContentType.has_char() && strContentType.CompareNoCase("unknown") != 0)
      {
         outheader(__id(content_type)) = strContentType;
      }
      else
      {

         outheader(__id(content_type)) = get_file_extension_mime_type(strExtension);

      }

      if(strExtension == "ttf")
      {
         outheader("access-control-allow-origin") = "*";
      }
      else if(strExtension == "otf")
      {
         outheader("access-control-allow-origin") = "*";
      }
      else if(strExtension == "woff")
      {
         outheader("access-control-allow-origin") = "*";
      }


      if(!Application.file().exists(lpcsz))
      {
         outattr(__id(http_status_code)) = 404;
         outattr(__id(http_status)) = "Not Found";
         return false;
      }
      if(prangea == NULL || prangea->get_count() == 0)
      {
         if(::str::begins_ci(strContentType,"audio/"))
         {
            if(!System.compress().ungz(response().ostream(),lpcsz))
            {
               ::file::file_sp spfile(allocer());
               if(spfile->open(lpcsz,::file::type_binary | ::file::mode_read | ::file::share_deny_none).failed())
               {
                  return false;
               }
               response().ostream().transfer_from(*spfile);
            }
         }
         else
         {
			 if (response().ostream().get_position() == 0)
			 {

				 response().m_strFile = lpcsz;
             outattr(__id(http_status_code)) = 200;
             outattr(__id(http_status)) = "OK";
             //outattr(__id(content_length)) = Application.file().length(lpcsz);

			 }
			 else
			 {

				 ::file::file_sp spfile(allocer());
				try
				{
				   if(spfile->open(lpcsz,::file::type_binary | ::file::mode_read | ::file::share_deny_none).failed())
				   {
					  return false;
				   }
				}
				catch(...)
				{
				   return false;
				}
			
				response().ostream().transfer_from(*spfile);

			}
         }
      }
      else
      {
         ::file::file_sp spfile(allocer());
         try
         {
            if(spfile->open(lpcsz,::file::type_binary | ::file::mode_read | ::file::share_deny_none).failed())
            {
               return false;
            }
         }
         catch(...)
         {
            return false;
         }
         memory_size_t iLen = (memory_size_t) spfile->get_length();
         if(prangea->get_count() > 1)
         {
            memory_size_t uiTotal = 0;
            memory mem;
            mem.allocate(128 * 1024 * 1024);
            for(int32_t i = 0; i < prangea->get_count(); i++)
            {
               memory_size_t iStart = prangea->element_at(i)->element_at(0);
               memory_size_t iEnd = prangea->element_at(i)->element_at(1);
               if(iStart >= iLen)
                  continue;
               // iEnd > iLen is not verified because file may be growing
               spfile->seek(iStart,::file::seek_begin);
               memory_size_t uiRead;
               ::file::memory_buffer memfile(get_app());
               memory_size_t iPos = iStart;
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
                     uiRead = MIN(mem.get_size(),(memory_size_t) (iEnd - iPos + 1));
                  }
                  else
                  {
                     uiRead = mem.get_size();
                  }
                  uiRead = spfile->read(mem.get_data(),uiRead);
                  uiTotal += uiRead;
                  if(uiRead == 0)
                     break;
                  memfile.write(mem.get_data(),uiRead);
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
            memory_size_t iStart = prangea->element_at(0)->element_at(0);
            memory_size_t iEnd = prangea->element_at(0)->element_at(1);
            if(iStart < iLen)
            {
               // iEnd > iLen is not verified because file may be growing
               spfile->seek(iStart,::file::seek_begin);
               memory_size_t uiRead;
               ::file::memory_buffer memfile(get_app());
               memory_size_t iPos = iStart;
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
                     uiRead = MIN(mem.get_size(),(memory_size_t) (iEnd - iPos + 1));
                  }
                  else
                  {
                     uiRead = mem.get_size();
                  }
                  uiRead = spfile->read(mem.get_data(),uiRead);
                  uiTotal += uiRead;
                  if(uiRead == 0)
                     break;
                  response().file().write(mem.get_data(),uiRead);
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
         //      brk1:;
      }
      return true;
   }


} // namespace sockets


