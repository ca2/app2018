#pragma once


template<class TYPE>
void dump_elements(dump_context & dumpcontext, const TYPE* pElements, ::count nCount)
{
   ENSURE(nCount == 0 || pElements != NULL);
   ASSERT(nCount == 0 ||
      __is_valid_address(pElements, (size_t)nCount * sizeof(TYPE), FALSE));
#ifdef WINDOWS
   &dumpcontext; // not used
   pElements;  // not used
   nCount; // not used
#endif
   // default does nothing
}


template<class TYPE>
inline void CopyElements(TYPE* pDest, const TYPE* pSrc, ::count nCount)
{
   ENSURE(nCount == 0 || pDest != 0 && pSrc != 0);
   ASSERT(nCount == 0 ||
      __is_valid_address(pDest, (size_t)nCount * sizeof(TYPE)));
   ASSERT(nCount == 0 ||
      __is_valid_address(pSrc, (size_t)nCount * sizeof(TYPE)));

   // default is element-copy using assignment
   while (nCount--)
      *pDest++ = *pSrc++;
}



#undef new


inline void * __cdecl operator new(size_t nSize) new_throw_spec
{

   return memory_alloc(nSize);

}


#if defined(LINUX) || defined(METROWIN)


inline void * __cdecl operator new(size_t nSize, void * p)
{

   UNREFERENCED_PARAMETER(nSize);

   return p;

}


inline void __cdecl operator delete(void * p, void * palloc)
{

   UNREFERENCED_PARAMETER(p);
   UNREFERENCED_PARAMETER(palloc);

}


#endif


inline void __cdecl operator delete(void * p) del_throw_spec
{

   memory_free(p);

}


inline void * __cdecl operator new[](size_t nSize) new_throw_spec
{

   return ::operator new(nSize);

}


inline void __cdecl operator delete[](void * p) del_throw_spec
{

   ::operator delete(p);

}





class CLASS_DECL_BASE c_class
{
public:


   static c_class s_cclass;


   c_class();
   c_class(const c_class &);
   virtual ~c_class();


};


inline CLASS_DECL_BASE void * __cdecl operator new (size_t size, const c_class &)
{

   return memory_alloc(size);

}


inline CLASS_DECL_BASE void * __cdecl operator new[](size_t size, const c_class &)
{

   return memory_alloc(size);

}


#define C_NEW new(c_class::s_cclass)


inline void * __cdecl operator new(size_t nSize, const char * lpszFileName, int32_t nLine) new_throw_spec
{

   return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);

}


inline void * __cdecl operator new[](size_t nSize, const char * lpszFileName, int32_t nLine) new_throw_spec
{

   return ::operator new[](nSize, _NORMAL_BLOCK, lpszFileName, nLine);

}


inline void __cdecl operator delete(void * pData, const char * /* lpszFileName */,  int32_t /* nLine */) del_throw_spec
{

   ::operator delete(pData, _NORMAL_BLOCK, NULL, -1);

}


inline void __cdecl operator delete[](void * pData, const char * /* lpszFileName */,  int32_t /* nLine */) del_throw_spec
{

   ::operator delete(pData, _NORMAL_BLOCK, NULL, -1);

}


inline void * __cdecl operator new(size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine)
{

   return memory_alloc_dbg(nSize, nType, lpszFileName, nLine);

}


inline void __cdecl operator delete(void * p, int32_t nType, const char * /* lpszFileName */, int32_t /* nLine */)
{

   memory_free_dbg(p, nType);

}


inline void * __cdecl operator new[](size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine)
{

   return ::operator new(nSize, nType, lpszFileName, nLine);

}


inline void __cdecl operator delete[](void * p, int32_t nType, const char * lpszFileName, int32_t nLine)
{

   ::operator delete(p, nType, lpszFileName, nLine);

}






//#include "core/collection/collection_array.h"


namespace user
{


   template < class VIEW >
   inline sp(VIEW) impact::create_view(::user::object * pdoc, sp(::user::interaction) pwndParent, id id, sp(::user::interaction) pviewLast)
   {
      return create_view(System.type_info < VIEW >(), pdoc, pwndParent, id, pviewLast);
   }


   template < class VIEW >
   inline sp(VIEW) impact::create_view(::user::interaction * pwndParent, id id, sp(::user::interaction) pviewLast)
   {
      return create_view < VIEW >(NULL, pwndParent, id, pviewLast);
   }

   template < class VIEW >
   inline sp(VIEW) impact::create_view(::user::view_creator_data * pcreatordata, sp(::user::interaction) pviewLast)
   {

      VIEW * pview = create_view < VIEW >(pcreatordata->m_pholder, pcreatordata->m_id, pviewLast);

      if (pview != NULL)
      {
         pcreatordata->m_pdoc = get_document();
      }

      return pview;

   }



   template < class DOCUMENT >
   ::data::data * impact::get_data()
   {
      ASSERT(this != NULL);
      DOCUMENT * pdocument = get_typed_document < DOCUMENT >();
      if (pdocument == NULL)
         return NULL;
      return pdocument->get_data();
   }

   template < class DOCUMENT >
   DOCUMENT * impact::get_typed_document()
   {
      if (m_spdocument.is_null())
         return NULL;
      return dynamic_cast < DOCUMENT * > (m_spdocument.m_p);
   }


} //   namespace user

