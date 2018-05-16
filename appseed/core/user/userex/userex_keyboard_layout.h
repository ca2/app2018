#pragma once


namespace userex
{


   class CLASS_DECL_CORE keyboard_layout :
      virtual public ::user::split_view,
      virtual public ::user::form_callback,
      public ::user::list_data
   {
   public:



      sp(::user::document)                m_pdoc;
      sp(::user::form_view)               m_pview;
      sp(simple_list_view)                m_plistview;
      ::user::keyboard_layout_ida         m_layoutida;


      keyboard_layout(::aura::application * papp);
      virtual ~keyboard_layout();

      bool CreateViews();

      void on_show();

      void on_control_event(::user::control_event * pevent);

      virtual void _001GetItemText(::user::mesh_item * pitem);

      virtual int_ptr _001GetItemCount();


   private:


      keyboard_layout(const keyboard_layout & ) :
         ::user::place_holder_container(NULL),
         ::user::split_layout(NULL),
         ::user::split_view(NULL),
         ::user::mesh_data(NULL),
         ::user::list_data(NULL)
      {
      }


   };


} // namespace core



