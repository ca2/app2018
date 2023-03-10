#pragma once


namespace userfs
{

   class main_view;

   class CLASS_DECL_CORE list :
      virtual public ::user::form_list_view
   {
   public:


      stringa              m_straFileSize;
      ::file::patha        m_pathaStrictOrder;
      index                m_iSelectionSubItem;
      index                m_iNameSubItem;
      index                m_iNameSubItemText;
      index                m_iIconSubItem;



      list(::aura::application * papp);
      virtual ~list();

      virtual void _001InsertColumns();

      virtual bool _001OnUpdateItemCount(uint32_t dwFlags = 0);

      sp(::userfs::document) get_document();
      virtual list_data * get_fs_mesh_data();

      virtual COLORREF get_background_color();
      void add_item(const char * pszPath, const char * pszTitle);
      //virtual void schedule_file_size(const char * psz) = 0;
      virtual void install_message_routing(::message::sender * pinterface);

      sp(image_list) GetActionButtonImageList(index i);



      void GetSelected(::fs::item_array & itema);

      void _001OnInitializeForm(sp(::user::control) pcontrol);
      void _001OnButtonAction(sp(::user::control) pcontrol);

      virtual void _017OpenFile(const ::fs::item_array & itema, ::action::context actioncontext);
      virtual void _017OpenFolder(sp(::fs::item)  item, ::action::context actioncontext);
      virtual void _017OpenFolder(const ::userfs::list_item & item, ::action::context actioncontext);
      virtual void _017OpenSelected(bool bOpenFile, ::action::context actioncontext);
      virtual void _017OpenContextMenuSelected(::action::context actioncontext);
      virtual void _017OpenContextMenuFolder(sp(::fs::item)  item, ::action::context actioncontext);
      virtual void _017OpenContextMenuFile(const ::fs::item_array &itema, ::action::context actioncontext);
      virtual void _017OpenContextMenu(::action::context actioncontext);
      void TakeAnimationSnapshot();
      virtual void StartAnimation();
      virtual void _001OnDraw(::draw2d::graphics * pgraphics);
      void GetSelectedFilePath(stringa & array);
      virtual void _001GetItemImage(::user::mesh_item * pitem);
      virtual void _001GetItemText(::user::mesh_item * pitem);
      virtual count _001GetItemCount();

      DECL_GEN_SIGNAL(_001OnHScroll);
      DECL_GEN_SIGNAL(_001OnVScroll);
      DECL_GEN_SIGNAL(_001OnFileRename);
      DECL_GEN_SIGNAL(_001OnUpdateFileRename);
      DECL_GEN_SIGNAL(_001OnShowWindow);

      virtual void _001InitializeFormPreData();

      virtual bool query_drop(index iDisplayDrop, index iDisplayDrag);
      virtual bool do_drop(index iDisplayDrop, index iDisplayDrag);



      virtual bool _001OnClick(uint_ptr uiFlags, point point);


      virtual void on_update(::user::impact * pSender, LPARAM lHint, object* pHint);
      virtual bool pre_create_window(::user::create_struct& cs);

      DECL_GEN_SIGNAL(_001OnCreate);
      void _001OnTimer(::timer * ptimer);
      DECL_GEN_SIGNAL(_001OnLButtonDblClk);
      DECL_GEN_SIGNAL(_001OnCancelMode);


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      ::user::list_data * create_mesh_data();

   };


} // namespace userfs

