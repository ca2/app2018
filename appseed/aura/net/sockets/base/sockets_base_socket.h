#pragma once




#include "aura/net/sockets/bsd/basic/sockets_ssl_client_context.h"


#if defined(LINUX)
//struct ucred {
//  pid_t pid;    /* process ID of the sending process */
//uid_t uid;    /* user ID of the sending process */
//gid_t gid;    /* group ID of the sending process */
//};
#endif // defined

//#include "aura/net/sockets/bsd/basic/sockets_socket_handler.h"

namespace sockets
{


   class ssl_context;

   class base_socket_handler;


   /** \defgroup basic Basic sockets */
   /** base_socket base class.
   \ingroup basic */
   class CLASS_DECL_AURA base_socket :
      virtual public ::file::file
   {
   public:


      enum e_status
      {

         status_ok,
         status_connection_timed_out,
         status_timed_out,
         status_cancelled,

      };


      class CLASS_DECL_AURA callback
      {
      public:
         virtual void OnRawData(base_socket * psocket, void * pdata, size_t len) = 0;
      };

      friend class base_socket_handler;


      /** Detached base_socket run thread.
      \ingroup internal */
      class CLASS_DECL_AURA socket_thread:
         virtual public ::thread
      {
      public:


         sp(socket_handler)   m_sphandler;

         sp(base_socket)      m_spsocket;


         socket_thread(base_socket * psocket);

         virtual ~socket_thread();

         virtual bool init_thread() override;

         sp(base_socket) get_socket() const { return m_spsocket; }

         virtual void run() override;

      };


      /** Data pass class from source to destination. */
      class trigger_data
      {
      public:


         base_socket *m_src;


         trigger_data() : m_src(NULL) {}
         virtual ~trigger_data() {}

         base_socket *GetSource() const { return m_src; }
         void SetSource(base_socket *x) { m_src = x; }

      };

      // former TCP_SOCKET ::sockets::tcp_socket
      sp(ssl_client_context)     m_spsslclientcontext;

      ssl_context *              m_psslcontext;
      string m_password; ///< ssl password


      base_socket_handler &   m_handler; ///< Reference of base_socket_handler in control of this socket
      SOCKET                  m_socket; ///< File descriptor

      static mutex *           s_pmutex;

      ::net::address          m_addressRemote; ///< Remote end ::net::address
      ::net::address          m_addressRemoteClient; ///< Address of last connect()
      ::file::file_sp       m_spfileTrafficMonitor;

      bool                 m_b_chunked;


      ::memory_file *         m_pmemfileInput;
      bool                    m_bEnd; // should finish by not sending no more writes
      string                  m_strCat;
      string                  m_strCipherList;
      callback *              m_pcallback;
      time_t                  m_timeCreate; ///< time in seconds when this base_socket was created
      bool                    m_bDisableRead; ///< Disable checking for read events
      bool                    m_bConnected; ///< base_socket is connected (tcp/udp)
      bool                    m_bLost; ///< connection lost
      bool                    m_bErasedByHandler; ///< Set by handler before delete
      time_t                  m_timeClose; ///< time in seconds when ordered to close
      int                     m_iBindPort;
      bool                    m_bDelete; ///< Delete by handler flag
      bool                    m_bClose; ///< close and delete flag
      base_socket *           m_psocketParent; ///< Pointer to listen_socket class, valid for incoming sockets
      time_t                  m_timeTimeoutStart; ///< Set by SetTimeout
      time_t                  m_timeTimeoutLimit; ///< Defined by SetTimeout
      bool                    m_bNonBlocking;
      //    unsigned long           m_flags; ///< boolean flags, replacing old 'bool' members

#if !defined(BSD_STYLE_SOCKETS)
      manual_reset_event      m_event;
#endif

      bool                    m_bEnableSsl; ///< Enable SSL for this tcp_socket
      bool                    m_bSsl; ///< ssl negotiation mode (tcp_socket)
      bool                    m_bSslServer; ///< True if this is an incoming ssl tcp_socket connection

      bool                    m_bIpv6; ///< This is an ipv6 base_socket if this one is true

      int                     m_iSocketType; ///< Type of base_socket, from base_socket() call
      string                  m_strSocketProtocol; ///< Protocol, from base_socket() call
      bool                    m_bClient; ///< only client connections are pooled
      bool                    m_bRetain; ///< keep connection on close
      bool                    m_bEnablePool; ///< true if this socket may enter in a pool

      bool                    m_bSocks4; ///< socks4 negotiation mode (tcp_socket)
      in_addr                 m_socks4_host; ///< socks4 server ::net::address
      port_t                  m_socks4_port; ///< socks4 server port number
      string                  m_socks4_userid; ///< socks4 server usedid

      bool                    m_detach; ///< base_socket ordered to detach flag
      bool                    m_detached; ///< base_socket has been detached
      sp(socket_thread)       m_pThread; ///< detach base_socket thread class pointer
      base_socket_handler *   m_slave_handler; ///< Actual sockethandler while detached


      // LineProtocol
      bool                    m_bLineProtocol; ///< Line protocol mode flag
      bool                    m_skip_c; ///< Skip m_element2 char of CRLF or LFCR sequence in OnRead
      char                    m_c; ///< First char in CRLF or LFCR sequence
      string                  m_line; ///< Current line in line protocol mode

      e_status                m_estatus;
      DWORD                   m_dwStart;

#if !defined(BSD_STYLE_SOCKETS)
      bool                    m_bExpectResponse;
      bool                    m_bExpectRequest;
      bool                    m_bReading;
      bool                    m_bWriting;
#endif

      /** base_socket mode flags. */
      /*
      enum {
      // base_socket
      SOCK_DEL =          0x01, ///< Delete by handler flag
      SOCK_CLOSE =          0x02, ///< close and delete flag
      SOCK_DISABLE_READ =       0x04, ///< Disable checking for read events
      SOCK_CONNECTED =       0x08, ///< base_socket is connected (tcp/udp)

      SOCK_ERASED_BY_HANDLER =    0x10, ///< Set by handler before delete
      // HAVE_OPENSSL
      SOCK_ENABLE_SSL =       0x20, ///< Enable SSL for this tcp_socket
      SOCK_SSL =          0x40, ///< ssl negotiation mode (tcp_socket)
      SOCK_SSL_SERVER =       0x80, ///< True if this is an incoming ssl tcp_socket connection

      // ENABLE_IPV6
      SOCK_IPV6 =          0x0100, ///< This is an ipv6 base_socket if this one is true
      SOCK_CLIENT =          0x0200, ///< only client connections are pooled
      SOCK_RETAIN =          0x0400, ///< keep connection on close
      SOCK_LOST =          0x0800, ///< connection lost

      // ENABLE_SOCKS4
      SOCK_SOCKS4 =          0x1000, ///< socks4 negotiation mode (tcp_socket)
      SOCK_DETACH =          0x2000, ///< base_socket ordered to detach flag
      SOCK_DETACHED =       0x4000, ///< base_socket has been detached
      // stream_socket
      STREAMSOCK_CONNECTING =      0x8000, ///< Flag indicating connection in progress

      STREAMSOCK_FLUSH_BEFORE_CLOSE = 0x010000L, ///< Send all data before closing (default true)
      STREAMSOCK_CALL_ON_CONNECT =   0x020000L, ///< OnConnect will be called next base_socket_handler cycle if true
      STREAMSOCK_RETRY_CONNECT =   0x040000L, ///< Try another connection attempt next base_socket_handler cycle
      STREAMSOCK_LINE_PROTOCOL =   0x080000L, ///< Line protocol mode flag

      };
      */

      /** "Default" constructor */
      base_socket(base_socket_handler&);

      virtual ~base_socket();

      /** base_socket class instantiation method. Used when a "non-standard" constructor
      * needs to be used for the base_socket class. Note: the base_socket class still needs
      * the "default" constructor with one base_socket_handler& as input parameter.
      */
      virtual base_socket *create() { return NULL; }

      /** Returns reference to sockethandler that owns the base_socket.
      If the base_socket is detached, this is a reference to the slave sockethandler.
      */
      base_socket_handler& Handler() const;

      /** Returns reference to sockethandler that owns the base_socket.
      This one always returns the reference to the original sockethandler,
      even if the base_socket is detached.
      */
      base_socket_handler& MasterHandler() const;

      virtual void free_ssl_session();

      virtual void get_ssl_session();


      /** Called by listen_socket after accept but before base_socket is added to handler.
      * CTcpSocket uses this to create its ICrypt member var.
      * The ICrypt member var is created by a virtual method, therefore
      * it can't be called directly from the CTcpSocket constructor.
      * Also used to determine if incoming HTTP connection is normal (port 80)
      * or ssl (port 443).
      */
      virtual void Init();


      /** Assign this socket a file descriptor created
      by a call to socket() or otherwise. */
      void attach(SOCKET s);

      /** Return file descriptor assigned to this base_socket. */
      SOCKET GetSocket();

      /** close connection immediately - internal use.
      \sa SetCloseAndDelete */
      virtual void close();


      virtual int32_t close_socket(SOCKET s);


      virtual bool is_connecting();

      /** add file descriptor to sockethandler fd_set's. */
      void Set(bool bRead,bool bWrite,bool bException = true);

      /** Returns true when base_socket file descriptor is valid
      and base_socket is not about to be closed. */
      virtual bool Ready();

      virtual bool is_valid();

      /** Returns pointer to listen_socket that created this instance
      * on an incoming connection. */
      base_socket *get_parent();

      /** Used by listen_socket to set parent pointer of newly created
      * base_socket instance. */
      void set_parent(base_socket *);

      /** get listening port from listen_socket<>. */
      virtual port_t GetPort();

      /** Set base_socket non-block operation. */
      bool SetNonblocking(bool);

      /** Set base_socket non-block operation. */
      bool SetNonblocking(bool, SOCKET);

      /** Total lifetime of instance. */
      time_t Uptime();

      /** Set address/port of last connect() call. */
      void SetClientRemoteAddress(const ::net::address & address);

      /** get address/port of last connect() call. */
      ::net::address GetClientRemoteAddress();


      /** Outgoing traffic counter. */
      virtual uint64_t GetBytesSent(bool clear = false);

      /** Incoming traffic counter. */
      virtual uint64_t GetBytesReceived(bool clear = false);

      // LIST_TIMEOUT

      /** Enable timeout control. 0=disable timeout check. */
      void SetTimeout(time_t secs);

      /** Check timeout. \return true if time limit reached */
      bool Timeout(time_t tnow);

      /** Used by listen_socket. ipv4 and ipv6 */
      void SetRemoteHostname(const ::net::address & address);

      /** \name Event callbacks */
      //@{

      /** Called when there is something to be read from the file descriptor. */
      virtual void OnRead();
      /** Called when there is room for another write on the file descriptor. */
      virtual void OnWrite();
      /** Called on base_socket exception. */
      virtual void OnException();
      /** Called before a base_socket class is deleted by the base_socket_handler. */
      virtual void OnDelete();
      /** Called when a connection has completed. */
      virtual void OnConnect();
      /** Called when an incoming connection has been completed. */
      virtual void OnAccept();
      //void OnAccept(::Windows::Foundation::IAsyncAction ^ action, ::Windows::Foundation::AsyncStatus status);
      /** Called when a complete line has been read and the base_socket is in
      * line protocol mode. */
      virtual void OnLine(const string & );


      virtual bool on_select_idle();

      //virtual memory_size_t read(void * buf, memory_size_t c);

      virtual void on_read(const void * buf, int_ptr c);
      virtual void OnRawData(char * buf, size_t len);

      /** Called on connect timeout (5s). */
      virtual void OnConnectFailed();
      /** Called when a client base_socket is created, to set base_socket options.
      \param family AF_INET, AF_INET6, etc
      \param type SOCK_STREAM, SOCK_DGRAM, etc
      \param protocol Protocol number (tcp, udp, sctp, etc)
      \param s base_socket file descriptor
      */
      virtual void OnOptions(int family,int type,int protocol,SOCKET s) = 0;
      /** Connection retry callback - return false to abort connection attempts */
      virtual bool OnConnectRetry();
      /** a reconnect has been made */
      virtual void OnReconnect();
      /** tcp_socket: When a disconnect has been detected (recv/SSL_read returns 0 bytes). */
      virtual void OnDisconnect();
      /** Timeout callback. */
      virtual void OnTimeout();
      /** Connection timeout. */
      virtual void OnConnectTimeout();
      //@}

      /** \name base_socket mode flags, set/reset */
      //@{
      /** Set delete by handler true when you want the sockethandler to
      delete the base_socket instance after use. */
      void SetDeleteByHandler(bool = true);
      /** Check delete by handler flag.
      \return true if this instance should be deleted by the sockethandler */
      bool DeleteByHandler();

      // LIST_CLOSE - conditional event queue

      /** Set close and delete to terminate the connection. */
      void SetCloseAndDelete(bool = true);
      /** Check close and delete flag.
      \return true if this base_socket should be closed and the instance removed */
      bool CloseAndDelete();

      /** Return number of seconds since base_socket was ordered to close. \sa SetCloseAndDelete */
      time_t TimeSinceClose();

      /** Ignore read events for an output only base_socket. */
      void DisableRead(bool x = true);
      /** Check ignore read events flag.
      \return true if read events should be ignored */
      bool IsDisableRead();

      /** Set connected status. */
      void SetConnected(bool = true);
      /** Check connected status.
      \return true if connected */
      bool IsConnected();

      /** Connection lost - error while reading/writing from a base_socket - tcp_socket only. */
      void SetLost();
      /** Check connection lost status flag, used by tcp_socket only.
      \return true if there was an error while r/w causing the base_socket to close */
      bool Lost();

      /** Set flag indicating the base_socket is being actively deleted by the sockethandler. */
      void SetErasedByHandler(bool x = true);
      /** get value of flag indicating base_socket is deleted by sockethandler. */
      bool ErasedByHandler();

      //@}

      /** \name Information about remote connection */
      //@{
      /** Returns address/port of remote end. */
      //::net::address GetRemoteSocketAddress();
      /** Returns address/port of remote end: ipv4. */
      //string GetRemoteIP4();
      /** Returns address/port of remote end: ipv6. */
      //struct in6_addr GetRemoteIP6();
      /** Returns remote port number: ipv4 and ipv6. */
      virtual port_t GetRemotePort();
      /** Returns remote ip as string? ipv4 and ipv6. */
      virtual ::net::address GetRemoteAddress();
      /** ipv4 and ipv6(not implemented) */
      virtual ::net::address GetRemoteHostname();
      //@}

      /** Returns local port number for bound base_socket file descriptor. */
      virtual port_t GetLocalPort();
      /** Returns local ipv4 address/port for bound base_socket file descriptor. */
      //ipaddr_t GetSockIP4();
      /** Returns local ipv4 address/port as text for bound base_socket file descriptor. */
      virtual ::net::address GetLocalAddress();
      /** Returns local ipv6 address/port for bound base_socket file descriptor. */
      //struct in6_addr GetSockIP6();
      /** Returns local ipv6 address/port as text for bound base_socket file descriptor. */
      //string GetSockAddress6();










      // --------------------------------------------------------------------------
      /** @name IP options
      When an ip or base_socket option is available on all of the operating systems
      I'm testing on (linux 2.4.x, _win32, macosx, solaris9 intel) they are not
      checked with an #ifdef below.
      This might cause a compile error on other operating systems. */
      // --------------------------------------------------------------------------

      // IP options
      //@{

      bool SetIpOptions(const void *p, socklen_t len);
      bool SetIpTOS(unsigned char tos);
      unsigned char IpTOS();
      bool SetIpTTL(int ttl);
      int IpTTL();
      bool SetIpHdrincl(bool x = true);
      bool SetIpMulticastTTL(int);
      int IpMulticastTTL();
      bool SetMulticastLoop(bool x = true);
      bool IpAddMembership(struct ip_mreq&);
      bool IpDropMembership(struct ip_mreq&);

#ifdef IP_PKTINFO
      bool SetIpPktinfo(bool x = true);
#endif
#ifdef IP_RECVTOS
      bool SetIpRecvTOS(bool x = true);
#endif
#ifdef IP_RECVTTL
      bool SetIpRecvTTL(bool x = true);
#endif
#ifdef IP_RECVOPTS
      bool SetIpRecvopts(bool x = true);
#endif
#ifdef IP_RETOPTS
      bool SetIpRetopts(bool x = true);
#endif
#ifdef IP_RECVERR
      bool SetIpRecverr(bool x = true);
#endif
#ifdef IP_MTU_DISCOVER
      bool SetIpMtudiscover(bool x = true);
#endif
#ifdef IP_MTU
      int IpMtu();
#endif
#ifdef IP_ROUTER_ALERT
      bool SetIpRouterAlert(bool x = true);
#endif
#ifdef LINUX
      bool IpAddMembership(struct ip_mreqn&);
#endif
#ifdef LINUX
      bool IpDropMembership(struct ip_mreqn&);
#endif
      //@}

      // SOCKET options
      /** @name base_socket Options */
      //@{

      bool SoAcceptconn();
      bool SetSoBroadcast(bool x = true);
      bool SetSoDebug(bool x = true);
      int SoError();
      bool SetSoDontroute(bool x = true);
      bool SetSoLinger(int onoff, int linger);
      bool SetSoOobinline(bool x = true);
      bool SetSoRcvlowat(int);
      bool SetSoSndlowat(int);
      bool SetSoRcvtimeo(struct timeval&);
      bool SetSoSndtimeo(struct timeval&);
      bool SetSoRcvbuf(int);
      int SoRcvbuf();
      bool SetSoSndbuf(int);
      int SoSndbuf();
      int SoType();
      bool SetSoReuseaddr(bool x = true);
      bool SetSoKeepalive(bool x = true);

#ifdef SO_BSDCOMPAT
      bool SetSoBsdcompat(bool x = true);
#endif
#ifdef SO_BINDTODEVICE
      bool SetSoBindtodevice(const string & intf);
#endif
#ifdef SO_PASSCRED
      bool SetSoPasscred(bool x = true);
#endif
#ifdef SO_PEERCRED
      bool SoPeercred(ucred & );
#endif
#ifdef SO_PRIORITY
      bool SetSoPriority(int);
#endif
#ifdef SO_RCVBUFFORCE
      bool SetSoRcvbufforce(int);
#endif
#ifdef SO_SNDBUFFORCE
      bool SetSoSndbufforce(int);
#endif
#ifdef SO_TIMESTAMP
      bool SetSoTimestamp(bool x = true);
#endif
#ifdef SO_NOSIGPIPE
      bool SetSoNosigpipe(bool x = true);
#endif
      //@}

      // TCP options in tcp_socket.h/tcp_socket.cpp







      /** Called after OnRead if base_socket is in line protocol mode.
      \sa SetLineProtocol */
      /** Enable the OnLine callback. Do not create your own OnRead
      * callback when using this. */
      virtual void SetLineProtocol(bool = true);

      /** Check line protocol mode.
      \return true if base_socket is in line protocol mode */
      bool LineProtocol();






      /** @name SSL Support */
      //@{
      /** SSL client/server support - internal use. \sa tcp_socket */
      virtual void OnSSLConnect();
      /** SSL client/server support - internal use. \sa tcp_socket */
      virtual void OnSSLAccept();
      /** SSL negotiation failed for client connect. */
      virtual void OnSSLConnectFailed();
      /** SSL negotiation failed for server accept. */
      virtual void OnSSLAcceptFailed();
      /** new SSL support */
      virtual bool SSLNegotiate();
      /** Check if SSL is Enabled for this tcp_socket.
      \return true if this is a tcp_socket with SSL enabled */
      bool IsSSL();
      /** Enable SSL operation for a tcp_socket. */
      void EnableSSL(bool x = true);
      /** Still negotiating ssl connection.
      \return true if ssl negotiating is still in progress */
      bool IsSSLNegotiate();
      /** Set flag indicating ssl handshaking still in progress. */
      void SetSSLNegotiate(bool x = true);
      /** OnAccept called with SSL Enabled.
      \return true if this is a tcp_socket with an incoming SSL connection */
      bool IsSSLServer();
      /** Set flag indicating that this is a tcp_socket with incoming SSL connection. */
      void SetSSLServer(bool x = true);
      /** SSL; get pointer to ssl context structure. */
      //         virtual SSL_CTX *GetSslContext() { return NULL; }
      /** SSL; get pointer to ssl structure. */
      //       virtual SSL *GetSsl() { return NULL; }
      //@}
      /** Enable ipv6 for this base_socket. */
      void SetIpv6(bool x = true);
      /** Check ipv6 base_socket.
      \return true if this is an ipv6 base_socket */
      bool IsIpv6();
      /** @name Connection Pool */
      //@{
      /** Client = connecting tcp_socket. */
      void SetIsClient();
      /** base_socket type from base_socket() call. */
      void SetSocketType(int x);
      /** base_socket type from base_socket() call. */
      int GetSocketType();
      /** Protocol type from base_socket() call. */
      void SetSocketProtocol(const string & x);
      /** Protocol type from base_socket() call. */
      const string & GetSocketProtocol();
      /** Instruct a client base_socket to stay open in the connection pool after use.
      If you have connected to a server using tcp, you can call SetRetain
      to leave the connection open after your base_socket instance has been deleted.
      The next connection you make to the same server will reuse the already
      opened connection, if it is still available.
      */
      void SetRetain();
      /** Check retain flag.
      \return true if the base_socket should be moved to connection pool after use */
      bool Retain();
      /** copy connection parameters from sock. */
      void CopyConnection(base_socket *sock);
      //@}
      /** \name Socks4 support */
      //@{
      /** Socks4 client support internal use. \sa tcp_socket */
      virtual void OnSocks4Connect();
      /** Socks4 client support internal use. \sa tcp_socket */
      virtual void OnSocks4ConnectFailed();
      /** Socks4 client support internal use. \sa tcp_socket */
      virtual bool OnSocks4Read();
      /** Called when the last write caused the tcp output buffer to
      * become is_empty. */
      /** base_socket still in socks4 negotiation mode */
      bool Socks4();
      /** Set flag indicating Socks4 handshaking in progress */
      void SetSocks4(bool x = true);

      /** Set socks4 server host address/port to use */
      void SetSocks4Host(in_addr a);
      /** Set socks4 server hostname to use. */
      void SetSocks4Host(const string & );
      /** Socks4 server port to use. */
      void SetSocks4Port(port_t p);
      /** Provide a socks4 userid if required by the socks4 server. */
      void SetSocks4Userid(const string & x);
      /** get the ip address/port of socks4 server to use.
      \return socks4 server host address/port */
      in_addr GetSocks4Host();
      /** get the socks4 server port to use.
      \return socks4 server port */
      port_t GetSocks4Port();
      /** get socks4 userid.
      \return Socks4 userid */
      const string & GetSocks4Userid();
      //@}
      /** \name Asynchronous Resolver */
      //@{
      /** Request an asynchronous dns resolution.
      \param host hostname to be resolved
      \param port port number passed along for the ride
      \return Resolve ID */
      int Resolve(const string & host,port_t port = 0);
      int Resolve6(const string & host, port_t port = 0);
      /** Callback returning a resolved ::net::address.
      \param id Resolve ID from Resolve call
      \param a resolved ip address/port
      \param port port number passed to Resolve */
      virtual void OnResolved(int id, const ::net::address & addr);
      //virtual void OnResolved(int id, in6_addr & a, port_t port);
      /** Request asynchronous reverse dns lookup.
      \param a in_addr to be translated */
      int Resolve(in_addr a);
      int Resolve(in6_addr& a);
      /** Callback returning reverse resolve results.
      \param id Resolve ID
      \param name Resolved hostname */
      virtual void OnReverseResolved(int id,const string & name);
      /** Callback indicating failed dns lookup.
      \param id Resolve ID */
      virtual void OnResolveFailed(int id);
      //@}
      /** \name Thread Support */
      //@{
      /** Callback fires when a new base_socket thread has started and this
      base_socket is ready for operation again.
      \sa resolv_socket */
      virtual void OnDetached();

      // LIST_DETACH

      /** Internal use. */
      void SetDetach(bool x = true);
      /** Check detach flag.
      \return true if the base_socket should detach to its own thread */
      bool IsDetach();

      /** Internal use. */
      void SetDetached(bool x = true);
      /** Check detached flag.
      \return true if the base_socket runs in its own thread. */
      const bool IsDetached() const;
      /** Order this base_socket to start its own thread and call OnDetached
      when ready for operation. */
      bool detach();
      /** Store the slave sockethandler pointer. */
      void SetSlaveHandler(base_socket_handler *);
      /** create new thread for this base_socket to run detached in. */
      void DetachSocket();
      //@}

      /** write traffic to an IFile. base_socket will not delete this object. */
      void SetTrafficMonitor(::file::file_sp p) { m_spfileTrafficMonitor = p; }
      /** All traffic will be written to this IFile, if set. */
      ::file::file_sp GetTrafficMonitor() { return m_spfileTrafficMonitor; }

      /** \name Triggers */
      //@{
      /** Subscribe to trigger id. */
      void Subscribe(int id);
      /** Unsubscribe from trigger id. */
      void Unsubscribe(int id);
      /** Trigger callback, with data passed from source to destination. */
      virtual void OnTrigger(int id, const trigger_data & data);
      /** Trigger cancelled because source has been deleted (as in delete). */
      virtual void OnCancelled(SOCKET id);
      //@}


      virtual void run();
      virtual void step();

#ifdef DEBUG

      virtual void log(const string & strUser, int32_t err, const string & strSystem, ::aura::log::e_level elevel = ::aura::log::level_warning);

#endif


      virtual string get_short_description();

   };

   typedef map < base_socket *, base_socket *, bool, bool > socket_bool;
   typedef map < SOCKET, SOCKET, socket_bool, socket_bool > socket_socket_bool;
   typedef map < SOCKET, SOCKET, sp(base_socket), sp(base_socket) > socket_map;
   typedef ::comparable_eq_list < sp(base_socket) > socket_list;


} // namespace sockets


