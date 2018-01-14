#include "framework.h"

#include "netserver_socket_handler.h"

namespace netserver
{


   socket_handler::socket_handler(::aura::application * papp) :
      ::object(papp),
      ::sockets::socket_handler(papp)
   {

   }


} // namespace netserver




