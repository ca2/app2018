#pragma once


namespace file
{


   class listing;


   class CLASS_DECL_AURA listing_provider
   {
   public:


      virtual listing & perform_file_listing(listing & listing) = 0;


   };



} // namespace file


