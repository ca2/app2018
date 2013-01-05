#include "framework.h"



int32_t c_dll_main(HINSTANCE hinstance, uint32_t dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if(dwReason == DLL_PROCESS_ATTACH)
	{

      initialize_primitive_heap();

      if(!os_initialize())
         return FALSE;

      ::OutputDebugString("::ca2:: c.dll :: initializing!\n");

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
      
      ::OutputDebugString("::ca2:: c.dll :: terminating!\n");

      os_finalize();

      finalize_primitive_heap();


	}
	return 1;   // ok
}



/*extern "C" int32_t APIENTRY _DllMainCRTStartup(HINSTANCE hinstance, uint32_t dwReason, LPVOID lpReserved)
{
   return c_dll_main(hinstance, dwReason, lpReserved);
}

*/

extern "C"
BOOL WINAPI DllMain(HINSTANCE hinstance, uint32_t dwReason, LPVOID lpReserved)
{
   return c_dll_main(hinstance, dwReason, lpReserved);
}


extern "C" void __chkstk()
{
}







