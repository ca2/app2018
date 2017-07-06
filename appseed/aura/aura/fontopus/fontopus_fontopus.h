#pragma once


namespace fontopus
{


   class CLASS_DECL_AURA fontopus :
      virtual public ::aura::department
   {
   public:


      mutex                                        m_mutex;
      class create_user_thread *                   m_pthreadCreatingUser;
      sp(user)                                     m_puser;
      string_to_string                             m_mapFontopusServer;
      string                                       m_strFirstFontopusServer;
      string                                       m_strFirstAccountServer;
      string                                       m_strBestFontopusServerIp;
      string                                       m_strBestApiServer;
      authentication_map                           m_authmap;
      //string_map < ::sockets::http_session * >     m_mapFontopusSession;
      string_map < stringa >                       m_mapSomeBrothersAndSisters;
      string_to_string                             m_mapFontopusRsa;
      string_to_string                             m_mapFontopusSessId;
      string_to_string                             m_mapLabelUser;
      string_to_string                             m_mapLabelPass;
      string_to_string                             m_mapLabelOpen;
      //::sockets::socket_handler *                  m_phandler;


      fontopus(::aura::application * papp);
      virtual ~fontopus();


      virtual bool initialize_user(::fontopus::user * puser);
      virtual bool create_system_user(const string & strSystemUserName);
      virtual bool create_current_user(const char * psz = NULL);
      virtual sp(user) allocate_user();
      virtual sp(user) login(property_set & set);

      virtual void logout();

      virtual user * get_user(bool bSynch = true, const char * pszRequestUrl = NULL);
      virtual void set_user(const char * psz);
      virtual void set_user(::fontopus::user * psz);


      virtual bool get_auth(const char * psz, string & strUsername, string & strPassword);

      virtual bool check_license(const char * pszId, bool bInteractive);

      virtual void on_user_login(::fontopus::user * puser);

      virtual void on_request(::create * pcreate);


      virtual string get_server(const char * pszSourceUrl, int32_t iRetry = 8);

      virtual string get_fontopus_server(const char * pszRequestingServerOrUrl,int iRetry = 8);

      void cleanup_fontopus();
      void cleanup_networking();

   };



   class CLASS_DECL_AURA create_user_thread:
      virtual public thread
   {
   public:


      manual_reset_event m_evReady;
      string m_strRequestUrl;

      create_user_thread(::aura::application * papp);
      virtual ~create_user_thread();

      int32_t run();



   };


} // namespace fontopus





