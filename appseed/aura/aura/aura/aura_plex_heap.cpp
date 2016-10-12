//#include "framework.h"


#undef new


plex_heap * plex_heap::create(plex_heap*& pHead, uint_ptr nMax, uint_ptr cbElement)
{
   ASSERT(nMax > 0 && cbElement > 0);
   if (nMax == 0 || cbElement == 0)
   {
      throw invalid_argument_exception(get_thread_app());
   }

   plex_heap* p = (plex_heap*) system_heap_alloc(sizeof(plex_heap) + nMax * cbElement);

#ifdef DEBUG
   Free_check_pointer_in_cpp(p);
#endif
         // may throw exception
   p->pNext = pHead;
   pHead = p;  // change head (adds in reverse order for simplicity)
   return p;
}


void plex_heap::FreeDataChain()     // free this one and links
{

   plex_heap* p = this;
   while (p != NULL)
   {
      BYTE* bytes = (BYTE*) p;
      plex_heap* pNext = p->pNext;
      system_heap_free(bytes);
      p = pNext;
   }

}




plex_heap_alloc_sync::plex_heap_alloc_sync(UINT nAllocSize, UINT nBlockSize)
{

//   m_pprotect = new critical_section();

   if(nBlockSize <= 1)
      nBlockSize = 4;

   ASSERT(nAllocSize >= sizeof(node));
   ASSERT(nBlockSize > 1);

   if (nAllocSize < sizeof(node))
      nAllocSize = sizeof(node);
   if (nBlockSize <= 0)
      nBlockSize = 64;

   m_nAllocSize = nAllocSize;
   m_nBlockSize = nBlockSize;
   m_pnodeFree = NULL;
   m_pBlocks = NULL;
   m_iFreeHitCount = 0;
   m_pnodeLastBlock = NULL;

}


plex_heap_alloc_sync::~plex_heap_alloc_sync()
{

   FreeAll();

//   delete m_pprotect;

}


void plex_heap_alloc_sync::FreeAll()
{

   m_protect.lock();



   try
   {
      m_pBlocks->FreeDataChain();
      m_pBlocks = NULL;
      m_pnodeFree = NULL;
   }
   catch(...)
   {
   }


   m_protect.unlock();

}


void plex_heap_alloc_sync::NewBlock()
{

   if (m_pnodeFree == NULL)
   {
      size_t nAllocSize = m_nAllocSize;

      // add another block
      plex_heap* pNewBlock = plex_heap::create(m_pBlocks, m_nBlockSize, nAllocSize);


      if(nAllocSize == 1024)
      {

      //output_debug_string("plex_heap_alloc_sync::NewBlock() 1024");

      }

      // chain them into free list
      node* pNode = (node*)pNewBlock->data();
      // free in reverse order to make it easier to debug
      ((BYTE*&)pNode) += (nAllocSize * m_nBlockSize) - nAllocSize;
      for (int32_t i = m_nBlockSize-1; i >= 0; i--, ((BYTE*&)pNode) -= nAllocSize)
      {
         pNode->pNext = m_pnodeFree;
         m_pnodeFree = pNode;
      }
   }
   ASSERT(m_pnodeFree != NULL);  // we must have something
#ifdef DEBUG
   Free_check_pointer_in_cpp(m_pnodeFree);
#endif

}

plex_heap_alloc::plex_heap_alloc(UINT nAllocSize, UINT nBlockSize)
{

#if !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS) && !defined(ANDROID)

   uint32_t uiShareCount = ::get_current_process_maximum_affinity() + 1;

#else

   int32_t uiShareCount = 0;

#endif

   if(uiShareCount == 0)
      uiShareCount = 4;

   allocate(uiShareCount);

   for(int32_t i = 0; i < get_count(); i++)
   {
      set_at(i, new plex_heap_alloc_sync(nAllocSize, nBlockSize));
   }

   m_uiShareCount = uiShareCount;

   m_uiShareBound = uiShareCount - 1;

   m_uiAllocSize = nAllocSize;

   m_uiAlloc = 0;
   m_uiFree = 0;

}

plex_heap_alloc::~plex_heap_alloc()
{

   for(int32_t i = 0; i < get_count(); i++)
   {
      delete element_at(i);
   }

}

void plex_heap_alloc::FreeAll()
{

   for(int32_t i = 0; i < get_count(); i++)
   {

      try
      {
         element_at(i)->FreeAll();
      }
      catch(...)
      {
      }

   }

}


void plex_heap_alloc::pre_finalize()
{

   FreeAll();

   //m_pData = NULL; // FreeAll freed all data alloced
   //m_nSize = 0;
   //m_nMaxSize = 0;

}


extern plex_heap_alloc_array * g_pheap;

plex_heap_alloc_array::plex_heap_alloc_array()
{

   if(g_pheap == NULL)
   {

      g_pheap = this;

   }

//   ::zero(m_aa,sizeof(m_aa));
  // ::zero(m_bb,sizeof(m_bb));
   //::zero(m_aaSize,sizeof(m_aaSize));
   //::zero(m_bbSize,sizeof(m_bbSize));

//   m_iWorkingSize = 0;
//
//   m_aa[0] = 0;
//   m_bb[0] = get_size();
   add(new plex_heap_alloc(8,256));
   add(new plex_heap_alloc(16, 256));
   add(new plex_heap_alloc(24, 256));
   add(new plex_heap_alloc(32, 256));
   add(new plex_heap_alloc(48, 256));
//   m_bbSize[0] = last()->GetAllocSize();
//
//   m_bb[1] = get_size();
   add(new plex_heap_alloc(64, 128));
   add(new plex_heap_alloc(96, 128));
   add(new plex_heap_alloc(128, 128));
   add(new plex_heap_alloc(192, 128));
   add(new plex_heap_alloc(256, 128));
//   m_bbSize[1] = last()->GetAllocSize();
//   m_aaSize[0] = last()->GetAllocSize();
//
//   m_aa[1] = 2;
//   m_bb[2] = get_size();
   add(new plex_heap_alloc(384, 64));
   add(new plex_heap_alloc(512, 64));
   add(new plex_heap_alloc(768, 48));
   add(new plex_heap_alloc(1024, 48));
   add(new plex_heap_alloc(1024 * 2, 32));
//   m_bbSize[2] = last()->GetAllocSize();
//
//
//   m_bb[3] = get_size();
   add(new plex_heap_alloc(1024 * 4,32));
   add(new plex_heap_alloc(1024 * 8, 16));
   add(new plex_heap_alloc(1024 * 16, 16));
   add(new plex_heap_alloc(1024 * 32, 16));
   add(new plex_heap_alloc(1024 * 64, 16));
//   m_bbSize[3] = last()->GetAllocSize();
//   m_aaSize[1] = last()->GetAllocSize();
//
//
//   m_aa[2] = 4;
//   m_bb[4] = get_size();
   add(new plex_heap_alloc(1024 * 128,16));
#if defined(OS64BIT) && defined(LINUX)
   add(new plex_heap_alloc(1024 * 192, 16));
   add(new plex_heap_alloc(1024 * 256, 16));
   add(new plex_heap_alloc(1024 * 384, 16));
   add(new plex_heap_alloc(1024 * 512, 16));
   add(new plex_heap_alloc(1024 * 768, 16));
   add(new plex_heap_alloc(1024 * 1024, 16));
#endif
////   m_bbSize[4] = last()->GetAllocSize();
//
#if defined(OS64BIT) && defined(LINUX)
////   m_bb[5] = get_size();
   add(new plex_heap_alloc(1024 * 1024 * 2, 16));
   add(new plex_heap_alloc(1024 * 1024 * 4,16));
   add(new plex_heap_alloc(1024 * 1024 * 8,16));
   add(new plex_heap_alloc(1024 * 1024 * 16,16));
////   m_bbSize[5] = last()->GetAllocSize();
#endif
//   m_aaSize[2] = last()->GetAllocSize();
//
//   m_iWorkingSize = get_size();



}


plex_heap_alloc_array::~plex_heap_alloc_array()
{

   pre_finalize();

//   m_iWorkingSize = 0;

   for(index i = this->get_upper_bound(); i >= 0; i--)
   {

      delete this->element_at(i);

   }

   remove_all();

   if(g_pheap == this)
   {

      g_pheap = NULL;

   }

}

void plex_heap_alloc_array::pre_finalize()
{

   for (index i = this->get_upper_bound(); i >= 0; i--)
   {

      this->element_at(i)->pre_finalize();

   }

}



void * plex_heap_alloc_array::alloc_dbg(size_t size, int32_t nBlockUse, const char * pszFileName, int32_t iLine)
{

#if LAST_MEM_FILE_AND_LINE

   string str;

   str.Format("%s(%d)", pszFileName, iLine);

   size_t nAllocSize = size + sizeof(size_t) + 128;

   plex_heap_alloc * palloc = find(nAllocSize);

   size_t * psize = NULL;

   if(palloc != NULL)
   {

      psize = (size_t *) palloc->Alloc();

      psize[0] = nAllocSize;

   }
   else
   {

      psize = (size_t *) ::system_heap_alloc(nAllocSize);

      psize[0] = 0;

   }

   strncpy((char *) &psize[1], str.Mid(MAX(0, str.get_length() - 124)), 124);

   return ((byte *) &psize[1]) + 128;

#else

   return _alloc(size);

#endif



}


void plex_heap_alloc_array::free_dbg(void * p, size_t size)
{


#if LAST_MEM_FILE_AND_LINE


   size_t * psize = &((size_t *)((byte *)pvoid) - 128))[-1];

   if(*psize == 0)
   {

      return ::system_heap_free(psize);

   }

   plex_heap_alloc * palloc = find(*psize);

   if(palloc != NULL)
   {

      return palloc->Free(psize);

   }
   else
   {

      return ::system_heap_free(psize);

   }

#else

   return _free(p, size);

#endif

}


void * plex_heap_alloc_array::realloc_dbg(void * p,  size_t size, size_t sizeOld, int align, int32_t nBlockUse, const char * pszFileName, int32_t iLine)
{

#if LAST_MEM_FILE_AND_LINE
   string str;
   str.Format("%s(%d)", szFileName, iLine);
   size_t * psize = (size_t *) realloc(&((size_t *)(((byte *)pvoidOld) - 128))[-1], ((size_t *)(((byte *)pvoidOld) - 128))[-1], size + sizeof(size_t) + 128);
   psize[0] = size + sizeof(size_t) + 128;
   strncpy((char *) &psize[1], str.Mid(MAX(0, str.get_length() - 124)), 124);
   return ((byte *) &psize[1]) + 128;
#else

   return _realloc(p, size, sizeOld, align);

#endif

}






void * plex_heap_alloc_array::_realloc(void * p, size_t size, size_t sizeOld, int align)
{

   plex_heap_alloc * pallocOld = find(sizeOld);

   plex_heap_alloc * pallocNew = find(size);

   void * pNew = NULL;

   if (pallocOld == NULL && pallocNew == NULL)
   {

      pNew = (size_t *) ::system_heap_realloc(p, size);

   }
   else if (pallocOld == pallocNew)
   {

      pNew = p;

   }
   else
   {

      if (pallocNew != NULL)
      {

         pNew = (size_t *) pallocNew->Alloc();

      }
      else
      {

         pNew = (size_t *) ::system_heap_alloc(size);

      }

      if (align > 0)
      {

         int_ptr oldSize = (((int_ptr) p & ((int_ptr) align - 1)));

         if(oldSize > 0)
         {

            oldSize = sizeOld - (align - oldSize);

         }
         else
         {

            oldSize = sizeOld;

         }

         int_ptr newSize = (((int_ptr) pNew & ((int_ptr) align - 1)));

         if(newSize > 0)
         {

            newSize = size - (align - newSize);

         }
         else
         {

            newSize = size;

         }

         memcpy(
                (void *)(((int_ptr)pNew + align - 1) & ~((int_ptr)align - 1)),
                (void *)(((int_ptr)p + align - 1) & ~((int_ptr)align - 1)),
                MIN(oldSize, newSize));

      }
      else
      {

         memcpy(pNew, p, MIN(sizeOld, size));

      }

      if (pallocOld != NULL)
      {

         pallocOld->Free(p);

      }
      else
      {

         ::system_heap_free(p);

      }

   }

   return pNew;

}

void * g_pf1 = NULL;

void Free_check_pointer_in_cpp(void * p)
{

   if((byte *) p <  (byte *)  g_pf1)
   {
      debug_print("hit g_pf1");
   }
   if((dword_ptr) p   & 0x8000000000000000LLU)
   {
      debug_print("hit hiptr");
   }

}

#ifdef DEBUG
void plex_heap_alloc_array::_free(void * p,size_t size)
{

/*   int xxx = MIN(1024,size * 2 / 3);

   memset(&((byte *)p)[xxx],0xCD,size - xxx); // attempt to invalidate memory so it get unusable (as it should be after free). // but preserve first xxx bytes so vtable indicating object type may be eventually preserved for debugging
   */

   //memset(p,0xCD,size); // attempt to invalidate memory so it get unusable (as it should be after free).

   plex_heap_alloc * palloc = find(size);

   if(palloc != NULL)
   {

      if(palloc->m_uiAllocSize == 1024)
      {

         //output_debug_string("|");

      }

      return palloc->Free(p);

   }
   else
   {

      return ::system_heap_free(p);

   }

}
#endif
