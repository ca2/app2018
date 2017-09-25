


namespace axis
{


   class CLASS_DECL_AXIS system:
      virtual public ::aura::system,
      virtual public ::axis::application
   {
   public:


      //::url::department                           m_urldepartment;

      ::net::email_department                     m_emaildepartment;

      ::visual::visual                             m_visual;

      string_map < int_to_string >                 m_mapEnumToName;
      string_map < string_to_int >                 m_mapNameToEnum;



      bool                                         m_bSystemSynchronizedCursor;
      bool                                         m_bSystemSynchronizedScreen;


#ifdef WINDOWSEX


      raw_array < MONITORINFO >                    m_monitorinfoa;
      raw_array < HMONITOR >                       m_hmonitora;
      raw_array < MONITORINFO >                    m_monitorinfoaDesk;

#endif

      system(::aura::application * papp);
      virtual ~system();

      bool initialize() override;





      virtual void construct(const char * pszAppId) override;

      virtual bool initialize_application() override;
      virtual int32_t exit_application() override;

      virtual bool finalize() override;

      //::url::department                           & url()     { return m_urldepartment; }


      ::net::email_department               & email();
      inline class ::visual::visual                & visual() { return m_visual; }



      //virtual string install_get_platform() override;
      //virtual void install_set_platform(const char * pszPlatform) override;
      //virtual string install_get_version() override;
      //virtual void install_set_version(const char * pszVersion) override;
      //virtual string install_get_latest_build_number(const char * pszVersion) override;
      //virtual int32_t install_start(const char * pszCommandLine, const char * pszBuild) override;
      //virtual int32_t install_progress_app_add_up(int iAddUp = 1) override;

      //virtual ::install::canvas * install_create_canvas();
      //virtual void install_canvas_on_paint(::draw2d::graphics * pgraphics, const RECT & rect);
      //virtual int install_canvas_increment_mode();

      //virtual string install_get_platform() override;
      //virtual void install_set_platform(const char * pszPlatform) override;
      //virtual string install_get_version() override;
      //virtual void install_set_version(const char * pszVersion) override;
      //virtual string install_get_latest_build_number(const char * pszVersion) override;
      //virtual int32_t install_start(const char * pszCommandLine,const char * pszBuild) override;
      //virtual int32_t install_progress_app_add_up(int iAddUp = 1) override;

      virtual bool process_initialize() override;

      virtual bool initialize1() override;

      virtual bool initialize2() override;


      virtual bool verb() override; // ambigous inheritance from ::aura::system/::axis::application


      virtual ::file::path dir_appmatter_locator(::aura::application * papp) override;



      virtual bool is_system() override;


      virtual string crypto_md5_text(const string & str) override;




      virtual ::aura::session * on_create_session() override;

      virtual void on_request(::create * pcreate) override;



      //virtual void construct(const char * pszAppId);

      //virtual bool initialize_application() override;

      //virtual int32_t exit_instance();
      //virtual bool finalize();






      //virtual bool verb();





      //virtual bool process_initialize();

      //virtual bool initialize1();

      //virtual bool initialize2();

      //virtual bool is_system();




      virtual ::user::document * place_hold(::user::interaction * pui);

      virtual ::aura::session * query_session(index iEdge) override;



      void enum_display_monitors();

#if defined(WINDOWS)
      static_function BOOL CALLBACK monitor_enum_proc(HMONITOR hmonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData);
      void monitor_enum(HMONITOR hmonitor,HDC hdcMonitor,LPRECT lprcMonitor);
#endif

      virtual index get_main_monitor(LPRECT lprect = NULL);
      virtual ::count get_monitor_count();
      virtual bool  get_monitor_rect(index iMonitor,LPRECT lprect);
      virtual ::count get_desk_monitor_count();
      virtual bool  get_desk_monitor_rect(index iMonitor,LPRECT lprect);

      virtual index get_main_wkspace(LPRECT lprect = NULL);
      virtual ::count get_wkspace_count();
      virtual bool  get_wkspace_rect(index iWkspace,LPRECT lprect);
      virtual ::count get_desk_wkspace_count();
      virtual bool  get_desk_wkspace_rect(index iWkspace,LPRECT lprect);


      /*
      virtual string get_ca2_module_folder();
      virtual string get_ca2_module_file_path();
      virtual string get_module_folder();
      virtual string get_module_file_path();
      virtual string get_module_title();
      virtual string get_module_name();
      */

      //virtual string dir_appmatter_locator(::aura::application * papp);

      virtual ::user::interaction_impl * impl_from_handle(void * posdata);
      virtual ::user::interaction * ui_from_handle(void * posdata);

      virtual void hist_hist(const char * psz);


      //virtual void on_request(::create * pcreate);


      //virtual uint32_t crc32(uint32_t dwPrevious, const char * psz);


	  virtual string url_encode(const string & str) override;

   };


} // namespace axis







void CLASS_DECL_AXIS __start_system(::axis::system * psystem);




