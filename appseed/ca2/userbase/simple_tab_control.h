#pragma once

class CLASS_DECL_ca2 simple_tab_control :
   virtual public ::user::interaction,
   virtual public ::user::tab
{
public:


   simple_tab_control(sp(::ca::application) papp);
   virtual ~simple_tab_control();


   virtual void pre_subclass_window();

   virtual void install_message_handling(::ca::message::dispatch * pinterface);

   virtual void _001OnDraw(::ca::graphics * pdc);

};

