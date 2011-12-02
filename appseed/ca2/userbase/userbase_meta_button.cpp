#include "StdAfx.h"


MetaButton::MetaButton(::ax::application * papp) :
   ax(papp),
   ::user::button(papp),
   ::userbase::button(papp),
   m_spregion(papp)
{

//   m_bFocus = false;

}

MetaButton::~MetaButton()
{
}


BOOL MetaButton::PreCreateWindow(CREATESTRUCT& cs) 
{
   cs.style |= BS_OWNERDRAW;
   return ::userbase::button::PreCreateWindow(cs);
}

void MetaButton::SetEllipseBrushs(
   ::ax::brush * pbrush,
   ::ax::brush * pbrushSel,
   ::ax::brush * pbrushFocus,
   ::ax::brush * pbrushDisabled)
{

   ASSERT(pbrush != NULL);

   if(pbrush == NULL)
      return;

   m_brushEllipse             = *pbrush;
   m_brushEllipseSel          = pbrushSel != NULL ? *pbrushSel : (pbrushFocus != NULL ? *pbrushFocus : *pbrush);
   m_brushEllipseFocus        = pbrushFocus != NULL ? *pbrushFocus : (pbrushSel != NULL ? *pbrushSel : *pbrush);
   m_brushEllipseDisabled     = pbrushDisabled != NULL ? *pbrushDisabled : *pbrush;

}

void MetaButton::SetEllipsePens(        
   ::ax::pen * ppen,
   ::ax::pen * ppenSel,
   ::ax::pen * ppenFocus,
   ::ax::pen * ppenDisabled)
{

   ASSERT(ppen != NULL);

   if(ppen == NULL)
      return;

   m_penEllipse               = *ppen;
   m_penEllipseSel            = ppenSel != NULL ? *ppenSel : (ppenFocus != NULL ? *ppenFocus : *ppen);
   m_penEllipseFocus          = ppenFocus != NULL ? *ppenFocus : (ppenSel != NULL ? *ppenSel : *ppen);
   m_penEllipseDisabled       = ppenDisabled != NULL ? *ppenDisabled : *ppen;

}

void MetaButton::SetTextColors(
   COLORREF cr,
   COLORREF crSel,
   COLORREF crFocus,
   COLORREF crDisabled)
{

   m_crText = cr;
   m_crTextSel = crSel;
   m_crTextFocus = crFocus;
   m_crTextDisabled = crDisabled;


}


void MetaButton::_001OnDraw(::ax::graphics * pdc)
{

   rect rectClient;

   ::user::interaction::GetClientRect(rectClient);

   if(rectClient.area() <= 0)
      return;

   COLORREF crText;

   if(!IsWindowEnabled())
   {

      pdc->SelectObject(&m_brushEllipseDisabled);
      pdc->SelectObject(&m_penEllipseDisabled);
      crText = m_crTextDisabled;

   }
   else if(m_iHover >= 0)
   {

      pdc->SelectObject(&m_brushEllipseSel);
      pdc->SelectObject(&m_penEllipseSel);
      crText = m_crTextSel;

   }
   /*else if(m_bFocus)
   {

      pdc->SelectObject(&m_brushEllipseFocus);
      pdc->SelectObject(&m_penEllipseFocus);
      crText = m_crTextFocus;

   }*/
   else
   {

      pdc->SelectObject(&m_brushEllipse);
      pdc->SelectObject(&m_penEllipse);
      crText = m_crText;

   }

   pdc->set_alpha_mode(::ax::alpha_mode_blend);
   pdc->DrawEllipse(rectClient);
   pdc->FillEllipse(rectClient);

   string str;
   GetWindowText(str);

   pdc->set_font(GetFont());
   pdc->set_color(crText);
   pdc->set_alpha_mode(::ax::alpha_mode_set);
   pdc->DrawText(str, rectClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


}

void MetaButton::_001OnShowWindow(gen::signal_object * pobj) 
{

   UNREFERENCED_PARAMETER(pobj);

}

void MetaButton::_001OnLButtonDown(gen::signal_object * pobj) 
{
   
   UNREFERENCED_PARAMETER(pobj);
   
}

void MetaButton::_001OnLButtonUp(gen::signal_object * pobj) 
{
   
   UNREFERENCED_PARAMETER(pobj);
   
   _001RedrawWindow();
   
}

LRESULT MetaButton::OnAppForwardSyncMessage(WPARAM wParam, LPARAM lParam)
{
   LPMSG lpmsg = (LPMSG) lParam;
   if(wParam == WM_MOUSEMOVE)
   {
      point ptCursor = lpmsg->pt;
      rect rectClient;
      ::user::interaction::GetClientRect(rectClient);
      ::user::interaction::ClientToScreen(rectClient);

      if(rectClient.contains(ptCursor))
      {
         _001RedrawWindow();
      }
      else
      {
         _001RedrawWindow();
      }
   }
   return 0;
}

void MetaButton::layout() 
{

   UpdateWndRgn();

}


void MetaButton::_001OnTimer(gen::signal_object * pobj) 
{
   SCAST_PTR(::gen::message::timer, ptimer, pobj)
      // TODO: add your message handler code here and/or call default
      if(ptimer->m_nIDEvent == TimerCheckFocus)
      {
         /*if(m_bFocus)
         {
            point ptCursor;
            System.get_cursor_pos(&ptCursor);
            rect rectClient;
            ::user::interaction::GetClientRect(rectClient);
            ::user::interaction::ClientToScreen(rectClient);

            if(!rectClient.contains(ptCursor))
            {
               KillTimer(ptimer->m_nIDEvent);
               m_bFocus = false;
               _001RedrawWindow();
            }
         }
         else
         {
            KillTimer(ptimer->m_nIDEvent);
         }*/
      }
      else
      {

         //      ::user::interaction * pwnd = ::user::interaction::GetParent();


         rect rect;
         ::user::interaction::GetClientRect(rect);
      }


}

void MetaButton::UpdateWndRgn()
{

   rect rectClient;

   ::user::interaction::GetClientRect(rectClient);

   m_spregion->CreateEllipticRgnIndirect(rectClient);


}

void MetaButton::install_message_handling(::gen::message::dispatch *pinterface)
{
   ::userbase::button::install_message_handling(pinterface);
   IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &MetaButton::_001OnShowWindow);
   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &MetaButton::_001OnLButtonDown);
   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &MetaButton::_001OnLButtonUp);
   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &MetaButton::_001OnSize); 
   IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &MetaButton::_001OnTimer);
}



int MetaButton::hit_test(point point, e_element & eelement)
{

   if(m_spregion.is_null())
   {

      eelement = element_none;
      return -1;

   }

   if(m_spregion->get_os_data() == NULL)
   {

      eelement = element_none;
      return -1;

   }

   ScreenToClient(&point);

   if(!m_spregion->PtInRegion(point))
   {

      eelement = element_none;
      return -1;
      
   }

   eelement = element_client;
   return 0;

}