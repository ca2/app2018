//#include "framework.h"


namespace sockets
{


   resolv_server::resolv_server(::aura::application * papp, port_t port) :
      ::object(papp),
      thread(papp),
      simple_thread(papp),
      go_thread(papp),
      m_quit(false),
      m_port(port),
      m_ready(false)
   {

      start();

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
      while (!m_quit && get_run_thread())
      {
         h.select(0, 500000);
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
