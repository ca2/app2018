#pragma once


namespace gen
{

   class CLASS_DECL_ca process
   {
   public:
      process();
      virtual ~process();

      cross_pipe m_pipe;
      bool m_bPiped;
      PROCESS_INFORMATION m_pi;
      STARTUPINFO m_si;

      bool create_child_process(const char * pszCmdLine, bool bPiped);

      DWORD wait_until_exit(int iWaitMax = 0);
      bool has_exited(DWORD * pdwExitCode = NULL);

      bool write(const char * psz);
      string read(bool bUntilExit = false);
   };

} // namespace gen