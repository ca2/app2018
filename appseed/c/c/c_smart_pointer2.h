#pragma once


// ::ca::null back link to operational system oswindow.h
//
//
//
//
// operational system null/void itself to a windowing service
//
// Curitiba, inha-metro-win-ubuntu-mountain-lion-macos 4 de novembro de 2012
namespace c
{




   template < class T >
   smart_pointer < T > ::smart_pointer()
   {
      m_p = ::null();
   }


   template < class T >
   smart_pointer < T > ::smart_pointer(const smart_pointer < T > & t)
   {
      m_p = ::null();
      operator = (t);
   }

   template < class T >
   smart_pointer < T > ::smart_pointer(smart_pointer < T > && t)
   {

      m_p      = t.m_p;
      t.m_p    = ::null();

   }




   template < class T >
   smart_pointer < T > ::smart_pointer(const ::ca::null &)
   {
      m_p = ::null();
   }


   template < class T >
   smart_pointer < T > ::smart_pointer(T * p)
   {
      m_p = p;
      ::c::add_ref(m_p);
   }

   template < class T >
   smart_pointer < T > ::smart_pointer(lparam lparam)
   {
      m_p = (T *) lparam.m_lparam;
   }

   template < class T >
   smart_pointer < T > ::smart_pointer(void * p)
   {
      m_p = (T *) p;
      ::c::add_ref(m_p);
   }


   template < class T >
   smart_pointer < T > ::smart_pointer(const ::ca::allocatorsp & a)
   {
      m_p = ::null();
      create(a);
   }

   template < class T >
   smart_pointer < T > ::smart_pointer(::ca::allocatorsp && a)
   {
      m_p = ::null();
      create(a);
   }

   template < class T >
   smart_pointer < T > ::~smart_pointer()
   {
      release();
   }

   template < class T >
   inline T * smart_pointer < T > ::operator ->()
   {
      return m_p;
   }

   template < class T >
   inline T * smart_pointer < T > ::operator ->() const
   {
      return m_p;
   }

   template < class T >
   inline smart_pointer < T > ::operator T & ()
   {
      return *m_p;
   }

   template < class T >
   inline smart_pointer < T > ::operator T & () const
   {
      return *m_p;
   }

   template < class T >
   inline smart_pointer < T > ::operator T * const ()
   {
      return m_p;
   }

   template < class T >
   inline smart_pointer < T > ::operator T * const () const
   {
      return m_p;
   }

   template < class T >
   inline smart_pointer < T > ::operator lparam ()
   {
      ::c::add_ref(m_p);
      return lparam(m_p);
   }

   template < class T >
   inline smart_pointer < T > ::operator lparam () const
   {
      ::c::add_ref(((smart_pointer *) this)->m_p);
      return lparam(m_p);
   }

/*   template < class T >
   inline smart_pointer < T > ::operator void * ()
   {
      ::c::add_ref(m_p);
      return (void *) m_p;
   }

   template < class T >
   inline smart_pointer < T > ::operator void * () const
   {
      ::c::add_ref(((smart_pointer *) this)->m_p);
      return (void *) m_p;
   }
*/

   template < class T >
   inline T * smart_pointer < T > ::get_()
   {
      return m_p;
   }

   template < class T >
   inline const T * smart_pointer < T > ::get_() const
   {
      return m_p;
   }

   template < class T >
   inline bool smart_pointer < T > ::is_null() const
   {
      return m_p == ::null();
   }

   template < class T >
   inline bool smart_pointer < T > ::is_set() const
   {
      return m_p != ::null();
   }

   template < class T >
   inline smart_pointer < T > & smart_pointer < T > ::operator = (T * p)
   {
      if(m_p != p)
      {
         T * pOld = m_p;
         if(p != ::null())
         {
            ::c::add_ref(p);
         }
         m_p = p;
         if(pOld != ::null())
         {
            ::c::release(pOld);
         }
      }
      return *this;
   }

   template < class T >
   inline smart_pointer < T > & smart_pointer < T > ::operator = (const smart_pointer < T > & t)
   {
      return operator = (t.m_p);
   }

template < class T >
   inline smart_pointer < T > & smart_pointer < T > ::operator = (smart_pointer < T > && t)
   {

      if(&t != this)
      {

         m_p      = t.m_p;
         t.m_p    = ::null();

      }

      return *this;

   }

   template < class T >
   inline smart_pointer < T > & smart_pointer < T > ::operator = (const ::ca::null & )
   {

      if(is_set())
         release();

      return * this;

   }

   template < class T >
   inline smart_pointer < T > & smart_pointer < T > ::operator = (LPARAM lparam)
   {

      release();

      m_p = (T *) lparam;

      return * this;

   }

   template < class T >
   inline smart_pointer < T > & smart_pointer < T > ::operator = (void * p)
   {

      release();

      m_p = (T *) p;

      return * this;

   }

   template < class T >
   inline T * smart_pointer < T > ::detach()
   {
      T * p = m_p;
      m_p = ::null();
      return p;
   }


   template < class T >
   void smart_pointer < T > ::oattrib(const sp(T) & p)
   {
      if(p.m_p == ::null())
      {
         release();
         return;
      }
      if(is_null())
         create(p->get_app());
      *m_p = *p.m_p;
   }

    
    // cut and paste with very good capabilities of RealVNC for MacOS in OVH.fr/eu/pt cloud from Windows client.
    // slashes with ABNT 2 keyboard and even c cedilha working with RealVNC
    template < class T >
    void smart_pointer <T>::release()
    {
        
        ::c::release(m_p);
        
    }


} // namespace c

template < class T1, class T2 >
bool operator ==(const T1  * t1, const ::c::smart_pointer < T2 > & t2)
{
   T1 * pt1 = dynamic_cast < T1 * > (t2.m_p);
   if(pt1 != ::null())
   {
      return pt1 == t1;
   }
   T2 * pt2 = dynamic_cast < T2 * > ((T1 *) t1);
   if(pt2 != ::null())
   {
      return pt2 == t2.m_p;
   }
   return false;
}

template < class T1, class T2 >
bool operator ==(const ::c::smart_pointer < T1 > & t1, const T2 * t2)
{
   T1 * pt1 = dynamic_cast < T1 * > ((T2 *) t2);
   if(pt1 != ::null())
   {
      return pt1 == t1.m_p;
   }
   T2 * pt2 = dynamic_cast < T2 * > (t1.m_p);
   if(pt2 != ::null())
   {
      return pt2 == t2;
   }
   return false;
}


template < class T1, class T2 >
bool operator ==(const ::c::smart_pointer < T1 > & t1, const ::c::smart_pointer < T2 > & t2)
{
   T1 * pt1 = dynamic_cast < T1 * > (t2.m_p);
   if(pt1 != ::null())
   {
      return pt1 == t1.m_p;
   }
   T2 * pt2 = dynamic_cast < T2 * > (t1.m_p);
   if(pt2 != ::null())
   {
      return pt2 == t2.m_p;
   }
   return false;
}

template < class T1, class T2 >
bool operator ==(T1 * t1, const ::c::smart_pointer < T2 > & t2)
{
   return operator == ((const T1 *) t1, t2);
}


template < class T1, class T2 >
bool operator ==(const ::c::smart_pointer < T1 > & t1, T2 * t2)
{
   return operator == (t1,  (const T2 *) t2);
}


template < class T1, class T2 >
bool operator !=(const T1 * t1, const ::c::smart_pointer < T2 > & t2)
{
   return !operator == (t1, t2);
}


template < class T1, class T2 >
bool operator !=(const ::c::smart_pointer < T1 > & t1, const T2 * t2)
{
   return !operator == (t1, t2);
}

template < class T1, class T2 >
bool operator !=(T1 * t1, const ::c::smart_pointer < T2 > & t2)
{
   return !operator == ((const T1 *) t1, t2);
}


template < class T1, class T2 >
bool operator !=(const ::c::smart_pointer < T1 > & t1, T2 * t2)
{
   return !operator == (t1, (const T2 *) t2);
}


template < class T1, class T2 >
bool operator !=(const ::c::smart_pointer < T1 > & t1, const ::c::smart_pointer < T2 > & t2)
{
   return !operator == (t1, t2);
}

//template < class T >
//bool operator ==(const T * p, const ::c::smart_pointer < T > & sp) { return p == sp.m_p; }
//template < class T >
//bool operator !=(const T * p, const ::c::smart_pointer < T > & sp) { return p != sp.m_p; }
template < class T >
bool operator ==(LPARAM l, const ::c::smart_pointer < T > & sp) { return ((T *) l) == sp.m_p; }
template < class T >
bool operator !=(LPARAM l, const ::c::smart_pointer < T > & sp) { return ((T *) l) == sp.m_p; }
/*template < class T >
bool operator ==(void * p, const ::c::smart_pointer < T > & sp) { return ((T *) p) == sp.m_p; }
template < class T >
bool operator !=(void * p, const ::c::smart_pointer < T > & sp) { return ((T *) p) == sp.m_p; }*/
template < class T >
bool operator ==(const ::ca::null &, const ::c::smart_pointer < T > & sp) { return ((T *) 0) == sp.m_p; }
template < class T >
bool operator !=(const ::ca::null &, const ::c::smart_pointer < T > & sp) { return ((T *) 0) == sp.m_p; }




