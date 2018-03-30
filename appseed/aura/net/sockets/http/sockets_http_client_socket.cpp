#include "framework.h" // from "aura/net/net_sockets.h"
#include "aura/net/net_sockets.h"


#ifdef LINUX

#include <sys/time.h>

#endif


string op_sys()
{

#if defined(WINDOWSEX)
   return "Windows";
#elif defined(LINUX)
   return "Linux";
#elif defined(MACOS)
   return "OS X";
#elif defined(APPLE_IOS)
   return "iOS";
#elif defined(ANDROID)
   return "Android";
#elif defined(METROWIN)
   return "Windows Store";
#else
   return "";
#endif

}



namespace sockets
{


   http_client_socket::http_client_socket(base_socket_handler& h) :
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_socket(h),
      http_tunnel(h),
      m_content_length((size_t)-1),
      m_content_ptr(0),
      m_b_complete(false),
      m_b_close_when_complete(false),
      m_memoryfile(h.get_app()),
      m_mutexData(h.get_app())
   {

      m_progress.m_scalar.m_psource = this;
      m_progress.m_scalar.m_escalar = scalar_download_size;

      m_pfile           = NULL;
      m_iFinalSize      = -1;

      memcnts_inc(this);

   }


   http_client_socket::http_client_socket(base_socket_handler & h, const string & strUrlParam) :
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_socket(h),
      http_tunnel(h),
      m_content_length((size_t)-1),
      m_content_ptr(0),
      m_b_complete(false),
      m_b_close_when_complete(false),
      m_memoryfile(h.get_app()),
      m_mutexData(h.get_app())
   {

      string strRequestUri;

      url_this(strUrlParam, m_protocol, m_host, m_port, strRequestUri, m_url_filename);

      m_strHost                                 = m_host;
      m_request.header(__id(host))              = m_host;
      m_request.attr(__id(http_protocol))       = m_protocol;
      m_request.attr(__id(request_uri))         = strRequestUri;
      m_response.attr(__id(request_uri))        = strRequestUri;
      m_strUrl                                  = strUrlParam;

      if (m_host.is_empty())
      {

         m_strInitSSLClientContext = System.url().get_server(strRequestUri);

      }
      else
      {

         m_strInitSSLClientContext = m_host;

      }

      m_strConnectHost                          = m_host;
      m_iConnectPort                            = m_port;

      m_pfile                                   = NULL;
      m_iFinalSize                              = -1;

      memcnts_inc(this);

   }


   http_client_socket::~http_client_socket()
   {

      memcnts_dec(this);

   }


   void http_client_socket::OnConnect()
   {

      m_request.attr(__id(http_method)) = http_method_string(m_emethod);

      http_tunnel::OnConnect();

   }


   void http_client_socket::OnFirst()
   {
      if (!IsResponse())
      {
#ifdef DEBUG
         log("OnFirst", 0, "Response expected but not received - aborting", ::aura::log::level_fatal);
#endif
         SetCloseAndDelete();
      }
      m_content = m_response.attr(__id(http_version)) + " " +
                  m_response.attr(__id(http_status_code)) + " " +
                  m_response.attr(__id(http_status)) + "\r\n";
   }


   void http_client_socket::OnHeader(id key, const string & value)
   {
      m_content += key + ": " + value + "\r\n";
      m_response.m_propertysetHeader[key] = value;
      if (key == __id(content_length))
      {
         m_content_length = atoi(value);
      }
      else if (key == __id(content_type))
      {
         m_content_type = value;
      }
      else if (key == __id(set_cookie))
      {
         m_response.m_cookies.add(value);
      }
   }


   void http_client_socket::OnHeaderComplete()
   {

      http_tunnel::OnHeaderComplete();

      m_memoryfile.Truncate(0);

      if(m_content_length != ((size_t) (-1)))
      {
         m_memoryfile.allocate_internal(m_content_length);
         if(outheader(__id(content_encoding)).compare_value_ci("gzip") != 0
               && (m_response.attr(__id(http_status_code)) < 300 || m_response.attr(__id(http_status_code)) >= 400))
         {

            m_iFinalSize = m_content_length;
         }
      }

      m_memoryfile.seek_to_begin();

      if(::str::ends(m_strUrl, "arialuni.ttf"))
      {
         TRACE("Debug Here : arialuni.ttf");
      }


      if(m_content_length > 0)
      {
         m_event.ResetEvent();
         m_bExpectResponse = true;
      }
      else
      {
         m_bExpectResponse = false;
      }

      if(::str::ends(m_strUrl, "en_us_international.xml"))
      {
         TRACE("Debug Here");
      }


      if(::str::ends(m_strUrl, "text_select.xml"))
      {
         TRACE("Debug Here");
      }


   }


   void http_client_socket::OnDataComplete()
   {

      m_b_complete = true;

      if (outheader(__id(content_encoding)).compare_value_ci("gzip") == 0)
      {

         ASSERT(System.m_pcompress != NULL);

         Application.compress_ungz(*m_memoryfile.get_memory());

      }

      if(m_pfile != NULL && (m_response.attr(__id(http_status_code)) < 300 || m_response.attr(__id(http_status_code)) >= 400))
      {

         m_pfile->write(m_memoryfile.get_data(), m_memoryfile.get_size());

      }

      OnContent();

      if(!m_bNoClose || m_b_close_when_complete)
      {

         SetCloseAndDelete();

      }

      m_bExpectRequest = false;

      m_bExpectResponse = false;

   }


   void http_client_socket::OnData(const char *buf,size_t len)
   {

      if(m_response.attr(__id(http_status_code)).int32() >= 300 && m_response.attr(__id(http_status_code)).int32() <= 399)
      {

         return;

      }

      if(m_pfile != NULL)
      {

         if(outheader(__id(content_encoding)).compare_value_ci("gzip") != 0)
         {

            m_pfile->write(buf,len);

            return;

         }
         else
         {

            m_memoryfile.write(buf,len);

         }

      }
      else
      {

         m_memoryfile.write(buf,len);

      }

      OnDataArrived(buf, len);

      increment_scalar(scalar_download_size, len);

      m_event.ResetEvent();

      m_bExpectResponse = true;

   }


   void http_client_socket::OnDataArrived(const char * buf, size_t len)
   {

      UNREFERENCED_PARAMETER(buf);
      UNREFERENCED_PARAMETER(len);
   }


   void http_client_socket::OnDelete()
   {
      if (!m_b_complete)
      {
         m_b_complete = true;
         OnContent();
      }
   }

   const string & http_client_socket::GetContent()
   {
      return m_content;
   }

   size_t http_client_socket::GetContentLength()
   {
      if(m_content_length == ((size_t)-1))
      {
         return m_memoryfile.get_size();
      }
      else
      {
         return m_memoryfile.get_size();
      }
   }


   size_t http_client_socket::GetContentPtr()
   {
      return m_content_ptr;
   }

   size_t http_client_socket::GetPos()
   {
      return m_content_ptr;
   }

   bool http_client_socket::Complete()
   {
      return m_b_complete;
   }

   const unsigned char *http_client_socket::GetDataPtr() const
   {

      return m_memoryfile.get_data();

   }

   size_t http_client_socket::GetDataLength() const
   {

      return m_memoryfile.get_size();

   }


   void http_client_socket::OnContent()
   {
   }

   void http_client_socket::SetCloseOnComplete(bool x)
   {
      m_b_close_when_complete = x;
   }

   const string & http_client_socket::GetUrlProtocol()
   {
      return m_protocol;
   }

   const string & http_client_socket::GetUrlFilename()
   {
      return m_url_filename;
   }

   const string & http_client_socket::GetContentType()
   {
      return m_content_type;
   }

   void http_client_socket::Url(const string & url_in, string & host, port_t & port)
   {
      string url;
      url_this(url_in, m_protocol, m_host, m_port, url, m_url_filename);
      m_request.attr("url") = url;
      host = System.url().get_server(url);
      port = (port_t) System.url().get_port(url);
   }


   void http_client_socket::request_url(string strUrlParam)
   {
      string strRequestUri;

      url_this(strUrlParam, m_protocol, m_host, m_port, strRequestUri, m_url_filename);

      m_request.attr(__id(http_protocol))     = m_protocol;
      m_request.header(__id(host))                   = m_host;
      m_request.attr(__id(request_uri))       = strRequestUri;
      m_response.attr(__id(request_uri))      = strRequestUri;

      m_strUrl = strUrlParam;

      m_pfile = NULL;
   }

   void http_client_socket::on_set_scalar(e_scalar escalar,int64_t iValue,int iFlags)
   {

      if (escalar == scalar_download_size)
      {

         m_content_ptr = (memory_position_t) iValue;

      }
      else
      {

         return ::int_scalar_source::on_set_scalar(escalar, iValue, iFlags);

      }

   }


   void http_client_socket::get_scalar_minimum(e_scalar escalar, int64_t & i)
   {

      if (escalar == scalar_download_size)
      {

         i = 0;

      }
      else
      {

         ::int_scalar_source::get_scalar_minimum(escalar, i);

      }

   }

   void http_client_socket::get_scalar(e_scalar escalar, int64_t & i)
   {

      if (escalar == scalar_download_size)
      {

         i = m_content_ptr;

      }
      else
      {

         ::int_scalar_source::get_scalar(escalar, i);

      }

   }

   void http_client_socket::get_scalar_maximum(e_scalar escalar, int64_t & i)
   {

      if (escalar == scalar_download_size)
      {

         i = m_content_length;

      }
      else
      {

         ::int_scalar_source::get_scalar_minimum(escalar, i);

      }

   }

   CLASS_DECL_AURA string http_method_string(e_http_method emethod)
   {

      switch (emethod)
      {
      case http_method_get:

         return "GET";

      case http_method_post:

         return "POST";

      case http_method_put:

         return "PUT";

      default:

         return "GET";

      }

   }


   CLASS_DECL_AURA e_http_method string_http_method(const string & str)
   {

      string strMethod(str);

      strMethod.make_lower();

      if (strMethod == "get")
      {

         return http_method_get;

      }
      else if (strMethod == "post")
      {

         return http_method_post;

      }
      else if (strMethod == "put")
      {

         return http_method_put;

      }
      else
      {

         return http_method_get;

      }

   }


   string http_client_socket::MyUseragent()
   {

      string strAddUp;

      if (Application.m_strHttpUserAgentToken.has_char() && Application.m_strHttpUserAgentVersion.has_char())
      {

         strAddUp = Application.m_strHttpUserAgentToken + "/" + Application.m_strHttpUserAgentVersion;

      }
      else
      {

         strAddUp = "ca2/1.0";

      }

#ifdef WINDOWS
      return "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (ca2, like Gecko) " + strAddUp;
#elif defined(MACOS)
      return "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12) AppleWebKit/537.36 (ca2, like Gecko) " + strAddUp;
#elif defined(LINUX)
      return "Mozilla/5.0 (Linux; x86_64) AppleWebKit/537.36 (ca2, like Gecko) " + strAddUp;
#elif defined(APPLE_IOS)
      return "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12) AppleWebKit/537.36 (ca2, like Gecko) " + strAddUp;
#elif defined(ANDROID)
      return "Mozilla/5.0 (Android; x86) AppleWebKit/537.36 (ca2, like Gecko) " + strAddUp;
#else
      _throw(todo(get_app()));
#endif
//      string str;
//
//      string strSockets = "ca2-sockets/0.1";
//
//      string strOpSys = op_sys();
//
//      if (Application.m_strAppName.has_char())
//      {
//
//         str += Application.m_strAppName;
//
//         str += " (";
//
//         if (strOpSys.has_char())
//         {
//
//            str += strOpSys;
//
//            str += ";";
//
//         }
//
//         str += strSockets;
//
//         str += ";)";
//
//      }
//      else
//      {
//
//         str += strSockets;
//
//         str += " (";
//
//         if (strOpSys.has_char())
//         {
//
//            str += strOpSys;
//
//            str += ";";
//
//         }
//
//         str += ")";
//
//      }
//
//      return str;

   }

} // namespace sockets


namespace http
{

   session::session(::aura::application * papp) :
      m_handler(papp)
   {

      //m_handler.EnablePool();

   }


   session::~session()
   {


   }

}
