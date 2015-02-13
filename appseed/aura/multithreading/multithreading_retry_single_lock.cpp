#include "framework.h"

/////////////////////////////////////////////////////////////////////////////
// retry_single_lock

retry_single_lock::retry_single_lock(object * psyncobject, duration durationLock, duration durationSleep, int32_t iRetry, bool bInitialLock)
{

   m_pobjectSync     = psyncobject;
   m_bAcquired       = FALSE;
   m_durationLock    = durationLock;
   m_durationSleep   = durationSleep;
   m_iRetry          = iRetry;

   if(bInitialLock)
   {

      lock();

   }

}

bool retry_single_lock::lock()
{

   if(m_pobjectSync == NULL)
      return FALSE;

   int32_t iRetry = 0;

   try
   {
      while(!(m_bAcquired = m_pobjectSync->lock(m_durationLock)))
      {

         sleep(m_durationSleep);

         iRetry++;

         if(m_iRetry >= 0 && iRetry >= m_iRetry)
            break;
      }
   }
   catch(...)
   {
      m_bAcquired = false;
   }

   return m_bAcquired;

}

bool retry_single_lock::unlock()
{

   if(m_pobjectSync == NULL)
      return FALSE;

   if (m_bAcquired)
   {
      try
      {
         m_bAcquired = !m_pobjectSync->unlock();
      }
      catch(...)
      {
         m_bAcquired = true;
      }
   }

   // successfully unlocking means it isn't acquired
   return !m_bAcquired;
}


retry_single_lock::~retry_single_lock()
{
   unlock();
}

bool retry_single_lock::IsLocked()
{
   return m_bAcquired;
}
