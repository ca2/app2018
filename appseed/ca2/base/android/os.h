#pragma once

DWORD GetTickCount();




#define WINAPI



void RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ulong_ptr *lpArguments);


namespace ca2
{


   class file;


} // namespace ca2


typedef ::file::file * HFILE;



#include "os_allocate_debug.h"
#include "os_debug.h"


CLASS_DECL_ca2 errno_t fopen_s(FILE ** pFile, const char * pFileName, const char * mode);

#include "os_multithreading.h"
