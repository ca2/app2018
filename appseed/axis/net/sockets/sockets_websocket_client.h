#pragma once


namespace sockets
{


   class CLASS_DECL_AXIS websocket_client :
      virtual public http_client_socket
   {
   public:

      enum e_ping
      {

         ping_none,
         ping_sent_ping,
         ping_pong_received,

      };

      bool        m_bTls;
      string      m_strBase64;
      bool        m_bRequestSent;
      bool        m_bWebSocket;
      string      m_strWebSocketProtocol;
      memory      m_memResponse;
      memory      m_memReceivedData;
      bool        m_bUseMask;
      DWORD       m_dwLastPing;
      DWORD       m_dwLastPong;
      e_ping      m_eping;




      websocket_client(base_socket_handler&);
      websocket_client(base_socket_handler&, const string & url, const string & strProtocol);
      //websocket_client(base_socket_handler&, const string & host, port_t port, const string & url);
      virtual ~websocket_client();

      virtual void InitSSLClient() override;

      virtual void step() override;

      virtual void OnLine(const string &) override;

      virtual void OnHeaderComplete() override;

      virtual long cert_common_name_check(const char * common_name) override;

      virtual void OnDataComplete();

      virtual void send_json(var varJson);
      virtual void send_memory(memory & memory);

      virtual void OnRawData(char *buf, size_t len) override;

      virtual void on_websocket_data(byte * pdata, int len);
      virtual void on_websocket_data(string str);

      virtual bool client_ping_pong_ok();

   };


} // namespace sockets


CLASS_DECL_AXIS void websocket_prefix_varuint(memory & m, unsigned int ui);



