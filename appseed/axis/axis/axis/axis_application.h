#pragma once


namespace axis
{


   class CLASS_DECL_AXIS application :
      virtual public ::aura::application,
      virtual public ::user::interactive,
      virtual public ::database::client
   {
   public:


      bool                                            m_bInitializeDataCentral;
      bool                                            m_bAxisProcessInitialize;
      bool                                            m_bAxisProcessInitializeResult;

      bool                                            m_bAxisInitializeInstance;
      bool                                            m_bAxisInitializeInstanceResult;

      bool                                            m_bAxisInitialize1;
      bool                                            m_bAxisInitialize1Result;

      bool                                            m_bAxisInitialize;
      bool                                            m_bAxisInitializeResult;

      string                                          m_strLicense;


      bool                                            m_bUpdateMatterOnInstall;


      int32_t                                         m_iWaitCursorCount;         // for wait cursor (>0 => waiting)



      sp(::simpledb::simpledb)                        m_psimpledb;






      //static UINT                                     APPM_LANGUAGE;
      //static WPARAM                                   WPARAM_LANGUAGE_UPDATE;

      ::user::interaction *                           m_pwndMain;




      application();
      virtual ~application();


      virtual void assert_valid() const override;
      virtual void dump(dump_context & dumpcontext) const override;


      virtual bool app_data_get(::id id, serialize & os) override;
      virtual bool app_data_set(::id id, serialize & is) override;

      virtual bool app_data_set(::id id, object & obj) override;
      virtual bool app_data_get(::id id, object & obj) override;


      virtual string fontopus_get_cred(::aura::application * papp, const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive, ::user::interactive * pinteractive) override;
      virtual string fontopus_get_cred(string & strUsername, string & strPassword, string strToken);
      virtual void set_cred(string strToken, const char * pszUsername, const char * pszPassword);
      virtual void set_cred_ok(string strToken, bool bOk);

      virtual ::database::key calc_data_key() override;

      virtual string load_string(id id) override;
      virtual bool load_string(string & str, id id) override;
      virtual void load_string_table() override;
      virtual bool load_cached_string(string & str, id id, bool bLoadStringTable) override;
      virtual bool load_cached_string_by_id(string & str, id id, bool bLoadStringTable) override;
      virtual void load_string_table(const string & pszApp, const string & pszId) override;



      virtual bool is_system() override;
      virtual bool is_session() override;
      virtual bool is_serviceable() override;


      inline class ::simpledb::simpledb         & simpledb() { return *m_psimpledb; }
      inline ::database::server &               dataserver() { return *m_psimpledb->get_data_server(); }


      virtual bool verb() override;


      virtual bool Ex2OnAppInstall() override;
      virtual bool Ex2OnAppUninstall() override;

      virtual bool init_application() override;

      virtual bool init1() override;
      virtual bool init2() override;
      virtual bool init3() override;


      virtual bool init() override;


      virtual bool init_instance() override;


      virtual void run() override;

      virtual void main() override;
      virtual void on_run() override;
      virtual bool application_pre_run() override;
      virtual bool initial_check_directrix() override;
      virtual bool os_native_bergedge_start() override;




      virtual void term() override;


      virtual void term_application() override;




      virtual bool InitApplication() override;


      virtual bool do_install() override;
      virtual bool do_uninstall() override;


      virtual bool on_install() override;
      virtual bool on_uninstall() override;





      virtual bool update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession, const ::file::path & pszRoot, const string & pszRelative);
      virtual bool update_appmatter(::sockets::socket_handler & handler, sp(::sockets::http_session) & psession, const ::file::path & pszRoot, const string & pszRelative, const string & strLocale, const string & strStyle);


      virtual void SetCurrentHandles() override;










      virtual bool final_handle_exception(::exception::exception * pe) override;




      virtual sp(::aura::application) assert_running(const char * pszAppId) override;

      virtual bool is_running() override;



      virtual void on_request(::create * pcreate) override;



      virtual bool assert_user_logged_in() override;

      virtual string matter_as_string(const char * pszMatter, const char * pszMatter2 = NULL);

      virtual bool file_is_read_only(const char * pszPath);






      virtual void on_create_keyboard();




      virtual int32_t simple_message_box(::user::primitive * puiOwner, const char * pszMessage, UINT fuStyle = MB_OK) override;
      virtual int32_t simple_message_box_timeout(::user::primitive * pwndOwner, const char * pszMessage, ::duration durationTimeOut, UINT fuStyle) override;
      int32_t simple_message_box(const char * pszMessage, UINT fuStyle);
      virtual string message_box(const string & pszMatter, property_set & propertyset) override;



      virtual bool do_prompt_file_name(var & varFile, UINT nIDSTitle, uint32_t lFlags, bool bOpenFileDialog, ::user::impact_system * ptemplate, ::user::document * pdocument);


      virtual void process_message_filter(int32_t code, ::message::message * pobj) override;



      virtual void DoWaitCursor(int32_t nCode) override; // 0 => restore, 1=> begin, -1=> end

      virtual void _001CloseApplication() override;


      virtual string get_license_id() override;




      virtual bool process_init() override;





      virtual void TermThread(HINSTANCE hInstTerm) override;


      virtual void set_env_var(const string & var, const string & value) override;


      virtual sp(::aura::printer) get_printer(const char * pszDeviceName);


      virtual ::visual::icon * set_icon(object * pobject, ::visual::icon * picon, bool bBigIcon) override;

      virtual ::visual::icon * get_icon(object * pobject, bool bBigIcon) const override;

      virtual void on_service_request(::create * pcreate) override;

      virtual string get_mutex_name_gen() override;

      //virtual void draw2d_factory_exchange();

      //virtual sp(::message::base) get_message_base(LPMESSAGE lpmsg) override;
      virtual ::user::primitive * window_from_os_data(void * pdata);


      virtual bool set_keyboard_layout(const char * pszPath, ::action::context actioncontext);

      virtual int32_t hotplugin_host_starter_start_sync(const char * pszCommandLine, ::aura::application * papp, hotplugin::host * phost, hotplugin::plugin * pplugin = NULL) override;
      virtual int32_t hotplugin_host_host_starter_start_sync(const char * pszCommandLine, ::aura::application * papp, hotplugin::host * phost, hotplugin::plugin * pplugin = NULL);

      virtual void on_update_view(::user::impact * pview, ::user::impact * pviewSender, LPARAM lHint, object* pHint);

      virtual void on_control_event(::user::control_event * pevent) override;



      virtual bool on_open_document(::user::document * pdocument, var varFile);
      virtual bool on_save_document(::user::document * pdocument, var varFile);



      inline ::html::html * html() { return m_papp->m_psystem->m_phtml; }

      virtual ::html::html * create_html();

      virtual string http_get(const string & strUrl, ::property_set & set) override;

      virtual bool compress_ungz(stream & ostreamUncompressed, const ::file::path & lpcszGzFileCompressed) override;
      virtual bool compress_ungz(::primitive::memory_base & mem) override;
      virtual bool compress_gz(::file::file * pfileOut, const ::file::path & lpcszUncompressed, int iLevel = 6) override;
      virtual bool compress_gz(::file::file * pfileOut, ::file::file * pfileIn, int iLevel = 6) override;


      virtual bool is_local_data() override;


   };


} // namespace axis










