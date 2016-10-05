#include "framework.h" // #include "axis/net/sockets/bsd/sockets.h"
#include "axis/net/net_sockets.h"


#include <openssl/ssl.h>


#include <fcntl.h>
//#include <assert.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#ifndef ETIMEDOUT
   #define ETIMEDOUT       138
#endif

#if defined(LINUX)
   #include <signal.h>
   #include <unistd.h>
#endif


namespace sockets
{

#ifdef LINUX
   // ssl_sigpipe_handle ---------------------------------------------------------
   void ssl_sigpipe_handle(int x);
#endif

   //#ifdef DEBUG
   //#define DEB(x) x
   //#else
   //#define DEB(x)
   //#endif


   // statics
#ifdef HAVE_OPENSSL
   //SSLInitializer tcp_socket::m_ssl_init;
#endif


// thanks, q
#ifdef _MSC_VER
   #pragma warning(disable:4355)
#endif
   tcp_socket::tcp_socket(base_socket_handler& h):
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h)
      ,ibuf(TCP_BUFSIZE_READ)
      ,m_b_input_buffer_disabled(false)
      ,m_bytes_sent(0)
      ,m_bytes_received(0)
      ,m_obuf_top(NULL)
      ,m_transfer_limit(0)
      ,m_output_length(0)
#ifdef HAVE_OPENSSL
      ,m_ssl_ctx(NULL)
      ,m_ssl(NULL)
      ,m_sbio(NULL)
#endif
      ,m_socks4_state(0)
      ,m_resolver_id(0)
      ,m_bReconnect(false)
      ,m_bTryingReconnect(false)
   {
      m_bClientSessionSet = false;
      m_memRead.allocate(TCP_BUFSIZE_READ + 1);
      m_bCertCommonNameCheckEnabled = true;
      m_pmutexSslCtx = NULL;
   }
#ifdef _MSC_VER
   #pragma warning(default:4355)
#endif


#ifdef _MSC_VER
   #pragma warning(disable:4355)
#endif
   tcp_socket::tcp_socket(base_socket_handler& h,size_t isize,size_t osize):
      object(h.get_app()),
      base_socket(h),
      socket(h),
      stream_socket(h)
      ,ibuf(isize)
      ,m_b_input_buffer_disabled(false)
      ,m_bytes_sent(0)
      ,m_bytes_received(0)
      ,m_obuf_top(NULL)
      ,m_transfer_limit(0)
      ,m_output_length(0)
#ifdef HAVE_OPENSSL
      ,m_ssl_ctx(NULL)
      ,m_ssl(NULL)
      ,m_sbio(NULL)
#endif
      ,m_socks4_state(0)
      ,m_resolver_id(0)
      ,m_bReconnect(false)
      ,m_bTryingReconnect(false)
   {
      m_bClientSessionSet = false;
      m_memRead.allocate(TCP_BUFSIZE_READ + 1);
      m_bCertCommonNameCheckEnabled = true;
      m_pmutexSslCtx = NULL;
      UNREFERENCED_PARAMETER(osize);
   }
#ifdef _MSC_VER
   #pragma warning(default:4355)
#endif


   tcp_socket::~tcp_socket()
   {

      if(m_socket != INVALID_SOCKET) // this could happen
      {

         close();

      }


   }


   /*
   bool tcp_socket::open(in_addr ip,port_t port,bool skip_socks)
   {
      address ad(get_app(), ip, port);
      address local(get_app());
      return open(ad, local, skip_socks);
   }


   bool tcp_socket::open(in6_addr ip,port_t port,bool skip_socks)
   {
      address ad(get_app(), ip, port);
      return open(ad, skip_socks);
   }
   */

   bool tcp_socket::open(const ::net::address & ad,bool skip_socks)
   {
      ::net::address bind_ad("0.0.0.0",0);
      return open(ad,bind_ad,skip_socks);
   }


   bool tcp_socket::open(const ::net::address & ad,const ::net::address & bind_ad,bool skip_socks)
   {
      output_debug_string("tcp_socket::open address = " + ad.get_display_number() + " : " + ::str::from(ad.get_service_number()));
      if(!ad.is_valid())
      {
         log("open",0,"Invalid ::net::address",::aura::log::level_fatal);
         SetCloseAndDelete();
         return false;
      }
      if(Handler().get_count() >= FD_SETSIZE)
      {
         log("open",0,"no space left in fd_set",::aura::log::level_fatal);
         SetCloseAndDelete();
         return false;
      }
      SetConnecting(false);
      SetSocks4(false);
      // check for pooling
      if(Handler().PoolEnabled())
      {
         base_socket_handler::pool_socket *pools = Handler().FindConnection(SOCK_STREAM,"tcp",ad);
         if(pools)
         {
            CopyConnection(pools);
            delete pools;

            SetIsClient();
            SetCallOnConnect(); // base_socket_handler must call OnConnect
            log("SetCallOnConnect",0,"Found pooled connection",::aura::log::level_info);
            return true;
         }
      }
      // if not, create new connection
      SOCKET s = CreateSocket(ad.get_family(),SOCK_STREAM,"tcp");
      if(s == INVALID_SOCKET)
      {
         return false;
      }
      // socket must be nonblocking for async connect
      if(!SetNonblocking(true,s))
      {
         SetCloseAndDelete();
         ::closesocket(s);
         return false;
      }
      SetIsClient(); // client because we connect
      SetClientRemoteAddress(ad);
      int32_t n = 0;
      if(bind_ad.get_service_number() != 0)
      {
         bind(s,bind_ad.sa(),bind_ad.sa_len());
      }
      in_addr addrSocks4 = GetSocks4Host();
      if(!skip_socks && ISNT_ZERO(addrSocks4) && GetSocks4Port())
      {
         ::net::address sa(GetSocks4Host(),GetSocks4Port());
         {
            string sockshost;
            Session.sockets().net().convert(sockshost,GetSocks4Host());
            log("open",0,"Connecting to socks4 server @ " + sockshost + ":" + ::str::from(GetSocks4Port()),::aura::log::level_info);
         }
         SetSocks4();
         n = connect(s,sa.sa(),sa.sa_len());
         SetRemoteHostname(sa);
      }
      else
      {
         n = connect(s,ad.sa(),ad.sa_len());
         SetRemoteHostname(ad);
      }
      if(n == -1)
      {
#ifdef _WIN32
         // check error code that means a connect is in progress
         int32_t iError = ::WSAGetLastError();
         if(iError == WSAEWOULDBLOCK || iError == 0)
#else
         int32_t iError = Errno;
         if(iError == EINPROGRESS)
#endif
         {
            attach(s);
            SetConnecting(true); // this flag will control fd_set's
         }
         else if(Socks4() && Handler().Socks4TryDirect()) // retry
         {
            ::closesocket(s);
            return open(ad,true);
         }
         else if(Reconnect())
         {
            string strError = StrError(iError);
            log("connect: failed, reconnect pending",iError,StrError(iError),::aura::log::level_info);
            attach(s);
            SetConnecting(true); // this flag will control fd_set's
         }
         else
         {
            string strError = StrError(iError);
            log("connect: failed",iError,StrError(iError),::aura::log::level_fatal);
            SetCloseAndDelete();
            ::closesocket(s);
            return false;
         }
      }
      else
      {
         attach(s);
         SetCallOnConnect(); // base_socket_handler must call OnConnect
      }

      // 'true' means connected or connecting(not yet connected)
      // 'false' means something failed
      return true; //!Connecting();
   }


   bool tcp_socket::open(const string &host,port_t port)
   {
      SetCloseAndDelete(false);
      if(IsIpv6())
      {
         if(!Handler().ResolverEnabled() || Session.sockets().net().isipv6(host))
         {
            in6_addr a;
            if(!Session.sockets().net().convert(a,host))
            {
               SetCloseAndDelete();
               return false;
            }
            ::net::address ad(a,port);
            ::net::address addrLocal;
            if(!open(ad,addrLocal))
               return false;
            return true;
         }
         m_resolver_id = Resolve6(host,port);
         return true;
      }
      if(!Handler().ResolverEnabled() || Session.sockets().net().isipv4(host))
      {
         in_addr l;
         if(!Session.sockets().net().convert(l,host))
         {
            SetCloseAndDelete();
            return false;
         }
         ::net::address ad(l,port);
         ::net::address addrLocal;
         if(!open(ad,addrLocal))
            return false;
         return true;
      }
      // resolve using async resolver thread
      m_resolver_id = Resolve(host,port);
      return true;
   }


   void tcp_socket::OnResolved(int32_t id,const ::net::address & a)
   {

      TRACE("tcp_socket::OnResolved id %d addr %s port %d\n",id,Session.sockets().net().canonical_name(a).c_str(),a.u.s.m_port);

      if(id == m_resolver_id)
      {
         if(a.is_valid() && a.u.s.m_port)
         {
            ::net::address addrLocal;
            if(open(a,addrLocal))
            {
               if(!Handler().Valid(this))
               {
                  Handler().add(this);
               }
            }
         }
         else
         {
            log("OnResolved",0,"Resolver failed",::aura::log::level_fatal);
            SetCloseAndDelete();
         }
      }
      else
      {
         log("OnResolved",id,"Resolver returned wrong job id",::aura::log::level_fatal);
         SetCloseAndDelete();
      }
   }




   memory_size_t tcp_socket::recv(void * buf,memory_size_t nBufSize)
   {

      memory_size_t n = nBufSize;

#ifdef HAVE_OPENSSL

      if(IsSSL())
      {

         if(!Ready())
         {

            TRACE("tcp_socket::recv not ready");
            return 0;

         }

         n = SSL_read(m_ssl,buf,(int)nBufSize);
         if(n == -1)
         {
            n = SSL_get_error(m_ssl,(int)n);
            switch(n)
            {
            case SSL_ERROR_NONE:
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
               break;
            case SSL_ERROR_ZERO_RETURN:
               TRACE("SSL_read() returns zero - closing socket\n");
               OnDisconnect();
               SetCloseAndDelete(true);
               SetFlushBeforeClose(false);
               SetLost();
               break;
            case SSL_ERROR_SYSCALL:
               TRACE("SSL read problem, errcode = %d (SSL_ERROR_SYSCALL) errno = %d \n",n,errno);
               OnDisconnect();
               SetCloseAndDelete(true);
               SetFlushBeforeClose(false);
               SetLost();
               break;
            default:
               TRACE("SSL read problem, errcode = %d\n",n);
               OnDisconnect();
               SetCloseAndDelete(true);
               SetFlushBeforeClose(false);
               SetLost();
            }
            TRACE("tcp_socket::recv ssl error(1)");
         }
         else if(!n)
         {
            OnDisconnect();
            SetCloseAndDelete(true);
            SetFlushBeforeClose(false);
            SetLost();
            SetShutdown(SHUT_WR);
            //TRACE("tcp_socket::recv ssl disconnect(2)");

         }
         else if(n > 0 && n <= nBufSize)
         {
            return n;
         }
         else
         {
            log("tcp_socket::recv(ssl)",(int)n,"abnormal value from SSL_read",::aura::log::level_error);
            TRACE("tcp_socket::recv ssl abnormal value from SSL_read(3)");

         }
      }
      else
#endif // HAVE_OPENSSL
      {
#if defined(APPLEOS) || defined(SOLARIS)
         //         n = (int32_t) recv(GetSocket(), buf, nBufSize, SO_NOSIGPIPE);
         n = (int32_t) ::recv(GetSocket(),buf,nBufSize,0);

#else
         n = ::recv(GetSocket(),(char *)buf,(int)nBufSize,MSG_NOSIGNAL);
#endif
         if(n == -1)
         {
            log("recv",Errno,StrError(Errno),::aura::log::level_fatal);
            OnDisconnect();
            SetCloseAndDelete(true);
            SetFlushBeforeClose(false);
            SetLost();
            TRACE("tcp_socket::recv (B1) recv error(" + string(StrError(Errno)) + ")");
         }
         else if(!n)
         {
            int32_t iError = ::WSAGetLastError();
            if (iError == WSAEWOULDBLOCK)
            {
               TRACE("tcp_socket::recv 0 No Error or WOULD BLOCK");
            }
            else
            {
               TRACE("tcp_socket::recv (B2) recv disconnect");
               OnDisconnect();
               SetCloseAndDelete(true);
               SetFlushBeforeClose(false);
               SetLost();
               SetShutdown(SHUT_WR);
            }
            
         }
         else if(n > 0 && n <= nBufSize)
         {
            return n;
         }
         else
         {
            log("tcp_socket::recv",(int32_t)n,"abnormal value from recv",::aura::log::level_error);
            TRACE("tcp_socket::recv (B3) recv abnormal value from recv");
         }

      }

      return n;

   }


   memory_size_t tcp_socket::read(void * buf,memory_size_t nBufSize)
   {

      memory_size_t n = nBufSize;

      n = recv(buf,nBufSize);

      if(n > 0 && n <= nBufSize)
      {

         m_bytes_received += n;

         if(GetTrafficMonitor())
         {

            GetTrafficMonitor()->write(buf,n);

         }

         if(!m_b_input_buffer_disabled)
         {

            try
            {


               ibuf.write(buf,n);

            }
            catch(...)
            {

               log("tcp_socket::read",0,"ibuf overflow",::aura::log::level_warning);

            }
         }

      }
      else if(n < 0)
      {
         log("tcp_socket::read",(int32_t)n,"abnormal value from rcv",::aura::log::level_error);
      }

      return n;

   }

   void tcp_socket::OnRead()
   {

      char *buf = (char *) m_memRead.get_data();


      int_ptr n = 0;

      try
      {

         n = read(buf,TCP_BUFSIZE_READ);

      }
      catch(...)
      {

         return;

      }

      on_read(buf,n);

   }


   void tcp_socket::on_read(const void * buf,int_ptr n)
   {

      // unbuffered
      if(n > 0)
      {

         stream_socket::on_read(buf,n);

      }

      if(m_b_input_buffer_disabled)
      {

         return;

      }


      // further processing: socks4
      if(Socks4())
      {

         bool need_more = false;

         while(GetInputLength() && !need_more && !CloseAndDelete())
         {

            need_more = OnSocks4Read();

         }

      }

   }


   void tcp_socket::OnWriteComplete()
   {
   }


   void tcp_socket::OnWrite()
   {
      if(Connecting())
      {
         int32_t err = SoError();

         // don't reset connecting flag on error here, we want the OnConnectFailed timeout later on
         if(!err) // ok
         {
            set(!IsDisableRead(),false);
            SetConnecting(false);
            SetCallOnConnect();
            return;
         }
         log("tcp: connect failed",err,StrError(err),::aura::log::level_fatal);
         set(false,false); // no more monitoring because connection failed

         // failed
         if(Socks4())
         {
            // %! leave 'Connecting' flag set?
            OnSocks4ConnectFailed();
            return;
         }
         if(GetConnectionRetry() == -1 ||
               (GetConnectionRetry() && GetConnectionRetries() < GetConnectionRetry()))
         {
            // even though the connection failed at once, only retry after
            // the connection timeout.
            // should we even try to connect again, when CheckConnect returns
            // false it's because of a connection error - not a timeout...
            return;
         }
         SetConnecting(false);
         SetCloseAndDelete(true);
         /// \todo state reason why connect failed
         OnConnectFailed();
         return;
      }
      // try send next block in buffer
      // if full block is sent, repeat
      // if all blocks are sent, reset m_wfds

      bool repeat = false;
      size_t sz = m_transfer_limit ? GetOutputLength() : 0;
      do
      {
         POSITION pos = m_obuf.get_head_position();
         if(pos == NULL)
            return;
         OUTPUT * p = &m_obuf.get_at(pos);
         repeat = false;
         int32_t n = (int32_t)try_write(p -> Buf(),p -> Len());
         if(n > 0)
         {
            size_t left = p -> remove(n);
            m_output_length -= n;
            if(!left)
            {
               delete p;
               m_obuf.remove_at(pos);
               if(!m_obuf.get_size())
               {
                  m_obuf_top = NULL;
                  OnWriteComplete();
               }
               else
               {
                  repeat = true;
               }
            }
         }
      } while(repeat);

      if(m_transfer_limit && sz > m_transfer_limit && GetOutputLength() < m_transfer_limit)
      {
         OnTransferLimit();
      }

      // check output buffer set, set/reset m_wfds accordingly
      {
         bool br;
         bool bw;
         bool bx;
         Handler().get(GetSocket(),br,bw,bx);
         if(m_obuf.get_size())
            set(br,true);
         else
            set(br,false);
      }
   }


   memory_size_t tcp_socket::try_write(const void * buf,memory_size_t len)
   {

      memory_size_t n = 0;

#ifdef HAVE_OPENSSL

      if(IsSSL())
      {

         n = SSL_write(m_ssl,buf,(int32_t)len);

         if(n == -1)
         {
            int32_t errnr = SSL_get_error(m_ssl,(int32_t)n);
            if(errnr != SSL_ERROR_WANT_READ && errnr != SSL_ERROR_WANT_WRITE)
            {
               if(errnr == SSL_ERROR_SYSCALL)
               {
                  int iError = errno;

                  const char * pszError = strerror(iError);
                  TRACE(pszError);
               }
               OnDisconnect();
               SetCloseAndDelete(true);
               SetFlushBeforeClose(false);
               SetLost();
               const char *errbuf = ERR_error_string(errnr,NULL);
               log("OnWrite/SSL_write",errnr,errbuf,::aura::log::level_fatal);
               throw io_exception(get_app(), errbuf);
            }
            return 0;
         }
         else if(!n)
         {
            OnDisconnect();
            SetCloseAndDelete(true);
            SetFlushBeforeClose(false);
            SetLost();
            int32_t errnr = SSL_get_error(m_ssl,(int32_t)n);
            const char *errbuf = ERR_error_string(errnr,NULL);
            TRACE("SSL_write() returns 0: %d : %s\n",errnr,errbuf);
            throw io_exception(get_app(), errbuf);
         }
      }
      else
#endif // HAVE_OPENSSL
      {
         retry:
#if defined(APPLEOS)
         int iSocket = GetSocket();
         n = send(iSocket,buf,len,SO_NOSIGPIPE);
#elif defined(SOLARIS)
         n = send(GetSocket(),(const char *)buf,(int)len,0);
#else
         n = send(GetSocket(),(const char *)buf,(int)len,MSG_NOSIGNAL);
#endif
         if(n == -1)
         {
            int iError = Errno;
            // normal error codes:
            // WSAEWOULDBLOCK
            //       EAGAIN or EWOULDBLOCK
#ifdef _WIN32
            if(iError != WSAEWOULDBLOCK) // 10035L
#else
            if(iError != EWOULDBLOCK)
#endif
            {
               log("send",Errno,StrError(Errno),::aura::log::level_fatal);
               OnDisconnect();
               SetCloseAndDelete(true);
               SetFlushBeforeClose(false);
               SetLost();
               throw io_exception(get_app(), StrError(Errno));
            }
            else
            {
               fd_set w;
               fd_set e;
               FD_ZERO(&e);
               FD_ZERO(&w);
               FD_SET(GetSocket(), &e);
               FD_SET(GetSocket(), &w);
               struct timeval tv;
               tv.tv_sec = 1;
               tv.tv_usec = 0;
               ::select(GetSocket() + 1, NULL, &w, &e, &tv);
               goto retry;
            }

            return 0;
         }
      }
      if(n > 0)
      {
         m_bytes_sent += n;
         if(GetTrafficMonitor())
         {
            GetTrafficMonitor() -> write(buf,n);
         }
      }
      return (int32_t)n;
   }


   void tcp_socket::buffer(const void * pdata,memory_size_t len)
   {

      const char * buf = (const char *)pdata;

      memory_size_t ptr = 0;

      m_output_length += len;

      while(ptr < len)
      {
         // buf/len => pbuf/sz
         int32_t space = 0;
         if(m_obuf_top && (space = m_obuf_top -> Space()) > 0)
         {
            const char *pbuf = buf + ptr;
            int32_t sz = (int32_t)(len - ptr);
            if(space >= sz)
            {
               m_obuf_top -> add(pbuf,sz);
               ptr += sz;
            }
            else
            {
               m_obuf_top -> add(pbuf,space);
               ptr += space;
            }
         }
         else
         {

            m_obuf.add_tail(OUTPUT());

         }

      }

   }


   /*
      void tcp_socket::write(const string &str)
      {
         write(str,  (int32_t) str.get_length());
      }
   */

   void tcp_socket::write(const void * pdata,memory_size_t len)
   {

      const byte * buf = (const byte *)pdata;

      if(!Ready() && !Connecting())
      {
         log("write",-1,"Attempt to write to a non-ready socket"); // warning
         if(GetSocket() == INVALID_SOCKET)
            log("write",0," * GetSocket() == INVALID_SOCKET",::aura::log::level_info);
         if(Connecting())
            log("write",0," * Connecting()",::aura::log::level_info);
         if(CloseAndDelete())
            log("write",0," * CloseAndDelete()",::aura::log::level_info);
         return;
      }
      if(!IsConnected())
      {
         log("write",-1,"Attempt to write to a non-connected socket, will be sent on connect"); // warning
         buffer(buf,len);
         return;
      }
      if(m_obuf_top)
      {
         buffer(buf,len);
         return;
      }
      else
      {
         int32_t n = (int32_t)try_write(buf,len);
         if(n >= 0 && n < (int32_t)len)
         {
            buffer(buf + n,len - n);
         }
      }
      // if ( data in buffer || !IsConnected )
      // {
      //   add to buffer
      // }
      // else
      // try_send
      // if any data is unsent, buffer it and set m_wfds

      // check output buffer set, set/reset m_wfds accordingly
      {
         bool br;
         bool bw;
         bool bx;
         Handler().get(GetSocket(),br,bw,bx);
         if(m_obuf.get_size())
            set(br,true);
         else
            set(br,false);
      }
   }


   void tcp_socket::OnLine(const string &)
   {
   }



   void tcp_socket::OnSocks4Connect()
   {
      char request[1000];
      memset(request,0,sizeof(request));
      request[0] = 4; // socks v4
      request[1] = 1; // command code: CONNECT
      {
         ::net::address ad = GetClientRemoteAddress();
         if(ad.is_valid())
         {
            struct sockaddr *p0 = (struct sockaddr *)ad.sa();
            struct sockaddr_in *p = (struct sockaddr_in *)p0;
            if(p -> sin_family == AF_INET)
            {
               memcpy(request + 2,&p -> sin_port,2); // nwbo is ok here
               memcpy(request + 4,&p -> sin_addr,sizeof(struct in_addr));
            }
            else
            {
               /// \todo warn
            }
         }
         else
         {
            /// \todo warn
         }
      }
      strcpy(request + 8,GetSocks4Userid());
      size_t length = GetSocks4Userid().get_length() + 8 + 1;
      write(request,length);
      m_socks4_state = 0;
   }


   void tcp_socket::OnSocks4ConnectFailed()
   {
      Handler().log(this,"OnSocks4ConnectFailed",0,"connection to socks4 server failed, trying direct connection",::aura::log::level_warning);
      if(!Handler().Socks4TryDirect())
      {
         SetConnecting(false);
         SetCloseAndDelete();
         OnConnectFailed(); // just in case
      }
      else
      {
         SetRetryClientConnect();
      }
   }


   bool tcp_socket::OnSocks4Read()
   {
      switch(m_socks4_state)
      {
      case 0:
         ibuf.read(&m_socks4_vn,1);
         m_socks4_state = 1;
         break;
      case 1:
         ibuf.read(&m_socks4_cd,1);
         m_socks4_state = 2;
         break;
      case 2:
         if(GetInputLength() > 1)
         {
            ibuf.read((char *)&m_socks4_dstport,2);
            m_socks4_state = 3;
         }
         else
         {
            return true;
         }
         break;
      case 3:
         if(GetInputLength() > 3)
         {
            ibuf.read((char *)&m_socks4_dstip,4);
            SetSocks4(false);

            switch(m_socks4_cd)
            {
            case 90:
               OnConnect();
               log("OnSocks4Read",0,"Connection established",::aura::log::level_info);
               break;
            case 91:
            case 92:
            case 93:
               Handler().log(this,"OnSocks4Read",m_socks4_cd,"socks4 server reports connect failed",::aura::log::level_fatal);
               SetConnecting(false);
               SetCloseAndDelete();
               OnConnectFailed();
               break;
            default:
               Handler().log(this,"OnSocks4Read",m_socks4_cd,"socks4 server unrecognized response",::aura::log::level_fatal);
               SetCloseAndDelete();
               break;
            }
         }
         else
         {
            return true;
         }
         break;
      }
      return false;
   }




   void tcp_socket::OnSSLConnect()
   {

      SetNonblocking(true);

      synch_lock slMap(&Session.sockets().m_clientcontextmap.m_mutex);

      {
         if(m_ssl_ctx)
         {
            TRACE("SSL Context already initialized - closing socket\n");
            SetCloseAndDelete(true);
            return;
         }
         InitSSLClient();
      }

      synch_lock sl(m_pmutexSslCtx);

      slMap.unlock();

      if(m_ssl_ctx)
      {
         /* Connect the SSL socket */
         m_ssl = SSL_new(m_ssl_ctx);
         if(!m_ssl)
         {
            TRACE(" m_ssl is NULL\n");
            SetCloseAndDelete(true);
            return;
         }
         
         if (m_strTlsHostName.has_char())
         {

            SSL_set_tlsext_host_name(m_ssl, (char *) (const char *) m_strTlsHostName);

         }

         SSL_set_mode(m_ssl,SSL_MODE_AUTO_RETRY);
         m_sbio = BIO_new_socket((int32_t)GetSocket(),BIO_NOCLOSE);
         if(!m_sbio)
         {
            TRACE(" m_sbio is NULL\n");
            SetCloseAndDelete(true);
            return;
         }
         SSL_set_bio(m_ssl,m_sbio,m_sbio);
         if(!SSLNegotiate())
         {
            SetSSLNegotiate();
         }
      }
      else
      {
         SetCloseAndDelete();
      }
   }


   void tcp_socket::OnSSLAccept()
   {

      SetNonblocking(true);

      synch_lock slMap(&Session.sockets().m_servercontextmap.m_mutex);

      {
         if(m_ssl_ctx)
         {
            TRACE("SSL Context already initialized - closing socket\n");
            SetCloseAndDelete(true);
            return;
         }
         InitSSLServer();
         SetSSLServer();
      }


      synch_lock sl(m_pmutexSslCtx);

      slMap.unlock();


      if(m_ssl_ctx)
      {
         m_ssl = SSL_new(m_ssl_ctx);
         if(!m_ssl)
         {
            TRACE(" m_ssl is NULL\n");
            SetCloseAndDelete(true);
            return;
         }
         SSL_set_mode(m_ssl,SSL_MODE_AUTO_RETRY);
         m_sbio = BIO_new_socket((int32_t)GetSocket(),BIO_NOCLOSE);
         if(!m_sbio)
         {
            TRACE(" m_sbio is NULL\n");
            SetCloseAndDelete(true);
            return;
         }
         SSL_set_bio(m_ssl,m_sbio,m_sbio);
         //      if (!SSLNegotiate())
         {
            SetSSLNegotiate();
         }
      }
   }


   bool tcp_socket::SSLNegotiate()
   {
      if(!IsSSLServer()) // client
      {
         if(!m_bClientSessionSet && m_spsslclientcontext->m_psession != NULL)
         {
            SSL_set_session(m_ssl,m_spsslclientcontext->m_psession);
            m_bClientSessionSet = true;
         }
         TRACE("SSL_connect!!");
         int32_t r = SSL_connect(m_ssl);
         if(r > 0)
         {
            SetSSLNegotiate(false);


            long x509_err = cert_common_name_check(m_strHost);
            if(x509_err != X509_V_OK
                  && x509_err != X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN
                  && x509_err != X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY)
            {
               log("SSLNegotiate/cert_common_name_check",0,"cert_common_name_check failed (error="+::str::from(x509_err)+",url=" + m_strUrl + ")",::aura::log::level_info);
               SetSSLNegotiate(false);
               SetCloseAndDelete();
               OnSSLConnectFailed();
               return false;
            }

            /// \todo: resurrect certificate check... client
            //         CheckCertificateChain( "");//ServerHOST);
            SetNonblocking(false);
            //
            {
               SetConnected();
               if(GetOutputLength())
               {
                  OnWrite();
               }
            }
            if(IsReconnect())
               OnReconnect();
            else
            {
               OnConnect();
            }
            log("SSLNegotiate/SSL_connect",0,"Connection established",::aura::log::level_info);
            if(m_spsslclientcontext->m_psession == NULL)
            {
               m_spsslclientcontext->m_psession = SSL_get1_session(m_ssl);
            }
            return true;
         }
         else if(!r)
         {
            
            long error = ERR_get_error();
            const char* error_str = ERR_error_string(error, NULL);
            printf("could not SSL_connect: %s\n", error_str);
            
            int iError = errno;
            int iErrorSsl = SSL_get_error(m_ssl,r);

            if(m_spsslclientcontext.is_set() &&
               iErrorSsl == SSL_ERROR_ZERO_RETURN
               && (m_spsslclientcontext->m_pmethod == TLSv1_client_method()
                  || m_spsslclientcontext->m_pmethod == SSLv3_client_method()))
            {
               TRACE("ssl_error_zero_return");
            }

            else
            {
               if (m_spsslclientcontext->m_psession != NULL)
               {

                  if (m_spsslclientcontext->m_iRetry == 0)
                  {
                     m_spsslclientcontext->m_iRetry = 1;
                     SSL_clear(m_ssl);
                     SSL_SESSION_free(m_spsslclientcontext->m_psession);
                     m_spsslclientcontext->m_psession = NULL;
                     goto skip;
                  }
                  else
                  {
                     m_spsslclientcontext->m_iRetry = 0;
                  }
               }
               log("SSLNegotiate/SSL_connect", 0, "Connection failed", ::aura::log::level_info);
               SetSSLNegotiate(false);
               SetCloseAndDelete();
               OnSSLConnectFailed();

            }
skip:
            ;
         }
         else
         {
            r = SSL_get_error(m_ssl,r);
            if(r == SSL_ERROR_WANT_READ || r == SSL_ERROR_WANT_WRITE)
            {
            }
            else if(r == SSL_ERROR_WANT_CONNECT || r == SSL_ERROR_WANT_ACCEPT)
            {
            }
            else
            {
               log("SSLNegotiate/SSL_connect",-1,"Connection failed",::aura::log::level_info);
               TRACE("SSL_connect() failed - closing socket, return code: %d\n",r);
               SetSSLNegotiate(false);
               SetCloseAndDelete(true);
               OnSSLConnectFailed();
            }
         }
      }
      else // server
      {
         int32_t r = SSL_accept(m_ssl);
         if(r > 0)
         {
            SetSSLNegotiate(false);
            /// \todo: resurrect certificate check... server
            //         CheckCertificateChain( "");//ClientHOST);
            SetNonblocking(false);
            //
            {
               SetConnected();
               if(GetOutputLength())
               {
                  OnWrite();
               }
            }
            OnAccept();
            log("SSLNegotiate/SSL_accept",0,"Connection established",::aura::log::level_info);
            return true;
         }
         else if(!r)
         {
            log("SSLNegotiate/SSL_accept",0,"Connection failed",::aura::log::level_info);
            SetSSLNegotiate(false);
            SetCloseAndDelete();
            OnSSLAcceptFailed();
         }
         else
         {
            r = SSL_get_error(m_ssl,r);
            if(r == SSL_ERROR_WANT_READ || r == SSL_ERROR_WANT_WRITE)
            {
            }
            else if(r == SSL_ERROR_WANT_CONNECT || r == SSL_ERROR_WANT_ACCEPT)
            {
            }
            else
            {
               log("SSLNegotiate/SSL_accept",-1,"Connection failed",::aura::log::level_info);
               TRACE("SSL_accept() failed - closing socket, return code: %d\n",r);
               SetSSLNegotiate(false);
               SetCloseAndDelete(true);
               OnSSLAcceptFailed();
            }
         }
      }
      return false;
   }


   void tcp_socket::InitSSLClient()
   {
      InitializeContext(m_strInitSSLClientContext,SSLv23_method());
   }


   void tcp_socket::InitSSLServer()
   {
      log("InitSSLServer",0,"You MUST implement your own InitSSLServer method",::aura::log::level_fatal);
      SetCloseAndDelete();
   }


   void tcp_socket::InitializeContext(const string & context,const SSL_METHOD * pmethod)
   {
      /* create our context*/
      if(m_spsslclientcontext.is_null())
      {
//         string_map < sp(ssl_client_context) > & clientcontextmap = Session.sockets().m_clientcontextmap;
//         if(clientcontextmap.PLookup(context) == NULL)
//         {
            m_spsslclientcontext = canew(ssl_client_context(get_app(),pmethod));
//            if(context.has_char())
//            {
//               clientcontextmap[context] = m_spsslclientcontext;
//            }
//         }
//         else
//         {
//            m_spsslclientcontext = clientcontextmap.PLookup(context)->m_element2;
//         }
      }
      if(m_spsslclientcontext.is_set())
      {
         m_ssl_ctx = m_spsslclientcontext->m_pcontext;
         m_pmutexSslCtx = &m_spsslclientcontext->m_mutex;
      }
   }


   void tcp_socket::InitializeContext(const string & context,const string & keyfile,const string & password,const SSL_METHOD *meth_in)
   {

      m_ssl_ctx = Session.sockets().m_servercontextmap.InitializeContext(&m_pmutexSslCtx,context,keyfile,password,meth_in);

   }


   void tcp_socket::InitializeContext(const string & context,const string & certfile,const string & keyfile,const string & password,const SSL_METHOD *meth_in)
   {

      m_ssl_ctx = Session.sockets().m_servercontextmap.InitializeContext(&m_pmutexSslCtx,context,certfile,keyfile,password,meth_in);
   }


   int32_t tcp_socket_SSL_password_cb(char *buf,int32_t num,int32_t rwflag,void *userdata)
   {
      UNREFERENCED_PARAMETER(rwflag);
      socket *p0 = static_cast<socket *>(userdata);
      tcp_socket *p = dynamic_cast<tcp_socket *>(p0);
      string pw = p ? p -> GetPassword() : "";
      if(num < pw.get_length() + 1)
      {
         return 0;
      }
      strcpy(buf,pw);
      return (int32_t)pw.get_length();
   }


   void tcp_socket::close()
   {
      
      if (GetSocket() == INVALID_SOCKET) // this could happen
      {

         log("socket::close", 0, "file descriptor invalid", ::aura::log::level_warning);

         return;

      }

      int32_t n;

      SetNonblocking(true);

      if (!Lost() && IsConnected() && !(GetShutdown() & SHUT_WR))
      {

         if (shutdown(GetSocket(), SHUT_WR) == -1)
         {

            // failed...
            log("shutdown", Errno, StrError(Errno), ::aura::log::level_error);

         }

      }

      char tmp[1000];

      if (!Lost() && (n = (int32_t) ::recv(GetSocket(), tmp, 1000, 0)) >= 0)
      {

         if (n)
         {

            log("read() after shutdown", n, "bytes read", ::aura::log::level_warning);

         }

      }

#ifdef HAVE_OPENSSL
      if(m_ssl)
      {
         //#ifdef LINUX
         //  signal(SIGPIPE, &::sockets::ssl_sigpipe_handle);
         //#endif
         /*       struct sigaction m_saPipe;
                struct sigaction m_saPipeOld;
                memset(&m_saPipe, 0, sizeof(m_saPipe));
                sigemptyset(&m_saPipe.sa_mask);
                sigaddset(&m_saPipe.sa_mask, SIGSEGV);
                m_saPipe.sa_flags = SA_NODEFER | SA_SIGINFO;
                m_saPipe.sa_sigaction = &::exception::translator::filter_sigpipe;
                sigaction(SIGPIPE, &m_saPipe, &m_saPipeOld);

                //pthread_t thread;
                  sigset_t set;
                  int s;

                  /* Block SIGQUIT and SIGUSR1; other threads created by main()
                     will inherit a copy of the signal mask. */
#ifdef LINUX
         sigset_t set;
         sigemptyset(&set);
         sigaddset(&set,SIGPIPE);
         pthread_sigmask(SIG_BLOCK,&set,NULL);
#endif
         if(SSL_get_shutdown(m_ssl) == 0)
            SSL_shutdown(m_ssl);
      }

      if(m_ssl)
      {
         SSL_free(m_ssl);
         m_ssl = NULL;
      }

#endif

      socket::close();

   }



#ifdef HAVE_OPENSSL
   SSL_CTX *tcp_socket::GetSslContext()
   {
      if(!m_ssl_ctx)
         log("GetSslContext",0,"SSL Context is NULL; check InitSSLServer/InitSSLClient",::aura::log::level_warning);
      return m_ssl_ctx;
   }

   SSL *tcp_socket::GetSsl()
   {
      if(!m_ssl)
         log("GetSsl",0,"SSL is NULL; check InitSSLServer/InitSSLClient",::aura::log::level_warning);
      return m_ssl;
   }
#endif


   void tcp_socket::SetReconnect(bool bReconnect)
   {
      m_bReconnect = bReconnect;
   }

   void tcp_socket::OnRawData(char * buf_in,size_t len)
   {
      socket::OnRawData(buf_in,len);
   }


   size_t tcp_socket::GetInputLength()
   {
      return (size_t)ibuf.get_length();
   }


   size_t tcp_socket::GetOutputLength()
   {
      return m_output_length;
   }


   uint64_t tcp_socket::GetBytesReceived(bool clear)
   {
      uint64_t z = m_bytes_received;
      if(clear)
         m_bytes_received = 0;
      return z;
   }


   uint64_t tcp_socket::GetBytesSent(bool clear)
   {
      uint64_t z = m_bytes_sent;
      if(clear)
         m_bytes_sent = 0;
      return z;
   }


   bool tcp_socket::Reconnect()
   {
      return m_bReconnect;
   }


   void tcp_socket::SetIsReconnect(bool bTryingReconnect)
   {

      m_bTryingReconnect = bTryingReconnect;

   }


   bool tcp_socket::IsReconnect()
   {

      return m_bTryingReconnect;

   }


   const string & tcp_socket::GetPassword()
   {

      return m_password;

   }


   void tcp_socket::DisableInputBuffer(bool x)
   {

      m_b_input_buffer_disabled = x;

   }


   void tcp_socket::OnOptions(int32_t family,int32_t type,int32_t protocol,SOCKET s)
   {

      UNREFERENCED_PARAMETER(family);
      UNREFERENCED_PARAMETER(type);
      UNREFERENCED_PARAMETER(protocol);
      UNREFERENCED_PARAMETER(s);
      //TRACE("socket::OnOptions()\n");
#ifdef SO_NOSIGPIPE
      SetSoNosigpipe(true);
#endif
      SetSoReuseaddr(true);
      SetSoKeepalive(true);
   }


   void tcp_socket::SetLineProtocol(bool x)
   {
      stream_socket::SetLineProtocol(x);
      DisableInputBuffer(x);
   }


   bool tcp_socket::SetTcpNodelay(bool x)
   {
#ifdef TCP_NODELAY
      int32_t optval = x ? 1 : 0;
      if(setsockopt(GetSocket(),IPPROTO_TCP,TCP_NODELAY,(char *)&optval,sizeof(optval)) == -1)
      {
         log("setsockopt(IPPROTO_TCP, TCP_NODELAY)",Errno,StrError(Errno),::aura::log::level_fatal);
         return false;
      }
      return true;
#else
      log("socket option not available",0,"TCP_NODELAY",::aura::log::level_info);
      return false;
#endif
   }



   void tcp_socket::OnConnectTimeout()
   {
      log("connect",-1,"connect timeout",::aura::log::level_fatal);
      m_estatus = status_connection_timed_out;
      if(Socks4())
      {
         OnSocks4ConnectFailed();
         // retry direct connection
      }
      else if(GetConnectionRetry() == -1 ||
              (GetConnectionRetry() && GetConnectionRetries() < GetConnectionRetry()))
      {
         IncreaseConnectionRetries();
         // ask socket via OnConnectRetry callback if we should continue trying
         if(OnConnectRetry())
         {
            SetRetryClientConnect();
         }
         else
         {
            SetCloseAndDelete(true);
            /// \todo state reason why connect failed
            OnConnectFailed();
         }
      }
      else
      {
         SetCloseAndDelete(true);
         /// \todo state reason why connect failed
         OnConnectFailed();
      }
      //
      SetConnecting(false);
   }


#ifdef _WIN32
   void tcp_socket::OnException()
   {
      if(Connecting())
      {
         int32_t iError = this->Handler().m_iSelectErrno;
         if(iError == ETIMEDOUT)
         {
            m_estatus = status_connection_timed_out;
         }
         else
         {
            //m_estatus = status_failed;
         }
         if(Socks4())
            OnSocks4ConnectFailed();
         else if(GetConnectionRetry() == -1 ||
                 (GetConnectionRetry() &&
                  GetConnectionRetries() < GetConnectionRetry()))
         {
            // even though the connection failed at once, only retry after
            // the connection timeout
            // should we even try to connect again, when CheckConnect returns
            // false it's because of a connection error - not a timeout...
         }
         else
         {
            SetConnecting(false); // tnx snibbe
            SetCloseAndDelete();
            OnConnectFailed();
         }
         return;
      }
      // %! exception doesn't always mean something bad happened, this code should be reworked
      // errno valid here?
      int32_t err = SoError();
      log("exception on select",err,StrError(err),::aura::log::level_fatal);
      SetCloseAndDelete();
   }
#endif // _WIN32


   int32_t tcp_socket::Protocol()
   {
      return IPPROTO_TCP;
   }


   void tcp_socket::SetTransferLimit(size_t sz)
   {
      m_transfer_limit = sz;
   }


   void tcp_socket::OnTransferLimit()
   {
   }

   string tcp_socket::get_url()
   {
      return m_strUrl;
   }


   string tcp_socket::get_short_description()
   {
      return get_url();
   }

   long tcp_socket::cert_common_name_check(const char * common_name)
   {

      if(!m_bCertCommonNameCheckEnabled)
      {
         return X509_V_OK;
      }

      ::X509 *cert = NULL;
      ::X509_name_st *subject = NULL;

      cert = SSL_get_peer_certificate(m_ssl);
      bool ok = false;
      if(cert != NULL && strlen(common_name) > 0)
      {
         char data[256];
         if((subject = X509_get_subject_name(cert)) != NULL && X509_NAME_get_text_by_NID(subject,NID_commonName,data,256) > 0)
         {
            data[255] = 0;
            if(strnicmp_dup(data,common_name,255) == 0)
            {
               ok = true;
            }
            else
            {
               string str = data;
               if(::str::begins_eat(str,"*."))
               {
                  string strCommon = common_name;
                  if(strCommon == str)
                  {
                     ok = true;
                  }
                  else
                  {

                     int iFind = strCommon.find('.');

                     if(iFind >= 0)
                     {
                        if(str == strCommon.Mid(iFind + 1))
                        {
                           ok = true;
                        }
                     }

                  }


               }
            }
         }
      }

      if(cert)
      {
         X509_free(cert);
      }

      if(ok)
      {
         return SSL_get_verify_result(m_ssl);
      }

      return X509_V_ERR_APPLICATION_VERIFICATION;

   }


   void tcp_socket::enable_cert_common_name_check(bool bEnable)
   {

      m_bCertCommonNameCheckEnabled = bEnable;

   }


   string tcp_socket::get_connect_host()
   {

      return m_strConnectHost;

   }


   port_t tcp_socket::get_connect_port()
   {

      return m_iConnectPort;

   }






} // namespace sockets





