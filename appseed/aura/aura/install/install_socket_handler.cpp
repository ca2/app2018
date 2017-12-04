#include "framework.h"


#include "install_socket_handler.h"


namespace install
{


   socket_handler::socket_handler(::aura::application * papp) :
      ::object(papp),
      ::sockets::socket_handler(papp),
      ::netserver::socket_handler(papp)
   {

   }


} // namespace install


