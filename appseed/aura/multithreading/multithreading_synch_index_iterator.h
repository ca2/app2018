#pragma once

//
//template < class T >
//class synch_ptr_array;
//
//
//class  CLASS_DECL_AURA synch_index_iterator:
//   public single_lock
//{
//public:
//
//   
//   index                         m_i;
//   synch_index_ptr_array *       m_pindexptra;
//   ::mutex *                     m_pmutex;
//   ::count                       m_c;
//   raw_ref_array < void * > *    m_pptra;
//
//
//   synch_index_iterator(synch_index_ptr_array * pindexptra,mutex * pmutex,bool bInitialLock = true):
//      single_lock(pmutex,true)
//   {
//      m_pptra = NULL;
//      init(pindexptra,bInitialLock);
//   }
//
//   template < class T >
//   synch_index_iterator(synch_ptr_array < T > & ptra,bool bInitialLock = true);
//
//
//   void init(synch_index_ptr_array * pindexptra,bool bInitialLock)
//   {
//      try
//      {
//
//         m_pindexptra = pindexptra;
//         m_c = m_pindexptra->get_count();
//         m_pindexptra->reg(&m_i);
//
//      }
//      catch(...)
//      {
//
//      }
//
//      if(!bInitialLock)
//      {
//
//         unlock();
//
//      }
//
//   }
//
//   ~synch_index_iterator()
//   {
//      try
//      {
//
//         lock();
//         m_pindexptra->unreg(&m_i);
//
//      }
//      catch(...)
//      {
//
//      }
//
//   }
//
//};
//
//
