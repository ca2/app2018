#pragma once


#undef new

void Free_check_pointer_in_cpp(void * p);

inline void * plex_heap_alloc_sync::Alloc()
{

   synch_lock sl(&m_protect);

   void * pdata = NULL;
   try
   {
      if (m_pnodeFree == NULL)
      {
         NewBlock();
      }
      // remove the first available node from the free list
      void * pNode = m_pnodeFree;
      m_pnodeFree = m_pnodeFree->pNext;
      pdata = pNode;
   }
   catch(...)
   {
   }
   //memset(pdata, 0, m_nAllocSize); // let constructors and algorithms initialize... "random initialization" of not initialized :-> C-:!!
   return pdata;
}

inline void plex_heap_alloc_sync::Free(void * p)
{

   if(p == NULL)
      return;
   
   #ifdef DEBUG
   Free_check_pointer_in_cpp(p);
   #endif

   synch_lock sl(&m_protect);
   
   memset(p, 0xCD, m_nAllocSize); // attempt to invalidate memory so it get unusable (as it should be after freed).

   try
   {

      // simply return the node to the free list
      node* pnode = (node*)p;

#ifdef MEMDFREE // Free Debug - duplicate freeing ?

      node * pnodeFree = m_pnodeFree;

      while(pnodeFree != NULL)
      {

         if(pnode == pnodeFree) // dbgsnp - debug snippet
         {

            // already in free list

            if(is_debugger_attached())
            {

               debug_break();

            }

            return;

         }

         pnodeFree = pnodeFree->pNext;

      }

#endif

#if STORE_LAST_BLOCK

      if(m_pnodeLastBlock != NULL)
         system_heap_free(m_pnodeLastBlock);

      m_pnodeLastBlock = (node *) system_heap_alloc(m_nAllocSize + 32);

      memcpy(m_pnodeLastBlock, pnode, m_nAllocSize + 32);

#endif

      pnode->pNext = m_pnodeFree;

      m_pnodeFree = pnode;

   }
   catch(...)
   {

   }


}






class CLASS_DECL_AURA plex_heap_alloc :
   public array < plex_heap_alloc_sync * >
{
public:


   uint32_t                      m_ui;
   uint32_t                      m_uiShareCount;
   uint32_t                      m_uiAllocSize;


   plex_heap_alloc(UINT nAllocSize, UINT nBlockSize = 64);
   virtual ~plex_heap_alloc();

   inline UINT GetAllocSize() { return m_uiAllocSize; }

   inline void * Alloc();               // return a chunk of primitive::memory of nAllocSize
   inline void Free(void * p);          // free chunk of primitive::memory returned from Alloc
   void FreeAll();               // free everything allocated from this allocator

   void NewBlock();

   void pre_finalize();

   void * operator new(size_t s)
   {
      return ::malloc(sizeof(plex_heap_alloc));
   }

   void operator delete(void * p)
   {
      ::free(p);
   }


};



inline void * plex_heap_alloc::Alloc()
{

   // veripseudo-random generator, don't need to be
   // perfectly sequential or perfectly distributed,
   // just fair well distributed
   // but very important is extremely fast

   uint32_t ui = m_ui % m_uiShareCount;

   m_ui++; // changed to unsigned (it was turning negative  by overflow and generating out of bound errors below)

   uint32_t * pui = (uint32_t *) element_at(ui)->Alloc();

   *pui = ui;

   return &pui[1];

}

inline void plex_heap_alloc::Free(void * p)
{

   if (p == NULL)
      return;

   uint32_t ui = ((uint32_t *)p)[-1];

   if(ui < m_uiShareCount)
   {

      element_at(ui)->Free(&((uint32_t *)p)[-1]);

   }
   else
   {

      ::OutputDebugStringW(L"plex_heap_alloc::Free error");

   }


}




class CLASS_DECL_AURA plex_heap_alloc_array :
   public array < plex_heap_alloc * >
{
public:


   struct memdleak_block
   {


      int32_t               m_iBlockUse;
      const char *      m_pszFileName;
      int32_t               m_iLine;
      memdleak_block *  m_pnext;
      memdleak_block *  m_pprevious;

   };


   ::count m_iWorkingSize;


   static memdleak_block * s_pmemdleakList;

   plex_heap_alloc_array();
   virtual ~plex_heap_alloc_array();


   static ::count get_mem_info(int32_t ** ppiUse, const char *** ppszFile, int32_t ** ppiLine);


   inline void * alloc(size_t nAllocSize);
   void * realloc(void * p, size_t nAllocSize, size_t nOldAllocSize, int align);
   inline void free(void * p, size_t nAllocSize);

   void pre_finalize();

   inline plex_heap_alloc * find(size_t nAllocSize);

   void * alloc_dbg(size_t nAllocSize, int32_t nBlockUse, const char * szFileName, int32_t iLine);
   void * realloc_dbg(void * p, size_t nAllocSize, size_t nOldAllocSize, int align, int32_t nBlockUse, const char * szFileName, int32_t iLine);
   void free_dbg(void * p, size_t nAllocSize);

   void * operator new(size_t s)
   {
      return ::malloc(sizeof(plex_heap_alloc_array));
   }

   void operator delete(void * p)
   {
      ::free(p);
   }

};



inline void * plex_heap_alloc_array::alloc(size_t size)
{

   plex_heap_alloc * palloc = find(size);

   if(palloc != NULL)
   {

      return palloc->Alloc();

   }
   else
   {

      return ::system_heap_alloc(size);

   }

}


void plex_heap_alloc_array::free(void * p, size_t size)
{

   memset(p, 0xCD, size); // attempt to invalidate memory so it get unusable (as it should be after free).

   plex_heap_alloc * palloc = find(size);

   if(palloc != NULL)
   {

      return palloc->Free(p);

   }
   else
   {

      return ::system_heap_free(p);

   }

}




inline plex_heap_alloc * plex_heap_alloc_array::find(size_t nAllocSize)
{

   for(int32_t i = 0; i < m_iWorkingSize; i++)
   {

      if(this->element_at(i)->GetAllocSize() >= nAllocSize)
      {

         return this->element_at(i);

      }

   }

   return NULL;

}




#define new AURA_NEW
