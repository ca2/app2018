﻿#include "framework.h"
#include <stdio.h>


namespace user
{


   CLASS_DECL_AURA void init_windowing();
   CLASS_DECL_AURA void term_windowing();


} // namespace user

//CLASS_DECL_AURA aura_str_pool * s_paurastrpool = NULL;


::aura::system * create_aura_system(app_core * pappcore)
{

   return g_pfn_create_system(pappcore);

}


BEGIN_EXTERN_C
CLASS_DECL_AURA int g_bAura = 0;
END_EXTERN_C


BEGIN_EXTERN_C

CLASS_DECL_AURA uint32_t g_dwStartTime = 0;


END_EXTERN_C

extern "C"
CLASS_DECL_AURA void debug_print(const char * pszFormat,...)
{

   if (is_ptr_null(pszFormat, 1024))
   {

      return;

   }

   //if (strstr(pszFormat, "%") == NULL)
   //{
   //
   //   ::output_debug_string(wstring(pszFormat));
   //
   //   return;

   //}

   va_list argList;

   va_start(argList,pszFormat);

   char sz[4096];

   vsnprintf(sz,sizeof(sz),pszFormat,argList);

   ::output_debug_string(sz);

   va_end(argList);

}




void __post_quit_message(int32_t nExitCode)
{

#ifdef WINDOWSEX

   ::PostQuitMessage(nExitCode);

#else

   ::multithreading::post_quit(get_app());

#endif

}

string_map < ::aura::PFN_GET_NEW_LIBRARY, ::aura::PFN_GET_NEW_LIBRARY  > * g_pmapLibrary = NULL;

extern "C"
CLASS_DECL_AURA string_map < ::aura::PFN_GET_NEW_LIBRARY, ::aura::PFN_GET_NEW_LIBRARY  > & __library()
{

   return *g_pmapLibrary;

}

extern "C"
CLASS_DECL_AURA::aura::PFN_GET_NEW_LIBRARY get_library_factory(const char * psz)
{

   return __library()[psz];

}

extern "C"
CLASS_DECL_AURA void register_library(const char * psz, ::aura::PFN_GET_NEW_LIBRARY p)
{
   __library()[psz] = p;

}


int g_iAuraRefCount = 0;


CLASS_DECL_AURA int get_aura_init()
{

   return g_iAuraRefCount;

}


CLASS_DECL_AURA ::aura::system * aura_create_aura_system(app_core * pappcore)
{

   return new ::aura::system(NULL, pappcore, NULL);

}


CLASS_DECL_AURA int_bool defer_aura_init()
{

   g_iAuraRefCount++;

   if(g_iAuraRefCount > 1)
      return TRUE;

   if(!aura_init())
      return FALSE;

   s_paurastrpool = new aura_str_pool();

   ::user::init_windowing();

   g_bAura = 1;

   return TRUE;

}


CLASS_DECL_AURA int_bool defer_aura_term()
{

   g_iAuraRefCount--;

   if(g_iAuraRefCount >= 1)
      return TRUE;

   ::user::term_windowing();

   g_bAura = 0;

   ::aura::del(s_paurastrpool);

   aura_term();

   return TRUE;

}

#ifdef WINDOWS

void set_simple_output_debug_string_a();

void set_extended_output_debug_string_a();

void set_simple_output_debug_string_w();

void set_extended_output_debug_string_w();

#endif

bool aura_init()
{

   ::aura::static_start::init();

   if(!__node_aura_pre_init())
      return false;

   //::aura::static_start::init();

   __init_threading_count();

   ::multithreading::init_multithreading();

//   ::hthread::s_pmutex = new mutex();

   // ::hthread::s_pptra = new ref_array <  hthread >();


   if(!__node_aura_pos_init())
      return false;


#ifdef WINDOWS

   set_extended_output_debug_string_a();

   set_extended_output_debug_string_w();

#endif

   g_pfn_create_system = &aura_create_aura_system;

   return true;

}


bool aura_term()
{

   __wait_threading_count(::millis((5000) * 8));

   __node_aura_pre_term();

   if (g_axisontermthread)
   {

      g_axisontermthread();

   }

   on_term_thread();

   ::multithreading::term_multithreading();

   __term_threading_count();

#ifdef WINDOWS

   set_simple_output_debug_string_a();

   set_simple_output_debug_string_w();

#endif

   processor_cache_oriented_destroy_all_memory_pools();

   __node_aura_pos_term();

   ::aura::static_start::term();

   return true;

}




void dappy(const char * psz)
{

   //debug_print("app._ : %s : %s\n",_argv[2],psz);
   //debug_print("hello!!    : %s\n",psz);
   //::output_debug_string("hello!!    : " + string(psz) + "\n");

}




::map < void *,void *,::aura::application *,::aura::application * > * g_pmapAura = NULL;

void set_aura(void * p,::aura::application * papp)
{

   g_pmapAura->set_at(p,papp);

}

::aura::application * get_aura(void * p)
{

   return g_pmapAura->operator [](p);

}




// note (casey) : according to cmuratori (Casey Muratori) "non initialized" (for "static" variables) is zero initialized
CLASS_DECL_AURA LPFN_CREATE_SYSTEM g_pfn_create_system;

CLASS_DECL_AURA const char * g_pszCooperativeLevel;

CLASS_DECL_AURA int g_iDerivedApplication = 0;


int __cdecl debug_report(int, char const *, int, char const *, char const *,...)
{

   return 1;

}


int __cdecl debug_report(int, wchar_t const *, int, wchar_t const *, wchar_t const *, ...)
{

   return 1;

}


CLASS_DECL_AURA void writeln(const char * psz)
{

   output_debug_string(string(psz) + "\n");

}



BEGIN_EXTERN_C

CLASS_DECL_AURA void c_function_call(void * p)
{

   const char * psz = (const char *) p;


}

END_EXTERN_C


//template < >
//CLASS_DECL_AURA void function_call(const ::object * p)
//{
//
//   const char * psz = reinterpret_cast< const char * > (p);
//
//}






CLASS_DECL_AURA int is_ptr_null(const void * p, size_t s)
{

   return (((size_t) p) < s);

}






