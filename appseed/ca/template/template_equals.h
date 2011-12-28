#pragma once


template < class TYPE, class ARG_TYPE >
inline bool EqualsCompareElements(const TYPE * pElement1, const ARG_TYPE element2)
{
   return *pElement1 == element2;
}


namespace _template
{

   template < class TYPE, class ARG_TYPE >
   class equals_type_arg_type
   {
   public:

      inline static bool CompareElements(const TYPE * pElement1, const ARG_TYPE element2)
      {
         return ::EqualsCompareElements < TYPE, ARG_TYPE > (pElement1, element2);
      }


   };

   template < >
   class equals_type_arg_type < string, const string & >
   {
   public:

      inline static bool CompareElements(const string * pElement1, const string & element2)
      {
         return strcmp(*pElement1, element2) == 0;
      }

   };


   class strid_equals
   {
   public:

      inline static bool CompareElements(const id * pElement1, const id & element2)
      {
         return pElement1->m_psz == element2.m_psz;
      }

   };


} // namespace _template



