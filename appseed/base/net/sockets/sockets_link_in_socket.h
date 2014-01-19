#pragma once


namespace sockets
{


   class link_out_socket;


   class CLASS_DECL_BASE link_in_socket : public tcp_socket
   {
   public:

      socket *             m_in;
      link_out_socket *    m_out;
      event                m_eventFinished;


      link_in_socket(base_socket_handler& h);
      ~link_in_socket();


      void OnRead( char *buf, size_t n );
      void link_write(void * p, size_t size);


      static link_in_socket * from_server(httpd_socket * psocket);


      void server_to_link_in(httpd_socket * psocket);


   };

   
   typedef ptrptr(link_out_socket, link_in_socket) in_link_map; 


} // namespace sockets


