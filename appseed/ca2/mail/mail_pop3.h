#pragma once



namespace mail
{

   class pop3_socket;

   class CLASS_DECL_ca2 pop3 :
      virtual public ::ca::object
   {
   public:

      virtual string get_user();
      virtual string get_pass();
      virtual string get_host();
      virtual string get_transaction();

      virtual void set_stat_count(int32_t iCount);
      virtual void set_stat_size(int32_t iSize);

      virtual void set_list_size(int32_t iSize);

      ::sockets::socket_handler     m_handler;
      pop3_socket *                 m_psocket;
      event                         m_evFinish;
      bool                          m_bRun;
      bool                          m_bSynch;

      stringa                     m_straIndex;
      stringa                     m_straId;

      string                        m_id;
      string                        m_strHeaders;
      string                        m_strBody;
      ::ca::property_set             m_setHeaders;

      int32_t                           m_iHeaderLine;

      critical_section              m_csDataset;



      pop3(sp(::ca::application) papp);

      virtual void update_lists();

      virtual void start();
      virtual void store();

      virtual void filter_id();

      static UINT pop3_thread_proc(LPVOID lpvoid);

      virtual UINT run();

      virtual void on_finished_transaction();

   };

}