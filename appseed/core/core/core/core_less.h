#pragma once


namespace core
{


   template < typename TYPE >
   class less
   {
   public:
      
      
      inline static bool CompareElements(const TYPE * pIsFirst, const TYPE * pCompare)
      {
         
         return *pIsFirst < *pCompare;

      }


   };



} // namespace core


