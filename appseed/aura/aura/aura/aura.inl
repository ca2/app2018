﻿#pragma once


inline lparam::lparam(::object * p)
{
   p->add_ref();
   m_lparam = (LPARAM)p;
}


template<class TYPE>
inline void dump_elements(dump_context & dumpcontext, const TYPE* pElements, ::count nCount)
{
   ENSURE((nCount == 0) || (pElements != NULL));
   ASSERT((nCount == 0) ||
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
   ENSURE((nCount == 0) || ((pDest != 0) && (pSrc != 0)));
   ASSERT((nCount == 0) || __is_valid_address(pDest, (size_t)nCount * sizeof(TYPE)));
   ASSERT((nCount == 0) || __is_valid_address(pSrc, (size_t)nCount * sizeof(TYPE)));

   // default is element-copy using assignment
   while (nCount--)
      *pDest++ = *pSrc++;
}


#undef new

#if defined(LINUX) || defined(METROWIN) || defined(ANDROID)


inline void * __cdecl operator new(size_t nSize, void * p)
{

   UNREFERENCED_PARAMETER(nSize);

   return p;

}



inline void __cdecl operator delete(void * p, void * palloc) del_throw_spec
{

   UNREFERENCED_PARAMETER(p);
   UNREFERENCED_PARAMETER(palloc);

}


#endif




class CLASS_DECL_AURA c_class
{
public:


   static c_class s_cclass;


   c_class();
   c_class(const c_class &);
   virtual ~c_class();


};


inline CLASS_DECL_AURA void * __cdecl operator new (size_t size, const c_class &)
{

   return ace_memory_alloc(size);

}


inline CLASS_DECL_AURA void * __cdecl operator new[](size_t size, const c_class &)
{

   return ace_memory_alloc(size);

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

#if MEMDLEAK

   return ace_memory_alloc(nSize);

#else

   return memory_alloc_dbg(nSize, nType, lpszFileName, nLine);

#endif

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








namespace xml
{


   template < int32_t m_iNodeNameIndex,class TYPE >
   void
   smart_pointer_array<m_iNodeNameIndex,TYPE>::
   xml_export(output_tree & xmlof)
   {
      xmlof.set_attr("count",this->get_size());
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         node * pnode = xmlof.export_node(xmlof.get_node_name(m_iNodeNameIndex),this->operator()(i));
         pnode->add_attr("array_index",i);
      }
   }


   template < int32_t m_iNodeNameIndex,class TYPE >
   void
   smart_pointer_array<m_iNodeNameIndex,TYPE>::
   xml_import(input_tree & xmlif)
   {
      int32_t iSize;
      xmlif.get_attr("count",iSize);
      this->set_size_create(iSize);
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         attr_array attra(this->get_app());
         attra.set_at("array_index",i);
         xmlif.import_node(xmlif.get_node_name(m_iNodeNameIndex),attra,this->operator()(i));
      }
   }

} // namespace xml


//
//namespace file
//{
//
//
//   template < class T >
//   bool system::output(::aura::application * papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::ostream &,const char *),const char * lpszSource)
//   {
//
//      System.dir().mk(System.dir().name(pszOutput),papp);
//
//      ::file::file_sp fileOut = papp->m_paurasession->file_get_file(pszOutput,::file::mode_create | ::file::type_binary | ::file::mode_write);
//
//      if(fileOut.is_null())
//         return false;
//
//      ::file::ostream ostream(fileOut);
//
//      return (p->*lpfnOuput)(ostream,lpszSource);
//
//   }
//
//
//   template < class T >
//   bool system::output(::aura::application * papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::ostream &,::file::istream &),const char * lpszInput)
//   {
//
//      System.dir().mk(System.dir().name(pszOutput),papp);
//
//      ::file::file_sp fileOut = papp->m_paurasession->file_get_file(pszOutput,::file::mode_create | ::file::type_binary | ::file::mode_write);
//
//      if(fileOut.is_null())
//         return false;
//
//      ::file::file_sp fileIn = papp->m_paurasession->file_get_file(lpszInput,::file::type_binary | ::file::mode_read);
//
//      if(fileIn.is_null())
//         return false;
//
//      ::file::ostream ostream(fileOut);
//
//      ::file::istream istream(fileIn);
//
//      return (p->*lpfnOuput)(ostream,istream);
//
//   }
//
//
//   template < class T >
//   bool system::output(::aura::application * papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::ostream &,::file::istream &),::file::istream & istream)
//   {
//
//      ::file::ostream ostream(get(pszOutput,papp));
//
//      return (p->*lpfnOuput)(ostream,istream);
//
//   }
//
//
//} // namespace file
//
//


namespace aura
{

   template <  >
   inline void del(::object * & p)
   {

      //Thank you Fiora a Eterna!!

      //Fiora Aeterna☄ ‏@FioraAeterna 20m20 minutes ago

      //   maybe it's cynical but I'm starting to think the real reason so many newer games have constant autosaves is because they crash all the time
      //   Details

      // BRT 2015-02-18 19:08
      // catch all (...) here in aura::del ... but should remove try catch from all underlying calls (frees, memory_frees, memory_dbg_frees).

      try
      {

         if (p != NULL)
         {

            if (!(p->m_ulFlags & ::object::flag_shared))
            {

               ::object * pdel = p;

               p = NULL;

               delete pdel;

            }

         }

      }
      catch (...)
      {

      }

   }



   template < class APP >
   sp(::aura::application) single_application_library < APP > ::get_new_application(const char * pszAppId)
   {

      if(!contains_app(pszAppId))
      {

         return NULL;

      }

      sp(::aura::application) papp = canew(APP());

      if(papp == NULL)
      {

         return NULL;

      }

      return papp;

   }



   template < class APP >
   void single_application_library < APP > ::get_extension_list(stringa & stra)
   {

      if (m_strFileExt.has_char())
      {

         stra.add(m_strFileExt);

      }

   }

   template < typename T >
   void application::alloc(T * & pt)
   {

      if(pt != NULL)
      {

         ::release(pt);

      }

      sp(T) sp = alloc(System.type_info < T >());

      pt = sp.m_p;

      pt->add_ref();

   }


} // namespace aura





//void object::keep_alive()
//{
//   try
//   {
//      on_keep_alive();
//   }
//   catch(...)
//   {
//   }
//}
//
//void on_keep_alive();
//virtual bool is_alive();




inline bool IsDirSep(WCHAR ch)
{

   return (ch == '\\' || ch == '/');

}



#define new AURA_NEW








template < typename T >
::file::path memcnts_path(T * pthis)
{

   string str = typeid(*pthis).name();

   str.replace("::", "/");

   return memcnts_base_path() / (str + ".txt");

}

template < typename T >
void memcnts_inc(T * pthis)
{

   if (memcnts())
   {

      synch_lock sl(g_pmutexMemoryCounters);

      ::file::path path = memcnts_path(pthis);

      int i = atoi(file_as_string_dup(path));

      file_put_contents_dup(path, ::str::from(i + 1));

   }


}

template < typename T >
void memcnts_dec(T * pthis)
{

   if (memcnts())
   {

      synch_lock sl(g_pmutexMemoryCounters);

      ::file::path path = memcnts_path(pthis);

      int i = atoi(file_as_string_dup(path));

      file_put_contents_dup(path, ::str::from(i - 1));

   }

}


namespace message
{


   template < typename RECEIVER >
   void sender::add_route (RECEIVER * preceiverDerived, void (RECEIVER::* phandler)(::message::message * pmessage), ::message::id id)
   {

      synch_lock sl(m_pmutexIdRoute);

      ::object * pobjectReceiver = dynamic_cast < ::object * > (preceiverDerived);

      sp(type) ptypeReceiver = System.type_info < RECEIVER >();

      if (m_idroute[id].pred_find_first([=](auto & proute)
   {

      return proute->m_pobjectReceiver == pobjectReceiver && proute->m_ptypeReceiver == ptypeReceiver;

   }) >= 0)
      {

         return;

      }

      ::message::receiver * preceiver = dynamic_cast < ::message::receiver * >(preceiverDerived);

      if (preceiver == NULL)
      {

         ASSERT(FALSE);

         return;

      }

      auto pred = [=](::message::message * pmessage)
      {

         (preceiverDerived->*phandler)(pmessage);

      };

      route * proute = create_pred_route(preceiver, pobjectReceiver, pred, ptypeReceiver);

      m_idroute[id].add(proute);

      preceiver->m_sendera.add_unique(this);

   }



} // namespace message


namespace heap
{


   template < typename T >
   string & heap < T > ::to_string(string & str) const
   {

      return str = this->operator const T *();

   }


} // namespace heap

