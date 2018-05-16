#pragma once


class CLASS_DECL_CORE simple_child_frame :
   public simple_frame_window
{
public:
   
   
   simple_child_frame(::aura::application * papp);
   virtual ~simple_child_frame();

   
   virtual void route_command_message(::user::command * pcommand) override; 
   virtual bool pre_create_window(::user::create_struct& cs) override;


   virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;

   virtual bool get_translucency(::user::e_translucency & etranslucency);

};

