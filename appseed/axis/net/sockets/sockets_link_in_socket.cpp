#include "framework.h" // from "axis/net/net_sockets.h"
#include "axis/net/net_sockets.h"


namespace sockets
{



   link_in_socket::link_in_socket(base_socket_handler & h) :
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h),
      tcp_socket(h, 32000, 32000),
      m_in(NULL),
      m_out(NULL),
      m_eventFinished(h.get_app())
   {
   }


   link_in_socket::~link_in_socket()
   {
   }


   void link_in_socket::OnRead( char *buf, size_t n )
   {

      m_out->link_write(buf, n);

   }


   void link_in_socket::link_write(void * p, size_t n)
   {
      
      m_in->write((const char *) p, n);

   }


   void link_in_socket::server_to_link_in(httpd_socket * psocket)
   {
      socket_handler & h = dynamic_cast < socket_handler & > (psocket->Handler());
      POSITION pos = h.m_sockets.get_start_position();
      sp(::sockets::base_socket) psocket2;
      SOCKET key;
      while(pos != NULL)
      {
         h.m_sockets.get_next_assoc(pos, key, psocket2);
         if(psocket2 == psocket)
         {
            h.m_sockets.set_at(key, this);
         }
      }
//      m_ssl = psocket->GetSsl();
      m_socket = psocket->m_socket;

      m_bConnecting        = psocket->m_bConnecting; ///< Flag indicating connection in progress
      m_connect_timeout    = psocket->m_connect_timeout; ///< Connection timeout (seconds)
      m_flush_before_close = psocket->m_flush_before_close; ///< Send all data before closing (default true)
      m_connection_retry   = psocket->m_connection_retry; ///< Maximum connection retries (tcp)
      m_retries            = psocket->m_retries; ///< Actual number of connection retries (tcp)
      m_call_on_connect    = psocket->m_call_on_connect; ///< OnConnect will be called next base_socket_handler cycle if true
      m_b_retry_connect    = psocket->m_b_retry_connect; ///< Try another connection attempt next base_socket_handler cycle
      m_shutdown           = psocket->m_shutdown; ///< Shutdown status

      m_bSsl               = psocket->m_bSsl;
      m_bReconnect         = psocket->m_bReconnect;
      m_bSsl               = psocket->m_bSsl;
      m_bSslServer         = psocket->m_bSslServer;
      m_bEnableSsl         = psocket->m_bEnableSsl;

   }

   link_in_socket * link_in_socket::from_server(httpd_socket * psocket)
   {
      
      link_in_socket * pinsocket = new link_in_socket(psocket->Handler());

	   pinsocket->m_in = psocket;
	
      pinsocket->m_memfileInput.transfer_from(psocket->m_memfileInput);

      pinsocket->server_to_link_in(psocket);

      psocket->m_bEnd = true;

      return pinsocket;

   }

} // namespace sockets




