#pragma once


namespace user
{


   class CLASS_DECL_BASE form_interface:
      virtual public ::user::scroll_view
   {
   public:


      form_interface(::aura::application * papp);
      virtual ~form_interface();


      using ::user::control::_001IsPointInside;
      virtual bool _001IsPointInside(sp(control) pcontrol,point64 pt);

      virtual void control_get_window_rect(sp(control) pcontrol,LPRECT lprect);
      virtual void control_get_client_rect(sp(control) pcontrol,LPRECT lprect);


      virtual bool open_document(var varFile);


   };


} // namespace user


