#pragma once


namespace install
{


   class CLASS_DECL_BASE install :
      virtual public ::base_departament,
      virtual public int_scalar_source
   {
   public:

      bool m_bAdmin;

      string m_strId;

      string_to_string m_strmapLatestBuildNumber;

      uint32_t m_dwLatestBuildNumberLastFetch;

      HANDLE m_hmutexBoot;
      UINT g_nCa2StarterStartThreadID;
      bool g_bCa2Installed;
      bool g_bCa2Updated;
      bool g_bInstallingCa2;
      bool g_bSpaInstalled;
      bool g_bSpaUpdated;
      bool g_bUpdated;
      char * g_pszCa2Build ;
      oswindow g_oswindowMessage;

      mutex m_mutex;


      class trace m_trace;


      int_progress         m_progressApp;
      int64_t              m_iProgressCountApp;
      int64_t              m_iProgressStepApp;




      install(sp(base_application) papp);
      virtual ~install();



      inline class trace & trace() { return m_trace; }


      virtual bool is_file_ok(const char * path1, const char * pszTemplate);
      virtual bool is_lock_file_locked();
      virtual int32_t synch_install(const char * pszCommandLine, bool bBackground = true);
      virtual int32_t asynch_install(const char * pszCommandLine, bool bBackground = true);



      virtual bool is_admin();
      virtual void set_admin(bool bSet);
      virtual string get_id();
      virtual void set_id(const char * psz);
      virtual string get_platform();


      virtual string get_latest_build_number(const char * pszVersion);
      virtual string fetch_latest_build_number(const char * pszVersion);

      int32_t spalib_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow);


      int32_t spaboot_registry_register(const char * lpszFile);


      int32_t start_app(const char * id);


      virtual void installation_file_lock(bool bLock);


      virtual string get_installation_lock_file_path();




      virtual void update_ca2_build();
      virtual const char * ca2_get_build();
      virtual void update_ca2_installed(bool bUnloadIfNotInstalled = true);
      virtual bool is_ca2_installed();
      virtual void update_ca2_updated();
      virtual bool is_ca2_updated();
      virtual void set_ca2_updated(const char * pszBuild);
      virtual void update_updated();
      virtual bool is_updated();
      virtual void set_updated(const char * pszBuild);


      virtual void set_installing_ca2(bool bSet = true);
      virtual bool is_installing_ca2();

      const char * get_starter_version();
      const char * get_version();
      const char * get_ca2_version();

      virtual bool app_install_ensure_executable(string & strPath);
      virtual void app_install_call_sync(const char * szParameters);
      virtual bool app_install_send_short_message(const char * psz, bool bLaunch);
      virtual void app_install_send_response(const char * param);



      virtual void add_spa_start(const char * pszType, const char * pszId);
      virtual void remove_spa_start(const char * pszType, const char * pszId);
      virtual void add_app_install(const char * pszBuild, const char * pszType, const char * pszId, const char * pszLocale, const char * pszSchema);
virtual bool is_installed(const char * pszVersion, const char * pszBuild, const char * pszType, const char * pszId, const char * pszLocale, const char * pszSchema);
          virtual bool is(const char * pszVersion, const char * pszBuild, const char * pszType, const char * pszId, const char * pszLocale, const char * pszSchema);
      virtual int32_t  start(const char * pszCommandLine);
      virtual int32_t  synch(const char * pszCommandLine);

      virtual void on_set_scalar(e_scalar escalar, int64_t iValue);
      virtual void get_scalar_minimum(e_scalar escalar, int64_t & i);
      virtual void get_scalar(e_scalar escalar, int64_t & i);
      virtual void get_scalar_maximum(e_scalar escalar, int64_t & i);


   };


} // namespace spa








