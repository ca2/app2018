#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE form :
      virtual public ::html_form_view,
      virtual public ::filemanager::impact
   {
   public:
      
      
      
      form(::aura::application * papp);
      
      
      virtual void install_message_routing(::message::sender * pinterface);


      void on_update(::user::impact * pSender,LPARAM lHint,object* phint);
      virtual void on_control_event(::user::control_event * pevent);


   };



} // namespace filemanager



















