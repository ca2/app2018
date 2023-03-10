#pragma once


namespace process
{


   class CLASS_DECL_AURA department:
      public ::aura::department
   {
   public:


      class process_thread: virtual public simple_thread
      {
      public:

         string                           m_strCmdLine;
         sp(process)                      m_spprocess;
         string *                         m_pstrRead;
         manual_reset_event *             m_pevReady;
         uint32_t                         m_uiTimeout;
         uint32_t                         m_uiStartTime;
         bool *                           m_pbInitFailure;
         bool *                           m_pbPotentialTimeout;
         exit_status *                    m_pexitstatus;
         bool                             m_bElevated;


         process_thread(::aura::application * papp,const string & strCmdLine,const ::duration & dur,bool * pbPotentialTimeout = NULL,string * pstrRead = NULL,bool bElevated = false);

         void run();

         void run_normal();

         void run_elevated();

         bool retry();

      };


      class process_processor :
         virtual public object
      {
      public:


         manual_reset_event               m_evReady;
         bool                             m_bInitFailure;
         bool                             m_bPotentialTimeout;
         bool *                           m_pbPotentialTimeout;
         process_thread *                 m_pthread;
         exit_status                      m_exitstatus;
         bool                             m_bElevated;


         process_processor(::aura::application * papp, const string & strCmdLine, const ::duration & dur, bool * pbPotentialTimeout = NULL, string * pstrRead = NULL, bool bElevated = false);
         virtual ~process_processor();

      };


      department(::aura::application * papp);
      virtual ~department();


      // run process and get output
      virtual var get_output(const char * pszCmdLine,const ::duration & dur= ::duration::infinite(),int32_t iShow = SW_HIDE, bool * pbPotentialTimeout = NULL);
      virtual exit_status retry(const char * pszCmdLine,const ::duration & dur,int32_t iShow = SW_HIDE, bool * pbPotentialTimeout = NULL);
      virtual exit_status synch(const char * pszCmdLine,int32_t iShow = SW_HIDE, const ::duration & dur = ::duration::infinite(), bool * pbPotentialTimeout = NULL);
      virtual bool launch(const char * pszCmdLine,int32_t iShow = SW_HIDE, const char * pszDir = NULL);
      virtual exit_status elevated_synch(const char * pszCmdLine,int32_t iShow = SW_HIDE,const ::duration & dur = ::duration::infinite(),bool * pbPotentialTimeout = NULL);



   };


} // namespace process


