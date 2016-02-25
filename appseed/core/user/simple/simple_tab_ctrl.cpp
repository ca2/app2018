//#include "framework.h"
//#include "simple_tab_control.h"


simple_tab_control::simple_tab_control(::aura::application * papp):
   object(papp),
   ::user::interaction(papp),
   
   ::user::tab(papp),
   place_holder_container(papp)
{

}


simple_tab_control::~simple_tab_control()
{

}


void simple_tab_control::_001OnDraw(::draw2d::dib * pdib)
{
   
   ::user::tab::_001OnDraw(pdib);

}


void simple_tab_control::pre_subclass_window() 
{

   _001BaseWndInterfaceMap();

   ::user::interaction::pre_subclass_window();

}


void simple_tab_control::install_message_handling(::message::dispatch * pinterface)
{
   ::user::interaction::install_message_handling(pinterface);
   ::user::interaction_base::install_message_handling(pinterface);
}