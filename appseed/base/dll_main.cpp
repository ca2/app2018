#include <windows.h>

#ifdef METROWIN
[Platform::MTAThread]
#endif
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   

   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);


   if (dwReason == DLL_PROCESS_ATTACH)
   {


      OutputDebugStringW(L"::ca2freetype.dll :: initializing!\n");
      

   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {


      OutputDebugStringW(L"::ca2freetype.dll :: terminating!\n");


   }

   return 1;   // ok

}


