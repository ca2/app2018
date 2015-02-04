/////////////////////////////////////////////////////////////////////////////////////////
//
//   All Pure and Aural ca2 and God and Gods and Goddssesses and devils's blessings
//
//   C++
//
//   C => in ca2 scope
//
//   Thank you God for C++!!
//

#pragma once

#pragma warning(disable: 4251)

#include "nodeapp/operational_system/operational_system.h"

namespace aura
{

   class menu_base_item;
   class menu_base;
   class menu;


}



namespace base
{

   class application;
   class system;
   class copydesk;
   class system_interaction_impl;

   typedef smart_pointer < copydesk > copydesk_sp;



} // namespace base


namespace database
{


   class server;


} // namespace database


namespace visual
{


   class icon;


} // namespace visual


namespace user
{

   class interaction;
   class user;
   class schema_simple_impl;
   class window_draw;
   class control_event;
   class interaction_impl;
   class frame_window;
   class place_holder;

#if defined(METROWIN) || defined(APPLE_IOS)

   class native_window_initialize;

#endif


} // namespace user

using window_sp = sp(::user::interaction_impl);

namespace primitive
{

   class memory;

} // namespace primitive

namespace plane
{

   class session;
   class system;
   //class cube;


} // namespace plane

class cmd_ui;


namespace draw2d
{


   class graphics;


} // namespace draw2d


namespace file
{


   class stream_buffer;
   class input_stream;
   class output_stream;
   class serializable;


} // namespace file

class machine_event_data;
namespace html
{
   class html;
} // namespace html


namespace message
{

   class mouse;

}


namespace base
{

   template < typename T >
   inline void del(T * & p)
   {
      if(p != NULL)
      {
         delete p;
         p = NULL;
      }
   }

} // namespace base



class image_list;




namespace base
{

#if defined METROWIN && defined(__cplusplus_winrt)

   interface class system_window
   {

      virtual Windows::Foundation::Rect get_window_rect() = 0;
      virtual Windows::Foundation::Point get_cursor_pos() = 0;



   };

   CLASS_DECL_BASE bool get_window_rect(system_window ^ pwindow,RECTD * lprect);
   CLASS_DECL_BASE bool get_window_rect(system_window ^ pwindow,LPRECT lprect);

#endif


   class session;

} // namespace base


namespace data
{


   class item;


} // namespace data







namespace str
{


   namespace international
   {


      class locale_schema;


   } // namespace international


} // namespace str




namespace draw2d
{


   class graphics;


} // namespace draw2d



namespace user
{
   
   
   enum e_control_type
   {
      control_type_none,
      control_type_static,
      control_type_check_box,
      control_type_edit,
      control_type_simple_list,
      control_type_button,
      control_type_combo_box,
      control_type_edit_plain_text,
   };


} // namespace user

class event;


class thread;




class thread;


typedef smart_pointer < thread > thread_sp;


class thread_impl;


typedef smart_pointer < thread_impl > thread_impl_sp;




#define SCAST_PTR(TYPE, ptarget, psource) TYPE * ptarget = dynamic_cast < TYPE * > (psource);
#define SCAST_REF(TYPE, rtarget, psource) TYPE & rtarget = *(dynamic_cast < TYPE * > (psource));

#undef CaSys
#define CaSys(pca) (*pca->m_pauraapp->m_pbasesystem)
#undef Sys
#define Sys(pauraapp) (*pauraapp->m_pbasesystem)
//#define System (Sys(this->m_pauraapp))
//#define threadSystem (Sys(get_thread_app()))

#undef Sess
#define Sess(pauraapp) (*pauraapp->m_pbasesession)
//#define Session (Sess(m_pauraapp))
//#define Plat(pauraapp) (*pauraapp->m_pcoreplatform)
//#define Platform (Plat(m_pauraapp))


#undef App
#define App(pauraapp) (*pauraapp->m_pbaseapp)
//#define Application (App(m_pauraapp))



CLASS_DECL_BASE bool base_init();
CLASS_DECL_BASE bool base_term();

CLASS_DECL_BASE bool __node_base_pre_init();
CLASS_DECL_BASE bool __node_base_pos_init();

CLASS_DECL_BASE bool __node_base_pre_term();
CLASS_DECL_BASE bool __node_base_pos_term();



#if defined(LINUX)

#include "base/os/ansios/ansios.h"
#include "base/os/linux/linux_user_impl.h"

#elif defined(METROWIN)

#include "base/os/metrowin/metrowin.h"
#include "base/os/metrowin/metrowin_user_impl.h"

#elif defined(MACOS)

#include "base/os/ansios/ansios.h"
#include "base/os/macos/macos.h"
#include "base/os/macos/macos_windowing.h"

#elif defined(ANDROID)

#include "base/os/ansios/ansios.h"
#include "base/os/android/android.h"

#elif defined(WINDOWS)

#include "base/os/windows/windows.h"

#elif defined(APPLE_IOS)

#include "base/os/ansios/ansios.h"
#include "base/os/ios/ios.h"
#include "base/os/ios/ios_windowing.h"

#elif defined(SOLARIS)

#include "base/os/ansios/ansios.h"
#include "base/os/solaris/solaris_user_impl.h"

#else


#error "not implemented!!"

#endif

//CLASS_DECL_BASE void __trace_message(const char * lpszPrefix,::signal_details * pobj);
//CLASS_DECL_BASE void __trace_message(const char * lpszPrefix,LPMESSAGE lpmsg);


//#include "base/base/base/base_command_target.h"

#include "base/base/base_static_start.h"

#include "base/graphics/graphics.h"

#include "base/base/base_platform_interface.h"

#include "filesystem/file/file_edit_buffer.h"

//#include "base/base/base_command_target.h"

#include "base/base/base_application.h"

#include "user/user/user_enum.h"

#include "user/user/user_key_enum.h"

#include "user/user/user_schema.h"

#include "base/base/base_session.h"

#include "base/base/base_system.h"

//#include "base/user/user.h"

//#include "base/base/hotplugin/hotplugin.h"

//#include "base/install/install.h"

#include "base/base/base_simple_app.h"

//#include "base/user/userpresence/userpresence.h"

//#include "base/user/fontopus/fontopus.h"

#include "base/base/base.inl"




