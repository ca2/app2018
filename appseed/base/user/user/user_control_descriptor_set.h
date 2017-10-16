#pragma once


namespace user
{


   class CLASS_DECL_AURA control_descriptor_set :
      public smart_pointer_array < control_descriptor >
   {
      public:


         control_descriptor_set();
         virtual ~control_descriptor_set();


         sp(control) get_control(::user::form_window * pform, id id, int iItem = 0);
         control_descriptor * get_by_sub_item(int32_t iSubItem);

         bool find_control(::user::interaction * pui, index & iItem, index & iSubItem);


   };


} // namespace user


