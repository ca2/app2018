#pragma once


namespace collection
{


   template < class T >
   class smart_pointer_array :
      virtual public base_array < ::ca::smart_pointer < T > >
   {
   public:

      smart_pointer_array < T >()
      {
      }

      smart_pointer_array < T  > (::ca::application * papp) :
         ::ca::ca(papp),
         base_array < ::ca::smart_pointer < T > >(papp)
      {
      }


      sp(T) & add_new()
      {

         sp(T) & sp = base_array < ::ca::smart_pointer < T > >::add_new();

         sp.create(this->get_app());

         return sp;

      }


      ::count set_size(::count nNewSize, ::count nGrowBy = -1)
      {
         UNREFERENCED_PARAMETER(nGrowBy);
         while(nNewSize > this->get_size())
         {
            add_new();
         }
         if(this->get_size() > nNewSize)
         {
            this->base_array < ::ca::smart_pointer < T > >::set_size(nNewSize);
         }
         return this->get_size();
      }


      template < class DERIVED >
      DERIVED * get(index iStart = 0)
      {

         DERIVED * pderived = NULL;

         for(index i = iStart; i < this->get_count(); i++)
         {

            try
            {

               pderived = dynamic_cast < DERIVED * > (this->element_at(i).m_p);

            }
            catch(...)
            {

            }

            if(pderived != NULL)
               return pderived;

         }

         return NULL;

      }

      template < class DERIVED >
      DERIVED * typed_ptr_at(index iIndex)
      {

         return dynamic_cast < DERIVED * > (this->element_at(iIndex).m_p);

      }

      bool add_unique(T * p)
      {

         if(contains(p))
            return false;

         add(p);

         return true;

      }


      bool contains(T * p, index iStart = 0, ::count nCount = -1)
      {

         return find_first(p, iStart, nCount) >= 0;

      }

      ::index find_first(const T * p, index iStart = 0, ::count nCount = -1)
      {

         ::count c = 0;

         index iEnd;

         if(nCount < 0)
            iEnd = this->get_upper_bound(nCount);
         else
            iEnd = iStart + nCount - 1;

         for(int i = iStart; i <= iEnd; i++)
         {

            if(this->element_at(i).m_p == p)
               return i;

         }

         return -1;

      }


      ::index find_first(const T & t, index (* lpfnCompare)(const T *, const T *), index iStart = 0, ::count nCount = -1)
      {

         return this->find_first(&t, lpfnCompare, iStart, nCount);

      }


      ::index find_first(const T * p, index (* lpfnCompare)(const T *, const T *), index iStart = 0, ::count nCount = -1)
      {

         ::count c = 0;

         index iEnd;

         if(nCount < 0)
            iEnd = this->get_upper_bound(nCount);
         else
            iEnd = iStart + nCount - 1;

         for(int i = iStart; i <= iEnd; i++)
         {

            if(lpfnCompare(this->element_at(i).m_p, p) == 0)
               return i;

         }

         return -1;

      }

      template < class DERIVED >
      smart_pointer_array & operator -= (DERIVED * p)
      {

         this->remove(dynamic_cast < T * > (p));

         return *this;

      }

      ::count remove(T * p)
      {

         ::count c = 0;

         index iFind = 0;

         while((iFind = this->find_first(p, iFind)) >= 0)
         {

            c++;

            this->remove_at(iFind);

         }

         return c;

      }

      // overloaded operator helpers
      const T & operator[](index nIndex) const
      {

         return *this->base_array < ::ca::smart_pointer < T > >::operator[](nIndex).m_p;

      }

      T & operator[](index nIndex)
      {

         return *this->base_array < ::ca::smart_pointer < T > >::operator[](nIndex).m_p;

      }

      const ::ca::smart_pointer < T > & sp_at(index nIndex) const
      {

         return this->base_array < ::ca::smart_pointer < T > >::element_at(nIndex);

      }


      ::ca::smart_pointer < T > & sp_at(index nIndex)
      {

         return this->base_array < ::ca::smart_pointer < T > >::element_at(nIndex);

      }


      T * ptr_at(index nIndex) const
      {

         return this->sp_at(nIndex);

      }


      T * ptr_at(index nIndex)
      {

         return this->sp_at(nIndex);

      }


      T & at(index nIndex) const
      {

         return *this->ptr_at(nIndex);

      }


      T & at(index nIndex)
      {

         return *this->ptr_at(nIndex);

      }

      const ::ca::smart_pointer < T > & first_sp(index n = 0) const
      {

         return this->base_array < ::ca::smart_pointer < T > >::first_element(n);

      }


      ::ca::smart_pointer < T > & first_sp(index n = 0)
      {

         return this->base_array < ::ca::smart_pointer < T > >::first_element(n);

      }


      T * first_ptr(index n = 0) const
      {

         return this->first_sp(n);

      }


      T * first_ptr(index n = 0)
      {

         return this->first_sp(n);

      }


      T & first(index n = 0) const
      {

         return *this->first_ptr(n);

      }


      T & first(index n = 0)
      {

         return *this->first_ptr(n);

      }


      T * last_sp(index n = 0) const
      {

         return this->base_array < ::ca::smart_pointer < T > >::last_element(n);

      }


      ::ca::smart_pointer < T > & last_sp(index n = 0)
      {

         return this->base_array < ::ca::smart_pointer < T > >::last_element(n);

      }


      T * last_ptr(index n = 0) const
      {

         return this->last_sp(n);

      }


      T * last_ptr(index n = 0)
      {

         return this->last_sp(n);

      }


      T & last(index n = 0) const
      {

         return *last_ptr(n);

      }


      T & last(index n = 0)
      {

         return *this->last_ptr(n);

      }


      const ::ca::smart_pointer < T > & operator() (index nIndex) const
      {

         return this->sp_at(nIndex);

      }


      ::ca::smart_pointer < T > & operator() (index nIndex)
      {

         return this->sp_at(nIndex);

      }


      template < class ARRAY >
      smart_pointer_array & copy(ARRAY * pa)
      {

         if(pa == dynamic_cast < ARRAY * > (this))
            return *this;

         for(int i = 0; i < pa->get_count(); i++)
         {
            this->add(pa->operator[](i));
         }

         return *this;

      }

      template < class ARRAY >
      smart_pointer_array & copy(sp(ARRAY) pa)
      {

         if(pa == dynamic_cast < ARRAY * > (this))
            return *this;

         for(int i = 0; i < pa->get_count(); i++)
         {
            this->add(pa->operator[](i));
         }

         return *this;

      }

      smart_pointer_array & copy(const smart_pointer_array & a)
      {

         if(&a == this)
            return *this;

         for(int i = 0; i < a.get_count(); i++)
         {
            this->add(a[i]);
         }

         return *this;

      }


      template < class ARRAY >
      smart_pointer_array & copy_ptra(ARRAY * pptra)
      {

         for(int i = 0; i < pptra->get_count(); i++)
         {
            this->add(*pptra->operator[](i));
         }

         return *this;

      }

   };


} // namespace collection


#define spa(TYPE) ::collection::smart_pointer_array < TYPE >







