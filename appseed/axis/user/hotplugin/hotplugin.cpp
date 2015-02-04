//#include "framework.h"
//#include "base/user/user.h"
//#include "hotplugin.h"


namespace hotplugin
{

   ::base::system * g_pbasesystem = NULL;

   int g_iSystemCount = 0;

   uint32_t c_cdecl base_system_main(LPVOID lpVoid);


   CLASS_DECL_BASE ::base::system * get_base_system()
   {
      
      return g_pbasesystem;

   }

   CLASS_DECL_BASE bool defer_start_base_system()
   {

      if(g_pbasesystem != NULL)
         return true;

      g_iSystemCount++;

      try
      {

         g_pbasesystem = new ::base::system(NULL);

         if(file_exists_dup("C:\\ca2\\config\\plugin\\npca2_beg_debug_box.txt"))
         {

            debug_box("hotplugin boxmain NP_Initialize","ZZZzzz hotplugin box",MB_OK);

         }

         ::set_thread(g_pbasesystem);

         g_pbasesystem->m_bMatterFromHttpCache = true;

         g_pbasesystem->m_bSystemSynchronizedCursor = false;

         g_pbasesystem->m_bShouldInitializeGTwf = false;

         g_pbasesystem->m_bEnableOnDemandDrawing = false;

         g_pbasesystem->construct(NULL);

#ifdef WINDOWS

         g_pbasesystem->m_hinstance = (HINSTANCE)get_hinstance();

#endif

         xxdebug_box("box1","box1",MB_ICONINFORMATION);

#ifdef WINDOWS

         set_main_thread((HTHREAD) GetCurrentThread());

         set_main_thread_id(GetCurrentThreadId());

#endif

         g_pbasesystem->m_bReady = false;

         ::create_thread(NULL,0,&base_system_main,NULL,0,NULL);
         
      }
      catch(...)
      {

         return false;

         // debug_box("failed to create system", "npca2", 0);
         //      return NULL;
      }


      return true;

   }

   uint32_t c_cdecl base_system_main(LPVOID lpVoid)
   {

      int32_t iReturnCode = 0;

      try
      {

         if(!g_pbasesystem->pre_run())
         {

            if(g_pbasesystem->m_iReturnCode == 0)
            {

               g_pbasesystem->m_iReturnCode = -1;

            }

            g_pbasesystem->m_bReady = true;

            return -1;

         }

      }
      catch(...)
      {

         if(g_pbasesystem->m_iReturnCode == 0)
         {

            g_pbasesystem->m_iReturnCode = -1;

         }

         g_pbasesystem->m_bReady = true;

         return -1;

      }

      return g_pbasesystem->main();

   }

#ifdef WINDOWS

   HINSTANCE g_hinstance = NULL; // hotplugin plugin dll HINSTANCE

   CLASS_DECL_BASE HINSTANCE get_hinstance()
   {

      return g_hinstance;

   }

   CLASS_DECL_BASE void set_hinstance(HINSTANCE hinstance)
   {

      g_hinstance = hinstance;

   }

#endif


   CLASS_DECL_BASE void defer_stop_base_system()
   {

      g_iSystemCount--;

      if(g_iSystemCount == 0)
      {

         if(g_pbasesystem != NULL)
         {

            g_pbasesystem->post_thread_message(WM_QUIT);

            g_pbasesystem = NULL;

         }

         __wait_threading_count(minutes(1));

      }

   }

} // namespace hotplugin