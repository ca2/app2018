#pragma once


#define ZEROED_ALLOC 1



BEGIN_EXTERN_C

#ifdef __VLD
#define unaligned_memory_alloc malloc
#define unaligned_memory_alloc_dbg _malloc_dbg
#define aligned_memory_alloc malloc
#define aligned_memory_alloc_dbg _malloc_dbg
#define memory_alloc_no_track malloc
#define memory_calloc calloc
#define memory_alloc_dbg _malloc_dbg
#define memory_realloc_dbg _realloc_dbg
#define memory_free_dbg _free_dbg

#else
   CLASS_DECL_ACE void * unaligned_memory_alloc(size_t size);
   CLASS_DECL_ACE void * unaligned_memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);

   CLASS_DECL_ACE void * aligned_memory_alloc(size_t size);
   CLASS_DECL_ACE void * aligned_memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);
   CLASS_DECL_ACE void * memory_alloc_no_track(size_t size);
   CLASS_DECL_ACE void * memory_calloc(size_t size, size_t bytes);
   CLASS_DECL_ACE void * memory_alloc_dbg(size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);
   CLASS_DECL_ACE void * memory_realloc_dbg(void * pvoid, size_t nSize, int32_t nBlockUse, const char * szFileName, int32_t nLine);
   CLASS_DECL_ACE void   memory_free_dbg(void * pvoid, int32_t iBlockType);
#endif
   CLASS_DECL_ACE size_t memory_size(void * p);
   CLASS_DECL_ACE size_t memory_size_dbg(void * p, int32_t iBlockType);


#if !defined(MCHECK) && !defined(__VLD) && !defined(__MCRTDBG)
   CLASS_DECL_ACE void * memory_alloc(size_t size);
   CLASS_DECL_ACE void * memory_realloc(void * pvoid, size_t nSize);
   CLASS_DECL_ACE void   memory_free(void * pvoid);
#endif


   END_EXTERN_C

#if MEMDLEAK
      CLASS_DECL_ACE void set_last_block_file_name(const char * psz);
   CLASS_DECL_ACE void memdleak_dump();
#endif

#if MEMDLEAK  || defined(__MCRTDBG)
CLASS_DECL_ACE void * system_heap_alloc_dbg(size_t size, int nBlockUse, const char * pszFile, int iLine);
#define system_heap_alloc(s) system_heap_alloc_dbg(s, 49, __FILE__, __LINE__)
CLASS_DECL_ACE void * system_heap_realloc_dbg(void * p, size_t size, int nBlockUse, const char * pszFile, int iLine);
#define system_heap_realloc(p, s) system_heap_realloc_dbg(p, s, 49, __FILE__, __LINE__)
#else
CLASS_DECL_ACE void * system_heap_alloc(size_t size);
CLASS_DECL_ACE void * system_heap_realloc(void * pvoidOld, size_t size);
#endif
CLASS_DECL_ACE void   system_heap_free(void * pvoid);


struct memdleak_block
{


   int32_t                 m_iBlockUse;
   const char *            m_pszFileName;
#if OSBIT == 32
   DWORD                m_puiStack[64];
#else
   DWORD64                m_puiStack[64];
#endif
   int                     m_iStack;
   int32_t                 m_iLine;
   int_ptr                 m_iSize;
   struct memdleak_block *        m_pnext;
   struct memdleak_block *        m_pprevious;

};





#if defined(__cplusplus)

namespace heap
{


   class CLASS_DECL_ACE system_memory_allocator
   {
   public:

#if MEMDLEAK  || defined(__MCRTDBG)
      inline static void * alloc(size_t iSize,  const char * pszFile, int iLine)
      {

         //TODO("jai"); jas = Jonathan Blow
         return system_heap_alloc_dbg(iSize, 725, pszFile, iLine);

      }
#else
      inline static void * alloc(size_t iSize)
      {

         //TODO("jai"); jas = Jonathan Blow
         return system_heap_alloc(iSize);

      }
#endif


      inline static void _free(void * p)
      {

         //TODO("jai"); jas = Jonathan Blow
         system_heap_free(p);

      }


   };


   class CLASS_DECL_ACE default_memory_allocator
   {
   public:


#if MEMDLEAK || defined(__MCRTDBG)
      inline static void * alloc(size_t iSize, const char * pszFile, int iLine)
      {

         //TODO("jai"); jas = Jonathan Blow
         return memory_alloc_dbg(iSize, 724, pszFile, iLine);

      }
#else
      inline static void * alloc(size_t iSize)
      {

         //TODO("jai"); jas = Jonathan Blow
         return memory_alloc(iSize);

      }
#endif


      inline static void _free(void * p)
      {

         //TODO("jai"); jas = Jonathan Blow
         ::memory_free(p);

      }


   };


   class CLASS_DECL_ACE heap_base
   {
   public:


      void *         m_p;
      uint_ptr       m_uiSize;


      heap_base()
      {

         m_p         = NULL;
         m_uiSize    = 0;

      }

      heap_base(uint_ptr uiSize)
      {

         m_p         = NULL;
         m_uiSize    = 0;

         size(uiSize);

      }

      virtual ~heap_base()
      {

         _free();

      }

      uint_ptr size()
      {

         return m_uiSize;

      }

      virtual uint_ptr size(uint_ptr uiSize)
      {

         if(m_p == NULL)
         {

            m_p = memory_alloc(uiSize);

         }
         else
         {

            m_p = memory_realloc(m_p,uiSize);

         }

         if(m_p != NULL)
         {

            m_uiSize = uiSize;

         }

         return m_uiSize;

      }


      void _free()
      {

         if(m_p != NULL)
         {

            ::memory_free(m_p);

            m_p = NULL;

         }

      }


   };


   template < typename T >
   class heap:
      public heap_base
   {
   public:


      heap() {   }

      heap(uint_ptr iCount): heap_base(iCount * sizeof(T)) {   }

      operator T * () { return (T *)m_p; }
      operator const T * () const { return (T *)m_p; }

      uint_ptr count() { return size() / sizeof(T); }

   };

} // namespace heap



typedef ::heap::heap < char > hstring_base;

class CLASS_DECL_ACE hstring:
   public hstring_base
{
public:

   hstring() {}
   hstring(uint_ptr uiSize): hstring_base(uiSize) {}

};


typedef ::heap::heap < unichar > hwstring_base;

class CLASS_DECL_ACE hwstring:
   public hwstring_base
{
public:

   hwstring() {}
   hwstring(uint_ptr uiSize): hwstring_base(uiSize) {}

};


#endif


