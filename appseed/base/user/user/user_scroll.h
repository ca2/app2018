#pragma once


//#include "base/user/user/user_interaction.h"


namespace user
{


   class scroll_bar;


   class CLASS_DECL_BASE scroll_data
   {
   public:


      LONG        m_iPage;
      LONG        m_iLine;
      bool        m_bScroll;
      int32_t     m_iWidth;
      bool        m_bScrollEnable;


   };


   class CLASS_DECL_BASE scroll_x :
      virtual public interaction
   {
   public:


      sp(scroll_bar)    m_pscrollbarHorz;
      scroll_data       m_scrolldataHorz;


      scroll_x();
      virtual ~scroll_x();


      virtual void install_message_routing(::message::sender * pinterface) override;

      virtual void send_xscroll_message(int nSBCode);

      //virtual void GetScrollRect(LPRECT lprect);
      virtual void on_change_view_size() override;
      virtual void on_change_viewport_offset() override;
      virtual void create_x_scroll_bar(const RECT & rect);
      virtual void layout_scroll_bar() override;
      virtual void _001DeferCreateXScrollBar();
      virtual void _001OnDeferCreateXScrollBar();
      virtual bool validate_viewport_offset(point & pt) override;
      virtual void scroll_left_line();
      virtual void scroll_right_line();
      virtual void scroll_left_page();
      virtual void scroll_right_page();
      virtual void scroll_horz(int nPos);


      DECL_GEN_SIGNAL(_001OnHScroll);

      virtual int get_final_x_scroll_bar_width() override;

   };























   class CLASS_DECL_BASE scroll_y :
      virtual public interaction
   {
   public:

      //int                  m_iVScrollOffset;
      sp(scroll_bar)       m_pscrollbarVert;
      scroll_data          m_scrolldataVert;
      int16_t              m_iWheelDelta;
      int16_t              m_iWheelDeltaScroll;


      scroll_y();
      virtual ~scroll_y();


      virtual void send_yscroll_message(int nSBCode);

      virtual void install_message_routing(::message::sender * pinterface) override;


      //virtual void GetScrollRect(LPRECT lprect);
      virtual void on_change_view_size() override;
      virtual void on_change_viewport_offset() override;
      virtual int32_t get_wheel_scroll_delta();
      virtual void create_y_scroll_bar(const RECT & rect);
      virtual void layout_scroll_bar() override;
      virtual void _001DeferCreateYScrollBar();
      virtual void _001OnDeferCreateYScrollBar();
      virtual bool validate_viewport_offset(point & p) override;


      DECL_GEN_SIGNAL(_001OnVScroll);
      DECL_GEN_SIGNAL(_001OnMouseWheel);


      virtual int get_final_y_scroll_bar_width() override;

      virtual void scroll_up_line();
      virtual void scroll_down_line();
      virtual void scroll_up_page();
      virtual void scroll_down_page();
      virtual void scroll_vert(int nPos);


   };

   class CLASS_DECL_BASE scroll :
      virtual public ::user::scroll_x,
      virtual public ::user::scroll_y
   {
   public:


      ::size            m_sizeTotal;


      scroll();
      scroll(::aura::application * papp);
      virtual ~scroll();



      virtual void on_change_view_size() override;
      virtual void on_change_viewport_offset() override;
      virtual bool validate_viewport_offset(point & p) override;
      void layout_scroll_bar() override;

      virtual void install_message_routing(::message::sender * pinterface) override;

      //void GetScrollRect(LPRECT lprect);

      virtual bool GetClientRect(LPRECT lprect) override;

      virtual bool GetActiveClientRect(LPRECT lprect);

      virtual bool GetFocusRect(LPRECT lprect);


      virtual ::size get_total_size() override;

      virtual void defer_draw_scroll_gap(::draw2d::graphics * pgraphics);

   };


} // namespace user
























