#pragma once


namespace calculator
{

   class document;


   class CLASS_DECL_sphere view :
      virtual public ::user::view
   {
   public:
      view(sp(::ca::application) papp);
      virtual ~view();

#ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
#endif

      virtual sp(::user::interaction) BackViewGetWnd();
      virtual sp(::user::interaction) get_guie();

      virtual void _001OnDraw(::ca::graphics * pdc);


      virtual void install_message_handling(::ca::message::dispatch * pinterface);
      virtual bool pre_create_window(CREATESTRUCT& cs);
      DECL_GEN_SIGNAL(_001OnInitialUpdate)
         virtual void on_update(sp(::user::view) pSender, LPARAM lHint, ::ca::object* pHint);

      sp(::user::document) get_document();

      DECL_GEN_SIGNAL(_001OnDestroy)
      DECL_GEN_SIGNAL(_001OnSize)
      DECL_GEN_SIGNAL(_001OnPaint)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnContextMenu)
      DECL_GEN_SIGNAL(_001OnSetCursor)
      DECL_GEN_SIGNAL(_001OnUpdateViewEncoding)
      DECL_GEN_SIGNAL(_001OnViewEncoding)
      DECL_GEN_SIGNAL(_001OnWavePlayerEvent)

      virtual void _001OnTabClick(int32_t iTab);

   };

} // namespace calculator