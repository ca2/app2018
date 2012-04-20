#pragma once


class CLASS_DECL_ca2 simple_button :
   public ::userbase::button
{
public:
   
   
   bool                 m_bTransparent;
   ::ca::brush_sp       m_brushBkg;
   bool                 m_bFocus;


   simple_button(::ca::application * papp);
   virtual ~simple_button();



   virtual void pre_subclass_window();
   virtual void install_message_handling(::gen::message::dispatch * pinterface);

   virtual void _001OnDraw(::ca::graphics * pdc);

   virtual void ResizeToFit();

   DECL_GEN_SIGNAL(_001OnCtlColor)
   DECL_GEN_SIGNAL(_001OnSetFocus)


};



