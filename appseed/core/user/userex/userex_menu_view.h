#pragma once


class CLASS_DECL_CORE menu_view :
   virtual public ::user::form_view
{
public:


   menu_view(::aura::application * papp);


   void on_update(::user::impact * pSender, LPARAM lHint, object* phint);
   virtual void on_control_event(::user::control_event * pevent);
   DECL_GEN_SIGNAL(_001OnCreate);
   void _001OnTimer(::timer * ptimer);
   DECL_GEN_SIGNAL(_001OnUser123);
   virtual void install_message_routing(::message::sender * pinterface);


};
















