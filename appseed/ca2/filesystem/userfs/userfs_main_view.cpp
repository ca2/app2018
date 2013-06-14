#include "framework.h"


namespace userfs
{


   main_view::main_view(sp(::ca2::application) papp) :
      ca2(papp),
      ::user::split_layout(papp),
      
      ::user::split_view(papp),
      place_holder_container(papp)
   {
   }

   void main_view::install_message_handling(::ca2::message::dispatch * pinterface)
   {
      ::user::split_view::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &main_view::_001OnCreate);
   }

   void main_view::_001OnCreate(::ca2::signal_object * pobj)
   {
      pobj->previous();
      if(pobj->m_bRet)
         return;

//      sp(application) papp =  (get_app());
      //papp->m_pmainview = this;
      SetPaneCount(2);
      SetSplitOrientation(orientation_vertical);
      set_position_rate(0, 0.2);
      sp(::ca2::create_context) cc(allocer());

//      cc->m_usercreatecontext.m_pCurrentDoc = get_document();
  //    cc->m_usercreatecontext.m_typeinfoNewView = System.type_info < tree > ();
      sp(::user::interaction) pwnd = create_view(System.type_info < tree > (), get_document(), this, 100);
      SetPane(0, pwnd, false);
//      sp(tree) ptree =  (pwnd);


//      cc->m_usercreatecontext.m_pCurrentDoc = get_document();
  //    cc->m_usercreatecontext.m_typeinfoNewView = System.type_info < list > ();
      pwnd = create_view(System.type_info < list > (), get_document(), this, 101);
//      list * plist = dynamic_cast < list * > (pwnd);
      SetPane(1, pwnd, false);
      layout();
      SetTimer(123, 1000, NULL);


   }

   void main_view::_001OnTimer(::ca2::signal_object * pobj)
   {

      SCAST_PTR(::ca2::message::timer, ptimer, pobj);
      if(ptimer->m_nIDEvent == 123)
      {
//         Application.TimerStep();
      }

   }

   sp(::user::document) main_view::get_document()
   {
      return  (::user::split_view::get_document());
   }


} // namespace userfs






