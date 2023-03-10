// This is core API library.
// 
// 
//
// 
// 
// 
// 
// 

// Inlines for AFXMT.H

#pragma once

#ifdef _AFXMT_INLINE

_AFXMT_INLINE int_bool semaphore::Unlock()
   { return Unlock(1, NULL); }

_AFXMT_INLINE int_bool CEvent::SetEvent()
   { ASSERT(m_hObject != NULL); return ::SetEvent(m_hObject); }
_AFXMT_INLINE int_bool CEvent::PulseEvent()
   { ASSERT(m_hObject != NULL); return ::PulseEvent(m_hObject); }
_AFXMT_INLINE int_bool CEvent::ResetEvent()
   { ASSERT(m_hObject != NULL); return ::ResetEvent(m_hObject); }

_AFXMT_INLINE CSingleLock::~CSingleLock()
   { Unlock(); }
_AFXMT_INLINE int_bool CSingleLock::IsLocked()
   { return m_bAcquired; }

_AFXMT_INLINE int_bool CMultiLock::IsLocked(DWORD dwObject)
   { ASSERT(dwObject < m_dwCount);
       return m_bLockedArray[dwObject]; }

_AFXMT_INLINE int_bool critical_section::Init()
{
   __try
   {
      ::InitializeCriticalSection(&m_sect);
   }
   __except(STATUS_NO_MEMORY == GetExceptionCode())
   {
      return FALSE;
   }

   return TRUE;
}

_AFXMT_INLINE critical_section::critical_section() : sync_object < HANDLE > (NULL)
   {    
      int_bool bSuccess;

      bSuccess = Init();
      if (!bSuccess)
         _throw(memory_exception(get_app()));
   }

_AFXMT_INLINE critical_section::operator CRITICAL_SECTION*()
   { return (CRITICAL_SECTION*) &m_sect; }
_AFXMT_INLINE critical_section::~critical_section()
   { ::DeleteCriticalSection(&m_sect); }
_AFXMT_INLINE int_bool critical_section::Lock()
   {   
      __try
      {
         ::EnterCriticalSection(&m_sect); 
      }
      __except(STATUS_NO_MEMORY == GetExceptionCode())
      {
         _throw(memory_exception(get_app()));
      }
      return TRUE; 
   }
_AFXMT_INLINE int_bool critical_section::Lock(DWORD dwTimeout)
   { ASSERT(dwTimeout == INFINITE); (void)dwTimeout; return Lock(); }
_AFXMT_INLINE int_bool critical_section::Unlock()
   { ::LeaveCriticalSection(&m_sect); return TRUE; }

#endif //_AFXMT_INLINE







	inline bool wait_result::abandoned() const
	{ return m_iWaitResult <= Abandon0; }

	inline size_t wait_result::abandoned_index() const
	{
		if ( !abandoned() )
         _throw(range_error(get_app(), "abandoned index out of range"));
		return -(m_iWaitResult + Abandon0);
	}

	inline bool wait_result::failed() const
	{ return m_iWaitResult == Failure; }

	inline bool wait_result::bad_thread() const
	{ return m_iWaitResult == BadThread; }

	inline bool wait_result::timeout() const
	{ return m_iWaitResult == Timeout; }

	inline bool wait_result::signaled() const
	{ return m_iWaitResult >= Event0; }

   inline bool wait_result::succeeded() const
   {
      return signaled();
   }

	inline size_t wait_result::signaled_index() const
	{
		if ( !signaled() )
			_throw(range_error(get_app(), "signaled index out of range"));
		return m_iWaitResult - Event0;
	}





   //template <class T >
   //inline synch_index_iterator::synch_index_iterator(synch_ptr_array < T > & ptra,bool bInitialLock):
   //   single_lock(&ptra.m_mutex,true)
   //{
   //   
   //   m_pptra = &ptra;

   //   init(&ptra.m_indexptra,bInitialLock);

   //}









template <typename T>
struct hold
{
public:
   
   T                    m_t;
   bool                 m_bInitialized = false;
   manual_reset_event   m_evReady;
   string               m_strErrorMessage;
   
   
   void wait()
   {
      
      m_evReady.wait();
      
   }
   
   T & get()
   {
      
      wait();
      
      if(m_bInitialized)
      {
         
         return m_t;
         
      }
      
      _throw(::simple_exception(get_app(), m_strErrorMessage));
      
   }
   
   template <typename U>
   void set_value(const U& value)
   {
      
      m_t = value;
      
      m_bInitialized = true;
      
      m_evReady.set_event();
      
   }
   
   void set_error_message(const string & strErrorMessage)
   {
      
      m_strErrorMessage = strErrorMessage;
      
      m_evReady.set_event();
      
   }
   
   
   
   hold(::aura::application * papp) :
   m_evReady(papp)
   {
      
   }
   
   ~hold()
   {
      
   }
   
   
   bool valid() const noexcept
   {
      
      return m_bInitialized;
      
   }
   
   template < typename PRED >
   void work(PRED pred)
   {
      
      m_bInitialized = false;
      
      ::fork(m_evReady.get_app(), [&]()
             {
                
                try
                {
                   
                   set_value(pred());
                   
                }
                catch(const ::exception::base & e)
                {
                   
                   m_strErrorMessage = ((::exception::base &)e).get_message();
                   
                }
                
             });
      
   }
   
};



