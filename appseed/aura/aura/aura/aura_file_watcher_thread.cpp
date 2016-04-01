//#include "framework.h"


namespace file_watcher
{


   bool listener_thread::file_watcher_initialize_thread(::aura::application * papp)
   {

      set_app(papp);

//      m_pthreadimpl.alloc(allocer());
//
//      if(m_pthreadimpl.is_null())
//         return false;

      return true;

   }

   int32_t listener_thread::run()
   {


#ifndef METROWIN


      try
      {

         MESSAGE msg;

         while(true)
         {

            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
               switch(msg.message)
               {
               case WM_QUIT:
                  goto quit;
               case WM_USER + 123:
                  {

                     listener_thread::op * pop = (listener_thread::op *) msg.lParam;

                     try
                     {

                        try
                        {

                           pop->m_id = add_watch(pop->m_str, pop->m_plistener, pop->m_bRecursive, pop->m_bOwn);

                        }
                        catch(...)
                        {

                           try
                           {

                              pop->m_id = -1;

                           }
                           catch(...)
                           {

                           }

                        }

                        pop->m_event.set_event();

                     }
                     catch(...)
                     {

                     }

                  }
                  break;
               }
            }

            try
            {

//               UINT ui = GetCurrentThreadId();

               update();

            }
            catch(::file_watcher::exception &)
            {

            }




         }
quit:;

      }
      catch(...)
      {

      }
#endif

      return 0;

   }


} // namespace file_watcher




