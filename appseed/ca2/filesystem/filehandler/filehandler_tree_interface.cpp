#include "framework.h"


namespace filehandler
{


   tree_interface::tree_interface(sp(::ca2::application) papp) :
      ca2(papp),
      ::ca2::tree(papp)
   {

      ::ca2::data_container::m_spdata = new tree_data(get_app());
         
   }


   void tree_interface::update_list()
   {

   }


   tree_interface::~tree_interface()
   {

   }

   sp(tree_data) tree_interface::get_filehandler_tree_data()
   {
      return  (::ca2::data_container::m_spdata.m_p);
   }
   
   
} // namespace filehandler




