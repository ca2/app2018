﻿#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE data :
      virtual public ::data::data,
      virtual public ::user::style
   {
      public:


         ::draw2d::font_sp                   m_spfont;
         ::user::document *                  m_pdocumentTopic;
         callback   *                        m_pfilemanager;
         manager *                           m_pmanager;
         manager *                           m_pmanagerMain;
         callback *                          m_pcallback;
         ::filemanager::manager_template *   m_pmanagertemplate;
         bool                                m_bTransparentBackground;
         string                              m_strDataKeyModifier;
         ::user::place_holder *              m_pholderFileList;
         ::user::tree *                      m_ptreeFileTreeMerge;
         bool                                m_bSetBergedgeTopicFile;

         property_set                        m_setToolbar;

         bool                                m_bEnableRecursiveFolderSelectionList;

         id                                  m_id;


         int32_t                             m_iTemplate;
         int32_t                             m_iDocument;
         int32_t                             m_iIconSize;
         bool                                m_bListText;
         bool                                m_bListSelection;
         bool                                m_bFileSize;
         bool                                m_bPassBk;
         bool                                m_bIconView;


         data(::aura::application * papp);
         virtual ~data();

         bool is_topic();

         void FileManagerBrowse(const char * lpcsz, ::action::context actioncontext);
         void FileManagerBrowse(sp(::fs::item) item, ::action::context actioncontext);
         void OnFileManagerOpenContextMenuFolder(sp(::fs::item)  item, stringa & straCommand, stringa & straCommandTitle, ::action::context actioncontext);
         void OnFileManagerOpenContextMenuFile(const ::fs::item_array & itema, ::action::context actioncontext);
         void OnFileManagerOpenContextMenu(::action::context actioncontext);
         void OnFileManagerOpenFile(const ::fs::item_array & itema, ::action::context actioncontext);
         void OnFileManagerOpenFolder(sp(::fs::item)  str, ::action::context actioncontext);

         void OnFileManagerItemUpdate(::user::command * pcommand, const ::fs::item_array & itema);
         void OnFileManagerItemCommand(const char * pszId, const ::fs::item_array & itema);

         virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pui) override;
         virtual bool get_font(::draw2d::font_sp & font, ::user::e_font efont, ::user::interaction * pui) override;
         virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pui) override;


         virtual sp(::filemanager::manager_template) get_filemanager_template();

   };


} // namespace filemanager




