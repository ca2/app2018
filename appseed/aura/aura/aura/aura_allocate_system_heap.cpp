//#include "framework.h"

//#include <crtdbg.h>

memdleak_block * s_pmemdleakList;
extern mutex * g_pmutgen;
extern thread_pointer < memdleak_block > t_plastblock;

#define PREFER_MALLOC 0

// uint32_t aligned allocation

#if defined(WINDOWSEX) && !defined(__VLD) && !defined(__MCRTDBG)

HANDLE g_system_heap()
{

   static HANDLE s_hSystemHeap = HeapCreate(0, 0, 0);

   return s_hSystemHeap;

}

#endif

extern CLASS_DECL_AURA ::exception::engine * g_ee;

critical_section * g_pmutexSystemHeap = NULL;

#if !MEMDLEAK && !defined(__MCRTDBG)
void * system_heap_alloc(size_t size)
{

   if(size > 16 * 1024)
   {

      ::OutputDebugStringA("allocating memory block greater than 16kb\n");

   }

   //synch_lock lock(g_pmutexSystemHeap);

//#if ZEROED_ALLOC
  // byte * p = (byte *) ::HeapAlloc(g_hSystemHeap, HEAP_ZERO_MEMORY, ((size + 4 + 3) & ~3));
//#else  // let constructors and algorithms initialize... "random initialization" of not initialized :-> C-:!!

   void * p;
#if defined(__VLD) || defined(__MCRTDBG)

   p = malloc(size);

#elif defined(WINDOWSEX) && !PREFER_MALLOC

   cslock csl(g_pmutexSystemHeap);

   p = ::HeapAlloc(g_system_heap(), 0, size);

#else

   p = ::malloc(size);

#endif
//#endif

   if(p == NULL)
   {

      throw_memory_exception();

   }

   memset(p, 0x00, size);

   return p;

}

#else

void * system_heap_alloc_dbg(size_t size, int nBlockUse, const char * pszFileName, int iLine)
{

   void * p;

   size_t nAllocSize = size + sizeof(size_t) + sizeof(memdleak_block);

   memdleak_block * pblock;

#if defined(WINDOWSEX) && !PREFER_MALLOC

   {
      cslock csl(g_pmutexSystemHeap);

      pblock = (memdleak_block *) ::HeapAlloc(g_system_heap(), 0, nAllocSize);

   }

#else

   pblock = (memdleak_block *) malloc(nAllocSize);

#endif

p = pblock;


   //pblock->m_iBlockUse     = nBlockUse;
   //if (g_ee == NULL)
   //{
   //   pblock->m_pszCallStack = NULL;
   //   pblock->m_pszFileName = NULL;
   //}
   //else
   //{
   //   string strCallStack;
   //   g_ee->stack_trace(1);
   //   pblock->m_pszCallStack = strdup(g_ee->_strS);
   //   pblock->m_pszFileName = strdup(pszFileName == NULL ? "" : pszFileName); // not trackable, at least think so certainly causes memory leak
   //}


   //pblock->m_iLine         = iLine;

   //pblock->m_iSize         = nAllocSize;

   //synch_lock lock(g_pmutgen);

   //pblock->m_pprevious                 = NULL;

   //pblock->m_pnext                     = s_pmemdleakList;

   //if(s_pmemdleakList != NULL)
   //{

   //   s_pmemdleakList->m_pprevious     = pblock;

   //}

   //s_pmemdleakList                     = pblock;

   //t_plastblock = pblock;

   //lock.unlock();

   //size_t * psize = (size_t *) &pblock[1];

   //psize[0] = 0;

   //memset(&psize[1], 0, size);

   //return &psize[1];

   if(p == NULL)
   {

      throw_memory_exception();

   }

   memset(p, 0x00, size);

   return p;

}

#endif

#if !MEMDLEAK && !defined(__MCRTDBG)

void * system_heap_realloc(void * p, size_t size)
{

#if defined(__VLD) || defined(__MCRTDBG)

   return realloc(p, size);

#elif defined(WINDOWSEX) && !PREFER_MALLOC

   cslock lock(g_pmutexSystemHeap);

   return ::HeapReAlloc(g_system_heap(), 0, p, size);

#else

   return ::realloc(p, size);

#endif

}

#else

void * system_heap_realloc_dbg(void * p,  size_t size, int32_t nBlockUse, const char * pszFileName, int32_t iLine)
{


   //size_t nAllocSize = size + sizeof(size_t) + sizeof(memdleak_block);

   //size_t * psizeOld = &((size_t *)p)[-1];

   //memdleak_block * pblock = &((memdleak_block *)psizeOld)[-1];

   //synch_lock lock(g_pmutgen);

   //if(s_pmemdleakList == pblock)
   //{
   //   s_pmemdleakList = pblock->m_pnext;
   //   s_pmemdleakList->m_pprevious = NULL;
   //}
   //else
   //{
   //   pblock->m_pprevious->m_pnext = pblock->m_pnext;
   //   if(pblock->m_pnext != NULL)
   //   {
   //      pblock->m_pnext->m_pprevious = pblock->m_pprevious;
   //   }
   //}

   //if(pblock->m_pszCallStack)
   //::free((void *)pblock->m_pszCallStack);
   //if(pblock->m_pszFileName)
   //::free((void *) pblock->m_pszFileName);

   //size_t * psizeNew = NULL;

#if defined(WINDOWSEX) && !PREFER_MALLOC
   {
      cslock csl(g_pmutexSystemHeap);

//      pblock = (memdleak_block *) ::HeapReAlloc(g_system_heap(), 0, pblock, size + sizeof(memdleak_block));
      p = (memdleak_block *) ::HeapReAlloc(g_system_heap(), 0, p, size);

   }

#else

   p = (memdleak_block *) ::realloc(p, size + sizeof(memdleak_block));

#endif


   //psizeNew = (size_t *) &pblock[1];

   //psizeNew[0] = nAllocSize;

   //pblock->m_iBlockUse     = nBlockUse;
   //if (g_ee == NULL)
   //{
   //   pblock->m_pszCallStack = NULL;
   //   pblock->m_pszFileName = NULL;
   //}
   //else
   //{
   //   string strCallStack;
   //   g_ee->stack_trace(1);
   //   pblock->m_pszCallStack = strdup(g_ee->_strS);
   //   pblock->m_pszFileName = strdup(pszFileName == NULL ? "" : pszFileName);
   //}

   //
   //pblock->m_iLine         = iLine;
   //pblock->m_iSize         = nAllocSize;


   //pblock->m_pprevious                 = NULL;
   //pblock->m_pnext                     = s_pmemdleakList;
   //if(s_pmemdleakList != NULL)
   //{
   //   s_pmemdleakList->m_pprevious        = pblock;
   //}
   //s_pmemdleakList                     = pblock;
   //lock.unlock();


   //return &psizeNew[1];

   return p;

//   size_t * psize = (size_t *) g_pheap->realloc_dbg(&((size_t *)pvoidOld)[-1], ((size_t *)pvoidOld)[-1], size + sizeof(size_t), nBlockUse, szFileName, iLine);
  // psize[0] = size + sizeof(size_t);
   //return &psize[1];

}
#endif



void system_heap_free(void * p)
{

#if defined(__VLD) || defined(__MCRTDBG)

   return free(p);

#else

#if !MEMDLEAK

#if defined(WINDOWSEX) && !PREFER_MALLOC

   cslock lock(g_pmutexSystemHeap);

   if(!::HeapFree(g_system_heap(), 0, p))
   {

      uint32_t dw = ::GetLastError();

      ::OutputDebugString("system_heap_free : Failed to free memory");

   }

#else

   try
   {

      ::free(p);

   }
   catch(...)
   {

      ::output_debug_string("system_heap_free : Failed to free memory");

   }

#endif

#else
/*
   size_t * psize = &((size_t *)p)[-1];

   memdleak_block * pblock = &((memdleak_block *)psize)[-1];

   if(s_pmemdleakList != NULL)
   {

      synch_lock lock(g_pmutgen);

      if(s_pmemdleakList == pblock)
      {
         s_pmemdleakList = pblock->m_pnext;
         s_pmemdleakList->m_pprevious = NULL;
      }
      else
      {
         pblock->m_pprevious->m_pnext = pblock->m_pnext;
         if(pblock->m_pnext != NULL)
         {
            pblock->m_pnext->m_pprevious = pblock->m_pprevious;
         }
      }
      if(pblock->m_pszCallStack)
      ::free((void *)pblock->m_pszCallStack);
      if(pblock->m_pszFileName)
      ::free((void *) pblock->m_pszFileName);

   }
*/
#if defined(WINDOWSEX) && !PREFER_MALLOC
   {
      cslock csl(g_pmutexSystemHeap);

      // ::HeapFree(g_system_heap(), 0, pblock);
      ::HeapFree(g_system_heap(), 0, p);

   }

#else
   ::free(p);
#endif

#endif

#endif

}








::count get_mem_info(int32_t ** ppiUse, const char *** ppszFile, const char *** ppszCallStack, int32_t ** ppiLine, int64_t ** ppiSize)
{

#if MEMDLEAK

   throw simple_exception(get_thread_app(), "plex_heap_alloc_array::get_mem_info member function is available only with \"memdleak\" builds - MEMDLEAK defined");


   synch_lock lock(g_pmutgen);

   memdleak_block * pblock = s_pmemdleakList;

   ::count ca = 0;

   while(pblock != NULL)
   {

      ca++;

      pblock = pblock->m_pnext;

   }


   int32_t * piUse =(int32_t *)  malloc(sizeof(int32_t) * ca);
   const char ** pszFile = (const char **) malloc(sizeof(const char *) * ca);
   const char ** pszCallStack = (const char **)malloc(sizeof(const char *) * ca);
   int32_t * piLine =(int32_t *)  malloc(sizeof(int32_t) * ca);
   int64_t * piSize =(int64_t *)  malloc(sizeof(int64_t) * ca);

   index i = 0;

   pblock = s_pmemdleakList;

   while(pblock != NULL && i < ca)
   {
      piUse[i] = pblock->m_iBlockUse;
      pszFile[i] = pblock->m_pszFileName== NULL ? NULL : _strdup(pblock->m_pszFileName);
      pszCallStack[i] = pblock->m_iStack <= 0 ? NULL :_strdup(g_ee->stack_trace(pblock->m_puiStack, pblock->m_iStack));
      piLine[i] = pblock->m_iLine;
      piSize[i] = pblock->m_iSize;

      i++;

      pblock = pblock->m_pnext;



   }

   *ppiUse = piUse;
   *ppszFile = pszFile;
   *ppszCallStack = pszCallStack;
   *ppiLine = piLine;
   *ppiSize = piSize;


   return ca;

#endif

   return 0;

}
//typedef DWORD64[64]
//::count get_mem_info2(int32_t ** ppiUse, const char *** ppszFile, DWORD64 ** ppuiStack[64], int64_t ** ppiStack, int32_t ** ppiLine, int64_t ** ppiSize)
//{
//
//#ifndef MEMDLEAK
//
//   throw simple_exception(get_thread_app(), "plex_heap_alloc_array::get_mem_info member function is available only with \"memdleak\" builds - MEMDLEAK defined");
//
//#endif
//
//   synch_lock lock(g_pmutgen);
//
//   memdleak_block * pblock = s_pmemdleakList;
//
//   ::count ca = 0;
//
//   while (pblock != NULL)
//   {
//
//      ca++;
//
//      pblock = pblock->m_pnext;
//
//   }
//
//
//   int32_t * piUse = (int32_t *)malloc(sizeof(int32_t) * ca);
//   const char ** pszFile = (const char **)malloc(sizeof(const char *) * ca);
//   DWORD64 ** puiStack[64] = (DWORD64 **[64])malloc(sizeof(DWORD64[64]) * ca);
//   int64_t * piStack = (int64_t *)malloc(sizeof(int64_t) * ca);
//   int32_t * piLine = (int32_t *)malloc(sizeof(int32_t) * ca);
//   int64_t * piSize = (int64_t *)malloc(sizeof(int64_t) * ca);
//
//   index i = 0;
//
//   pblock = s_pmemdleakList;
//
//   while (pblock != NULL && i < ca)
//   {
//      piUse[i] = pblock->m_iBlockUse;
//      pszFile[i] = pblock->m_pszFileName == NULL ? NULL : _strdup(pblock->m_pszFileName);
//      memcpy(puiStack[i], pblock->m_puiStack, pblock->m_iStack * sizeof(DWORD64));
//      piStack[i] = pblock->m_iStack;
//      piLine[i] = pblock->m_iLine;
//      piSize[i] = pblock->m_iSize;
//
//      i++;
//
//      pblock = pblock->m_pnext;
//
//
//
//   }
//
//   *ppiUse = piUse;
//   *ppszFile = pszFile;
//   *ppuiStack = puiStack;
//   *ppiStack = piStack;
//   *ppiLine = piLine;
//   *ppiSize = piSize;
//
//
//   return ca;
//
//}
//

#ifdef MEMDLEAK
void set_last_block_file_name(const char * psz)
{

   //t_plastblock->m_pszFileName = strdup(psz == NULL ? "" : psz);

}
#endif


