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


      simple_ui(sp(::base::application) papp);
      virtual ~simple_ui();


      virtual string get_cred(LPCRECT lprect, string & strUsername, string & strPassword, string strToken, string strTitle);

      virtual string fontopus(LPCRECT lprect);

      void install_message_handling(::message::dispatch * pdispatch);


      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnChar);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      

      virtual void layout();


   };


} // namespace fontopus




