#include "framework.h"


#ifdef METROWIN

[Platform::MTAThread]
extern "C" int32_t APIENTRY
DllMain(HINSTANCE hInstance, uint32_t dwReason, LPVOID lpReserved)
{
   
   
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);


   if (dwReason == DLL_PROCESS_ATTACH)
   {


      ::OutputDebugStringW(L"::ca:: optca.dll :: initializing!\n");
      

   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {
   

      ::OutputDebugStringW(L"::ca:: optca.dll :: terminating!\n");


   }


   return 1;   // ok


}

#else 

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   
   
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);


   if (dwReason == DLL_PROCESS_ATTACH)
   {


      ::OutputDebugStringW(L"::ca:: optca.dll :: initializing!\n");
      

   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {
   

      ::OutputDebugStringW(L"::ca:: optca.dll :: terminating!\n");


   }


   return 1;   // ok


}

#endif
