#pragma once


template < class TYPE,class ARG_TYPE >
inline int_ptr CompareElements(const TYPE * pElement1,const ARG_TYPE element2)
{

   return *pElement1 - element2;

}

namespace comparison
{


   template < class TYPE,class ARG_TYPE >
   class compare_type_arg_type
   {
   public:


      inline static int_ptr CompareElements(const TYPE * pElement1,const ARG_TYPE element2)
      {

         return ::CompareElements < TYPE,ARG_TYPE >(pElement1,element2);

      }


   };



} // namespace comparison




namespace comparison
{




   class compare_string_case:
      public compare_type_arg_type < string,const string & >
   {
   public:


      inline static int_ptr CompareElements(const string * pElement1,const string  & element2)
      {

         return pElement1->Compare(element2);

      }


   };


   class compare_string_no_case:
      public compare_type_arg_type < string,const string & >
   {
   public:

      inline static int_ptr CompareElements(const string * pElement1,const string & element2)
      {
         return pElement1->CompareNoCase(element2);
      }

   };




} // namespace comparison









