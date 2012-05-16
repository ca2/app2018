// libct.h

#pragma once




#ifdef WIN32


#include <tchar.h>




BEGIN_EXTERN_C

extern TCHAR *_argv[];
int _init_args();
void _term_args();

typedef void (__cdecl *_PVFV)();
extern _PVFV __xc_a[], __xc_z[];    /* C++ initializers */

CLASS_DECL_c void _initterm(_PVFV *pfbegin, _PVFV *pfend);
CLASS_DECL_c void _init_atexit();
CLASS_DECL_c void _doexit();
CLASS_DECL_c void _init_file();

END_EXTERN_C



#include "c_heap.h"

#endif
