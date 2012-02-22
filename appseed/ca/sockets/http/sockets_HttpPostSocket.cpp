/** \file http_post_socket.cpp
 **   \date  2004-10-30
 **   \author grymse@alhem.net
**/
/*
Copyright (C) 2004-2007  Anders Hedstrom

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
#include "StdAfx.h"


namespace sockets
{



   http_post_socket::http_post_socket(socket_handler_base& h) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_client_socket(h),
      m_fields(h.get_app()),
      m_bMultipart(false)
   {
   }


   http_post_socket::http_post_socket(socket_handler_base& h,const string & url_in) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_client_socket(h, url_in),
      m_fields(h.get_app()),
      m_bMultipart(false)
   {
      single_lock lock(&System.m_mutexHttpPostBoundary, true);

      m_boundary = "----";
      for (int i = 0; i < 12; i++)
      {
         char c = System.m_countHttpPostBoundary++ % 128;
         while (!isalnum(c))
            c = System.m_countHttpPostBoundary++ % 128;
         m_boundary += c;
      }
      m_boundary += "__" + gen::str::itoa(System.m_countHttpPostBoundary++);
   }


   http_post_socket::~http_post_socket()
   {
   }


   void http_post_socket::AddField(const string & name, const string & value)
   {
      m_fields[name] = value;
   }


   void http_post_socket::AddMultilineField(const string & name, stringa & value)
   {
      m_fields[name] = value;
   }


   void http_post_socket::AddFile(const string & name,const string & filename,const string & type)
   {
      if (Application.file().exists(filename))
      {
         m_files[name] = filename;
         m_content_length[filename] = System.file().length(filename);
         m_content_type[filename] = type;
         m_bMultipart = true;
      }
      else
      {
         Handler().LogError(this, "AddFile", Errno, StrError(Errno), ::gen::log::level::fatal);
         SetCloseAndDelete();
      }
   }

   void http_post_socket::step()
   {
      if (m_bMultipart)
      {
         DoMultipartPost();
      }
      else
      {


         string body;

         if(m_fields.has_property("xml") && m_fields["xml"].get_value().get_type() == var::type_ca2)
         {
            ::xml::node * pnode = m_fields["xml"].ca2 < ::xml::node >();
            body = pnode->get_xml();
            body.trim();
            if(inheader("Content-type").get_string().find_ci("application/xml") < 0)
            {
               inheader("Content-type") = "application/xml; " + inheader("Content-type").get_string();
            }
         }
         else
         {
            body = m_fields.get_http_post();
            if(inheader("Content-type").get_string().find_ci("application/x-www-form-urlencoded") < 0)
            {
               inheader("Content-type") = "application/x-www-form-urlencoded; " + inheader("Content-type").get_string();
            }
         }

         // only fields, no files, add urlencoding
         /*for (std::collection::map<string,list<string> >::iterator it = m_fields.begin(); it != m_fields.end(); it++)
         {
            string name = (*it).first;
            list<string>& ref = (*it).second;
            if (body.get_length())
            {
               body += '&';
            }
            body += name + "=";
            bool first = true;
            for (list<string>::iterator it = ref.begin(); it != ref.end(); it++)
            {
               string value = *it;
               if (!first)
               {
                  body += "%0d%0a"; // CRLF
               }
               body += Utility::rfc1738_encode(value);
               first = false;
            }
         }*/

         // build header, send body
         m_request.attr("http_method") = "POST";
         m_request.attr("http_version") = "HTTP/1.1";
         string strHost = GetUrlHost();
         inheader("Host") = strHost; // oops - this is actually a request header that we're adding..
         string strUserAgent = MyUseragent();
         if(!(bool)m_request.attr("minimal_headers"))
         {
            inheader("User-agent") = "ca2_netnode";
            inheader("Accept") = "text/html, text/plain, application/xml, */*;q=0.01";
            inheader("Connection") = "close";
         }
         inheader("Content-length") = body.get_length();
         SendRequest();

         if(body.get_length() > 0)
         {
         // send body
         Send( body );
         }
      }
   }


   void http_post_socket::DoMultipartPost()
   {
      long length = 0; // calculate content_length of our post body
      string tmp;

      // fields
      {
         for(int i = 0; i < m_fields.m_propertya.get_count(); i++)
         {
            string name = m_fields.m_propertya[i].name();
            var & var = m_fields.m_propertya[i].get_value();
            tmp = "--" + m_boundary + "\r\n"
               "content-disposition: form-data; name=\"" + name + "\"\r\n"
               "\r\n";
            for(int j = 0; j < var.get_count(); j++)
            {
               string value = var[j].get_string();
               tmp += value + "\r\n";
            }
            length += (long)tmp.get_length();
         }
      }

      // files
      {
         POSITION pos = m_files.get_start_position();
         for(; pos != NULL; )
         {
            string name;
            string filename;
            m_files.get_next_assoc(pos, name, filename);
            long content_length = m_content_length[filename];
            string content_type = m_content_type[filename];
            tmp = "--" + m_boundary + "\r\n"
               "content-disposition: form-data; name=\"" + name + "\"; filename=\"" + filename + "\"\r\n"
               "content-type: " + content_type + "\r\n"
               "\r\n";
            length += (long)tmp.get_length();
            length += content_length;
            length += 2; // crlf after file
         }
      }

      // end
      tmp = "--" + m_boundary + "--\r\n";
      length += (long)tmp.get_length();

      // build header, send body
      m_request.attr("http_method") = "POST";
      m_request.attr("http_version") = "HTTP/1.1";
      inheader("Host") = GetUrlHost(); // oops - this is actually a request header that we're adding..
      inheader("User-agent") = MyUseragent();
      inheader("Accept") = "text/html, text/plain, */*;q=0.01";
      inheader("Connection") = "close";
      inheader("Content-type") = "multipart/form-data; boundary=" + m_boundary;
      inheader("Content-length") = length;

      SendRequest();

      // send fields
      {
         for(int i = 0; i < m_fields.m_propertya.get_count(); i++)
         {
            string name = m_fields.m_propertya[i].name();
            var & var = m_fields.m_propertya[i].get_value();
            tmp = "--" + m_boundary + "\r\n"
               "content-disposition: form-data; name=\"" + name + "\"\r\n"
               "\r\n";
            for(int j = 0; j < var.get_count(); j++)
            {
               string value = var[j].get_string();
               tmp += value + "\r\n";
            }
            Send( tmp );
         }
      }

      // send files
      {
         POSITION pos = m_files.get_start_position();
         for(; pos != NULL;)
         {
            string name;
            string filename;
            m_files.get_next_assoc(pos, name, filename);
            string content_type = m_content_type[filename];
            tmp = "--" + m_boundary + "\r\n"
               "content-disposition: form-data; name=\"" + name + "\"; filename=\"" + filename + "\"\r\n"
               "content-type: " + content_type + "\r\n"
               "\r\n";
            Send( tmp );
            {
               ::ex1::filesp file(get_app());
               if(file->open(filename, ::ex1::file::type_binary | ::ex1::file::mode_read))
               {
                  primitive::memory mem;
                  mem.FullLoad(file);
                  SendBuf((const char *) mem.get_data(), mem.get_size());
               }
            }
            Send("\r\n");
         }
      }

      // end of send
      Send("--" + m_boundary + "--\r\n");
   }


   void http_post_socket::SetMultipart()
   {
      m_bMultipart = true;
   }


}


