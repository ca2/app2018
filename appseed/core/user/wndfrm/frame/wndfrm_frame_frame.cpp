//#include "framework.h"


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         frame::frame(::aura::application * papp)
         {

//            m_typeinfoControlBox = System.type_info < MetaControlBox >();

            m_rectControlBoxMarginFullScreen.set(0, 0, 0, 0);
            m_rectControlBoxMarginZoomed.set(0, 0, 0, 0);
            m_rectControlBoxMarginNormal.set(0, 0, 0, 0);

            m_rectMarginFullScreen.set(0, 0, 0, 0);
            m_rectMarginZoomed.set(0, 0, 0, 0);
            m_rectMarginNormal.set(5, 5, 5, 5);

            m_iControlBoxRight = 0;
            m_bControlBoxAlignRight = true;
            m_bInitialControlBoxPosition = true;

         }


         frame::~frame()
         {
         }

         ::user::front_end_schema * frame::get_user_front_end_schema()
         {

            ::exception::throw_interface_only(get_app());

            return NULL;

         }

         void frame::set_style(const char * pszStyle)
         {

            UNREFERENCED_PARAMETER(pszStyle);

         }


         void frame::OnMove(sp(::user::interaction) pwnd)
         {

            UNREFERENCED_PARAMETER(pwnd);

         }


         void frame::_001OnDraw(::draw2d::graphics * pgraphics)
         {

            UNREFERENCED_PARAMETER(pgraphics);

         }


         void frame::OnAttach()
         {

            if(m_pworkset->GetSizingManager() != NULL)
            {

               m_pworkset->GetSizingManager()->SetSWPFlags(0);

            }

         }

         size frame::GetMinSize()
         {

            ::size sizeMin;

            if(!get_draw_window()->get_window_minimum_size(sizeMin))
            {

               Session.get_window_minimum_size(sizeMin);

            }

            return sizeMin;

         }

         /*
         void frame::OnNcCalcSize(LPRECT lprect)
         {

            UNREFERENCED_PARAMETER(lprect);

         }
         */

         void frame::OnNcCalcSize(LPRECT lprect)
         {

            if(get_appearance()->m_bUseNc)
            {

               calc_window_client_rect(lprect,rect(lprect));

            }

         }

         

         appearance * frame::get_appearance()
         {
            ASSERT(m_pworkset != NULL);
            return m_pworkset->get_appearance();
         }


         void frame::_000OnBeforeSize(const RECT & lpcrectWindow)
         {

            UNREFERENCED_PARAMETER(lpcrectWindow);

         }

         void frame::OnActivate()
         {
         }


         bool frame::_000OnCommand(WPARAM wparam, LPARAM lparam, LRESULT & lresult)
         {

            UNREFERENCED_PARAMETER(wparam);
            UNREFERENCED_PARAMETER(lparam);
            UNREFERENCED_PARAMETER(lresult);

            return false;

         }



         void frame::on_layout()
         {

            reset_layout();

            rect rectWindow;

            rect rectClient;

            sp(::user::interaction) pwndDraw = get_draw_window();

            synch_lock sl(pwndDraw->m_pmutex);

            sp(::user::wndfrm::frame::WorkSetClientInterface) pinterface = pwndDraw;

            title_bar_layout(pinterface != NULL && pinterface->m_bInitialFramePosition);

            update_window_client_rect();

            pwndDraw->GetWindowRect(rectWindow);

            pwndDraw->GetClientRect(rectClient);

            get_window_client_rect(rectClient);

            sp(::user::interaction) pwnd = get_window();

            ::user::frame_window * pfrmwnd = dynamic_cast < ::user::frame_window * >(pwnd.m_p);

            if (pfrmwnd != NULL)
            {
               if (pwnd->GetStyle() & FWS_SNAPTOBARS)
               {
                  rect rect(0, 0, 32767, 32767);
                  pwnd->RepositionBars(0, 0xffff, "pane_first", pwnd->reposQuery,
                     &rect, &rect, FALSE);
                  rect.offset(rectClient.top_left());
                  class rect rectBorder;
                  pfrmwnd->GetBorderRect(rectBorder);
                  pwnd->RepositionBars(0, 0xffff, "pane_first", pwnd->reposExtra,
                     &rectBorder, &rect, TRUE);
                  pfrmwnd->SetBorderRect(rectBorder);
                  pwnd->CalcWindowRect(&rect);
                  OnNcCalcSize(&rect);
                  //rect rectSnap(0, 0, 0, 0);
                  //CalcWndClient(rectSnap, rectSnap);
                  //rect.deflate(rectSnap);
                  pwnd->SetWindowPos(0, 0, 0, rect.width(), rect.height(),
                     SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

               }
               else
               {
                  rect rectBorder;
                  pfrmwnd->GetBorderRect(rectBorder);
                  pwnd->RepositionBars(0, 0xffff, "pane_first", pwnd->reposExtra, &rectBorder, &rectClient);
                  pfrmwnd->SetBorderRect(rectBorder);
               }
            }

            update_drawing_objects();

            update_window();

            update_window_region(rectWindow);

/*            if (pwnd->IsWindowVisible())
            {
               pwnd->RedrawWindow(NULL, NULL, RDW_INVALIDATE);

#if !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS)
               RedrawWindow(NULL,
                  m_pworkset->m_rectPending,
                  NULL,
                  RDW_INVALIDATE | RDW_ALLCHILDREN);
#endif

               m_pworkset->m_rectPending.set(0, 0, 0, 0);
            }

*/

         }


         COLORREF frame::get_border_main_body_color()
         {

            return 0;

         }

         /*
         bool frame::_000OnLButtonDown(::message::mouse * pmouse)
         {

            UNREFERENCED_PARAMETER(pmouse);

            return false;

         }

         bool frame::_000OnLButtonUp(::message::mouse * pmouse)
         {

            UNREFERENCED_PARAMETER(pmouse);

            return false;

         }

         bool frame::_000OnMouseMove(::message::mouse * pmouse)
         {

            UNREFERENCED_PARAMETER(pmouse);

            return false;

         }

         bool frame::_000OnNcLButtonDown(::message::mouse * pmouse)
         {

            UNREFERENCED_PARAMETER(pmouse);

            return false;

         }

         bool frame::_000OnNcLButtonUp(::message::mouse * pmouse)
         {

            UNREFERENCED_PARAMETER(pmouse);

            return false;

         }

         bool frame::_000OnNcMouseMove(::message::mouse * pmouse)
         {

            UNREFERENCED_PARAMETER(pmouse);

            return false;

         }

         bool frame::_000OnNcHitTest(point pt, LRESULT & nHitTest)
         {

            UNREFERENCED_PARAMETER(pt);
            UNREFERENCED_PARAMETER(nHitTest);

            return false;

         }
         */

         bool frame::_000OnLButtonDown(::message::mouse * pmouse)
         {

            if(m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
            {

               //if(m_pworkset->GetDockingManager()->_000OnLButtonDown(pmouse))
                 // return true;

               if(m_pworkset->GetSizingManager()->_000OnLButtonDown(pmouse))
                  return true;

               if(m_pworkset->GetMovingManager()->_000OnLButtonDown(pmouse))
                  return true;

            }

            return false;

         }

         bool frame::_000OnLButtonUp(::message::mouse * pmouse)
         {

            if(m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
            {

               if(m_pworkset->GetDockingManager()->_000OnLButtonUp(pmouse))
                  return true;

               if(m_pworkset->GetSizingManager()->_000OnLButtonUp(pmouse))
                  return true;

               if(m_pworkset->GetMovingManager()->_000OnLButtonUp(pmouse))
                  return true;

            }

            return false;
         }

         bool frame::_000OnMouseMove(::message::mouse * pmouse)
         {

            if(m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
            {

               if(!m_pworkset->GetMovingManager()->IsMoving()
               && !m_pworkset->GetSizingManager()->IsSizing()
               && m_pworkset->GetDockingManager()->_000OnMouseMove(pmouse))
                  return true;

               if(!m_pworkset->GetMovingManager()->IsMoving()
               && m_pworkset->GetSizingManager()->_000OnMouseMove(pmouse))
                  return true;

               if(m_pworkset->GetMovingManager()->_000OnMouseMove(pmouse))
                  return true;

            }

            return false;
         }

         bool frame::_000OnNcLButtonDown(::message::mouse * pmouse)
         {

            if(m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
            {

               if(m_pworkset->GetDockingManager()->_000OnLButtonDown(pmouse))
                  return true;

               if(m_pworkset->GetSizingManager()->_000OnLButtonDown(pmouse))
                  return true;

               if(m_pworkset->GetMovingManager()->_000OnLButtonDown(pmouse))
                  return true;

            }

            return false;
         }

         bool frame::_000OnNcLButtonUp(::message::mouse * pmouse)
         {

            if(m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
            {

               if(m_pworkset->GetDockingManager()->Relay(pmouse))
                  return true;

               if(m_pworkset->GetSizingManager()->Relay(pmouse))
                  return true;

               if(m_pworkset->GetMovingManager()->Relay(pmouse))
                  return true;

            }

            return false;
         }

         bool frame::_000OnNcMouseMove(::message::mouse * pmouse)
         {

            if(m_pworkset->GetAppearance() != ::user::AppearanceZoomed && m_pworkset->GetAppearance() != ::user::AppearanceFullScreen)
            {

               if(!m_pworkset->GetMovingManager()->IsMoving()
                  && !m_pworkset->GetSizingManager()->IsSizing()
                  && m_pworkset->GetDockingManager()->_000OnMouseMove(pmouse))
                  return true;

               if(!m_pworkset->GetMovingManager()->IsMoving()
                  && m_pworkset->GetSizingManager()->_000OnMouseMove(pmouse))
                  return true;

               if(m_pworkset->GetMovingManager()->_000OnMouseMove(pmouse))
                  return true;

            }

            return false;
         }

         bool frame::_000OnNcHitTest(point pt,LRESULT & nHitTest)
         {
            UNREFERENCED_PARAMETER(pt);
            UNREFERENCED_PARAMETER(nHitTest);
            return false;
         }



         bool frame::_000OnTimer(UINT nIDEvent)
         {

            UNREFERENCED_PARAMETER(nIDEvent);

            return false;

         }
         /*
         bool frame::_000OnSize(UINT nType, int32_t cx, int32_t cy)
         {

            UNREFERENCED_PARAMETER(nType);
            UNREFERENCED_PARAMETER(cx);
            UNREFERENCED_PARAMETER(cy);

            return false;

         }

         */

         bool frame::_000OnSize(uint32_t nType,int32_t cx,int32_t cy)
         {

            UNREFERENCED_PARAMETER(cx);
            UNREFERENCED_PARAMETER(cy);

            sp(::user::interaction) pwnd = get_window();

            if(pwnd == NULL || pwnd->m_pimpl->m_bIgnoreSizeEvent)
               return false;

            synch_lock sl(pwnd->m_pmutex);

            //on_layout();

            return false;

         }


         bool frame::_000OnMove(int32_t x, int32_t y)
         {

            UNREFERENCED_PARAMETER(x);
            UNREFERENCED_PARAMETER(y);

            return false;

         }


         //int32_t frame::get_margin()
         //{
         //   int32_t iMargin = m_iMargin;

         //   /*            if (get_appearance()->GetAppearance() == AppearanceZoomed
         //   || get_appearance()->GetAppearance() == AppearanceFullScreen)
         //   {
         //   iMargin = 0;
         //   }*/

         //   return iMargin;
         //}

         void frame::reset_layout()
         {

            if (m_spcontrolbox.is_set())
            {

               m_spcontrolbox->reset_layout();

            }

            m_iCaptionHeight = calc_caption_height();


            /*         m_rectControlBoxMarginFullScreen.left = 0;
                     m_rectControlBoxMarginFullScreen.top = 0;
                     m_rectControlBoxMarginFullScreen.right = 0;
                     m_rectControlBoxMarginFullScreen.bottom = 0;

                     m_rectControlBoxMarginZoomed.left = 0;
                     m_rectControlBoxMarginZoomed.top = 0;
                     m_rectControlBoxMarginZoomed.right = 0;
                     m_rectControlBoxMarginZoomed.bottom = 0;

                     m_rectControlBoxMarginNormal.left = m_iMargin + 3;
                     m_rectControlBoxMarginNormal.top = 3;
                     m_rectControlBoxMarginNormal.right = m_iMargin - 3;
                     m_rectControlBoxMarginNormal.bottom = 0;*/


         }


         int32_t frame::get_caption_height()
         {

            return m_iCaptionHeight;

         }


         int32_t frame::calc_caption_height(::user::EAppearance eappearance)
         {

            if(eappearance == AppearanceFullScreen)
               return 0;

            if(eappearance == AppearanceMinimal)
               return 0;

            rect * prectMargin = get_margin_rect();

            rect * prectControlBoxMargin = get_control_box_margin_rect();

            int32_t iMargin = prectMargin->top + prectControlBoxMargin->top + prectControlBoxMargin->bottom;

            int32_t iCaptionHeight = 0;

            iCaptionHeight = MAX(m_spcontrolbox->calc_control_box_height() + iMargin,iCaptionHeight);

            return iCaptionHeight;

         }


         int32_t frame::calc_caption_height()
         {

            return calc_caption_height(m_pworkset->get_appearance()->GetAppearance());

         }


         int32_t frame::title_bar_layout(bool bInitialControlBoxPosition)
         {

            appearance * pappearance = m_pworkset->get_appearance();

            sp(::user::interaction) pwndDraw = get_draw_window();

            rect * prectControlBoxMargin = get_control_box_margin_rect();

            rect * prectMargin = get_margin_rect();

//            int32_t iCaptionHeight = get_caption_height();

            rect rectClient;

            pwndDraw->GetWindowRect(rectClient);

            int32_t cx = rectClient.width();

            int iControlBoxHeight = m_spcontrolbox->calc_control_box_height();

            rect rect;

            m_rectCaption.left = rectClient.left + prectMargin->left + prectControlBoxMargin->left;
            m_rectCaption.top = rectClient.top + prectMargin->top + prectControlBoxMargin->top;
            m_rectCaption.right = rectClient.right - +prectMargin->right + prectControlBoxMargin->right;
            m_rectCaption.bottom = m_rectCaption.top + iControlBoxHeight;




            bool bShow = true;

            if(m_bInitialControlBoxPosition || bInitialControlBoxPosition)
            {

               m_bInitialControlBoxPosition = false;

               int iControlBoxWidthFullScreen = m_spcontrolbox->calc_control_box_full_screen_width();
               int iControlBoxWidthZoomed = m_spcontrolbox->calc_control_box_zoomed_width();
               int iControlBoxWidthNormal = m_spcontrolbox->calc_control_box_normal_width();

               m_rectControlBoxFullScreen.left = cx  - iControlBoxWidthFullScreen ;
               m_rectControlBoxFullScreen.right = cx;
               m_rectControlBoxFullScreen.top = 0;
               m_rectControlBoxFullScreen.bottom = m_rectControlBoxFullScreen.top + iControlBoxHeight;

               m_rectControlBoxZoomed.right = cx - m_rectControlBoxMarginZoomed.right - m_rectMarginZoomed.right;
               m_rectControlBoxZoomed.left = m_rectControlBoxZoomed.right - iControlBoxWidthZoomed;
               m_rectControlBoxZoomed.top = m_rectMarginZoomed.top + m_rectControlBoxMarginZoomed.top;
               m_rectControlBoxZoomed.bottom = m_rectControlBoxZoomed.top + iControlBoxHeight;

               m_rectControlBoxNormal.right = cx - m_rectControlBoxMarginNormal.right - m_rectMarginNormal.right;
               m_rectControlBoxNormal.left = m_rectControlBoxNormal.right - iControlBoxWidthNormal;
               m_rectControlBoxNormal.top = m_rectMarginNormal.top + m_rectControlBoxMarginNormal.top;
               m_rectControlBoxNormal.bottom = m_rectControlBoxNormal.top + iControlBoxHeight;

            }


            ::rect * prectControlBox;

            ::rect rectParent;

            int iControlBoxRightMargin;

            if (m_pworkset->IsFullScreen())
            {

               prectControlBox = &m_rectControlBoxFullScreen;

               pwndDraw->best_monitor(rectParent);

               bShow = !(m_bInitialControlBoxPosition || bInitialControlBoxPosition);

            }
            else if (pappearance->IsZoomed())
            {

               prectControlBox = &m_rectControlBoxZoomed;

               pwndDraw->GetWindowRect(rectParent);

            }
            else
            {

               prectControlBox = &m_rectControlBoxNormal;

               pwndDraw->GetWindowRect(rectParent);

            }

            rectParent -= rectParent.top_left();

            iControlBoxRightMargin = prectControlBoxMargin->right;

            int x;

            if (!bInitialControlBoxPosition)
            {

               if (m_bControlBoxAlignRight)
               {

                  x = rectClient.width() - m_iControlBoxRight - prectControlBox->width();

               }
               else
               {

                  x = prectControlBox->left;

               }

            }
            else
            {

               x = prectControlBox->left;

            }


            if (x > rectParent.width() - prectMargin->right - prectControlBoxMargin->right - prectControlBox->width())
               x = rectParent.width() - prectMargin->right - prectControlBoxMargin->right - prectControlBox->width();

            if (x < rectParent.left + prectMargin->left + prectControlBoxMargin->left)
               x = rectParent.left + prectMargin->left + prectControlBoxMargin->left;

            int y = prectMargin->top + prectControlBoxMargin->top;

            prectControlBox->top = y;

            prectControlBox->bottom = y + iControlBoxHeight;

            prectControlBox->right = x + prectControlBox->width();

            prectControlBox->left = x;

            ::rect rectControlBoxWindow;

            get_control_box()->GetWindowRect(rectControlBoxWindow);

            if(prectControlBox->size() != rectControlBoxWindow.size())
            {

               get_control_box()->defer_set_window_pos(
                  ZORDER_TOP,
                  prectControlBox->left,
                  prectControlBox->top,
                  prectControlBox->width(),
                  prectControlBox->height(),(bShow ? SWP_SHOWWINDOW : 0) | SWP_NOZORDER);

            }
            else
            {

               get_control_box()->defer_set_window_pos(
                  ZORDER_TOP,
                  prectControlBox->left,
                  prectControlBox->top,
                  prectControlBox->width(),
                  prectControlBox->height(), (bShow ? SWP_SHOWWINDOW : 0) | SWP_NOZORDER);

               if(get_control_box()->m_eappearance != pappearance->GetAppearance())
               {

                  get_control_box()->on_layout();

                  get_control_box()->m_eappearance = pappearance->GetAppearance();

               }

               

            }

            m_rectWindow = rectClient;

            ::rect rectIcon = ::null_rect();

            if(pappearance->GetAppearance() == ::user::AppearanceMinimal)
            {

               if(get_element_rect(rectIcon,ElementTopLeftIcon))
               {

                  m_pointWindowIcon.x = get_control_box_rect()->left - 2 - get_control_box_rect()->height();

               }
               else
               {

                  m_pointWindowIcon.x = get_control_box_rect()->left;

               }

            }
            else
            {

               get_element_rect(rectIcon, ElementTopLeftIcon);

               m_pointWindowIcon.x = prectMargin->left + prectControlBoxMargin->left + 5;

            }

            m_pointMoveGripMinimal.x = m_pointWindowIcon.x - 2 - get_control_box_rect()->height();

            m_pointMoveGripMinimal.y = prectMargin->top + prectControlBoxMargin->top;

            m_pointWindowIcon.y = prectMargin->top + prectControlBoxMargin->top;

            if (pappearance->m_picon != NULL)
            {

               m_rectWindowText.left = m_pointWindowIcon.x + rectIcon.width() + 2;

            }
            else
            {

               m_rectWindowText.left = m_pointWindowIcon.x;

            }

            m_rectWindowText.top = prectControlBox->top;
            m_rectWindowText.right = rectClient.width() - prectMargin->right;
            m_rectWindowText.bottom = prectControlBox->bottom;

            m_rectCaptionTextBk.left = prectMargin->left;
            m_rectCaptionTextBk.top = prectMargin->top;
            m_rectCaptionTextBk.right = rectClient.width() - prectMargin->right;
            m_rectCaptionTextBk.bottom = prectControlBox->bottom + prectControlBoxMargin->bottom;

            m_rectCaptionTextBk.bottom = prectControlBox->bottom + prectControlBoxMargin->bottom;

            return 0;

         }


         void frame::on_initialize_appearance()
         {

            if (m_spcontrolbox.is_null())
            {
               m_spcontrolbox = m_pinteraction->m_plibrary->create_object(get_app(), "control_box", NULL);
               m_spcontrolbox->m_pworkset = m_pworkset;
               sp(::user::interaction) pwnd = m_pworkset->get_draw_window();
               m_spcontrolbox->create_window(null_rect(), pwnd, 1);
            }

            m_spcontrolbox->update_control_box_buttons();

            on_style_change();

            update_drawing_objects();

            update_window();

         }



         sp(control_box) frame::get_control_box()
         {

            return m_spcontrolbox;

         }


         void frame::update_window_style()
         {

         }


         void frame::update_window()
         {

            update_window_style();

         }


         bool frame::get_window_client_rect(LPRECT lprect)
         {
            
            *lprect = m_rectClient;

            return true;

         }


         bool frame::get_draw_client_rect(LPRECT lprect)
         {

            rect rect;

            if(!get_window_client_rect(rect))
            {

               return false;

            }

            rect.offset(-rect.top_left());

            *lprect = rect;

            return true;

         }


         void frame::update_drawing_objects()
         {

         }


         void frame::update_window_client_rect()
         {

            sp(::user::interaction) pwnd = get_window();

            ASSERT(pwnd != NULL);

            if (pwnd == NULL)
               return;

            rect rectClient;

            pwnd->::user::interaction::GetClientRect(rectClient);

            calc_window_client_rect(m_rectClient, rectClient);

         }


         sp(::user::interaction) frame::get_draw_window()
         {

            return get_window();

         }


         sp(::user::interaction) frame::get_window()
         {
            try
            {
               return m_pworkset->GetEventWindow();
            }
            catch (...)
            {
               return NULL;
            }

         }


         void frame::update_window_region(const RECT & rect)
         {

            sp(::user::interaction) pwnd = get_window();

            if (pwnd == NULL)
               return;

            //pwnd->SetWindowRgn(NULL, TRUE);

         }


         void frame::on_style_change()
         {

         }

         void frame::calc_window_client_rect(LPRECT lprect, const RECT &  rectWindow)
         {

            rect rect(rectWindow);

            ::rect * prectMargin = get_margin_rect();

            int iTopDeflate;

            int iLeftDeflate;

            if(get_appearance()->GetAppearance() == ::user::AppearanceFullScreen)
            {

               iTopDeflate = 0;

               iLeftDeflate = 0;

            }
            else if (m_pworkset->GetWndDraw()->frame_is_transparent())
            {

               iTopDeflate = 0;

               iLeftDeflate = 0;

            }
            else if(get_appearance()->GetAppearance() == ::user::AppearanceMinimal)
            {

               iTopDeflate = prectMargin->top;

               iLeftDeflate = prectMargin->left;

            }
            else
            {

               iTopDeflate = get_caption_height();

               iLeftDeflate = prectMargin->left;

            }

            int iRightDeflate;
            
            if(get_appearance()->GetAppearance() == ::user::AppearanceMinimal)
            {

               iRightDeflate = (rect.right - get_control_box_rect()->left) + 2 ;

               ::rect rectIcon;

               if(get_element_rect(rectIcon,ElementTopLeftIcon))
               {

                  iRightDeflate += calc_caption_height(::user::AppearanceNormal);

               }

               iRightDeflate += calc_caption_height(::user::AppearanceNormal); // for the ElementMoveGripMinimal

            }
            else
            {

               iRightDeflate = prectMargin->right;

            }

            rect.deflate(iLeftDeflate,iTopDeflate,iRightDeflate, prectMargin->bottom);

            *lprect = rect;

         }


         rect * frame::get_margin_rect()
         {

            if (m_pworkset->get_appearance()->GetAppearance() == AppearanceFullScreen)
            {

               return &m_rectMarginFullScreen;

            }
            else if (m_pworkset->get_appearance()->GetAppearance() == AppearanceZoomed)
            {

               return &m_rectMarginZoomed;

            }
            else
            {

               return &m_rectMarginNormal;

            }

         }


         rect * frame::get_control_box_margin_rect()
         {

            if (m_pworkset->get_appearance()->GetAppearance() == AppearanceFullScreen)
            {

               return &m_rectControlBoxMarginFullScreen;

            }
            else if (m_pworkset->get_appearance()->GetAppearance() == AppearanceZoomed)
            {

               return &m_rectControlBoxMarginZoomed;

            }
            else
            {

               return &m_rectControlBoxMarginNormal;

            }

         }

         bool frame::get_element_rect(LPRECT lprect,e_element eelement)
         {

            return false;

         }

         rect * frame::get_control_box_rect()
         {

            if(m_pworkset->get_appearance()->GetAppearance() == AppearanceFullScreen)
            {

               return &m_rectControlBoxFullScreen;

            }
            else if(m_pworkset->get_appearance()->GetAppearance() == AppearanceZoomed)
            {

               return &m_rectControlBoxZoomed;

            }
            else
            {

               return &m_rectControlBoxNormal;

            }

         }

         EHitTest frame::_000HitTest(point pt)
         {

            UNREFERENCED_PARAMETER(pt);

            return HitTestClient;

         }


      } // namespace frame


   } // namespace frame


} // namespace user












