#pragma once


namespace linux
{


   class CLASS_DECL_AURA application :
      virtual public ::aura::application
   {
   public:



      WINUSHORT               m_atomApp;
      WINUSHORT               m_atomSystemTopic;


      application(sp(::aura::application) papp);
      virtual ~application();

      virtual HINSTANCE GetHinstance();
      bool _001OnDDECommand(const char * lpcsz);
      virtual void _001EnableShellOpen();
      //virtual ::aura::document * _001OpenDocumentFile(var varFile);
      virtual void _001OnFileNew();


      virtual string get_version();

      virtual void TermThread(HINSTANCE hInstTerm);


      virtual void SetCurrentHandles();

      virtual bool process_command(::command::command * pdata);

      virtual bool process_init();
      virtual bool init1();
      virtual bool init2();
      virtual bool init3();
      virtual void term_instance() override;




      virtual bool startup_command(::command::command * pdata);

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
      virtual bool init_application();

      virtual ::message::e_prototype GetMessagePrototype(UINT uiMessage, UINT uiCode);

      // running and idle processing
      virtual int32_t run();
      virtual void pre_translate_message(::message::message * pobj);
      virtual bool pump_message();     // low level message pump
      virtual bool on_idle(LONG lCount); // return TRUE if more idle processing
      virtual bool is_idle_message(MESSAGE* pMsg);  // checks for special messages

      // thread termination
      virtual int32_t term_instance() override; // default will 'delete this'

      // Advanced: exception handling
      virtual LRESULT ProcessWndProcException(::exception::aura* e, const MESSAGE* pMsg);

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






