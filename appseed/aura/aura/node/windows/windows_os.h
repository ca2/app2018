#pragma once


CLASS_DECL_AURA HRESULT os_create_link(::file::path pathObj, ::file::path pathLink, string strDesc, ::file::path pathIcon = "", int iIcon = 0);

namespace windows
{


   class CLASS_DECL_AURA os :
      virtual public ::aura::os
   {
   public:


      os(::aura::application * papp);
      virtual ~os();


      virtual string get_command_line();


      virtual bool reboot();
      virtual bool shutdown(bool bPowerOff);

      virtual void terminate_processes_by_title(const char * pszName);
      virtual ::file::path get_module_path(HMODULE hmodule);
      virtual bool get_pid_by_path(const char * pszName, DWORD & dwPid);
      virtual bool get_pid_by_title(const char * pszName, DWORD & dwPid);
      virtual void get_all_processes(uint_array & dwa);
      virtual ::file::path get_process_path(DWORD dwPid);

      virtual int get_pid();


      virtual bool connection_settings_get_auto_detect();
      virtual string connection_settings_get_auto_config_url();


      virtual bool local_machine_set_run(const char * pszKey, const char * pszCommand, bool bSet);
      virtual bool local_machine_set_run_once(const char * pszKey, const char * pszCommand, bool bSet);
      virtual bool current_user_set_run(const char * pszKey, const char * pszCommand, bool bSet);
      virtual bool current_user_set_run_once(const char * pszKey, const char * pszCommand, bool bSet);
      virtual bool defer_register_ca2_plugin_for_mozilla();

      virtual bool file_extension_get_open_with_list_keys(stringa & straKey, const char * pszExtension);
      virtual bool file_extension_get_open_with_list_commands(stringa & straCommand, const char * pszExtension);

      virtual bool file_association_set_default_icon(const char * pszExtension, const char * pszExtensionNamingClass, const char * pszIconPath);
      virtual bool file_association_set_shell_open_command(const char * pszExtension, const char * pszExtensionNamingClass, const char * pszCommand, const char * pszParam);
      virtual bool file_association_get_shell_open_command(const char * pszExtension, string & strExtensionNamingClass, string & strCommand, string & strParam);


      virtual bool open_in_ie(const char * lpcsz);

      virtual bool file_open(::aura::application * papp, ::file::path path, string strParams = "", string strFolder = "") override;

      virtual bool browse_file_open(oswindow oswindowOwner, property_set & set) override;
      virtual bool browse_file_save(oswindow oswindowOwner, property_set & set) override;
      virtual bool browse_folder(oswindow oswindowOwner, property_set & set) override;
      virtual bool browse_file_or_folder(oswindow oswindowOwner, property_set & set) override;

      virtual bool create_service(::aura::application * papp);
      virtual bool remove_service(::aura::application * papp);

      virtual bool start_service(::aura::application * papp);
      virtual bool stop_service(::aura::application * papp);

      virtual bool create_service(const string & strServiceName, const string & strDisplayName, const string & strCommand, const string & strUser = "", const string & strPass = "");
      virtual bool remove_service(const string & strServiceName);

      virtual bool start_service(const string & strServiceName);
      virtual bool stop_service(const string & strServiceName);

      virtual string calc_service_name(::aura::application * papp);


      virtual bool resolve_link(::file::path & pathLink, string & strDirectory, string & strParams, const string & pszSource, ::user::primitive * puiMessageParentOptional = NULL);


      DECLSPEC_NO_RETURN void raise_exception(DWORD dwExceptionCode, DWORD dwExceptionFlags);

      virtual bool is_remote_session();

      virtual void set_file_status(const char * lpszFileName, const ::file::file_status& status);


      virtual bool initialize_wallpaper_fileset(::file::set * pset, bool bAddSearch);

      virtual bool get_default_browser(string & strId, ::file::path & path, string & strParam) override;

      virtual bool register_user_auto_start(string strId, string strCommand, bool bRegister) override;

      virtual bool is_user_auto_start(string strId) override;

      virtual ::file::path get_app_path(const string & strApp) override;

      virtual bool set_default_browser(::aura::application * papp) override;

      virtual bool add_default_program(::aura::application * papp, stringa & straExtension, stringa & straMimeType) override;

   };


} // namespace windows



