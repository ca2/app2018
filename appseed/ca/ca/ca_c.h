#pragma once



namespace ca
{

   template < class c_derived >
   inline int64_t add_ref(c_derived * pca)
   {
      if(pca == ::null())
         return -1;
      return pca->add_ref();
   }

   template < class c_derived >
   inline int64_t release(c_derived * & pca)
   {
      if(pca == ::null())
         return -1;
      int64_t count = pca->::ca::c::release();
      pca = ::null();
      return count;
   }

   template < class c_derived >
   inline int64_t ref_count(c_derived * pca)
   {
      if(pca == ::null())
         return -1;
      return pca->get_ref_count();
   }


   template < class TYPE >
   bool is_null(TYPE * p)
   {
      return (((int_ptr) p) < sizeof(TYPE));
   }

   template <class t>
   inline void delptr(t *& p)
   {
      if(p != ::null())
      {
         delete p;
         p = ::null();
      }
   }


   class CLASS_DECL_ca c
   {
   public:


      int64_t                    m_countReference;


      c();
      virtual ~c();


      virtual void delete_this();




      inline int64_t get_ref_count()
      {
         return m_countReference;
      }

      inline int64_t add_ref()
      {
         return InterlockedIncrement64(&m_countReference);
      }

      inline int64_t release()
      {
         int64_t i = InterlockedDecrement64(&m_countReference);
         if(i == 0)
         {
            delete_this();
         }
         return i;
      }



   };

   template < class T >
   T * dereference_no_delete(T * p) { p->m_countReference--; return p; }






} // namespace ca





#define canew(x) ::ca::dereference_no_delete(new x)
