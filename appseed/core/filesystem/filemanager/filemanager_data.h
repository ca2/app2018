#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE data :
      virtual public ::data::data
   {
   public:


      sp(::user::object)      m_pdocumentSave;
      callback   *                        m_pfilemanager;
      sp(manager)                         m_pmanager;
      sp(manager)                         m_pmanagerMain;
      callback *                          m_pcallback;
      schema *                            m_pschema;
      bool                                m_bTransparentBackground;
      string                              m_strDISection;
      sp(::user::place_holder)            m_pholderFileList;
      sp(::user::interaction)             m_ptreeFileTreeMerge;
      bool                                m_bSetBergedgeTopicFile;

      string                              m_strToolBar;
      string                              m_strToolBarSave;

      id                                  m_id;


      int32_t                             m_iTemplate;
      int32_t                             m_iDocument;
      int32_t                             m_iIconSize;
      bool                                m_bListText;
      bool                                m_bListSelection;
      bool                                m_bFileSize;
      bool                                m_bPassBk;
      bool                                m_bIconView;


      data(sp(base_application) papp);
      virtual ~data();

      bool is_saving();

      void FileManagerBrowse(const char * lpcsz, ::action::context actioncontext);
      void FileManagerBrowse(sp(::fs::item) item, ::action::context actioncontext);
      void OnFileManagerOpenContextMenuFolder(sp(::fs::item)  item, stringa & straCommand, stringa & straCommandTitle, ::action::context actioncontext);
      void OnFileManagerOpenContextMenuFile(const ::fs::item_array & itema, ::action::context actioncontext);
      void OnFileManagerOpenContextMenu(::action::context actioncontext);
      void OnFileManagerOpenFile(const ::fs::item_array & itema, ::action::context actioncontext);
      void OnFileManagerOpenFolder(sp(::fs::item)  str, ::action::context actioncontext);

      void OnFileManagerItemUpdate(cmd_ui * pcmdui, const ::fs::item_array & itema);
      void OnFileManagerItemCommand(const char * pszId, const ::fs::item_array & itema);


   };


} // namespace filemanager




