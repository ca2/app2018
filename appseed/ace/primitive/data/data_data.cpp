//#include "framework.h"


namespace data
{
/*
   simple_lock::simple_lock(simple_data * pdata) :
      interlocked_long_pulse(pdata != NULL ? &pdata->m_lockedlong : NULL, 1)
   {
      
      m_spdata = pdata;
      
      //if(pdata != NULL)
      //{
      //   if(pdata->m_lockedlong == 1)
      //   {
      //      Sys(pdata->m_paceapp).wait_twf();
      //   }
      //}
   }

   simple_lock::~simple_lock()
   {
   
      if(m_plockedlong != NULL)
      {
         m_plockedlong->subtract(m_lLevel);
         m_plockedlong = NULL;
      }
      
   }
*/

   simple_data::simple_data()
   {
//      m_pmutex = new mutex();
   }
   
   simple_data::~simple_data()
   {
      //::ace::del(m_pmutex);
   }

   
//   bool simple_data::is_locked() const
//   {
//
//      return m_lockedlong > 0;
//
//   }


   void simple_data::on_update_data(int32_t iHint)
   {

   }


//   lock::lock(::data::data * pdata) :
//      interlocked_long_pulse(pdata != NULL ? &pdata->m_lockedlong : NULL, 1)
//   {
//      //if (pdata != NULL)
//      //{
//      //   if (pdata->m_lockedlong == 1 && (pdata->m_spdataParentLock.is_null() || !pdata->m_spdataParentLock->is_locked()))
//      //   {
//      //      Sys(pdata->m_paceapp).wait_twf();
//      //   }
//      //}
//   }
//
//   lock::~lock()
//   {
//   }


   data::data(::ace::application * papp) :
      object(papp)
   {
      
      m_pcontainerbase = NULL;

   }


   data::~data()
   {

      data_listener_ptra listenerptra = m_listenerptra;

      for(int32_t i = 0; i < listenerptra.get_count(); i++)
      {

         listenerptra[i]->listen(this, false);

      }

   }


   ::count data::get_data_bound_view_count() const
   {

      return  0;

   }


   ::user::impact *  data::get_data_bound_view(index iView)
   {

      return NULL;

   }


//   bool data::is_locked() const
//   {
//      return ::data::simple_data::is_locked() || (m_spdataParentLock.is_set() && m_spdataParentLock->is_locked());
//   }

   void data::on_update_data(int32_t iHint)
   {
      for(int32_t i = 0; i < m_listenerptra.get_count(); i++)
      {
         m_listenerptra[i]->on_update_data(this, iHint);
      }
   }

   void data::edit(base_edit * pbaseedit)
   {

      UNREFERENCED_PARAMETER(pbaseedit);

   }


} // namespace data




