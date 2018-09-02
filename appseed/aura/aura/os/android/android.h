#pragma once



//#include <unistd.h>
//#include "_stdint.h"
//#include <asm/byteorder.h>

BEGIN_EXTERN_C


// The lockf() function is not available on Android; we translate to flock().
#define F_LOCK LOCK_EX
#define F_ULOCK LOCK_UN
CLASS_DECL_AURA int lockf(int fd, int cmd, off_t ignored_len);

CLASS_DECL_AURA void swab(const void *from, void*to, ssize_t n);
CLASS_DECL_AURA void* lfind( const void * key, const void * aura, size_t * num, size_t width, int (*fncomparison)(const void *, const void * ) );

#undef lconv
#undef localeconv

struct lconv
{

   const char * decimal_point;

};

struct lconv *localeconv(void);

double rint(double x);


END_EXTERN_C


oswindow get_capture();
oswindow set_capture(oswindow window);
WINBOOL release_capture();
oswindow set_focus(oswindow window);
oswindow get_focus();
oswindow get_window(oswindow window, int iParentHood);
oswindow get_active_window();
oswindow set_active_window(oswindow window);
WINBOOL destroy_window(oswindow window);
oswindow get_desktop_window();



