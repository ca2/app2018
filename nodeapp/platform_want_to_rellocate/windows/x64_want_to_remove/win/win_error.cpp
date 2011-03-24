#include "StdAfx.h"

string get_error_message(DWORD dwError)
{
   LPWSTR lpBuffer;
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		0,
		(LPWSTR) &lpBuffer,
		1,
		NULL);
   wstring wstr = lpBuffer;
   LocalFree(lpBuffer);
   return gen::international::unicode_to_utf8(wstr);
}
