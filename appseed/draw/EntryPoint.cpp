#include "stdafx.h"
#include "html.h"



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			TRACE("html.dll DLL_PROCESS_ATTACH");
		}
		break;
	case DLL_THREAD_ATTACH:
		{
			TRACE("html.dll DLL_THREAD_ATTACH");
		}
		break;
	case DLL_THREAD_DETACH:
		{
			TRACE("html.dll DLL_THREAD_DETACH");
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			TRACE("html.dll DLL_PROCESS_DETACH");
		}
		break;
	}
	return TRUE;
}

