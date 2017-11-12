#pragma once


namespace helloworld
{


   class CLASS_DECL_APP_CORE_HELLOMULTIVERSE main_frame :
      virtual public simple_frame_window
   {
      public:


         bool                       m_bExplicitTranslucency;


         main_frame(::aura::application * papp);
         virtual ~main_frame();


         virtual void assert_valid() const;
         virtual void dump(dump_context & dumpcontext) const;


         using ::simple_frame_window::get_translucency;
         virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pui);
         virtual sp(::user::wndfrm::frame::frame) create_frame_schema();

         virtual bool has_pending_graphical_update();

   };


} // namespace helloworld












