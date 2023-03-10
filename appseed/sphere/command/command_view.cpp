#include "framework.h"


namespace prompt
{


   view::view(::aura::application * papp) :
      ::object(papp),
      ::user::interaction(papp)
   {

   }


   view::~view()
   {

   }


   void view::install_message_routing(::message::sender * pinterface)
   {

      ::user::impact::install_message_routing(pinterface);

	   IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &view::_001OnDestroy);
	   IGUI_MSG_LINK(WM_SIZE, pinterface, this, &view::_001OnSize);
	   IGUI_MSG_LINK(WM_PAINT, pinterface, this, &view::_001OnPaint);
	   IGUI_MSG_LINK(WM_CREATE, pinterface, this, &view::_001OnCreate);
	   IGUI_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &view::_001OnContextMenu);
	   IGUI_MSG_LINK(WM_SETCURSOR, pinterface, this, &view::_001OnSetCursor);

   //   IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &::user::interaction::_001OnLButtonDown);
      //IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &::user::interaction::_001OnLButtonUp);
      //IGUI_MSG_LINK(WM_KEYDOWN, pinterface, this, &::user::interaction::_001OnKeyDown);
      //IGUI_MSG_LINK(WM_KEYUP, pinterface, this, &::user::interaction::_001OnKeyUp);

      IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &view::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &view::_001OnLButtonUp);
      IGUI_MSG_LINK(WM_RBUTTONUP, pinterface, this, &view::_001OnRButtonUp);
//      

      IGUI_MSG_LINK(MessageOp, pinterface, this, &view::_001OnOp);
      IGUI_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &view::_001OnShowWindow);




   }
   /////////////////////////////////////////////////////////////////////////////
   // view drawing

   void view::OnDraw(::draw2d::graphics * pgraphics)
   {
      UNREFERENCED_PARAMETER(pgraphics);
   }

   /////////////////////////////////////////////////////////////////////////////
   // view diagnostics


   void view::assert_valid() const
   {
	   ::user::impact::assert_valid();
   }

   void view::dump(dump_context & dumpcontext) const
   {
	   ::user::impact::dump(dumpcontext);
   }


   /////////////////////////////////////////////////////////////////////////////
   // view message handlers

   // vmpLightView.cpp : implementation of the view class
   //


   /////////////////////////////////////////////////////////////////////////////
   // view


   bool view::pre_create_window(::user::create_struct& cs)
   {
      
      cs.style &= ~WS_EX_CLIENTEDGE;

	   return ::user::impact::pre_create_window(cs);

   }


   void view::_001OnInitialUpdate()
   {
      ::user::impact::_001OnInitialUpdate();
   }


   void view::on_update(::user::impact * pSender, LPARAM lHint, ::object* phint)
   {
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(phint);
      if(lHint == 5432108)
      {
         SetTimer(5432108, 100, NULL);
      }

   }

   void view::_001OnDestroy(::message::message * pobj)
   {
	   ::user::impact::_001OnDestroy(pobj);
   }

   void view::_001OnSize(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      rect rectDesktop;
      
      best_monitor(rectDesktop);

      rect rectClient;
      GetClientRect(rectClient);
      rectClient.deflate(2, 2);
      int32_t iW = rectClient.width() / 2;
      int32_t iH = rectClient.height() / 2;
      iH = MIN(iH, 120);
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

   void view::_001OnPaint(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }


   void view:: _001OnDraw(::draw2d::graphics * pgraphics)
   {
      
      UNREFERENCED_PARAMETER(pgraphics);

   }


   void view::_001OnCreate(::message::message * pobj)
   {
      if(pobj->previous())
         return;
      SetTimer(543218, 1000, NULL);
   }


   void view::_001OnContextMenu(::message::message * pobj)
   {
//      SCAST_PTR(::message::context_menu, pcontextmenu, pobj);
//      point point = pcontextmenu->GetPoint();


   }


   void view::_001OnTabClick(int32_t iTab)
   {
      if(iTab == 1)
      {
         Application.simple_message_box(NULL, "Playlist");
      }
   }

   void view::_001OnSetCursor(::message::message * pobj)
   {
   
      SCAST_PTR(::message::mouse, pmouse, pobj);
   
      pmouse->m_ecursor = ::visual::cursor_arrow;

	   pobj->previous();

   }

   
   ::user::document * view::get_document()
   {

      return ::user::impact::get_document();

   }


   void view::_001OnLButtonDown(::message::message * pobj)
   {
   //   SCAST_PTR(::message::mouse, pmouse, pobj);
      UNREFERENCED_PARAMETER(pobj);

   }

   void view::_001OnLButtonUp(::message::message * pobj)
   {
      //SCAST_PTR(::message::mouse, pmouse, pobj);
      UNREFERENCED_PARAMETER(pobj);
      KillTimer(5432180);


   }

   void view::_001OnRButtonUp(::message::message * pobj)
   {
      //SCAST_PTR(::message::mouse, pmouse, pobj);
      UNREFERENCED_PARAMETER(pobj);

   }

   void view::_001OnOp(::message::message * pobj)
   {
      //SCAST_PTR(::message::base, pbase, pobj);
      UNREFERENCED_PARAMETER(pobj);
   }


   void view::_001OnTimer(::timer * ptimer)
   {
      
      ::user::impact::_001OnTimer(ptimer);
      
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
         strWeekDay = "ter??a-feira";
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
         strWeekDay = "s??bado";
         break;
      }
      strTime.Format("%02d:%02d:%02d",
         time.GetHour(),
         time.GetMinute(),
         time.GetSecond());
      strDateTime.Format("%s %s %s", strDate, strWeekDay, strTime);
   }

   void view::_001OnShowWindow(::message::message * pobj)
   {
   //   SCAST_PTR(::message::show_window, pshowwindow, pobj);
      UNREFERENCED_PARAMETER(pobj);
   }


} // namespace prompt

