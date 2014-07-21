#pragma once

#include "app/appseed/base/base/base/base.h"

#if !defined(_MM) && !defined(_M)

#include "nodeapp/operational_system/operational_system.h"

#pragma warning(disable: 4251)  // using non-exported as public in exported


#ifdef _CORE_LIBRARY
   #define CLASS_DECL_CORE  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_CORE  CLASS_DECL_IMPORT
#endif


//extern CLASS_DECL_CORE string g_strNote;


class image_list;



namespace user
{

   class printer;
   class tree;

   namespace uinteraction
   {

      class uinteraction;

   }

} // namespace user


#undef App
#undef Application
#define planeApp(pbaseapp) (*pbaseapp->m_pplaneapp)
#define planeApplication (planeApp(m_pbaseapp))
#define App(pbaseapp) planeApp(pbaseapp)
#define Application (App(m_pbaseapp))
#undef CaSys
#define CaSys(pca) (*pca->m_pbaseapp->m_pplanesystem)
#undef Sys
#define Sys(pbaseapp) (*pbaseapp->m_pplanesystem)

#undef PlaneSess
#define PlaneSess(pbaseapp) (*pbaseapp->m_pplaneapp->m_pbasesession->m_pplanesession)
#define PlaneSession (PlaneSess(this->m_pbaseapp))


#if defined(WIN32) && !defined(VC6) && ! defined(VC71)
#include <vadefs.h>
#endif


class job;


#include "core_enum.h"


#include "core_main_init_data.h"




#pragma warning(disable: 4250)






#include "core/core/api/api_rotate.h"



#ifdef WINDOWS
#include <crtdbg.h>
#else
//#include "core/api/api_debug.h"
#endif


#if defined(APPLEOS)

#include "core/core/macos/macos.h"

#elif defined(METROWIN)

#include "core/core/metrowin/metrowin.h"

#elif defined(LINUX)

#include "core/core/linux/os.h"

#endif


//#include "primitive/count.h"


#include "core/core/api/api.h"







// former core

// Since core API itself is built with wchar_t as a native type, it will not have
// the correct type info for types built with wchar_t typedef'd to uint32_t
// int16_t.  Make sure that the ::fontopus::user's cast builds this type info in this case.
#ifndef _NATIVE_WCHAR_T_DEFINED
#define ___FULLTYPEINFO
#endif


#ifdef __DBGMEM_H__
#error <atldbgmem.h> cannot be used in core API projects. See __enable_memory_tracking
#endif

#if defined(_MFC_DLL_BLD) && defined(DEBUG)
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#endif


#ifdef WINDOWS
#include <new.h>
#endif





class stdio_file;
struct memory_state;


/////////////////////////////////////////////////////////////////////////////
// Basic types
// abstract iteration position
//struct __POSITION {};
//typedef __POSITION* POSITION;

// Standard constants
#undef FALSE
#undef TRUE

#define FALSE   0
#define TRUE    1

/////////////////////////////////////////////////////////////////////////////
// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after the _ include files

#pragma warning(disable: 4505)  // unreferenced local function has been removed
#pragma warning(disable: 4511)  // private copy constructors are good to have
#pragma warning(disable: 4512)  // private operator= are good to have
#pragma warning(disable: 4514)  // unreferenced inlines are common
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4660)  // template is already instantiated
#pragma warning(disable: 4127)  // constant expression used in macros do while(0);

#pragma warning(disable: 4275)  // deriving exported class from non-exported
#pragma warning(disable: 4251)  // using non-exported as public in exported


#ifdef ___ALL_WARNINGS
#pragma warning( push )
#endif

// warnings caerated with common core API/Windows code
#pragma warning(disable: 4134)  // message map member fxn casts
#pragma warning(disable: 4201)  // nameless unions are part of C++
#pragma warning(disable: 4320)  // structs with uuid handled as interfaces
#pragma warning(disable: 4705)  // statement has no effect in optimized code
#pragma warning(disable: 4191)  // pointer-to-function casting
// warnings caused by normal optimizations
#ifndef DEBUG
#pragma warning(disable: 4701)  // local var *may* be used without init
#pragma warning(disable: 4702)  // unreachable code caused by optimizations
#pragma warning(disable: 4791)  // loss of debugging info in release version
#pragma warning(disable: 4189)  // initialized but unused var
#pragma warning(disable: 4390)  // is_empty controlled statement
#endif
#pragma warning(disable: 4204)  // non-constant aggregate initializer
#pragma warning(disable: 4263 4264)  // base class method is hidden


/////////////////////////////////////////////////////////////////////////////
// Other implementation helpers

#define BEFORE_START_POSITION ((POSITION)-1L)


#undef __DATA
#define __DATA __CORE_DATA


#ifndef _OLEAUTO_H_
   typedef wchar_t * BSTR;// must (semantically) match typedef in oleauto.h
#endif

#undef new



#define new BASE_NEW



#include "core/primitive/primitive.h"


#include "core_strn.h"
#include "core/primitive/primitive_fixed_string.h"


#include "core_less.h"




#include "core_smart_pointer_query.h"



#define ASSERT_KINDOF(class_name, object) ASSERT(base_class < class_name > ::bases(object))
#define DYNAMIC_DOWNCAST(class_name, object) (dynamic_cast < class_name * > (object))
#define STATIC_DOWNCAST(class_name, object) (dynamic_cast<class_name*>(object))
#define STATIC_DOWNCAST_T(class_name, T1, object) (static_cast<class_name<T1>*>(::core::StaticDownCast(RUNTIME_CLASS_T(class_name, T1), object)))
#define STATIC_DOWNCAST_T2(class_name, T1, T2, object) (static_cast<class_name<T1, T2>*>(::core::StaticDownCast(RUNTIME_CLASS_T2(class_name, T1, T2), object)))

// optional bit for schema number that enables object versioning
#define VERSIONABLE_SCHEMA  (0x80000000)

/////////////////////////////////////////////////////////////////////////////
// other helpers

// zero fill everything after the vtbl pointer
#define __ZERO_INIT_OBJECT(base_class) memset(((base_class*)this)+1, 0, sizeof(*this) - sizeof(class base_class));



#include "core.inl"


#undef __DATA
#define __DATA CLASS_DECL_CORE


#ifdef ___ALL_WARNINGS
#pragma warning( pop )
#endif


/*typedef struct tagVMSNMHDR {
    oswindow oswindowFrom;
    UINT idFrom;
    UINT code;
} VMSNMHDR;

typedef struct tagE17_KEYDOWN {
    VMSNMHDR hdr;
    WORD wVKey;
    UINT flags;
} E17_KEYDOWN;

typedef struct tagE17_SYSKEYDOWN {
    VMSNMHDR hdr;
    WORD wVKey;
    UINT flags;
} E17_SYSKEYDOWN;*/



#define APPM_NOTIFY WM_APP + 1117
#define APPN_CHILDDELETE 1000
#define APPN_OPENLINK 1001
   // lParam is a pointer to a Unicode string
   // that can�t be deleted

#define VMSM_NOTIFY WM_USER + 1117
#define VMSM_TIMER WM_USER + 1119

#define E17N_KEYDOWN 17
#define E17N_SYSKEYDOWN 19
#define E17N_ESCAPE 1
#define E17N_RETURN 0


#define VMS_DOC_VERSION_1    1

#define VMS_EVENT_NULL 0
#define VMS_EVENT_MIDI 1
#define VMS_EVENT_MIDI_DELETED 2
#define VMS_EVENT_SYMBOL 3
#define VMS_EVENT_SYMBOL_DELETED 4


#define FOURCC_MShd     mmioFOURCC('M','S','h','d')
#define FOURCC_MStk     mmioFOURCC('M','S','t','k')

#define APP_MAX_STRING_ERROR_CHARS      512
#define APP_MAX_STRING_ERROR_BYTES      (APP_MAX_STRING_ERROR_CHARS * sizeof(TCHAR))

#define VMSM_EXCEPTION WM_APP + 1000
#define EXCEPTION_OLEDB 1000

#define VMSM_RENDER_RECT    WM_USER + 2000
#define VMSM_RENDER_RGN     WM_USER + 2001

#define VMSM_ECONOMODECHANGE WM_USER + 2002

#define VMSM_V001 WM_APP + 1001
#define NOTIFY_BUILDALBUMPROGRESSUPDATE 1000
#define NOTIFY_BUILDALBUMSTATUSUPDATE 1001
#define NOTIFY_BUILDALBUMTASKSUPDATE 1002
#define NOTIFY_BUILDALBUMDELETE 1003
#define NOTIFY_BUILDALBUMFINISHED 1010
#define NOTIFY_TIMERCALLBACKCREATETIMER 1104
#define NOTIFY_TIMERCALLBACKDELETETIMER 1105
#define NOTIFY_CLOSE 1106
#define NOTIFY_BUILDALBUMONHIDETASKDIALOG 120

#define VMSM_COMMAND WM_APP + 1002
#define VMSC_BRINGTOTOP 1000

class CTimerCallback;


typedef struct tag_TimerCallbackCreateTimer
{
    CTimerCallback *    m_pTimerCallback;
    UINT                m_uiElapse;
    UINT                m_uiIDEvent;
} TIMERCALLBACKCREATETIMER, * LPTIMERCALLBACKCREATETIMER;











#include "core_constraint.h"

#ifdef WINDOWS
#ifndef _INC_SHELLAPI
   #include <shellapi.h>
#endif
#endif



namespace user
{


   class document;


} // namespace user


// Avoid mapping GetFileTitle to GetFileTitle[A/W]
#ifdef GetFileTitle
#undef GetFileTitle
inline int16_t APIENTRY GetFileTitle(const char * lpszFile, LPTSTR lpszTitle, WORD cbBuf)
#ifdef UNICODE
   { return ::GetFileTitleW(lpszFile, lpszTitle, cbBuf); }
#else
   { return ::GetFileTitleA(lpszFile, lpszTitle, cbBuf); }
#endif
#endif


#if (_WIN32_WINNT >= 0x501)
#include <uxtheme.h>
#include <vssym32.h>
#endif   // (_WIN32_WINNT >= 0x501)

#ifndef _CA_DLL
#pragma comment(lib, "uuid.lib")
#endif

#ifdef _INC_WINDOWSX
// The following names from WINDOWSX.H collide with names in this header
#undef subclass_window
#undef CopyRgn
#endif

//#include <htmlhelp.h>

#pragma warning( push )
#pragma warning( disable: 4121 )


/////////////////////////////////////////////////////////////////////////////
// Classes declared in this file

         class resource_exception;// Win resource failure exception
         class user_exception;    // Message Box alert and stop operation


      namespace draw2d
      {
         class graphics;                   // a Display Context / HDC wrapper
         class object;            // ::draw2d::graphics_sp drawing tool
         class bitmap;           // a bitmap
         class palette;          // a palette
         class region;
         class font;
         class pen;
         class brush;
         class memory_graphics;         // ::draw2d::graphics_sp for client of window
         class window_graphics;         // ::draw2d::graphics_sp for entire window
         class paint_graphics;          // embeddable BeginPaint struct helper
      }

      namespace core
      {
         class job;
      }

   class command_target;            // a target for ::fontopus::user commands

   namespace user
   {
      class menu;                 // a menu
      class edit_plain_text;            // Edit control
      class view;            // a ::user::impact on a document
      class document_manager;      // impact_system manager object
      class frame_window;        // standard SDI frame
      class interaction_child;
   } // namespace user


      
         class dialog;          // a dialog

         // standard windows controls
         class CStatic;          // Static control
         class simple_button;          // Button control
         class CListBox;         // ListBox control
            class CCheckListBox;// special listbox with checks
         class simple_combo_box;        // ComboBox control
         class CScrollBar;       // ScrollBar control

         // frame windows
            class CMDIFrameWnd; // standard MDI frame
            class CMDIChildWnd; // standard MDI child
            class CMiniFrameWnd;// half-height caption frame wnd

         // views on a document
            class CScrollView;  // a scrolling ::user::impact

               class thread;           // thread base class

//      class impact_system;         // template for document creation
         class single_document_template;// SDI support
         class multiple_document_template; // MDI support

//      class document;            // main document abstraction


// Helper classes
class cmd_ui;           // Menu/button enabling
//class CDataExchange;    // Data exchange and validation context
//class CCommandLineInfo; // CommandLine parsing helper

enum __HELP_TYPE
{
   afxWinHelp = 0,
   afxHTMLHelp = 1
};



#ifdef DEBUG
// Diagnostic Output
//CLASS_DECL_CORE dump_context & operator<<(dump_context & dumpcontext, SIZE size);
//CLASS_DECL_CORE dump_context & operator<<(dump_context & dumpcontext, POINT point);
//CLASS_DECL_CORE dump_context & operator<<(dump_context & dumpcontext, const RECT& rect);
#endif //DEBUG

// Serialization
/*CLASS_DECL_CORE CArchive& operator<<(CArchive& ar, SIZE size);
CLASS_DECL_CORE CArchive& operator<<(CArchive& ar, POINT point);
CLASS_DECL_CORE CArchive& operator<<(CArchive& ar, const RECT& rect);
CLASS_DECL_CORE CArchive& operator>>(CArchive& ar, SIZE& size);
CLASS_DECL_CORE CArchive& operator>>(CArchive& ar, POINT& point);
CLASS_DECL_CORE CArchive& operator>>(CArchive& ar, RECT& rect);
*/






CLASS_DECL_CORE void __get_gray_bitmap(sp(::base::application) papp, const ::draw2d::bitmap &rSrc, ::draw2d::bitmap *pDest, COLORREF crBackground);
CLASS_DECL_CORE void __draw_gray_bitmap(sp(::base::application) papp, ::draw2d::graphics * pgraphics, int32_t x, int32_t y, const ::draw2d::bitmap &rSrc, COLORREF crBackground);
CLASS_DECL_CORE void __get_dithered_bitmap(sp(::base::application) papp, const ::draw2d::bitmap &rSrc, ::draw2d::bitmap *pDest, COLORREF cr1, COLORREF cr2);
CLASS_DECL_CORE void __draw_dithered_bitmap(sp(::base::application) papp, ::draw2d::graphics * pgraphics, int32_t x, int32_t y, const ::draw2d::bitmap &rSrc, COLORREF cr1, COLORREF cr2);


#include "graphics/visual/visual_const.h"


class CRecentFileList;          // forward reference (see afxadv.h)


/*class CLASS_DECL_CORE CCommandLineInfo : public ::core::object
{
public:
   // Sets default values
   CCommandLineInfo();

   // plain char* version on UNICODE for source-code backwards compatibility
   virtual void ParseParam(const char* pszParam, bool bFlag, bool bLast);
#ifdef _UNICODE
   virtual void ParseParam(const char* pszParam, bool bFlag, bool bLast);
#endif

   bool m_bShowSplash;
   bool m_bRunEmbedded;
   bool m_bRunAutomated;
   enum { FileNew, FileOpen, FilePrint, FilePrintTo, FileDDE, AppRegister,
      AppUnregister, FileNothing = -1 } m_nShellCommand;

   // not valid for FileNew
   string m_strFileName;

   // valid only for FilePrintTo
   string m_strPrinterName;
   string m_strDriverName;
   string m_strPortName;

   ~CCommandLineInfo();
// Implementation
protected:
   void ParseParamFlag(const char* pszParam);
   void ParseParamNotFlag(const char* pszParam);
#ifdef _UNICODE
   void ParseParamNotFlag(const char* pszParam);
#endif
   void ParseLast(bool bLast);
};
*/
/////////////////////////////////////////////////////////////////////////////
// ::user::document_manager


/*CLASS_DECL_CORE bool __delete_reg_key(const char * lpszKey);



CLASS_DECL_CORE bool _API
__set_reg_key(const char * lpszKey, const char * lpszValue, const char * lpszValueName = NULL);

*/



#define ___MRU_COUNT   4      // default support for 4 entries in file MRU
#define ___MRU_MAX_COUNT 16   // currently allocated id range supports 16

#define ___SYSPOLICY_NOTINITIALIZED         0
#define ___SYSPOLICY_NORUN               1
#define ___SYSPOLICY_NODRIVES               2
#define ___SYSPOLICY_RESTRICTRUN            4
#define ___SYSPOLICY_NONETCONNECTDISCONNECTD   8
#define ___SYSPOLICY_NOENTIRENETWORK         16
#define ___SYSPOLICY_NORECENTDOCHISTORY      32
#define ___SYSPOLICY_NOCLOSE               64
#define ___SYSPOLICY_NOPLACESBAR            128
#define ___SYSPOLICY_NOBACKBUTTON            256
#define ___SYSPOLICY_NOFILEMRU            512

struct __system_policy_data
{
   const char * szPolicyName;
   uint32_t dwID;
};

struct __system_policies
{
   const char * szPolicyKey;
   __system_policy_data *pData;
};

//#include "core_file_manager_interface.h"


namespace user
{

   class user;


} // namespace user


//#include "user_element_2d.h"
#include "core/user/user/user_control_property.h"
#include "core/user/user/user_window_id.h"


#include "database/database_id.h"
#include "database/database_key.h"

#include "database/database_selection_item.h"
#include "database/database_selection.h"
#include "database/database_update_hint.h"




#include "core_profiler.h"
#include "core_timer.h"
#include "core_timer_callback.h"
#include "core_timer_listener.h"
#include "user/user/user_window_draw.h"
#include "core_history.h"


// services
#include "core/core/service/service_plain_service.h"
#include "core/core/service/service_thread_pool.h"




// former core

#include "core_os_history.h"


namespace windows
{
   template < class APP >
   inline ::application & cast(APP * papp)
   {
      return *((papp));
   }
}




/////////////////////////////////////////////////////////////////////////////
// Extra diagnostic tracing options

#ifdef DEBUG
extern CLASS_DECL_CORE UINT g_uiTraceFlags;
#endif // DEBUG

#ifdef DEBUG
#define DECLARE___TRACE_CATEGORY( name ) extern CLASS_DECL_CORE ::base::trace::category name;
#else
#define DECLARE___TRACE_CATEGORY( name ) const uint_ptr name = 0;
#endif


//////////////////////////////////////////////////////////////////////////////
// simple_message_box helpers

//CLASS_DECL_CORE void ::core::FormatString1(string & rString, UINT nIDS, const char * lpsz1);
//CLASS_DECL_CORE void ::core::FormatString2(string & rString, UINT nIDS,
//            const char * lpsz1, const char * lpsz2);
/*CLASS_DECL_CORE int32_t System.simple_message_box(const char * lpszText, UINT nType = MB_OK,
            UINT nIDHelp = 0);*/
/*CLASS_DECL_CORE int32_t System.simple_message_box(UINT nIDPrompt, UINT nType = MB_OK,
            UINT nIDHelp = (UINT)-1);*/

// Implementation string helpers
//CLASS_DECL_CORE void __format_strings(string & rString, UINT nIDS,
//            const char * const* rglpsz, int32_t nString);
namespace core
{

   CLASS_DECL_CORE void format_strings(string & rString, const char * lpszFormat, const char * const* rglpsz, int32_t nString);
   CLASS_DECL_CORE bool extract_sub_string(string & rString, const char * lpszFullString, int32_t iSubString, char chSep = '\n');

}




#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int32_t)(int16_t)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int32_t)(int16_t)HIWORD(lp))
#endif


#include "core_process_data.h"


#include "core/core/data/data_item.h"
#include "core/core/data/data_tree_item.h"
#include "core/core/data/data_tree.h"

#include "core_fs_interface.h"


// memory primitives
#include "core/primitive/primitive_shared_file.h"



//#include "include/zlib.h"
#include "core/database/sqlite/sqlite.h"



#include "core/core/libcharguess/libcharguess.h"









#include "core_types.h"

#include "core_match.h"

// interfaces
#include "core_progress_interface.h"


// from 7-zip
#include "core/filesystem/file/file_dynamic_buffer.h"
#include "core/filesystem/file/file_in_buffer.h"
#include "core/filesystem/file/file_out_buffer.h"
//#include "core/filesystem/file/file_circular_buffer.h"
#include "core/filesystem/file/file_bitl_decoder.h"
#include "core/filesystem/file/file_bitl_encoder.h"
#include "core/filesystem/file/file_bitm_decoder.h"
#include "core/filesystem/file/file_bitm_encoder.h"
#include "core/filesystem/file/file_limited_reader.h"
#include "core/filesystem/file/file_limited_input_stream.h"
#include "core/filesystem/file/file_clustered_input_stream.h"
#include "core/filesystem/file/file_limited_writer.h"

#include "core/filesystem/file/file_stream_binder.h"
#include "core/filesystem/file/file_locked_in_stream.h"
#include "core/filesystem/file/file_io_temp_buffer.h"
#include "core/filesystem/file/file_size_count_writer.h"
#include "core/filesystem/file/file_dynamic_buffered_writer.h"


//#include "core/filesystem/file/file_buffered_buffer.h"
//#include "core/filesystem/file/file_timeout_buffer.h"
#include "core/filesystem/file/file_edit_buffer.h"
//#include "core/filesystem/file/file_text_buffer.h"
//#include "core_file_system.h"
#include "core_resource.h"

#include "core/core/data/data_simple_item.h"



// former core start - former before concatenation of core + core + core + core

#include "core_debug.h"

#include "core/math/math_math.h"


#include "core_iterator.h"
#include "core_insert_iterator.h"
#include "core_back_insert_iterator.h"
#include "core_std_pair.h"


#include "core_muldiv32.h"


#define EX1ASSERT_VALID(pobj) ASSERT(pobj->Ex1IsObjValid())


#include "core_logic.h"



#include "core_pipe.h"
#include "core_process.h"

#include "core_base64.h"

#include "core_util1.h"

#include "core/primitive/primitive_int_biunique.h"

template <class TYPE>
inline bool is_null(const TYPE & ref)
{
   return &ref == NULL;
}

#define NULL_REF(class) (*((class *) NULL))


//CLASS_DECL_CORE ::core::byte_input_stream &  operator >>(::core::byte_input_stream & istream, string & string);
//CLASS_DECL_CORE ::core::byte_output_stream &  operator <<(::core::byte_output_stream & ostream, const string & string);

//#ifdef WIN32
//#include "core_file_association.h"
//#endif


#include "core_timer.h"

//#include "core/filesystem/file/file_set.h"

//#include "core/filesystem/file/file_folder_watch.h"

//#include "core/filesystem/file/file_transfer_buffer.h"


class document;


class main_frame;


typedef sp(::base::application) (* LP_GET_NEW_APP) ();


namespace core
{


   class command_line;


} // namespace core

namespace visual
{


   class icon;


} // namespace visual



namespace math
{


   class math;


} // namespace math


namespace geometry
{


   class geometry;


} // namespace geometry






class file_system;
typedef smart_pointer < file_system > file_system_sp;
class Ex1FactoryImpl;





#include "core/math/calculator/calculator.h"


#include "net/http/http_output_stream.h"






#include "core/user/colorertake5/colorertake5.h"

#include "core/graphics/html/html.h"


#include "core_debug.h"





//#include "core/filesystem/file/file_application.h"
//#include "core/filesystem/file/file_system.h"
//#include "core/filesystem/file/file_dir_application.h"
//#include "core/filesystem/file/file_dir_system.h"
#include "core/filesystem/file/file_edit_buffer.h"
#include "core_stra.h"
#include "net/net_url_domain.h"


#include "core_service.h"


#include "core_library.h"


#include "core/filesystem/filehandler/filehandler.h"








#include "core/compress/compress.h"
//#include "core/programming/javascript/javascript.h"

// former core - changed to core on 2013-02-21
////////////////////////////////////////////////////////////////
// ca2api
// Layer level 5
// Project named ca2api002_00002
// Namespace ca8




#include "core_patch.h"


#include "core/net/net_email_departament.h"


#include "core/net/hi5/hi5_oauth.h"
#include "core/net/hi5/hi5_twit.h"


#include "core_process_departament.h"






#include "core/programming/dynamic_source/dynamic_source.h"


#include "core/graphics/visual/visual.h"


#include "core/user/user/user.h"




#include "core/graphics/html/html2.h"
#include "core/user/userex/userex.h"



#include "core/filesystem/filemanager/filemanager.h"

#include "core/net/usernet/usernet.h"
//#include "core/user/fontopus/fontopus.h"
#include "core/net/usermail/usermail.h"


#include "core/net/hi5/hi5.h"

#include "core_application.h"




#include "core_session.h"
#include "core_system.h"


#endif




namespace core
{


   CLASS_DECL_CORE bool init_core();

   CLASS_DECL_CORE bool term_core();


} // namespace core

CLASS_DECL_CORE bool defer_core_init();

CLASS_DECL_CORE bool defer_core_term();


#include "core_simple_app.h"





