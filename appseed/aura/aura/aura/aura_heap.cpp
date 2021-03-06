#include "framework.h"

#include "aura_plex_heap1.h"

#include "aura_plex_heap_impl1.h"

#ifdef RASPBIAN

#define AXIS_MEMORY_MANAGEMENT TRUE

#else

#define AXIS_MEMORY_MANAGEMENT TRUE

#endif


#if !AXIS_MEMORY_MANAGEMENT


#include "aura_heap_bare.cpp"


#else


#ifdef LINUX


#include <malloc.h>


#endif

#include "align_byte_count.h"


plex_heap_alloc_array * g_pheap = NULL;


#ifdef MCHECK


#include "aura_heap_mcheck.cpp"


#elif defined(_VLD)


#include "aura_heap_vld.cpp"


#elif defined(__MCRTDBG)


#include "aura_heap_mcrtdbg.cpp"


#elif MEMDLEAK


#include "aura_heap_memdleak.cpp"


#else


#include "aura_heap_std.cpp"


#endif


#endif


#if !defined(MCHECK) && !defined(_VLD) && !defined(__MCRTDBG)

const int heap_memory::m_iPaddingAfter = 16;

#endif





c_class c_class::s_cclass;


c_class::c_class()
{
}

c_class::c_class(const c_class &)
{
}

c_class::~c_class()
{
}


namespace heap
{


   void heap_base::zero()
   {

      if(size() > 0 && m_p != NULL)
      {

         ::zero(m_p, size());

      }

   }


} // namespace heap



