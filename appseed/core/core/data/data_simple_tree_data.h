#pragma once


namespace data
{


   class CLASS_DECL_CORE simple_tree_data :
      virtual public tree_data
   {
   public:


      simple_tree_data(sp(base_application) papp);


      virtual sp(tree_item_data) on_allocate_item();
      virtual void on_delete_item(sp(tree_item_data) pitem);

   };



} // namespace data



