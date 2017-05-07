#include "framework.h" // from "aura/net/net_sockets.h"
#include "aura/net/net_sockets.h"


namespace sockets
{


   http_post_socket::http_post_socket(base_socket_handler& h) :
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_socket(h),
      http_tunnel(h),
      http_client_socket(h),
      m_fields(h.get_app()),
      m_pmultipart(NULL)
   {

      m_emethod = http_method_post;

   }


   http_post_socket::http_post_socket(base_socket_handler& h,const string & url_in) :
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_socket(h),
      http_tunnel(h),
      http_client_socket(h, url_in),
      m_fields(h.get_app()),
      m_pmultipart(NULL)
   {

      m_emethod = http_method_post;

      single_lock lock(&Session.sockets().m_mutexHttpPostBoundary, true);

      m_boundary = "----";

      for (int i = 0; i < 12; i++)
      {

         char c = Session.sockets().m_countHttpPostBoundary++ % 128;

         while (!isalnum((unsigned char)c))
         {

            c = Session.sockets().m_countHttpPostBoundary++ % 128;

         }

         m_boundary += c;

      }

      m_boundary += "__" + ::str::from(Session.sockets().m_countHttpPostBoundary++);

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
         if (m_pmultipart == NULL)
         {

            m_pmultipart = canew(multipart(get_app()));

         }
         m_pmultipart->m_map[name].m_spfile = Application.file().get_file(filename, ::file::type_binary | ::file::mode_read | ::file::share_deny_none);
         //m_mapFiles[name]              = filename;
         m_pmultipart->m_map[name].m_uiContentLength = m_pmultipart->m_map[name].m_spfile->get_length();
         m_pmultipart->m_map[name].m_strContentType = type;
         //m_mapContentLength[filename]  = Application.file().length(filename);
         //m_mapContentType[filename]    = type;
         //m_bMultipart                  = true;
      }
      else
      {
         log("AddFile", Errno, wsa_str_error(Errno), ::aura::log::level_fatal);
         SetCloseAndDelete();
      }
   }

   void http_post_socket::step()
   {



      if (m_pmultipart != NULL)
      {

         DoMultipartPost();

      }
      else
      {


         string body;

         if (m_fields.has_property("json") && m_fields["json"].get_value().get_type() == var::type_propset)
         {
            m_fields["json"].propset().get_json(body);
            if (inheader(__id(content_type)).get_string().find_ci("application/json") < 0)
            {
               inheader(__id(content_type)) = "application/json" + ::str::has_char(inheader(__id(content_type)).get_string(), "; ");
            }
         }
         else if (m_fields.has_property("xml") && m_fields["xml"].get_value().get_type() == var::type_element)
         {
            ::xml::node * pnode = m_fields["xml"].cast < ::xml::node >();
            body = pnode->get_xml();
            body.trim();
            if(inheader(__id(content_type)).get_string().find_ci("application/xml") < 0)
            {
               inheader(__id(content_type)) = "application/xml; " + inheader(__id(content_type)).get_string();
            }
         }
         else
         {
            m_fields.get_http_post(body);
            if(inheader(__id(content_type)).get_string().find_ci("application/x-www-form-urlencoded") < 0)
            {
               inheader(__id(content_type)) = "application/x-www-form-urlencoded" + ::str::has_char(inheader(__id(content_type)).get_string(),"; ");
            }
         }

         // only fields, no files, add urlencoding
         /*for (std::map<string,list<string> >::iterator it = m_fields.begin(); it != m_fields.end(); it++)
         {
            string name = (*it).first;
            list<string>& ref = (*it).m_element2;
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
         m_request.attr(__id(http_method)) = "POST";
         m_request.attr(__id(http_version)) = "HTTP/1.1";
         string strHost = GetUrlHost();
         inheader(__id(host)) = strHost; // oops - this is actually a request header that we're adding..
         string strUserAgent = MyUseragent();
         if(!(bool)m_request.attr("minimal_headers"))
         {
            inheader(__id(user_agent)) = "ca2_netnode";

            if(inheader(__id(accept)).is_empty())
            {

               inheader(__id(accept)) = "text/html, text/plain, application/xml, */*;q=0.01";

            }
            //inheader(__id(connection)) = "close";
         }
         inheader(__id(content_length)) = (int64_t) body.get_length();

      m_bExpectResponse = true;
      m_bExpectRequest = false;

         SendRequest();

         if(body.get_length() > 0)
         {
         // send body
         write( body );
         }
      }
   }


   void http_post_socket::DoMultipartPost()
   {
      
      uint64_t length = 0; // calculate content_length of our post body
      
      string tmp;

      if (m_pmultipart->m_strOverrideBoundary_is_the_bounday_the_issue_i_e_should_it_be_the_same_across_appends.has_char())
      {

         m_boundary = m_pmultipart->m_strOverrideBoundary_is_the_bounday_the_issue_i_e_should_it_be_the_same_across_appends;

      }
      else
      {

         // Thanks Mummi for letting me program till dawn...
         // Thanks Alfredo Plays and mates from Twitch for keeping the vibe up...
         // And Denis for sticking on details and being so patient with so stuborn person like me and a software like ca2...

         m_pmultipart->m_strOverrideBoundary_is_the_bounday_the_issue_i_e_should_it_be_the_same_across_appends = m_boundary;

      }

      string strFields;

      {

         for(auto & property : m_fields)
         {

            id & id = property.m_element1;

            var & var = property.m_element2;

            strFields += "--" + m_boundary + "\r\nContent-Disposition: form-data; name=\"" + id.to_string() + "\"\r\n\r\n";

            string value = var.get_string();

            strFields += value + "\r\n";

            

         }

         length += (long)strFields.get_length();

      }

      {
         for (auto & pair : m_pmultipart->m_map)
         {
            
            string & name = pair.m_element1;
            
            uint64_t content_length = pair.m_element2.m_uiContentLength;

            string filename;

            if (pair.m_element2.m_spfile->GetFilePath().has_char())
            {

               filename = "; filename=\"" + string(::file::path(pair.m_element2.m_spfile->GetFilePath()).name()) + "\"";

            }

            string content_type;

            if (pair.m_element2.m_strContentType.has_char())
            {

               content_type = "Content-Type: " + pair.m_element2.m_strContentType + "\r\n";

            }

            string strContentLength;

            strContentLength = "Content-Length: " + ::str::from(content_length) + "\r\n";

            tmp = "--" + m_boundary + "\r\nContent-Disposition: form-data; name=\"" + name + "\""+ filename + "\r\n"
               + content_type + strContentLength + "\r\n";

            length += (long)tmp.get_length();

            length += content_length;

            length += 2; // crlf after file

         }
         //POSITION pos = m_mapFiles.get_start_position();

         //for(; pos != NULL; )
         //{
         //   
         //   string name;
         //   
         //   string filename;
         //   
         //   m_mapFiles.get_next_assoc(pos, name, filename);
         //   
         //   uint64_t content_length = m_mapContentLength[filename];

         //   string content_type = m_mapContentType[filename];

         //   tmp = "--" + m_boundary + "\r\n"
         //      "content-disposition: form-data; name=\"" + name + "\"; filename=\"" + filename + "\"\r\n"
         //      "content-type: " + content_type + "\r\n"
         //      "\r\n";

         //   length += (long)tmp.get_length();

         //   length += content_length;

         //   length += 2; // crlf after file

         //}

      }

      // end
      tmp = "--" + m_boundary + "--\r\n";
      length += (long)tmp.get_length();

      // build header, send body
      m_request.attr(__id(http_method)) = "POST";
      m_request.attr(__id(http_version)) = "HTTP/1.1";
      string strHost = GetUrlHost();
      inheader(__id(host)) = strHost; // oops - this is actually a request header that we're adding..
      inheader(__id(user_agent)) = MyUseragent();
      inheader(__id(accept)) = "text/html, text/plain, */*;q=0.01";
      //inheader(__id(connection)) = "close";
      inheader(__id(content_type)) = "multipart/form-data; boundary=" + m_boundary;
      inheader(__id(content_length)) = (int64_t) length;

      m_bExpectResponse = true;
      m_bExpectRequest = false;

      SendRequest();

      // send fields
      {
         //tmp = "";
         //for(auto & property : m_fields)
         //{
         //   id & id = property.m_element1;
         //   var & var = property.m_element2;
         //   tmp += "--" + m_boundary + "\r\nContent-Disposition: form-data; name=\"" + id.to_string() + "\"\r\n\r\n";
         //   string value = var.get_string();
         //   tmp += value + "\r\n";
         //   //for(int j = 0; j < var.get_count(); j++)
         //   //{
         //     // string value = var[j].get_string();
         //      //tmp += value + "\r\n";
         //   //}
         //}
         if (strFields.has_char())
         {
            write(strFields);
         }
      }

      // send files
      {

         for (auto & pair : m_pmultipart->m_map)
         {

            string & name = pair.m_element1;

            uint64_t content_length = pair.m_element2.m_uiContentLength;

            string filename;

            if (pair.m_element2.m_spfile->GetFilePath().has_char())
            {

               filename = "; filename=\"" + string(::file::path(pair.m_element2.m_spfile->GetFilePath()).name()) + "\"";

            }

            string content_type;

            if (pair.m_element2.m_strContentType.has_char())
            {

               content_type = "Content-Type: " + pair.m_element2.m_strContentType + "\r\n";

            }

            string strContentLength;

            strContentLength = "Content-Length: " + ::str::from(content_length) + "\r\n";

            tmp = "--" + m_boundary + "\r\nContent-Disposition: form-data; name=\"" + name + "\"" + filename + "\r\n"
               + content_type + strContentLength + "\r\n";

            write( tmp );
            //{
               //::file::file_sp spfile(allocer());
               //if(spfile->open(filename, ::file::type_binary | ::file::mode_read).succeeded())
               //{
            
            memory m;
            
            m.allocate(content_length);
            
            pair.m_element2.m_spfile->read(m.get_data(), m.get_size());
            
            write(m.get_data(), m.get_size());

                  //transfer_from(*pair.m_element2.m_spfile, content_length);
               //}
            //}
            write("\r\n");
         }
         //POSITION pos = m_mapFiles.get_start_position();
         //for (; pos != NULL;)
         //{
         //   string name;
         //   string filename;
         //   m_mapFiles.get_next_assoc(pos, name, filename);
         //   string content_type = m_mapContentType[filename];
         //   tmp = "--" + m_boundary + "\r\n"
         //      "content-disposition: form-data; name=\"" + name + "\"; filename=\"" + filename + "\"\r\n"
         //      "content-type: " + content_type + "\r\n"
         //      "\r\n";
         //   write(tmp);
         //   {
         //      ::file::file_sp spfile(allocer());
         //      if (spfile->open(filename, ::file::type_binary | ::file::mode_read).succeeded())
         //      {
         //         transfer_from(*spfile);
         //      }
         //   }
         //   write("\r\n");
         //}
      }

      // end of send
      write("--" + m_boundary + "--\r\n");
   }


   //void http_post_socket::SetMultipart()
   //{
   //   m_pmulbMultipart = true;
   //}


}


