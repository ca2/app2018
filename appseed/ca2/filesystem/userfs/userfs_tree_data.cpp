#include "framework.h"


namespace userfs
{


   tree_data::tree_data(sp(::ca::application) papp) :
      ca(papp),
      ::ca::tree_data(papp)
   {
   }


   sp(::ca::tree_item_data) tree_data::on_allocate_item()
   {
      return canew(::userfs::tree_item_data());
   }


} // namespace userfs


