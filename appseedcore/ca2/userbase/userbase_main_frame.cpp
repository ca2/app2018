#include "StdAfx.h"


namespace userbase
{

   main_frame::main_frame(::ca::application * papp) :
      ca(papp),
      window_frame::WorkSetClientInterface(papp),
      userbase::frame_window_interface(papp),
      userbase::frame_window(papp),
      simple_frame_window(papp)
   {
      m_bPlacing = false;
   }

   main_frame::~main_frame()
   {
   }



#ifdef _DEBUG
   void main_frame::assert_valid() const
   {
      simple_frame_window::assert_valid();
   }

   void main_frame::dump(dump_context & dumpcontext) const
   {
      simple_frame_window::dump(dumpcontext);
   }
#endif //_DEBUG

   void main_frame::install_message_handling(::user::win::message::dispatch * pinterface)
   {
      simple_frame_window::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE,         pinterface, this, &main_frame::_001OnCreate);
   }

   void main_frame::_001OnCreate(gen::signal_object * pobj)
   {

//      SCAST_PTR(::user::win::message::create, pcreate, pobj)
      
      m_bCustomFrame = !Application.directrix().m_varTopicQuery["client_only"].is_set();

      if(pobj->previous())
         return;

      //if(!m_bPlacing)
      //{
      //   keeper < bool > keepPlacing(&m_bPlacing, true, false, true);
      //   Bergedge.place(this);
      //}

   }


} // namespace userbase

