#pragma once


class CLASS_DECL_ca menu_view :
   virtual public form_view
{
public:
   menu_view(::ca::application * papp);

   void on_update(::view * pSender, LPARAM lHint, ::radix::object* phint);
   virtual bool BaseOnControlEvent(::user::control_event * pevent);
   DECL_GEN_SIGNAL(_001OnCreate)
   DECL_GEN_SIGNAL(_001OnTimer)
   DECL_GEN_SIGNAL(_001OnUser123)
   virtual void _001InstallMessageHandling(::user::win::message::dispatch * pinterface);
};