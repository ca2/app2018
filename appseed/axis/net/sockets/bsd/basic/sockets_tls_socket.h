#pragma once




   namespace sockets
   {

      class CLASS_DECL_AXIS tls_socket : 
         virtual public tcp_socket
      {
      public:


         tls_socket(base_socket_handler& );
         ~tls_socket();


         virtual void InitSSLClient();

      };



   } // namespace sockets




