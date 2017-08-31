#pragma once


namespace simple_ui
{


   class CLASS_DECL_BASE tap :
      virtual public interaction
   {
   public:


      bool                 m_bAutoStockIconPenWidth;
      double               m_dStockIconPenWidth;
      e_stock_icon         m_estockicon;
      bool                 m_bDown;
      bool                 m_bMouseMove;


      tap(::aura::application * papp);
      virtual ~tap();

      virtual void install_message_routing(::message::sender * psender);
      virtual void _001OnDraw(::draw2d::graphics * pgraphics);

      virtual void simple_ui_draw_simple(::draw2d::graphics * pgraphics);
      virtual void simple_ui_draw_volume(::draw2d::graphics * pgraphics);

      virtual void simple_ui_draw_text(::draw2d::graphics * pgraphics);


      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnKeyDown);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);


      virtual bool keyboard_focus_is_focusable();
      virtual bool is_hover();

      virtual void on_layout();


      virtual void _000OnDraw(::draw2d::graphics * pgraphics);
      virtual void _001DrawThis(::draw2d::graphics * pgraphics);
      virtual void _001CallOnDraw(::draw2d::graphics * pgraphics);

   };




} // namespace simple_ui




