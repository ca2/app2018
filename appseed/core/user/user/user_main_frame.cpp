//#include "framework.h"


namespace user
{


   main_frame::main_frame(::aura::application * papp) :
      ::object(papp),
      simple_frame_window(papp)
   {
      m_bPlacing = false;
   }

   main_frame::~main_frame()
   {
   }



#ifdef DEBUG
   void main_frame::assert_valid() const
   {
      simple_frame_window::assert_valid();
   }

   void main_frame::dump(dump_context & dumpcontext) const
   {
      simple_frame_window::dump(dumpcontext);
   }
#endif //DEBUG

   void main_frame::install_message_handling(::message::dispatch * pinterface)
   {
      simple_frame_window::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE,         pinterface, this, &main_frame::_001OnCreate);
   }

   void main_frame::_001OnCreate(signal_details * pobj)
   {

//      SCAST_PTR(::message::create, pcreate, pobj);

      m_bWindowFrame = !Application.directrix()->m_varTopicQuery.has_property("client_only");

      if(pobj->previous())
         return;

      //if(!m_bPlacing)
      //{
      //   keep < bool > keepPlacing(&m_bPlacing, true, false, true);
      //   Session.place(this);
      //}

   }


} // namespace user



