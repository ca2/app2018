#include "framework.h"

const int32_t simple_list_header_control::s_iDividerWidth = 4;

simple_list_header_control::simple_list_header_control(::ca::application * papp) :
   ca(papp),
   ::user::list_header(papp)
{
   AddMessageHandling(this);
   m_etranslucency      = TranslucencyPresent;
}

simple_list_header_control::~simple_list_header_control()
{
}



void simple_list_header_control::install_message_handling(::gen::message::dispatch * pinterface)
{
   ::user::interaction::install_message_handling(pinterface);
   ::user::list_header::install_message_handling(pinterface);
#ifdef WINDOWSEX
   IGUI_WIN_MSG_LINK(HDN_ENDTRACK, pinterface, this, &simple_list_header_control::_001OnEndTrack);
   IGUI_WIN_MSG_LINK(HDN_TRACK, pinterface, this, &simple_list_header_control::_001OnTrack);
   IGUI_WIN_MSG_LINK(HDN_ENDDRAG, pinterface, this, &simple_list_header_control::_001OnEndDrag);
#endif
   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &simple_list_header_control::_001OnCreate);
   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &simple_list_header_control::_001OnLButtonDown);
   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &simple_list_header_control::_001OnLButtonUp);
   IGUI_WIN_MSG_LINK(WM_MOVE, pinterface, this, &simple_list_header_control::_001OnMove);
   IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &simple_list_header_control::_001OnMouseMove);
}

/*int32_t simple_list_header_control::GetItemWidth(int32_t iItem)
{
   HDITEM hditem;

   hditem.mask = HDI_WIDTH | HDI_TEXT;

   char pszText[1024];

   hditem.pszText = pszText;
   hditem.cchTextMax = 1024;

   get_item(iItem, &hditem);

   return hditem.cxy;

}*/

void simple_list_header_control::_001OnEndTrack(gen::signal_object * pobj)
{
#ifdef WINDOWSEX
   SCAST_PTR(::gen::message::notify, pnotify, pobj)
#else
   throw todo(get_app());
#endif
   /*CTransparentWndInterface * ptwi = NULL;
   get_parent()->SendMessage(
      WM_APP_GET_TRANSPARENT_INTERFACE, (WPARAM) &ptwi, 0);
   if(ptwi != NULL)
   {
      ::user::interaction * plist = dynamic_cast<::user::interaction *>(ptwi);
      if(plist != NULL)
      {
         plist->PostMessage(::user::list::MESSAGE_ENDCOLUMNHEADERTRACK);
      }
   }*/

   get_parent()->send_message(::user::list::MESSAGE_ENDCOLUMNHEADERTRACK);
#ifdef WINDOWSEX
   pnotify->m_bRet = false;
#else
   throw todo(get_app());
#endif
}

void simple_list_header_control::_001OnTrack(gen::signal_object * pobj)
{
#ifdef WINDOWSEX
   SCAST_PTR(::gen::message::notify, pnotify, pobj)
#else
   throw todo(get_app());
#endif

/*   CTransparentWndInterface * ptwi = NULL;
   get_parent()->SendMessage(
      WM_APP_GET_TRANSPARENT_INTERFACE, (WPARAM) &ptwi, 0);
   if(ptwi != NULL)
   {
      ::user::interaction * plist = dynamic_cast<::user::interaction *>(ptwi);
      if(plist != NULL)
      {
         plist->PostMessage(::user::list::MESSAGE_COLUMNHEADERTRACK);
      }
   }*/

   get_parent()->send_message(::user::list::MESSAGE_COLUMNHEADERTRACK);
#ifdef WINDOWSEX
   pnotify->m_bRet = false;
#else
   throw todo(get_app());
#endif
}


void simple_list_header_control::_001OnCreate(gen::signal_object * pobj) 
{
   SCAST_PTR(::gen::message::create, pcreate, pobj)
//   LOGFONT lf;

   ::user::list_header::m_font->operator = (*System.visual().font_central().GetListCtrlFont());
   
   pcreate->m_bRet = false;
}


void simple_list_header_control::_001OnEndDrag(gen::signal_object * pobj)
{
#ifdef WINDOWSEX
   SCAST_PTR(::gen::message::notify, pnotify, pobj)
#else
   throw todo(get_app());
#endif
//   LPNMHEADER lpnmhd = (LPNMHEADER) pnotify->get_lpnmhdr();
   get_parent()->send_message(::user::list::MESSAGE_ENDCOLUMNHEADERDRAG);
/*   CTransparentWndInterface * ptwi = NULL;
   get_parent()->SendMessage(
      WM_APP_GET_TRANSPARENT_INTERFACE, (WPARAM) &ptwi, 0);
   if(ptwi != NULL)
   {
      ::user::interaction * plist = dynamic_cast<::user::interaction *>(ptwi);
      if(plist != NULL)
      {
         plist->PostMessage(::user::list::MESSAGE_ENDCOLUMNHEADERDRAG);
      }*/
/*      if(plist != NULL)
      {
         HDITEM hditem;

         hditem.mask = HDI_ORDER;

         hditem.iOrder = lpnmhd->iItem;
         ASSERT(lpnmhd->pitem->mask & HDI_ORDER);
         SetItem(
            lpnmhd->pitem->iOrder, 
            &hditem);

         plist->OnEndColumnDrag();
         *pResult = TRUE;
         return;
      }*/
      
//   }
#ifdef WINDOWSEX
   pnotify->set_lresult(FALSE);
   pnotify->m_bRet = false;
#else
   throw todo(get_app());
#endif
}

/*void simple_list_header_control::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
   LPNMHDDISPINFO lpdi = (LPNMHDDISPINFO) pNMHDR;
   CTransparentWndInterface * ptwi = NULL;
   CTransparentWndInterface::CGetProperty getp;
   getp.m_eproperty = CTransparentWndInterface::PropertyInterface;
   get_parent()->SendMessage(CTransparentWndInterface::MessageGetProperty, 0, (LPARAM) &getp);
   ptwi = getp.m_pinterface;
   if(ptwi != NULL)
   {
      ::user::list * plist = dynamic_cast<::user::list *>(ptwi);
      if(plist != NULL)
      {
         if(lpdi->mask & HDI_TEXT
            && lpdi->iItem >= 0 &&
            lpdi->iItem < plist->_001GetColumnCount())
         {
            string & str = m_strBuffer;
            str.load_string(plist->_001GetColumnTextId(lpdi->iItem));
            lpdi->pszText = (LPTSTR) (const char *) str;
            lpdi->cchTextMax = str.get_length();
         }*/
/*         if(lpdi->mask & HDI_TEXT
            && lpdi->lParam >= 0 &&
            lpdi->lParam < plist->_001GetColumnCount())
         {
            string & str = m_strBuffer;
            str.load_string(plist->_001GetColumnTextId(lpdi->lParam));
            lpdi->pszText = (LPTSTR) (const char *) str;
            lpdi->cchTextMax = str.get_length();
         }
      }
   }

}*/

/*LPARAM simple_list_header_control::GetItemLParam(int32_t iItem)
{
   HDITEM hditem;

   hditem.mask = HDI_LPARAM;

   get_item(iItem, &hditem);

   return hditem.lParam;
}*/

bool simple_list_header_control::create(UINT nStyle, LPCRECT lpcrect, ::user::interaction *pwndParent, UINT nID)
{
   return ::user::interaction::create(NULL, NULL, nStyle, *lpcrect, pwndParent, nID) != 0;
}


void simple_list_header_control::_001OnMove(gen::signal_object * pobj) 
{
   SCAST_PTR(::gen::message::move, pmove, pobj)
   point point(pmove->m_pt.x, pmove->m_pt.y);
   pmove->m_bRet = false;
}

