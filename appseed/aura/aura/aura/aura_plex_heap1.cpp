#include "framework.h"

#include "aura_plex_heap1.h"

#include "aura_plex_heap_impl1.h"

#include "align_byte_count.h"

#if !defined(MCHECK) && !defined(_VLD) && !defined(__MCRTDBG) && !MEMDLEAK

#include "aura_heap_memory.h"

#endif

#undef new


void * plex_heap_alloc_sync::operator new(size_t s)
{

   return system_heap_alloc(sizeof(plex_heap_alloc_sync));

}

void plex_heap_alloc_sync::operator delete(void * p)
{

   system_heap_free(p);

}




void Alloc_check_pointer_in_cpp(void * p);

void Free_check_pointer_in_cpp(void * p);


#ifdef DEBUG

template < const int m_iSize >
class good_guard
{
public:


   char m_pdata[m_iSize];


   good_guard()
   {

      memset(m_pdata, 0xCD, sizeof(m_pdata));

   }


   operator char * ()
   {

      return m_pdata;

   }


   bool is_valid(char * pdata)
   {

      return memcmp(pdata, m_pdata, m_iSize) == 0;

   }

};

good_guard < 16 > g_goodguard16;

good_guard < 32 > g_goodguard32;


void defer_check_plex_heap_alloc_sync_node_palace_guard(plex_heap_alloc_sync::node * pnode, UINT nAllocSize)
{

   if (!g_goodguard16.is_valid(pnode->m_puchPalaceLeft))
   {

      output_debug_string("someone's pee outside of aim to left");

      ASSERT(FALSE);

   }

   if (!g_goodguard16.is_valid(pnode->m_puchPalaceRight))
   {

      output_debug_string("someone's pee outside of aim to the right");

      ASSERT(FALSE);

   }

   if (!g_goodguard32.is_valid(&pnode->m_puchPalaceLeft[sizeof(*pnode) + nAllocSize]))
   {

      output_debug_string("someone's pee outside of aim to the bottom");

      ASSERT(FALSE);

   }

}


void setup_plex_heap_alloc_sync_node_palace_guard(plex_heap_alloc_sync::node * pnode, UINT nAllocSize)
{

   memset(pnode->m_puchPalaceLeft, 0xCD, sizeof(pnode->m_puchPalaceLeft));

   memset(pnode->m_puchPalaceRight, 0xCD, sizeof(pnode->m_puchPalaceRight));

   memset(&pnode->m_puchPalaceLeft[sizeof(*pnode) + nAllocSize], 0xCD, 32);

}


#endif





plex_heap_alloc_sync::plex_heap_alloc_sync(UINT nAllocSize, UINT nBlockSize)
{

   if (nBlockSize <= 1)
   {

      nBlockSize = 4;

   }

#ifndef DEBUG

   ASSERT(nAllocSize >= sizeof(node));

   if (nAllocSize < sizeof(node))
   {

      nAllocSize = sizeof(node);

   }

#endif

   ASSERT(nBlockSize > 1);

   if (nBlockSize <= 0)
   {

      nBlockSize = 64;

   }

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

}






plex_heap_alloc::plex_heap_alloc(UINT nAllocSize, UINT nBlockSize)
{

#if !defined(METROWIN) && !defined(LINUX) && !defined(APPLEOS) && !defined(ANDROID)

   uint32_t uiShareCount = ::get_current_process_maximum_affinity() + 1;

#else

   int32_t uiShareCount = 0;

#endif

   if (uiShareCount == 0)
   {

      uiShareCount = 4;

   }

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

#undef new

void Free_check_pointer_in_cpp(void * p);



plex_heap_alloc::~plex_heap_alloc()
{

   for(int32_t i = 0; i < get_count(); i++)
   {

      delete element_at(i);

   }

}

void * plex_heap_alloc::operator new(size_t s)
{

   return system_heap_alloc(sizeof(plex_heap_alloc));

}


void plex_heap_alloc::operator delete(void * p)
{

   system_heap_free(p);

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

}


extern plex_heap_alloc_array * g_pheap;
//
//u64 b[64];
//
//void init_log2()
//{
//   for (int i = 0; i<64; ++i)
//      b[u64(c*((2 << i) - 1)) >> s] = i;
//   return;
//}
//u64 log2(u64 v)
//{//first round down to one less than a power of 2
//   for (int i = 1; i<64; i <<= 1) //unroll for speed
//      v |= v >> i;
//   return b[u64(v*c) >> s];
//}

//





plex_heap_alloc_array::plex_heap_alloc_array()
{
   //init_log2();
   if(g_pheap == NULL)
   {

      g_pheap = this;

   }

   ////add(new plex_heap_alloc(8,256));
   ////add(new plex_heap_alloc(16, 256));
   ////add(new plex_heap_alloc(24, 256));
   //add(new plex_heap_alloc(32, 256));
   ////add(new plex_heap_alloc(48, 256));

   //add(new plex_heap_alloc(64, 128));
   ////add(new plex_heap_alloc(96, 128));
   //add(new plex_heap_alloc(128, 128));
   ////add(new plex_heap_alloc(192, 128));
   //add(new plex_heap_alloc(256, 128));

   ////add(new plex_heap_alloc(384, 64));
   //add(new plex_heap_alloc(512, 64));
   ////add(new plex_heap_alloc(768, 64));
   //add(new plex_heap_alloc(1024, 64));
   //add(new plex_heap_alloc(1024 * 2, 64));

   //add(new plex_heap_alloc(1024 * 4, 32));
   //add(new plex_heap_alloc(1024 * 8, 32));

   //add(new plex_heap_alloc(1024 * 16, 16));
   //add(new plex_heap_alloc(1024 * 32, 16));
   //add(new plex_heap_alloc(1024 * 64, 16));
   //add(new plex_heap_alloc(1024 * 128,16));

   ////add(new plex_heap_alloc(1024 * 192, 4));
   //add(new plex_heap_alloc(1024 * 256, 4));
   ////add(new plex_heap_alloc(1024 * 384, 4));
   //add(new plex_heap_alloc(1024 * 512, 4));
   ////add(new plex_heap_alloc(1024 * 768, 4));
   //add(new plex_heap_alloc(1024 * 1024, 4));

   //add(new plex_heap_alloc(1024 * 1024 * 2, 2));
   //add(new plex_heap_alloc(1024 * 1024 * 4, 2));
   //add(new plex_heap_alloc(1024 * 1024 * 8, 2));
   //add(new plex_heap_alloc(1024 * 1024 * 16, 2));

   int i = 5;
   add(new plex_heap_alloc(1 << i++, 256)); // 32
   add(new plex_heap_alloc(1 << i++, 128)); // 64
   add(new plex_heap_alloc(1 << i++, 128)); // 128
   add(new plex_heap_alloc(1 << i++, 128)); // 256
   add(new plex_heap_alloc(1 << i++, 64)); // 512
   add(new plex_heap_alloc(1 << i++, 64)); // 1024


#if OSBIT > 32

   int iDiv = 1;

#else

   int iDiv = 4;

#endif

   add(new plex_heap_alloc(1 << i++, 256 / iDiv)); // 2 * 1024
   add(new plex_heap_alloc(1 << i++, 128 / iDiv)); // 4 * 1024
   add(new plex_heap_alloc(1 << i++, 128 / iDiv)); // 8 * 1024
   add(new plex_heap_alloc(1 << i++, 128 / iDiv)); // 16 * 1024
   add(new plex_heap_alloc(1 << i++, 64 / iDiv)); // 32 * 1024
   add(new plex_heap_alloc(1 << i++, 64 / iDiv)); // 64 * 1024
   add(new plex_heap_alloc(1 << i++, 64 / iDiv)); // 128 * 1024
   add(new plex_heap_alloc(1 << i++, 64 / iDiv)); // 256 * 1024
   add(new plex_heap_alloc(1 << i++, 64 / iDiv)); // 512 * 1024
   add(new plex_heap_alloc(1 << i++, 64 / iDiv)); // 1024 * 1024

#if OSBIT > 32

   add(new plex_heap_alloc(1 << i++, 2)); // 2 * 1024 * 1024
   add(new plex_heap_alloc(1 << i++, 2)); // 4 * 1024 * 1024
   add(new plex_heap_alloc(1 << i++, 2)); // 8 * 1024 * 1024
   add(new plex_heap_alloc(1 << i++, 2)); // 16 * 1024 * 1024

#endif

}


plex_heap_alloc_array::~plex_heap_alloc_array()
{

   pre_finalize();

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

      if(size > sizeOld)
      {

         memset(&((byte *)pNew)[sizeOld], 0, size - sizeOld);

      }

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

         if(size > sizeOld)
         {

            memset(&((byte *)pNew)[sizeOld], 0, size - sizeOld);

         }

      }

#if !MEMDLEAK
      if (align > 0)
      {

         int_ptr oldSize = sizeOld - heap_memory::aligned_provision_get_size(0);

         int_ptr newSize = size - heap_memory::aligned_provision_get_size(0);

         memcpy(heap_memory::aligned(pNew,newSize,0),
                heap_memory::aligned(p,oldSize,0),
                MIN(oldSize, newSize));

      }
      else
#endif
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

CLASS_DECL_AURA void simple_debug_print(const char * psz);

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#ifdef RASPBIAN
#define QUOTED_KERNEL_SPACE (1024)
#elif  defined(ANDROID)
#define QUOTED_KERNEL_SPACE (1024 * 1024)
#else
#if OSBIT == 64
#define QUOTED_KERNEL_SPACE (1024 * 1024)
#else
#define QUOTED_KERNEL_SPACE 1024
#endif
#endif

void Alloc_check_pointer_in_cpp(void * p)
{

   if ((uint_ptr)p < QUOTED_KERNEL_SPACE)
   {

      if (p == NULL)
      {

         simple_debug_print("Alloc_check_pointer_in_cpp return NULL ?!?!?!");

      }
      else
      {

         simple_debug_print("Alloc_check_pointer_in_cpp returning address less than " STR(QUOTED_KERNEL_SPACE));

      }

   }



}


void Free_check_pointer_in_cpp(void * p)
{

   if((byte *) p <  (byte *)  g_pf1)
   {
      simple_debug_print("hit g_pf1");
   }
   if((dword_ptr) p   & 0x8000000000000000LLU)
   {
      simple_debug_print("hit hiptr");
   }
   if ((dword_ptr)p == 0x0000000200000020LLU)
   {

      simple_debug_print("hit 0x0000000200000020LLU");
   }
   if ((uint_ptr)p < QUOTED_KERNEL_SPACE)
   {

      if (p == NULL)
      {

         simple_debug_print("Free_check_pointer_in_cpp FREEING NULL?!?! WHAT!! == NULL!!");

      }
      else
      {

         simple_debug_print("Free_check_pointer_in_cpp WHAT!! < " STR(QUOTED_KERNEL_SPACE));

      }

   }

}


void * plex_heap_alloc_array::operator new(size_t s)
{

   return system_heap_alloc(sizeof(plex_heap_alloc_array));

}


void plex_heap_alloc_array::operator delete(void * p)
{

   system_heap_free(p);

}


plex_heap_alloc * plex_heap_alloc_array::find(size_t nAllocSize)
{

   for (index i = 0; i < m_nSize; i++)
   {

      if (nAllocSize <= m_pData[i]->m_uiAllocSize)
      {

         return m_pData[i];

      }

   }

   return NULL;

}


void * plex_heap_alloc_array::_alloc(size_t size)
{

   plex_heap_alloc * palloc = find(size);

   if (palloc != NULL)
   {

      return palloc->Alloc();

   }
   else
   {

      return ::system_heap_alloc(size);

   }

}



void plex_heap_alloc_array::_free(void * p,size_t size)
{

   /*   int xxx = MIN(1024,size * 2 / 3);

      memset(&((byte *)p)[xxx],0xCD,size - xxx); // attempt to invalidate memory so it get unusable (as it should be after free). // but preserve first xxx bytes so vtable indicating object type may be eventually preserved for debugging
      */

   //memset(p,0xCD,size); // attempt to invalidate memory so it get unusable (as it should be after free).

   plex_heap_alloc * palloc = find(size);

   if(palloc != NULL)
   {

      ASSERT(size <= palloc->m_uiAllocSize);

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


void defer_check_plex_heap_alloc_sync_node_palace_guard(plex_heap_alloc_sync::node * pnode, UINT nAllocSize);


void * plex_heap_alloc_sync::Alloc()
{

   cslock sl(&m_protect);

   if (m_pnodeFree == NULL)
   {

      NewBlock();

   }

   void * pnode = m_pnodeFree;

#ifdef DEBUG

   defer_check_plex_heap_alloc_sync_node_palace_guard(m_pnodeFree, m_nAllocSize);

   pnode = ((byte*)pnode) + 16 + sizeof(node *) + 16;

#endif

   m_pnodeFree = m_pnodeFree->m_pnext;

   zero(pnode, m_nAllocSize);

   return pnode;

}


void plex_heap_alloc_sync::Free(void * pParam)
{

   if (is_null(pParam))
   {

      return;

   }


#ifdef DEBUG

   void * p = ((byte*)pParam) - (16 + sizeof(node *) + 16);

   node * pnode = (node *)p;

   defer_check_plex_heap_alloc_sync_node_palace_guard(pnode, m_nAllocSize);

#else

   node * pnode = (node *)pParam;

#endif

#ifdef DEBUG

   memset(pParam, 0xCD, m_nAllocSize);

   setup_plex_heap_alloc_sync_node_palace_guard(pnode, m_nAllocSize);

#endif

   cslock sl(&m_protect);

#ifdef MEMDFREE // Free Debug - duplicate freeing ?

   node * pnodeFree = m_pnodeFree;

   while (pnodeFree != NULL)
   {

      if (pnode == pnodeFree) // dbgsnp - debug snippet
      {

         // already in free list

         if (is_debugger_attached())
         {

            debug_break();

         }

         return;

      }

      pnodeFree = pnodeFree->m_pnext;

   }

#endif

#if STORE_LAST_BLOCK

   if (m_pnodeLastBlock != NULL)
   {

      system_heap_free(m_pnodeLastBlock);

   }

   m_pnodeLastBlock = (node *)system_heap_alloc(m_nAllocSize + 32);

   memcpy(m_pnodeLastBlock, pnode, m_nAllocSize + 32);

#endif

   pnode->m_pnext = m_pnodeFree;

   m_pnodeFree = pnode;

}





//#pragma pack(show)


#pragma pack(push, 1)


#undef new


class CLASS_DECL_AURA plex_heap     // warning var length structure
{
public:
   plex_heap* pNext;
   // BYTE data[maxNum*elementSize];

   void * data() { return this+1; }

   static plex_heap* create(plex_heap*& head, uint_ptr nMax, uint_ptr cbElement);
   // like 'calloc' but no zero fill
   // may _throw( memory exceptions

   void FreeDataChain();       // free this one and links
};

#pragma pack(pop)

//#pragma pack(show)







plex_heap * plex_heap::create(plex_heap*& pHead, uint_ptr nMax, uint_ptr cbElement)
{
   ASSERT(nMax > 0 && cbElement > 0);
   if (nMax == 0 || cbElement == 0)
   {
      _throw(invalid_argument_exception(get_app()));
   }

   plex_heap* p = (plex_heap*) system_heap_alloc(sizeof(plex_heap) + nMax * cbElement);

#ifdef DEBUG
   Alloc_check_pointer_in_cpp(p);
#endif
   // may _throw( exception
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

#ifdef DEBUG

      UINT nAllocSize = 16 + sizeof(node *) + 16 + m_nAllocSize + 32;

#else

      UINT nAllocSize = m_nAllocSize;

#endif

      plex_heap * pnewblock = plex_heap::create(m_pBlocks, m_nBlockSize, nAllocSize);

      node * pnode = (node *) pnewblock->data();

      // free in reverse order to make it easier to debug
      ((BYTE*&)pnode) += (nAllocSize * m_nBlockSize) - nAllocSize;

      for (int32_t i = m_nBlockSize - 1; i >= 0; i--, ((BYTE*&)pnode) -= nAllocSize)
      {

#ifdef DEBUG

         setup_plex_heap_alloc_sync_node_palace_guard(pnode, m_nAllocSize);

#endif

         pnode->m_pnext = m_pnodeFree;

         m_pnodeFree = pnode;

      }

   }

   ASSERT(m_pnodeFree != NULL);  // we must have something

#ifdef DEBUG

   Free_check_pointer_in_cpp(m_pnodeFree);

#endif

}
