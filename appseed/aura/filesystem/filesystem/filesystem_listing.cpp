#include "framework.h"


namespace file
{


   listing::listing(listing_provider * pprovider)
   {

      m_pprovider = pprovider;

   }


   listing::~listing()
   {

   }


   listing & listing::ls()
   {

      return m_pprovider->perform_file_listing(*this);

   }




} // namespace file



CLASS_DECL_AURA bool matches_wildcard_criteria(const stringa & straCriteria, const string & strValue)
{

   if (straCriteria.is_empty())
   {

      return true;

   }

   for (auto & strCriteria : straCriteria)
   {

      if (matches_wildcard_criteria_dup(strCriteria, strValue))
      {

         return true;

      }

   }

   return false;

}


CLASS_DECL_AURA bool matches_wildcard_criteria_ci(const stringa & straCriteria, const string & strValue)
{

   if (straCriteria.is_empty())
   {

      return true;

   }

   for (auto & strCriteria : straCriteria)
   {

      if (matches_wildcard_criteria_ci_dup(strCriteria, strValue))
      {

         return true;

      }

   }

   return false;

}




CLASS_DECL_AURA string normalize_wildcard_criteria(const string & strPattern)
{

   if (strPattern.is_empty() || strPattern == "*.*")
   {

      return "*";

   }
   else
   {

      return strPattern;

   }

}
