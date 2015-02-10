//#include "framework.h"

/*BEGIN_EXTERN_C

BOOL WINAPI openssl_DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

END_EXTERN_C*/

#ifdef METROWIN
[MTAThread]
#endif
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   
   //Sleep(30000);

//   ASSERT(FALSE);

   xxdebug_box("base.dll DllMain (0)", "box", MB_OK);

   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);


   //openssl_DllMain(hInstance, dwReason, lpReserved);


   if (dwReason == DLL_PROCESS_ATTACH)
   {

      OutputDebugStringW(L"base.dll initializing!\n");

      xxdebug_box("base.dll DllMain", "box", MB_OK);
      
   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {

      OutputDebugStringW(L"base.dll terminating!\n");

   }

   return 1;   // ok

}


