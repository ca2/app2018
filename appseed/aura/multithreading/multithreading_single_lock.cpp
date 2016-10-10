#include "framework.h"


single_lock::single_lock(sync_object * psyncobject, bool bInitialLock)
{

   m_pobjectSync = psyncobject;

   m_bAcquired = FALSE;

   if (bInitialLock)
   {

      lock();

   }

}


bool single_lock::lock()
{

   if(m_bAcquired)
   {

      return true;

   }

   if(m_pobjectSync == NULL)
   {

      return false;

   }

   try
   {

      m_pobjectSync->lock();

      m_bAcquired = true;

   }
   catch(...)
   {

      m_bAcquired = false;

   }

   return m_bAcquired;

}


bool single_lock::lock(const duration & durationTimeOut)
{
   //ASSERT(m_pobjectSync != NULL || m_hObject != NULL);
   //ASSERT(m_pobjectSync != NULL);
   //ASSERT(!m_bAcquired);


   if(m_bAcquired)
      return true;

   if(m_pobjectSync == NULL)
      return FALSE;
   try
   {
      if(durationTimeOut.is_pos_infinity())
      {
         m_pobjectSync->lock();
         m_bAcquired = true;
      }
      else
      {
         m_bAcquired = m_pobjectSync->lock(durationTimeOut);
      }
   }
   catch(...)
   {
      m_bAcquired = false;
   }
   return m_bAcquired;
}

bool single_lock::unlock()
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

bool single_lock::unlock(LONG lCount, LPLONG lpPrevCount /* = NULL */)
{
   ASSERT(m_pobjectSync != NULL);
   if (m_bAcquired)
      m_bAcquired = !m_pobjectSync->unlock(lCount, lpPrevCount);

   // successfully unlocking means it isn't acquired
   return !m_bAcquired;
}

single_lock::~single_lock()
{
   unlock();

   /*if(::get_thread() != NULL)
   {

      if(::get_thread()->m_pslUser == this)
      {
         ::get_thread()->m_pslUser = NULL;
      }

   }*/

}

bool single_lock::IsLocked()
{
   return m_bAcquired;
}
