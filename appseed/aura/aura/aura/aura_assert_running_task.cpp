#include "framework.h"


namespace aura
{


   assert_running_task::assert_running_task(::aura::application * papp,const string & strAppId):
      object(papp),
      thread(papp)
   {

      m_strAppId = strAppId;

      m_durationCheckPeriod = millis(184 + 1951 + 1977);

   }


   assert_running_task::~assert_running_task()
   {

   }


   void assert_running_task::run()
   {

      while(thread_get_run())
      {

         try
         {

            Session.assert_running(m_strAppId);

         }
         catch(::exit_exception &)
         {

            ::multithreading::post_quit(&System);

         }
         catch(::exception::exception &)
         {

         }
         catch(...)
         {

         }

         Sleep(m_durationCheckPeriod);

      }

//      return 0;

   }


} // namespace aura



















































