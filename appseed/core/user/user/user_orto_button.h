#pragma once


class CLASS_DECL_CORE OrtoButton:
   virtual public ::user::wndfrm::frame::control_box_button
{
public:


   enum ETimer
   {
      TimerCheckFocus,
   };


   oswindow                            m_oswindowPreviousFocus;
   ::draw2d::region_sp                 m_spregion;


   OrtoButton(::aura::application * papp);
   virtual ~OrtoButton();


   virtual index hit_test(point point,e_element & eelement);


   virtual void _001OnDraw(::draw2d::graphics * pgraphics);
   virtual bool pre_create_window(::user::create_struct& cs);

   virtual void install_message_handling(::message::dispatch *pinterface);
   void UpdateWndRgn();
   //void SetEllipsePens(
   //     ::draw2d::pen * ppen,
   //     ::draw2d::pen * ppenSel = NULL,
   //     ::draw2d::pen * ppenFocus = NULL,
   //     ::draw2d::pen * ppenDisabled = NULL);
   //void SetEllipseBrushs(
   //     ::draw2d::brush * pbrush,
   //     ::draw2d::brush * pbrushSel = NULL,
   //     ::draw2d::brush * pbrushFocus = NULL,
   //     ::draw2d::brush * pbrushDisabled = NULL);
   //void SetTextColors(
   //     COLORREF cr,
   //     COLORREF crSel,
   //     COLORREF crFocus,
   //     COLORREF crDisabled);


   virtual void layout();


   DECL_GEN_SIGNAL(_001OnShowWindow);
   DECL_GEN_SIGNAL(_001OnLButtonDown);
   DECL_GEN_SIGNAL(_001OnLButtonUp);
   void _001OnTimer(::timer * ptimer);
   LRESULT OnAppForwardSyncMessage(WPARAM wParam,LPARAM lParam);




};



