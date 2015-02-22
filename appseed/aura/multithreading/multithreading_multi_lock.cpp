#include "framework.h"


#if defined(WINDOWS)

#define M_OBJECTA m_handlea

#else

#define M_OBJECTA m_objecta

#endif


multi_lock::multi_lock(const sync_object_ptra & syncobjectptra,bool bInitialLock)
{

   ASSERT(syncobjectptra.get_count() > 0 && syncobjectptra.get_count() <= MAXIMUM_WAIT_OBJECTS);

   if(syncobjectptra.get_count() <= 0)
   {

      throw invalid_argument_exception(::get_thread_app());

   }

   m_syncobjectptra  = syncobjectptra;

   M_OBJECTA.allocate(m_syncobjectptra.get_count());

   m_baLocked.allocate(m_syncobjectptra.get_count());

   
   for (index i = 0; i < m_syncobjectptra.get_count(); i++)
   {

      if(m_syncobjectptra[i] == NULL)
      {

         throw invalid_argument_exception(::get_thread_app());

      }

#ifdef WINDOWS

      m_handlea[i] = (HANDLE) m_syncobjectptra[i]->get_os_data();

#else

      M_OBJECTA[i] = m_syncobjectptra[i];

#endif

      m_baLocked[i] = FALSE;

   }

   if(bInitialLock)
   {

      lock();

   }

}


multi_lock::~multi_lock()
{

   unlock();

}


wait_result multi_lock::lock(const duration & duration, bool bWaitForAll, uint32_t dwWakeMask /* = 0 */)
{

   int32_t iResult;

   if(M_OBJECTA.get_count() < 0)
      return wait_result(wait_result::Failure);

   if (dwWakeMask == 0)
   {

      iResult = ::WaitForMultipleObjectsEx((uint32_t) M_OBJECTA.get_count(), M_OBJECTA.get_data(), bWaitForAll, duration.lock_duration(), FALSE);

   }
   else
   {

      iResult = ::MsgWaitForMultipleObjects((uint32_t) M_OBJECTA.get_count(), M_OBJECTA.get_data(), bWaitForAll, duration.lock_duration(), dwWakeMask);

   }

   index iUpperBound = WAIT_OBJECT_0 + M_OBJECTA.get_count();
   if (iResult >= WAIT_OBJECT_0 && iResult < iUpperBound)
   {
      if (iUpperBound >= M_OBJECTA.get_count() && iUpperBound >= WAIT_OBJECT_0)
      {
         if (bWaitForAll)
         {
            for (index i = 0; i < M_OBJECTA.get_count(); i++)
               m_baLocked[i] = TRUE;
         }
         else
         {
            ASSERT((iResult >= WAIT_OBJECT_0) && (iResult - WAIT_OBJECT_0) <= natural(iResult));
            if ((iResult >= WAIT_OBJECT_0) && (iResult - WAIT_OBJECT_0 <= natural(iResult)))
               m_baLocked[(index)(iResult - WAIT_OBJECT_0)] = TRUE;
         }
      }
   }
   return wait_result(iResult);
}

bool multi_lock::unlock()
{
   for (index i=0; i < M_OBJECTA.get_count(); i++)
   {
      if (m_baLocked[i])
         m_baLocked[i] = !m_syncobjectptra[i]->unlock();
   }
   return TRUE;
}

bool multi_lock::unlock(LONG lCount, LPLONG lpPrevCount /* =NULL */)
{
   bool bGotOne = false;
   for (index i=0; i < M_OBJECTA.get_count(); i++)
   {
      if (m_baLocked[i])
      {
         semaphore* pSemaphore = dynamic_cast < semaphore * > (m_syncobjectptra[i]);
         if (pSemaphore != NULL)
         {
            bGotOne = true;
            m_baLocked[i] = !m_syncobjectptra[i]->unlock(lCount, lpPrevCount);
         }
      }
   }

   return bGotOne;
}




bool multi_lock::IsLocked(index dwObject)
{
   ASSERT(dwObject < M_OBJECTA.get_count());
   return m_baLocked[dwObject];
}



