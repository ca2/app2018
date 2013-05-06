#include "framework.h"


namespace ca
{


   http_get_socket::http_get_socket(::sockets::socket_handler & handler, const string & url) :
      ::ca::ca(handler.get_app()),
      ::sockets::socket(handler),
      ::sockets::stream_socket(handler),
      ::sockets::tcp_socket(handler),
      ::sockets::http_socket(handler),
      ::sockets::http_tunnel(handler),
      ::sockets::http_client_socket(handler, url),
      ::sockets::http_get_socket(handler, url)
      
   {
      m_pcookies = ::null();
   }

      
   http_get_socket::http_get_socket(::sockets::socket_handler & handler, const string & host, port_t port, const string & url) :
      ::ca::ca(handler.get_app()),
      ::sockets::socket(handler),
      ::sockets::stream_socket(handler),
      ::sockets::tcp_socket(handler),
      ::sockets::http_socket(handler),
      ::sockets::http_tunnel(handler),
      ::sockets::http_client_socket(handler, url),
      ::sockets::http_get_socket(handler, host, port, url)
   {
      m_pcookies = ::null();
   }

   void http_get_socket::OnDataArrived(const char *, size_t len)
   {
      UNREFERENCED_PARAMETER(len);
   }

   void http_get_socket::OnHeader(id key, const string & value)
   {

      ::sockets::http_get_socket::OnHeader(key, value);

      if(key == __id(location))
      {

         m_strHeaderLocation = value;

      }
      else if(key == __id(set_cookie))
      {

         if(m_pcookies != ::null())
         {

            m_pcookies->add(value);

         }

      }

   }

   string http_get_socket::MyUseragent()
   {
	   return "ca-sockets/10.0.2";
   }


} // namespace ca



