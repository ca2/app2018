#pragma once


namespace databaseuser
{


   class CLASS_DECL_ca2 data_key_list_data :
      public ::user::list_data,
      public ::database::client
   {
   public:


      data_key_list_data(::ca::application * papp);
      virtual ~data_key_list_data(void);


      virtual void _001GetItemText(::user::list_item * pitem);

      virtual count _001GetItemCount();

      bool add_unique(const stringa & stra);
      bool remove(const stringa & stra);

      void GetSel(::user::list * plist , stringa & stra);


   private:


      data_key_list_data(const data_key_list_data & ) :
         ::user::list_data(NULL)
      {
      }


   };


} // namespace databaseuser



