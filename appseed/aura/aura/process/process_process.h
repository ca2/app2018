﻿#pragma once


namespace process
{


   class CLASS_DECL_AURA process :
      virtual public ::object
   {
   public:


      bidi_pipe               m_pipe;
      bool                    m_bPiped;
      int                     m_iPid;
      exit_status             m_exitstatus;


      process(::aura::application * papp);
      virtual ~process();


      virtual bool create_child_process(const char * pszCmdLine,bool bPiped,const char * pszDir = NULL,int32_t iPriorityClass = ::multithreading::priority_none);

      virtual void wait_until_exit(int32_t iWaitMax = 0);
      virtual bool has_exited();

      virtual bool write(const char * psz);
      virtual string read(bool bUntilExit = false);

      virtual void synch_elevated(const char * pszCmdLine,int iShow,const ::duration & durationTimeOut,bool * pbTimeOut);

      virtual bool kill();

   };


   typedef sp(process) process_sp;


} // namespace process






















