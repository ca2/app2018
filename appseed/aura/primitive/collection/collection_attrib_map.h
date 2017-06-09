#pragma once


//template <class KEY,class ARG_KEY,class VALUE,class ARG_VALUE,class HASH = ::comparison::hash < ARG_KEY >,class EQUALS = ::comparison::equals_type_arg_type < KEY,ARG_KEY > >
//map < KEY,ARG_KEY, VALUE,ARG_VALUE,HASH,EQUALS > & copy(map < KEY,ARG_KEY, VALUE,ARG_VALUE,HASH,EQUALS > & m,const map < KEY,ARG_KEY, VALUE,ARG_VALUE,HASH,EQUALS > & attribmap)
//{
//   if(&m != &attribmap)
//   {
//      m.remove_all();
//      type_map::m_nBlockSize = attribmap.type_map::m_nBlockSize;
//      const typename type_map::pair * ppair = attribmap.PGetFirstAssoc();
//      while(ppair != NULL)
//      {
//         m.set_at(ppair->m_element1,ppair->m_element2);
//         ppair  = attribmap.PGetNextAssoc(ppair);
//      }
//   }
//   return m;
//}


namespace lemon
{

   namespace map
   {
      
      template < class MAP, class MAP2 >
      MAP & copy(MAP & m,const MAP2 & map2)
      {

         if((void *) &m != (void *)&map2)
         {

            m.remove_all();

            m.m_nBlockSize = map2.m_nBlockSize;

            auto it = map2.begin();

            while(it != map2.end())
            {

               m.set_at(it->m_element1,it->m_element2);

               it++;

            }

         }

         return m;

      }

   } // namespace map

} // namespace lemon


template < typename TYPE,typename ARG_TYPE = const TYPE &, typename PAIR = pair < id, TYPE > >
using id_map = ::map < id,const id &, TYPE, ARG_TYPE, PAIR > ;

using id_to_id = id_map < id,const id & > ;

using id_to_index = id_map < index,index > ;


template < typename KEY, typename TYPE, typename ARG_TYPE = const TYPE &, typename PAIR = pair < KEY *, TYPE > >
using ptrmap = ::map < KEY *, KEY *, TYPE, ARG_TYPE, PAIR >;