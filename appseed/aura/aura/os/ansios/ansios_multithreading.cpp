#include "framework.h"
//#include <sched.h>
//#include <time.h>
//#include <pthread.h>

CLASS_DECL_AURA void thread_get_os_priority(int32_t * piOsPolicy, sched_param * pparam, int32_t iCa2Priority);

CLASS_DECL_AURA void process_get_os_priority(int32_t * piOsPolicy, sched_param * pparam, int32_t iCa2Priority);

CLASS_DECL_AURA int32_t thread_get_scheduling_priority(int iOsPolicy, const sched_param * pparam);

CLASS_DECL_AURA int32_t process_get_scheduling_priority(int iOsPolicy, const sched_param * pparam);

//mq * __get_mq(HTHREAD  h);
//
//
//mq * __get_mq()
//{
//
//   synch_lock sl(g_pmutexMq);
//
//   auto pmq = (mq *) thread_get_data(TLS_MESSAGE_QUEUE);
//
//   if(pmq != NULL)
//      return pmq;
//
//   pmq   = new mq();
//
//   pmq->m_hthread    = (IDTHREAD) pthread_self();
//
//   thread_set_data(TLS_MESSAGE_QUEUE,pmq);
//
//   return pmq;
//
//}

//
//LPVOID WINAPI thread_get_data(IDTHREAD hthread,DWORD dwIndex);
//
//
//int_bool WINAPI thread_set_data(HTHREAD hthread,DWORD dwIndex,LPVOID lpTlsValue);
//

//void __clear_mq()
//{
//
//   synch_lock sl(g_pmutexMq);
//
//   HTHREAD hthread = GetCurrentThread();
//
//   auto pmq = (mq *) thread_get_data(hthread, TLS_MESSAGE_QUEUE);
//
//   if(pmq == NULL)
//      return;
//
//   ::aura::del(pmq);
//
//   thread_set_data(hthread, TLS_MESSAGE_QUEUE, NULL);
//
//}
//

DWORD MsgWaitForMultipleObjectsEx(DWORD dwSize, sync_object * * pobjectptra, DWORD dwTimeout, DWORD dwWakeMask, DWORD dwFlags)
{

   DWORD start = 0;

   if(dwTimeout != (DWORD) INFINITE)
   {
      start = ::get_tick_count();
   }

   mq * pmq = NULL;

   if(dwWakeMask > 0)
   {

      pmq = __get_mq((IDTHREAD)pthread_self(), false);

      //if(pmq == NULL)
      // return 0;

   }



   int_bool bWaitForAll        = dwFlags & MWMO_WAITALL;
//   int_bool bAlertable         = dwFlags & MWMO_ALERTABLE;
//   int_bool bInputAvailable    =  dwFlags & MWMO_INPUTAVAILABLE;

   timespec delay;

   delay.tv_sec = 0;
   delay.tv_nsec = 1000000;

   if(bWaitForAll)
   {

      while(true)
      {
         int32_t i;
         int32_t j;
         i = 0;
         for(; comparison::lt(i, dwSize);)
         {

            if(pmq != NULL)
            {

               synch_lock sl(&pmq->m_mutex);

               if(pmq->ma.get_count() > 0)
               {

                  return WAIT_OBJECT_0 + dwSize;

               }

            }

            if(dwTimeout != (DWORD) INFINITE && ::get_tick_count() - start >= dwTimeout)
            {

               for(j = 0; j < i; j++)
               {

                  pobjectptra[j]->unlock();

               }

               return WAIT_TIMEOUT;

            }

            if(pobjectptra[i]->lock(millis(1)))
            {

               i++;

            }
            else
            {

               nanosleep(&delay, NULL);

            }

         }
         //      for(j = 0; j < dwSize; j++)
         //    {
         //     pobjectptra[j]->unlock();
         //}

         return WAIT_OBJECT_0;

      }

   }
   else
   {

      int32_t i;
//      int32_t j;
      while(true)
      {

         for(i = 0; comparison::lt(i, dwSize); i++)
         {

            if(pmq != NULL)
            {

               synch_lock sl(&pmq->m_mutex);

               if(pmq->ma.get_count() > 0)
               {

                  return WAIT_OBJECT_0 + dwSize;

               }

            }

            if(dwTimeout != (DWORD) INFINITE && ::get_tick_count() - start >= dwTimeout)
            {

               return WAIT_TIMEOUT;

            }

            if(pobjectptra[i]->lock(millis(0)))
            {

               return WAIT_OBJECT_0 + i;

            }

         }

         nanosleep(&delay, NULL);

      }

   }

}


DWORD MsgWaitForMultipleObjects(DWORD dwSize, sync_object ** pobjectptra, WINBOOL bWaitForAll, DWORD dwTimeout, DWORD dwWakeMask)
{

   return MsgWaitForMultipleObjectsEx(dwSize, pobjectptra, dwTimeout, dwWakeMask, (bWaitForAll ?  MWMO_WAITALL : 0));

}


DWORD WaitForMultipleObjectsEx(DWORD dwSize, sync_object ** pobjectptra, WINBOOL bWaitForAll, DWORD dwTimeout, WINBOOL bAlertable)
{

   return MsgWaitForMultipleObjectsEx(dwSize, pobjectptra, dwTimeout, 0, (bWaitForAll ?  MWMO_WAITALL : 0) | (bAlertable ?  MWMO_ALERTABLE : 0));

}


DWORD WaitForMultipleObjects(DWORD dwSize, sync_object ** pobjectptra, WINBOOL bWaitForAll, DWORD dwTimeout)
{

   return WaitForMultipleObjectsEx(dwSize, pobjectptra, bWaitForAll, dwTimeout, FALSE);

}


DWORD WaitForSingleObjectEx(sync_object * pobject, DWORD dwTimeout, WINBOOL bAlertable)
{

   return WaitForMultipleObjectsEx(1, &pobject, TRUE, dwTimeout, bAlertable);

}


DWORD WaitForSingleObject(sync_object * pobject, DWORD dwTimeout)
{

   return WaitForSingleObjectEx(pobject, dwTimeout, FALSE);

}




thread_data::thread_data()
{

   pthread_key_create(&m_key, NULL);

}


thread_data::~thread_data()
{

   //pthread_key_destroy(&m_key);

}

void * thread_data::get()
{

   return pthread_getspecific(m_key);

}


void thread_data::set(void * p)
{

   pthread_setspecific(m_key,p);

}



CLASS_DECL_AURA HTHREAD get_current_thread()
{

   return ::GetCurrentThread();

}


CLASS_DECL_AURA IDTHREAD get_current_thread_id()
{

   return ::GetCurrentThreadId();

}









void __node_init_multithreading()
{

   //s_pmapHthreadHthread = new map < HTHREAD,HTHREAD,HTHREAD,HTHREAD >();

   //s_pmapDwordHthread = new map < DWORD,DWORD,HTHREAD,HTHREAD >();

   //s_pmapHthreadDword = new map < HTHREAD,HTHREAD,DWORD,DWORD >();

   __node_init_cross_windows_threading();


}



void __node_term_multithreading()
{

   __node_term_cross_windows_threading();

   //delete s_pmapHthreadDword;

   //s_pmapHthreadDword = NULL;

   //delete s_pmapDwordHthread;

   //s_pmapDwordHthread = NULL;

   //delete s_pmapHthreadHthread;

   //s_pmapHthreadHthread = NULL;

}










#if defined(LINUX) // || defined(ANDROID)

bool (* g_pfn_defer_process_x_message)(HTHREAD hthread,LPMESSAGE lpMsg,oswindow oswindow,bool bPeek) = NULL;

bool aura_defer_process_x_message(HTHREAD hthread,LPMESSAGE lpMsg,oswindow oswindow,bool bPeek)
{

   if(g_pfn_defer_process_x_message == NULL)
      return false;

   return (*g_pfn_defer_process_x_message)(hthread, lpMsg, oswindow, bPeek);

}

void set_defer_process_x_message(bool (* pfn)(HTHREAD hthread,LPMESSAGE lpMsg,oswindow oswindow,bool bPeek))
{

   g_pfn_defer_process_x_message = pfn;

}

UINT (* g_x11_thread)(void * pparam) = NULL;

#ifdef LINUX
UINT __x11_thread(void * pparam)
{

   (*g_x11_thread)(pparam);

}
#endif

#endif



extern "C"
void * os_thread_thread_proc(LPVOID lpparameter);







//map < HTHREAD,HTHREAD,PendingThreadInfo,PendingThreadInfo > * g_ppendingThreads = NULL;
//
//mutex * g_pmutexPendingThreadsLock = NULL;

//mutex * g_pmutexThreadHandleLock = NULL;
//
//mutex * g_pmutexThreadIdHandleLock = NULL;

//mutex * g_pmutexThreadIdLock = NULL;

//map < HTHREAD,HTHREAD,PendingThreadInfo,PendingThreadInfo > & pendingThreads()
//{
//
//   return * g_ppendingThreads;
//
//}
//


//map < HTHREAD,HTHREAD,HTHREAD,HTHREAD > & thread_handle_map()
//{
//
//   return *s_pmapHthreadHthread;
//
//}

//map < DWORD,DWORD,HTHREAD,HTHREAD > & thread_id_handle_map()
//{
//
//   return *s_pmapDwordHthread;
//
//}


//map < HTHREAD,HTHREAD,DWORD,DWORD > & thread_id_map()
//{
//
//   return *s_pmapHthreadDword;
//
//}

//DWORD DwThreadId()
//{
//   static DWORD g_dw_thread_id = 0;
//
//   if(g_dw_thread_id <= 0)
//      g_dw_thread_id = 1;
//
//   return g_dw_thread_id++;
//}
//




//mutex * hthread::s_pmutex = NULL;
//ref_array <  hthread > * hthread::s_pptra = NULL;


// Converts a Win32 thread priority to WinRT format.
int32_t GetWorkItemPriority(int32_t nPriority)
{
   if(nPriority < 0)
      return nPriority; // WorkItemPriority::Low;
   else if(nPriority > 0)
      return nPriority; // WorkItemPriority::High;
   else
      return nPriority; // WorkItemPriority::Normal;
}


// Helper shared between CreateThread and ResumeThread.
//HTHREAD StartThread(LPTHREAD_START_ROUTINE pfn,LPVOID pv,HTHREAD hthread,int32_t nPriority,SIZE_T cbStack)
//{
//
//
//   //pthread->m_hthread = hthread;
//
//   //hthread->m_posthread = pthread;
//
//   pthread_t & thread = *(pthread_t*) pthread->m_pthread;
//
//   pthread_attr_t threadAttr;
//
//   pthread_attr_init(&threadAttr);
//
//   if(cbStack > 0)
//   {
//
//      pthread_attr_setstacksize(&threadAttr,120 * 1024); // Set the stack size of the thread
//
//   }
//
//   int iPolicy;
//
//   sched_param schedparam; // scheduling priority
//
//   thread_get_os_priority(&iPolicy,&schedparam,nPriority);
//
//   pthread_attr_setschedpolicy(&threadAttr,iPolicy);
//
//   pthread_attr_setschedparam(&threadAttr,&schedparam);
//
//   pthread_attr_setdetachstate(&threadAttr,PTHREAD_CREATE_DETACHED); // Set thread to detached state. No need for pthread_join
//
//   pthread_create(&thread,&threadAttr,&hthread::thread_proc,(LPVOID)pthread); // Create the thread
//
//   return pthread;
//
//}



//HTHREAD WINAPI CreateThread(LPSECURITY_ATTRIBUTES unusedThreadAttributes,uint_ptr cbStack,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,uint32_t dwCreationFlags,uint32_t * lpdwThreadId)
//{
//   // Validate parameters.
//   //   assert(unusedThreadAttributes == nullptr);
//   //   assert(unusedStackSize == 0);
//   //assert((dwCreationFlags & ~CREATE_SUSPENDED) == 0);
//   //assert(unusedThreadId == nullptr);
//
//   // Create a handle that will be signalled when the thread has completed
//   HTHREAD threadHandle = new hthread();
//
//   if(threadHandle == NULL)
//      return NULL;
//
//
//   synch_lock mlThreadId(g_pmutexThreadIdLock);
//
//   thread_id_map().set_at(threadHandle,DwThreadId());
//
//   if(lpdwThreadId != NULL)
//   {
//      *lpdwThreadId = thread_id_map()[threadHandle];
//   }
//
//
//
//   synch_lock mlThreadIdHandle(g_pmutexThreadIdHandleLock);
//
//   thread_id_handle_map().set_at(thread_id_map()[threadHandle],threadHandle);
//
//   mlThreadIdHandle.unlock();
//
//   mlThreadId.unlock();
//
//   // Make a copy of the handle for internal use. This is necessary because
//   // the caller is responsible for closing the handle returned by CreateThread,
//   // and they may do that before or after the thread has finished running.
//   /*HANDLE completionEvent;
//
//   if (!DuplicateHandle(GetCurrentProcess(), threadHandle, GetCurrentProcess(), &completionEvent, 0, false, DUPLICATE_SAME_ACCESS))
//   {
//   CloseHandle(threadHandle);
//   return nullptr;
//   }*/
//
//   //   synch_lock mlThreadHandle(threadHandleLock);
//
//   // thread_handle_map().set_at(completionEvent, threadHandle);
//
//   //mlThreadHandle.unlock();
//
//   PendingThreadInfo info;
//
//   ZERO(info);
//
//   try
//   {
//      if(dwCreationFlags & CREATE_SUSPENDED)
//      {
//         // Store info about a suspended thread.
//
//         info.lpStartAddress     = lpStartAddress;
//         info.lpParameter        = lpParameter;
//         info.m_hthread    = threadHandle;
//         info.suspensionEvent    = new event(get_thread_app(),false,true);
//         info.nPriority = 0;
//
//         synch_lock lock(g_pmutexPendingThreadsLock);
//
//         pendingThreads()[threadHandle] = info;
//
//         //::WaitForSingleObjectEx(info.suspensionEvent, INFINITE, FALSE);
//      }
//      else
//      {
//         // Start the thread immediately.
//         StartThread(lpStartAddress,lpParameter,threadHandle,0,cbStack);
//      }
//
//      return threadHandle;
//   }
//   catch(...)
//   {
//      // Clean up if thread creation fails.
//      threadHandle->m_pevent->set_event();
//      delete threadHandle;
//
//      if(info.suspensionEvent)
//      {
//         info.suspensionEvent->set_event();
//         delete info.suspensionEvent;
//      }
//
//
//      return NULL;
//   }
//}


//DWORD WINAPI ResumeThread(HTHREAD hThread)
//{
//   synch_lock lock(g_pmutexPendingThreadsLock);
//
//   // Look up the requested thread.
//   map < HTHREAD,HTHREAD,PendingThreadInfo,PendingThreadInfo >::pair * threadInfo = pendingThreads().PLookup(hThread);
//
//   if(threadInfo == NULL)
//   {
//      // Can only resume threads while they are in CREATE_SUSPENDED state.
//      //assert(false);
//      return (DWORD)-1;
//   }
//
//   // Start the thread.
//   try
//   {
//      PendingThreadInfo& info = threadInfo->m_element2;
//
//      StartThread(info.lpStartAddress,info.lpParameter,info.m_hthread,info.nPriority,info.cbStack);
//   }
//   catch(...)
//   {
//      return (DWORD)-1;
//   }
//
//   // Remove this thread from the pending list.
//   pendingThreads().remove_key(hThread);
//
//   return 0;
//}


int_bool WINAPI SetThreadPriority(HTHREAD hThread,int32_t nCa2Priority)
{

   //synch_lock lock(g_pmutexPendingThreadsLock);

   //// Look up the requested thread.
   //map < HTHREAD,HTHREAD,PendingThreadInfo,PendingThreadInfo >::pair * threadInfo = pendingThreads().PLookup(hThread);

   //if(threadInfo == NULL)
   //{

   int32_t iPolicy;

   sched_param schedparam;

   thread_get_os_priority(&iPolicy,&schedparam,nCa2Priority);

   pthread_setschedparam((IDTHREAD) hThread,iPolicy,&schedparam);

   return TRUE;

   //}

   //// Store the new priority.
   //threadInfo->m_element2.nPriority = nCa2Priority;

   return TRUE;
}







int32_t WINAPI GetThreadPriority(HTHREAD  hthread)
{

   //synch_lock lock(g_pmutexPendingThreadsLock);

   //// Look up the requested thread.
   //map < HTHREAD,HTHREAD,PendingThreadInfo,PendingThreadInfo >::pair * threadInfo = pendingThreads().PLookup(hthread);

   //if(threadInfo == NULL)
   //{

   int iOsPolicy = SCHED_OTHER;

   sched_param schedparam;

   schedparam.sched_priority = 0;

   pthread_getschedparam((IDTHREAD) hthread,&iOsPolicy,&schedparam);

   return thread_get_scheduling_priority(iOsPolicy,&schedparam);

//   return threadInfo->m_element2.nPriority;

}



//void hthread::wait()
//{
//
//   m_pevent->wait();
//
//}
//
//
//
//
//hthread::~hthread()
//{
//   synch_lock ml(&*s_pmutex);
//
//   for(index i = s_pptra->get_count() - 1; i >= 0; i--)
//   {
//
//      if(s_pptra->element_at(i) == this)
//      {
//
//         s_pptra->remove_at(i);
//
//      }
//
//   }
////htread
//{
//
//delete m_pevent;
//}
//
//}
//
//
//void hthread::begin()
//{
//
//   m_hthread = create_thread(NULL, 0, &::thread_layer::proc, this, 0, &m_nId);
//
//}
//
//
//os_thread * os_thread::get()
//{
//
//   return t_posthread;
//
//}
//
//void os_thread::set(os_thread * posthread)
//{
//
//   t_posthread = posthread;
//
//}
//
//
//void os_thread::stop_all(uint32_t millisMaxWait)
//{
//
//   millisMaxWait = millisMaxWait;
//
//   uint32_t start = get_tick_count();
//
//   while(get_tick_count() - start < millisMaxWait)
//   {
//
//      {
//
//         synch_lock ml(&*s_pmutex);
//
//         for(int i = 0; i < s_pptra->get_count(); i++)
//         {
//
//            s_pptra->element_at(i)->m_bRun = false;
//
//         }
//
//         if(s_pptra->get_count() <= 0)
//         {
//
//            break;
//
//         }
//
//      }
//
//      Sleep(200);
//
//   }
//
//
//}
//
//extern "C"
//void * os_thread_thread_proc(LPVOID lpparameter)
//{
//
//   keep_threading_count keepthreadingcount;
//
//   os_thread * posthread = (os_thread *)lpparameter;
//
//   t_posthread = posthread;
//
//   if(!on_init_thread())
//   {
//
//      return (void *) (int_ptr) 34;
//
//   }
//
//   void * pvRet = (void *)(int_ptr)posthread->run();
//
//   t_posthread = NULL;
//
//   on_term_thread();
//
//   delete posthread;
//
//   return pvRet;
//
//}
//
//
//HTHREAD start_thread(DWORD(WINAPI * pfn)(LPVOID),LPVOID pv,int32_t iPriority)
//{
//
//   UNREFERENCED_PARAMETER(iPriority);
//
//   return create_thread(NULL,0,pfn,pv,0,NULL);
//
//}
//
//HTHREAD create_thread(LPSECURITY_ATTRIBUTES lpsa,uint_ptr cbStack,LPTHREAD_START_ROUTINE pfn,LPVOID pv,uint32_t f,uint32_t * lpdwId)
//{
//
//   return ::CreateThread(lpsa,cbStack,pfn,pv,f,lpdwId);
//
//}
//






static HANDLE g_hMainThread = NULL;
static UINT g_uiMainThread = -1;

CLASS_DECL_AURA void set_main_thread(HANDLE hThread)
{

   // MESSAGE msg;

   // PeekMessage function used to create message queue Windows Desktop
   // PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);

   g_hMainThread = hThread;

}

CLASS_DECL_AURA void set_main_thread_id(UINT uiThread)
{

   //   MESSAGE msg;

   // PeekMessage function used to create message queue Windows Desktop
   // PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);

   g_uiMainThread = uiThread;

}


CLASS_DECL_AURA HANDLE get_main_thread()
{
   return g_hMainThread;

}
CLASS_DECL_AURA UINT   get_main_thread_id()
{
   return g_uiMainThread;
}


CLASS_DECL_AURA void attach_thread_input_to_main_thread(bool bAttach)
{

}



LPVOID WINAPI thread_get_data(HTHREAD hthread,DWORD dwIndex);


int_bool WINAPI thread_set_data(HTHREAD hthread,DWORD dwIndex,LPVOID lpTlsValue);



DWORD g_dwDebug_post_thread_msg_time;
int g_iDebug_post_thread_msg_time;

CLASS_DECL_AURA int_bool WINAPI PostThreadMessageW(IDTHREAD iThreadId,UINT Msg,WPARAM wParam,LPARAM lParam)
{

   //HTHREAD h = ::get_thread_handle(idThread);

   //if(h == NULL)
   //   return FALSE;


   mq * pmq = __get_mq((IDTHREAD) iThreadId, Msg != WM_QUIT);

   if(pmq == NULL)
      return FALSE;

   synch_lock ml(&pmq->m_mutex);

   MESSAGE msg;

   //zero(&msg, sizeof(msg));

   if(Msg == WM_QUIT)
   {
      ::output_debug_string("\n\n\nWM_QUIT posted to thread " + ::str::from((uint64_t)iThreadId) + "\n\n\n");
   }


   DWORD dwNow = get_tick_count();

   if(dwNow - g_dwDebug_post_thread_msg_time < 1)
   {

      if(g_iDebug_post_thread_msg_time > 10)
      {

//         writeln("PostThreadMessage" flooded?");

      }
      else
      {

         g_iDebug_post_thread_msg_time++;

      }

   }
   else
   {

      g_iDebug_post_thread_msg_time = 0;

   }

   g_dwDebug_post_thread_msg_time = dwNow;



   msg.message = Msg;
   msg.wParam  = wParam;
   msg.lParam  = lParam;
   msg.pt.x    = 0x80000000;
   msg.pt.y    = 0x80000000;
   msg.hwnd    = NULL;

   pmq->ma.add(msg);

   //   void * p = pmq->ma[pmq->ma.get_count() -1].hwnd;

   pmq->m_eventNewMessage.set_event();

   return true;

}








//void thread_layer::wait_thread(DWORD dwMillis)
//{
//
//   try
//   {
//
//      m_hthread->m_pevent->wait(millis(dwMillis));
//
//   }
//   catch(...)
//   {
//
//   }
//
//}
//
//
//
//
//
//thread_layer::~thread_layer()
//{
//
//   delete m_hthread;
//
//}
//
//


CLASS_DECL_AURA HTHREAD GetCurrentThread()
{

   return (HTHREAD) pthread_self();

}

CLASS_DECL_AURA IDTHREAD GetCurrentThreadId()
{

   return pthread_self();

}


namespace multithreading
{

   CLASS_DECL_AURA bool set_priority(int32_t priority)
   {

      return (::SetThreadPriority(::GetCurrentThread(),priority) != 0);
   }


   CLASS_DECL_AURA int32_t thread_priority()
   {
      return ::GetThreadPriority(::GetCurrentThread());
   }


} // namespace aura







bool on_init_thread()
{

   //attach_thread_input_to_main_thread();

   if(!__os_init_thread())
      return false;

   return true;

}


bool on_term_thread()
{

   bool bOk1 = __os_term_thread();

   return bOk1;

}







//CLASS_DECL_AURA IDTHREAD get_current_thread_id()
//{
//   return ::GetCurrentThreadId();
//}




CLASS_DECL_AURA DWORD_PTR translate_processor_affinity(int iOrder)
{

   return 1 << iOrder;

}
