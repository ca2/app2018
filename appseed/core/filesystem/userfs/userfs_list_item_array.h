#pragma once


namespace userfs
{


   class CLASS_DECL_CORE list_item_array :
      virtual object
   {
   public:

      class CLASS_DECL_CORE base_list_item_array :
         public spa(list_item)
      {
      public:
         list_item & get_item(index i, index (* fCompare)(const sp(list_item) &, const sp(list_item) &));
         void SoftSwap(index i1, index i2);
      };


      base_list_item_array m_itema;
      index (* m_pfnCompare)(const sp(list_item) &, const sp(list_item) &);

   public:


      list_item_array(::aura::application * papp);
      virtual ~list_item_array();


      void list_item_array::add_item(list_item & item)
      {
         m_itema.add(canew(list_item(item, this->get_size())));
      }
      ::count get_count();
      ::count get_size();
      index get_upper_bound(::count i = -1);
      list_item & get_item(index i);
      //void SetItemAt(index i, list_item & item);
      //void SetItemCount(::count iCount);
      index find_item_by_path(const ::file::path & pszPath);


      void arrange(::fs::e_arrange earrange);


   };


} // namespace userfs




