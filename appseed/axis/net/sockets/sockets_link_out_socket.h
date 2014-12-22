#pragma once


namespace sockets
{


   class link_in_socket;

   
   class CLASS_DECL_AXIS link_out_socket : public tcp_socket
   {
   public:
      

      link_in_socket * m_in;
      socket * m_out;
      

      link_out_socket(base_socket_handler & h);
      virtual ~link_out_socket();

      using ::sockets::tcp_socket::OnRead;
      virtual void OnRead( char *buf, size_t n );
      void link_write(void * p, size_t size);


      void server_to_link_out(httpd_socket * psocket);

   };



} // namespace sockets


