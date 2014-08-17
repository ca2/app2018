#pragma once


#define ZEROED_ALLOC 1



BEGIN_EXTERN_C


   CLASS_DECL_AXIS void * unaligned_memory_alloc(size_t size);
   CLASS_DECL_AXIS void * unaligned_memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);

   CLASS_DECL_AXIS void * aligned_memory_alloc(size_t size);
   CLASS_DECL_AXIS void * aligned_memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);

   CLASS_DECL_AXIS void * memory_alloc(size_t size);
   CLASS_DECL_AXIS void * memory_calloc(size_t size, size_t bytes);
   CLASS_DECL_AXIS void * memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);
   CLASS_DECL_AXIS void * memory_realloc(void * pvoid, size_t nSize);
   CLASS_DECL_AXIS void * memory_realloc_dbg(void * pvoid, size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);
   CLASS_DECL_AXIS void   memory_free(void * pvoid);
   CLASS_DECL_AXIS void   memory_free_dbg(void * pvoid, int32_t iBlockType);
   CLASS_DECL_AXIS size_t memory_size(void * p);
   CLASS_DECL_AXIS size_t memory_size_dbg(void * p, int32_t iBlockType);



END_EXTERN_C




CLASS_DECL_AXIS void * system_heap_alloc(size_t size);
CLASS_DECL_AXIS void * system_heap_realloc(void * pvoidOld, size_t size);
CLASS_DECL_AXIS void   system_heap_free(void * pvoid);






