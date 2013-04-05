#pragma once


namespace filemanager
{


   class document;


} // namespace filemanager


namespace userex
{

   class pane_view;

   class CLASS_DECL_ca2 pane_tab_view :
      virtual public ::userbase::tab_view,
      virtual public ::user::view_creator
   {
   public:


      pane_view * m_ppaneview;


      pane_tab_view(::ca::applicationsp papp);
      virtual ~pane_tab_view();


      virtual sp(::user::interaction) get_view_uie();

      virtual id get_view_id();

      virtual void on_show_view();

      virtual void on_new_view_creator_data(::user::view_creator_data * pcreatordata);

      virtual void GetTabClientRect(LPRECT lprect);

      virtual bool on_hold(sp(::user::interaction) pui, sp(::user::place_holder) pholder);

      virtual ::user::view_creator_data * ensure(id id);

      virtual void on_create_view(::user::view_creator_data * pcreatordata);

      virtual sp(::filemanager::document) get_filemanager_document();

      virtual sp(::filemanager::document) get_tabbed_filemanager_document();

      virtual void _001OnTabClose(int32_t iTab);

   };


} // namespace userex