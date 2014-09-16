#pragma once


namespace process
{


   class CLASS_DECL_AURA departament:
      public ::aura::departament
   {
   public:


      class process_thread: virtual public simple_thread
      {
      public:


         sp(process)                      m_spprocess;
         string *                         m_pstrRead;
         manual_reset_event *             m_pevReady;


         process_thread(sp(::aura::application) papp,string * pstrRead = NULL,manual_reset_event * pevReady = NULL);
         int32_t run();


      };


      class on_retry
      {
      public:


         uint32_t    m_dwTimeout;
         uint32_t    m_dwStartTime;
         bool        m_bPotentialTimeout;

         on_retry();

      };


      departament(sp(::aura::application) papp);
      virtual ~departament();



      // run process and get output
      virtual var get_output(const char * pszCmdLine);
      virtual uint32_t retry(const char * pszCmdLine,uint32_t dwTimeOut,int32_t iShow = SW_HIDE, bool * pbPotentialTimeout = NULL);
      virtual uint32_t synch(const char * pszCmdLine,int32_t iShow = SW_HIDE, const ::duration & dur = ::duration::infinite(), bool * pbPotentialTimeout = NULL);
      virtual bool launch(const char * pszCmdLine,int32_t iShow = SW_HIDE);


      static int32_t s_on_retry(int32_t iTry,uint_ptr dwParam);


   };


} // namespace process


