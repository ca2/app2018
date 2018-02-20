﻿/////////////////////////////////////////////////////////////////////////////////////////
//
//  ca2
//
//  ca2 is an all-purpose multi-platform framework and set of libraries
//  written in C++ language to help developers create great apps for users.
//
//  ca2/C++/C/Assembly/Huffman Machine/Hardware/Universe/Multiverse stack
//
//

#pragma once

#ifdef WINDOWS
#pragma warning(disable: 4251)
#endif

#include "nodeapp/operational_system/operational_system.h"

class critical_section;

extern CLASS_DECL_AURA critical_section * g_pcsGlobal;

namespace command
{

   class command;

} // namespace command

// very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_null
{
   cnull
};

// very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_empty
{
   cempty
};

// very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_no_exception
{
   no_exception
};

// very short name ([{c}])ontext (switchers, as it as context) enums
enum e_context_switcher_failed
{
   failure,
   failure_no_log
};

enum e_command
{

   command_default,
   command_on_agree_exit,
   command_france_exit,
   command_check_exit

};


namespace aura
{

   enum e_setting
   {

      setting_none,
      setting_wallpaper,
      setting_app_activation

   };

   class library;

   class application;

#if defined(APPLE_IOS)
   extern "C"
#endif
   typedef ::aura::library * (*PFN_GET_NEW_LIBRARY)(::aura::application * papp);

#if defined(APPLE_IOS)
   extern "C"
#endif
   typedef ::aura::application * (*PFN_GET_NEW_APP)(::aura::application * papp);

} // namespace aura

extern "C"
CLASS_DECL_AURA void register_library(const char * psz, ::aura::PFN_GET_NEW_LIBRARY pfnNewLibrary);


#define DECL_LIB(X) extern "C" \
::aura::library * X##_get_new_library(::aura::application * papp)

#define SET_LIB(X) g_pfnNewLibrary = &X##_get_new_library

#define REG_LIB(X) register_library(#X, &X##_get_new_library)

namespace install
{



   class installer;


} // namespace install

template<class T>
struct remove_reference
{
   typedef T TYPE;
};

template<class T>
struct remove_reference<T&>
{
   typedef T TYPE;
};

template<class T>
struct remove_reference<T&&>
{
   typedef T TYPE;
};

template<class T> inline
typename remove_reference<T>::TYPE&& move(T && t)
{

   return (static_cast<typename remove_reference<T>::TYPE&&>(t));

}



template < class T >
class smart_pointer;


#define sp(TYPE) ::smart_pointer < TYPE >




class string;
class istring;
class var;
class property;
class property_set;
class object;
class type;
class base_edit;

class timer_callback;


namespace aura
{

   class Timer;

};

class timer;

typedef bool FN_TIMER(timer * ptimer);
typedef FN_TIMER * PFN_TIMER;


//template < typename Type,typename RawType = typename Type >
//class string_array;
//typedef string_array < string > stringa;
namespace user
{

   class interactive;

#if defined(METROWIN) || defined(APPLE_IOS) || defined(ANDROID)

   class native_window_initialize;

#endif


} // namespace user





namespace aura
{


   class application;
   class system;
   class application_message;

   class command;

   class str;
   class str_context;


} // namespace aura


namespace user
{

   class primitive;
   class frame;
   class menu;
   class controller;
   class document;

   class impact_system;
   class printer;
   class impact;
   class elemental;


} // namespace aura


namespace install
{


   class install;
   class canvas;


} // namespace install

namespace data
{


   class listener;
   class data;
   class simple_data;
   class data_container_base;


} // namespace data

namespace sockets // only usable from base.dll and dependants
{

   class sockets; // only usable from base.dll and dependants


} // namespace sockets // only usable from base.dll and dependants


namespace url
{

   class department; // only usable from base.dll and dependants


} // namespace url


class compress_department; // only usable from axis.dll and dependants

class command_target;

class dump_context;

//class string_interface;
class id_space;
class type;
class ptra;
class factory_item_base;
class fixed_alloc_no_sync;
class critical_section;
class var_array;
//class pair_set_interface;
//class str_str_interface;
class handler;
class thread;
class command_target;
class critical_section;
class mutex;
class string;
class id;

namespace colorertake5
{

   class ParserFactory;


} // namespace colorertake5

namespace visual
{


   class icon;
   class cursor;


} // namespace visual


class memory;


//namespace plane
//{
//
//   class session;
//   class system;
//   //class cube;
//
//
//} // namespace plane


namespace user
{


   class command;


} // namespace user


namespace datetime
{


   class department;


} // namespace datetime


namespace draw2d
{


   class graphics;


} // namespace draw2d


namespace file
{

   class listing;
   class path;
   typedef CLASS_DECL_AURA ::string_array < ::file::path,string > patha;
   class file;
   class istream;
   class ostream;
   class serializable;


} // namespace file


class machine_event_data;

namespace hotplugin
{

   class host;
   class plugin;

}



namespace html
{

   class html;

} // namespace html


namespace multimedia
{

   namespace audio_plugin
   {

      class plugin;

   } // namespace audio_plugin


} // namespace multimedia


#include "aura/aura/aura/aura_auto.h"
#include "aura/primitive/comparison/comparison_compare.h"

#include "aura/aura/aura/aura_pointer.h"

namespace aura
{

   class allocatorsp;

   template < typename T >
   inline void del(T * & p)
   {

      //Thank you Fiora a Eterna!!

      //Fiora Aeterna☄ ‏@FioraAeterna 20m20 minutes ago

      //   maybe it's cynical but I'm starting to think the real reason so many newer games have constant autosaves is because they crash all the time
      //   Details

      // BRT 2015-02-18 19:08
      // catch all (...) here in aura::del ... but should remove try catch from all underlying calls (frees, memory_frees, memory_dbg_frees).

      try
      {

         if(p != NULL)
         {

            T * pdel = p;

            p = NULL;

            delete pdel;

         }

      }
      catch(...)
      {

      }

   }

   template < typename T >
   inline void adel(T * & p)
   {

      //Thank you Fiora a Eterna!!

      //Fiora Aeterna☄ ‏@FioraAeterna 20m20 minutes ago

      //   maybe it's cynical but I'm starting to think the real reason so many newer games have constant autosaves is because they crash all the time
      //   Details

      // BRT 2015-02-18 19:08
      // catch all (...) here in aura::del ... but should remove try catch from all underlying calls (frees, memory_frees, memory_dbg_frees).

      try
      {

         if(p != NULL)
         {

            delete[] p;

            p = NULL;

         }

      }
      catch(...)
      {

      }

   }


   template < typename T >
   inline void free(T * & p)
   {

      //Thank you Mummi!!

      //Nice late afternoon coffee of ever!!

      //Thank you Fiora a Eterna!!

      //Fiora Aeterna☄ ‏@FioraAeterna some minutes ago

      //   maybe it's cynical but I'm starting to think the real reason so many newer games have constant autosaves is because they crash all the time
      //   Details

      // BRT 2016-11-23 19:08
      // catch all (...) here in aura::del ... but should remove try catch from all underlying calls (frees, memory_frees, memory_dbg_frees).

      try
      {

         if (p != NULL)
         {

            void * pdel = (void *) p;

            p = NULL;

            ::free(pdel);

         }

      }
      catch (...)
      {

      }

   }

} // namespace aura

#include "aura/aura/aura/aura_malloc.h"

class image_list;




struct SIZEPARENTPARAMS
{

   RECT rect;       // parent client rectangle (trim as appropriate)
   SIZE sizeTotal;  // total size on each side as on_layout proceeds
   bool bStretch;   // should stretch to fill all space

};


typedef struct RECTD RECTD;


namespace aura
{


//#if defined METROWIN && defined(__cplusplus_winrt)
//
//   interface class system_window
//   {
//
//      virtual Windows::Foundation::Rect get_window_rect() = 0;
//      virtual Windows::Foundation::Point get_cursor_pos() = 0;
//
//
//
//   };
//
//   CLASS_DECL_AURA bool get_window_rect(system_window ^ pwindow,RECTD * lprect);
//   CLASS_DECL_AURA bool get_window_rect(system_window ^ pwindow,LPRECT lprect);
//
//#endif


   class file;


   namespace trace
   {

      class trace;

   }

   class session;

} // namespace aura


// only usable from axis and axis dependants
namespace axis
{

   class application;
   class session;
   class system;

}


// only usable from base and base dependants
namespace base
{

   class application;
   class session;
   class system;

}

// only usable from core and core dependants
namespace core
{

   class application;
   class session;
   class system;

}


class app_core;


typedef ::aura::system * CREATE_SYSTEM(app_core * pappcore);
typedef CREATE_SYSTEM * LPFN_CREATE_SYSTEM;

CLASS_DECL_AURA extern LPFN_CREATE_SYSTEM g_pfn_create_system;
CLASS_DECL_AURA extern const char * g_pszCooperativeLevel;
CLASS_DECL_AURA extern int g_iDerivedApplication; // one-based-index of derived application, zero if main application


namespace data
{


   class item;


} // namespace data


namespace xml
{

   class node;

} // namespace xml






enum e_extract
{

   extract_none,
   extract_first,
   extract_all

};


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


class random_access_iterator
{
public:
};





class event;


class thread;




class thread;


typedef smart_pointer < thread > thread_sp;


//class thread_impl;


//typedef smart_pointer < thread_impl > thread_impl_sp;


namespace html
{

   class html; // defined and set by html library/component

}



#define SCAST_PTR(TYPE, ptarget, psource) TYPE * ptarget = dynamic_cast < TYPE * > (psource);
#define SCAST_REF(TYPE, rtarget, psource) TYPE & rtarget = *(dynamic_cast < TYPE * > (psource))


#define CaSys(pca) (*pca->m_pauraapp->m_paurasystem)
#define Sys(pauraapp) (*pauraapp->m_paurasystem)
#define System (Sys(get_app()))
#define threadSystem (System)


#define Sess(pauraapp) (*pauraapp->m_paurasession)
#define Session (Sess(get_app()))


#undef App
#define App(pauraapp) (*pauraapp)
#define Application (App(get_app()))

#define AppUser(pauraapp) (*pauraapp->m_paurasession->fontopus()->get_user())
#define ApplicationUser (AppUser(get_app()))


// return - result - if not ok
#ifndef RINOK
#define RINOK(x) { int32_t __result__ = (x); if (__result__ != 0) return __result__; }
#endif

// _throw( - exception - result exception - if not ok
#ifndef TINOK
#define TINOK(e, x) { int32_t __result__ = (x); if (__result__ != 0) _throw(e(get_app(), __result__)); }
#endif


#ifdef WINDOWS
#pragma warning(disable: 4251)  // using non-exported as public in exported
#endif


#ifndef WINDOWSEX

typedef void * HDWP;

#endif

#ifdef APPLE_IOS

struct plane_system;

#endif

typedef  void(*PFN_ca2_factory_exchange)(::aura::application * papp);

#ifdef WINDOWS
CLASS_DECL_AURA bool defer_co_initialize_ex(bool bMultiThread);
#endif


CLASS_DECL_AURA bool aura_init();
CLASS_DECL_AURA bool aura_term();

CLASS_DECL_AURA bool __node_aura_pre_init();
CLASS_DECL_AURA bool __node_aura_pos_init();

CLASS_DECL_AURA bool __node_aura_pre_term();
CLASS_DECL_AURA bool __node_aura_pos_term();

extern "C"
CLASS_DECL_AURA ::aura::PFN_GET_NEW_LIBRARY get_library_factory(const char * psz);

extern "C"
CLASS_DECL_AURA void register_library(const char * psz, ::aura::PFN_GET_NEW_LIBRARY p);

CLASS_DECL_AURA ::aura::application * get_app();

#include "aura/primitive/primitive_cflag.h"

#include "aura/aura/aura/aura_definition.h"

#include "aura/aura/aura/aura_action_source.h"

#include "aura/aura/aura/aura_new.h"

#include "aura/aura/aura/aura_lparam.h"
#include "aura/aura/aura/aura_muldiv64.h"

inline bool is_null(const void * p, size_t s)
{

   return ((size_t)p) <= s;

}



template < typename TYPE >
inline bool is_null(const TYPE * p)
{

   return is_null(p, ((sizeof(TYPE) + sizeof(void *)) * 2));

}


template < >
inline bool is_null(const void * p)
{

   return is_null(p, 65536);

}


template < typename TYPE >
inline bool is_set(const TYPE * p)
{

   return !is_null(p);

}


template < typename TYPE >
inline bool is_null_ref(const TYPE & t)
{

   return is_null(&t);

}


template < typename TYPE >
inline bool is_set_ref(const TYPE & t)
{
   return !is_null_ref(t);
}


template <class t>
inline void delptr(t *& p)
{
   if(p != NULL)
   {
      delete p;
      p = NULL;
   }
}


#include "aura/aura/aura/aura_auto_pointer.h"


// C-includes
//#include "aura/aura/os/os.h"




#include "aura/aura/aura/aura_smart_pointer1.h"
#include "aura/aura/aura/aura_smart_pointer2.h"


#include "aura/aura/aura/aura_keep_true.h"


namespace file
{


   using file_sp = sp(file);


} // namespace file


#include "aura/primitive/primitive_logic.h"



#include "aura/primitive/math/math_static_numeric_info.h"
#include "aura/primitive/math/math_numeric_info.h"
#include "aura/primitive/math/math_c_number.h"
#include "aura/primitive/math/math_clip.h"

//#include "aura/primitive/math/math_complex.h"


#include "aura/primitive/datetime/datetime_duration.h"


#include "aura/primitive/str/str.h"



#include "aura/primitive/datetime/datetime_date_span.h"
#include "aura/primitive/datetime/datetime_time_span.h"
#include "aura/primitive/datetime/datetime_time.h"
#include "aura/primitive/datetime/datetime_zonetime.h"
#include "aura/primitive/datetime/datetime_file_time_span.h"
#include "aura/primitive/datetime/datetime_file_time.h"

#include "aura/primitive/enum.h"


#include "aura/primitive/primitive_object.h"

#include "aura/aura/aura/aura_debug.h"


#include "aura/aura/aura/aura_app_core.h"



#include "aura/aura/aura/aura_class.h"

#include "aura/primitive/comparison/comparison_md5.h"

#include "aura/aura/aura/aura_conv.h"

#include "aura/aura/aura/aura_core.h"

#include "aura/primitive/primitive_request_interface.h"

#include "aura/multithreading/multithreading_wait_result.h"

#include "aura/multithreading/multithreading_waitable.h"

#include "aura/primitive/primitive_object.h"


#include "aura/aura/xml/xml_exportable.h"
#include "aura/aura/xml/xml_importable.h"


#include "aura/primitive/comparison/comparison.h"


#include "aura/primitive/collection/collection_decl.h"
#include "aura/primitive/collection/collection_raw_array_decl.h"
#include "aura/primitive/collection/collection_smart_pointer_array.h"
#include "aura/primitive/collection/collection_comparable_eq_array.h"
#include "aura/primitive/collection/collection_comparable_array.h"
#include "aura/primitive/collection/collection_comparable_raw_array.h"


#include "aura/primitive/collection/collection_lemon_array_decl.h"
#include "aura/primitive/collection/collection_lemon_heap.h"


#include "aura/aura/aura/aura_action_context.h"


CLASS_DECL_AURA string merge_colon_args(const array < stringa > & str2a);


#include "aura/aura/aura/aura_fixed_alloc.h"



#include "aura/aura/aura/aura_allocate.h"
#include "aura/aura/aura/aura_plex.h"


#include "aura/primitive/primitive_id.h"

#include "aura/primitive/str/str_interface.h"
#include "aura/primitive/str/str_composite.h"
#include "aura/primitive/str/str_base64.h"



#include "aura/aura/exception/exception.h"


template<class TYPE>
inline void dump_elements(dump_context & dumpcontext, const TYPE* pElements, ::count nCount);

#include "aura/aura/aura/aura_common.h"
#include "aura/filesystem/filesystem/filesystem_path.h"



#include "aura/filesystem/file/file_cpp.h"



#include "aura/aura/aura/aura_library.h"






#include "aura/filesystem/filesystem.h"





#include "aura/primitive/geometry/geometry.h"


#include "aura/primitive/collection/collection.h"


#include "aura/primitive/geometry/geometry_box.h"


#include "aura/aura/aura/aura_cmp.h"


#include "aura/filesystem/file/file_string_file.h"


#include "aura/primitive/str/str_format.h"


#include "aura/primitive/primitive_var2.h"


#include "aura/primitive/primitive_id_space.h"


#include "aura/primitive/math/math_scalar_enum.h"


#include "aura/primitive/math/math_scalar.h"


#include "aura/aura/aura/aura_progress.h"


#include "aura/aura/aura/aura_enum.h"


#include "aura/multithreading/multithreading.h"


#include "aura/aura/aura/aura_draw2d.h"


//CLASS_DECL_AURA mutex * draw2d_mutex();


#include "aura/aura/aura/aura_gudo.h"

namespace aura
{

#if defined METROWIN && defined(__cplusplus_winrt)

   interface class system_window
      {

         virtual Windows::Foundation::Rect get_window_rect() = 0;
         virtual Windows::Foundation::Point get_cursor_pos() = 0;



      };

   CLASS_DECL_AURA bool get_window_rect(system_window ^ pwindow, RECTD * lprect);
   CLASS_DECL_AURA bool get_window_rect(system_window ^ pwindow, LPRECT lprect);

#endif


   class session;

} // namespace aura



//#include "aura/user/user_create_context.h"





//#if defined(LINUX)
//
//#include "aura/os/ansios/ansios.h"
//
//#elif defined(METROWIN)
//
//#include "aura/os/metrowin/metrowin.h"
//#include "aura/os/metrowin/metrowin_user_impl.h"
//
//#elif defined(MACOS)
//
//#include "aura/aura/os/ansios/ansios.h"
//#include "aura/aura/os/macos/macos.h"
//
//#elif defined(ANDROID)
//
//#include "aura/os/ansios/ansios.h"
//#include "aura/os/android/android.h"
//
//#elif defined(WINDOWSEX)
//
//#include "aura/aura/os/windows/windows.h"
//
//#elif defined(APPLE_IOS)
//
//#include "aura/os/ansios/ansios.h"
//#include "aura/os/ios/ios.h"
////#include "aura/os/ios/ios_windowing.h"
//
//#elif defined(SOLARIS)
//
//#include "aura/os/ansios/ansios.h"
//#include "aura/os/solaris/solaris_user_impl.h"
//
//#else
//
//#error "not implemented!!"
//
//#endif





class CLASS_DECL_AURA openweather_city :
   virtual public mini_object
{
public:

   index    m_iIndex;
   string   m_strCnt;
   string   m_strCit;
   int64_t  m_iId;
   double   m_dLat;
   double   m_dLon;

};




CLASS_DECL_AURA string get_system_error_message(uint32_t dwError);


#include "aura/aura/aura/aura_plex_heap.h"

#include "aura/primitive/primitive_type.h"


//#include "user/user/user_enum.h"
//#include "user/user/user_keyboard_focus.h"
//#include "user/user/user_mouse_focus.h"
//#include "user/user/user_elemental.h"
#include "aura/aura/aura/aura_flags.h"
#include "aura/primitive/primitive_check.h"

//#include "user/user/user_check_interface.h"


#include "aura/aura/message/message_base.h"
#include "aura/aura/message/message_timer.h"

//#include "aura/user/user/user_const.h"

#include "aura/user/user/user_command.h"
//#include "aura/aura/aura/aura_::user::command.h"
//#include "aura/aura/aura/aura_::user::command_probe.h"
//#include "graphics/visual/visual_const.h"
//#include "user/user/user_key_enum.h"
#include "aura/aura/message/message.h"
//#include "aura/user/user/user_check_interface.h"
#include "aura/aura/aura/aura_command_target.h"
//#include "user/user/user_style.h"
//#include "user/user/user_style_simple_impl.h"
//#include "user/user/user_style_layered_frame.h"
//#include "user/user/user_text_interface.h"
//#include "user/user/user_draw_interface.h"
//#include "user/user/user_window_util.h"
//#include "user/user/user_interaction_base.h"
//#include "user/user/user_buffer.h"
#include "aura/aura/aura/aura_keep.h"
#include "aura/aura/aura/aura_restore.h"
//#include "aura/aura/aura/aura_md5.h"
#include "aura/filesystem/file/file_stream2.h"
//#include "user/user/user_interaction.h"
//#include "user/user/user_interaction_impl_base.h"
//#include "user/user/user_interaction_child.h"
//#include "user/user/user_interaction_impl.h"
//#include "user/user/user_control_bar.h"
//#include "user/user/user_wait_cursor.h"
//#include "user/user/user_frame_window.h"
//#include "user/user/user_server.h"
//#include "user/user/user_impact_system.h"





#include "aura/primitive/primitive_interlocked_long.h"
#include "aura/primitive/primitive_interlocked_long_pulse.h"

#include "aura/primitive/data/data_data.h"
#include "aura/primitive/data/data_data_listener.h"
#include "aura/primitive/data/data_data_container.h"
#include "aura/primitive/data/data_item.h"
//#include "aura/primitive/data/data_tree_item.h"
//#include "aura/primitive/data/data_tree.h"
//#include "aura/primitive/data/data_simple_item.h"







//----------




#include "aura/primitive/math/math_objects.h"




//----------





#include "aura/aura/aura/aura_simple_log.h"


#include "aura/primitive/collection/collection_file_path_map.h"

#include "aura/primitive/primitive_edit.h"
#include "aura/aura/aura/aura_department.h"
#include "aura/aura/aura/aura_department_container.h"
#include "aura/aura/xml/xml.h"


#ifndef WINDOWSEX
class mq;
#endif

#include "aura/aura/aura/aura_live_signal.h"
#include "aura/multithreading/multithreading_pred_holder.h"
#include "aura/multithreading/multithreading_thread.h"
#include "aura/multithreading/multithreading_forking_thread.h"
#include "aura/multithreading/multithreading_delay_thread.h"
#include "aura/multithreading/multithreading_tools.h"
#include "aura/aura/aura/aura_timer_callback.h"
#include "aura/aura/aura/aura_timer_item.h"
#include "aura/aura/aura/aura_timer_array.h"
#include "aura/aura/aura/aura_timer.h"
#include "aura/aura/aura/aura_timer_event.h"

#include "aura/multithreading/multithreading_thread_impl.h"
#include "aura/multithreading/multithreading_simple_thread.h"
#include "aura/multithreading/multithreading_go_thread.h"
#include "aura/multithreading/multithreading_signal_thread.h"
#include "aura/multithreading/multithreading_procedure_array.h"


#include "aura/primitive/primitive_job.h"

#include "aura/user/simple/simple_message_box.h"

#include "aura/aura/aura/aura_user.h"

#include "aura/aura/aura/aura_log.h"
#include "aura/aura/aura/aura_trace.h"



#include "aura/primitive/primitive_memory_base.h"
#include "aura/primitive/primitive_memory.h"
#include "aura/primitive/primitive_shared_memory.h"
#include "aura/primitive/primitive_virtual_memory.h"
#include "aura/primitive/primitive_memory_container.h"





#include "aura/filesystem/file/file_memory_file.h"
#include "aura/filesystem/file/file_circular_file.h"


//#include "aura/filesystem/file/file_timeout_buffer.h"
//#include "aura/filesystem/file/file_transfer_buffer.h"

#include "aura/aura/aura/aura_file_memory_map.h"

#include "aura/primitive/str/str_international2.h"



//#if defined(METROWIN)
//
//#include "app/appseed/aura/aura/os/metrowin/metrowin_sockets.h"
//
//#endif


#include "aura/primitive/primitive_factory.h"


#include "aura/primitive/math/math_rng.h"
#include "aura/primitive/math/math_department.h"
#include "aura/primitive/geometry/geometry_geometry.h"


CLASS_DECL_AURA string get_exe_path();


#include "aura/aura/aura/aura_savings.h"

#include "aura/net/net.h"

#include "aura/aura/aura/aura_launcher.h"

#include "aura/aura/aura/aura_ipc.h"

#include "aura/aura/aura/aura_app_launcher.h"

#include "aura/aura/aura/aura_ipi.h"


#include "aura/filesystem/filesystem/filesystem_application.h"

#include "aura/filesystem/filesystem/filesystem_dir_application.h"

#include "aura/filesystem/filesystem/filesystem_listing.h"

#include "aura/filesystem/file/file_set.h"

#include "aura/primitive/primitive_application_bias.h"

#include "aura/primitive/primitive_command.h"


#include "aura/user/user/user_interactive.h"




#include "aura/aura/exception/exception_engine.h"

#include "aura/aura/aura/aura_fixed_alloc_impl.h"
#include "aura/aura/aura/aura_plex_heap_impl.h"
#include "aura/primitive/primitive_command_line.h"
#include "aura/user/user/user_create.h"
#include "aura/primitive/primitive_create.h"
#include "aura/primitive/primitive_request_signal.h"




#include "aura/aura/aura/aura_net.h"



#include "aura/primitive/primitive_handler.h"




#include "aura/filesystem/file/file_buffered_file.h"


#include "aura/aura/aura/aura_machine_event_data.h"
#include "aura/aura/aura/aura_machine_event.h"
#include "aura/aura/aura/aura_machine_event_central.h"

#include "aura/primitive/datetime/datetime.h"

#include "aura/primitive/str/str_international_locale_schema.h"

#include "aura/aura/aura/aura_id_pool.h"



#include "aura/aura/aura/aura_cregexp.h"
#include "aura/aura/aura/aura_cregexp_util.h"


class CLASS_DECL_AURA ptra :
   virtual public spa(object)
{
public:


};


typedef ::map < sp(object), sp(object), sp(object), sp(object) > element_map;
typedef ::map < sp(object), sp(object), ptra, ptra > map_many;

struct install_status
{

   int         m_iCheck;
   bool        m_bOk;

   install_status()
   {

      m_iCheck = 0;
      m_bOk = false;

   }


   DWORD calc_when_is_good_to_check_again();

};



#include "aura/aura/os/os_text.h"

#ifdef VSNORD

#include "aura/aura/os/android/android_data_exchange.h"

#endif


#include "aura/aura/process/process.h"

#include "aura/filesystem/filesystem/filesystem_system.h"

#include "aura/aura/aura/aura_file_watcher.h"
#include "aura/aura/aura/aura_file_watcher_listener_thread.h"

#include "aura/filesystem/filesystem/filesystem_dir_system.h"

#include "aura/aura/aura/aura_os.h"

#include "aura/aura/crypto/crypto.h"

#include "aura/multimedia/multimedia.h"

#include "aura/aura/aura/aura_console_window.h"

#include "aura/primitive/primitive_color.h"

#include "aura/graphics/graphics.h"

#include "aura/user/user/user.h"

//#include "aura/aura/aura/aura_application_interface.h"
#include "aura/aura/aura/aura_application.h"
#include "aura/aura/aura/aura_application_ptra.h"


#include "aura/aura/aura/aura_gudo_application.inl"


#include "aura/aura/aura/aura_application_message.h"

#include "aura/aura/fontopus/fontopus.h"




#include "aura/aura/aura/aura_session.h"

#include "aura/net/net_email_department.h"

#include "aura/aura/os/os2.h"

#include "aura/aura/aura/aura_system.h"

#include "aura/aura/install/install.h"

#include "aura/aura/aura/aura_system_str.h"

#include "aura/aura/os/os_process.h"









//#include "aura/install/install_trace.h"


#define new AURA_NEW


#include "aura/aura/aura/aura_system_trace.h"




#include "aura/multithreading/multithreading.inl"


#include "aura/primitive/str/str.inl"

#include "aura/aura/aura/aura.inl"

#include "aura/primitive/collection/collection.inl"


/*namespace numeric_info
{


template < typename T >
inline T get_maximum_value()
{
_throw(not_implemented(get_app()));
}

template < typename T >
inline T get_minimum_value()
{
_throw(not_implemented(get_app()));
}
template < typename T >
inline T get_null_value()
{
return 0;
}
template < typename T >
inline T get_unitary_value()
{
return 1;
}

template < typename T >
inline T get_allset_value()
{
T t;
memset(&t,0xff,sizeof(T));
return t;
}

template < typename T >
inline bool is_signed()
{
return ((T)-1) < 0;
}

template < typename T >
inline bool is_integer()
{
// guess, as float and double is implemented
return true;
}


template < typename T >
inline string json(const T & value)
{
return ::str::from(value);
}

} // namespace numeric_info

*/


#ifdef VARIADIC_TEMPLATE_FORMAT2

template<typename T,typename... Args>
inline void string_format::format(const char * & s,const T & value,Args... args)
{

   while(*s)
   {

      if(*s == '%' && *(++s) != '%')
      {

         defer_get_additional_argument(s,value,args...);

         return;

      }

      append(*s++);

   }

   _throw(simple_exception(get_app(),"extra arguments provided to format"));


}




#endif





#ifndef WINDOWSEX

#include "aura/aura/os/cross/windows/windows_thread_impl.h"

#endif




#include "aura/primitive/collection/collection_array_base.inl"
#include "aura/primitive/collection/collection_array_impl.inl"
#include "aura/primitive/collection/collection_raw_array_impl.inl"
#include "aura/primitive/collection/collection_sort_array_impl.inl"
#include "aura/primitive/collection/collection_lemon_array_impl.inl"

#include "aura/primitive/collection/collection_list_impl.inl"
#include "aura/primitive/collection/collection_map_impl.inl"

#include "aura/primitive/collection/collection_sort_map_impl.inl"

#include "aura/primitive/collection/collection_fifo_map_impl.inl"



#include "aura/primitive/primitive_factory_impl.inl"





#include "aura/aura/aura/aura_system_smart_pointer.inl"
#include "aura/aura/aura/aura_system_object.inl"
#include "aura/aura/aura/aura_system_id.inl"
#include "aura/aura/aura/aura_system_factory.inl"
#include "aura/aura/aura/aura_system_primitive_var.inl"
#include "aura/aura/aura/aura_system_xml.inl"



//#include "aura/aura/aura/aura_core_os.h"


#include "aura/aura/aura/aura_chronometer.h"







#include "aura/aura/aura/aura_assert_running_task.h"










#include "aura/aura/aura/aura_number.h"


#include "aura/aura/aura/aura_debug.h"


#include "aura/aura/aura/aura_printf.h"
#include "aura/aura/aura/aura_sprintf.h"



#include "aura/aura/aura/aura_math.h"









#include "aura/aura/aura/aura_file_watcher.h"
#include "aura/aura/aura/aura_file_watcher_impl.h"


#include "aura/aura/aura/aura_console_window.h"



// C++ includes
//#include "aura/aura/os/os_os.h"



#include "aura/multithreading/multithreading_data.h"


#include "aura/aura/aura/aura_shell_launcher.h"


#include "aura/aura/aura/aura_async.h"


#include "aura/aura/aura/aura_message_loop.h"

#include "aura/aura/aura/aura_international.h"

#include "aura/aura/aura/aura_static_start.h"



#define return_(y, x) {y = x; return;}


extern "C"
{


   int32_t _c_lock_is_active(const char * pszName);
   int32_t _c_lock(const char * pszName,void ** pdata);
   int32_t _c_unlock(void ** pdata);


}


CLASS_DECL_AURA string _ca_get_file_name(const char * psz,bool bCreate = false,int32_t * pfd = NULL);

CLASS_DECL_AURA string get_system_error_message(uint32_t dwError);


#include "aura/aura/aura/aura_simple_app.h"


#include "aura/aura/aura/aura_profiler.h"


#include "aura/aura/aura/aura_speaker.h"


#include "app/appseed/aura/aura/node/node.h"


#include "aura/aura/install/install.h"


namespace math
{

   template < typename T >
   class complex;


} // namespace math

namespace _std
{


   template <class T> void swap(T& a, T& b)
   {
      T c(a);
      a = b;
      b = c;
   }


}

namespace std
{

   using string = ::string;
   using wstring = ::wstring;

   template < class KEY,class VALUE >
   using map = ::map < KEY,const KEY &,VALUE,const VALUE & >;

   template < class TYPE >
   using list = ::list< TYPE >;

   template < class TYPE >
   using vector = ::array< TYPE >;

   template < class TYPE >
   using set = ::set< TYPE >;

   using ostream = ::file::ostream;
   using ofstream = ::file::file_ostream;

   using istream = ::file::istream;
   using ifstream = ::file::file_istream;

   using stream = ::file::stream;
   using fstream = ::file::file_stream;

   using iostream = ::file::stream;
   using iofstream = ::file::file_stream;

   using stringstream = ::file::plain_text_stream_string_file;
   using ostringstream = ::file::plain_text_stream_string_file;

   template < typename T >
   using numeric_limits = ::numeric_info < T >;

   //      template <class T> void sort(T & t1,T & t2)
   //    {
   //     ::sort::sort < T >(t1,t2);
   //}

   template < class iterator >
   void sort(iterator & a, iterator & b)
   {
      ::sort::quick_sort_iter(a, b);
   }


   template < typename T >
   using stack = ::stack < T >;


   using filebuf = ::file::stdio_file;

   //using ios_base = ::file::stream_base;

   // replace ::::file:: => ::file::


   using runtime_error = ::runtime_error;

   using stdio_file = ::file::file;


   template < typename T >
   T abs(T t)
   {
      if(t < ::numeric_info< T >::null())
         return -t;
      else
         return t;
   }


   using streamsize = file_size_t;
   using streampos = file_position_t;
   using streamoff = file_offset_t;

   using setw = ::file::set_width;

#if !defined(WINDOWSEX) && !defined(APPLEOS) && !defined(METROWIN)

   using bad_alloc = ::memory_exception;

#endif

   template < typename T >
   using complex = ::math::complex < T >;


   template <class T> const T& min(const T& a,const T& b)
   {
      return !(a > b) ? a : b;
   }
   template <class T> const T& max(const T& a,const T& b)
   {
      return !(a < b) ? a : b;
   }

   template <class RandomAccessIterator>
   void make_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::make_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void make_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::make_heap(first,last,comp);
   }

   template <class RandomAccessIterator,class Compare>
   void pop_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::pop_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void pop_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::pop_heap(first,last,comp);
   }

   template <class RandomAccessIterator,class Compare>
   void push_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::push_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void push_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::push_heap(first,last,comp);
   }

   template <class RandomAccessIterator>
   void sort_heap(RandomAccessIterator first,RandomAccessIterator last)
   {
      ::lemon::sort_heap(first,last);
   }

   template <class RandomAccessIterator,class Compare>
   void sort_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
   {
      ::lemon::sort_heap(first,last,comp);
   }



#if !defined(__APPLE__)

   template <class T> void swap(T& a, T& b)
   {
      _std::swap(a, b);
   }

#endif


}


template < typename T >
inline string & to_json(string & str, const T & value, bool bNewLine)
{

   return str = ::str::from(value);

}


CLASS_DECL_AURA void dappy(const char * psz);




#include "aura/primitive/primitive_color.h"


#include "aura/aura/aura/aura_str_context.h"


#include "aura/filesystem/file/file_html_file.h"


#include "aura/primitive/data/data_simple_item.h"


#include "aura/primitive/data/data_tree_item.h"


#include "aura/primitive/data/data_tree.h"


#include "aura/filesystem/file/file_edit_file.h"


CLASS_DECL_AURA int trace_hr(const char * psz,HRESULT hr);





CLASS_DECL_AURA string get_last_error_string();

CLASS_DECL_AURA string get_error_string(uint64_t uiError);

CLASS_DECL_AURA void set_aura(void * p,::aura::application * papp);
CLASS_DECL_AURA ::aura::application * get_aura(void * p);

#include "aura/aura/aura/aura_error.h"
#include "aura/primitive/primitive.h"


#include "aura/user/user/user.h"


//#include "aura/pcre/pcre.h"


//#include "aura/charguess/charguess.h"


#include "aura/net/net.h"




#include "aura/aura/scripting/javascript/javascript.h"


#include "aura/aura/compress/compress.h"


#include "aura/primitive/str/str_str3.h"


#include "aura/graphics/graphics.h"


#include "aura/aura/install/install2.h"


#include "aura/user/user/user_controller.h"

#if !defined(METROWIN)

#include "aura/aura/aura/aura_serial.h"

#endif



namespace user
{

   inline oswindow primitive::get_safe_handle() const
   {
      if(((byte *)this) < (byte *)(((byte *)NULL) + (16 * 1024))) // consider invalid
      {
         return NULL;
      }
      return get_handle();
   }






} // namespace user

#include "aura/primitive/collection/collection_string_array_impl.inl"

#include "aura/primitive/primitive.inl"

#include "aura/primitive/geometry/geometry.inl"

#include "aura/filesystem/filesystem.inl"

#include "aura/net/net.inl"

#include "aura/graphics/visual/visual.inl"

#include "aura/aura/exception/exception.inl"








