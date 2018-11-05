#pragma once


namespace filehandler
{


   class CLASS_DECL_SPHERE pane_view : 
      virtual public ::userex::pane_tab_view
   {
   public:
      pane_view(::aura::application * papp);
      
      void rotate();

      void on_create_view(::user::view_creator_data * pcreatordata);
      virtual void install_message_routing(::message::sender * pinterface);

      virtual void on_update(::user::impact * pSender, LPARAM lHint, ::object* pHint);
      virtual bool pre_create_window(::user::create_struct& cs);

      DECL_GEN_SIGNAL(_001OnMenuMessage);
      virtual ~pane_view();
   
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   
      DECL_GEN_SIGNAL(_001OnCreate);
   };


} // namespace filehandler


