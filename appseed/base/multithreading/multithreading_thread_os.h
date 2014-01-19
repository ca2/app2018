#pragma once


#ifdef WINDOWSEX

typedef HANDLE HTHREAD;

#else

class event;

typedef hthread * HTHREAD;

#endif



/**
* \file		src/lib/pal/linux/thread_linux.hpp
*/

namespace core
{


	// An enum representing thread priorities.
	enum e_scheduling_priority
   {

      scheduling_priority_none    = -1,
		scheduling_priority_idle 	= 1,
		scheduling_priority_lowest 	= 48,
		scheduling_priority_below_normal,
		scheduling_priority_normal,
		scheduling_priority_above_normal,
		scheduling_priority_highest,
		scheduling_priority_time_critical = 99

	};

	//  \brief		global function to set thread priority for current thread
	//  \param		new priority
	CLASS_DECL_BASE bool set_thread_priority(int32_t priority);
	CLASS_DECL_BASE bool set_priority_class(int32_t priority);

	//  \brief		global function to get thread priority for current thread
	//  \return	priority of current thread
	CLASS_DECL_BASE int32_t thread_priority();


} // namespace core


inline int32_t get_scheduling_priority_none()
{

      return ::core::scheduling_priority_none;

}


inline int32_t get_scheduling_priority_normal()
{

      return ::core::scheduling_priority_normal;

}



class CLASS_DECL_BASE os_thread :
   virtual public object
{
public:


   uint32_t (*                            m_pfn)(void *);
   LPVOID                                 m_pv;
   bool                                   m_bRun;
   HTHREAD                                m_hthread;

#if defined(LINUX) || defined(MACOS) || defined(ANDROID)

   pthread_t                              m_pthread;

#endif

   static mutex *                  s_pmutex;
   static comparable_raw_array < os_thread * >::type *  s_pptra;


   os_thread(uint32_t ( * pfn)(void *), void * pv);
   virtual ~os_thread();


#if defined(LINUX) || defined(MACOS) || defined(ANDROID)

   static void * thread_proc(void * pparam);

#else

   static unsigned int WINAPI thread_proc(void * pparam);

#endif

   uint32_t run();

   static os_thread * get();
   static void set(os_thread * posthread);

   static bool get_run();

   static void stop_all(uint32_t millisMaxWait);

};


CLASS_DECL_BASE HTHREAD start_thread(uint32_t (*)(void *), void * pv, int32_t iPriority = 0);

CLASS_DECL_BASE HTHREAD create_thread(LPSECURITY_ATTRIBUTES lpsa, uint_ptr cbStack, uint32_t (*)(void *), void * pv, uint32_t uiFlags, uint32_t * puiId);


class CLASS_DECL_BASE thread_layer
{
public:


   int32_t              m_iSleepiness;
   int32_t              m_iResult;
   HTHREAD              m_hthread;
   UINT                 m_nId;


   thread_layer();
   virtual ~thread_layer();


   void begin();


   static uint32_t proc(void * lp);

   virtual int32_t run();
   virtual bool on_idle();

   virtual void wait_thread(uint32_t dwMillis = INFINITE);

};




CLASS_DECL_BASE DWORD get_current_thread_id();




