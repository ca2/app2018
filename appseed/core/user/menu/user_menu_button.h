#pragma once


namespace user
{


   class CLASS_DECL_CORE menu_button :
      virtual public button,
      virtual public menu_interaction
   {
   public:


      menu_button(menu_item * pitem);
      virtual ~menu_button();

      virtual void install_message_routing(::message::sender * pinterface);

      virtual void on_layout();

      virtual bool create_window(const RECT & rect, ::user::interaction *pparent, id id) override;

      virtual void _001DrawCheck(::draw2d::graphics * pgraphics);

      virtual void _001OnNcDraw(::draw2d::graphics * pgraphics);

      virtual void _001OnDraw(::draw2d::graphics * pgraphics);

      virtual void _001OnDrawDefault(::draw2d::graphics * pgraphics);

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnMouseMove);

      virtual void on_calc_size(calc_size * psize);

      virtual bool keyboard_focus_is_focusable() override;


   };


} // namespace user







