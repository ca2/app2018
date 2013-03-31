#pragma once

namespace user
{

   class CLASS_DECL_ca2 simple_list_data  :
      virtual public list_data
   {
   public:
      
      
      bool m_bEnable;

      ::ca::smart_pointer_array < stringa , stringa & > m_array;

      
      simple_list_data(::ca::application * papp);
      virtual ~simple_list_data();


      bool RemoveItem(index iItem);
      void get_data(stringa & stra);
      void GetColumnData(stringa & stra, index iColumn);
      void set_data(list * plist, stringa & stra);
      void SetColumnData(list * plist, stringa & stra, index iColumn);
      void Enable(bool bEnable);
      void SetItemText(list * plist, index iItem, index iSubItem, const char * lpcsz);
      

      virtual void _001GetItemText(list_item * pitem);

      virtual void _001OnDeleteRange(::user::list::range & range);


      virtual int_ptr _001GetItemCount();

   protected:
      void SetItemText(int_ptr iItem, int_ptr iSubItem, const char * lpcsz);
   };

} // namespace user