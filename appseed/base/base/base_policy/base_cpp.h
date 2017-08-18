#pragma once

#pragma warning(disable: 4251)

#include "nodeapp/operational_system/operational_system.h"

#define DRAWDD() virtual void _001OnDraw(::draw2d::graphics * pgraphics)


#include "base/user/user/user_const.h"

namespace user
{



   class message_queue_listener;

   namespace wndfrm
   {

      class wndfrm;

      namespace frame
      {
         
         class WorkSetClientInterface;

         class WorkSetUpDownInterface;

      }

   }

   class scroll_info;


   class interaction_impl_base;
   class interaction;
   class user;
   class window_draw;
   class control_event;
   class interaction_impl;
   class frame_window;
   class place_holder;
   class form_view;
   class view_creator_data;
   class message;
   class map_form_window;
   class document_manager;
   class mouse;

   class interaction_spa;

#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)

   class native_window_initialize;

#endif

   CLASS_DECL_BASE bool is_docking_appearance(::user::e_appearance eappearance);


} // namespace user

namespace base
{

   class application;
   class system;
//   class system_interaction_impl;

   //typedef smart_pointer < copydesk > copydesk_sp;



} // namespace base


namespace database
{


   class server;


} // namespace database


namespace visual
{


   class icon;


} // namespace visual



using window_sp = sp(::user::interaction_impl);


namespace plane
{

   class session;
   class system;
   //class cube;


} // namespace plane

class command_ui;


namespace draw2d
{


   class graphics;


} // namespace draw2d


namespace file
{


   class file;
   class istream;
   class ostream;
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





class event;


class thread;




class thread;


typedef smart_pointer < thread > thread_sp;


class thread_impl;


typedef smart_pointer < thread_impl > thread_impl_sp;





#undef CaSys
#define CaSys(pca) (*pca->m_pauraapp->m_pbasesystem)
#undef Sys
#define Sys(pauraapp) (*pauraapp->m_pbasesystem)
//#define System (Sys(this->m_pauraapp))
//#define threadSystem (Sys(get_thread_app()))

#undef Sess
#define Sess(pauraapp) (*pauraapp->m_pbasesession)
//#define Session (Sess(m_pauraapp))
//#define Sess(pauraapp) (*pauraapp->m_pcoresession)
//#define Session (Sess(m_pauraapp))


#undef App
#define App(pauraapp) (*pauraapp->m_pbaseapp)
//#define Application (App(m_pauraapp))



CLASS_DECL_BASE bool base_init();
CLASS_DECL_BASE bool base_term();

CLASS_DECL_BASE bool __node_base_pre_init();
CLASS_DECL_BASE bool __node_base_pos_init();

CLASS_DECL_BASE bool __node_base_pre_term();
CLASS_DECL_BASE bool __node_base_pos_term();

#include "base/graphics/graphics.h"


#if defined(LINUX)

#include "base/os/ansios/ansios.h"
#include "base/os/linux/linux.h"

#elif defined(METROWIN)

#include "base/os/metrowin/metrowin.h"
//#include "base/os/metrowin/metrowin_user_impl.h"

#elif defined(MACOS)

#include "base/os/ansios/ansios.h"
#include "base/os/macos/macos.h"
//#include "base/os/macos/macos_windowing.h"

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


CLASS_DECL_BASE ::user::interaction_impl * window_from_handle(oswindow oswindow);

#include "base/base/base_static_start.h"


//#include "base/user/user/user_style.h"



#include "base/database/database.h"

#include "base/user/user_prelude.h"

#include "base/base/base_application.h"

#include "base/base/base_session.h"

#include "base/base/base_system.h"

#include "base/user/user.h"

#include "base/base/node/node.h"

#include "base/base/base_simple_app.h"

#include "base/user/user.inl"

#include "base/base/base.inl"









