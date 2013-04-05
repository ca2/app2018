#include "framework.h"


namespace ca
{


   data_listener::data_listener(::ca::applicationsp papp) :
      ca(papp)
   {

   }


   data_listener::~data_listener()
   {

      listen(::null(), false);

   }


   bool data_listener::listen(data * pdata, bool bListen)
   {

      if(bListen)
      {

         if(pdata == ::null())
            return false;

         pdata->m_listenerptra.add_unique(this);

         m_dataptra.add_unique(pdata);

         return true;

      }
      else
      {

         if(pdata == ::null())
         {

            for(int32_t i = 0; i < m_dataptra.get_count(); i++)
            {

               m_dataptra[i]->m_listenerptra.remove(this);

            }

            m_dataptra.remove_all();

            return true;

         }

         pdata->m_listenerptra.remove(this);

         m_dataptra.remove(pdata);

         return true;

      }

   }


   void data_listener::on_update_data(data *, int32_t)
   {

   }


} // namespace ca




