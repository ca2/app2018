#pragma once

#include "collection/_raw_array.h"

template < class DERIVED > class pointer;
template < class DERIVED  > class pointer_array;

// A pointer class add to its derived classes
// the ability to have pointer class features.
// One of the features is to notify the pointer
// object pointer arrays (listeners) when the
// pointer object is deleted.
template < class DERIVED > 
class pointer
{
public:
   DERIVED * m_p;
   base_array < DERIVED ** , DERIVED ** > m_deriveda;
	comparable_array < pointer_array < DERIVED > *, pointer_array < DERIVED > * > m_listenera;

	pointer();
	virtual ~pointer();

	DERIVED * derived();
   const DERIVED * derived() const;

	void pointer_clear_references();

	void add_reference(DERIVED * & pderived);
	void add_listener(pointer_array < DERIVED > * plistener);
	void remove_listener(pointer_array < DERIVED > * plistener);

   operator DERIVED * ();
   operator const DERIVED * () const;

};

template < class DERIVED >
pointer < DERIVED >::operator DERIVED * ()
{
   return dynamic_cast < DERIVED * > (this);
}

template < class DERIVED >
pointer < DERIVED >::operator const DERIVED * () const
{
   return dynamic_cast < const DERIVED * > (this);
}

template < class DERIVED >
pointer < DERIVED >::pointer()
{
   m_p            = NULL;

}

template < class DERIVED >
pointer < DERIVED >::~pointer()
{
	pointer_clear_references();
}

template < class DERIVED >
void pointer < DERIVED >::pointer_clear_references()
{
	try
	{
		int i;
		for(i = 0; i < m_deriveda.get_size() ; i++)
		{
			*m_deriveda.element_at(i) = NULL;
		}
		comparable_array < pointer_array < DERIVED > *, pointer_array < DERIVED > * > 
			listenera = m_listenera;
		for(i = 0; i < listenera.get_size() ; i++)
		{
			try
			{
			 listenera.element_at(i)->remove(this);
			}
			catch(...)
			{
			}
			try
			{
				listenera.element_at(i)->OnPointerClear(m_p);
			}
			catch(...)
			{
			}
		}
	}
	catch(...)
	{
		return;
	}
}

template < class DERIVED > 
DERIVED * pointer < DERIVED >::derived()
{
	return dynamic_cast < DERIVED * > ( this );
}

template < class DERIVED > 
const DERIVED * pointer < DERIVED >::derived() const
{
	return dynamic_cast < const DERIVED * > ( this );
}

template < class DERIVED > 
void pointer < DERIVED >::add_reference(DERIVED * & pderived)
{
	if(pderived != NULL
      && ((pointer<DERIVED> *) pderived) == this)
	{
		m_deriveda.add(&pderived);
	}
}

template < class DERIVED > 
void pointer < DERIVED >::
add_listener(pointer_array<DERIVED > * plistener)
{
	ASSERT(plistener != NULL);
	if(plistener != NULL)
	{
		m_listenera.add_unique(plistener);
	}
}

template < class DERIVED > 
void pointer < DERIVED >::
remove_listener(pointer_array<DERIVED > * plistener)
{
	ASSERT(plistener != NULL);
	if(plistener != NULL)
	{
		m_listenera.remove(plistener);
	}
}








namespace gen
{

   template < class DERIVED > 
   static void add_reference(DERIVED * & lpderived)
   {
	   pointer<DERIVED> * lppi = dynamic_cast<pointer<DERIVED> *>(lpderived);
	   ASSERT(lppi != NULL);
	   if(lppi != NULL)
	   {
		   lppi->add_reference(lpderived);
	   }
   }

} // namespace gen








template < class DERIVED >
class pointer_array :
   virtual public comparable_array < pointer < DERIVED > *, pointer < DERIVED > * >
{
public:
   pointer_array();
   pointer_array(const pointer_array & a);
   virtual ~pointer_array();
	virtual void OnPointerClear(DERIVED * p);

	virtual index add(pointer < DERIVED > * p);
   virtual void set_at(int iIndex, pointer < DERIVED > * p);
	virtual count remove(pointer < DERIVED > * p);

   virtual void remove_all();


   virtual DERIVED * derived_at(index idx);
   virtual const DERIVED * derived_at(index idx) const;


   void get_array(raw_array < DERIVED * > & a);
   void get_array(comparable_array < DERIVED * > & a);

   pointer_array & operator = (const pointer_array & a);

};


template < class DERIVED >
void pointer_array < DERIVED >::get_array(raw_array < DERIVED * > & a)
{
   for(int i = 0; i < get_count(); i++)
   {
      a.add(derived_at(i));
   }
}

template < class DERIVED >
void pointer_array < DERIVED >::get_array(comparable_array < DERIVED * > & a)
{
   for(int i = 0; i < get_count(); i++)
   {
      a.add(derived_at(i));
   }
}

template < class DERIVED > 
pointer_array < DERIVED >::
pointer_array()
{
}

template < class DERIVED > 
pointer_array < DERIVED >::
pointer_array(const pointer_array & a)
{
   operator = (a);
}

template < class DERIVED > 
pointer_array < DERIVED >::
~pointer_array()
{
   remove_all();
}

template < class DERIVED > 
void pointer_array < DERIVED >::
remove_all()
{
   for(int i = 0; i < get_size(); i++)
   {
      element_at(i)->remove_listener(this);
   }
   comparable_array < pointer < DERIVED > *, pointer < DERIVED > * >::remove_all();
}

template < class DERIVED > 
void pointer_array < DERIVED >::
OnPointerClear(DERIVED * p)
{
   remove(dynamic_cast < pointer < DERIVED > * > (p));
}

template < class DERIVED > 
index pointer_array < DERIVED >::
add(pointer < DERIVED > * p)
{
	ASSERT(p != NULL);
	if(p != NULL)
	{
      if(p->m_p == NULL)
         p->m_p = dynamic_cast < DERIVED * >(p);
      index idx = comparable_array < pointer < DERIVED > *, pointer < DERIVED > * >::add(p);
		p->add_listener(this);
      return idx;
	}
   return -1;
}

template < class DERIVED >
void pointer_array < DERIVED >::
set_at(int iIndex, pointer < DERIVED > * p)
{
	if(p != NULL)
	{
      if(p->m_p == NULL)
         p->m_p = dynamic_cast < DERIVED * >(p);
      p->add_listener(this);
      comparable_array < pointer < DERIVED > *, pointer < DERIVED > * >::set_at(iIndex, p);
   }
}


template < class DERIVED > 
count pointer_array < DERIVED >::
remove(pointer < DERIVED > * p)
{
	if(p != NULL)
	{
      count cnt = comparable_array < pointer < DERIVED > *, pointer < DERIVED > * >::remove(p);
		p->remove_listener(this);
      return cnt;
	}
   return 0;
}

template < class DERIVED > 
DERIVED * pointer_array < DERIVED >::
derived_at(index idx)
{
   return element_at(idx)->derived();
}

template < class DERIVED > 
const DERIVED * pointer_array < DERIVED >::
derived_at(index idx) const
{
   return element_at(idx)->derived();
}

template < class DERIVED > 
pointer_array < DERIVED > &
pointer_array < DERIVED >::
operator = (const pointer_array & a)
{
   if(this != &a)
   {
      set_app(a.get_app());
      for(int i = 0; i < a.get_size(); i++)
      {
         add(a.element_at(i));
      }
   }
   return *this;
}

#include "reference.h"