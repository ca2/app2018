#pragma once


//template<class TYPE>
//void dump_elements(dump_context & dumpcontext, const TYPE* pElements, ::count nCount)
//{
//   ENSURE(nCount == 0 || pElements != NULL);
//   ASSERT(nCount == 0 ||
//      __is_valid_address(pElements, (size_t)nCount * sizeof(TYPE), FALSE));
//#ifdef WINDOWS
//   &dumpcontext; // not used
//   pElements;  // not used
//   nCount; // not used
//#endif
//   // default does nothing
//}
//
//
//template<class TYPE>
//inline void CopyElements(TYPE* pDest, const TYPE* pSrc, ::count nCount)
//{
//   ENSURE(nCount == 0 || pDest != 0 && pSrc != 0);
//   ASSERT(nCount == 0 ||
//      __is_valid_address(pDest, (size_t)nCount * sizeof(TYPE)));
//   ASSERT(nCount == 0 ||
//      __is_valid_address(pSrc, (size_t)nCount * sizeof(TYPE)));
//
//   // default is element-copy using assignment
//   while (nCount--)
//      *pDest++ = *pSrc++;
//}



#undef new

//#ifdef APPLEOS
//
//void * __cdecl operator new(size_t nSize) new_throw_spec;
//
//#else
//
//inline void * __cdecl operator new(size_t nSize) new_throw_spec
//{
//
//    return memory_alloc(nSize);
//
//}
//
//#endif


#if defined(LINUX) || defined(METROWIN) || defined(ANDROID)


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


//#ifdef APPLEOS
//
//void __cdecl operator delete(void * p) del_throw_spec;
//void * __cdecl operator new[](size_t nSize) new_throw_spec;
//void __cdecl operator delete[](void * p) del_throw_spec;
//
//#else
//
//inline void __cdecl operator delete(void * p) del_throw_spec
//{
//
//   memory_free(p);
//
//}
//
//
//inline void * __cdecl operator new[](size_t nSize) new_throw_spec
//{
//
//   return ::operator new(nSize);
//
//}
//
//
//inline void __cdecl operator delete[](void * p) del_throw_spec
//{
//
//   ::operator delete(p);
//
//}
//
//#endif



//class CLASS_DECL_BASE c_class
//{
//public:
//
//
//   static c_class s_cclass;
//
//
//   c_class();
//   c_class(const c_class &);
//   virtual ~c_class();
//
//
//};


//inline CLASS_DECL_BASE void * __cdecl operator new (size_t size, const c_class &)
//{
//
//   return memory_alloc(size);
//
//}
//
//
//inline CLASS_DECL_BASE void * __cdecl operator new[](size_t size, const c_class &)
//{
//
//   return memory_alloc(size);
//
//}
//
//
//#define C_NEW new(c_class::s_cclass)


//inline void * __cdecl operator new(size_t nSize, const char * lpszFileName, int32_t nLine) new_throw_spec
//{
//
//   return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
//
//}
//
//
//inline void * __cdecl operator new[](size_t nSize, const char * lpszFileName, int32_t nLine) new_throw_spec
//{
//
//   return ::operator new[](nSize, _NORMAL_BLOCK, lpszFileName, nLine);
//
//}
//
//
//inline void __cdecl operator delete(void * pData, const char * /* lpszFileName */,  int32_t /* nLine */) del_throw_spec
//{
//
//   ::operator delete(pData, _NORMAL_BLOCK, NULL, -1);
//
//}
//
//
//inline void __cdecl operator delete[](void * pData, const char * /* lpszFileName */,  int32_t /* nLine */) del_throw_spec
//{
//
//   ::operator delete(pData, _NORMAL_BLOCK, NULL, -1);
//
//}
//
//
//inline void * __cdecl operator new(size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine)
//{
//
//   return memory_alloc_dbg(nSize, nType, lpszFileName, nLine);
//
//}
//
//
//inline void __cdecl operator delete(void * p, int32_t nType, const char * /* lpszFileName */, int32_t /* nLine */)
//{
//
//   memory_free_dbg(p, nType);
//
//}
//
//
//inline void * __cdecl operator new[](size_t nSize, int32_t nType, const char * lpszFileName, int32_t nLine)
//{
//
//   return ::operator new(nSize, nType, lpszFileName, nLine);
//
//}
//
//
//inline void __cdecl operator delete[](void * p, int32_t nType, const char * lpszFileName, int32_t nLine)
//{
//
//   ::operator delete(p, nType, lpszFileName, nLine);
//
//}






//#include "core/collection/collection_array.h"


//namespace user
//{
//
//
//   template < class VIEW >
//   inline sp(VIEW) impact::create_view(::user::document * pdoc,const RECT & rect, sp(::user::interaction) pwndParent,id id,sp(::user::interaction) pviewLast)
//   {
//      return create_view(System.type_info < VIEW >(), pdoc, rect, pwndParent, id, pviewLast);
//   }
//
//
//   template < class VIEW >
//   inline sp(VIEW) impact::create_view(const RECT & rect,::user::interaction * pwndParent,id id,sp(::user::interaction) pviewLast)
//   {
//
//      return create_view < VIEW >(NULL, rect, pwndParent, id, pviewLast);
//
//   }
//
//
//   template < class VIEW >
//   inline sp(VIEW) impact::create_view(::user::view_creator_data * pcreatordata, sp(::user::interaction) pviewLast)
//   {
//
//      VIEW * pview = create_view < VIEW >(pcreatordata->m_rectCreate, pcreatordata->m_pholder, pcreatordata->m_id, pviewLast);
//
//      if (pview != NULL)
//      {
//
//         pcreatordata->m_pdoc = get_document();
//
//      }
//
//      return pview;
//
//   }
//
//
//   template < class DATA >
//   DATA * impact::get_typed_data()
//   {
//      ASSERT(this != NULL);
//      return m_spdocument->get_typed_data < DATA >();
//   }
//
//   template < class DOCUMENT >
//   ::data::data * impact::get_typed_document_data()
//   {
//      ASSERT(this != NULL);
//      return m_spdocument->get_typed_document_data < DOCUMENT >();
//   }
//
//   template < class DOCUMENT >
//   DOCUMENT * impact::get_typed_document()
//   {
//      ASSERT(this != NULL);
//      return m_spdocument->get_typed_document < DOCUMENT >();
//   }
//
//
//} //   namespace user
//


//namespace xml
//{
//
//
//   template < int32_t m_iNodeNameIndex,class TYPE >
//   void
//      smart_pointer_array<m_iNodeNameIndex,TYPE>::
//      xml_export(output_tree & xmlof)
//   {
//         xmlof.set_attr("count",this->get_size());
//         for(int32_t i = 0; i < this->get_size(); i++)
//         {
//            node * pnode = xmlof.export_node(xmlof.get_node_name(m_iNodeNameIndex),this->operator[](i));
//            pnode->add_attr("array_index",i);
//         }
//      }
//
//
//   template < int32_t m_iNodeNameIndex,class TYPE >
//   void
//      smart_pointer_array<m_iNodeNameIndex,TYPE>::
//      xml_import(input_tree & xmlif)
//   {
//         int32_t iSize;
//         xmlif.get_attr("count",iSize);
//         this->set_size_create(iSize);
//         for(int32_t i = 0; i < this->get_size(); i++)
//         {
//            attr_array attra(this->get_app());
//            attra.add("array_index",i);
//            xmlif.import_node(xmlif.get_node_name(m_iNodeNameIndex),attra,this->operator[](i));
//         }
//      }
//
//} // namespace xml
//

//
//namespace file
//{
//
//   
//   template < class T >
//   bool system::output(sp(::axis::application) papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,const char *),const char * lpszSource)
//   {
//
//      System.dir().mk(System.dir().name(pszOutput),papp);
//
//      ::file::binary_buffer_sp fileOut = papp->m_pbasesession->file().get_file(pszOutput,::file::mode_create | ::file::type_binary | ::file::mode_write);
//
//      if(fileOut.is_null())
//         return false;
//
//      ::file::output_stream ostream(fileOut);
//
//      return (p->*lpfnOuput)(ostream,lpszSource);
//
//   }
//
//
//   template < class T >
//   bool system::output(sp(::axis::application) papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,::file::input_stream &),const char * lpszInput)
//   {
//
//      System.dir().mk(System.dir().name(pszOutput),papp);
//
//      ::file::binary_buffer_sp fileOut = papp->m_pbasesession->file().get_file(pszOutput,::file::mode_create | ::file::type_binary | ::file::mode_write);
//
//      if(fileOut.is_null())
//         return false;
//
//      ::file::binary_buffer_sp fileIn = papp->m_pbasesession->file().get_file(lpszInput,::file::type_binary | ::file::mode_read);
//
//      if(fileIn.is_null())
//         return false;
//
//      ::file::output_stream ostream(fileOut);
//
//      ::file::input_stream istream(fileIn);
//
//      return (p->*lpfnOuput)(ostream,istream);
//
//   }
//
//
//   template < class T >
//   bool system::output(sp(::axis::application) papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,::file::input_stream &),::file::input_stream & istream)
//   {
//
//      ::file::output_stream ostream(get(pszOutput,papp));
//
//      return (p->*lpfnOuput)(ostream,istream);
//
//   }
//
//
//} // namespace file
//
//
//






//inline ::base::session & Sess(::axis::application * papp)
//{
//
//   return *papp->m_pbasesession;
//
//}
//
//
//inline ::base::session & root::Session
//{
//
//   return *m_paxisapp->m_pbasesession;
//
//}

//
//inline ::core::platform & root::platform()
//{
//
//   return *m_paxisapp->m_pcoreplatform;
//
//}
//
//





//
//
//namespace base
//{
//
//   // impl
//   template < class APP >
//   sp(::axis::application) single_application_library < APP > ::get_new_app(const char * pszAppId)
//   {
//
//      if(!contains_app(pszAppId))
//         return NULL;
//
//      sp(::axis::application) papp = canew(APP());
//
//      if(papp == NULL)
//         return NULL;
//
//      try
//      {
//
//         papp->construct(pszAppId);
//
//      }
//      catch(...)
//      {
//
//         try
//         {
//
//            papp.release();
//
//         }
//         catch(...)
//         {
//
//         }
//
//         return NULL;
//
//      }
//
//      return papp;
//
//   }
//
//
//} // namespace base
//
//
//
//
//
//
//
//



namespace file
{

   
   template < class T >
   bool system::output(sp(::axis::application) papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,const char *),const char * lpszSource)
   {

      System.dir().mk(Application.dir_name(pszOutput),papp);

      ::file::binary_buffer_sp fileOut = papp->m_paxissession->file_get_file(pszOutput,::file::mode_create | ::file::type_binary | ::file::mode_write);

      if(fileOut.is_null())
         return false;

      ::file::output_stream ostream(fileOut);

      return (p->*lpfnOuput)(ostream,lpszSource);

   }


   template < class T >
   bool system::output(sp(::axis::application) papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,::file::input_stream &),const char * lpszInput)
   {

      System.dir().mk(Application.dir_name(pszOutput),papp);

      ::file::binary_buffer_sp fileOut = papp->m_paxissession->file_get_file(pszOutput,::file::mode_create | ::file::type_binary | ::file::mode_write);

      if(fileOut.is_null())
         return false;

      ::file::binary_buffer_sp fileIn = papp->m_paxissession->file_get_file(lpszInput,::file::type_binary | ::file::mode_read);

      if(fileIn.is_null())
         return false;

      ::file::output_stream ostream(fileOut);

      ::file::input_stream istream(fileIn);

      return (p->*lpfnOuput)(ostream,istream);

   }


   template < class T >
   bool system::output(sp(::axis::application) papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,::file::input_stream &),::file::input_stream & istream)
   {

      ::file::output_stream ostream(get(pszOutput,papp));

      return (p->*lpfnOuput)(ostream,istream);

   }


} // namespace file


