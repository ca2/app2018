#include "framework.h"


namespace command
{


   view::view(sp(::ca2::application) papp) :
      ca2(papp),
      ::user::interaction(papp)
   {
   }

   view::~view()
   {
   }

   void view::install_message_handling(::ca2::message::dispatch * pinterface)
   {
      ::user::view::install_message_handling(pinterface);

	   IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &view::_001OnDestroy);
	   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &view::_001OnSize);
	   IGUI_WIN_MSG_LINK(WM_PAINT, pinterface, this, &view::_001OnPaint);
	   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &view::_001OnCreate);
	   IGUI_WIN_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &view::_001OnContextMenu);
	   IGUI_WIN_MSG_LINK(WM_SETCURSOR, pinterface, this, &view::_001OnSetCursor);

   //   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &::user::interaction::_001OnLButtonDown);
      //IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &::user::interaction::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_KEYDOWN, pinterface, this, &::user::interaction::_001OnKeyDown);
      IGUI_WIN_MSG_LINK(WM_KEYUP, pinterface, this, &::user::interaction::_001OnKeyUp);

      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &view::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &view::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_RBUTTONUP, pinterface, this, &view::_001OnRButtonUp);
      IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &view::_001OnTimer);

      IGUI_WIN_MSG_LINK(MessageOp, pinterface, this, &view::_001OnOp);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &view::_001OnShowWindow);




   }
   /////////////////////////////////////////////////////////////////////////////
   // view drawing

   void view::OnDraw(::draw2d::graphics * pdcScreen)
   {
      UNREFERENCED_PARAMETER(pdcScreen);
   }

   /////////////////////////////////////////////////////////////////////////////
   // view diagnostics

   #ifdef DEBUG
   void view::assert_valid() const
   {
	   ::user::view::assert_valid();
   }

   void view::dump(dump_context & dumpcontext) const
   {
	   ::user::view::dump(dumpcontext);
   }
   #endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // view message handlers

   // vmpLightView.cpp : implementation of the view class
   //


   /////////////////////////////////////////////////////////////////////////////
   // view


   bool view::pre_create_window(CREATESTRUCT& cs)
   {
      
      cs.style &= ~WS_EX_CLIENTEDGE;

	   return ::user::view::pre_create_window(cs);

   }


   void view::_001OnInitialUpdate(::ca2::signal_object * pobj)
   {
      ::user::view::_001OnInitialUpdate(pobj);
   }


   void view::on_update(sp(::user::view) pSender, LPARAM lHint, ::ca2::object* phint)
   {
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(phint);
      if(lHint == 5432108)
      {
         SetTimer(5432108, 100, NULL);
      }

   }

   void view::_001OnDestroy(::ca2::signal_object * pobj)
   {
	   ::user::view::_001OnDestroy(pobj);
   }

   void view::_001OnSize(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   //   SCAST_PTR(::ca2::message::size, psize, pobj)

      rect rectDesktop;
      Application.get_screen_rect(rectDesktop);

      rect rectClient;
      GetClientRect(rectClient);
      rectClient.deflate(2, 2);
      int32_t iW = rectClient.width() / 2;
      int32_t iH = rectClient.height() / 2;
      iH = min(iH, 120);
      double r = (double) iW / (double) iH;
      double rScreen = (double) rectDesktop.width() / (double) rectDesktop.height();
      if(r < rScreen)
      {
         iH = (int32_t) (iW / rScreen);
      }
      else if(r > rScreen)
      {
         iW = (int32_t) (iH * rScreen);
      }
   }

   void view::_001OnPaint(::ca2::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   void view:: _001OnDraw(::draw2d::graphics * pdc)
   {
      UNREFERENCED_PARAMETER(pdc);
   }

   void view::_001OnCreate(::ca2::signal_object * pobj)
   {
      if(pobj->previous())
         return;
      SetTimer(543218, 1000, NULL);
   }


   void view::_001OnContextMenu(::ca2::signal_object * pobj)
   {
//      SCAST_PTR(::ca2::message::context_menu, pcontextmenu, pobj)
//      point point = pcontextmenu->GetPoint();


   }


   void view::_001OnTabClick(int32_t iTab)
   {
      if(iTab == 1)
      {
         Application.simple_message_box(NULL, "Playlist");
      }
   }

   void view::_001OnSetCursor(::ca2::signal_object * pobj)
   {
   
      SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
   
      pmouse->m_ecursor = ::visual::cursor_arrow;

	   pobj->previous();

   }

   sp(::user::document) view::get_document()
   {
      return  (::user::view::get_document());
   }



   void view::_001OnLButtonDown(::ca2::signal_object * pobj)
   {
   //   SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
      UNREFERENCED_PARAMETER(pobj);

   }

   void view::_001OnLButtonUp(::ca2::signal_object * pobj)
   {
      //SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
      UNREFERENCED_PARAMETER(pobj);
      KillTimer(5432180);


   }

   void view::_001OnRButtonUp(::ca2::signal_object * pobj)
   {
      //SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
      UNREFERENCED_PARAMETER(pobj);

   }

   void view::_001OnOp(::ca2::signal_object * pobj)
   {
      //SCAST_PTR(::ca2::message::base, pbase, pobj)
      UNREFERENCED_PARAMETER(pobj);
   }


   void view::_001OnTimer(::ca2::signal_object * pobj)
   {
      //SCAST_PTR(::ca2::message::timer, ptimer, pobj)
      UNREFERENCED_PARAMETER(pobj);
   }

   void view::GetDateTime(string & strDateTime)
   {
      ::datetime::time time = ::datetime::time::get_current_time();
      string strDate;
      string strWeekDay;
      string strTime;
      strDate.Format(
         "%04d-%02d-%02d",
         time.GetYear(),
         time.GetMonth(),
         time.GetDay());
      switch(time.GetDayOfWeek())
      {
      case 1:
         strWeekDay = "domingo";
         break;
      case 2:
         strWeekDay = "segunda-feira";
         break;
      case 3:
         strWeekDay = "ter�a-feira";
         break;
      case 4:
         strWeekDay = "quarta-feira";
         break;
      case 5:
         strWeekDay = "quinta-feira";
         break;
      case 6:
         strWeekDay = "sexta-feira";
         break;
      case 7:
         strWeekDay = "s�bado";
         break;
      }
      strTime.Format("%02d:%02d:%02d",
         time.GetHour(),
         time.GetMinute(),
         time.GetSecond());
      strDateTime.Format("%s %s %s", strDate, strWeekDay, strTime);
   }

   void view::_001OnShowWindow(::ca2::signal_object * pobj)
   {
   //   SCAST_PTR(::ca2::message::show_window, pshowwindow, pobj)
      UNREFERENCED_PARAMETER(pobj);
   }


} // namespace command

