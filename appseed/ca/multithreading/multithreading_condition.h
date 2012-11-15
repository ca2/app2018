#pragma once


class CLASS_DECL_ca condition :
   virtual public waitable
{
public:

#if defined(LINUX) || defined(MACOS)
   int_ptr     m_object;
#else
   CRITICAL_SECTION     m_sect;
   CONDITION_VARIABLE   m_var;
#endif


   condition(::ca::application * papp);
   virtual ~condition();



   virtual bool lock(const duration & durationTimeout = duration::infinite());

   using waitable::unlock;
   virtual bool unlock();

   virtual void * get_os_data() const;

   bool pulse();

/**
* \brief	Platform independent waitables (windows version)
* \author	Thomas Hoefer
*/

	///  \brief		waits for an event forever
	virtual void wait ();

	///  \brief		waits for an event for a specified time
	///  \param		duration time period to wait for an event
	///  \return	waiting action result as WaitResult
   virtual wait_result wait (const duration & duration);

   bool is_signaled() const;

};

