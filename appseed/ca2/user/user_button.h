#pragma once

namespace state
{
   enum e_state
   {
      unchecked,
      checked,
      tristate,
   };

   typedef flags < e_state > state;
   
} // namespace check

namespace user
{

   class CLASS_DECL_ca2 button :
      virtual public control,
      virtual public check_interface
   {
   protected:


      check::e_check                      m_echeck;
      bool                                m_bEnabled;
      istring                             m_istrButtonText;
      rect                                m_rectText;


   public:

      state::state                        m_state;
      int                                 m_iHover;
      int                                 m_iClick;
      ::user::front_end_schema::button *  m_pschema;


      button(::ca::application * papp);
      virtual ~button();

      void VirtualOnSize();

      virtual bool create_control(class control::descriptor * pdescriptor);


      virtual void install_message_handling(::gen::message::dispatch * pinterface);
      virtual bool _001IsWindowEnabled();
      virtual void _001EnableWindow(bool bEnable = true);
      virtual void _001SetCheck(check::e_check check, bool bUser);
      virtual check::e_check _001GetCheck();
      virtual void _001SetState(state::state state, bool bUser);
      virtual state::state _001GetState();

      virtual bool _001IsPressed();


      void _001OnDraw(::ca::graphics * pdc);

      virtual index get_hover();

      
      virtual void ResizeToFit();
      virtual void _001Layout();



      virtual index hit_test(point pt, e_element & eelement);

      virtual ::ca::font * _001GetFont();
      void _001SetButtonText(const char * lpcszText);
      void _001SetButtonTextId(const char * lpcszText);
      string _001GetButtonText();

      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(on_create);

   };

} // namespace user