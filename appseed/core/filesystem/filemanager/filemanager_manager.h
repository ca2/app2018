#pragma once


namespace filemanager
{

   
   class CLASS_DECL_CORE manager :
      virtual public ::file_watcher::file_watch_listener,
      virtual public ::userfs::document
   {
   public:


      sp(::filemanager::data)             m_spfilemanagerdata;
      sp(::fs::item)                      m_item;
      ::critical_section                  m_csItemIdListAbsolute;
      ::file::path                        m_strTopic;
      bool                                m_bFullBrowse;


      manager(::aura::application * papp);
      virtual ~manager();

      virtual int64_t add_ref()
      {
         return ::object::add_ref();
      }

      virtual int64_t dec_ref()
      {
         return ::object::dec_ref();
      }

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      virtual sp(::fs::data)                       get_fs_data();
      virtual ::fs::item &                         get_filemanager_item();
      virtual sp(::filemanager::data)              get_filemanager_data();
      virtual sp(::filemanager::manager_template)  get_filemanager_template();



      DECL_GEN_SIGNAL(_001OnLevelUp);
      DECL_GEN_SIGNAL(_001OnUpdateLevelUp);
      DECL_GEN_SIGNAL(_001OnAddLocation);
      DECL_GEN_SIGNAL(_001OnUpdateAddLocation);
      DECL_GEN_SIGNAL(_001OnReplaceText);
      DECL_GEN_SIGNAL(_001OnUpdateReplaceText);
      DECL_GEN_SIGNAL(_001OnEditPaste);
      DECL_GEN_SIGNAL(_001OnUpdateEditPaste);
      DECL_GEN_SIGNAL(_001OnFileSaveAs);
      DECL_GEN_SIGNAL(_001OnUpdateFileSaveAs);


      ::critical_section * GetItemIdListCriticalSection();

      virtual bool FileManagerBrowse(sp(::fs::item) item, ::action::context actioncontext);
      virtual bool FileManagerBrowse(const char * lpcsz, ::action::context actioncontext);

      virtual void FileManagerOneLevelUp(::action::context actioncontext);
      virtual void OnFileManagerBrowse(::action::context actioncontext);
      virtual void OpenSelectionProperties();

      virtual void GetActiveViewSelection(::fs::item_array & itema);


      virtual void FileManagerSaveAs(::user::document * pdocument);
      virtual void FileManagerSaveOK();
      virtual void FileManagerSaveCancel();


      virtual void handle_file_action(::file_watcher::file_watch_id watchid, const char * dir, const char * filename, ::file_watcher::e_action action) override;


      string calc_data_key(::database::id & id);

      virtual void on_create(sp(::create) pcreatecontext);


      sp(operation_document) get_operation_doc(bool bSwitch);


      virtual bool on_simple_action(id id);
      virtual bool on_simple_update(cmd_ui * pcmdui);

      virtual bool on_new_document();

      virtual bool HandleDefaultFileManagerItemCmdMsg(::aura::cmd_msg * pcmdmsg,::fs::item_array & itema);

      void PopViews();
      void CreateViews();
      void OpenFolder(sp(::fs::item) item,::action::context actioncontext);
      void Initialize(bool bMakeVisible);

      bool set_filemanager_data(::filemanager::data * pdata);

      virtual bool on_create_bars(simple_frame_window * pframe);


      virtual void start_full_browse(string strPath, ::action::context actioncontext);
      virtual void full_browse(string strPath, ::action::context actioncontext);

   };


} // namespace filemanager




