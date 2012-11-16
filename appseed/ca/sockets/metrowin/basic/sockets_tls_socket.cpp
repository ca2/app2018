#include "framework.h"
#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#endif

namespace sockets
{


   tls_socket::tls_socket(::sockets::socket_handler_base& h) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h)
   {
   }


   tls_socket::~tls_socket()
   {
   }

   void tls_socket::InitSSLClient()
   {
#ifdef HAVE_OPENSSL
      InitializeContext(m_strCat, TLSv1_client_method());
#endif
   }


} // namespace sockets


