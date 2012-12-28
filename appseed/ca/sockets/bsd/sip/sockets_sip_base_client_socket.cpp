#include "framework.h"

namespace sockets
{

   sip_base_client_socket::sip_base_client_socket(socket_handler_base& h) :
      ::ca::ca(h.get_app()),
      socket(h),
      m_request(h.get_app()),
      m_response(h.get_app()),
      m_bFirst(true)
      ,m_bHeader(true)
      ,m_bRequest(false)
      ,m_bResponse(false)
      ,m_body_size_left(0)
      ,m_b_http_1_1(false)
      ,m_b_keepalive(false)
      ,m_b_chunked(false)
      ,m_chunk_size(0)
      ,m_chunk_state(0)
   {
      m_request.attr("http_version") = "SIP/2.0";
      m_eerrorLast = error_none;
      SetLineProtocol();
   }

   sip_base_client_socket::sip_base_client_socket(const sip_base_client_socket& s) :
      ::ca::ca(s.get_app()),
      socket(s.m_handler),
      m_request(s.get_app()),
      m_response(s.get_app())
   {
   }


   sip_base_client_socket::~sip_base_client_socket()
   {
   }


   void sip_base_client_socket::OnRawData(char *buf,size_t len)
   {
      if (!m_bHeader)
      {
         if (m_b_chunked)
         {
            size_t ptr = 0;
            while (ptr < len)
            {
               switch (m_chunk_state)
               {
               case 4:
                  while (ptr < len && (m_chunk_line.get_length() < 2 || m_chunk_line.Mid(m_chunk_line.get_length() - 2) != "\r\n"))
                     m_chunk_line += buf[ptr++];
                  if (m_chunk_line.get_length() > 1 && m_chunk_line.Mid(m_chunk_line.get_length() - 2) == "\r\n")
                  {
                     OnDataComplete();
                     // prepare for next request(or response)
                     m_b_chunked = false;
                     SetLineProtocol( true );
                     m_bFirst = true;
                     m_bHeader = true;
                     m_body_size_left = 0;
                     if (len - ptr > 0)
                     {
                        char tmp[TCP_BUFSIZE_READ];
                        memcpy(tmp, buf + ptr, len - ptr);
                        tmp[len - ptr] = 0;
                        OnRead( tmp, len - ptr );
                        ptr = len;
                     }
                  }
                  break;
               case 0:
                  while (ptr < len && (m_chunk_line.get_length() < 2 || m_chunk_line.Mid(m_chunk_line.get_length() - 2) != "\r\n"))
                     m_chunk_line += buf[ptr++];
                  if (m_chunk_line.get_length() > 1 && m_chunk_line.Mid(m_chunk_line.get_length() - 2) == "\r\n")
                  {
                     m_chunk_line = m_chunk_line.Left(m_chunk_line.get_length() - 2);
                     ::gen::parse pa(m_chunk_line, ";");
                     string size_str = pa.getword();
                     m_chunk_size = gen::hex::to_uint(size_str);
                     if (!m_chunk_size)
                     {
                        m_chunk_state = 4;
                        m_chunk_line = "";
                     }
                     else
                     {
                        m_chunk_state = 1;
                        m_chunk_line = "";
                     }
                  }
                  break;
               case 1:
                  {
                     size_t left = len - ptr;
                     size_t sz = m_chunk_size < left ? m_chunk_size : left;
                     OnData(buf + ptr, sz);
                     m_chunk_size -= sz;
                     ptr += sz;
                     if (!m_chunk_size)
                     {
                        m_chunk_state = 2;
                     }
                  }
                  break;
               case 2: // skip CR
                  ptr++;
                  m_chunk_state = 3;
                  break;
               case 3: // skip LF
                  ptr++;
                  m_chunk_state = 0;
                  break;
               }
            }
         }
         else
         if (!m_b_keepalive)
         {
            OnData(buf, len);
            /*
               request is HTTP/1.0 _or_ HTTP/1.1 and not keep-alive

               This means we destroy the connection after the response has been delivered,
               hence no need to reset all internal state variables for a new incoming
               request.
            */
            m_body_size_left -= len;
            if (!m_body_size_left)
            {
               OnDataComplete();
            }
         }
         else
         {
            size_t sz = m_body_size_left < len ? m_body_size_left : len;
            OnData(buf, sz);
            m_body_size_left -= sz;
            if (!m_body_size_left)
            {
               OnDataComplete();
               // prepare for next request(or response)
               SetLineProtocol( true );
               m_bFirst = true;
               m_bHeader = true;
               m_body_size_left = 0;
               if (len - sz > 0)
               {
                  char tmp[TCP_BUFSIZE_READ];
                  memcpy(tmp, buf + sz, len - sz);
                  tmp[len - sz] = 0;
                  OnRead( tmp, len - sz );
               }
            }
         }
      }
   }


   void sip_base_client_socket::OnLine(const string & line)
   {
      if (m_bFirst)
      {
         m_request.attr("remote_addr") = GetRemoteAddress().get_display_number();
         {

#ifdef WINDOWS

            int64_t count;
            int64_t freq;
            if(QueryPerformanceCounter((LARGE_INTEGER *) &count)
            && QueryPerformanceFrequency((LARGE_INTEGER *) &freq))
            {
               m_iFirstTime = count * 1000 * 1000 / freq;
            }
            else
            {
               m_iFirstTime = ::get_tick_count();
            }
#else

            timeval t;
            gettimeofday(&t, NULL);
            m_iFirstTime = t.tv_sec * 1000 * 1000 + t.tv_usec;

#endif


         }
         ::gen::parse pa(line);
         string str = pa.getword();
         if (str.get_length() > 4 &&  gen::str::begins_ci(str, "http/")) // response
         {
            m_request.attr("http_version") = str;
            m_request.attr("http_status_code") = pa.getword();
            m_request.attr("http_status") = pa.getrest();
            m_bResponse = true;
         }
         else // request
         {
            m_request.attr("http_method") = str;
            m_request.attr("https") = IsSSL();
            if(IsSSL())
            {
               m_request.attr("http_protocol") = "https";
            }
            else
            {
               m_request.attr("http_protocol") = "http";
            }
            m_request.attr("http_method") = str;
            m_request.attr("request_uri") = pa.getword();
            m_request.attr("http_version") = pa.getword();
            m_b_http_1_1 = gen::str::ends(m_request.attr("http_version"), "/1.1");
            m_b_keepalive = m_b_http_1_1;
            m_bRequest = true;
         }
         m_bFirst = false;
         OnFirst();
         return;
      }
      if (!line.get_length())
      {
         if (m_body_size_left || !m_b_keepalive || m_b_chunked)
         {
            SetLineProtocol(false);
            m_bHeader = false;
         }
         OnHeaderComplete();
         if (!m_body_size_left && !m_b_chunked)
         {
            OnDataComplete();
         }
         return;
      }
      ::gen::parse pa(line,":");
      string key = pa.getword();
      string value = pa.getrest();
      string lowvalue = value;
      lowvalue.make_lower();
      OnHeader(key, value, lowvalue);
      if(gen::str::equals_ci(key, "content-length"))
      {
         m_body_size_left = atol(value);
      }
      if(gen::str::equals_ci(key, "connection"))
      {
         if (m_b_http_1_1)
         {
            if(gen::str::equals_ci(value, "close"))
            {
               m_b_keepalive = false;
            }
            else
            {
               m_b_keepalive = true;
            }
         }
         else
         {
            if(gen::str::equals_ci(value, "keep-alive"))
            {
               m_b_keepalive = true;
            }
            else
            {
               m_b_keepalive = false;
            }
         }
      }
      if (gen::str::equals_ci(key, "transfer-encoding") && gen::str::ends_ci(value, "chunked"))
      {
         m_b_chunked = true;
      }
      /* If remote end tells us to keep connection alive, and we're operating
      in http/1.1 mode (not http/1.0 mode), then we mark the socket to be
      retained. */
   #ifdef ENABLE_POOL
      if(m_b_keepalive)
      {
         SetRetain();
      }
   #endif
   }


   void sip_base_client_socket::SendResponse()
   {
      TRACE("\n");
      TRACE("SendResponse\n");
      string msg;
      string strLine;
      string strTrace;
      strLine = m_response.attr("http_version").get_string() + " " + m_response.attr("http_status_code") + " " + m_response.attr("http_status");
      msg = strLine + "\r\n";
      strTrace = strLine;
      strTrace.replace("%", "%%");
      TRACE(strTrace + "\n");
      for(int32_t i = 0; i < m_response.m_propertysetHeader.m_propertya.get_size(); i++)
      {
         strLine = m_response.m_propertysetHeader.m_propertya[i].name() +
                   ": " +
                   m_response.m_propertysetHeader.m_propertya[i].get_string();
         msg += strLine + "\r\n";
         strTrace = strLine;
         strTrace.replace("%", "%%");
         TRACE(strTrace + "\n");
      }
      msg += "\r\n";
      Send( msg );
   }

   void sip_base_client_socket::SendResponseBody()
   {
      //SendBuf((const char *) response().file().get_data(), response().file().get_size());
   }


   void sip_base_client_socket::SendRequest()
   {
      string msg;
      msg = m_request.attr("http_method").get_string() + " " + m_request.attr("request_uri").get_string() + " " + m_request.attr("http_version").get_string() + "\r\n";
      for(int32_t i = 0; i < m_response.m_propertysetHeader.m_propertya.get_count(); i++)
      {
         msg += m_response.m_propertysetHeader.m_propertya[i].name() + ": " + m_response.m_propertysetHeader.m_propertya[i].get_string() + "\r\n";
      }
      msg += "\r\n";
      Send( msg );
   }


   string sip_base_client_socket::MyUseragent()
   {
      string version = "C++Sockets/";
   #ifdef _VERSION
      version += _VERSION;
   #endif
      return version;
   }


   void sip_base_client_socket::Reset()
   {
      m_bFirst = true;
      m_bHeader = true;
      m_bRequest = false;
      m_bResponse = false;
      SetLineProtocol(true);
      m_request.clear();
      m_response.clear();
   }




   bool sip_base_client_socket::IsRequest()
   {
      return m_bRequest;
   }


   bool sip_base_client_socket::IsResponse()
   {
      return m_bResponse;
   }





   void sip_base_client_socket::url_this(const string & url_in,string & protocol,string & host,port_t& port,string & url,string & file)
   {
      ::gen::parse pa(url_in,"/");
      protocol = pa.getword(); // http
      if (!strcasecmp(protocol, "https:"))
      {
   #ifdef HAVE_OPENSSL
         EnableSSL();
   #else
         Handler().LogError(this, "url_this", -1, "SSL not available", ::gen::log::level::warning);
   #endif
         port = 443;
      }
      else
      {
         port = 80;
      }
      host = pa.getword();
      if (strstr(host,":"))
      {
         ::gen::parse pa(host,":");
         pa.getword(host);
         port = static_cast<port_t>(pa.getvalue());
      }
      url = "/" + pa.getrest();
      {
         ::gen::parse pa(url,"/");
         string tmp = pa.getword();
         while (tmp.get_length())
         {
            file = tmp;
            tmp = pa.getword();
         }
      }
   } // url_this


   sip::request & sip_base_client_socket::request()
   {
      return m_request;
   }

   sip::response & sip_base_client_socket::response()
   {
      return m_response;
   }

   gen::property & sip_base_client_socket::inattr(const char * pszName)
   {
      return m_request.attr(pszName);
   }

   gen::property_set & sip_base_client_socket::inattrs()
   {
      return m_request.attrs();
   }

   gen::property & sip_base_client_socket::outattr(const char * pszName)
   {
      return m_response.attr(pszName);
   }

   gen::property_set & sip_base_client_socket::outattrs()
   {
      return m_response.attrs();
   }

   gen::property & sip_base_client_socket::inheader(const char * pszName)
   {
      return m_request.header(pszName);
   }

   gen::property_set & sip_base_client_socket::inheaders()
   {
      return m_request.headers();
   }

   gen::property & sip_base_client_socket::outheader(const char * pszName)
   {
      return m_response.header(pszName);
   }

   gen::property_set & sip_base_client_socket::outheaders()
   {
      return m_response.headers();
   }

   void sip_base_client_socket::OnHeader(const string & key,const string & value, const string & lowvalue)
   {
      //sip_base_client_socket::OnHeader(key, value);
      TRACE("  (request)OnHeader %s: %s\n", (const char *) key, (const char *) value);
      if(key == "cookie")
      {
         m_request.cookies().parse_header(value);
         m_response.cookies().parse_header(value);
      }
      else
         m_request.header(key) = value;
   }


   void sip_base_client_socket::OnHeaderComplete()
   {
      int32_t iStatusCode = m_response.attr("http_status_code").get_integer();
      string strMethod = m_response.attr("http_method");
      if(m_estate == state_free)
      {
         if(strMethod == "INVITE")
         {
            //m_estate = state_ringing;
   //         sip_event event;
            //event.m_eevent = sip_event::event_invite;
            //m_pcallback->on_event(&event);
         }
      }
      if(m_estate == state_waiting_call_trying)
      {
         if(iStatusCode >= 100 && iStatusCode < 200)
         {
            if(iStatusCode == 100)
            {
   //            m_estate = state_waiting_call_ringing;
            }
            else if(iStatusCode == 180)
            {
     //          m_estate = state_waiting_call_ok;
            }
            else
            {
       //        m_estate = state_waiting_call_ok;
            }
         }
      }
      if(m_estate == state_waiting_call_trying)
      {
         if(m_response.attr("http_status_code") == 200)
         {
         //   m_estate = state_waiting_call_ringing;
         }
      }
   /*   m_body_size_left = atol(m_request.header("content-length"));
      if (m_body_size_left > 0)
      {
         m_request.InitBody( m_body_size_left );
      }
      else
      {
         // execute
         Execute();
      }*/
   }


} // namespace sockets
