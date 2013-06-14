#include "framework.h"

simple_tab_view::simple_tab_view(sp(::ca2::application) papp) :
   ca2(papp),
   
   ::user::interaction(papp), 
   ::user::tab(papp),
   place_holder_container(papp)
{
   m_pguie = this;
}

simple_tab_view::~simple_tab_view()
{
}


// simple_tab_view diagnostics

#ifdef DEBUG
void simple_tab_view::assert_valid() const
{
   ::user::view::assert_valid();
}

#ifndef _WIN32_WCE
void simple_tab_view::dump(dump_context & dumpcontext) const
{
   ::user::view::dump(dumpcontext);
}
#endif
#endif //DEBUG

void simple_tab_view::layout()
{
   ::user::tab::layout();
}








void simple_tab_view::GetClientRect(LPRECT lprect)
{
   ::user::view::GetClientRect(lprect);
}


void simple_tab_view::_001OnCreate(::ca2::signal_object * pobj)
{
   SCAST_PTR(::ca2::message::create, pcreate, pobj)
   if(pcreate->previous())
      return;
   
   if(!::user::control::create(this, 1))
   {
      pcreate->set_lresult(-1);
      return;
   }
}

void simple_tab_view::_001OnDraw(::ca2::graphics * pdc)
{
   ::user::tab::_001OnDraw(pdc);
}

void simple_tab_view::install_message_handling(::ca2::message::dispatch * pinterface)
{
   ::user::view::install_message_handling(pinterface);
   ::user::tab::install_message_handling(pinterface);
}


void simple_tab_view::_001OnLButtonUp(::ca2::signal_object * pobj)
{
   SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
   ::user::tab::_001OnLButtonUp(pobj);
   if(pmouse->m_bRet)
      return;
 // trans  virtual_user_interface::_001OnLButtonUp(pobj);
   if(pmouse->m_bRet)
      return;
}

void simple_tab_view::_001OnMouseMove(::ca2::signal_object * pobj)
{
   SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
   ::user::tab::_001OnMouseMove(pobj);
   if(pmouse->m_bRet)
      return;
// trans   virtual_user_interface::_001OnMouseMove(pobj);
   if(pmouse->m_bRet)
      return;
}

void simple_tab_view::_001OnMouseLeave(::ca2::signal_object * pobj)
{
   SCAST_PTR(::ca2::message::mouse, pmouse, pobj)
   ::user::tab::_001OnMouseLeave(pobj);
   if(pmouse->m_bRet)
      return;
// trans   virtual_user_interface::_001OnMouseLeave(pobj);
   if(pmouse->m_bRet)
      return;
}
