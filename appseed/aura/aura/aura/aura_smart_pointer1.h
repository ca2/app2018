#pragma once




template < class root_derived >
inline int64_t add_ref(root_derived * pca);

template < class root_derived >
inline int64_t release(root_derived * & pca);

// ::ca::null_class back link to operational system oswindow.h
//
//
//
//
// operational system NULL/void itself to a windowing service
//
// Curitiba, inha-metro-win-ubuntu-mountain-lion-macos 4 de novembro de 2012
template < class T >
class smart_pointer
{
public:



   T * m_p;


   smart_pointer();
   smart_pointer(const lparam & lparam);
   smart_pointer(const smart_pointer < T > & t);
#ifdef MOVE_SEMANTICS
   smart_pointer(smart_pointer < T > && t);
#endif
   smart_pointer(const ::aura::allocatorsp & t);
#ifdef MOVE_SEMANTICS
   smart_pointer(::aura::allocatorsp && t);
#endif
   template < class T2 >
   smart_pointer(T2 * p)
   {
      if(p == NULL)
      {
         m_p = NULL;
         return;
      }
      m_p = dynamic_cast < T * > (p);
      if(m_p != NULL) ::add_ref(p);
   }
   smart_pointer(T * p);

   template < class T2 >
   smart_pointer(const T2 * p)
   {
      if(p == NULL)
      {
         m_p = NULL;
         return;
      }
      m_p = dynamic_cast < T * > ((T2 *) p);
      if(m_p != NULL) ::add_ref(p);
   }

   template < class T2 >
   smart_pointer(const smart_pointer < T2 > & t)
   {
      m_p = NULL;
      operator = (dynamic_cast < T * >(t.m_p));
   }


#ifdef MOVE_SEMANTICS
   template < class T2 >
   smart_pointer(smart_pointer < T2 > && t)
   {
      if(t.m_p != NULL)
      {
         m_p = dynamic_cast < T * >(t.m_p);
         if(m_p != NULL)
         {
            t.m_p = NULL;
         }
      }
      else
      {
         m_p = NULL;
      }
   }
#endif

   ~smart_pointer();


   inline T * operator ->();
   inline T * operator ->() const ;
   inline T & operator *();
   inline T & operator *() const ;
   inline operator T * ();
   inline operator T * const () const;
   inline T * get_();
   inline const T * get_() const;
   inline operator lparam ();
   inline operator lparam () const;
   //inline operator void * ();
   //inline operator void * () const;

   inline bool is_null() const;
   inline bool is_set() const;

   inline smart_pointer & operator = (const smart_pointer < T > & t);
#ifdef MOVE_SEMANTICS
   inline smart_pointer & operator = (smart_pointer < T > && t);
#endif
   inline smart_pointer & operator = (const lparam & lparam);

   template < class T2 >
   inline smart_pointer & operator = (T2 * p)
   {
      return operator = (dynamic_cast < T * > (p));
   }


   inline smart_pointer & operator = (T * p)
   {
      return reset(p);
   }

   template < class T2 >
   inline smart_pointer & operator = (const smart_pointer < T2 > & t)
   {
      return operator = (dynamic_cast < T * > (t.m_p));
   }

#ifdef MOVE_SEMANTICS
   template < class T2 >
   inline smart_pointer & operator = (smart_pointer < T2 > && t)
   {
      T * pOld = m_p;
      if(t.m_p != NULL)
      {
         m_p = dynamic_cast < T * >(t.m_p);
         if(m_p != NULL)
         {
            t.m_p = NULL;
         }
      }
      else
      {
         m_p = NULL;
      }
      if(pOld != NULL)
      {
         ::release(pOld);
      }
      return *this;
   }
#endif

   void release();
   inline T * detach();


   template < class T2 >
   inline T2 * cast()
   {
      return dynamic_cast < T2 * > (m_p);
   }

   template < class T2 >
   inline T2 * cast() const
   {
      return dynamic_cast < T2 * > (m_p);
   }

   template < class T2 >
   inline bool is() const
   {
      return cast < T2 > () != NULL;
   }

   inline smart_pointer & reset(T * p);

//      bool operator ==(const smart_pointer & p) const { return m_p == p.m_p; }
//      bool operator !=(const smart_pointer & p) const { return m_p != p.m_p; }
//      bool operator ==(const T * p) const { return m_p == p; }
//      bool operator !=(const T * p) const { return m_p != p; }
   template < typename T2 >
   inline bool operator ==(const T2 * p) const { return m_p == p; }
   template < typename T2 >
   inline bool operator !=(const T2 * p) const { return m_p != p; }
   inline bool operator ==(const T * p) const { return m_p == p; }
   inline bool operator !=(const T * p) const { return m_p != p; }
   //bool operator ==(void * p) const { return m_p == (T *) p; }
   //bool operator !=(void * p) const { return m_p != (T *) p; }



   void oattrib(const sp(T) & o);
   void copy(T * po) { return oattrib(po); }


   //sp(T) clone() const;

   void alloc(const ::aura::allocatorsp & spallocator);


   inline void run_and_release()
   {

      if (is_null())
      {

         return;

      }

      m_p->run();

      release();

   }

};

// It must not free memory directly allocated to object pointed by 'p'.
// It is recommended to let final deletion and destruction happens at normal destructor.
// 'destruct' semantics gives a class the ability to use the scoped guard_pointer
// to release outer references (from operating system for example) that would prevent
// the object to be deleted/destroyed when the object is released by a conventional smart_pointer.
template < class T >
void destruct(T * p)
{

   try
   {

      p->destruct();

   }
   catch(...)
   {

   }

}


template < class T >
class guard_pointer :
   public ::smart_pointer < T >
{
public:
   guard_pointer() {}
   guard_pointer(const lparam & lparam) : ::smart_pointer<T>(lparam) {}
   guard_pointer(const smart_pointer < T > & t): ::smart_pointer< T>(t) {}
   guard_pointer(guard_pointer < T > && t): ::smart_pointer<T>(::move(t)) {}
   guard_pointer(const ::aura::allocatorsp & t): ::smart_pointer<T>(t) {}
   guard_pointer(::aura::allocatorsp && t): ::smart_pointer<T>(::move(t)) {}
   template < class T2 >
   guard_pointer(T2 * p) : ::smart_pointer <T>(p) {}
   guard_pointer(T * p) : ::smart_pointer <T>(p) {}

   template < class T2 >
   guard_pointer(const T2 * p): ::smart_pointer <T>(p) {}

   template < class T2 >
   guard_pointer(const smart_pointer < T2 > & t): ::smart_pointer <T>(t) {}


   template < class T2 >
   guard_pointer(guard_pointer < T2 > && t) :
      ::smart_pointer <T>(::move(t)) {}

   ~guard_pointer()
   {

      if(::is_set(this->m_p))
      {

         ::destruct(this->m_p);

      }

      this->release();

   }


};



