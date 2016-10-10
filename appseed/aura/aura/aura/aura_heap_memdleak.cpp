

BEGIN_EXTERN_C


#define AXIS_MEMORY_MANAGEMENT TRUE


#if AXIS_MEMORY_MANAGEMENT


#if !defined(__VLD) && !defined(__MCRTDBG)
void * aligned_memory_alloc(size_t size)
{

   void * p;
#if defined(MCHECK)

	p = aligned_alloc(64, size);

#elif MEMDLEAK

   p = unaligned_memory_alloc(size);

#else

   if(g_pheap == NULL)
   {

       void * pbase = system_heap_alloc(heap_memory::aligned_provision_get_size(size));

       if (pbase == NULL)
       {

          return NULL;

       }

      p = heap_memory::aligned(pbase, size, 128);

   }
   else
   {

      if (heap_memory::aligned_provision_get_size(size) == 831
         && heap_memory::aligned_provision_get_size(size) < 1024)
      {
         output_debug_string("*");
      }

       void * pbase = g_pheap->_alloc(heap_memory::aligned_provision_get_size(size));

       if (pbase == NULL)
       {

          return NULL;

       }

       p = heap_memory::aligned(pbase, size, 0);

   }

#endif

   //zero(p, size);

   return p;

}

void * unaligned_memory_alloc(size_t size)
{

   void * p;

#if defined(MCHECK)

   p =  malloc(size);

#elif MEMDLEAK
   size_t nAllocSize = size + sizeof(memdleak_block);

   memdleak_block * pblock;

   pblock = (memdleak_block *) ::system_heap_alloc(nAllocSize);

   pblock->m_iBlockUse = 0;

   if (g_ee == NULL)
   {
//      pblock->m_puiStack = NULL;
      pblock->m_iStack = 0;
      pblock->m_pszFileName = NULL;
}
   else
   {
      //string strCallStack;
      //g_ee->stack_trace(1);
      pblock->m_iStack = sizeof(pblock->m_puiStack) / sizeof(pblock->m_puiStack[0]);
      g_ee->backtrace(pblock->m_puiStack, pblock->m_iStack);
      pblock->m_pszFileName = NULL;
      //pblock->m_pszFileName = strdup(pszFileName); // not trackable, at least think so certainly causes memory leak
   }

   ::lemon::set_maximum(pblock->m_uiLine);

   pblock->m_size = nAllocSize;

   synch_lock lock(g_pmutgen);

   pblock->m_pprevious = NULL;

   pblock->m_pnext = s_pmemdleakList;

   if (s_pmemdleakList != NULL)
   {

      s_pmemdleakList->m_pprevious = pblock;

   }

   s_pmemdleakList = pblock;

   t_plastblock = pblock;

   lock.unlock();

   p = &pblock[1];


#else

#if defined(APPLEOS) || defined(LINUX)

   p = aligned_memory_alloc(size);

#else

   void * pbase = g_pheap->_alloc(heap_memory::unaligned_provision_get_size(size));

   if (pbase == NULL)
   {

       return NULL;

   }
   else
   {

      p = heap_memory::unaligned(pbase, size, 2);

   }

#endif

#endif

   //zero(p, size);

   return p;

}


void * aligned_memory_alloc_dbg(size_t size, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{

   void * p;

#if defined(MCHECK)

   p = aligned_alloc(64, size);

#elif MEMDLEAK

   p = unaligned_memory_alloc(size);

#else

   UNREFERENCED_PARAMETER(nBlockUse);
   UNREFERENCED_PARAMETER(szFileName);
   UNREFERENCED_PARAMETER(nLine);

   //TODO: to do the dbg version
   //byte * p = (byte *) _system_heap_alloc_dbg(nSize + ALIGN_BYTE_COUNT + 32, nBlockUse, szFileName, nLine);
    if(g_pheap == NULL)
    {

        void * pbase = system_heap_alloc(heap_memory::aligned_provision_get_size(size));

        if (pbase == NULL)
        {

            return NULL;

        }

        p = heap_memory::aligned(pbase, size, 129);

    }
    else
    {

        void * pbase = g_pheap->alloc_dbg(heap_memory::aligned_provision_get_size(size), nBlockUse, szFileName, nLine);

        if (pbase == NULL)
        {

           return NULL;

        }

        p = heap_memory::aligned(pbase, size, 1);

    }

#endif

    //zero(p, size);

    return p;

}


void * unaligned_memory_alloc_dbg(size_t size, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{

   void * p;

#if defined(MCHECK)

   return malloc(size);

#elif MEMDLEAK

   p = unaligned_memory_alloc(size);

#else


#ifdef APPLEOS

   p = aligned_memory_alloc(size);

#else

   UNREFERENCED_PARAMETER(nBlockUse);
   UNREFERENCED_PARAMETER(szFileName);
   UNREFERENCED_PARAMETER(nLine);

   //TODO: to do the dbg version
   //byte * p = (byte *) _system_heap_alloc_dbg(nSize + ALIGN_BYTE_COUNT + 32, nBlockUse, szFileName, nLine);
   void * pbase = g_pheap->alloc_dbg(heap_memory::unaligned_provision_get_size(size), nBlockUse, szFileName, nLine);

   if (pbase == NULL)
   {

      return NULL;

   }

   p = heap_memory::unaligned(pbase, size, 3);

#endif

#endif

   //zero(p, size);

   return p;

}


#endif

#if !defined(__VLD) && !defined(__MCRTDBG)

BEGIN_EXTERN_C


#if !defined(MCHECK)

#undef memory_alloc

void * memory_alloc(size_t size)
{

#if defined(APPLEOS)

   return aligned_memory_alloc(size);

#else

   return unaligned_memory_alloc(size);

#endif

}

#undef memory_alloc

#endif


void * memory_alloc_no_track(size_t size)
{

#if defined(MCHECK) || defined(__VLD) || defined(__MCRTDBG)

   return memory_alloc(size);

#else

#if defined(APPLEOS)

   return aligned_memory_alloc(size);

#else

   return unaligned_memory_alloc(size);

#endif

#endif

}




void * memory_calloc(size_t size, size_t bytes)
{

   return memory_alloc(size * bytes);

}


void * memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{
#ifdef MCHECK
   return memory_alloc(nSize);
#else
   return unaligned_memory_alloc_dbg(nSize, nBlockUse, szFileName, nLine);
#endif

}

#if !defined(MCHECK) && !defined(__VLD) && !defined(__MCRTDBG)

void * memory_realloc(void * pmemory, size_t nSize)
{

   return memory_realloc_dbg(pmemory, nSize, 0, NULL, -1);

}
#endif




END_EXTERN_C

void * memory_realloc_dbg(void * pmemory, size_t size, int32_t nBlockUse, const char * szFileName, int32_t nLine)
{

#if defined(__VLD)

   return realloc(pmemory, size);

#elif defined(__MCRTDBG)

   return _realloc_dbg(pmemory, size, _NORMAL_BLOCK, szFileName, nLine);

#elif defined(MCHECK)

   return memory_realloc(pmemory, size);

#elif MEMDLEAK
   size_t nAllocSize = size + sizeof(memdleak_block);

   memdleak_block * pblock = &((memdleak_block *)pmemory)[-1];

   synch_lock lock(g_pmutgen);

   if (s_pmemdleakList == pblock)
   {
      s_pmemdleakList = s_pmemdleakList->m_pnext;
      s_pmemdleakList->m_pprevious = NULL;
   }
   else
   {
      pblock->m_pprevious->m_pnext = pblock->m_pnext;
      if (pblock->m_pnext != NULL)
      {
         pblock->m_pnext->m_pprevious = pblock->m_pprevious;
      }
   }

   //if (pblock->m_pszFileName)
     // ::system_heap_free((void *)pblock->m_pszFileName);
   //if (pblock->m_puiStack)
     // ::system_heap_free((void *)pblock->m_puiStack);

   size_t * psizeNew = NULL;

   pblock = (memdleak_block *) ::system_heap_realloc(pblock, size + sizeof(memdleak_block));

   pblock->m_iBlockUse = nBlockUse;
   if (g_ee == NULL)
   {
      pblock->m_iStack = 0;
      pblock->m_pszFileName = NULL;

   }
   else
   {
      //string strCallStack;
      //g_ee->stack_trace(1);
      pblock->m_iStack = sizeof(pblock->m_puiStack) / sizeof(pblock->m_puiStack[0]);
      g_ee->backtrace(pblock->m_puiStack, pblock->m_iStack);
      pblock->m_pszFileName = NULL;
   }

   ::lemon::set_maximum(pblock->m_uiLine);

   pblock->m_size = nAllocSize;

   pblock->m_pprevious = NULL;

   pblock->m_pnext = s_pmemdleakList;

   if (s_pmemdleakList != NULL)
   {

      s_pmemdleakList->m_pprevious = pblock;

   }

   s_pmemdleakList = pblock;
   lock.unlock();


   return &pblock[1];

#else

   if (pmemory == NULL)
      return memory_alloc_dbg(size, nBlockUse, szFileName, nLine);

   byte blockuse = heap_memory::heap_get_block_use(pmemory);

   size_t sizeOld = heap_memory::heap_get_size(pmemory);

   void * pbase = NULL;

   if(blockuse == 0) // aligned
   {

      pbase = g_pheap->_realloc(heap_memory::base_get(pmemory),heap_memory::aligned_provision_get_size(size),heap_memory::aligned_provision_get_size(sizeOld),ALIGN_BYTE_COUNT);

   }
   else if(blockuse == 1) // aligned
   {

      //TODO: to do the dbg version

      pbase = g_pheap->realloc_dbg(heap_memory::base_get(pmemory),heap_memory::aligned_provision_get_size(size),heap_memory::aligned_provision_get_size(sizeOld),ALIGN_BYTE_COUNT,nBlockUse,szFileName,nLine);

   }
   else if(blockuse == 128) // aligned
   {

      pbase = realloc(heap_memory::base_get(pmemory),heap_memory::aligned_provision_get_size(size));

   }
   else if(blockuse == 129) // aligned
   {

      //TODO: to do the dbg version

      pbase = realloc(heap_memory::base_get(pmemory),heap_memory::aligned_provision_get_size(size));

   }
   else if(blockuse == 2) // unaligned
   {

      pbase = g_pheap->_realloc(heap_memory::base_get(pmemory),heap_memory::unaligned_provision_get_size(size),heap_memory::unaligned_provision_get_size(sizeOld),0);

   }
   else if(blockuse == 3) // unaligned
   {

      //TODO: to do the dbg version

      pbase = g_pheap->realloc_dbg(heap_memory::base_get(pmemory),heap_memory::unaligned_provision_get_size(size),heap_memory::unaligned_provision_get_size(sizeOld),0,nBlockUse,szFileName,nLine);

   }
   else
   {

      return NULL;

   }

   if(pbase == NULL)
   {

      return NULL;

   }

   if(blockuse == 0 || blockuse == 1 || blockuse == 128 || blockuse == 129) // aligned
   {

      return heap_memory::aligned(pbase,size,blockuse);

   }
   else
   {

      return heap_memory::unaligned(pbase,size,blockuse);

   }

   return NULL;

#endif

}

#if !defined(MCHECK) && !defined(__VLD) && !defined(__MCRTDBG)

void memory_free(void * pmemory)
{

   return memory_free_dbg(pmemory, 0);

}

#endif





void memory_free_dbg(void * pmemory, int32_t iBlockType)
{

#if defined(__VLD) || defined(MCHECK) || defined(__MCRTDBG)

   memory_free(pmemory);

#elif MEMDLEAK
   memdleak_block * pblock = &((memdleak_block *)pmemory)[-1];

   synch_lock lock(g_pmutgen);

   if (s_pmemdleakList == pblock)
   {
      s_pmemdleakList = s_pmemdleakList->m_pnext;
      if (s_pmemdleakList != NULL)
      {
         s_pmemdleakList->m_pprevious = NULL;

      }
   }
   else
   {
      if (pblock->m_pprevious != NULL)
      {
         pblock->m_pprevious->m_pnext = pblock->m_pnext;
      }
      if (pblock->m_pnext != NULL)
      {
         pblock->m_pnext->m_pprevious = pblock->m_pprevious;
      }
   }
   //if (pblock->m_pszFileName)
     // ::system_heap_free((void *)pblock->m_pszFileName);
   //if (pblock->m_puiStack)
     // ::system_heap_free((void *)pblock->m_puiStack);

      return ::system_heap_free(pblock);


#else

   heap_memory * pheap =  ::heap_memory::heap_get(pmemory);

   void * pbase = (void *)(((int_ptr)pmemory) - pheap->m_back);


   pheap->check_padding_after();


   if(pheap->m_blockuse == 0)
   {

      g_pheap->_free(pbase,heap_memory::aligned_provision_get_size(pheap->m_size));

   }
   else if(pheap->m_blockuse == 1)
   {

      //TODO: to do the dbg version

      g_pheap->free_dbg(pbase,heap_memory::aligned_provision_get_size(pheap->m_size));

   }
   else if(pheap->m_blockuse == 128)
   {

      system_heap_free(pbase);

   }
   else if(pheap->m_blockuse == 129)
   {

      //TODO: to do the dbg version

      system_heap_free(pbase);

   }
   else if(pheap->m_blockuse == 2)
   {

      g_pheap->_free(pbase,heap_memory::unaligned_provision_get_size(pheap->m_size));

   }
   else if(pheap->m_blockuse == 3)
   {

      //TODO: to do the dbg version

      g_pheap->free_dbg(pbase,heap_memory::unaligned_provision_get_size(pheap->m_size));

   }
   else
   {

      ::output_debug_string("wrong free");

   }
#endif

}


#endif

size_t memory_size(void * pmemory)
{

   return memory_size_dbg(pmemory, _NORMAL_BLOCK);

}

size_t memory_size_dbg(void * pmemory, int32_t iBlockType)
{

#if defined(__VLD) || defined(__MCRTDBG)

   return _msize(pmemory);

#elif MEMDLEAK

   memdleak_block * pblock = &((memdleak_block *)pmemory)[-1];

   return pblock->m_size;

#elif defined(MCHECK)

   return malloc_usable_size(pmemory);

#else

   if (pmemory == NULL)
      return 0;

   return heap_memory::heap_get_size(pmemory);

#endif

}













BEGIN_EXTERN_C

#define MEMDLEAK_DEFAULT 0

int g_iGlobalMemdleakEnabled;

thread_int_ptr < int_ptr > t_iMemdleak;

CLASS_DECL_AURA int  memdleak_enabled()
{

   return global_memdleak_enabled() && (t_iMemdleak == (int_ptr) 1 || t_iMemdleak == (int_ptr)0);

}

CLASS_DECL_AURA void memdleak_enable(int enable)
{

   if (!enable)
   {

      t_iMemdleak = 1;

   }
   else
   {

      t_iMemdleak = -1;

   }


}

CLASS_DECL_AURA int  global_memdleak_enabled()
{

   if (g_iGlobalMemdleakEnabled == 0)
   {

      uint32_t dwFileAttributes = GetFileAttributesW(L"C:\\archive\\ca2\\config\\system\\memdleak.txt");

      if (dwFileAttributes != INVALID_FILE_ATTRIBUTES && (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
      {

         g_iGlobalMemdleakEnabled = 1;

      }
      else
      {

         if (MEMDLEAK_DEFAULT)
         {

            g_iGlobalMemdleakEnabled = 1;

         }
         else
         {

            g_iGlobalMemdleakEnabled = -1;

         }

      }

   }

   return g_iGlobalMemdleakEnabled == 1;

}


END_EXTERN_C




