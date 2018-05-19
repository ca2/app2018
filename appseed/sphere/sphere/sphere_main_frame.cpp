#include "framework.h"


namespace sphere
{


   main_frame::main_frame(::aura::application * papp) :
      ::object(papp),
      simple_frame_window(papp)
   {

      WfiEnableFullScreen();

   }


   main_frame::~main_frame()
   {

   }


   //bool main_frame::get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::style_context * pcontext)
   //{

   //   etranslucency = ::user::translucency_present;

   //   return true;

   //}


   //bool main_frame::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::style_context * pcontext)
   //{

   //   bool bOk = true;

   //   if (ecolor == ::user::color_text || ecolor == ::user::color_edit_text)
   //   {

   //      cr = Application.get_theme_fg_color();

   //   }
   //   else if (ecolor == ::user::color_text_selected)
   //   {

   //      cr = ARGB(255, 255, 255, 255);

   //   }
   //   else if (ecolor == ::user::color_text_highlight)
   //   {

   //      cr = ARGB(255, 55, 105, 255);

   //   }
   //   else if (ecolor == ::user::color_text_selected_highlight)
   //   {

   //      cr = ARGB(255, 102, 153, 255);

   //   }
   //   else if (ecolor == ::user::color_background
   //            || ecolor == ::user::color_edit_background
   //            || ecolor == ::user::color_toolbar_background)
   //   {


   //      cr = Application.get_theme_bg_color();

   //   }
   //   else if (ecolor == ::user::color_background_selected)
   //   {

   //      if (_001IsTranslucent())
   //      {

   //         cr = ARGB(184, 0, 0, 127);

   //      }
   //      else
   //      {

   //         cr = ARGB(255, 0, 0, 127);

   //      }

   //   }
   //   else if (ecolor == ::user::color_background_selected)
   //   {

   //      if (_001IsTranslucent())
   //      {

   //         cr = ARGB(184, 0, 0, 127);

   //      }
   //      else
   //      {

   //         cr = ARGB(255, 0, 0, 127);

   //      }

   //   }
   //   else if (ecolor == ::user::color_border)
   //   {

   //      cr = ARGB(255, 84, 90, 80);

   //   }
   //   else if (ecolor == ::user::color_border_hover)
   //   {

   //      cr = ARGB(255, 84, 190, 80);

   //   }
   //   else if (ecolor == ::user::color_text_hover)
   //   {

   //      cr = ARGB(255, 84, 184, 255);

   //   }
   //   else
   //   {

   //      bOk = false;

   //   }

   //   return bOk;

   //}


   bool main_frame::has_pending_graphical_update()
   {

      return IsWindowVisible();

   }


} // namespace sphere



