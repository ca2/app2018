#pragma once


class CLASS_DECL_ca2 menu_view :
   virtual public form_view
{
public:
   menu_view(::ca::application * papp);

   void on_update(::view * pSender, LPARAM lHint, ::gen::object* phint);
   virtual bool BaseOnControlEvent(::user::control_event * pevent);
   DECL_GEN_SIGNAL(_001OnCreate)
   DECL_GEN_SIGNAL(_001OnTimer)
   DECL_GEN_SIGNAL(_001OnUser123)
   virtual void install_message_handling(::gen::message::dispatch * pinterface);
};