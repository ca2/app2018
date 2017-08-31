#pragma once


#include "base/user/user/user_plain_edit.h"


namespace simple_ui
{


   class CLASS_DECL_BASE edit_box :
      virtual public ::user::plain_edit
   {
   public:


      int_ptr         m_iPos;


      edit_box(::aura::application * papp);
      virtual ~edit_box();


      virtual void install_message_routing(::message::sender * psender);

      
      virtual void _001OnDraw(::draw2d::graphics * pgraphics);


      DECL_GEN_SIGNAL(_001OnKeyDown);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);

      virtual bool keyboard_focus_is_focusable();


   };



} // namespace simple_ui



