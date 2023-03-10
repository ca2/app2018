#pragma once


#include <shellapi.h>
#include <wincrypt.h>

#include "framework.h"


#ifndef CUBE

CLASS_DECL_AURA WINBOOL _001DefaultDllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

#endif

#include "windows_file.h"
#include "windows_multithreading.h"
#include "windows_utils.h"
#include "windows_graphics.h"

#include "windows_itemidlist.h"
#include "aura/aura/os/windows_common/windows_common_comptr.h"


char get_drive_letter(const char * lpDevicePath);

CLASS_DECL_AURA void set_main_thread(HANDLE hThread);
CLASS_DECL_AURA void set_main_thread_id(UINT uiThread);


CLASS_DECL_AURA HANDLE get_main_thread();
CLASS_DECL_AURA UINT   get_main_thread_id();

#ifdef __cplusplus

CLASS_DECL_AURA string get_command_line(HANDLE handleProcess);

CLASS_DECL_AURA string get_display_error(uint32_t NTStatusMessage);

CLASS_DECL_AURA void attach_thread_input_to_main_thread(bool bAttach = true);

#endif

CLASS_DECL_AURA HMONITOR GetUiMonitorHandle(HWND hwnd);
CLASS_DECL_AURA HMONITOR GetPrimaryMonitorHandle();
CLASS_DECL_AURA bool GetPrimaryMonitorRect(LPRECT lprect);

#ifdef __cplusplus
class memory;
CLASS_DECL_AURA string key_to_char(WPARAM wparam, LPARAM lparam);
CLASS_DECL_AURA string read_resource_as_string_dup(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
CLASS_DECL_AURA bool read_resource_as_memory_dup(memory & m, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);

#endif









CLASS_DECL_AURA int WinRegGetValueW(HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData);

#include "windows.inl"










CLASS_DECL_AURA HFONT wingdi_CreatePointFont(int nPointSize, const char * lpszFaceName, HDC hdc, LOGFONTW* lpLogFont = NULL);

CLASS_DECL_AURA HFONT wingdi_CreatePointFontIndirect(LOGFONTW* lpLogFont, HDC hdc);










