#include "framework.h" // from "aura/net/net_sockets.h"
#include "aura/net/net_sockets.h"


namespace sockets
{


   void trace_logger::sockets_log(base_socket_handler * phandler, base_socket * sock, const string & strUser, int32_t iError, const string & strSystem, ::aura::log::e_level elevel)
   {

      string strLevel = ::log_level_name(elevel);

      if (sock)
      {
         Sys(phandler->m_papp).log().trace("fd %d :: %s: %d %s (%s)\n", sock->GetSocket(), strUser.c_str(), iError, strSystem.c_str(), strLevel.c_str());
      }
      else
      {
         Sys(phandler->m_papp).log().trace("%s: %d %s (%s)\n", strUser.c_str(), iError, strSystem.c_str(), strLevel.c_str());
      }

   }


} // namespace sockets



