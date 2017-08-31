#include "framework.h"


#if defined(INSTALL_SUBSYSTEM)


namespace install
{


   socket_thread::socket_thread(::aura::application * papp) :
      ::object(papp),
      thread(papp),
      ::netserver::socket_thread(papp)
   {

      m_iSsl = 1;

   }

   socket_thread::~socket_thread()
   {


   }


   void socket_thread::install_message_routing(::message::sender * pinterface)
   {

   }

   ::netserver::socket_handler * socket_thread::create_socket_handler()
   {

      return new ::install::socket_handler(get_app());


   }


   ::sockets::listen_socket_base * socket_thread::create_listen_socket()
   {

      return new ::sockets::listen_socket < socket >(*m_psockethandler);

   }


} // namespace install


#endif


