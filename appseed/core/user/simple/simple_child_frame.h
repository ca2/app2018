#pragma once


class CLASS_DECL_CORE simple_child_frame :
   public simple_frame_window
{
public:
   
   
   simple_child_frame(::aura::application * papp);
   virtual ~simple_child_frame();

   
   virtual bool _001OnCmdMsg(::user::command * pcommand); 
   virtual bool pre_create_window(::user::create_struct& cs);


   virtual void _001OnDraw(::draw2d::graphics * pgraphics);

   virtual bool get_translucency(::user::e_translucency & etranslucency);

};

