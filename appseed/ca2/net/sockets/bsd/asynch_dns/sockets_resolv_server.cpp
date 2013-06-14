#include "framework.h"

namespace sockets
{

   resolv_server::resolv_server(sp(::ca2::application) papp, port_t port) :
      ca2(papp),
      thread(papp),
      simple_thread(papp),
      go_thread(papp),
      m_quit(false),
      m_port(port),
      m_ready(false)
   {
      start();
   }

   resolv_server::resolv_server(const resolv_server& s) : 
      ::ca2::ca2(((resolv_server&)s).get_app()),
      thread(((resolv_server&)s).get_app()),
      simple_thread(((resolv_server&)s).get_app()),
      go_thread(((resolv_server&)s).get_app())
   {
   }


   resolv_server::~resolv_server()
   {
   }


   void resolv_server::go()
   {
   //   trace_log log;
      socket_handler h(get_app());
      listen_socket<resolv_socket> l(h);

      if (l.Bind("127.0.0.1", m_port))
      {
         return;
      }
      h.add(&l);

      m_ready = true;
      while (!m_quit && get_run())
      {
         h.Select(0, 500000);
      }
   }


   void resolv_server::Quit()
   {
      m_quit = true;
   }


   bool resolv_server::Ready()
   {
      return m_ready;
   }


} // namespace sockets