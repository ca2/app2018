#pragma once

namespace nature
{

   class document;


   class CLASS_DECL_ca2 view :
      virtual public ::user::view
   {
   public:


      ::user::button    m_buttonKaraoke;
      ::user::button    m_buttonDevEdge;
      ::user::button    m_buttonNetShare;
      ::user::button    m_buttonHtmlApp;


      view(sp(::application) papp);
      virtual ~view();
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

      virtual sp(::user::interaction) BackViewGetWnd();
      virtual sp(::user::interaction) get_guie();

      virtual void _001OnDraw(::draw2d::graphics * pdc);

      virtual void install_message_handling(::ca2::message::dispatch * pinterface);
      virtual bool pre_create_window(CREATESTRUCT& cs);
      virtual void on_update(sp(::user::view) pSender, LPARAM lHint, object* pHint);

      sp(::user::document) get_document();

      DECL_GEN_SIGNAL(_001OnInitialUpdate)

      DECL_GEN_SIGNAL(_001OnDestroy)
      DECL_GEN_SIGNAL(_001OnSize)
      DECL_GEN_SIGNAL(_001OnPaint)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnContextMenu)
      DECL_GEN_SIGNAL(_001OnSetCursor)
      DECL_GEN_SIGNAL(_001OnEraseBkgnd)
      DECL_GEN_SIGNAL(_001OnUpdateViewEncoding)
      DECL_GEN_SIGNAL(_001OnViewEncoding)
      DECL_GEN_SIGNAL(_001OnWavePlayerEvent)

      virtual void _001OnTabClick(int32_t iTab);
      
   };


} // namespace nature