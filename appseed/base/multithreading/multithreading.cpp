#include "framework.h"


namespace multithreading
{


   CLASS_DECL_BASE comparable_array < HTHREAD > * s_phaThread = NULL;
   CLASS_DECL_BASE comparable_array < thread * > * s_pthreadptra = NULL;
   CLASS_DECL_BASE mutex * s_pmutex = NULL;


   CLASS_DECL_BASE void init_multithreading()
   {
      s_pmutex = new mutex(NULL);
      s_phaThread = new comparable_array < HTHREAD >;
      s_pthreadptra = new comparable_array < thread * >;
   }


   CLASS_DECL_BASE void term_multithreading()
   {

      delete s_pthreadptra;
      s_pthreadptra = NULL;

      delete s_phaThread;
      s_phaThread = NULL;

      delete s_pmutex;
      s_pmutex = NULL;

   }

   CLASS_DECL_BASE void __node_on_init_thread(HTHREAD hthread,thread * pthread)
   {

      synch_lock sl(s_pmutex);

      ::set_thread(pthread);

      __init_thread();



      s_phaThread->add(hthread);

      s_pthreadptra->add(pthread);

   }


   CLASS_DECL_BASE void __node_on_term_thread(HTHREAD hthread,thread * pthread,int nExitCode,bool bDelete)
   {

      synch_lock sl(s_pmutex);

      s_phaThread->remove(hthread);

      s_pthreadptra->remove(pthread);

      //__term_thread();

      __end_thread(pthread->m_pbaseapp,(UINT)nExitCode,bDelete);


   }



} // namespace multithreading




thread_pointer < ::thread > t_pthread;




::thread * get_thread()
{

   return t_pthread;

}



void set_thread(::thread* pthread)
{

   t_pthread = pthread;

}






thread* __begin_thread(sp(::base::application) papp,__THREADPROC pfnThreadProc,LPVOID pParam,int32_t epriority,UINT nStackSize,uint32_t dwCreateFlags,LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{

   ASSERT(pfnThreadProc != NULL);

   thread* pThread = new thread(papp,pfnThreadProc,pParam);
   ASSERT_VALID(pThread);

   if(!pThread->create_thread(epriority,dwCreateFlags,nStackSize,lpSecurityAttrs))
   {
      pThread->Delete();
      return NULL;
   }

   return pThread;

}





