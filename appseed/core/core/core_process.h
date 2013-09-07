#pragma once

#pragma once


#ifndef METROWIN


namespace core
{


   class CLASS_DECL_ca2 process
   {
   public:


      cross_pipe              m_pipe;
      bool                    m_bPiped;

#ifdef WINDOWSEX

      PROCESS_INFORMATION     m_pi;
      STARTUPINFOW             m_si;

#else

      int32_t                     m_iPid;

#endif


      process();
      virtual ~process();


      bool create_child_process(const char * pszCmdLine, bool bPiped, const char * pszDir = NULL, int32_t iPriorityClass = ::get_scheduling_priority_none());

      uint32_t wait_until_exit(int32_t iWaitMax = 0);
      bool has_exited(uint32_t * pdwExitCode = NULL);

      bool write(const char * psz);
      string read(bool bUntilExit = false);
   };

} // namespace core



#endif


