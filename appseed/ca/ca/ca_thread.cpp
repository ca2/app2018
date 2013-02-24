#include "framework.h"


namespace ca
{


   CLASS_DECL_ca PFN_get_thread g_pfn_get_thread = NULL;
   CLASS_DECL_ca PFN_get_thread_state g_pfn_get_thread_state = NULL;


   thread::thread()
   {

      m_dwAlive                     = ::get_tick_count();
      m_bReady                      = false;
      m_bRun                        = true;
      m_pappDelete                  = NULL;
      m_pbReady                     = NULL;
      m_pthread                     = NULL;

   }



   thread * get_thread()
   {

      if(g_pfn_get_thread == NULL)
         return NULL;

      return g_pfn_get_thread();

   }


   application * get_thread_app()
   {

      thread * pthread = get_thread();

      if(pthread == NULL)
         return NULL;

      return pthread->get_app();

   }


   thread_state * get_thread_state()
   {
      if(g_pfn_get_thread_state == NULL)
         return NULL;
      return g_pfn_get_thread_state();
   }

   void thread::set_p(::gen::thread * p)
   {
      UNREFERENCED_PARAMETER(p);
      throw interface_only_exception(get_app());
   }

   void thread::construct()
   {
      CommonConstruct();
   }

   void thread::construct(__THREADPROC pfnThreadProc, LPVOID pParam)
   {
      UNREFERENCED_PARAMETER(pfnThreadProc);
      UNREFERENCED_PARAMETER(pParam);
      throw interface_only_exception(get_app());
   }


   void thread::CommonConstruct()
   {
      m_pappDelete      = NULL;
      m_puiMain         = NULL;
      m_pbReady         = NULL;
      m_bReady          = false;
/*      m_ptimera = NULL;
      m_puieptra = NULL;
      GetMainWnd() = NULL;
      m_pActiveWnd = NULL;*/

      // no HTHREAD until it is created
      //      m_hThread = NULL;
      //      m_nThreadID = 0;

      /*      ___THREAD_STATE* pState = __get_thread_state();
      // initialize message pump
      #ifdef DEBUG
      pState->m_nDisablePumpCount = 0;
      #endif
      pState->m_msgCur.message = WM_NULL;
      pState->m_nMsgLast = WM_NULL;
      System.get_cursor_pos(&(pState->m_ptCursorLast));*/

      // most threads are deleted when not needed
/*      m_bAutoDelete = TRUE;
      m_bRun = false;*/

   }

   thread::~thread()
   {
   }


   void * thread::get_os_data() const
   {

      throw interface_only_exception(get_app());

   }


   int_ptr thread::get_os_int() const
   {

      throw interface_only_exception(get_app());

   }


   HTHREAD thread::get_os_handle() const
   {

      return (HTHREAD) get_os_data();

   }


   void thread::start()
   {
      throw interface_only_exception(get_app());
   }


   bool thread::begin(::ca::e_thread_priority epriority, uint_ptr nStackSize, uint32_t dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {

      UNREFERENCED_PARAMETER(epriority);
      UNREFERENCED_PARAMETER(nStackSize);
      UNREFERENCED_PARAMETER(dwCreateFlags);
      UNREFERENCED_PARAMETER(lpSecurityAttrs);

      throw interface_only_exception(get_app());

   }


   bool thread::create_thread(::ca::e_thread_priority epriority, uint32_t dwCreateFlags, uint_ptr nStackSize, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      UNREFERENCED_PARAMETER(epriority);
      UNREFERENCED_PARAMETER(dwCreateFlags);
      UNREFERENCED_PARAMETER(nStackSize);
      UNREFERENCED_PARAMETER(lpSecurityAttrs);
      throw interface_only_exception(get_app());
   }

   void thread::Delete()
   {
      thread * pthread = this;
      gen::del(pthread);
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread default implementation

   bool thread::PreInitInstance()
   {
      return true;
   }

   bool thread::initialize_instance()
   {
      ASSERT_VALID(this);

      return true;   // by default enter run loop
   }

   bool thread::finalize()
   {
      return true;
   }

   int32_t thread::exit()
   {

      try
      {
         if(!finalize())
         {
            TRACE("There occurred errors durring ::ca::application::finalize virtual member function");
         }
      }
      catch(...)
      {
      }

      return exit_instance();

   }

   // main running routine until thread exits
   int32_t thread::run()
   {
      throw interface_only_exception(get_app());
   }

   bool thread::is_idle_message(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      throw interface_only_exception(get_app());
   }

   int32_t thread::exit_instance()
   {
      throw interface_only_exception(get_app());
   }

   bool thread::on_idle(LONG lCount)
   {
      UNREFERENCED_PARAMETER(lCount);
      throw interface_only_exception(get_app());
   }

   ::gen::message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode)
   {
      UNREFERENCED_PARAMETER(uiMessage);
      UNREFERENCED_PARAMETER(uiCode);
      throw interface_only_exception(get_app());
   }

   void thread::DispatchThreadMessageEx(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      throw interface_only_exception(get_app());
   }

   void thread::pre_translate_message(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void thread::ProcessWndProcException(base_exception* e, gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(e);
      UNREFERENCED_PARAMETER(pobj);
      throw interface_only_exception(get_app());
   }

   void thread::ProcessMessageFilter(int32_t code, gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(code);
      UNREFERENCED_PARAMETER(pobj);
      throw interface_only_exception(get_app());
   }

   /////////////////////////////////////////////////////////////////////////////
   // Access to GetMainWnd() & m_pActiveWnd

   ::user::interaction* thread::GetMainWnd()
   {
      throw interface_only_exception(get_app());
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread implementation helpers

   bool thread::pump_message()
   {
      throw interface_only_exception(get_app());
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread diagnostics

   void thread::assert_valid() const
   {
      throw interface_only_exception(get_app());
   }

   void thread::dump(dump_context & dumpcontext) const
   {
      UNREFERENCED_PARAMETER(dumpcontext);
      throw interface_only_exception(get_app());
   }

   bool thread::post_message(::user::interaction * pguie, UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
      UNREFERENCED_PARAMETER(pguie);
      UNREFERENCED_PARAMETER(uiMessage);
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);
      throw interface_only_exception(get_app());
   }

   bool thread::on_run_exception(::ca::exception & e)
   {

      UNREFERENCED_PARAMETER(e);

      return false;

   }

   ::ca::e_thread_priority thread::get_thread_priority()
   {
      throw interface_only_exception(get_app());
   }

   bool thread::set_thread_priority(::ca::e_thread_priority epriority)
   {
      UNREFERENCED_PARAMETER(epriority);
      throw interface_only_exception(get_app());
   }

   uint32_t thread::ResumeThread()
   {
      throw interface_only_exception(get_app());
   }

   /*uint32_t thread::SuspendThread()
   {
      throw interface_only_exception(get_app());
   }*/

   bool thread::post_thread_message(UINT message, WPARAM wParam, LPARAM lParam)
   {
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      throw interface_only_exception(get_app());
   }

   ::user::interaction * thread::SetMainWnd(::user::interaction * pui)
   {
      UNREFERENCED_PARAMETER(pui);
      throw interface_only_exception(get_app());
   }

   void thread::add(::user::interaction * pui)
   {
      UNREFERENCED_PARAMETER(pui);
      throw interface_only_exception(get_app());
   }

   void thread::remove(::user::interaction * pui)
   {
      UNREFERENCED_PARAMETER(pui);
      throw interface_only_exception(get_app());
   }

   ::count thread::get_ui_count()
   {
      throw interface_only_exception(get_app());
   }

   ::user::interaction * thread::get_ui(index iIndex)
   {
      UNREFERENCED_PARAMETER(iIndex);
      throw interface_only_exception(get_app());
   }

   void thread::set_timer(::user::interaction * pui, uint_ptr nIDEvent, UINT nEllapse)
   {
      UNREFERENCED_PARAMETER(pui);
      UNREFERENCED_PARAMETER(nIDEvent);
      UNREFERENCED_PARAMETER(nEllapse);
      throw interface_only_exception(get_app());
   }

   void thread::unset_timer(::user::interaction * pui, uint_ptr nIDEvent)
   {
      UNREFERENCED_PARAMETER(pui);
      UNREFERENCED_PARAMETER(nIDEvent);
      throw interface_only_exception(get_app());
   }

   void thread::set_auto_delete(bool bAutoDelete)
   {
      UNREFERENCED_PARAMETER(bAutoDelete);
      throw interface_only_exception(get_app());
   }

   void thread::set_run(bool bRun)
   {
      UNREFERENCED_PARAMETER(bRun);
      throw interface_only_exception(get_app());
   }

   event & thread::get_finish_event()
   {
      throw interface_only_exception(get_app());
   }

   bool thread::get_run()
   {
      throw interface_only_exception(get_app());
   }

   ::ca::thread * thread::get_app_thread()
   {
      throw interface_only_exception(get_app());
   }

   ::user::interaction * thread::get_active_ui()
   {
      throw interface_only_exception(get_app());
   }

   ::user::interaction * thread::set_active_ui(::user::interaction * pui)
   {
      UNREFERENCED_PARAMETER(pui);
      throw interface_only_exception(get_app());
   }

   void thread::step_timer()
   {
      throw interface_only_exception(get_app());
   }

   void thread::on_delete(::ca::ca * pui)
   {
      UNREFERENCED_PARAMETER(pui);
   }

   void thread::set_os_data(void * pvoidOsData)
   {
      UNREFERENCED_PARAMETER(pvoidOsData);
      throw interface_only_exception(get_app());
   }

   void thread::set_os_int(int_ptr iData)
   {
      UNREFERENCED_PARAMETER(iData);
      throw interface_only_exception(get_app());
   }


   int32_t thread::main()
   {
      return 0;
   }

   void thread::on_keep_alive()
   {
      m_dwAlive = ::get_tick_count();
   }

   bool thread::is_alive()
   {
      return true;
      if(!m_bRun)
         return false;
      if((::get_tick_count() - m_dwAlive) > ((1984 + 1977) * 91))
         return false;
      return true;
   }

   bool thread::has_message()
   {
      return false;
   }

   bool thread::verb()
   {
      return true;
   }

   bool thread::is_auto_delete()
   {
      throw not_implemented(get_app());
   }

	void thread::wait()
	{
      throw not_implemented(get_app());
      // on Windows ==>       ::WaitForSingleObject(m_loginthread.get_os_data(), INFINITE);

   }

	///  \brief		waits for signaling the thread for a specified time
	///  \param		duration time period to wait for thread
	///  \return	result of waiting action as defined in wait_result
   wait_result thread::wait(const duration & duration)
	{
      UNREFERENCED_PARAMETER(duration);
		throw not_implemented(get_app());
		return wait_result();
	}

	///  \brief		sets thread priority
	///  \param		new priority
	void thread::set_priority(int32_t priority)
	{
      UNREFERENCED_PARAMETER(priority);
      throw not_implemented(get_app());
   }

	///  \brief		gets thread priority
	///  \param		priority
	int32_t thread::priority()
	{
      throw not_implemented(get_app());
      return 0x80000000;
   }


} // namespace ca


namespace user
{


   LRESULT message::send()
   {
      return m_pguie->send_message(m_uiMessage, m_wparam, m_lparam);
   }

   UINT message::ThreadProcSendMessage(LPVOID lp)
   {
      message * pmessage = (message *) lp;
      pmessage->send();
      delete pmessage;
      return 0;
   }

   void message::post(::user::interaction * puie, UINT uiMessage, WPARAM wparam, LPARAM lparam, ::ca::e_thread_priority epriority)
   {
      message * pmessage = new message;
      pmessage->m_pguie = puie;
      pmessage->m_uiMessage = uiMessage;
      pmessage->m_wparam = wparam;
      pmessage->m_lparam = lparam;
      __begin_thread(puie->get_app(), &ThreadProcSendMessage, pmessage, epriority);
   }


} // namespace win



#include "framework.h"

namespace gen
{

   bool thread::s_bAllocReady = false;

   CLASS_DECL_ca void thread_alloc_ready(bool bReady)
   {
      ::gen::thread::s_bAllocReady = bReady;
   }

   thread::thread() :
      m_mutex(NULL)
   {
      m_pthread = this;
   }

   thread::thread(::ca::application * papp) :
      ca(papp),
      m_mutex(papp)
   {
      m_pthread = this;
      if(papp == NULL)
         return;
      set_app(papp);
      if(!s_bAllocReady)
         return;
      ::ca::thread_sp::create(papp);
      m_p->set_p(this);
      m_p->construct();
   }

   thread::thread(::ca::application * papp, __THREADPROC pfnThreadProc, LPVOID pParam) :
      ca(papp),
      m_mutex(papp)
   {
      m_pthread = this;
      ::ca::thread_sp::create(papp);
      m_p->set_p(this);
      m_p->construct(pfnThreadProc, pParam);
   }

   thread::~thread()
   {
   }

   void * thread::get_os_data() const
   {
      return m_p->get_os_data();
   }

   int_ptr thread::get_os_int() const
   {
      return m_p->get_os_int();
   }

   bool thread::begin(::ca::e_thread_priority epriority, uint_ptr nStackSize, uint32_t dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return m_p->begin(epriority, nStackSize, dwCreateFlags, lpSecurityAttrs);
   }

   bool thread::create_thread(::ca::e_thread_priority epriority, uint32_t dwCreateFlags, uint_ptr nStackSize, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return m_p->create_thread(epriority, dwCreateFlags, nStackSize, lpSecurityAttrs);
   }

   ::ca::e_thread_priority thread::get_thread_priority()
   {
      return m_p->get_thread_priority();
   }

   bool thread::set_thread_priority(::ca::e_thread_priority epriority)
   {
      return m_p->set_thread_priority(epriority);
   }

/*   uint32_t thread::SuspendThread()
   {
      return m_p->SuspendThread();
   }
   */

   uint32_t thread::ResumeThread()
   {
      return m_p->ResumeThread();
   }
   

   bool thread::post_thread_message(UINT message, WPARAM wParam, LPARAM lParam)
   {
      
      if(m_p == NULL)
         return false;

      return m_p->post_thread_message(message, wParam, lParam);

   }

   void thread::ProcessMessageFilter(int32_t code, gen::signal_object * pobj)
   {
      return  m_p->ProcessMessageFilter(code, pobj);
   }

   bool thread::post_message(::user::interaction * pguie, UINT message, WPARAM wParam, LPARAM lParam)
   {
      if(m_p != NULL)
      {
         return m_p->post_message(pguie, message, wParam, lParam);
      }
      else
      {
         return false;
      }
   }

   bool thread::PreInitInstance()
   {
      return m_p->PreInitInstance();
   }

   // called when occurs an standard_exception exception in run
   // return true to call run again
   bool thread::on_run_exception(::ca::exception & e)
   {
      return m_p->on_run_exception(e);
   }

   // Overridables
   // thread initialization
   bool thread::initialize_instance()
   {
      return m_p->initialize_instance();
   }

   ::gen::message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode)
   {
      return m_p->GetMessagePrototype(uiMessage, uiCode);
   }

   // running and idle processing
   int32_t thread::run()
   {
      return m_p->run();
   }

   void thread::pre_translate_message(gen::signal_object * pobj)
   {
      if(m_p == NULL)
         return;
      return m_p->pre_translate_message(pobj);
   }

   bool thread::pump_message()     // low level message pump
   {
      return m_p->pump_message();
   }

   bool thread::on_idle(LONG lCount) // return TRUE if more idle processing
   {
      return m_p->on_idle(lCount);
   }

   bool thread::is_idle_message(gen::signal_object * pobj)  // checks for special messages
   {
      return m_p->is_idle_message(pobj);
   }

   // thread termination
   int32_t thread::exit_instance() // default will 'delete this'
   {


      return m_p->exit_instance();
   }

   // Advanced: exception handling
   void thread::ProcessWndProcException(base_exception* e, gen::signal_object * pobj)
   {
      return m_p->ProcessWndProcException(e, pobj);
   }


   // Advanced: access to GetMainWnd()
   ::user::interaction* thread::GetMainWnd()
   {
      return m_p->GetMainWnd();
   }

   ::user::interaction * thread::SetMainWnd(::user::interaction * pui)
   {
      return m_p->SetMainWnd(pui);
   }

   void thread::add(::user::interaction * pui)
   {
      m_p->add(pui);
   }

   void thread::remove(::user::interaction * pui)
   {

      try
      {

         if(m_p->m_ptimera != NULL)
         {

            m_p->m_ptimera->on_delete(pui);

         }

      }
      catch(...)
      {
      }
      try
      {
         if(m_p != NULL)
         {
            m_p->remove(pui, false);
         }
      }
      catch(...)
      {
      }
      if(pui == GetMainWnd()
      || pui->m_pguie == GetMainWnd()
      || pui->m_pimpl == GetMainWnd())
      {
         SetMainWnd(NULL);
      }
/*      try
      {
         if(pui->m_pthread == this)
         {
            pui->m_pthread = NULL;
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pimpl != NULL && pui->m_pimpl != pui)
         {
            if(pui->m_pimpl->m_pthread == this)
            {
               pui->m_pimpl->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pguie != NULL && pui->m_pguie != pui)
         {
            if(pui->m_pguie->m_pthread == this)
            {
               pui->m_pguie->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }*/
   }

   ::count thread::get_ui_count()
   {
      return m_p->get_ui_count();
   }

   ::user::interaction * thread::get_ui(index iIndex)
   {
      return m_p->get_ui(iIndex);
   }


   void thread::set_timer(::user::interaction * pui, uint_ptr nIDEvent, UINT nEllapse)
   {
      m_p->set_timer(pui, nIDEvent, nEllapse);
   }

   void thread::unset_timer(::user::interaction * pui, uint_ptr nIDEvent)
   {
      m_p->unset_timer(pui, nIDEvent);
   }

   void thread::set_auto_delete(bool bAutoDelete)
   {
      m_p->set_auto_delete(bAutoDelete);
   }

   void thread::set_run(bool bRun)
   {
      m_p->set_run(bRun);
   }

   event & thread::get_finish_event()
   {
      return m_p->get_finish_event();
   }

   bool thread::get_run()
   {
      return m_p->get_run();
   }

   ::ca::thread * thread::get_app_thread()
   {
      return m_p->get_app_thread();
   }

   ::user::interaction * thread::get_active_ui()
   {
      return m_p->get_active_ui();
   }

   ::user::interaction * thread::set_active_ui(::user::interaction * pui)
   {
      return m_p->set_active_ui(pui);
   }

   void thread::step_timer()
   {
/*      try
      {
         ::gen::thread * pthreadApp = dynamic_cast < ::gen::thread * > (System.GetThread());
         if(pthreadApp != NULL && m_p != pthreadApp)
         {
            pthreadApp->step_timer();
         }
      }
      catch(...)
      {
      }*/
      return m_p->step_timer();
   }

   int32_t thread::main()
   {
      return m_p->main();
   }



   void thread::assert_valid() const
   {
      m_p->assert_valid();
   }
   void thread::dump(dump_context & dumpcontext) const
   {
      m_p->dump(dumpcontext);
   }


   // 'delete this' only if m_bAutoDelete == TRUE
   void thread::Delete()
   {
      m_p->Delete();
   }

   void thread::DispatchThreadMessageEx(gen::signal_object * pobj)  // helper
   {
      return m_p->DispatchThreadMessageEx(pobj);
   }

	void thread::wait()
   {

      return m_p->wait();
      // on Windows ==>       ::WaitForSingleObject(m_loginthread.get_os_data(), INFINITE);

   }

#ifdef WINDOWS
	HANDLE thread::item() const
   {

      return m_p->item();
   }
#else
	int_ptr thread::item() const
   {
      return m_p->item();

   }
#endif



} // namespace gen




::gen::thread* __begin_thread(::ca::application * papp, __THREADPROC pfnThreadProc, LPVOID pParam, ::ca::e_thread_priority epriority, UINT nStackSize, uint32_t dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{

   ASSERT(pfnThreadProc != NULL);

   ::gen::thread* pThread = new ::gen::thread(papp, pfnThreadProc, pParam);
   ASSERT_VALID(pThread);

   if (!pThread->create_thread(epriority, dwCreateFlags, nStackSize, lpSecurityAttrs))
   {
      pThread->Delete();
      return NULL;
   }

   return pThread;

}



