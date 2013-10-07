#pragma once


struct plex;


class CLASS_DECL_c fixed_alloc_no_sync
{
public:


   struct node
   {
      node* pNext;   // only valid when in free list
   };

   UINT m_nAllocSize;   // size of each block from Alloc
   UINT m_nBlockSize;   // number of blocks to get at a time
   plex* m_pBlocks;   // linked list of blocks (is nBlocks*nAllocSize)
   node* m_pnodeFree;   // first free node (NULL if no free nodes)


   fixed_alloc_no_sync(UINT nAllocSize, UINT nBlockSize = 64);
   ~fixed_alloc_no_sync();


   UINT GetAllocSize() { return m_nAllocSize; }


   inline void * Alloc();  // return a chunk of primitive::memory of nAllocSize
   inline void Free(void * p); // free chunk of primitive::memory returned from Alloc
   void FreeAll(); // free everything allocated from this allocator


   void NewBlock();

};


inline void * fixed_alloc_no_sync::Alloc()
{
   if (m_pnodeFree == NULL)
   {
      NewBlock();
   }
   // remove the first available node from the free list
   void * pNode = m_pnodeFree;
   m_pnodeFree = m_pnodeFree->pNext;
   return (void *) (((byte *)pNode) + 16);
}

inline void fixed_alloc_no_sync::Free(void * p)
{
   p = (void *) (((byte *)p) - 16);
   if (p != NULL)
   {
      // simply return the node to the free list
      node* pNode = (node*)p;
      pNode->pNext = m_pnodeFree;
      m_pnodeFree = pNode;
   }
}



class CLASS_DECL_c fixed_alloc_sync
{
public:


   int32_t                                      m_i;
   int32_t                                      m_iShareCount;
   raw_array < critical_section * >             m_protectptra;
   raw_array < fixed_alloc_no_sync * >          m_allocptra;


   fixed_alloc_sync(UINT nAllocSize, UINT nBlockSize = 64, int32_t iShareCount = 2);
   ~fixed_alloc_sync();


   inline void * Alloc();   // return a chunk of primitive::memory of nAllocSize
   inline void Free(void * p);   // free chunk of primitive::memory returned from Alloc
   void FreeAll();   // free everything allocated from this allocator


};




class CLASS_DECL_c fixed_alloc
{
public:


   int32_t                                         m_i;
   int32_t                                         m_iShareCount;
   array < fixed_alloc_sync * >                    m_allocptra;


   fixed_alloc(UINT nAllocSize, UINT nBlockSize = 64);
   ~fixed_alloc();


   inline void * Alloc();   // return a chunk of primitive::memory of nAllocSize
   inline void Free(void * p);   // free chunk of primitive::memory returned from Alloc
   void FreeAll();   // free everything allocated from this allocator


};




class CLASS_DECL_c fixed_alloc_array :
   public array < fixed_alloc * >
{
public:


   fixed_alloc_array();
   virtual ~fixed_alloc_array();

   void * alloc(size_t nAllocSize);
   void free(void * p, size_t nAllocSize);
   void * realloc(void * p, size_t nOldAllocSize, size_t nNewAllocSize);

   fixed_alloc * find(size_t nAllocSize);


};




// DECLARE_FIXED_ALLOC -- used in class definition
#define DECLARE_FIXED_ALLOC(class_name) \
public: \
   void * operator new(size_t size) \
   { \
      UNUSED(size); \
      return s_alloc.Alloc(); \
   } \
   void * operator new(size_t, void * p) \
      { return p; } \
   void operator delete(void * p) { s_alloc.Free(p); } \
   void * operator new(size_t size, const char *, int32_t) \
   { \
      UNUSED(size); \
      return s_alloc.Alloc(); \
   } \
   void operator delete(void * p, const char *, int32_t) { s_alloc.Free(p); } \
protected: \
   static fixed_alloc s_alloc; \
public: \


// IMPLEMENT_FIXED_ALLOC -- used in class implementation file
#define IMPLEMENT_FIXED_ALLOC(class_name, block_size) \
fixed_alloc class_name::s_alloc(sizeof(class_name), block_size) \

// DECLARE_FIXED_ALLOC_NOSYNC -- used in class definition
#define DECLARE_FIXED_ALLOC_NOSYNC(class_name) \
public: \
   void * operator new(size_t size) \
   { \
      UNUSED(size); \
      return s_alloc.Alloc(); \
   } \
   void * operator new(size_t, void * p) \
      { return p; } \
   void operator delete(void * p) { s_alloc.Free(p); } \
   void * operator new(size_t size, const char *, int32_t) \
   { \
      UNUSED(size); \
      return s_alloc.Alloc(); \
   } \
protected: \
   static fixed_alloc_no_sync s_alloc; \
public : \


// IMPLEMENT_FIXED_ALLOC_NOSYNC -- used in class implementation file
#define IMPLEMENT_FIXED_ALLOC_NOSYNC(class_nbame, block_size) \
fixed_alloc_no_sync class_name::s_alloc(sizeof(class_name), block_size) \


