//
//  multithreading_thread_impl.h
//  ace
//
//
//
#pragma once


#ifdef WINDOWS

typedef uint32_t IDTHREAD;

#else
#define QS_ALLEVENTS 0xffff

void __clear_mq();

typedef HTHREAD IDTHREAD;

template <  >
inline bool EqualElements<IDTHREAD>(IDTHREAD r1, IDTHREAD r2)
{
   return id_thread_equals(r1, r2) != 0;
}
template <  >
inline UINT HashKey(IDTHREAD key)
{
   return 0;
}

#endif



class thread_startup :
   virtual public object
{
public:


   ::thread *              m_pthread;
//   ::thread_impl *         m_pthreadimpl;
   manual_reset_event      m_event;
   manual_reset_event      m_event2;
   DWORD                   m_dwCreateFlags;
   bool                    m_bError;
   int32_t                 m_iError;
   bool                    m_bSynch;


   thread_startup(::ace::application * papp);
   ~thread_startup();

};



//class CLASS_DECL_ACE thread_impl :
//   virtual public command_target
//{
//public:
//
//
//   //bool                                      m_bIdle;
//   //LONG                                      m_lIdleCount;
//
////   ::user::message_queue_sp                 m_spqueue;
//
//
//   thread_impl(::ace::application * papp);
//   virtual ~thread_impl();
//
//
//   void construct();
//
//   void construct(__THREADPROC pfnthread_implProc, LPVOID pParam);
//
//   void CommonConstruct();
//
//   virtual bool pre_init_instance();
//
//   virtual bool initialize_instance();
//
//   virtual bool finalize();
//
//   virtual void pre_translate_message(signal_details * pobj);
//
//   virtual void process_window_procedure_exception(::exception::base*,signal_details * pobj);
//
//   virtual void process_message_filter(int32_t code,signal_details * pobj);
//
//   virtual int32_t thread_startup(::thread_startup * pstartup);
//   virtual bool thread_entry();
//   virtual int32_t main();
//   virtual int32_t thread_term();
//
//
//   virtual int32_t exit_instance();
//   virtual bool on_idle(LONG lCount);
//
//
//   virtual void * get_os_data() const;
//   virtual IDTHREAD get_os_int() const;
//
//   operator HTHREAD() const;
//
//   virtual HTHREAD item() const;
//
//   virtual void add(::user::primitive * pui);
//   virtual void remove(::user::primitive * pui);
//
//   virtual bool is_idle_message(signal_details * pobj);
//   virtual bool is_idle_message(LPMESSAGE lpmsg);
//
//   virtual void post_to_all_threads(UINT message,WPARAM wparam,LPARAM lparam);
//   virtual bool post_thread_message(UINT message,WPARAM wParam = 0,lparam lParam = cnull);
//   virtual bool post_message(::user::primitive * pui,UINT message,WPARAM wParam = 0,lparam lParam = cnull);
//
//   virtual ::count get_ui_count();
//   virtual ::user::primitive * get_ui(::index iIndex);
//   virtual event & get_finish_event();
//   //virtual void step_timer();
//
//
//   virtual void set_os_data(void * pvoidOsData);
//   virtual void set_os_int(IDTHREAD iData);
//
//   virtual bool initialize_message_queue();
//
//
//   virtual int32_t run();
//
//   virtual void message_handler(signal_details * pobj);
//
//
//   virtual bool pump_message();
//   virtual bool defer_pump_message();
//   virtual bool process_message(LPMESSAGE lpmessage);
//
//
//
//
//   virtual bool begin(int32_t epriority,uint_ptr nStackSize,uint32_t dwCreateFlags,LPSECURITY_ATTRIBUTES lpSecurityAttrs);
//   virtual bool create_thread(int32_t epriority,uint_ptr nStackSize,uint32_t dwCreateFlagsParam,LPSECURITY_ATTRIBUTES lpSecurityAttrs);
//   virtual bool begin_synch(int32_t *piStartupError,int32_t epriority,uint_ptr nStackSize,uint32_t dwCreateFlags,LPSECURITY_ATTRIBUTES lpSecurityAttrs);
//   virtual bool create_thread_synch(int32_t *piStartupError,int32_t epriority,uint_ptr nStackSize,uint32_t dwCreateFlags,LPSECURITY_ATTRIBUTES lpSecurityAttrs);
//   virtual bool begin_thread(bool bSynchStartup,int32_t *piStartupError,int32_t epriority,uint_ptr nStackSize,uint32_t dwCreateFlagsParam,LPSECURITY_ATTRIBUTES lpSecurityAttrs);
//
//   virtual void thread_impl_delete();
//
//   virtual void dispatch_thread_message(signal_details * pobj);
//
//   virtual void start();
//   virtual uint32_t ResumeThread();
//   virtual bool has_message();
//
//   virtual void set_priority(int32_t priority);
//   virtual int32_t priority();
//
//   virtual bool set_thread_priority(int32_t priority);
//   virtual int32_t get_thread_priority();
//
//   virtual bool on_run_exception(::exception::exception &);
//
//
//   virtual message::e_prototype GetMessagePrototype(UINT uiMessage,UINT uiCode);
//
//   virtual int get_x_window_count() const;
//
//   virtual void do_events();
//
//   virtual void message_queue_message_handler(::signal_details * pobj);
//
//};
//
//
//
//
//