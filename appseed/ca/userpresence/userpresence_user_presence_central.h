#pragma once

namespace userpresence
{

   class CLASS_DECL_ca presence_central :
      virtual public ::ca::message_window_simple_callback
   {
   public:


      presence_central(sp(::ca::application) papp);
      virtual ~presence_central();


      virtual bool initialize();
      virtual bool finalize();

      virtual bool is_initialized();

      virtual void message_window_message_handler(::ca::signal_object * pobj);

   };

} // namespace userpresence