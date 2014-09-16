#pragma once


template < class T >
void smart_pointer < T >::alloc(const allocatorsp & allocer)
{
   static class id idType = CaSys(allocer).type_info < T > ()->m_id;
   if(m_p != NULL)
      ::release(m_p);
   element * pca = CaSys(allocer).alloc(allocer->m_pauraapp, idType);
   if(pca != NULL)
   {
      m_p = dynamic_cast < T * >(pca);
      if(m_p != NULL)
      {
         ::add_ref(m_p);
      }
   }
}

template < class T >
sp(T) smart_pointer <T>::clone() const
{
   if(m_p == NULL)
      return NULL;
   if(m_p->get_app() == NULL)
      return NULL;
   return CaSys(m_p).clone(*this);
}

