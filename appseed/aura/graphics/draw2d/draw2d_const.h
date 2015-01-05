#pragma once

enum e_stock_icon
{

   stock_icon_none,
   stock_icon_control_box_begin,
   // TO Begin - TIGHTLY ORDERED Begin (relations : {ca2/app/core}::user::wndfrm::frame::e_button[button_close:stock_icon_close,...])
   stock_icon_close =  stock_icon_control_box_begin,
   stock_icon_level_up,
   stock_icon_level_down,
   stock_icon_iconify,
   stock_icon_restore,
   stock_icon_zoom,
   stock_icon_notify,
   stock_icon_dock,
   // TO END - TIGHTLY ORDERED End
   stock_icon_control_box_end = stock_icon_dock
};

namespace draw2d
{


   class object;
   class pen;
   class graphics;
   class bitmap;
   class brush;
   class font;
   class palette;
   class region;
   // backlink from ca.dll on 2012-10-14 11:04 Alameda Princesa Izabel, 3000 - CWB-PR-Brasil

   enum e_alpha_mode
   {
      alpha_mode_set,
      alpha_mode_blend
   };

   enum e_fill_mode
   {
      fill_mode_winding,
      fill_mode_alternate
   };


} // namespace draw2d
