#pragma once


namespace prompt
{


   class CLASS_DECL_SPHERE form_callback :
      virtual public ::user::form_callback
   {
   public:


      form_callback();
      virtual ~form_callback();


      void on_update(::user::form * pview, sp(::user::impact) pSender, LPARAM lHint, ::object* phint);
      virtual bool BaseOnControlEvent(::user::control_event * pevent);


   };


} // namespace prompt





















