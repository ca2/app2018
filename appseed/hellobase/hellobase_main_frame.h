#pragma once


namespace hellobase
{


   class CLASS_DECL_APP_HELLO_BASE main_frame :
      virtual public ::user::frame_window
   {
   public:


      bool                       m_bExplicitTranslucency;


      main_frame(::aura::application * papp);
      virtual ~main_frame();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      using ::user::frame_window::get_translucency;
      virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pui);

      virtual bool has_pending_graphical_update();

   };


} // namespace hellobase












