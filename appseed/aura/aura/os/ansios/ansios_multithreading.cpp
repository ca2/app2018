//#include "framework.h"
//#include <sched.h>
//#include <time.h>
//#include <pthread.h>

CLASS_DECL_AURA void thread_get_os_priority(int32_t * piOsPolicy, sched_param * pparam, int32_t iCa2Priority);

CLASS_DECL_AURA void process_get_os_priority(int32_t * piOsPolicy, sched_param * pparam, int32_t iCa2Priority);

CLASS_DECL_AURA int32_t thread_get_scheduling_priority(int iOsPolicy, const sched_param * pparam);

CLASS_DECL_AURA int32_t process_get_scheduling_priority(int iOsPolicy, const sched_param * pparam);

mq * get_mq(HTHREAD  h);


mq * get_mq()
{

   return get_mq(GetCurrentThread());

}


DWORD MsgWaitForMultipleObjectsEx(DWORD dwSize, object * * pobjectptra, DWORD dwTimeout, DWORD dwWakeMask, DWORD dwFlags)
{

   DWORD start = 0;

   if(dwTimeout != (DWORD) INFINITE)
   {
      start = ::get_tick_count();
   }

   mq * pmq = NULL;

   if(dwWakeMask > 0)
   {

      pmq = get_mq();

      if(pmq == NULL)
         return 0;

   }

   single_lock ml(pmq == NULL ? NULL : &pmq->m_mutex,false);

   int_bool bWaitForAll        = dwFlags & MWMO_WAITALL;
//   int_bool bAlertable         = dwFlags & MWMO_ALERTABLE;
//   int_bool bInputAvailable    =  dwFlags & MWMO_INPUTAVAILABLE;

   timespec delay;

   delay.tv_sec = 0;
   delay.tv_nsec = 1000000;

   if(bWaitForAll)
   {

      int32_t i;
      int32_t j;
      i = 0;
      for(; i < dwSize;)
      {
         if(dwTimeout != (DWORD) INFINITE && ::get_tick_count() - start >= dwTimeout)
         {
            return WAIT_TIMEOUT;
         }
         if(pobjectptra[i]->is_locked())
         {
            for(j = 0; j < i; j++)
            {
               pobjectptra[j]->unlock();
            }
            nanosleep(&delay, NULL);
            i = 0;
         }
         else
         {
            pobjectptra[i]->lock();
            i++;
         }
      }
//      for(j = 0; j < dwSize; j++)
  //    {
    //     pobjectptra[j]->unlock();
      //}

      return WAIT_OBJECT_0;

   }
   else
   {

      int32_t i;
//      int32_t j;
      while(true)
      {

         for(i = 0; i < dwSize;i++)
         {
            if(dwTimeout != (DWORD) INFINITE && ::get_tick_count() - start >= dwTimeout)
            {
               return WAIT_TIMEOUT;
            }
            if(pobjectptra[i]->lock(millis(0)))
            {
               return WAIT_OBJECT_0 + i;
            }
            if(ml.lock(millis(0)))
            {
               if(pmq->ma.get_count() > 0)
               {
                  return WAIT_OBJECT_0 + dwSize;
               }
               ml.unlock();
            }
         }

         nanosleep(&delay, NULL);

      }

   }

}

DWORD MsgWaitForMultipleObjects(DWORD dwSize, object ** pobjectptra, WINBOOL bWaitForAll, DWORD dwTimeout, DWORD dwWakeMask)
{

   return MsgWaitForMultipleObjectsEx(dwSize, pobjectptra, dwTimeout, dwWakeMask, (bWaitForAll ?  MWMO_WAITALL : 0));

}


DWORD WaitForMultipleObjectsEx(DWORD dwSize, object ** pobjectptra, WINBOOL bWaitForAll, DWORD dwTimeout, WINBOOL bAlertable)
{

   return MsgWaitForMultipleObjectsEx(dwSize, pobjectptra, dwTimeout, 0, (bWaitForAll ?  MWMO_WAITALL : 0) | (bAlertable ?  MWMO_ALERTABLE : 0));

}


DWORD WaitForMultipleObjects(DWORD dwSize, object ** pobjectptra, WINBOOL bWaitForAll, DWORD dwTimeout)
{

   return WaitForMultipleObjectsEx(dwSize, pobjectptra, bWaitForAll, dwTimeout, FALSE);

}


DWORD WaitForSingleObjectEx(object * pobject, DWORD dwTimeout, WINBOOL bAlertable)
{

   return WaitForMultipleObjectsEx(1, &pobject, TRUE, dwTimeout, bAlertable);

}


DWORD WaitForSingleObject(object * pobject, DWORD dwTimeout)
{

   return WaitForSingleObjectEx(pobject, dwTimeout, FALSE);

}




thread_data::thread_data()
{

   pthread_key_create(&m_key, NULL);

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


CLASS_DECL_AURA DWORD get_current_thread_id()
{

   return ::GetCurrentThreadId();

}



typedef void_ptra ThreadLocalData;



thread_pointer < ThreadLocalData > currentThreadData;
//thread_int_ptr < DWORD > currentThreadId;
//thread_pointer < HTHREAD > currentThread;
//thread_pointer < hthread > t_phthread;


raw_array<DWORD> * freeTlsIndices = NULL;
mutex * g_pmutexTlsData = NULL;


//map < HTHREAD,HTHREAD,HTHREAD,HTHREAD > * s_pmapHthreadHthread = NULL;
//map < DWORD,DWORD,HTHREAD,HTHREAD > * s_pmapDwordHthread = NULL;
//map < HTHREAD,HTHREAD,DWORD,DWORD > * s_pmapHthreadDword = NULL;
map < int_ptr,int_ptr,ThreadLocalData *,ThreadLocalData * > * allthreaddata = NULL;






void __node_init_multithreading()
{

   //s_pmapHthreadHthread = new map < HTHREAD,HTHREAD,HTHREAD,HTHREAD >();

   //s_pmapDwordHthread = new map < DWORD,DWORD,HTHREAD,HTHREAD >();

   //s_pmapHthreadDword = new map < HTHREAD,HTHREAD,DWORD,DWORD >();

   allthreaddata = new map < int_ptr,int_ptr,ThreadLocalData *,ThreadLocalData * >();

   freeTlsIndices = new raw_array<DWORD>();


}



void __node_term_multithreading()
{

   delete freeTlsIndices;

   freeTlsIndices = NULL;

   delete allthreaddata;

   allthreaddata = NULL;

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


#endif



extern "C"
void * os_thread_thread_proc(LPVOID lpparameter);


mq::mq():
m_eventNewMessage(get_thread_app())
{

}






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


static DWORD nextTlsIndex = 0;

//mutex * hthread::s_pmutex = NULL;
//ptr_array <  hthread > * hthread::s_pptra = NULL;


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

      pthread_setschedparam( hThread,iPolicy,&schedparam);

      return TRUE;

   //}

   //// Store the new priority.
   //threadInfo->m_element2.nPriority = nCa2Priority;

   return TRUE;
}




DWORD WINAPI TlsAlloc()
{

   synch_lock lock(g_pmutexTlsData);

   // Can we reuse a previously freed TLS slot?
   if(freeTlsIndices->get_count() > 0)
   {
      DWORD result = freeTlsIndices->element_at(freeTlsIndices->get_count() - 1);
      freeTlsIndices->remove_at(freeTlsIndices->get_count() - 1);
      return result;
   }

   // Allocate a new TLS slot.
   return nextTlsIndex++;
}


int_bool WINAPI TlsFree(DWORD dwTlsIndex)
{

   synch_lock lock(g_pmutexTlsData);

   //assert(dwTlsIndex < nextTlsIndex);
   /*   for(int32_t i = 0; i < freeTlsIndices.get_count(); i++)
   {
   assert(freeTlsIndices.element_at(i) != dwTlsIndex);
   }*/

   // Store this slot for reuse by TlsAlloc.
   try
   {
      freeTlsIndices->add(dwTlsIndex);
   }
   catch(...)
   {
      return false;
   }

   // Zero the value for all threads that might be using this now freed slot.

   POSITION pos = allthreaddata->get_start_position();
   while(pos != NULL)
   {

      int_ptr iThreadId;

      ThreadLocalData * pdata;

      allthreaddata->get_next_assoc(pos,iThreadId,pdata);

      if(pdata->get_count() > dwTlsIndex)
      {
         pdata->element_at(dwTlsIndex) = NULL;
      }
   }

   return true;
}


LPVOID WINAPI TlsGetValue(DWORD dwTlsIndex)
{
   ThreadLocalData* threadData = currentThreadData;

   if(threadData && threadData->get_count() > dwTlsIndex)
   {
      // Return the value of an allocated TLS slot.
      return threadData->element_at(dwTlsIndex);
   }
   else
   {
      threadData = allthreaddata->operator[](pthread_self());
      if(threadData)
      {
         currentThreadData = threadData;
         if(threadData->get_count() > dwTlsIndex)
         {
            return threadData->element_at(dwTlsIndex);
         }
      }
      // Default value for unallocated slots.
      return NULL;
   }
}


LPVOID WINAPI TlsGetValue(HTHREAD hthread,DWORD dwTlsIndex)
{

   try
   {

      synch_lock lock(g_pmutexTlsData);

      if(allthreaddata->is_empty())
         return NULL;

      ThreadLocalData * threadData = allthreaddata->operator [] (hthread);

      if(threadData && threadData->get_count() > dwTlsIndex)
      {

         // Return the value of an allocated TLS slot.
         return threadData->element_at(dwTlsIndex);

      }
      else
      {

         // Default value for unallocated slots.
         return NULL;

      }

   }
   catch(...)
   {

      return NULL;

   }

}


int_bool WINAPI TlsSetValue(DWORD dwTlsIndex,LPVOID lpTlsValue)
{

   ThreadLocalData* threadData = currentThreadData;

   if(!threadData)
   {
      // First time allocation of TLS data for this thread.
      try
      {
         threadData = new ThreadLocalData;

         synch_lock lock(g_pmutexTlsData);

         allthreaddata->set_at(pthread_self(),threadData);

         currentThreadData = threadData;

      }
      catch(...)
      {
         if(threadData)
            delete threadData;

         return false;
      }
   }

   // Store the new value for this slot.
   threadData->set_at_grow(dwTlsIndex,lpTlsValue);

   return true;
}

int_bool WINAPI TlsSetValue(HTHREAD hthread,DWORD dwTlsIndex,LPVOID lpTlsValue)
{

   synch_lock lock(g_pmutexTlsData);

   ThreadLocalData * threadData = allthreaddata->operator [] (hthread);

   if(!threadData)
   {
      // First time allocation of TLS data for this thread.
      try
      {
         threadData = new ThreadLocalData;

         allthreaddata->set_at(hthread,threadData);

      }
      catch(...)
      {
         if(threadData)
            delete threadData;

         return false;
      }
   }

   // Store the new value for this slot.
   threadData->set_at_grow(dwTlsIndex,lpTlsValue);

   return true;
}

// Called at thread exit to clean up TLS allocations.
void WINAPI TlsShutdown()
{

   ThreadLocalData * threadData = currentThreadData;

   if(threadData)
   {

      /*      try
      {

      IDWriteFactory * pfactory = TlsGetWriteFactory();

      if(pfactory != NULL)
      {

      pfactory->Release();

      }

      }
      catch(...)
      {
      }

      try
      {

      ID2D1Factory1 * pfactory = GetD2D1Factory1();

      if(pfactory != NULL)
      {

      //pfactory->Release();

      }

      }
      catch(...)
      {
      }

      */

      synch_lock ml(g_pmutexTlsData);

      allthreaddata->remove_key(pthread_self());

      currentThreadData = NULL;

      delete threadData;
   }
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

      pthread_getschedparam(hthread,&iOsPolicy,&schedparam);

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
//bool os_thread::get_run()
//{
//
//   return get()->m_bRun;
//
//}
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
//      Sleep(184);
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
//uint32_t os_thread::run()
//{
//
//   synch_lock mlThreadHandle(g_pmutexThreadHandleLock);
//
//   currentThread =  m_hthread;
//
//   mlThreadHandle.unlock();
//
//   synch_lock mlThreadId(g_pmutexThreadIdLock);
//
//   currentThreadId =  thread_id_map()[currentThread];
//
//   mlThreadId.unlock();
//
//   mlThreadHandle.unlock();
//
//   // Run the user callback.
//   //attach_thread_input_to_main_thread();
//
//   DWORD dwRet = 0xffffffff;
//
//   try
//   {
//
//      dwRet = m_pfn(m_pv);
//
//   }
//   catch(...)
//   {
//
//   }
//
//   // Clean up any TLS allocations made by this thread.
//   TlsShutdown();
//
//   // Signal that the thread has completed.
//   currentThread->m_pevent->set_event();
//
//   currentThread->release();
//
//   return dwRet;
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

   //   MESSAGE msg;

   throw "todo"; // PeekMessage function used to create message queue Windows Desktop

   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);

   g_hMainThread = hThread;

}

CLASS_DECL_AURA void set_main_thread_id(UINT uiThread)
{

   //   MESSAGE msg;

   throw "todo"; // PeekMessage function used to create message queue Windows Desktop

   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);

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
   return;
   //   MESSAGE msg;

   // metrowin todo
   throw "todo"; // PeekMessage function used to create message queue Windows Desktop

   //PeekMessage(&msg, NULL, 0, 0xffffffff, FALSE);

   //   AttachThreadInput(::GetCurrentThreadId(), get_main_thread_id(), bAttach ? TRUE : FALSE); // AttachThreadInput function used to attach thread input to main thread in Windows Desktop

}

//DWORD WINAPI thread_layer::proc(LPVOID lp)
//{
//
//   thread_layer * player   = (thread_layer *)lp;
//
//   return player->run();
//
//}
//
//
//bool is_thread(HTHREAD h)
//{
//   return GetThreadId(h) != 0;
//}

mq * get_mq(HTHREAD hthread)
{


   mq * pmq = (mq *)TlsGetValue(hthread,TLS_MESSAGE_QUEUE);

   if(pmq != NULL)
      return pmq;

   pmq               = new mq();

   pmq->m_hthread    = hthread;

   //pmq->m_uiId       = ::GetThreadId(h);

   TlsSetValue(hthread,TLS_MESSAGE_QUEUE,pmq);

   return pmq;

}




CLASS_DECL_AURA int_bool WINAPI GetMessageW(LPMESSAGE lpMsg,oswindow oswindow,UINT wMsgFilterMin,UINT wMsgFilterMax)
{


   bool bFirst = true;

   mq * pmq = get_mq();

   if(pmq == NULL)
      return FALSE;

   single_lock ml(&pmq->m_mutex,false);

   if(wMsgFilterMax == 0)
      wMsgFilterMax = (UINT)-1;

#if defined(LINUX) || defined(ANDROID)
   HTHREAD hthread = ::GetCurrentThread();
   DWORD idThre = ::GetCurrentThreadId();
#endif

restart:

   ml.lock();

   for(int32_t i = 0; i < pmq->ma.get_count(); i++)
   {
      MESSAGE & msg = pmq->ma[i];


      if(msg.message == WM_QUIT)
      {
         *lpMsg = msg;
         pmq->ma.remove_at(i);
         return FALSE;
      }


      if((oswindow == NULL || msg.hwnd == oswindow) && msg.message >= wMsgFilterMin && msg.message <= wMsgFilterMax)
      {
         *lpMsg = msg;
         pmq->ma.remove_at(i);
         return TRUE;
      }
   }

   ml.unlock();

#if defined(LINUX) // || defined(ANDROID)
   if(::get_thread() != NULL)
   {

      if(!::get_thread()->get_run())
         return FALSE;

      ::get_thread()->step_timer();

      if(!::get_thread()->get_run())
         return FALSE;

   }

   if(aura_defer_process_x_message(hthread,lpMsg,oswindow,false))
      return TRUE;

#endif

   if(bFirst)
   {

      pmq->m_eventNewMessage.wait(millis(11));

      pmq->m_eventNewMessage.ResetEvent();

      bFirst = false;

      goto restart;

   }
   else
   {

      lpMsg->message = 0xffffffff;
      lpMsg->hwnd    = NULL;
      lpMsg->wParam  = 0;
      lpMsg->lParam  = 0;
      lpMsg->pt.x    = 0x80000000;
      lpMsg->pt.y    = 0x80000000;

      return TRUE;

   }






}


CLASS_DECL_AURA int_bool WINAPI PeekMessageW(LPMESSAGE lpMsg,oswindow oswindow,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
{

   mq * pmq = get_mq();

   if(pmq == NULL)
      return FALSE;

#if defined(LINUX) || defined(ANDROID)
   HTHREAD hthread = ::GetCurrentThread();
   DWORD idThre = ::GetCurrentThreadId();
#endif

   synch_lock ml(&pmq->m_mutex);

   if(wMsgFilterMax == 0)
      wMsgFilterMax = (UINT)-1;

   for(int32_t i = 0; i < pmq->ma.get_count(); i++)
   {
      MESSAGE & msg = pmq->ma[i];

      if((oswindow == NULL || msg.hwnd == oswindow) && msg.message >= wMsgFilterMin && msg.message <= wMsgFilterMax)
      {
         *lpMsg = msg;
         if(wRemoveMsg & PM_REMOVE)
         {
            pmq->ma.remove_at(i);
         }
         return TRUE;
      }
   }

   ml.unlock();

#if defined(LINUX) // || defined(ANDROID)
   if(aura_defer_process_x_message(hthread,lpMsg,oswindow,!(wRemoveMsg & PM_REMOVE)))
      return TRUE;
#endif

   return FALSE;

}



//CLASS_DECL_AURA DWORD WINAPI GetThreadId(HTHREAD Thread)
//{
//
//   synch_lock mlThreadId(g_pmutexThreadIdLock);
//
//   map < HTHREAD,HTHREAD,DWORD,DWORD >::pair * p = thread_id_map().PLookup(Thread);
//
//   if(p == NULL)
//      return -1;
//
//
//   return p->m_element2;
//
//}

//CLASS_DECL_AURA HTHREAD  WINAPI get_thread_handle(DWORD dw)
//{
//
//   synch_lock mlThreadIdHandle(g_pmutexThreadIdHandleLock);
//
//   map < DWORD,DWORD,HTHREAD,HTHREAD >::pair * p = thread_id_handle_map().PLookup(dw);
//
//   if(p == NULL)
//      return NULL;
//
//
//   return p->m_element2;
//
//}


CLASS_DECL_AURA int_bool WINAPI PostThreadMessageW(IDTHREAD iThreadId,UINT Msg,WPARAM wParam,LPARAM lParam)
{

   //HTHREAD h = ::get_thread_handle(idThread);

   //if(h == NULL)
   //   return FALSE;


   mq * pmq = get_mq((HTHREAD) iThreadId);

   if(pmq == NULL)
      return FALSE;

   synch_lock ml(&pmq->m_mutex);

   MESSAGE msg;

   //zero(&msg, sizeof(msg));

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
//int32_t thread_layer::run()
//{
//
//   MESSAGE msg;
//
//   ZERO(msg);
//
//   while(m_bRun)
//   {
//
//      if(m_bRun && !PeekMessage(&msg,NULL,0,0xffffffffu,TRUE))
//      {
//
//         if(m_bRun && !on_idle())
//         {
//
//            Sleep(m_iSleepiness);
//
//         }
//
//         continue;
//
//      }
//
//      if(msg.message == WM_QUIT)
//         break;
//
//
//      TranslateMessage(&msg);
//
//      DispatchMessage(&msg);
//
//   }
//
//   return m_iResult;
//
//}
//
//bool thread_layer::on_idle()
//{
//
//   return false;
//
//}
//


CLASS_DECL_AURA HTHREAD GetCurrentThread()
{

   return pthread_self();

}

CLASS_DECL_AURA UINT GetCurrentThreadId()
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






