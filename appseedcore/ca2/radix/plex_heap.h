#pragma once


class mutex;
class plex_heap_alloc;


class CLASS_DECL_ca plex_heap_alloc_array : 
   public base_array < plex_heap_alloc * >
{
public: 

   HANDLE      m_hmutex;

   plex_heap_alloc_array();
   virtual ~plex_heap_alloc_array();

   void * alloc(size_t nAllocSize);
   void * realloc(void * p, size_t nOldAllocSize, size_t nNewAllocSize);
   void free(void * p, size_t nAllocSize);

   plex_heap_alloc * find(size_t nAllocSize);

};
