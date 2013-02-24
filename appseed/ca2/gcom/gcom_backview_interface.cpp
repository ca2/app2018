#include "framework.h"


namespace gcom
{

   namespace backview
   {

      Interface::Interface(::ca::application * papp) :
         ca(papp)
      {
         m_bTransferVoid   = false;
         m_dwTimerStep      = 0;
         m_pmain           = new Main(papp, this);
      }

      Interface::~Interface()
      {
         Shutdown();
      }

      void Interface::Shutdown()
      {
         m_pmain->Shutdown();
      }


      void Interface::install_message_handling(::ca::message::dispatch * pinterface)
      {
         IGUI_WIN_MSG_LINK(MessageBackView, pinterface, this, &Interface::OnBackViewMessage);
         IGUI_WIN_MSG_LINK(MessageBackViewDequeue, pinterface, this, &Interface::OnDequeueMessage);
         IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &Interface::OnWndSize);
         IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &Interface::OnWndTimer);
         IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &Interface::OnWndCreate);
      }

      bool Interface::IsFullScreen()
      {
         return false;
      }

      bool Interface::RelayWndProc(UINT message, WPARAM wparam, LPARAM lparam)
      {
         UNREFERENCED_PARAMETER(message);
         UNREFERENCED_PARAMETER(wparam);
         UNREFERENCED_PARAMETER(lparam);
         ASSERT(FALSE);
/*         switch(message)
         {
         case MessageBackView:
            OnBackViewMessage(wparam, lparam);
            return true;
         case MessageBackViewDequeue:
            GetMain().OnDequeueMessage(wparam, lparam);
            return true;
         case WM_SIZE:
            UpdateDrawingObjects();
            return false;
         default:*/
            return false;
         //}
      }

/*      bool Interface::relay_event(LPMESSAGE lpmsg)
      {
         return relay_event(lpmsg->message, lpmsg->wParam, lpmsg->lParam);
      }*/

      uint32_t d_dwLastBackViewUpdate;

      void Interface::OnBackViewMessage(ca::signal_object * pobj)
      {
         SCAST_PTR(::ca::message::base, pbase, pobj)
         switch(pbase->m_wparam)
         {
         case BackViewWparamImageChangeEvent:
            GetMain().PulseEvent((e_event) pbase->m_lparam);
            break;
         case BackViewWparamUpdateScreenRect:
            BackViewUpdateScreen((LPCRECT) pbase->m_lparam, RDW_INVALIDATE);
            delete ((LPRECT) pbase->m_lparam);
            break;
         case BackViewWparamUpdateScreenBaseRectArray:
            if(((::get_tick_count() - d_dwLastBackViewUpdate) > 23))
            {
               d_dwLastBackViewUpdate = ::get_tick_count();

               BackViewUpdateScreen(*((rect_array *) pbase->m_lparam), RDW_INVALIDATE);
            }
            delete (rect_array * ) pbase->m_lparam;
            break;
         }
         pbase->set_lresult(0);
      }

      void Interface::OnDequeueMessage(ca::signal_object * pobj)
      {
         SCAST_PTR(::ca::message::base, pbase, pobj)
         GetMain().OnDequeueMessage(pbase->m_wparam, pbase->m_lparam);
         pbase->set_lresult(0);
      }

      void Interface::OnWndSize(ca::signal_object * pobj)
      {
         SCAST_PTR(::ca::message::base, pbase, pobj)
         GetMain().m_bPendingLayout = true;
         pbase->m_bRet = false;
      }

      void Interface::OnWndTimer(ca::signal_object * pobj)
      {
         SCAST_PTR(::ca::message::timer, ptimer, pobj)
         if(m_dwTimerStep > 0 && m_dwTimerStep == ptimer->m_nIDEvent)
         {
            ImageChangePostEvent(gcom::backview::event_timer);
         }
         //else
//            if(GetWnd() != NULL)
           // {
  //             GetWnd()->KillTimer(ptimer->m_nIDEvent);
            //}
            //GetWnd().PostMessage(Interface::MessageBackViewDequeue, 18, 4);
         //}
         ptimer->m_bRet = false;
      }


      void Interface::OnWndCreate(ca::signal_object * pobj)
      {
//         SCAST_PTR(::ca::message::create, pcreate, pobj)
         if(pobj->previous())
            return;
         initialize_user_interaction();
         GetMain().UpdateDrawingObjects();
      }


      void Interface::BackViewPostMessage(WPARAM wparam, LPARAM lparam)
      {
         BackViewPostMessage(MessageBackView, wparam, lparam);
      }


      bool Interface::UpdateBuffer(LPCRECT lpcrect)
      {
         GetMain().UpdateBuffer(lpcrect);
         return true;
      }


      HENHMETAFILE Interface::BackViewGetFillingMetaFile()
      {
         InterfaceData data;
         data.signalize(InterfaceDataEnhancedMetaFile);
         BackViewGetData(data);
         return data.m_henhmetafile;
      }

      bool Interface::BackViewGetImagePlacement(const char * lpcsz, EImagePlacement & eplacement)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(eplacement);
         return false;
      }

      void Interface::UpdateDrawingObjects()
      {
         GetMain().UpdateDrawingObjects();

      }

      bool Interface::LoadNextImage(bool bSynch)
      {
         return GetMain().LoadNextImage(bSynch);
      }

      void Interface::OnImageLoaded(::ca::dib * pdib)
      {
         GetMain().OnImageLoaded(pdib);
      }

      void Interface::OnAfterImageLoaded()
      {
         GetMain().OnAfterImageLoaded();
      }


      bool Interface::RenderBuffer()
      {
         return GetMain().RenderBuffer();
      }

      void Interface::OnDestroy()
      {
         GetMain().OnDestroy();


      }

      void Interface::SetBackgroundImageChangeInterval(uint32_t dwMillis)
      {
         GetMain().SetBackgroundImageChangeInterval(dwMillis);
      }

      void Interface::ImageChangePostEvent(e_event eevent)
      {
         GetMain().ImageChangePostEvent(eevent);
      }

      ::ca::graphics & Interface::GetTransferDC()
      {
         return GetMain().GetTransferDC();
      }

      mutex & Interface::GetTransferMutex()
      {
         return GetMain().GetTransferMutex();
      }

      EImagePlacement Interface::GetCurrentImagePlacement()
      {
         return GetMain().GetCurrentImagePlacement();
      }

      void Interface::SetCurrentImagePlacement(EImagePlacement eplacement)
      {
         GetMain().SetCurrentImagePlacement(eplacement);
      }

      void Interface::GetCurrentImagePath(string &str)
      {
         GetMain().GetCurrentImagePath(str);
      }

      void Interface::Enable(bool bEnable)
      {
         GetMain().Enable(bEnable);
      }

      bool Interface::IsEnabled()
      {
         return GetMain().IsEnabled();
      }

      void Interface::BackViewRender(::ca::graphics * pdc, LPCRECT lpcrect)
      {
         class rect rect(lpcrect);
         BackViewRender(pdc, rect.left, rect.top, rect.width(), rect.height());
      }

      void Interface::BackViewRender(
         ::ca::graphics * pdc,
         int32_t x, int32_t y,
         int32_t w, int32_t h)
      {

         Main & main = GetMain();
         Graphics & graphics = main.GetGraphics();

         main.DeferCheckLayout();

         if(!main.IsInitialized())
            return;


         single_lock sl(&graphics.m_mutex4Transfer, TRUE);

         ::ca::graphics & dcTransfer = graphics.GetTransferDC();

         if(&dcTransfer == NULL)
            return;

         if(dcTransfer.get_os_data() == NULL)
            return;

         dcTransfer.SelectClipRgn(NULL);

         pdc->BitBlt(
            x, y,
            w, h,
            &dcTransfer,
            x, y,
            SRCCOPY);

      }


      COLORREF Interface::GetBackgroundColor()
      {
         //if(main.IsFullScreen())
         //   return RGB(63, 106, 150)
         return RGB(63, 150, 106);
      }

      // if the wnd interface draws something over the gcom::_backview
      // background, this function call is a feedback requested by from some
      // transition effect or visual effect in order to display to the
      // the ::fontopus::user an intereactive effect. At first design, the only needed
      // feedback is the final output ::ca::window screenshot.
      void Interface::BackViewFeedback(::ca::graphics * pdc)
      {
         UNREFERENCED_PARAMETER(pdc);
      }

      bool Interface::BackViewGetDestroy()
      {
         return false;
      }

      void Interface::BackViewGetClientRect(LPRECT lprect)
      {
         UNREFERENCED_PARAMETER(lprect);
      }

      void Interface::BackViewClientToScreen(LPRECT lprect)
      {
         UNREFERENCED_PARAMETER(lprect);
      }

      void Interface::BackViewUpdateScreen(rect_array & recta, UINT uiRedraw)
      {
         UNREFERENCED_PARAMETER(recta);
         UNREFERENCED_PARAMETER(uiRedraw);
      }

      void Interface::BackViewUpdateScreen()
      {
      }

      void Interface::BackViewUpdateScreen(LPCRECT lpcrect, UINT uiRedraw)
      {
         UNREFERENCED_PARAMETER(lpcrect);
         UNREFERENCED_PARAMETER(uiRedraw);
      }


      void Interface::BackViewPostMessage(UINT uiMessage, WPARAM wparam, LPARAM lparam)
      {
         UNREFERENCED_PARAMETER(uiMessage);
         UNREFERENCED_PARAMETER(wparam);
         UNREFERENCED_PARAMETER(lparam);
         throw "implement in derived class";
      }

      void Interface::BackViewGetData(InterfaceData & data)
      {
         UNREFERENCED_PARAMETER(data);
      }

      void Interface::BackViewSetData(InterfaceData & data)
      {
         UNREFERENCED_PARAMETER(data);
      }

      string Interface::BackViewGetNextBackgroundImagePath()
      {
         return "";
      }

      void Interface::BackViewSetCurrentBackgroundImagePath(const char * psz)
      {
         UNREFERENCED_PARAMETER(psz);
      }

      user::interaction * Interface::BackViewGetInteraction()
      {
         return dynamic_cast < user::interaction * > (this);
      }

      Main & Interface::GetMain()
      {
         return * m_pmain;
      }

      void Interface::initialize_user_interaction()
      {
      }

   } // namespace backview

} // namespace gcom




