#include "framework.h"

#include "aura_plex_heap1.h"

#include "aura_plex_heap_impl1.h"


#undef new


extern plex_heap_alloc_array * g_pheap;

plex* plex::create(plex*& pHead, uint_ptr nMax, uint_ptr cbElement)
{

   memory_size_t size = sizeof(plex) + nMax * cbElement + CA2_PALACE_SAFE_ZONE_BORDER_SIZE * 2;
   plex* p = (plex*) memory_alloc(size);
   // may _throw( exception
   p->pNext = pHead;
   pHead = p;  // change head (adds in reverse order for simplicity)
   return p;
}

void plex::FreeDataChain()     // free this one and links
{
   try
   {
      //plex* pPrevious = NULL;
      plex* p = this;
      while (p != NULL)
      {
         BYTE* bytes = (BYTE*) p;
         plex* pNextLocal = p->pNext;
         memory_free(bytes);
         //pPrevious = p;
         p = pNextLocal;
      }
   }
   catch(...)
   {
   }
}


