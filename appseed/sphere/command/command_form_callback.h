#pragma once

namespace command
{

   class CLASS_DECL_sphere form_callback :
      virtual public ::user::form_callback
   {
   public:
      form_callback();
      virtual ~form_callback();

      void on_update(::user::form * pview, sp(::view) pSender, LPARAM lHint, ::ca::object* phint);
      virtual bool BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent);
   };

} // namespace command