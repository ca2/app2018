#include "framework.h"


namespace windows
{


   process::process(::aura::application * papp):
      object(papp),
      ::process::process(papp)
   {

      memset(&m_pi,0,sizeof(PROCESS_INFORMATION));

      memset(&m_si,0,sizeof(STARTUPINFO));

   }


   process::~process()
   {

      if(m_pi.hProcess != INVALID_HANDLE_VALUE && m_pi.hProcess != NULL)
      {

         CloseHandle(m_pi.hProcess);

      }

      if(m_pi.hThread != INVALID_HANDLE_VALUE && m_pi.hThread != NULL)
      {

         CloseHandle(m_pi.hThread);

      }

   }


   bool process::create_child_process(const char * pszCmdLine,bool bPiped,const char * pszDir,int32_t iCa2Priority)
   {


      if(!::process::process::create_child_process(pszCmdLine,bPiped,pszDir,iCa2Priority))
         return false;

      string szCmdline = pszCmdLine;

      bool bSuccess = false;

      m_si.cb = sizeof(STARTUPINFO);

      if(bPiped)
      {

         pipe * ppipeOut      = m_pipe.m_sppipeOut.cast < pipe >();

         m_si.hStdError       = ppipeOut->m_hWrite;

         m_si.hStdOutput      = ppipeOut->m_hWrite;

         pipe * ppipeIn       = m_pipe.m_sppipeIn.cast < pipe >();

         m_si.hStdInput       = ppipeIn->m_hRead;

         m_si.dwFlags         |= STARTF_USESTDHANDLES;

      }

      /* STARTUPINFO si;
      PROCESS_INFORMATION pi;
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_HIDE; */
      //         if(!::CreateProcess(NULL, (LPTSTR) (const char *) System.dir().appdata("production\\build.bat"), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
      m_si.dwFlags |= STARTF_USESHOWWINDOW;
      m_si.wShowWindow = SW_HIDE;


      DWORD dwPriorityClass = ::get_os_priority_class(iCa2Priority);

      wstring wstrDir(pszDir);

      const unichar * pwszDir;

      if (pszDir == NULL || *pszDir == '\0')
      {

         pwszDir = NULL;

      }
      else
      {

         pwszDir = wstrDir;

      }

      // create the child process.

      wstring wstrCommandLine;

      if(::str::ends_ci(szCmdline,".bat"))
      {

         string strCmd;

         strCmd = "";
         strCmd += szCmdline;
         strCmd += "";

         wstrCommandLine = strCmd;

      }
      else
      {

         wstrCommandLine = szCmdline;

      }

      unichar * pwszCommandLine = (unichar * ) (const unichar *) wstrCommandLine;
      DWORD dwFlags = 0;
      if (oprop("inherit").is_new() || (bool) oprop("inherit"))
      {
         dwFlags = CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT;
      }

      bSuccess = CreateProcessW(NULL,
                                pwszCommandLine,
                                NULL,          // process security attributes
                                NULL,          // primary thread security attributes
                                TRUE,          // handles are inherited
                                dwFlags | dwPriorityClass,             // creation flags
                                NULL,          // use parent's environment
                                pwszDir,
                                &m_si,  // STARTUPINFO pointer
                                &m_pi) != FALSE;  // receives PROCESS_INFORMATION

      // If an error occurs, exit the application.
      if (!bSuccess)
      {
         DWORD dwLastError = ::get_last_error();
         string strMessage = get_system_error_message(dwLastError);
         output_debug_string(pwszCommandLine);
         output_debug_string("\r\n");
         output_debug_string("CreateProcessW Error!!");
         output_debug_string("\r\n");
         output_debug_string(strMessage + " (" + ::str::from((uint32_t) dwLastError) + ")");
         output_debug_string("\r\n");
         return false;
      }
      else
      {
         m_iPid = m_pi.dwProcessId;
         // close handles to the child process and its primary thread.
         // Some applications might keep these handles to monitor the status
         // of the child process, for example.

         //CloseHandle(m_pi.hProcess);
         //CloseHandle(m_pi.hThread);
      }

      return true;

   }


   bool process::has_exited()
   {

      DWORD dwExitCode;

      bool bExited;

      if(!GetExitCodeProcess(m_pi.hProcess,&dwExitCode))
      {

         m_exitstatus.m_iExitCode = -1;

         bExited = true;

      }
      else
      {

         if(dwExitCode == STILL_ACTIVE)
         {

            bExited = false;

         }
         else
         {

            bExited = true;

         }

      }

      m_exitstatus.m_iExitCode = dwExitCode;

      return bExited;

   }


   bool process::synch_elevated(const char * pszCmdLine,int iShow,const ::duration & durationTimeOut,bool * pbTimeOut)
   {

      DWORD dwExitCode = 0;

      HANDLE h = NULL;

      ::str::parse parse(pszCmdLine,strlen(pszCmdLine), "= ");

      parse.EnableQuote(true);

      string strPath;

      if(parse.getrestlen())
      {

         parse.getword(strPath);

      }

      string strParam;

      parse.getrest(strParam);

      if(VistaTools::RunElevated(NULL,strPath,strParam,NULL,&h))
      {

         dwExitCode = 0;

      }
      else
      {

         return false;

      }

      bool bTimedOut = true;

      DWORD dwStart = ::get_tick_count();

      DWORD dwTimeOut = durationTimeOut.tick_duration();

      while(::get_tick_count() - dwStart < dwTimeOut)
      {

         if (!::GetExitCodeProcess(h, &dwExitCode))
         {

            break;

         }

         if(dwExitCode != STILL_ACTIVE)
         {

            bTimedOut = false;

            break;

         }

         Sleep(100);

      }

      ::CloseHandle(h);

      if(pbTimeOut != NULL)
      {

         *pbTimeOut = bTimedOut;

      }

      return !bTimedOut;

   }


   bool process::kill()
   {
      ::system(string("taskkill /F /T /PID " ) + ::str::from((int32_t) m_pi.dwProcessId));
      return true;
      //return TerminateProcess(m_pi.hThread, -1) != FALSE;

   }




} // namespace windows











