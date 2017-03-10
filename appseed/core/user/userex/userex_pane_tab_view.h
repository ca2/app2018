#pragma once


namespace userex
{


   class pane_view;

   class CLASS_DECL_CORE pane_tab_view :
      virtual public ::user::tab_view
   {
   public:


      sp(pane_view)                 m_ppaneview;
      ::object *                    m_pfilemanager;
      ::object *                    m_pfilemanagerTabbed;


      pane_tab_view(::aura::application * papp);
      virtual ~pane_tab_view();


      virtual ::user::interaction * get_view_uie();

      virtual id get_view_id();

      virtual void on_show_view();

      virtual bool on_prepare_view_creator_data(::user::view_creator_data * pcreatordata);

      virtual bool on_after_create_view_creator_data(::user::view_creator_data * pcreatordata);

      virtual void GetTabClientRect(LPRECT lprect);

      virtual bool on_hold(::user::interaction * pui, ::user::place_holder * pholder);

      virtual ::index create_tab_by_id(id id);

      virtual void on_create_view(::user::view_creator_data * pcreatordata);

      virtual ::filemanager::manager & filemanager_manager();

      virtual ::filemanager::manager & tabbed_filemanager_manager();

      virtual void FileManagerSaveAs(::user::document * pdocument);

      virtual void TabbedFileManagerSaveAs(::user::document * pdocument);

      virtual void _001OnTabClose(int32_t iTab);

      virtual string get_cred(const string & strRequestUrl, const RECT & rect, string & strUsername, string & strPassword, string strToken, string strTitle, bool bInteractive);

   };


} // namespace userex