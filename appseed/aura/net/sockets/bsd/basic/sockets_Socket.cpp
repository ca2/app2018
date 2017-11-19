#include "framework.h" // #include "axis/net/sockets/bsd/sockets.h"
#include "aura/net/net_sockets.h"

#ifdef _WIN32
#else
#include <fcntl.h>
#endif
////#include <ctype.h>


#ifdef LINUX
#include <unistd.h>
#endif

#ifdef APPLEOS
#include <netdb.h>
#endif

namespace sockets
{


//
//#ifdef DEBUG
//#define DEB(x) x; fflush(stderr);
//#else
//#define DEB(x)
//#endif
//

//   // statics
//#ifdef _WIN32
//   WSAInitializer socket::m_winsock_init;
//#endif


   socket::socket(base_socket_handler& h) :
      ::object(h),
      base_socket(h)
   {

      m_iBindPort    = -1;
      m_dwStart      = ::get_tick_count();
      m_pcallback    = NULL;

   }


   socket::~socket()
   {

      //if(&Handler() != NULL)
      //{
      //   Handler().remove(this);
      //}

      if (m_socket != INVALID_SOCKET            && !m_bRetain         )
      {
         close();
      }

   }


   int32_t socket::close_socket(SOCKET s)
   {

      return ::closesocket(s);

   }


   void socket::close()
   {
      if (m_socket == INVALID_SOCKET) // this could happen
      {
         //if(!is_null(Handler()))
         //{
         //   log("socket::close", 0, "file descriptor invalid", ::aura::log::level_warning);
         //}
         return;
      }
      int32_t n;
      if ((n = close_socket(m_socket)) == -1)
      {
         //if(!is_null(Handler()))
         //{
         //   // failed...
         //   log("close", Errno, bsd_socket_error(Errno), ::aura::log::level_error);
         //}
      }
      //if(!is_null(Handler()))
      //{
      //   Handler().set(m_socket, false, false, false); // remove from fd_set's
      //   Handler().AddList(m_socket, LIST_CALLONCONNECT, false);
      //   Handler().AddList(m_socket, LIST_DETACH, false);
      //   Handler().AddList(m_socket, LIST_TIMEOUT, false);
      //   Handler().AddList(m_socket, LIST_RETRY, false);
      //   Handler().AddList(m_socket, LIST_CLOSE, false);
      //}
      m_socket = INVALID_SOCKET;
      //throw n;
   }


   SOCKET socket::CreateSocket(int32_t af, int32_t iType, const string & strProtocol)
   {

      SOCKET s;

      m_iSocketType = iType;

      m_strSocketProtocol = strProtocol;

      int32_t protno;

#ifdef ANDROID

      protno = 6;

      if (strProtocol.compare_ci("tcp") == 0)
      {

         protno = 6;

      }

#else

      protno = IPPROTO_TCP;

      {

         struct protoent *p = NULL;

         if (strProtocol.get_length())
         {

            p = getprotobyname(strProtocol);

            if (p == NULL)
            {

#ifdef DEBUG
               log("getprotobyname", Errno, bsd_socket_error(Errno), ::aura::log::level_fatal);
#endif

               SetCloseAndDelete();

               throw simple_exception(get_app(), string("getprotobyname() failed: ") + bsd_socket_error(Errno));

               return INVALID_SOCKET;

            }

         }

         if (p != NULL)
         {

            protno = p->p_proto;

         }

      }

#endif

      s = ::socket(af, iType, protno);

      if (s == INVALID_SOCKET)
      {
#ifdef DEBUG
         log("socket", Errno, bsd_socket_error(Errno), ::aura::log::level_fatal);
#endif
         SetCloseAndDelete();
         throw simple_exception(get_app(), string("socket() failed: ") + bsd_socket_error(Errno));
         return INVALID_SOCKET;
      }
      attach(s);
      OnOptions(af, iType, protno, s);
      attach(INVALID_SOCKET);
      return s;
   }





   void socket::set(bool bRead, bool bWrite, bool bException)
   {
      Handler().set(m_socket, bRead, bWrite, bException);
   }


   bool socket::Ready()
   {
      if (m_socket != INVALID_SOCKET && !CloseAndDelete())
         return true;
      return false;
   }

   bool socket::is_valid()
   {

      return Ready();

   }


   bool socket::SetNonblocking(bool bNb, SOCKET s)
   {
#ifdef _WIN32
      u_long l = bNb ? 1 : 0;
      int32_t n = ioctlsocket(s, FIONBIO, &l);
      if (n != 0)
      {
#ifdef DEBUG
         log("ioctlsocket(FIONBIO)", Errno, "");
#endif
         return false;
      }
      return true;
#else
      if (bNb)
      {
         if (fcntl(s, F_SETFL, O_NONBLOCK) == -1)
         {
#ifdef DEBUG
            log("fcntl(F_SETFL, O_NONBLOCK)", Errno, bsd_socket_error(Errno), ::aura::log::level_error);
#endif
            return false;
         }
      }
      else
      {
         if (fcntl(s, F_SETFL, 0) == -1)
         {
#ifdef DEBUG
            log("fcntl(F_SETFL, 0)", Errno, bsd_socket_error(Errno), ::aura::log::level_error);
#endif
            return false;
         }
      }
      return true;
#endif
   }


   ::net::address socket::get_peer_address() // const
   {
      //ASSERT(psa != NULL);
      //ASSERT(m_hSocket != INVALID_SOCKET);

      // gets the address of the socket at the other end
      ::net::address psa;
      socklen_t nLengthAddr = sizeof(SOCKADDR);
      if (getpeername(GetSocket(), psa.sa(), &nLengthAddr) == SOCKET_ERROR)
      {
         throw transfer_socket_exception(get_app(), _T("GetPeerName"));
      }
      return psa;
   }

   ::net::address socket::get_socket_address() // const
   {
      //ASSERT(psa != NULL);
      //ASSERT(m_hSocket != INVALID_SOCKET);

      // gets the address of the socket at this end
      ::net::address psa;
      socklen_t nLengthAddr = sizeof(SOCKADDR);
      if (getsockname(GetSocket(), psa.sa(), &nLengthAddr) == SOCKET_ERROR)
      {
         throw transfer_socket_exception(get_app(), _T("GetSockName"));
      }
      return psa;
   }

} // namespace sockets





