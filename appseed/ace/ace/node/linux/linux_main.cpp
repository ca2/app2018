#include "framework.h"
#include "ace/node/ansios/ansios.h"
#include "linux.h"



CLASS_DECL_ACE int32_t ca2_main();


//CLASS_DECL_ACE extern fixed_alloc_array * g_pfixedallocaWstring;


//CLASS_DECL_ACE fixed_alloc_array * new_wstring_manager();


void CLASS_DECL_ACE __cdecl _ca2_purecall()
{

    throw simple_exception(::get_thread_app());

}


void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers)
{

   UNREFERENCED_PARAMETER(uiCode);

   UNREFERENCED_PARAMETER(ppointers);

}




