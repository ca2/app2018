#pragma once


#include "user_style_coord.h"
#include "user_style_rect.h"
#include "user_const.h"


#define SWP_CHANGEVIEWPORTOFFSET  0x100000


namespace message
{


   class key;
   class drag_and_drop;


} // namespace message


namespace user
{
   class form;
   class tooltip;



   class copydesk;
   class create_struct;
   class control_event;
   //class control_descriptor;
   class interaction_child;
   class interaction_impl;
   class interaction_spa;
   class place_holder;
   class tab;
   class tab_pane;
   class check_box;
   class menu_interaction;
   class menu_button;
   class menu_item;
   class menu_item_ptra;
   class frame_window;
   class toolbar;
   class scroll_bar;
   class split_layout;
   class style;
   class theme;
   class scroll_info;
   class window_map;
   using style_sp = sp(style);
   using theme_sp = sp(theme);
   namespace wndfrm
   {
      namespace frame
      {

         class WorkSetUpDownInterface;
      }

   }
   class system_interaction_impl;
   CLASS_DECL_AURA bool is_docking_appearance(::user::e_appearance eappearance);

}

class simple_scroll_bar;

#undef _

#define _(str) (__get_text(str))

CLASS_DECL_AURA string __get_text(string str);


namespace user
{

   class CLASS_DECL_AURA calc_size
   {
   public:


      ::draw2d::graphics *    m_pgraphics;
      size                    m_size;


   };


   class CLASS_DECL_AURA alpha_source
   {
   public:


      virtual double get_alpha(::user::interaction * puiTarget);
      virtual void on_alpha_target_initial_frame_position();


   };


}

#include "aura/user/user/user_builtin_strlangmap.h"

#include "aura/user/user/user_primitive.h"

#include "aura/user/user/user_create_struct.h"

#include "aura/user/user/user_windowing.h"
#include "aura/user/user/user_key.h"
#include "aura/user/user/user_mouse.h"
#include "aura/aura/message/message_user.h"
#include "aura/user/user/user_style_base.h"
#include "aura/user/user/user_style_composite.h"

#include "user_elemental.h"
#include "user_window_util.h"
#include "user_style.h"
#include "user_theme.h"
#include "aura/graphics/visual/visual_text_box.h"
#include "aura/graphics/visual/visual_font_list.h"
#include "aura/user/user/user_interaction_base.h"
#include "aura/user/user/user_interaction_impl_base.h"



#include "aura/user/user/user_interaction.h"
#include "aura/user/user/user_interaction_impl.h"
#include "aura/user/user/user_copydesk.h"
#include "aura/user/user/user_interaction_child.h"
#include "aura/user/user/user_scroll_info.h"
#include "aura/user/user/user_place_holder.h"
#include "aura/user/user/user_place_holder_container.h"
#include "aura/user/user/user_job.h"
#include "aura/user/user/user_frame.h"
#include "aura/user/user/user_menu_command.h"
#include "aura/user/user/user_menu_interaction.h"
#include "aura/user/user/user_menu_item.h"
#include "aura/user/user/user_menu.h"
#include "aura/user/user/user_control_event.h"
#include "aura/user/wndfrm/frame/wndfrm_frame_updowninterface.h"
#include "aura/user/user/user_tooltip.h"
#include "aura/user/user/user_window_util.h"
#include "aura/user/user/user_window_map.h"
#include "aura/user/user/user_print_job.h"
#include "aura/user/user/user_validate.h"
#include "aura/user/user/user_form.h"


#include "aura/user/user/user_prelude.h"


#include "aura/user/user/user_message_queue.h"
#include "aura/user/user/user_message_queue_listener.h"


#ifdef WINDOWSEX

#include "aura/aura/node/windows/windows_user.h"

#elif defined(MACOS)

#include "aura/aura/os/macos/macos_user.h"
#include "aura/aura/node/macos/macos_user.h"

#elif defined(LINUX)

#include "aura/aura/os/linux/linux_user.h"


#endif

