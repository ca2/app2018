#pragma once


   namespace sockets
   {


      class CLASS_DECL_ca2 sockets :
         virtual public ::ca::section
      {
      public:

#ifdef HAVE_OPENSSL
         ssl_client_context_map                       m_clientcontextmap;
#endif
         AjpBaseSocket::Initializer *                 m_pajpbasesocketinit;

         count                                        m_countHttpPostBoundary;
         mutex                                        m_mutexHttpPostBoundary;

         resolv_socket::cache_t                       m_resolvcache;
         resolv_socket::timeout_t                     m_resolvtimeout;
         mutex                                        m_mutexResolvCache;



         sockets();


         void on_application_signal(::ca::application_signal_object * papplicationsignal);
         virtual void http_config_proxy(const char * pszUrl, http_tunnel * psocket);


      };


   } // namespace sockets


