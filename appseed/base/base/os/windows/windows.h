#pragma once


#include <shellapi.h>
#include <wincrypt.h>



#include "windows_file.h"
#include "windows_thread.h"
#include "windows_utils.h"


#define GetAValue(rgb)      (LOBYTE((rgb)>>24))



CLASS_DECL_BASE int_bool os_initialize();
CLASS_DECL_BASE int_bool os_finalize();


CLASS_DECL_BASE int_bool main_initialize();
CLASS_DECL_BASE int_bool main_finalize();


CLASS_DECL_BASE void set_main_thread(HANDLE hThread);
CLASS_DECL_BASE void set_main_thread_id(UINT uiThread);


CLASS_DECL_BASE HANDLE get_main_thread();
CLASS_DECL_BASE UINT   get_main_thread_id();

#ifdef __cplusplus

CLASS_DECL_BASE void attach_thread_input_to_main_thread(bool bAttach = true);

#endif


#ifdef __cplusplus

CLASS_DECL_BASE string key_to_char(WPARAM wparam, LPARAM lparam);
CLASS_DECL_BASE string read_resource_as_string_dup(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);

#endif

#define MESSAGE MSG
#define LPMESSAGE LPMSG


CLASS_DECL_BASE int WinRegGetValueW(HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData);

