#pragma once


namespace process
{


   class CLASS_DECL_ACE process :
      virtual public ::object
   {
   public:


      bidi_pipe               m_pipe;
      bool                    m_bPiped;
      int                     m_iPid;


      process(::ace::application * papp);
      virtual ~process();


      virtual bool create_child_process(const char * pszCmdLine,bool bPiped,const char * pszDir = NULL,int32_t iPriorityClass = ::multithreading::priority_none);

      virtual uint32_t wait_until_exit(int32_t iWaitMax = 0);
      virtual bool has_exited(uint32_t * pdwExitCode = NULL);

      virtual bool write(const char * psz);
      virtual string read(bool bUntilExit = false);

      virtual int32_t synch_elevated(const char * pszCmdLine,int iShow,const ::duration & durationTimeOut,bool * pbTimeOut);

      virtual bool kill();

   };


   typedef sp(process) process_sp;


} // namespace process






















