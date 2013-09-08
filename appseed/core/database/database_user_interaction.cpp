#include "framework.h"

namespace database
{

   namespace user
   {

      interaction::interaction()
      {
         m_bEnableSaveWindowRect = false;
      }

      interaction::~interaction()
      {
      }

      void interaction::install_message_handling(::message::dispatch * pinterface)
      {
         ::user::interaction::install_message_handling(pinterface);
         IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &interaction::_001OnCreate);
         IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &interaction::_001OnSize);
         IGUI_WIN_MSG_LINK(WM_MOVE, pinterface, this, &interaction::_001OnMove);
         IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &interaction::_001OnShowWindow);
      }

      void interaction::_001OnCreate(signal_details * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);

         if(pobj->previous())
            return;

         set_data_server(Application.m_simpledb.get_data_server());
         if(m_dataidWindow.is_null())
         {
            m_dataidWindow = "window";
         }
         if(m_dataid.is_null())
         {
            m_dataid = m_dataidWindow;
         }
      }



      void interaction::_001OnMove(signal_details * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
      }

      void interaction::_001OnSize(signal_details * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
      }

      void interaction::_001OnShowWindow(signal_details * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
         //         SCAST_PTR(message::show_window, pshowwindow, pobj)
         if(get_parent() == NULL)
         {
#if !core_level_1 && !core_level_2 && defined(WINDOWS)
            if(GetExStyle() && WS_EX_LAYERED)
            {
               if(IsZoomed())
               {
               }
               else if(IsFullScreen())
               {
               }
               else if(IsIconic())
               {
               }
               else
               {

               }
            }
#endif
         }
      }


      void interaction::WindowDataEnableSaveWindowRect(bool bEnable)
      {
         m_bEnableSaveWindowRect = bEnable;
      }

      bool interaction::WindowDataSaveWindowRect()
      {

         single_lock sl(m_spmutex, true);

         bool bSave = false;

         if(m_bEnableSaveWindowRect)
         {

            m_strDisplay = calc_display();

            ::database::id idWindow    = m_dataidWindow;

            ::id idKey                 = "WindowRect." + m_strDisplay;

            sl.unlock();

            bSave = SaveWindowRect_(
               idWindow,
               idKey,
               this);
         }

         return true;

      }


      bool interaction::WindowDataLoadWindowRect(bool bForceRestore)
      {

         single_lock sl(m_spmutex, true);

         bool bLoad = false;

         keeper < bool > keepEnable(&m_bEnableSaveWindowRect, false, m_bEnableSaveWindowRect, true);

         m_strDisplay = calc_display();

         ::database::id idWindow       = m_dataidWindow;

         ::id idKey                    = "WindowRect." + m_strDisplay;

         sl.unlock();

         bLoad = LoadWindowRect_(idWindow, idKey, this, bForceRestore);

         return bLoad;

      }


      bool interaction::IsFullScreen()
      {

         return false;

      }


      bool interaction::LoadWindowRect_(::database::id key, ::database::id idIndex, sp(::user::interaction) pWnd, bool bForceRestore)
      {

         ::file::byte_stream_memory_buffer memstream(get_app());

         if(!data_get(key, idIndex, memstream))
            return false;

         memstream.seek_to_begin();

         bool bZoomed = false;

         memstream >> bZoomed;

         if(!bForceRestore && bZoomed)
         {

            pWnd->ShowWindow(SW_MAXIMIZE);

         }

         bool bFullScreen = false;

         memstream >> bFullScreen;

         if(!bForceRestore)
         {

            if(bFullScreen)
            {
               if(!pWnd->IsFullScreen())
               {
                  pWnd->ShowWindowFullScreen(true, false);
               }
            }
            else
            {
               if(pWnd->IsFullScreen())
               {
                  pWnd->ShowWindowFullScreen(false, false);
               }
            }

         }
         else if(!bFullScreen)
         {

            if(pWnd->IsFullScreen())
            {
               pWnd->ShowWindowFullScreen(false, false);
            }

         }

         bool bIconic = false;

         memstream >> bIconic;

         if(!bForceRestore && bIconic)
         {

            pWnd->ShowWindow(SW_MINIMIZE);

         }

         if(bForceRestore)
         {

            pWnd->ShowWindow(SW_RESTORE);

         }

         if(bForceRestore || (!bZoomed && !bFullScreen && !bIconic))
         {

            rect rectWindow;

            memstream >> rectWindow;

            rect rectDesktop;
            if(get_parent() != NULL)
            {
               get_parent()->GetClientRect(rectDesktop);
               get_parent()->ScreenToClient(rectWindow);
            }
            else
            {
               System.get_screen_rect(rectDesktop);
            }
            rect rectIntersect;
            rectIntersect.intersect(rectDesktop, rectWindow);
            if(rectIntersect.width() < rectDesktop.width() / 64
               || rectIntersect.height() < rectDesktop.height() / 64
               ||  rectDesktop.width() * 2 / 5 < 100
               ||  rectDesktop.height() * 2 / 5 < 100)
            {
               SetWindowPos(
                  -3,
                  rectDesktop.left + rectDesktop.width() / 7,
                  rectDesktop.top + rectDesktop.height() / 7,
                  rectDesktop.width() * 2 / 5,
                  rectDesktop.height() * 2 / 5, 0);
            }
            else
            {
               SetWindowPos(
                  -3,
                  rectWindow.left,
                  rectWindow.top,
                  rectWindow.width(),
                  rectWindow.height(),
                  0);
            }


            //pWnd->SetWindowPos(0, rect.left, rect.top, rect.width(), rect.height(), SWP_NOZORDER | SWP_NOACTIVATE);

         }

         return true;

      }


      bool interaction::SaveWindowRect_(::database::id key, ::database::id idIndex,  sp(::user::interaction) pWnd)
      {
         //WINDOWPLACEMENT wp;
         //pWnd->GetWindowPlacement(&wp);
         ::file::byte_stream_memory_buffer memstream(get_app());
         ::file::byte_stream_memory_buffer memstreamGet(get_app());
         bool bGet = data_get(
            key,
            idIndex,
            memstreamGet);
         memstreamGet.seek_to_begin();
         bool bZoomedOld = false;
         bool bFullScreenOld = false;
         bool bIconicOld = false;
         rect rectOld;
         if(bGet)
         {
            memstreamGet >> bZoomedOld;
            memstreamGet >> bFullScreenOld;
            memstreamGet >> bIconicOld;
            memstreamGet >> rectOld;
         }
         bool bZoomed = pWnd->IsZoomed() != FALSE;
         memstream << bZoomed;
         bool bFullScreen = pWnd->IsFullScreen();
         memstream << bFullScreen;
         bool bIconic = pWnd->IsIconic();
         memstream << bIconic;
         if(bGet && (bZoomed || bFullScreen || bIconic))
         {
            memstream << &rectOld;
         }
         else
         {
            rect rect;
            pWnd->GetWindowRect(rect);
            memstream << &rect;
         }
         return data_set(key, idIndex, memstream);
      }

      void interaction::_001WindowRestore()
      {
         WindowDataLoadWindowRect(true);
      }


      string interaction::calc_display()
      {

         string strDisplay;

         rect rectScreen;

         Session.get_screen_rect(rectScreen);

         strDisplay.Format("Display(%d, %d)", rectScreen.width(), rectScreen.height());

         return strDisplay;

      }

      bool interaction::does_display_match()
      {

         single_lock sl(m_spmutex, true);

         return m_strDisplay == calc_display();

      }

      bool interaction::on_simple_command(e_simple_command ecommand, lparam lparam, LRESULT & lresult)
      {

         if(::user::interaction::on_simple_command(ecommand, lparam, lresult))
            return true;

         switch(ecommand)
         {
         case simple_command_load_window_rect:
            WindowDataLoadWindowRect(lparam != FALSE);
            break;
         default:
            break;
         }


         return false;

      }


   } // namespace user


} //namespace database

















