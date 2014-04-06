#include "framework.h"


namespace user
{


   namespace uinteraction
   {


      namespace frame
      {


         WorkSetClientInterface::WorkSetClientInterface()
         {
            m_workset.set_app(m_pbaseapp);
         }

         WorkSetClientInterface::~WorkSetClientInterface()
         {

         }

         sp(::user::interaction) WorkSetClientInterface::WfiGetWindow()
         {
            return  (this);
         }

         void WorkSetClientInterface::WfiOnClose()
         {

         }


         void WorkSetClientInterface::WfiOnMaximize()
         {


         }


         void WorkSetClientInterface::WfiOnFullScreen()
         {

         }


         void WorkSetClientInterface::WfiOnNotifyIcon()
         {
         }

         void WorkSetClientInterface::WfiOnMinimize()
         {

         }

         void WorkSetClientInterface::WfiOnRestore()
         {

         }

         void WorkSetClientInterface::WfiOnUp()
         {

         }

         void WorkSetClientInterface::WfiOnDown()
         {

         }


         bool WorkSetClientInterface::WfiClose()
         {

            if(!WfiOnBeforeClose())
               return false;

            WfiOnClose();

            WfiOnAfterClose();

            return true;

         }


         bool WorkSetClientInterface::WfiRestore()
         {

            ::user::EAppearance eappearanceRestore;

            if(m_workset.GetAppearance() == AppearanceIconic)
            {

               eappearanceRestore = m_eappearanceBefore;

            }
            else
            {

               eappearanceRestore = AppearanceNormal;

            }

            switch(eappearanceRestore)
            {
            case AppearanceZoomed:
               return WfiMaximize();
            case AppearanceIconic:
               return WfiMinimize();
            case AppearanceFullScreen:
               return WfiFullScreen();
            case AppearanceNotifyIcon:
               return WfiNotifyIcon();
            case AppearanceUp:
               return WfiUp();
            case AppearanceDown:
               return WfiDown();
            default:
               break;
            }

            if(!WfiOnBeforeRestore())
               return false;

            m_workset.SetAppearance(AppearanceNormal);

            WfiOnRestore();

            WfiOnAfterRestore();

            return true;

         }


         bool WorkSetClientInterface::WfiMaximize()
         {

            if(!WfiOnBeforeMaximize())
               return false;

            m_workset.SetAppearance(AppearanceZoomed);

            WfiOnMaximize();

            WfiOnAfterMaximize();

            return true;

         }


         bool WorkSetClientInterface::WfiFullScreen()
         {

            if(!m_workset.IsFullScreenEnabled())
               return false;

            if(!WfiOnBeforeFullScreen())
               return false;

            m_eappearanceBefore = m_workset.GetAppearance();

            m_workset.SetAppearance(AppearanceFullScreen);

            WfiOnFullScreen();

            WfiOnAfterFullScreen();

            return true;

         }

         bool WorkSetClientInterface::WfiNotifyIcon()
         {

            if(!m_workset.IsNotifyIconEnabled())
               return false;

            if(!WfiOnBeforeNotifyIcon())
               return false;

            m_workset.SetAppearance(AppearanceNotifyIcon);

            WfiOnNotifyIcon();

            WfiOnAfterNotifyIcon();

            return true;

         }


         bool WorkSetClientInterface::WfiMinimize()
         {

            if(!WfiOnBeforeMinimize())
               return false;

            m_eappearanceBefore = m_workset.GetAppearance();

            m_workset.SetAppearance(AppearanceIconic);

            WfiOnMinimize();

            WfiOnAfterMinimize();

            return true;

         }


         bool WorkSetClientInterface::WfiDown()
         {

            if(!WfiOnBeforeDown())
               return false;

            m_workset.SetAppearance(AppearanceDown);

            WfiOnDown();

            WfiOnAfterDown();

            return true;

         }


         bool WorkSetClientInterface::WfiUp()
         {

            if(!WfiOnBeforeUp())
               return false;

            m_workset.SetAppearance(AppearanceUp);

            WfiOnUp();

            WfiOnAfterUp();

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeClose()
         {

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeFullScreen()
         {

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeMaximize()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeMinimize()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeRestore()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeNotifyIcon()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeUp()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeDown()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         void WorkSetClientInterface::WfiOnAfterClose()
         {

         }

         void WorkSetClientInterface::WfiOnExitFullScreen()
         {

         }


         void WorkSetClientInterface::WfiOnAfterFullScreen()
         {

            m_workset.m_pframeschema->get_control_box()->ShowWindow(SW_HIDE);

            WindowDataSaveWindowRect();

         }


         void WorkSetClientInterface::WfiOnAfterNotifyIcon()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterMaximize()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterMinimize()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterRestore()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterUp()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterDown()
         {
            WindowDataSaveWindowRect();
         }

         bool WorkSetClientInterface::WfiOnBeginMoving(point pt)
         {
            UNREFERENCED_PARAMETER(pt);
            return false;
         }

         bool WorkSetClientInterface::WfiOnBeginSizing(UINT nType, point pt)
         {
            UNREFERENCED_PARAMETER(nType);
            UNREFERENCED_PARAMETER(pt);
            return false;
         }

         bool WorkSetClientInterface::WfiOnMove(bool bTracking)
         {
            UNREFERENCED_PARAMETER(bTracking);
            return false;
         }

         bool WorkSetClientInterface::WfiOnSize(bool bTracking)
         {
            UNREFERENCED_PARAMETER(bTracking);
            return false;
         }

         bool WorkSetClientInterface::WfiIsFullScreen()
         {
            return m_workset.IsFullScreen();
         }

         bool WorkSetClientInterface::WfiIsFullScreenEnabled()
         {
            return m_workset.IsFullScreenEnabled();
         }


         void WorkSetClientInterface::WfiEnableFullScreen(bool bEnable)
         {
            m_workset.EnableFullScreen(bEnable);
         }

         void WorkSetClientInterface::_001OnDraw(::draw2d::graphics * pgraphics)
         {
            m_workset._001OnDraw(pgraphics);
         }


      } // namespace frame


   } // namespace uinteraction


} // namespace user




