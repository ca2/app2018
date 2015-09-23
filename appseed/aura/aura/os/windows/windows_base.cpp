//#include "framework.h"
//#include "aura/node/windows/windows.h"
//#include "windows.h"
//#include <VersionHelpers.h>
#undef new
#define min MIN
#define max MAX
#include <gdiplus.h>
#undef min
#undef max
#define new AURA_NEW
#include <ddeml.h>

void __term_threading();
void __term_windowing();


CLASS_DECL_AURA int32_t __cdecl _memory_type(const void * p);

Gdiplus::GdiplusStartupInput *   g_pgdiplusStartupInput     = NULL;
Gdiplus::GdiplusStartupOutput *  g_pgdiplusStartupOutput    = NULL;
DWORD_PTR                        g_gdiplusToken             = NULL;
DWORD_PTR                        g_gdiplusHookToken         = NULL;


typedef int
(WINAPI * LPFN_ChangeWindowMessageFilter)(
    UINT message,
    DWORD dwFlag);


LPFN_ChangeWindowMessageFilter g_pfnChangeWindowMessageFilter = NULL;

typedef  
LSTATUS
( APIENTRY * LPFN_RegGetValueW) (
    HKEY hkey,
    LPCWSTR lpSubKey,
    LPCWSTR lpValue,
    uint32_t dwFlags,
    LPDWORD pdwType,
	 PVOID pvData,
    LPDWORD pcbData
    );

LPFN_RegGetValueW g_pfnRegGetValueW = NULL;

CLASS_DECL_AURA thread_int_ptr < int_ptr >                             t_iCoInitialize;

CLASS_DECL_AURA thread_int_ptr < HRESULT > t_hresultCoInitialize;



bool defer_co_initialize_ex()
{

   if(t_iCoInitialize != FALSE)
      return true;

   t_hresultCoInitialize = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);

   if(FAILED(t_hresultCoInitialize))
   {

      ::output_debug_string("Failed to ::CoInitializeEx(NULL, COINIT_MULTITHREADED) at __node_pre_init");

      return false;

   }

   t_iCoInitialize = TRUE;

   return true;

}

bool __node_aura_pre_init()
{
   

   OutputDebugStringW(L"__node_aura_pre_init\n");

   xxdebug_box("__node_aura_pre_init","box",MB_OK);
   g_pgdiplusStartupInput     = new Gdiplus::GdiplusStartupInput();
   g_pgdiplusStartupOutput    = new Gdiplus::GdiplusStartupOutput();
   g_gdiplusToken             = NULL;
   g_gdiplusHookToken         = NULL;

   g_pgdiplusStartupInput->SuppressBackgroundThread = TRUE;

   Gdiplus::Status statusStartup = GdiplusStartup(&g_gdiplusToken,g_pgdiplusStartupInput,g_pgdiplusStartupOutput);

   if(statusStartup != Gdiplus::Ok)
   {

      simple_message_box(NULL,"Gdiplus Failed to Startup. ca cannot continue.","Gdiplus Failure",MB_ICONERROR);

      return 0;

   }




   statusStartup = g_pgdiplusStartupOutput->NotificationHook(&g_gdiplusHookToken);


   if(statusStartup != Gdiplus::Ok)
   {

      simple_message_box(NULL,"Gdiplus Failed to Hook. ca cannot continue.","Gdiplus Failure",MB_ICONERROR);

      return 0;

   }


   return true;

}

bool __node_aura_pos_init()
{

   _set_purecall_handler(_ca2_purecall);

   HMODULE hmoduleUser32 = ::LoadLibrary("User32");
   g_pfnChangeWindowMessageFilter = (LPFN_ChangeWindowMessageFilter) ::GetProcAddress(hmoduleUser32, "ChangeWindowMessageFilter");


   HMODULE hmoduleAdvApi32 = ::LoadLibrary("AdvApi32");
   g_pfnRegGetValueW = (LPFN_RegGetValueW) ::GetProcAddress(hmoduleAdvApi32, "RegGetValueW");




   return true;

} 


bool __node_aura_pre_term()
{



   return true;

}

bool __node_aura_pos_term()
{
   g_pgdiplusStartupOutput->NotificationUnhook(g_gdiplusHookToken);


   ::Gdiplus::GdiplusShutdown(g_gdiplusToken);


   OutputDebugStringW(L"draw2d_gdiplus.dll terminating!\n");

   //::CoUninitialize();

   return true;

}


int WinRegGetValueW(HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{

   if(g_pfnRegGetValueW != NULL)
   {
      return g_pfnRegGetValueW(hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
   }
   else
   {
      LSTATUS lstatus = RegQueryValueExW(hkey, lpSubKey, NULL, pdwType, (LPBYTE) pvData, pcbData);
      if(lstatus == ERROR_SUCCESS)
      {
         if(pvData != NULL && (dwFlags & RRF_RT_REG_SZ) != 0 && *pdwType == REG_SZ)
         {
            ((WCHAR *)pvData)[*pcbData] = L'\0';
         }
      }
      return lstatus;
   }
}





string key_to_char(WPARAM wparam, LPARAM lparam)
{



   unichar wsz[32];

   BYTE baState[256];

   for(int i = 0; i < 256; i++)
   {
      baState[i] = (BYTE) GetAsyncKeyState(i);
   }

   if((GetAsyncKeyState(VK_SHIFT) & 0x80000000) != 0)
   {
      baState[VK_SHIFT] |= 0x80;
   }


   int32_t iRet = ToUnicodeEx((UINT) wparam, (UINT) lparam, baState, wsz, 32, 0, GetKeyboardLayout(GetCurrentThreadId()));

   if(iRet > 0)
   {

      wsz[iRet] = L'\0';

      string str;

      str = ::str::international::unicode_to_utf8(wsz);

      if((GetAsyncKeyState(VK_CAPITAL) & 0x0001) != 0)
      {
         if((GetAsyncKeyState(VK_SHIFT) & 0x80000000) != 0)
         {
            str.make_lower();
         }
         else
         {
            str.make_upper();
         }
      }
      else
      {
         if((GetAsyncKeyState(VK_SHIFT) & 0x80000000) != 0)
         {
            str.make_upper();
         }
         else
         {
            str.make_lower();
         }
      }

         

      return str;

   }

   return "";

}

 string get_system_error_message(uint32_t dwError)
{

   LPWSTR lpBuffer;

   HMODULE Hand = NULL;

   if(!FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      dwError,
      0,
      (LPWSTR) &lpBuffer,
      1,
      NULL))
   {

      HMODULE Hand = ::LoadLibrary("NTDLL.DLL");

      if(!FormatMessageW(
         FORMAT_MESSAGE_ALLOCATE_BUFFER |
         FORMAT_MESSAGE_FROM_SYSTEM |
         FORMAT_MESSAGE_FROM_HMODULE,
         Hand,
         dwError,
         0,
         (LPWSTR) &lpBuffer,
         1,
         NULL))
      {
         FreeLibrary(Hand);
         return "";
      }

   }

   string str(lpBuffer);

   LocalFree(lpBuffer);
   
   if(Hand != NULL)
   {
      FreeLibrary(Hand);
   }

   return str;

}




/*

// This example checks if the operating system is of "NT-type"
// (which means Windows NT, 2000, XP).
int GetVersion_ex1()
{
   DWORD    dwVersion = GetVersion();
   // Get major and minor version numbers of Windows
   WORD loword = LOWORD(dwVersion);
   int lowbyte = LOBYTE(loword);
   int hibyte = HIBYTE(loword);

   debug_print("Window major version = %d and minor version = %d\n", lowbyte, hibyte);

   if (!(dwVersion & 0x80000000))                // Windows NT, 2000, XP
      return 1;
   else         // Windows 95, 98, ME
      return 0;
}


*/


BEGIN_EXTERN_C


int_bool is_windows_98_or_lesser()
{

//#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WIN7) // Windows 7 or greater
#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_LONGHORN) // Windows 7 or greater

   return FALSE;

#else

   OSVERSIONINFO osversioninfo;

   osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   if (!GetVersionEx(&osversioninfo))
      return 0;

   return
      osversioninfo.dwPlatformId == VER_PLATFORM_WIN32s
      || (osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && ((osversioninfo.dwMajorVersion == 4 && osversioninfo.dwMinorVersion <= 10) || osversioninfo.dwMajorVersion < 4));

#endif

}

int_bool is_windows_nt()
{

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WINXP) // winxp or greater

   return IsWindowsXPOrGreater();

#else

   return !(GetVersion() & 0x80000000);

#endif

}

int_bool is_windows_7_or_lower()
{
   
   return !is_windows_8_or_greater();

}

int_bool is_windows_8_or_greater()
{

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WINXP) // winxp or greater

   return IsWindows8OrGreater();

#else

   OSVERSIONINFO osversioninfo;

   osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   if(!GetVersionEx(&osversioninfo))
      return 0;

   throw todo(get_app());

   return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT && osversioninfo.dwMajorVersion >= 6 && osversioninfo.dwMinorVersion >= 2;

#endif

}

int_bool is_windows_vista_or_greater()
{

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WINXP) // winxp or greater

   return IsWindowsVistaOrGreater();

#else

   OSVERSIONINFO osversioninfo;

   osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   if (!GetVersionEx(&osversioninfo))
      return 0;

   return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT && osversioninfo.dwMajorVersion >= 6;

#endif

}


int_bool is_windows_xp_or_greater()
{

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WINXP) // winxp or greater

   return IsWindowsXPOrGreater();

#else

   OSVERSIONINFO osversioninfo;

   osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   if (!GetVersionEx(&osversioninfo))
      return 0;

   return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT && (osversioninfo.dwMajorVersion > 5 || (osversioninfo.dwMajorVersion == 5 && osversioninfo.dwMinorVersion >= 1);

#endif

}


int_bool is_windows_2000_or_greater()
{

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WINXP) // winxp or greater

   return IsWindowsXPOrGreater();

#else

   OSVERSIONINFO osversioninfo;

   osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   if (!GetVersionEx(&osversioninfo))
      return 0;

   return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT && osversioninfo.dwMajorVersion >= 5;

#endif

}


int_bool is_windows_nt_lesser_than_2000()
{

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WINXP) // winxp or greater

   return FALSE;

#else

   OSVERSIONINFO osversioninfo;
   
   osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   if (!GetVersionEx(&osversioninfo))
      return 0;

   return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT && osversioninfo.dwMajorVersion < 5;

#endif

}

int_bool is_windows_native_unicode()
{

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WINXP) // winxp or greater

   return TRUE;

#else

   static int_bool s_bNativeUnicode = -1;

   if (bNativeUnicode == -1)
   {

      DWORD dwVersion = GetVersion();

      // get the Windows version.

      DWORD dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
      DWORD dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

      // get the build number.

      DWORD dwBuild;

      if (dwVersion < 0x80000000)              // Windows NT
         dwBuild = (DWORD)(HIWORD(dwVersion));
      else if (dwWindowsMajorVersion < 4)      // Win32s
         dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
      else                                     // Windows Me/98/95
         dwBuild = 0;

      if (dwVersion < 0x80000000)              // Windows NT
         s_bNativeUnicode = TRUE;
      else if (dwWindowsMajorVersion < 4)      // Win32s
         s_bNativeUnicode = FALSE;
      else                                     // Windows Me/98/95
         s_bNativeUnicode = FALSE;

   }

   return bNativeUnicode;

#endif

}



/////////////////////////////////////////////////////////////////////////////
// stop on a specific memory request

// Obsolete API
/*
void CLASS_DECL_AURA __set_alloc_stop(LONG lRequestNumber)
{
_CrtSetBreakAlloc(lRequestNumber);
}
*/

/*
// -- true if block of exact size, allocated on the heap
// -- set *plRequestNumber to request number (or 0)
bool CLASS_DECL_AURA __is_memory_block(const void * pData, UINT nBytes,
LONG* plRequestNumber)
{
return _CrtIsMemoryBlock(pData, nBytes, plRequestNumber, NULL, NULL);
}

*/
END_EXTERN_C

#ifdef DEBUG


CLASS_DECL_AURA int32_t __cdecl _CrtDumpMemoryLeaks()
{
   return TRUE;
}

CLASS_DECL_AURA int32_t DECL_C _check_memory()
{
   return 1;
}


CLASS_DECL_AURA bool  __check_memory()
{

   return _check_memory() != FALSE;

}

void __cdecl __crt_dump_client(void * pvData,size_t nBytes)
{
   char sz[1024];
   try
   {

      if(_memory_type(pvData) != ___CLIENT_BLOCK)
         return;

      //      object * pca = (object * ) pvData;

      object * pobject = NULL;

      /*for(int32_t i = 0; i < 256; i++)
      {
      try
      {
      pobject = dynamic_cast < object * > ((object *)&((int_ptr *)pca)[i]);
      }
      catch(std::__non_rtti_object & e)
      {
      pobject = NULL;
      }
      catch(...)
      {
      pobject = NULL;
      }
      if(pobject != NULL)
      break;
      }*/

      if(pobject == NULL)
      {
         C_RUNTIME_ERRORCHECK_SPRINTF(_snprintf_s(sz,_countof(sz),_countof(sz) - 1,"unknown object at $%p, %u bytes long\n",pvData,nBytes));
      }
      else if(g_dumpcontext.GetDepth() > 0)
      {
         // long form
         pobject->dump(g_dumpcontext);
         g_dumpcontext << "\n";
      }
      if(false) // else
      {
         object & obj = *pobject;
         // int16_t form
         C_RUNTIME_ERRORCHECK_SPRINTF(_snprintf_s(sz,_countof(sz),_countof(sz) - 1,"a %hs object at $%p, %u bytes long\n",typeid(obj).name(),pvData,nBytes));
         g_dumpcontext << sz;
      }
   }
   catch(std::__non_rtti_object & e)
   {
      g_dumpcontext << "_gen::CrtdumpClient __non_rtti_object ";
      g_dumpcontext << e.what();
   }
   catch(...)
   {
      // int16_t form for trashed objects
      sprintf_s(sz,_countof(sz),"faulted while dumping object at $%p, %u bytes long\n",pvData,nBytes);
      g_dumpcontext << sz;
   }
}

int32_t __cdecl __crt_report_hook(int32_t nRptType,__in char *szMsg,int32_t* pResult)
{
   // no hook on asserts or when m_pFile is NULL
   if(nRptType == _CRT_ASSERT || g_dumpcontext.m_pfile == NULL)
      return FALSE;

   ASSERT(pResult != NULL);
   if(pResult == NULL)
      throw invalid_argument_exception(get_thread_app());

   ASSERT(szMsg != NULL);
   if(szMsg == NULL)
      throw invalid_argument_exception(get_thread_app());

   // non-NULL m_pFile, so go through g_dumpcontext for the message
   *pResult = FALSE;
   g_dumpcontext << szMsg;
   //Allow other report hooks to be called.
   return FALSE;
}

CLASS_DECL_AURA int32_t __cdecl _memory_type(const void * p)
{
   return ___CLIENT_BLOCK;
}

#endif







// out-of-line cleanup called from inline __exception_link destructor
CLASS_DECL_AURA void __try_cleanup()
{
}

// special out-of-line implementation of THROW_LAST (for auto-delete behavior)
void CLASS_DECL_AURA __throw_last_cleanup()
{
}






namespace core
{

#if defined( _CUSTOM_THROW )  // You can define your own throw hresult_exception to throw a custom exception.

   CLASS_DECL_AURA void WINAPI atl_throw_impl(HRESULT hr)
   {
      TRACE(atlTraceException,0,"throw hresult_exception: hr = 0x%x\n",hr);
#ifdef _AFX
      if(hr == E_OUTOFMEMORY)
      {
         throw memory_exception();
      }
      else
      {
         //      ::core::ThrowOleException( hr );
      }
#else
      throw atl_exception(hr);
#endif
   };

#endif


   // Throw a atl_exception with th given HRESULT
#if !defined( _CUSTOM_THROW )  // You can define your own throw hresult_exception

   //CLASS_DECL_AURA void WINAPI atl_throw_impl(HRESULT hr)
   //{
   //   TRACE("throw hresult_exception: hr = 0x%x\n", hr);
   //  throw hresult_exception(hr);
   /*   ::OutputDebugString("throw hresult_exception");
   char sz[200];
   sprintf(sz, "0x%s", hr);
   ::OutputDebugString(sz);
   ::OutputDebugString("\n");
   //TRACE(trace::category_Exception, 0, "throw hresult_exception: hr = 0x%x\n", hr );
   ASSERT( false );
   DWORD dwExceptionCode;
   switch(hr)
   {
   case E_OUTOFMEMORY:
   dwExceptionCode = STATUS_NO_MEMORY;
   break;
   default:
   dwExceptionCode = EXCEPTION_ILLEGAL_INSTRUCTION;
   }
   atl_raise_exception((DWORD)dwExceptionCode);*/
   //}
#endif


} // namespace core






// __is_valid_atom() returns TRUE if the passed parameter is 
// a valid local or global atom.

bool __is_valid_atom(ATOM nAtom)
{
   char sBuffer[256];
   if(GetAtomName(nAtom,sBuffer,_countof(sBuffer)))
   {
      return TRUE;
   }
   DWORD dwError = GetLastError();
   if(dwError == ERROR_INSUFFICIENT_BUFFER || dwError == ERROR_MORE_DATA)
   {
      return TRUE;
   }
   if(GlobalGetAtomName(nAtom,sBuffer,_countof(sBuffer)))
   {
      return TRUE;
   }
   dwError = GetLastError();
   if(dwError == ERROR_INSUFFICIENT_BUFFER || dwError == ERROR_MORE_DATA)
   {
      return TRUE;
   }
   return FALSE;
}

// __is_valid_address() returns TRUE if the passed parameter is 
// a valid representation of a local or a global atom within a const char *.

bool __is_valid_atom(const char * psz)
{
   return HIWORD(psz) == 0L && __is_valid_atom(ATOM(LOWORD(psz)));
}






int get_processor_count()
{
   
   SYSTEM_INFO sysinfo;
   GetSystemInfo(&sysinfo);

   return sysinfo.dwNumberOfProcessors;

}