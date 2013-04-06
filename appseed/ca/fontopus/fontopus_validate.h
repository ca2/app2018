#pragma once


namespace fontopus
{

   enum e_result
   {
      result_auth,
      result_registration_deferred,
      result_please_finish_registration,
      result_no_login,
      result_no_password,
      result_wrong_password_or_login,
      result_time_out,
      result_fail,
   };

   class CLASS_DECL_ca login_thread_callback :
      virtual public ::ca::object
   {
   public:
      virtual void on_login_thread_response(e_result iAuth, const char * pszResponse);
      virtual string calc_mod_hash();
      virtual string calc_key_hash();
      virtual string calc_ca2_hash();
      virtual bool get_defer_registration() = 0;
   };


   class CLASS_DECL_ca http_execute :
      virtual public ::ca::ca
   {
   public:
      string                  m_strUrl;
      ::ca::property_set       m_propertysetPost;
      ::ca::property_set       m_propertysetHeaders;
      string                  m_strResponse;
   };


   class CLASS_DECL_ca http_execute_array : 
      public spa(http_execute)
   {
   };


   class CLASS_DECL_ca login_thread : public simple_thread
   {
   public:

      user *                     m_puser;

      string                     m_strFontopusServerAuthUrl;
      string                     m_strRequestingServer;
      string                     m_strFontopusServer;
      string                     m_strUsername;
      string                     m_strPassword;
      string                     m_strPasshash;

      string                     m_strModHash;
      string                     m_strKeyHash;
      string                     m_strCa2Hash;

      stringa                    m_straRequestingServer;
      bool                       m_bFontopusServer;
      
      
      string                     m_strValidUntil;

      string                     m_strSalt;

      string                     m_strLicense;
      http_execute_array         m_httpexecutea;
      bool                       m_bOk;
      login_thread_callback *    m_pcallback;
      
      



      login_thread(sp(::ca::application) papp);
      virtual ~login_thread();

      virtual int32_t run();
      virtual string Login(::ca::http::e_status * pestatus);
      virtual string NetLogin(::ca::http::e_status * pestatus);
      void execute();
   };


   class CLASS_DECL_ca validate :
      virtual public ::user::form_callback,
      virtual public ::user::view_creator,
      virtual public login_thread_callback
   {
   public:


      class CLASS_DECL_ca auth
      {
      public:
         string m_strUsername;
         string m_strPassword;
      };


      class CLASS_DECL_ca license
      {
      public:
         string m_strUsername;
         id m_id;
      };



      ::fontopus::user *                        m_puser;
      login_thread                              m_loginthread;
      int32_t                                       m_iView;
      bool                                      m_bAuth;
      auth *                                    m_pauth;
      string                                    m_strForm;
      string                                    m_strLicense;
      bool                                      m_bLicense;
      ::user::network_configuration             m_netcfg;
      ::user::keyboard_layout *                 m_pkeyboardlayout;
      bool                                      m_bInteractive;
      bool                                      m_bDeferRegistration;


      validate(sp(::ca::application) papp, const char * pszForm, bool bAuth = false, bool bInteractive = true);
      virtual ~validate();


      virtual void get_mod(stringa & straHash, stringa & straSource);
      virtual string calc_mod_hash();
      virtual string calc_key_hash();
      virtual string calc_ca2_hash();
      // return hash and check if hash is valid
      virtual bool check_ca2_hash();


      virtual void on_login_thread_response(e_result iAuth, const char * pszResponse);

      virtual void close_all();
      virtual void show_and_request_auth();
      virtual void ensure_main_document();
      virtual void display_main_frame();
      virtual void page1(const stringa & straMatter = stringa());
      virtual void pageMessage(const stringa & straMatter, ::ca::property_set & set);

      virtual ::fontopus::user * get_user(const char * pszHost = ::null(), const char * pszSessid = ::null());
      virtual auth * get_auth();
      virtual bool get_license(const char * pszId);

      virtual void authentication_failed(e_result iAuth, const char * pszResponse);
      virtual void authentication_succeeded();

//      void on_create_view(::user::view_creator_data * pcreatordata);

      virtual void on_show_view();

      virtual bool get_defer_registration();

      
  //    virtual bool BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent);
   };


} // namespace user
