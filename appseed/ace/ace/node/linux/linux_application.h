#pragma once


namespace linux
{


   class CLASS_DECL_ACE main_init_data :
      public ::ace::main_init_data
   {
   public:


      HINSTANCE   m_hInstance;
      HINSTANCE   m_hPrevInstance;
      int32_t         m_nCmdShow;


   };


   class CLASS_DECL_ACE application :
      virtual public ::ace::application
   {
   public:



      USHORT               m_atomApp;
      USHORT               m_atomSystemTopic;

      main_init_data *     m_pmaininitdata;


      application(sp(::ace::application) papp);
      virtual ~application();

      virtual HINSTANCE GetHinstance();
      bool _001OnDDECommand(const char * lpcsz);
      virtual void _001EnableShellOpen();
      //virtual ::ace::document * _001OpenDocumentFile(var varFile);
      virtual void _001OnFileNew();


      virtual string get_version();

      virtual void TermThread(HINSTANCE hInstTerm);


      virtual void SetCurrentHandles();

      virtual bool set_main_init_data(::ace::main_init_data * pdata);

      virtual bool process_initialize();
      virtual bool initialize1();
      virtual bool initialize2();
      virtual bool initialize3();
      virtual int32_t  exit_instance();




      virtual bool win_init(main_init_data * pdata);

//	  virtual bool update_module_paths();































      /*virtual void construct(__THREADPROC pfnThreadProc, LPVOID pParam);

      virtual bool Begin(int32_t nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
         DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

      bool create_thread(DWORD dwCreateFlags = 0, UINT nStackSize = 0,
         LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

      virtual int_ptr get_os_data();
      virtual int_ptr get_os_int();


      int32_t GetThreadPriority();
      bool SetThreadPriority(int32_t nPriority);

   // Operations
      DWORD SuspendThread();
      DWORD ResumeThread();
      bool post_thread_message(UINT message, WPARAM wParam, LPARAM lParam);
      bool post_message(sp(::user::primitive) pguie, UINT message, WPARAM wParam, LPARAM lParam);

      virtual bool PreInitInstance();

      // called when occurs an standard_exception exception in run
      // return true to call run again
      virtual bool on_run_exception(::exception::exception & e);

   // Overridables
      // thread initialization
      virtual bool initialize_instance();

      virtual ::message::e_prototype GetMessagePrototype(UINT uiMessage, UINT uiCode);

      // running and idle processing
      virtual int32_t run();
      virtual void pre_translate_message(::signal_details * pobj);
      virtual bool pump_message();     // low level message pump
      virtual bool on_idle(LONG lCount); // return TRUE if more idle processing
      virtual bool is_idle_message(MESSAGE* pMsg);  // checks for special messages

      // thread termination
      virtual int32_t exit_instance(); // default will 'delete this'

      // Advanced: exception handling
      virtual LRESULT ProcessWndProcException(::exception::ace* e, const MESSAGE* pMsg);

      // Advanced: handling messages sent to message filter hook
      virtual bool ProcessMessageFilter(int32_t code, LPMESSAGE lpMsg);

      // Advanced: virtual access to GetMainWnd()
      virtual sp(::user::primitive) GetMainWnd();

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      virtual void CommonConstruct();
      virtual void Delete();
         // 'delete this' only if m_bAutoDelete == TRUE



      bool DispatchThreadMessageEx(MESSAGE* msg);  // helper*/

      //::draw2d::graphics * graphics_from_os_data(void * pdata);

      virtual void ShowWaitCursor(bool bShow = true);

      ::user::primitive * window_from_os_data(void * pdata);
      ::user::primitive * window_from_os_data_permanent(void * pdata);

      virtual ::thread * GetThread();
      virtual void set_thread(::thread * pthread);

      virtual ::user::primitive * FindWindow(const char * lpszClassName, const char * lpszWindowName);
      virtual ::user::primitive * FindWindowEx(oswindow hwndParent, oswindow hwndChildAfter, const char * lpszClass, const char * lpszWindow);

      virtual void get_time(struct timeval *p);
      virtual void set_env_var(const string & var,const string & value);
      virtual IDTHREAD get_thread_id();
      string draw2d_get_default_library_name();

      string multimedia_audio_get_default_library_name();
      string multimedia_audio_mixer_get_default_library_name();
      string veriwell_multimedia_music_midi_get_default_library_name();

   };



} // namespace linux






