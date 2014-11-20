#include "framework.h"


#undef USERNAME_LENGTH // mysql one


namespace android
{


   os::os(::aura::application * papp) :
      element(papp),
      ::base::os(papp)
   {
   }


   os::~os()
   {
   }


   bool os::shutdown(bool bIfPowerOff)
   {
/*      bool retval = true;
      HANDLE hToken;
      TOKEN_PRIVILEGES tkp;
      if (!OpenProcessToken(GetCurrentProcess(),
         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
         return false;
      LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
      if (bIfPowerOff)
         retval = ExitWindowsEx(EWX_POWEROFF, 0) != FALSE;
      else
         retval = ExitWindowsEx(EWX_SHUTDOWN, 0) != FALSE;

      //reset the previlages
      tkp.Privileges[0].Attributes = 0;
      AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
      return retval;*/

      ::exception::throw_not_implemented(get_app());
      return false;

   }

   bool os::reboot()
   {
/*      HANDLE hToken;
      TOKEN_PRIVILEGES tkp;
      if (!OpenProcessToken(GetCurrentProcess(),
         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
         return false;
      if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
      {
         TRACELASTERROR();
         return false;
      }
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0))
      {
         TRACELASTERROR();
         return false;
      }
      if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
      {
         return false;
      }
      if(!LookupPrivilegeValue(NULL, SE_REMOTE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
      {
         TRACELASTERROR();
         return false;
      }
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0))
      {
         TRACELASTERROR();
         return false;
      }
      if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
      {
         return false;
      }


      if(!WTSShutdownSystem(WTS_CURRENT_SERVER_HANDLE, WTS_WSD_REBOOT))
      {
         TRACELASTERROR();
         return false;
      }
      /*if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE,
      SHTDN_REASON_MAJOR_SOFTWARE | SHTDN_REASON_MINOR_INSTALLATION))
      {
      DWORD dwLastError = ::GetLastError();
      return false;
      }*/
      //reset the previlages
  /*    tkp.Privileges[0].Attributes = 0;
      AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
      return true;*/
      ::exception::throw_not_implemented(get_app());
      return false;

   }

   void os::terminate_processes_by_title(const char * pszName)
   {
      ::exception::throw_not_implemented(get_app());
      return;

/*      DWORD dwPid;
      while(get_pid_by_title(pszName, dwPid))
      {
         HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, dwPid );
         TerminateProcess(hProcess, (UINT) -1);
         CloseHandle(hProcess);
         /*::EnumWindows((WNDENUMPROC)
         CKillProcessHelper::TerminateAppEnum,
         (LPARAM) dwId);
         // Wait on the handle. If it signals, great.

         //If it times out, then you kill it.

         if(WaitForSingleObject(hProcess, 5000)
         !=WAIT_OBJECT_0)
         bResult = TerminateProcess(hProcess,0);
         else
         bResult = TRUE;
         CloseHandle(hProcess);
         return bResult == TRUE;*/

    //  }
   }

   bool os::get_pid_by_path(const char * pszName, DWORD & dwPid)
   {
      uint_array dwa;
      get_all_processes(dwa);
      for(int32_t i = 0; i < dwa.get_count(); i++)
      {
         if(get_process_path(dwa[i]).CompareNoCase(pszName) == 0)
         {
            dwPid = dwa[i];
            return true;
         }
      }
      return false;
   }

   bool os::get_pid_by_title(const char * pszName, DWORD & dwPid)
   {
      uint_array dwa;
      get_all_processes(dwa);
      for(int32_t i = 0; i < dwa.get_count(); i++)
      {
         if(System.file().title_(get_process_path(dwa[i]))
            .CompareNoCase(pszName) == 0)
         {
            dwPid = dwa[i];
            return true;
         }
      }
      return false;
   }

   string os::get_process_path(DWORD dwPid)
   {
      /*
      string strName = ":<unknown>";
      // get a handle to the process.
      HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
         PROCESS_VM_READ,
         FALSE, dwPid );

      // get the process name.

      if (NULL != hProcess )
      {
         HMODULE hMod;
         DWORD cbNeeded;

         if(EnumProcessModules( hProcess, &hMod, sizeof(hMod),
            &cbNeeded) )
         {
            strName = get_module_path(hMod);
         }
      }

      CloseHandle( hProcess );
      return strName;*/
      ::exception::throw_not_implemented(get_app());
      return "";

   }

   void os::get_all_processes(uint_array & dwa )
   {

      ::exception::throw_not_implemented(get_app());
      return;

/*
      dwa.set_size(0);
      DWORD cbNeeded = 0;
      while(cbNeeded == natural(dwa.get_count()))
      {
         dwa.set_size(dwa.get_count() + 1024);
         if(!EnumProcesses(
            dwa.get_data(),
            (DWORD) (dwa.get_count() * sizeof(DWORD)),
            &cbNeeded))
         {
            return;
         }
         dwa.set_size(cbNeeded / sizeof(DWORD));
      }*/
   }

   string os::get_module_path(HMODULE hmodule)
   {
      ::exception::throw_not_implemented(get_app());
      return "";
      /*
      string strPath;
      DWORD dwSize = 1;
      while(natural(strPath.get_length() + 1) == dwSize)
      {
         dwSize = ::GetModuleFileName(
            hmodule,
            strPath.GetBufferSetLength(dwSize + 1024),
            (dwSize + 1024));
         strPath.ReleaseBuffer();
      }
      return strPath;*/
   }


   bool os::connection_settings_get_auto_detect()
   {
      //::exception::throw_not_implemented(get_app());
      return false;

/*
      registry::Key key1;

      key1.OpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections", false);

      primitive::memory mem;

      key1.QueryValue("DefaultConnectionSettings", mem);

      bool bAutoDetect = (((LPBYTE) mem.get_data())[8] & 0x08) != 0;

      return bAutoDetect;
*/
   }



   string os::connection_settings_get_auto_config_url()
   {

      //::exception::throw_not_implemented(get_app());
      return "";
/*
      registry::Key key;

      key.OpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", false);

      string strUrl;

      key.QueryValue("AutoConfigURL", strUrl);

      return strUrl;
*/
   }

   bool os::local_machine_set_run(const char * pszKey, const char * pszCommand)
   {

//      ::exception::throw_not_implemented(get_app());
      return false;

/*
      registry::Key keyKar(HKEY_LOcaL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);


      keyKar.SetValue(pszKey, pszCommand);


      return true;
*/
   }


   bool os::local_machine_set_run_once(const char * pszKey, const char * pszCommand)
   {


        ::exception::throw_not_implemented(get_app());
      return false;
/*    registry::Key keyKar(HKEY_LOcaL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce", true);


      keyKar.SetValue(pszKey, pszCommand);
*/

      return false;

   }

   bool os::current_user_set_run(const char * pszKey, const char * pszCommand)
   {

      ::exception::throw_not_implemented(get_app());
      return false;

/*
      registry::Key keyKar(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);


      keyKar.SetValue(pszKey, pszCommand);
*/

      return false;

   }

   bool os::current_user_set_run_once(const char * pszKey, const char * pszCommand)
   {

      ::exception::throw_not_implemented(get_app());
      return false;

/*
      registry::Key keyKar(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce", true);


      keyKar.SetValue(pszKey, pszCommand);

*/
      return false;

   }


   bool os::defer_register_ca2_plugin_for_mozilla()
   {
    //  ::exception::throw_not_implemented(get_app());
      return false;

/*
      registry::Key keyPlugins;

      if(keyPlugins.OpenKey(HKEY_LOcaL_MACHINE, "SOFTWARE\\MozillaPlugins", true))
      {

         registry::Key keyPlugin;

         if(keyPlugin.OpenKey(keyPlugins, "@ca2.cc/npca2", true))
         {

            keyPlugin.SetValue("Description", "ca2 plugin for NPAPI");
            keyPlugin.SetValue("Path", System.dir().ca2module("npca2.dll"));
            keyPlugin.SetValue("ProductName", "ca2 plugin for NPAPI");
            keyPlugin.SetValue("Vendor", "ca2 Desenvolvimento de Software Ltda.");
            keyPlugin.SetValue("Version", Application.file().as_string(System.dir().ca2("appdata/x86/ca2_build.txt")));

            registry::Key keyApplicationca2;

            if(keyApplicationca2.OpenKey(keyPlugin, "application/ca2", true))
            {

               keyApplicationca2.SetValue("Description", "ca2 Document");

            }

         }

      }

      return true;
*/
   }

   bool os::file_extension_get_open_with_list_keys(stringa & straKey, const char * pszExtension)
   {
      ::exception::throw_not_implemented(get_app());
      return false;

/*
      string strExt;

      strExt = ".";
      strExt += pszExtension;

      string strOpenWithKey;
      strOpenWithKey = strExt + "\\OpenWithList";

      registry::Key key;

      key.OpenKey(HKEY_CLASSES_ROOT, strOpenWithKey, false);

      key.EnumKey(straKey);

      return true;
*/
   }


   bool os::file_extension_get_open_with_list_commands(stringa & straCommand, const char * pszExtension)
   {

      stringa straKey;

      if(!file_extension_get_open_with_list_keys(straKey, pszExtension))
         return false;


      return true;

   }

   bool os::file_association_set_default_icon(const char * pszExtension, const char * pszExtensionNamingClass, const char * pszIconPath)
   {

      ::exception::throw_not_implemented(get_app());
      return false;

/*
      string strExtensionNamingClass(pszExtensionNamingClass);

      registry::Key keyLink3(HKEY_CLASSES_ROOT, strExtensionNamingClass, true);
      keyLink3.SetValue("DefaultIcon", pszIconPath);


      return false;
*/
   }


   bool os::file_association_set_shell_open_command(const char * pszExtension, const char * pszExtensionNamingClass,  const char * pszCommand, const char * pszParam)
   {
      ::exception::throw_not_implemented(get_app());
      return false;

/*
      string strExt;

      strExt = ".";
      strExt += pszExtension;

      string strExtensionNamingClass(pszExtensionNamingClass);

      registry::Key key(HKEY_CLASSES_ROOT, strExt, true);
      key.SetValue(NULL, strExtensionNamingClass);

      registry::Key keyLink3(HKEY_CLASSES_ROOT, strExtensionNamingClass + "\\shell", true);
      keyLink3.SetValue(NULL, "open");

      registry::Key keyLink2(keyLink3, "open", true);
      keyLink2.SetValue(NULL, "&Abrir");

      registry::Key keyLink1(keyLink2, "command", true);

      string strFormat;
      strFormat.Format("\"%s\" \"%%L\" %s", pszCommand, pszParam);
      keyLink1.SetValue(NULL, strFormat);

      return true;
*/
   }

   bool os::file_association_get_shell_open_command(const char * pszExtension, string & strExtensionNamingClass, string & strCommand, string & strParam)
   {
      ::exception::throw_not_implemented(get_app());
      return false;

/*
      string strExt;

      strExt = ".";
      strExt += pszExtension;

      registry::Key key(HKEY_CLASSES_ROOT, strExt, false);
      if(!key.QueryValue(NULL, strExtensionNamingClass))
         return false;

      registry::Key keyLink(HKEY_CLASSES_ROOT, strExtensionNamingClass + "\\shell\\open\\command", false);

      string strFormat;
      if(keyLink.QueryValue(NULL, strFormat))
      {

         const char * psz = strFormat;

         try
         {

            strCommand = ::str::consume_quoted_value(psz);
            ::str::consume_spaces(psz);
            ::str::consume(psz, "\"%L\"");
            strParam = psz;

         }
         catch(...)
         {
         }


      }

      return true;
*/
   }

   bool os::open_in_ie(const char * lpcsz)
   {

        ::exception::throw_not_implemented(get_app());
      return false;

/*    registry reg;
      string str;
      string str2;
      string strCommand;
      registry::Key key;
      if(key.OpenKey(HKEY_CLASSES_ROOT, ".html", false))
      {
         if(reg.RegQueryValue(key.m_hkey, "", str))
         {
            if(key.OpenKey(HKEY_CLASSES_ROOT, ".html\\shell\\opennew\\command", false))
            {
               string str;
               if(reg.RegQueryValue(HKEY_CLASSES_ROOT, str, str2))
               {
                  string strCommand(str2);
                  strCommand.replace("%1", lpcsz);
                  WinExec(strCommand,SW_SHOW);
               }
            }
            else
            {
               if(key.OpenKey(HKEY_CLASSES_ROOT, str, false))
               {
                  str += "\\shell\\opennew\\command";
                  if(key.OpenKey(HKEY_CLASSES_ROOT, str, false))
                  {
                     if(reg.RegQueryValue(key.m_hkey, "", str2))
                     {
                        string strCommand(str2);
                        strCommand.replace("%1", lpcsz);
                        WinExec(strCommand,SW_SHOW);
                     }
                  }
               }
            }
         }
      }

      return true;
*/

   }

   bool os::create_service(::aura::application * papp)
   {

      //::exception::throw_not_implemented(get_app());
      return false;


   }


   bool os::remove_service(::aura::application * papp)
   {
//      ::exception::throw_not_implemented(get_app());
      return false;

   }

   bool os::start_service(::aura::application * papp)
   {
      //::exception::throw_not_implemented(get_app());
      return false;

   }

   bool os::stop_service(::aura::application * papp)
   {
      ::exception::throw_not_implemented(get_app());
      return false;

   }

   bool os::resolve_link(string & strTarget, const char * pszSource, sp(::user::interaction) puiMessageParentOptional)
   {

      ::exception::throw_not_implemented(get_app());
      return false;

//      return vfxResolveShortcut(strTarget, pszSource, puiMessageParentOptional);

   }

   void os::raise_exception( DWORD dwExceptionCode, DWORD dwExceptionFlags)
   {

      ::exception::throw_not_implemented(get_app());
      return;
/*
      RaiseException( dwExceptionCode, dwExceptionFlags, 0, NULL );
      */
   }

   bool os::is_remote_session()
   {

//      ::exception::throw_not_implemented(get_app());
      return false;
/*
      return GetSystemMetrics(SM_REMOTESESSION) != FALSE;
*/
   }


   void os::post_to_all_threads(UINT message, WPARAM wparam, LPARAM lparam)
   {

      ::exception::throw_not_implemented(get_app());
      return;


   }


} // namespace android






