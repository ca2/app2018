//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"
//#include "aura/user/colorertake5/colorertake5.h"


namespace simple_ui
{


   edit_box::edit_box(::aura::application * papp) :
      element(papp),
      ::user::interaction(papp),
      ::data::listener(papp),
      ::user::edit_plain_text(papp)
   {
      m_peditor = new ::colorertake5::base_editor(papp);
      m_plines = new ::colorertake5::text_lines();

      m_iPos = 0;

   }


   edit_box::~edit_box()
   {
      delete m_peditor;
      delete m_plines;
   }

   void edit_box::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::edit_plain_text::install_message_handling(pdispatch);

      IGUI_WIN_MSG_LINK(WM_KEYDOWN, pdispatch, this, &edit_box::_001OnKeyDown);
      IGUI_LBUTTONDOWN(edit_box);
      IGUI_LBUTTONUP(edit_box);


   }


   void edit_box::_001OnLButtonDown(signal_details * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);

//      SCAST_PTR(::message::mouse,pmouse,pobj);

  //    keyboard_set_focus();

//      pmouse->m_bRet = true;

   }


   void edit_box::_001OnLButtonUp(signal_details * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);


//      SCAST_PTR(::message::mouse,pmouse,pobj);

      //pmouse->m_bRet = true;

   }



   bool edit_box::keyboard_focus_is_focusable()
   {

      return true;

   }


   /*

   CLASS_DECL_BASE char to_upper(int32_t ch)
   {

   if(ch >= 'a' && ch <= 'z')
   {
   ch = ch - 'a' + 'A';
   }

   return ch;

   }


   CLASS_DECL_BASE char * to_upper(char * pchParam)
   {
   char * pch = pchParam;
   while(*pch == '\0')
   {
   *pch = to_upper(*pch);
   pch++;
   }
   return pchParam;
   }


   */


   void edit_box::_001OnDraw(::draw2d::graphics * pgraphics)
   {


      ::user::edit_plain_text::_001OnDraw(pgraphics);


   }

   void edit_box::_001OnKeyDown(signal_details * pobj)
   {

      return;


   }


} // namespace simple_ui
