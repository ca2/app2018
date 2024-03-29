﻿#pragma once

#include <shlobj.h>
#include <Security.h>
#include <shlobj.h>


#include "windows_exception.h"

string get_error_message(DWORD dwError);


CLASS_DECL_AURA bool __initialize();
CLASS_DECL_AURA void __set_alloc_stop(LONG lRequestNumber);
CLASS_DECL_AURA void __throw_last_cleanup();
CLASS_DECL_AURA void __try_cleanup();


#include "windows_dir.h"
#include "windows_file_system.h"

#include "windows_folder_watch.h"
#include "windows_factory_exchange.h"
#include "windows_uac_tools.h"
#include "windows_thread.h"
#include "windows_file.h"
#include "windows_crypto.h"
#include "windows_ip_enum.h"


#define NODE_THREAD(pthread) (dynamic_cast < ::windows::thread * > (dynamic_cast < thread * >(pthread)))



#include "windows_command.h"



void CLASS_DECL_AURA __cdecl _ca2_purecall();
void CLASS_DECL_AURA __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers);
bool CLASS_DECL_AURA __windows_init();
int32_t CLASS_DECL_AURA __windows_main(::aura::system * psystem, ::command::command * pmaininitdata);


#include "windows_console_window.h"



#include "windows_file_find.h"
#include "windows_file.h"
#include "windows_shell.h"
#include "windows_stdio_file.h"
#include "windows_registry.h"
#include "windows_os.h"


// Sanity checks for ATOMs
CLASS_DECL_AURA bool __is_valid_atom(ATOM nAtom);
CLASS_DECL_AURA bool __is_valid_atom(const char * psz);


/////////////////////////////////////////////////////////////////////////////
// locale-invariant comparison helpers till CRT gets that support
inline int32_t __invariant_stricmp(const char *pszLeft,const char *pszRight)
{
#ifdef WINDOWSEX
   return ::CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),
                           NORM_IGNORECASE,
                           pszLeft,
                           -1,
                           pszRight,
                           -1) - CSTR_EQUAL;
#else
   return stricmp_dup(pszLeft,pszRight);
#endif
}

inline int32_t __invariant_stricmp(const unichar *pwszLeft,const unichar *pwszRight)
{
#ifdef WINDOWSEX
   return ::CompareStringW(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),
                           NORM_IGNORECASE,
                           pwszLeft,
                           -1,
                           pwszRight,
                           -1) - CSTR_EQUAL;
#else
   return wcsicmp_dup(pwszLeft,pwszRight);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Portability abstractions

#define __set_dialog_control_id(oswindow, nID)     SetWindowLong(oswindow, GWL_ID, nID)
#define __set_dialog_control_id_(oswindow, nID)     oswindow->SetWindowLong(GWL_ID, nID)
#define __get_dialog_control_id(oswindow)         ((UINT)(WORD)::GetDlgCtrlID(oswindow))
#define __get_dialog_control_id_(oswindow)         ((UINT)(WORD)oswindow->GetDlgCtrlId())


#include "windows_pipe.h"
#include "windows_process.h"










namespace windows
{


   class thread;

   class windows
   {
      int32_t function();
   };

   CLASS_DECL_AURA HINSTANCE   load_library(const char * lpsz);
   CLASS_DECL_AURA bool        shell_get_special_folder_path(oswindow oswindow,::file::path &str,int32_t csidl,bool fCreate);
   CLASS_DECL_AURA ::file::path  shell_get_special_folder_path(int32_t csidl, bool fCreate = true, oswindow oswindow = NULL);
   CLASS_DECL_AURA DWORD       get_file_attributes(const char * lpFileName);
   CLASS_DECL_AURA bool        create_directory(const char * lpPathName,LPSECURITY_ATTRIBUTES lpSecurityAttributes);
   CLASS_DECL_AURA DWORD       get_current_directory(string & str);
   CLASS_DECL_AURA DWORD       get_temp_path(string & str);
   CLASS_DECL_AURA LONG        reg_query_value(HKEY hkey,const char * lpszSubKey,string & str);
   CLASS_DECL_AURA HICON       extract_icon(HINSTANCE hInst,const char * lpszExeFileName,UINT nIconIndex);
   CLASS_DECL_AURA bool        delete_file(const char * lpFileName);
   CLASS_DECL_AURA int32_t     get_menu_string(HMENU hMenu,UINT uIDItem,string & str,UINT flags);
   CLASS_DECL_AURA void        time_to_filetime(::aura::application * papp,const ::datetime::time& time,LPFILETIME pFileTime);


} // namespace windows



// Placed on frame for EXCEPTION linkage, or ::exception::base cleanup
struct CLASS_DECL_AURA __exception_link
{
   __exception_link* m_pLinkPrev;    // previous top, next in handler chain
   ::exception::base* m_pException;   // current exception (NULL in try block)

   __exception_link();       // for initialization and linking
   ~__exception_link()       // for cleanup and unlinking
   {
      __try_cleanup();
   };
};

// Exception global state - never access directly
struct CLASS_DECL_AURA __EXCEPTION_CONTEXT
{
   __exception_link* m_pLinkTop;

   // Note: most of the exception context is now in the __exception_link
};

#ifndef _PNH_DEFINED
typedef int32_t(__cdecl * _PNH)(size_t);
#define _PNH_DEFINED
#endif

_PNH CLASS_DECL_AURA __get_new_handler();
_PNH CLASS_DECL_AURA __set_new_handler(_PNH pfnNewHandler);
CLASS_DECL_AURA int32_t c_cdecl __new_handler(size_t nSize);

void CLASS_DECL_AURA __abort();





CLASS_DECL_AURA LONG delete_registry_tree_helper(HKEY hParentKey,const string & strKeyName);


CLASS_DECL_AURA sp(::aura::application) __get_app();
CLASS_DECL_AURA HINSTANCE __get_resource_handle();
CLASS_DECL_AURA void __set_resource_handle(HINSTANCE hInstResource);

CLASS_DECL_AURA HINSTANCE __get_resource_handle();
CLASS_DECL_AURA HINSTANCE __find_string_resource_handle(UINT nID);



CLASS_DECL_AURA int32_t app_main(::aura::system * psystem, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow);



#include "windows_tts_speaker.h"




