#include "StdAfx.h"

namespace user
{

   scroll_bar::scroll_bar()
   {
      m_scrollinfo.nMin    = 0;
      m_scrollinfo.nMax    = 100;
      m_scrollinfo.nPage   = 10;
      m_scrollinfo.nPos    = 0;
   }

   scroll_bar::~scroll_bar()
   {
   }


   void scroll_bar::send_scroll_message(UINT nSBCode)
   {
      ::user::win::message::scroll * pscroll = new ::user::win::message::scroll(get_app());
      pscroll->m_nSBCode = nSBCode;
      if(m_bTracking)
      {
         pscroll->m_nPos = m_scrollinfo.nTrackPos;
      }
      else
      {
         pscroll->m_nPos = m_scrollinfo.nPos;
      }
      if(m_eorientation == orientation_horizontal)
      {
         pscroll->m_uiMessage = WM_HSCROLL;
      }
      else
      {
         pscroll->m_uiMessage = WM_VSCROLL;
      }
      ::user::interaction * pParentWnd = GetParent();
      pParentWnd->SendMessage(pscroll);
   }

   BOOL scroll_bar::create(e_orientation eorientation, DWORD dwStyle, rect &rect, ::user::interaction *pParentWnd, UINT nID)
   {
       m_eorientation = eorientation;
       return ::user::interaction::create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
   }


} // namespace user