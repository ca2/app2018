//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


CLASS_DECL_BASE bool g_bSuppressTwf = false;


namespace user
{


   window_draw::window_draw(::aura::application * papp) :
      ::object(papp),
      thread(papp),
      m_eventFree(papp, TRUE, TRUE)
   {

      g_bSuppressTwf = file_exists_dup("C:\\ca2\\config\\system\\suppress_twf.txt") != 0;

      m_bProDevianMode = true;
      m_iFramesPerSecond = 60.0;
      m_bRunning = false;
      m_bRun = true;
      m_bRender = false;

   }


   bool window_draw::twf_start()
   {

      int iReturnCode;

      if (begin_synch(&iReturnCode))
         return false;

      return true;

   }


   bool window_draw::UpdateBuffer()
   {

      if (m_bRender)
         return false;

      keep<bool> keepRender(&m_bRender, true, false, true);



      //::user::interaction_spa wndpa(get_app());

      //wndpa = get_wnda();

      //rect rectWindow;

      //rect rect9;

      ::user::oswindow_array wndaApp;

      ::user::interaction * pui = NULL;

      while (System.get_frame(pui))
      {

         try
         {

            synch_lock sl(pui->m_pmutex);

            if (pui->oprop("session").is_new())
            {

               if (file_exists_dup("/ca2/debug_window_draw.txt"))
               {
                  TRACE("debug window draw");
               }

               if (pui->m_bMayProDevian)
               {

                  pui->defer_check_layout();

                  pui->_001UpdateWindow();

               }

            }

         }
         catch (simple_exception & se)
         {

            if (se.m_strMessage == "no more a window")
            {

               TRACE("No more a window explicitly");

               System.remove_frame(pui);

            }

         }
         catch (...)
         {

            TRACE("No more a window implicitly");

            System.remove_frame(pui);

         }

      }

      return true;

   }


   bool window_draw::ScreenOutput()
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }

   bool window_draw::twf_stop()
   {

      set_end_thread();

      return true;

   }

   void window_draw::asynch_redraw()
   {
      ::exception::throw_interface_only(get_app());
   }

   void window_draw::synch_redraw()
   {
      ::exception::throw_interface_only(get_app());
   }


   //ptr_array < ::user::interaction > window_draw::get_wnda()
   //{

   //   return System.frames();

   //}





   int32_t window_draw::run()
   {

      try
      {

         m_bRunning = true;

         //         MSG msg;
         uint64_t startTime;
         uint64_t endTime;

         while (m_bRun)
         {
            try
            {
               if (m_bProDevianMode)
               {
                  startTime = get_nanos();
                  UpdateBuffer();
                  endTime = get_nanos();
               }
            }
            catch (...)
            {
            }
            if (m_iFramesPerSecond <= 0)
            {
               Sleep(1000);
            }
            else
            {

               UINT uiFrameMillis = 1000 / m_iFramesPerSecond;

               uint64_t micros = (endTime - startTime) / 1000;

               uint64_t millis = micros / 1000;

               if (uiFrameMillis > millis + 2)
               {

                  Sleep(uiFrameMillis - millis - 2);

               }

               //char sz[512];

               //::ultoa_dup(sz, micros, 10);
               //::OutputDebugString(sz);
               //::OutputDebugString("\n");

            }

         }
         //      exit:;

      }
      catch (...)
      {

      }

      m_bRunning = false;

      return 0;

   }




} // namespace user



