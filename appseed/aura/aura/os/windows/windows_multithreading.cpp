//#include "framework.h"
//#include "framework.h"
//#include <process.h>    // for _beginthreadex and _endthreadex
#undef new
#define min MIN
#define max MAX
//#include <gdiplus.h>
#undef min
#undef max



thread_data::thread_data()
{

   m_dwIndex = thread_alloc();

}


thread_data::~thread_data()
{

   thread_free(m_dwIndex);

}


static HANDLE g_hMainThread = NULL;
static UINT g_uiMainThread = -1;

void set_main_thread(HANDLE hThread)
{

   MSG msg;

   PeekMessage(&msg,NULL,0,0xffffffff,FALSE);

   g_hMainThread = hThread;

}

void set_main_thread_id(UINT uiThread)
{

   MSG msg;

   PeekMessage(&msg,NULL,0,0xffffffff,FALSE);

   g_uiMainThread = uiThread;

}


HANDLE get_main_thread()
{
   return g_hMainThread;

}
UINT   get_main_thread_id()
{
   return g_uiMainThread;
}


void attach_thread_input_to_main_thread(bool bAttach)
{

   MSG msg;

   PeekMessage(&msg,NULL,0,0xffffffff,FALSE);

   AttachThreadInput(::GetCurrentThreadId(),get_main_thread_id(),bAttach ? TRUE : FALSE);

}


//uint32_t thread_layer::proc(void * lp)
//{
//
//   thread_layer * player   = (thread_layer *)lp;
//
//   return player->run();
//
//}


namespace multithreading
{

   bool set_priority(int32_t priority)
   {

      return (::SetThreadPriority(::GetCurrentThread(),get_os_thread_priority(priority)) != 0);
   }

   int32_t priority()
   {
      return ::GetThreadPriority(::GetCurrentThread());
   }


} // namespace core




HTHREAD get_current_thread()
{
   
   return ::GetCurrentThread();

}


IDTHREAD get_current_thread_id()
{

   return ::GetCurrentThreadId();

}






int32_t get_os_thread_priority(int32_t iCa2Priority)
{

   if(iCa2Priority <= (int) ::multithreading::priority_none)
      return THREAD_PRIORITY_NORMAL;

   if(iCa2Priority <= (int) ::multithreading::priority_idle)
      return THREAD_PRIORITY_IDLE;

   if(iCa2Priority <= (int) ::multithreading::priority_lowest)
      return THREAD_PRIORITY_LOWEST;

   if(iCa2Priority <= (int) ::multithreading::priority_below_normal)
      return THREAD_PRIORITY_BELOW_NORMAL;

   if(iCa2Priority <= (int) ::multithreading::priority_normal)
      return THREAD_PRIORITY_NORMAL;

   if(iCa2Priority <= (int) ::multithreading::priority_above_normal)
      return THREAD_PRIORITY_ABOVE_NORMAL;

   if(iCa2Priority <= (int) ::multithreading::priority_highest)
      return THREAD_PRIORITY_HIGHEST;

   return THREAD_PRIORITY_TIME_CRITICAL;

}



int32_t get_os_priority_class(int32_t iCa2Priority)
{

   if(iCa2Priority <= (int) ::multithreading::priority_none)
      return 0;

   if(iCa2Priority <= (int) ::multithreading::priority_idle)
      return IDLE_PRIORITY_CLASS;

   if(iCa2Priority <= (int) ::multithreading::priority_lowest)
      return BELOW_NORMAL_PRIORITY_CLASS;

   if(iCa2Priority <= (int) ::multithreading::priority_below_normal)
      return BELOW_NORMAL_PRIORITY_CLASS;

   if(iCa2Priority <= (int) ::multithreading::priority_normal)
      return NORMAL_PRIORITY_CLASS;

   if(iCa2Priority <= (int) ::multithreading::priority_above_normal)
      return ABOVE_NORMAL_PRIORITY_CLASS;

   if(iCa2Priority <= (int) ::multithreading::priority_highest)
      return HIGH_PRIORITY_CLASS;

   return REALTIME_PRIORITY_CLASS;

}







int32_t get_os_thread_scheduling_priority(int32_t nPriority)
{

   int32_t iCa2Priority;

   if(nPriority <= THREAD_PRIORITY_IDLE)
   {
      iCa2Priority = ::multithreading::priority_idle;
   }
   else if(nPriority <= THREAD_PRIORITY_LOWEST)
   {
      iCa2Priority = ::multithreading::priority_lowest;
   }
   else if(nPriority <= THREAD_PRIORITY_BELOW_NORMAL)
   {
      iCa2Priority = ::multithreading::priority_below_normal;
   }
   else if(nPriority <= THREAD_PRIORITY_NORMAL)
   {
      iCa2Priority = ::multithreading::priority_normal;
   }
   else if(nPriority <= THREAD_PRIORITY_ABOVE_NORMAL)
   {
      iCa2Priority = ::multithreading::priority_above_normal;
   }
   else if(nPriority <= THREAD_PRIORITY_HIGHEST)
   {
      iCa2Priority = ::multithreading::priority_highest;
   }
   else
   {
      iCa2Priority = ::multithreading::priority_time_critical;
   }

   return iCa2Priority;

}


int32_t get_os_class_scheduling_priority(int32_t nPriority)
{

   int32_t iCa2Priority;

   if(nPriority <= IDLE_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_idle;
   }
   else if(nPriority <= BELOW_NORMAL_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_below_normal;
   }
   else if(nPriority <= NORMAL_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_normal;
   }
   else if(nPriority <= ABOVE_NORMAL_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_above_normal;
   }
   else if(nPriority <= HIGH_PRIORITY_CLASS)
   {
      iCa2Priority = ::multithreading::priority_highest;
   }
   else
   {
      iCa2Priority = ::multithreading::priority_time_critical;
   }

   return iCa2Priority;

}



bool on_init_thread()
{

   attach_thread_input_to_main_thread();

   if(!__os_init_thread())
   {

      return false;

   }

   return true;

}


bool on_term_thread()
{

   bool bOk1 = __os_term_thread();

   return bOk1;

}





void __node_init_multithreading()
{

}



void __node_term_multithreading()
{

}



extern CLASS_DECL_AURA thread_int_ptr < HRESULT > t_hresultCoInitialize;



bool __os_init_thread()
{

   if(!defer_co_initialize_ex())
      return false;

   //t_hresultCoInitialize = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);

   //if(FAILED(t_hresultCoInitialize))
   ////{

   ////   if(t_hresultCoInitialize.operator HRESULT() == RPC_E_CHANGED_MODE)
   ////   {

   ////      t_hresultCoInitialize = ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);

   ////      if(FAILED(t_hresultCoInitialize))
   ////      {

   ////         ::simple_message_box(NULL,"Failed to ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) at __node_pre_init","__node_pre_init failure",MB_ICONEXCLAMATION);

   ////         return false;

   ////      }

   ////   }
   ////   else
   ////   {

   ////      ::simple_message_box(NULL,"Failed to ::CoInitializeEx(NULL, COINIT_MULTITHREADED) at __node_pre_init","__node_pre_init failure",MB_ICONEXCLAMATION);

   ////      return false;

   ////   }

   ////}

   return true;

}



bool __os_term_thread()
{

   if(SUCCEEDED(t_hresultCoInitialize))
   {
      
      CoUninitialize();

   }

   return true;

}


//void _on_os_hthread_end(int iRet)
//{
//
//   // allow C-runtime to cleanup, and exit the thread
//   try
//   {
//
//      _endthreadex(iRet);
//
//   }
//   catch(...)
//   {
//
//   }
//
//}

CLASS_DECL_AURA uint32_t thread_alloc()
{

   return (uint32_t) TlsAlloc();

}


CLASS_DECL_AURA void * thread_get_data(uint32_t uiIndex)
{
   
   return (void *) TlsGetValue((DWORD) uiIndex);

}


CLASS_DECL_AURA void thread_set_data(uint32_t uiIndex, void * pvalue)
{
  
   TlsSetValue((DWORD)uiIndex, (LPVOID) pvalue);

}


CLASS_DECL_AURA int_bool thread_free(uint32_t uiIndex)
{

   return TlsFree((DWORD) uiIndex);

}


CLASS_DECL_AURA void thread_shutdown()
{

}
