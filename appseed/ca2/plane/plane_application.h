#pragma once
   


namespace plane
{


   class CLASS_DECL_ca2 application : 
      virtual public ::ca2::application,
      virtual public ::database::client

   {
   public:

            sp(service_base)                    m_pservice;
      class ::ca::dir::application        m_dir;
      class ::ca::file_application        m_file;
      class ::ca::http::application       m_http;

      sp(class ::fontopus::license)       m_splicense;
      sp(class ::fs::data)                m_spfsdata;

      bool                                m_bIfs;
      bool                                m_bUpdateMatterOnInstall;
      sp(user::user)                      m_spuser;
      sp(::fs::fs)                        m_pfs;
      sp(::fontopus::fontopus)            m_pfontopus;
      sp(::html::html)                    m_phtml;
      ::simpledb::simpledb                m_simpledb;


      application();
      virtual ~application();



      virtual bool on_install();
      virtual bool on_uninstall();


      virtual bool is_serviceable();
      virtual bool is_installing();
      virtual bool is_uninstalling();


      virtual int32_t run();


      service_base * get_service();
      virtual service_base * allocate_new_service();
      virtual bool create_new_service();


      virtual bool create_service();
      virtual bool remove_service();

      virtual bool start_service();
      virtual bool stop_service();


      virtual void on_service_request(sp(::ca::create_context) pcreatecontext);




      virtual int32_t exit_instance();


      inline class ::ca::dir::application       & dir()        { return m_dir          ; }
      inline class ::ca::file_application       & file()       { return m_file         ; }
      inline class ::ca::http::application      & http()       { return m_http         ; }
      inline class ::fontopus::license          & license()    { return m_splicense    ; }
      inline sp(class ::fs::data)               fs()           { return m_spfsdata     ; }
      inline sp(class ::user::user)             user()         { return m_spuser       ; }
      inline sp(::fontopus::fontopus)           fontopus()     { return m_pfontopus    ; }
      inline sp(::html::html)                   html()         { return m_phtml        ; }
      inline class ::simpledb::simpledb         & simpledb()   { return m_simpledb     ; }


      virtual bool process_initialize();


      virtual bool initialize();
      virtual bool initialize1();
      virtual bool initialize2();
      virtual bool initialize_instance();

      virtual void defer_initialize_twf();

      virtual int32_t pre_run();
      virtual int32_t on_run();

      virtual bool main_start();
      virtual int32_t main();
      virtual bool bergedge_start();
      virtual bool os_native_bergedge_start();


      virtual sp(::ca::application) instantiate_application(const char * pszType, const char * pszId, ::ca::application_bias * pbias);
      virtual sp(::ca::application) create_application(const char * pszType, const char * pszId, bool bSynch, ::ca::application_bias * pbias);


      virtual bool is_licensed(const char * pszId, bool bInteractive = true);

      //virtual sp(::user::interaction) get_request_parent_ui(sp(::ca::command_line) pline);

      virtual bool initial_check_directrix();





      virtual void set_title(const char * pszTitle);





      //////////////////////////////////////////////////////////////////////////////////////////////////
      // System/Cube
      //
      sp(::document) hold(sp(::user::interaction) pui);

      virtual ::count get_monitor_count();
      virtual bool  get_monitor_rect(index i, LPRECT lprect);
      virtual ::count get_desk_monitor_count();
      virtual bool  get_desk_monitor_rect(index i, LPRECT lprect);



      //////////////////////////////////////////////////////////////////////////////////////////////////
      // Session/Bergedge
      //
      virtual ::bergedge::view * get_view();
      virtual ::bergedge::document * get_document();


      virtual void fill_locale_schema(::ca::international::locale_schema & localeschema);
      virtual void fill_locale_schema(::ca::international::locale_schema & localeschema, const char * pszLocale, const char * pszSchema);

      virtual bool update_appmatter(::sockets::socket_handler & h, ::sockets::http_session * & psession, const char * pszRoot, const char * pszRelative);
      virtual bool update_appmatter(::sockets::socket_handler & h, ::sockets::http_session * & psession, const char * pszRoot, const char * pszRelative, const char * pszLocale, const char * pszStyle);


      virtual bool add_library(::ca::library * plibrary);

      virtual bool system_add_app_install(const char * pszId);

      virtual ::fontopus::user * get_user();


      virtual ::fontopus::fontopus * create_fontopus();
      virtual ::user::user * create_user();
      virtual ::fs::fs * create_fs();
      virtual ::html::html * create_html();


      virtual void assert_valid() const;
      virtual void dump(dump_context & context) const;


      application();
      application(const char * pszId);
      virtual ~application();

      virtual void construct();
      virtual void construct(const char * pszId);
      
      virtual bool initialize_instance();
      virtual int32_t  exit_instance();

      virtual void _001OnFileNew();
      virtual bool bergedge_start();


      virtual bool is_serviceable();
      virtual service_base * allocate_new_service();


      virtual bool on_install();
      virtual bool on_uninstall();

      virtual int32_t run();

      virtual void on_request(sp(::ca::create_context) pcreatecontext);

      sp(::user::document_interface) _001OpenDocumentFile(var varFile);

      sp(::ca::application) get_system();

      virtual sp(::planebase::application) assert_running(const char * pszAppdId);

#pragma once


namespace user
{

   class printer;


} // namespace user


namespace cubebase
{


   class CLASS_DECL_ca2 application : 
      virtual public ::plane::application,
      virtual public ::ca::smart_pointer < ::cubebase::application >
   {
   public:


      application();
      virtual ~application();


      virtual bool initialize1();
      virtual bool initialize();


      sp(::ca::application) get_system();


      virtual bool is_cube();

      
      virtual void CubeOnFactoryExchange();


      virtual ::user::printer * get_printer(const char * pszDeviceName);



      // ::ca::smart_pointer < ::cubebase::application >::oattrib
      // or any function needing it
      application & operator = (const application & app)
      {

         UNREFERENCED_PARAMETER(app);

         // do nothing

         return *this;

      }

      // get a file and if there are exceptions, should show end user friendly messages
      virtual ::ca::filesp friendly_get_file(var varFile, UINT nOpenFlags);


      virtual ::fontopus::fontopus * create_fontopus();
      virtual ::user::user * create_user();
      virtual ::fs::fs * create_fs();
      virtual ::html::html * create_html();


   };



} // namespace cube




   };

   CLASS_DECL_ca2 UINT c_cdecl application_thread_procedure(LPVOID pvoid);

   typedef sp(::ca::application) (* LPFN_instantiate_application)(sp(::ca::application) pappParent, const char * pszId);

   extern CLASS_DECL_ca2 LPFN_instantiate_application g_lpfn_instantiate_application;


} // namespace plane


