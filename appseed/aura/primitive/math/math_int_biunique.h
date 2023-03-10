#pragma once


template < class T, class T_to_T = map < T, T, T, T > >
class biunique :
   virtual public ::object
{
public:


   biunique(::aura::application * papp = NULL);
   virtual ~biunique()
   {
   }

   bool has_a(T a) const;
   bool has_b(T b) const;


   T calc_max_a();
   T calc_max_b();


   T remove_a(T a);
   T remove_b(T b);

   T array_translate_a(T aNew, T aOld);

   T translate_a(T a1, T a2);
   T translate_b(T b1, T b2);


   T get_a(T b) const;
   T get_b(T a) const;
   void set(T a, T b);

   void swap(T a1, T a2);

   T operator [](T a) const
   {
      return get_b(a);
   }

   T get_max_a() const;
   T get_max_b() const;

   void remove_all();

   void set_empty_a(T iEmpty);
   void set_empty_b(T iEmpty);

   T get_free_a() const;
   T get_free_b() const;

   T add(T b);
   T add_b_in_first_free_a(T b);
   T add_b_in_first_free_a_mod_w(T b, T w, T mod);
   T add_unique(T b);
   T get_size() const;

   void copy_data(const biunique & ia);

   virtual void io(stream & serialize);

   biunique & operator = (const biunique & ia);

   void biunivoca(bool b = true);

   bool m_bBiunivoca;
   T m_iMaxA;
   T m_iMaxB;
   T m_iEmptyA;
   T m_iEmptyB;
   T_to_T m_ab;
   T_to_T m_ba;
};



template < class T, class T_to_T >
biunique < T, T_to_T > ::biunique(::aura::application * papp) :
   object(papp)
{
   m_bBiunivoca = true;
   m_iEmptyA = -1;
   m_iEmptyB = -1;
   m_iMaxA = -1;
   m_iMaxB = -1;
}


template < class T, class T_to_T >
void biunique < T, T_to_T > ::biunivoca(bool b)
{
   m_bBiunivoca = b;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::get_a (T b) const
{
   T a;
   if(m_ba.Lookup(b, a))
      return a;
   return m_iEmptyA;
}


template < class T, class T_to_T >
T biunique < T, T_to_T > ::get_b (T a) const
{
   T b;
   if(m_ab.Lookup(a, b))
      return b;
   return m_iEmptyB;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::remove_a(T a)
{
   if(has_a(a))
   {
      T b = get_b(a);
      m_ba.remove_key(b);
      m_ab.remove_key(a);
      m_iMaxA = calc_max_a();
      m_iMaxB = calc_max_b();
      return b;
   }
   return m_iEmptyB;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::array_translate_a(T aNew, T aOld)
{

   if (aOld < 0 || aNew < 0)
   {
      return m_iEmptyB;
   }

   T bParam = get_b(aOld);
   if (bParam == m_iEmptyB)
   {
      return m_iEmptyB;
   }

   if (aNew == aOld)
   {
      return bParam;
   }

   m_ba.remove_key(bParam);
   m_ab.remove_key(aOld);
   m_iMaxA = calc_max_a();
   m_iMaxB = calc_max_b();
   //if (aNew > aOld)
   //{
   //   T a = aOld;
   //   while (a + 1 <= m_iMaxA)
   //   {
   //      T b = get_b(a + 1);
   //      if (b == m_iEmptyB)
   //      {
   //         break;
   //      }
   //      m_ba.remove_key(b);
   //      m_ab.remove_key(a + 1);
   //      m_ba.set_at(b, a);
   //      m_ab.set_at(a, b);
   //      m_iMaxA = calc_max_a();
   //      m_iMaxB = calc_max_b();
   //      a++;
   //   }
   //}

   // making room
   if (has_a(aNew))
   {
      T a = aNew;
      T b;
      while (true)
      {
         a++;
         if (a > m_iMaxA)
         {
            break;
         }
         b = get_b(a);
         if (b == m_iEmptyB)
         {
            break;
         }
      }
      a--;
      while (a >= aNew)
      {
         b = get_b(a);
         if (b != m_iEmptyB)
         {
            m_ba.remove_key(b);
            m_ab.remove_key(a);
            m_ba.set_at(b, a + 1);
            m_ab.set_at(a + 1, b);
         }
         a--;
      }
   }

   // actually (in)se(r)tting
   m_ba.set_at(bParam, aNew);
   m_ab.set_at(aNew, bParam);

   m_iMaxA = calc_max_a();
   m_iMaxB = calc_max_b();

   return bParam;

}


template < class T, class T_to_T >
T biunique < T, T_to_T > ::remove_b(T b)
{
   if(has_b(b))
   {
      T a = get_a(b);
      m_ab.remove_key(a);
      m_ba.remove_key(b);
      m_iMaxA = calc_max_a();
      m_iMaxB = calc_max_b();
      return a;
   }
   return m_iEmptyA;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::translate_a(T a1, T a2)
{
   T b = remove_a(a2);
   set(a1, b);
   return b;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::translate_b(T b1, T b2)
{
   T a = remove_b(b2);
   set(a, b1);
   return a;
}

template < class T, class T_to_T >
void biunique < T, T_to_T > ::set(T a, T b)
{
   if(m_bBiunivoca)
   {
      if(has_a(a))
         remove_a(a);
      if(has_b(b))
         remove_b(b);
   }
   m_ab.set_at(a, b);
   m_ba.set_at(b, a);
   m_iMaxA = calc_max_a();
   m_iMaxB = calc_max_b();
}


template < class T, class T_to_T >
T biunique < T, T_to_T > ::get_max_a() const
{
   return m_iMaxA;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::get_max_b() const
{
   return m_iMaxB;
}



template < class T, class T_to_T >
void biunique < T, T_to_T > ::remove_all()
{
   m_ab.remove_all();
   m_ba.remove_all();
   m_iMaxA = -1;
   m_iMaxB = -1;
}

template < class T, class T_to_T >
void biunique < T, T_to_T > ::set_empty_a(T iEmpty)
{
   m_iEmptyA = iEmpty;
}

template < class T, class T_to_T >
void biunique < T, T_to_T > ::set_empty_b(T iEmpty)
{
   m_iEmptyB = iEmpty;
}

template < class T, class T_to_T >
void biunique < T, T_to_T > ::copy_data(const biunique & function)
{
   if(&function != this)
   {
      m_ab = function.m_ab;
      m_ba = function.m_ba;
   }
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::add(T b)
{
   T a = get_max_a() + 1;
   set(a, b);
   return a;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::add_b_in_first_free_a(T b)
{
   T a = get_free_a();
   set(a, b);
   return a;
}


template < class T, class T_to_T >
T biunique < T, T_to_T > ::add_b_in_first_free_a_mod_w(T b, T w, T mod)
{
   if(mod <= 1)
      return add_b_in_first_free_a(b);
   if(w <= 0)
      return add_b_in_first_free_a(b);
   T ca = get_max_a() + 1;
   T a;
   for(a = 0; a < ca ; a++)
   {
      if((a % mod) >= w)
      {
         a = ((a / mod) + 1) * mod;
      }
      if(get_b(a) == m_iEmptyB)
      {
         set(a, b);
         return a;
      }
   }
   set(a, b);
   return a;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::add_unique(T b)
{
   T a;
   if(m_ba.Lookup(b, a))
      return a;
   a = get_max_a() + 1;
   set(a, b);
   return a;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::get_size() const
{
   return get_max_a() + 1;
}

template < class T, class T_to_T >
void biunique < T, T_to_T > ::swap(T a1, T a2)
{
   // because of Biunivoca
   T b1 = get_b(a1);
   T b2 = get_b(a2);
   set(a1, b2);
   set(a2, b1);

}

template < class T, class T_to_T >
bool biunique < T, T_to_T > ::has_a(T a) const
{
   T b;
   return m_ab.Lookup(a, b) != FALSE;
}


template < class T, class T_to_T >
bool biunique < T, T_to_T > ::has_b(T b) const
{
   T a;
   return m_ba.Lookup(b, a) != FALSE;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::get_free_a() const
{
   for(T a = 0; a <= m_iMaxA; a++)
   {
      if(!has_a(a))
         return a;
   }
   return m_iMaxA + 1;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::get_free_b() const
{
   for(T b = 0; b <= m_iMaxB; b++)
   {
      if(!has_b(b))
         return b;
   }
   return m_iMaxB + 1;
}

template < class T, class T_to_T >
biunique < T, T_to_T > & biunique < T, T_to_T > ::operator = (const biunique & ia)
{
   if(this != &ia)
   {
      m_bBiunivoca = ia.m_bBiunivoca;
      m_iMaxA = ia.m_iMaxA;
      m_iMaxB = ia.m_iMaxB;
      m_iEmptyA = ia.m_iEmptyA;
      m_iEmptyB = ia.m_iEmptyB;
      m_ab = ia.m_ab;
      m_ba = ia.m_ba;
   }
   return *this;
}


template < class t1, class t2, class t3, class t4 >
void serialize_write(serialize & ostream, map < t1, t2, t3, t4 > & m)
{
   ::count count = m.get_count();
   typename map < t1, t2, t3, t4 >::pair * ppair = m.PGetFirstAssoc();
   ostream << count;
   while(ppair != NULL)
   {
      ostream << ppair->m_element1;
      ostream << ppair->m_element2;
      ppair = m.PGetNextAssoc(ppair);
   }
}

template < class t1, class t2, class t3, class t4 >
void serialize_read(serialize & istream, map < t1, t2, t3, t4 > & m)
{
   try
   {
      t1 iCount;
//      class map < t1, t2, t3, t4 >::pair * ppair =
      //       m.PGetFirstAssoc();
      istream >> iCount;
      t1 key;
      t3 value;
      m.remove_all();
      for(int32_t i = 0; i < iCount; i++)
      {
         istream >> key;
         istream >> value;
         m.set_at(key, value);
      }
   }
   catch(const char * psz)
   {
      m.remove_all();
      _throw(::simple_exception(get_app(), psz));
   }
}


template < class T, class T_to_T >
void biunique < T, T_to_T > ::io(stream & stream)
{

   stream(m_bBiunivoca);
   stream(m_iMaxA);
   stream(m_iMaxB);
   stream(m_iEmptyA);
   stream(m_iEmptyB);

   if(stream.is_storing())
   {

      if (m_bBiunivoca)
      {
         stream.stream_map(m_ab);
      }
      else
      {
         stream.stream_map(m_ab);
         stream.stream_map(m_ba);
      }

   }
   else
   {

      try
      {
         if (m_bBiunivoca)
         {
            T_to_T ab;
            stream.stream_map(ab);
            typename T_to_T::pair * ppair = ab.PGetFirstAssoc();
            while (ppair != NULL)
            {
               set(ppair->m_element1, ppair->m_element2);
               ppair = ab.PGetNextAssoc(ppair);
            }
         }
         else
         {
            stream.stream_map(m_ab);
            stream.stream_map(m_ba);
         }
      }
      catch (const char * psz)
      {
         m_ab.remove_all();
         m_ba.remove_all();
         m_bBiunivoca = true;
         m_iEmptyA = -1;
         m_iEmptyB = -1;
         m_iMaxA = -1;
         m_iMaxB = -1;
         _throw(::simple_exception(get_app(), psz));
      }

   }
}


template < class T, class T_to_T >
T biunique < T, T_to_T > ::calc_max_a()
{
   typename T_to_T::pair * ppair =
   m_ab.PGetFirstAssoc();
   T iMaxA = -1;
   while(ppair != NULL)
   {
      if(ppair->m_element1 > iMaxA)
         iMaxA = ppair->m_element1;
      ppair = m_ab.PGetNextAssoc(ppair);
   }
   return iMaxA;
}

template < class T, class T_to_T >
T biunique < T, T_to_T > ::calc_max_b()
{
   typename T_to_T::pair * ppair =
   m_ba.PGetFirstAssoc();
   T iMaxB = -1;
   while(ppair != NULL)
   {
      if(ppair->m_element1 > iMaxB)
         iMaxB = ppair->m_element1;
      ppair = m_ba.PGetNextAssoc(ppair);
   }
   return iMaxB;
}



class CLASS_DECL_AURA index_biunique :
   public biunique < index >
{
public:
   index_biunique(::aura::application * papp = NULL);
   virtual ~index_biunique() {}
};

class CLASS_DECL_AURA int_biunique :
   public biunique < int32_t, int_to_int >
{
public:
   int_biunique(::aura::application * papp = NULL);
   virtual ~int_biunique() {}
};










CLASS_DECL_AURA index array_translate_a(index_biunique & ia, index aNew, index aOld);





