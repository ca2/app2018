#pragma once


#ifdef __cplusplus

CLASS_DECL_AURA void defer_dock_application(bool bDock);

#endif

BEGIN_EXTERN_C
CLASS_DECL_AURA WINBOOL IsProcessRunning(DWORD pid);
END_EXTERN_C


#include "os_threading.h"

#include "cross.h"

/*
#if _POSIX_C_SOURCE == 200809L
#error "Only for test!! Good error!! _POSIX_C_SOURCE still 200809L"
#endf
*/


#ifdef cplusplus

inline bool IsDirSep(WCHAR ch);



#endif

CLASS_DECL_AURA void set_splash(oswindow oswindow);
CLASS_DECL_AURA oswindow get_splash();

CLASS_DECL_AURA int get_processor_count();

CLASS_DECL_AURA void output_debug_string(const char * psz);
CLASS_DECL_AURA void w_output_debug_string(const unichar * psz);
#ifdef __cplusplus
CLASS_DECL_AURA void output_debug_string(const unichar * pwsz);
#endif
CLASS_DECL_AURA void writeln(const char * psz);




//CLASS_DECL_AURA int_bool ui_get_cursor_pos(POINT * ppt);


BEGIN_EXTERN_C

CLASS_DECL_AURA int_bool is_windows_98_or_lesser();

CLASS_DECL_AURA int_bool is_windows_nt();

CLASS_DECL_AURA int_bool is_windows_nt_lesser_than_2000();
CLASS_DECL_AURA int_bool is_windows_2000_or_greater();
CLASS_DECL_AURA int_bool is_windows_xp_or_greater();
CLASS_DECL_AURA int_bool is_windows_vista_or_greater();
CLASS_DECL_AURA int_bool is_windows_7_or_lower();
CLASS_DECL_AURA int_bool is_windows_8_or_greater();

CLASS_DECL_AURA int_bool is_windows_native_unicode();



END_EXTERN_C

#include "os_time.h"

/*
#if _POSIX_C_SOURCE == 200809L
#error "Only for test!! Good error!! _POSIX_C_SOURCE still 200809L"
#endf
*/

//#ifdef __cplusplus
//
//class string;
//
//CLASS_DECL_AURA bool file_copy_dup(const string & strDup,const string & strSrc, bool bOverwrite = true);
//
//#endif // __cplusplus

#if defined(MACOS)


//#include "aura/aura/os/apple/apple.h"
//#include "aura/aura/os/macos/macos_macos.h"


#elif defined(APPLE_IOS)


//#include "aura/aura/os/apple/apple.h"
//#include "aura/os/ios/ios_ios.h"


#elif defined(LINUX)


//#include "aura/os/linux/linux_linux.h"


#elif defined(METROWIN)


//#define BYESHYTOULA_STYLE_SOCKS // Thanks to God, Engineers with Good Faith and also Microsoft bro-grammers getting pro.
#define BSD_STYLE_SOCKETS

#elif defined(WINDOWSEX)


#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#elif defined(ANDROID)


#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#elif defined(SOLARIS)


#define BSD_STYLE_SOCKETS
#define HAVE_MYSQL
#define HAVE_OPENSSL


#elif defined(APPLE_IOS)




#else


#error Not supported operating system


#endif


BEGIN_EXTERN_C

CLASS_DECL_AURA void set_command_line_dup(const char * psz);
CLASS_DECL_AURA void get_command_line_dup(char * psz, int iBuffer);

END_EXTERN_C

#ifdef __cplusplus

#include "os_windowing.h"

class runnable
{

public:

   virtual void run() = 0;

};


CLASS_DECL_AURA void run_runnable_on_main_thread(runnable * prunnable);


template < typename PRED >
class runnable_pred :
   virtual public runnable
{
public:

   PRED m_pred;

   runnable_pred(PRED pred) :
      m_pred(pred)
   {

   }

   virtual void run()
   {
      m_pred();

      delete this;

   }


};


template < typename PRED >
inline void run_pred_on_main_thread(PRED pred)
{

   runnable_pred < PRED > * prunnablepred = new runnable_pred < PRED >(pred);

   run_runnable_on_main_thread(prunnablepred);

}

#endif


#include "os_graphics.h"


#pragma once


#include "aura/aura/os/os_binreloc.h"




#ifdef AMD64
#define OS64BIT
#elif defined(__LP64)
#define OS64BIT
#else
#define OS32BIT
#endif











