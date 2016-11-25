#pragma once

#ifdef BSD_STYLE_SOCKETS
#include <openssl/ossl_typ.h>
#endif


namespace sockets
{


   /** \defgroup basic Basic sockets */
   /** socket axis class.
   \ingroup basic */
   class CLASS_DECL_AXIS socket :
      virtual public ::sockets::base_socket
   {
   public:



//#ifdef WINDOWSEX
#ifdef WINDOWS

      static WSAInitializer   m_winsock_init; ///< Winsock initialization singleton class

#endif





      /** "Default" constructor */
      socket(base_socket_handler & h);

      virtual ~socket();




      /** create a socket file descriptor.
      \param af Address family AF_INET / AF_INET6 / ...
      \param type SOCK_STREAM / SOCK_DGRAM / ...
      \param protocol "tcp" / "udp" / ... */
      SOCKET CreateSocket(int32_t af,int32_t type,const string & protocol = "");



      /** close connection immediately - internal use.
      \sa SetCloseAndDelete */
      virtual void close();

      /** add file descriptor to sockethandler fd_set's. */
      void set(bool bRead,bool bWrite,bool bException = true);

      /** Returns true when socket file descriptor is valid
      and socket is not about to be closed. */
      virtual bool Ready();

      virtual bool is_valid();

      virtual int32_t close_socket(SOCKET s);




      /** SSL; get pointer to ssl context structure. */
      virtual SSL_CTX *GetSslContext() { return NULL; }
      /** SSL; get pointer to ssl structure. */
      virtual SSL *GetSsl() { return NULL; }
      //@}


      using base_socket::SetNonblocking;
      bool SetNonblocking(bool bNb, SOCKET s);


      virtual ::net::address get_peer_address();
      virtual ::net::address get_socket_address();



   };


} // namespace sockets




