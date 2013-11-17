#pragma once


#include "base/base/base.h"


#ifndef __cplusplus
   #error ca API requires C++ compilation (use a .cpp suffix)
#endif


#ifdef _BOOT_STATIC
   #define CLASS_DECL_BOOT
#elif defined(_BOOT_LIBRARY)
   #define CLASS_DECL_BOOT  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_BOOT  CLASS_DECL_IMPORT
#endif


class application;


#if defined(MACOS)
#include "boot/macos/ca_os_internal.h"
#elif defined(LINUX)
#include "boot/linux/ca_os_internal.h"
#elif defined(METROWIN)
#include "boot/metrowin/ca_os_internal.h"
#elif defined(WINDOWS)
#include "boot/windows/ca_os_internal.h"
#elif defined(ANDROID)
#include "boot/android/ca_os_internal.h"
#endif





#include "boot/vms/vms.h"


#include "boot_logic.h"


#include "boot_cpu_architecture.h"


#include "boot_libc.h"


#include "boot_number.h"
#include "boot_keep_true.h"


#include "boot_debug.h"


#include "boot_printf.h"
#include "boot_sprintf.h"



#include "boot/simple_graphics/simple_graphics_first_phase.h"

#include "boot_math.h"

#include "boot_geometric_types.h"




#include "boot_launcher.h"


#ifdef METROWIN

#define BYESHYTOULA_STYLE_SOCKS

#else

#define BSD_STYLE_SOCKETS
#include "boot_small_ipca_channel.h"

#endif



#include "boot_library.h"

#include "boot_file_watcher.h"
#include "boot_file_watcher_impl.h"



/*#ifndef WINDOWS
#define VK_TAB 1
#define VK_RETURN 2
#define VK_BACK 3
#define VK_DELETE 6
#define VK_SPACE 9
#endif*/
#include "boot_window_graphics.h"

#include "boot_os.h"

//#include "boot/cross/cross.h"




#include "boot/simple_graphics/simple_graphics_second_phase.h"


#include "boot_simple_shell_launcher.h"



#include "boot_file_watcher_thread.h"
#include "boot_file_watcher_listener_thread.h"
#include "boot_async.h"


#include "boot/hotplugin/hotplugin.h"
#include "boot/simple_ui/simple_ui.h"
#include "boot/crypt/crypt.h"
#include "boot/spa/spa.h"

#include "boot/bsdiff/bsdiff.h"




#include "boot_message_loop.h"

#include "boot_url.h"

#include "boot_international.h"



#define return_(y, x) {y = x; return;}


extern "C"
{


   int32_t _c_lock_is_active(const char * pszName);
   int32_t _c_lock(const char * pszName, void ** pdata);
   int32_t _c_unlock(void ** pdata);


}


CLASS_DECL_BOOT string _ca_get_file_name(const char * psz, bool bCreate = false, int32_t * pfd = NULL);

CLASS_DECL_BASE string get_system_error_message(uint32_t dwError);

//#ifndef METROWIN
//#include <openssl/ssl.h>
//#endif



//#include "boot_user.h"

#include "zlib/zlib.h"

#include "app/appseed/boot/fontopus/fontopus.h"

#include "boot_simple_app.h"


