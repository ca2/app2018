#pragma once



namespace fontopus
{


   class simple_ui :
      virtual public ::simple_ui::interaction,
      virtual public ::simple_ui::style
   {
   public:


      login                      m_login;
      bool                       m_bLButtonDown;
      POINT                      m_ptLButtonDown;
      POINT                      m_ptLButtonDownPos;

      bool                       m_bDrag;
      bool                       m_bFontopusSimpleUiLayout;
      rect                       m_rectParent;

      simple_ui **               m_psimpleuiDeferTranslate;



      simple_ui(::aura::application * papp, const string & strRequestUrl);
      virtual ~simple_ui();


      virtual string get_cred(const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle);

      virtual string do_fontopus(const RECT & rect);

      void install_message_handling(::message::dispatch * pdispatch) override;


      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnChar);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      void _001OnTimer(::timer * ptimer) override;
      

      virtual void on_layout() override;


      virtual void _000OnDraw(::draw2d::graphics * pgraphics) override;

      virtual void _001DrawChildren(::draw2d::graphics * pgraphics) override;
      
      virtual bool has_pending_graphical_update() override;

   };


} // namespace fontopus




