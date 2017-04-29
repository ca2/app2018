#pragma once



#include "axis/axis/axis/axis.h"

#if defined(__cplusplus) || defined(__cplusplus_winrt)

#include "aura/user/user.h"


#define new AURA_NEW

namespace user
{


   class interaction;
   class create_struct;
   class control_event;

   class menu_base_item;
   class menu;
   class menu_base;

   class schema;
   class tree;
   class plain_text_group_command;


   class tooltip;



} // namespace user


namespace simple_ui
{

   class label;
   class edit_box;
   class password;
   class tap;

}




namespace user
{

   class control_event;
   class create_context;
   class printer;
   class user;
   class frame_window;

   //   class form_interface;
   class form_list;
   class form_callback;
   class form_view;


} // namespace user

namespace user
{

   class interaction;
   class interaction_impl;
   class view_creator_data;
   class schema;
   class tree;
   class str_context;
   class schema_simple_impl;
   class window_draw;
   class str;

}


namespace user
{

   class interaction;
   class control_event;
   class create_context;
   class user;
   class frame_window;

   //   class form_interface;
   class form_list;
   class form_callback;


} // namespace user


namespace simple_ui
{

   class label;
   class edit_box;
   class password;
   class tap;

}

namespace user
{


   class interaction;
   class interaction_impl;
   class frame_window;
   class create_struct;


} // namespace user


typedef sp(::user::interaction_impl) window_sp;


namespace user
{

   class place_holder;

#if defined METROWIN

   class CLASS_DECL_BASE native_window_initialize
   {
   public:


      Agile<Windows::UI::Core::CoreWindow> window;
      ::axis::system_window ^ pwindow;


   };

#elif defined(APPLE_IOS) || defined(ANDROID)

   class CLASS_DECL_BASE native_window_initialize
   {
   public:

      RECT   m_rect;

   };

#else

   class native_window_initialize;

#endif


   class control_event;
   class frame_window;


   CLASS_DECL_BASE bool is_descendant(::user::interaction * puiParent, ::user::interaction * puiChild);


} // namespace user



#if defined(__cplusplus) || defined(__cplusplus_winrt)


namespace user
{

   class CLASS_DECL_BASE create_struct :
#ifdef WINDOWSEX
      public CREATESTRUCTW
#else
      public CREATESTRUCTA
#endif
   {
   public:


      create_struct & operator = (const RECT & rect)
      {

         x = rect.left;
         y = rect.top;
         cx = rect.right - rect.left;
         cy = rect.bottom - rect.top;

         return *this;

      }


      create_struct & operator = (LPCRECT lpcrect)
      {

         if (lpcrect == NULL)
         {

            x = 0;
            y = 0;
            cx = 0;
            cy = 0;

         }
         else
         {

            operator = (*lpcrect);

         }

         return *this;

      }


   };


} // namespace user


#endif







#ifndef TBSTYLE_TOOLTIPS
#define TBSTYLE_TOOLTIPS        0x0100
#define TBSTYLE_WRAPABLE        0x0200
#define TBSTYLE_ALTDRAG         0x0400
#define TBSTYLE_FLAT            0x0800
#define TBSTYLE_LIST            0x1000
#define TBSTYLE_CUSTOMERASE     0x2000
#define TBSTYLE_REGISTERDROP    0x4000
#define TBSTYLE_TRANSPARENT     0x8000
#endif


#include "user/user_schema.h"


#include "base/base/message/message_user.h"



using user_interaction = ::user::interaction;


#include "base/user/user/user_message_queue_listener.h"

//#include "user/user/user_keyboard_focus.h"
//#include "user/user/user_mouse_focus.h"
//#include "base/user/user/user_elemental.h"
//#include "user/user/user_check_interface.h"
#include "base/user/user/user_schema_simple_impl.h"
#include "base/user/user/user_schema_layered_frame.h"
//#include "user/user/user_text_interface.h"
//#include "user/user/user_draw_interface.h"
#include "base/user/user/user_window_util.h"
#include "base/user/user/user_interaction_base.h"
//#include "user/user/user_buffer.h"
#include "base/user/user/user_interaction.h"
#include "base/user/user/user_interaction_impl_base.h"
#include "base/user/user/user_interaction_child.h"
#include "base/user/user/user_interaction_impl.h"

#endif
