#include "framework.h"


#if defined(WINDOWS)

#define M_OBJECTA m_handlea

#else

#define M_OBJECTA m_syncobjectptra

#endif
#undef new
multi_lock::multi_lock(std::initializer_list < sync_object * > list,bool bInitialLock)
{
   sync_object_ptra syncobjectptra;
   forallref(list)
   {
      syncobjectptra.add(item);
   }
   ::new((void *) this) multi_lock(syncobjectptra,bInitialLock);
}
#define new AURA_NEW
multi_lock::multi_lock(const sync_object_ptra & syncobjectptra,bool bInitialLock)
{

   ASSERT(syncobjectptra.get_count() > 0 && syncobjectptra.get_count() <= MAXIMUM_WAIT_OBJECTS);

   if(syncobjectptra.get_count() <= 0)
   {

      _throw(invalid_argument_exception(get_app()));

   }

   m_syncobjectptra  = syncobjectptra;

   M_OBJECTA.allocate(m_syncobjectptra.get_count());

   m_baLocked.allocate(m_syncobjectptra.get_count());


   for (index i = 0; i < m_syncobjectptra.get_count(); i++)
   {

      if(m_syncobjectptra[i] == NULL)
      {

         _throw(invalid_argument_exception(get_app()));

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


multi_lock::multi_lock(::count c, const sync_object_ptra & syncobjectptra, bool bInitialLock)
{

   ASSERT(syncobjectptra.get_count() > 0 && c > 0 && c <= syncobjectptra.get_count() && c <= MAXIMUM_WAIT_OBJECTS);

   if (syncobjectptra.get_count() <= 0 || c <= 0 || c > syncobjectptra.get_count())
   {

      _throw(invalid_argument_exception(get_app()));

   }

   m_syncobjectptra.set_size(c);

   for (index i = 0; i < c; i++)
   {

      m_syncobjectptra[i] = syncobjectptra[i];

   }

   M_OBJECTA.allocate(m_syncobjectptra.get_count());

   m_baLocked.allocate(m_syncobjectptra.get_count());

   for (index i = 0; i < m_syncobjectptra.get_count(); i++)
   {

      if (m_syncobjectptra[i] == NULL)
      {

         _throw(invalid_argument_exception(get_app()));

      }

#ifdef WINDOWS

      m_handlea[i] = (HANDLE)m_syncobjectptra[i]->get_os_data();

#else

      M_OBJECTA[i] = m_syncobjectptra[i];

#endif

      m_baLocked[i] = FALSE;

   }

   if (bInitialLock)
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

   uint32_t iResult;

   if(M_OBJECTA.get_count() < 0)
      return wait_result(wait_result::Failure);

   if (dwWakeMask == 0)
   {

      iResult = ::WaitForMultipleObjectsEx((uint32_t) M_OBJECTA.get_count(), M_OBJECTA.get_data(), bWaitForAll, duration.lock_duration(), FALSE);

   }
   else
   {

#ifdef METROWIN

      iResult = ::WinMsgWaitForMultipleObjects((uint32_t) M_OBJECTA.get_count(), M_OBJECTA.get_data(), bWaitForAll, duration.lock_duration(), dwWakeMask);

#else


      iResult = ::MsgWaitForMultipleObjects((uint32_t)M_OBJECTA.get_count(), M_OBJECTA.get_data(), bWaitForAll, duration.lock_duration(), dwWakeMask);

#endif

   }

   DWORD iUpperBound = WAIT_OBJECT_0 + (DWORD) M_OBJECTA.get_count();
   if(iResult == WAIT_FAILED)
   {
      DWORD dw = ::get_last_error();
      TRACELASTERROR();
   }
   else if (iResult >= WAIT_OBJECT_0 && iResult < iUpperBound)
   {
      if (bWaitForAll)
      {
         for (index i = 0; i < M_OBJECTA.get_count(); i++)
            m_baLocked[i] = TRUE;
      }
      else
      {
         m_baLocked[(index)(iResult - WAIT_OBJECT_0)] = TRUE;
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



