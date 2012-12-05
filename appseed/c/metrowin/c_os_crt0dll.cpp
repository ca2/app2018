// crt0tdll.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/13/06 (mv)

#include "framework.h"

#ifdef WINDOWS

bool WINAPI DllMain(HANDLE hInst, DWORD reason, LPVOID imp);


EXTERN_C bool WINAPI _DllMainCRTStartup(HANDLE hInst, DWORD reason, LPVOID imp)
{

	if (reason == DLL_PROCESS_ATTACH)
	{

      initialize_primitive_heap();
	   //_init_atexit();
	   _init_file();

      

#ifndef METROWIN
	   _initterm(__xc_a, __xc_z);
#endif

      if(!os_initialize())
         return FALSE;

	}

	bool ret = DllMain(hInst, reason, imp);

	if (reason == DLL_PROCESS_DETACH)
	{

      os_finalize();

      //_doexit();

      finalize_primitive_heap();

	}

	return ret;

}

#endif
