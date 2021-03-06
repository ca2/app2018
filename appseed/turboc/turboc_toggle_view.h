#pragma once


namespace turboc
{


   class CLASS_DECL_TURBOC toggle_view:
      virtual public ::aura::impact,
      virtual public ::user::button
   {
   public:


      top_view * m_ptopview;


      toggle_view(::aura::application * papp);
      virtual ~toggle_view();


      virtual void install_message_handling(::message::dispatch * pdispatch);


      virtual void on_update(::aura::impact * pSender,LPARAM lHint,object* phint);


      virtual bool keyboard_focus_is_focusable();


   };


} // namespace turboc




