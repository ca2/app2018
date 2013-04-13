#include "framework.h"


simple_tree_view::simple_tree_view(sp(::ca::application) papp) :
   ca(papp),
   ::user::view(papp),
   ::user::scroll_view(papp),
   user::tree(papp)
{
}

simple_tree_view::~simple_tree_view()
{
}

void simple_tree_view::install_message_handling(::ca::message::dispatch * pinterface)
{
   ::user::view::install_message_handling(pinterface);
   ::user::tree::install_message_handling(pinterface);
   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &simple_tree_view::_001OnCreate);
}




#ifdef DEBUG
void simple_tree_view::assert_valid() const
{
   ::user::view::assert_valid();
}

void simple_tree_view::dump(dump_context & dumpcontext) const
{
   ::user::view::dump(dumpcontext);
}
#endif //DEBUG


void simple_tree_view::_001OnCreate(::ca::signal_object * pobj)
{

   SCAST_PTR(::ca::message::create, pcreate, pobj);

   pcreate->previous();


}



