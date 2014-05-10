#pragma once


namespace user
{


   class CLASS_DECL_CORE split_view :
      virtual public impact,
      virtual public split_layout
   {
   public:


      split_view(sp(::base::application) papp);
      virtual ~split_view();


      virtual void install_message_handling(::message::dispatch * pinterface);
      virtual void _001OnDraw(::draw2d::graphics * pdc);
      virtual bool pre_create_window(CREATESTRUCT& cs);
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

      virtual void on_update(sp(::user::impact) pSender, LPARAM lHint, object* pHint);
      virtual bool create_views();
      virtual void on_create_views();

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnShowWindow);


   };


} // namespace user



