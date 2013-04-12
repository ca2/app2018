#pragma once

namespace mail
{

   class CLASS_DECL_ca pop3_socket : 
      public ::sockets::tcp_socket
   {
   public:
   
      enum e_state
      {
         state_disconnected,
         state_auth_1,
         state_auth_2,
         state_transaction,
         state_listmessages_start,
         state_listmessages,
         state_receive_message,
         state_receive_headers,
         state_receive_body,
         state_finished,
         state_stat_start,
         state_top_start,
         state_receive_top_headers,
         state_receive_top_body,
         state_receive_specific_list,
         state_receive_list,
         state_retr_only_headers_start,
         state_retr_only_headers,
         state_retr_start,
         state_retr_headers,
         state_retr_body,
         state_quit,
         state_rset,
      };

      bool retrieve();

      void InitSSLClient();

      pop3_socket(::sockets::socket_handler & handler);
      ~pop3_socket();

      void OnLine(const string & );

      pop3 *   m_ppop3;
      bool m_bSsl;
      bool m_bTls;
      string m_strKey;
      string m_strValue;


   



   public:
      void SendPrompt();
      e_state  m_estate;
   };

} // namespace mail