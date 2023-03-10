#include "framework.h"
//#include "metrowin.h"

CLASS_DECL_AURA WINBOOL GetCursorPos(LPPOINT lppoint);


CLASS_DECL_AURA int_bool ui_get_cursor_pos(POINT * ppt)
{

   if (ppt == NULL)
      return FALSE;

   int_bool iRet = FALSE;

   point ptCursor;

   ::wait(Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([=, &ptCursor, &iRet]()
   {

      try
      {

         iRet = ::GetCursorPos(&ptCursor);

      }
      catch (...)
      {

      }

   })));

   if (iRet != FALSE)
   {

      *ppt = ptCursor;

   }

   return iRet;

}





extern int g_iMouse;


CLASS_DECL_AURA WINBOOL GetCursorPos(LPPOINT lppoint)
{

   lppoint->x = 0;

   lppoint->y = 0;

   if (g_iMouse < 0)
      return FALSE;


   ::wait(Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(::Windows::UI::Core::CoreDispatcherPriority::Normal,
          ref new Windows::UI::Core::DispatchedHandler([lppoint]()
   {

      Windows::Foundation::Collections::IVectorView < Windows::Devices::Input::PointerDevice ^ > ^ deva = ::Windows::Devices::Input::PointerDevice::GetPointerDevices();

      for (unsigned int ui = 0; ui < deva->Size; ui++)
      {

         Windows::Devices::Input::PointerDevice ^ dev = deva->GetAt(ui);

         if (dev->PointerDeviceType == ::Windows::Devices::Input::PointerDeviceType::Mouse)
         {

            Windows::UI::Input::PointerPoint ^ pointerPoint = Windows::UI::Input::PointerPoint::GetCurrentPoint(g_iMouse);

            lppoint->x = (LONG)pointerPoint->RawPosition.X;

            lppoint->y = (LONG)pointerPoint->RawPosition.Y;

         }

      }

   })));

   return TRUE;

}




CLASS_DECL_AURA void defer_dock_application(bool bDock)
{

   UNREFERENCED_PARAMETER(bDock);

}



