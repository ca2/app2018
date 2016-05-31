#pragma once


#include "metrowin_base_file.h"
#include "metrowin_multithreading.h"
//#include "metrowin_utils.h"
#ifdef __cplusplus
#include "aura/aura/os/windows/windows_comptr.h"
#endif

#ifndef METROWIN

CLASS_DECL_AURA void WINAPI Sleep(DWORD dwMilliseconds);

#endif


/*#define GetAValue(rgb)      (LOBYTE((rgb)>>24))
#define ARGB(a, r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))*/


//CLASS_DECL_AURA int_bool os_initialize();
//CLASS_DECL_AURA int_bool os_finalize();


CLASS_DECL_AURA int_bool main_initialize();
CLASS_DECL_AURA int_bool main_finalize();


CLASS_DECL_AURA void set_main_thread(HTHREAD hThread);
CLASS_DECL_AURA void set_main_thread_id(UINT uiThread);


CLASS_DECL_AURA HTHREAD get_main_thread();
CLASS_DECL_AURA UINT   get_main_thread_id();


CLASS_DECL_AURA void attach_thread_input_to_main_thread(int_bool bAttach);


//CLASS_DECL_AURA DWORD WINAPI WaitForSingleObject( _In_ HANDLE hHandle, _In_ DWORD dwMilliseconds );

BEGIN_EXTERN_C

   
//CLASS_DECL_AURA char* getenv(const char* name);
//CLASS_DECL_AURA HANDLE WINAPI CreateMutex(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCSTR lpName  );
END_EXTERN_C
//CLASS_DECL_AURA HANDLE WINAPI CreateMutexW(    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,    _In_ BOOL bInitialOwner,    _In_opt_ LPCWSTR lpName    );


#ifdef __cplusplus

CLASS_DECL_AURA string normalize_path(const char * lpcszPath);


#endif

