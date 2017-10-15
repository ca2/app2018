#pragma once


// raw_array is an array that does not call constructors or destructor in elements
// array is an array that call only copy constructor and destructor in elements
// array is an array that call default constructors, copy constructs and destructors in elements


template<class TYPE,class ARG_TYPE = const TYPE &,class ALLOCATOR = ::allocator::nodef < TYPE > >
class raw_array :
   public array_base < TYPE, ARG_TYPE, ALLOCATOR >
{
   public:

      typedef TYPE BASE_TYPE;
      typedef ARG_TYPE BASE_ARG_TYPE;
      typedef raw_array < TYPE,ARG_TYPE > BASE_ARRAY;
      typedef typename array_base < TYPE,ARG_TYPE, ALLOCATOR >::iterator iterator;

      //class iterator
      //{
      //public:

      //   typedef TYPE BASE_TYPE;
      //   typedef ARG_TYPE BASE_ARG_TYPE;
      //   typedef raw_array < TYPE,ARG_TYPE > BASE_ARRAY;

      //   index            m_i;
      //   raw_array *     m_parray;

      //   iterator()
      //   {
      //      m_i = 0;
      //      m_parray = NULL;
      //   }

      //   iterator(index i,raw_array * parray)
      //   {
      //      m_i = i;
      //      m_parray = parray;
      //   }

      //   iterator(const iterator & it)
      //   {
      //      operator = (it);
      //   }


      //   iterator & operator = (const iterator & it)
      //   {
      //      if(this != &it)
      //      {
      //         m_i         = it.m_i;
      //         m_parray    = it.m_parray;
      //      }
      //      return *this;
      //   }

      //   bool operator == (const iterator & it)
      //   {
      //      if(this == &it)
      //         return true;
      //      if(m_parray != it.m_parray)
      //         return false;
      //      if(m_i >= m_parray->get_size() && it.m_i >= m_parray->get_size())
      //         return true;
      //      if(m_i <= 0 && it.m_i <= 0)
      //         return true;
      //      return m_i == it.m_i;
      //   }

      //   bool operator != (const iterator & it)
      //   {
      //      return !operator==(it);
      //   }

      //   iterator operator ++(int)
      //   {
      //      iterator it = *this;
      //      operator ++();
      //      return it;
      //   }

      //   iterator operator --(int)
      //   {
      //      iterator it = *this;
      //      operator --();
      //      return it;
      //   }

      //   iterator & operator ++()
      //   {
      //      m_i++;
      //      if(m_i >= m_parray->get_size())
      //         m_i = m_parray->get_size();
      //      return *this;
      //   }

      //   iterator & operator +(index i)
      //   {
      //      m_i += i;
      //      if(m_i >= m_parray->get_size())
      //         m_i = m_parray->get_size();
      //      return *this;
      //   }

      //   iterator & operator --()
      //   {
      //      m_i--;
      //      if(m_i < 0)
      //         m_i = 0;
      //      return *this;
      //   }

      //   iterator mid(const iterator & i) const
      //   {
      //      return iterator((m_i + i.m_i + 1) / 2,m_parray);
      //   }

      //   iterator & operator -(::count c)
      //   {
      //      m_i-=c;
      //      if(m_i < 0)
      //         m_i = 0;
      //      return *this;
      //   }

      //   bool operator < (const iterator & i) const
      //   {

      //      return m_i < i.m_i;

      //   }

      //   ::count get_count() const
      //   {
      //      return m_parray->get_count();
      //   }


      //   TYPE & operator * ()
      //   {
      //      return ((TYPE*)m_parray->m_pData)[m_i];
      //   }

      //   const TYPE & operator * () const
      //   {
      //      return ((const TYPE*)m_parray->m_pData)[m_i];
      //   }


      //};


      //class const_iterator
      //{
      //public:


      //   typedef TYPE BASE_TYPE;
      //   typedef ARG_TYPE BASE_ARG_TYPE;
      //   typedef raw_array < TYPE,ARG_TYPE > BASE_ARRAY;


      //   index            m_i;
      //   const raw_array *     m_parray;

      //   const_iterator()
      //   {
      //      m_i = 0;
      //      m_parray = NULL;
      //   }

      //   const_iterator(index i,const raw_array * parray)
      //   {
      //      m_i = i;
      //      m_parray = parray;
      //   }

      //   const_iterator(const iterator & it)
      //   {
      //      operator = (it);
      //   }

      //   const_iterator(const const_iterator & it)
      //   {
      //      operator = (it);
      //   }

      //   const_iterator & operator = (const iterator & it)
      //   {
      //      m_i         = it.m_i;
      //      m_parray    = it.m_parray;
      //      return *this;
      //   }

      //   const_iterator & operator = (const const_iterator & it)
      //   {
      //      if(this != &it)
      //      {
      //         m_i         = it.m_i;
      //         m_parray    = it.m_parray;
      //      }
      //      return *this;
      //   }

      //   bool operator == (const const_iterator & it)
      //   {
      //      if(this == &it)
      //         return true;
      //      if(m_parray != it.m_parray)
      //         return false;
      //      if(m_i >= m_parray->get_size() && it.m_i >= m_parray->get_size())
      //         return true;
      //      if(m_i <= 0 && it.m_i <= 0)
      //         return true;
      //      return m_i == it.m_i;
      //   }

      //   bool operator != (const const_iterator & it)
      //   {
      //      return !operator==(it);
      //   }

      //   const_iterator operator ++(int)
      //   {
      //      const_iterator it = *this;
      //      operator ++();
      //      return it;
      //   }

      //   const_iterator operator --(int)
      //   {
      //      const_iterator it = *this;
      //      operator --();
      //      return it;
      //   }

      //   const_iterator & operator ++()
      //   {
      //      m_i++;
      //      if(m_i >= m_parray->get_size())
      //         m_i = m_parray->get_size();
      //      return *this;
      //   }

      //   const_iterator & operator +(index i)
      //   {
      //      m_i += i;
      //      if(m_i >= m_parray->get_size())
      //         m_i = m_parray->get_size();
      //      return *this;
      //   }

      //   const_iterator & operator --()
      //   {
      //      m_i--;
      //      if(m_i < 0)
      //         m_i = 0;
      //      return *this;
      //   }

      //   const_iterator mid(const const_iterator & i) const
      //   {
      //      return const_iterator_base((m_i + i.m_i + 1) / 2,m_parray);
      //   }

      //   const_iterator & operator -(::count c)
      //   {
      //      m_i-=c;
      //      if(m_i < 0)
      //         m_i = 0;
      //      return *this;
      //   }

      //   bool operator < (const const_iterator & i) const
      //   {

      //      return m_i < i.m_i;

      //   }

      //   ::count get_count() const
      //   {
      //      return m_parray->get_count();
      //   }

      //   const TYPE & operator * ()
      //   {
      //      return ((const TYPE*)m_parray->m_pData)[m_i];
      //   }

      //};


      raw_array(::aura::application * papp = NULL, ::count nGrowBy = 32);
      raw_array(std::initializer_list < TYPE >  l);
      raw_array(const raw_array <TYPE, ARG_TYPE, ALLOCATOR> & a);
      raw_array(raw_array <TYPE,ARG_TYPE,ALLOCATOR> && a);
      raw_array(::count n);
      raw_array(ARG_TYPE t, ::count n = 1);
      raw_array(TYPE * ptypea, ::count n);
      virtual ~raw_array();


#ifdef DEBUG
      virtual void assert_valid() const override;
      virtual void dump(dump_context & dumpcontext) const override;
#endif




      void remove_last();

      ::count remove_all();
      void clear();




      // Accessing elements
      inline const TYPE& get_at(index nIndex) const;
      inline TYPE& get_at(index nIndex);
      inline void set_at(index nIndex, ARG_TYPE newElement);


      // Direct Access to the element data (may return NULL)
      inline const TYPE* get_data() const;
      inline TYPE* get_data();

      // Potentially growing the raw_array
      void set_at_grow(index nIndex, ARG_TYPE newElement);
      TYPE & element_at_grow(index nIndex);
      TYPE get_at_grow(index nIndex);
      index add(ARG_TYPE newElement);
      index add(const raw_array& src);
      virtual index add_new(::count count);
      virtual TYPE & add_new();
      //index append(const raw_array& src);
      //void copy(const raw_array& src);


      inline TYPE pop(index index = -1);
      inline index push(ARG_TYPE newElement,index i = 0);
      inline void pop_back(index index = -1);
      inline void push_back(ARG_TYPE newElement, index = 0);



      iterator erase(iterator pos);
      iterator erase(iterator first, iterator last);
      iterator begin()
      {
         return iterator(0, this);
      }

      iterator end()
      {
         return iterator(this->get_size(), this);
      }


      // overloaded operator helpers
      inline const TYPE& operator[](index nIndex) const;
      inline TYPE& operator[](index nIndex);

      // Operations that move elements around
      index insert_at(index nIndex, ARG_TYPE newElement, ::count nCount = 1);
      //index remove_at(index nIndex, ::count nCount = 1);
      //void _001RemoveIndexes(index_array & ia);
      //void remove_indexes(const index_array & ia); // remove indexes from index raw_array upper bound to index raw_array lower bound
      //void remove_descending_indexes(const index_array & ia); // remove indexes from index raw_array lower bound to index raw_array upper bound
      //index insert_at(index nStartIndex, raw_array* pNewArray);
      void swap(index index1, index index2);

      raw_array & operator = (const raw_array & src);

      index find_first(ARG_TYPE t, index (* lpfnCompare)(ARG_TYPE, ARG_TYPE), index start = 0, index last = -1) const;
      index raw_find_first(TYPE * pt, index first = 0, index last = -1) const;

      template < class DERIVED >
      index find_first(DERIVED * pt, index first = 0, index last = -1)
      {
         return raw_find_first(dynamic_cast < TYPE * > (pt), first, last);
      }

      template < class ARRAY >
      ::count get_array(ARRAY & a, index iStart = 0, ::count nCount = -1)
      {

         ::count ca = 0;

         index iEnd;

         if(nCount < 0)
            iEnd = this->get_upper_bound(nCount);
         else
            iEnd = iStart + nCount - 1;

         for(index i = iStart; i <= iEnd; i++)
         {

            a.add(this->element_at(i));
            ca++;

         }

         return ca;

      }

      //void quick_sort(index (* fCompare)(TYPE *, TYPE *));
      //void quick_sort(index (* fCompare)(TYPE *, TYPE *), void (* fSwap)(TYPE *, TYPE *));
      //void quick_sort(index (* fCompare)(TYPE *, TYPE *), index_array & ia);
      //bool binary_search(ARG_TYPE t, index & iIndex, index ( * fCompare ) (TYPE *, TYPE *)) const;

      //bool binary_search(ARG_TYPE t, index & iIndex, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia) const;
      //index sort_add(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia);
      //::count sort_add(const raw_array < TYPE, ARG_TYPE > & a, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia);
      //index sort_remove(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia);



      raw_array & operator += (const raw_array & a);
      raw_array operator + (const raw_array & a) const;


      inline raw_array & operator = (raw_array && a);
      inline raw_array & move(raw_array && a);


      operator TYPE *() { return this->m_pData;  }
      operator const TYPE *() const { return this->m_pData; }
      operator count () const { return this->get_count(); }


};


