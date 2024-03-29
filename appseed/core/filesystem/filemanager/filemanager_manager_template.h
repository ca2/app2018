﻿#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE manager_template :
      virtual public ::object
   {
   public:


      int32_t                    m_iTemplate;
      int32_t                    m_iNextDocument;

      ::file::path               m_pathDefault;
      string                     m_strMenuBar;
      property_set               m_setToolbar;
      string                     m_strDialogBar;
      string                     m_strPopup;
      string                     m_strFilePopup;
      string                     m_strFilePopupSubstId;

      string                     m_strFolderPopup;
      ::database::key            m_datakey;

      id                         m_idExpandBox;
      id                         m_idCollapseBox;

      ::database::key            m_dataidStatic;


      file_list_callback *       m_pfilelistcallback;

      string                     m_strLevelUp;


      sp(::user::multiple_document_template)    m_pdoctemplateMain;
      sp(::user::multiple_document_template)    m_pdoctemplate;
      sp(::user::multiple_document_template)    m_pdoctemplateChild;
      sp(::user::multiple_document_template)    m_pdoctemplateChildList;
      sp(::user::multiple_document_template)    m_pdoctemplateFolderSelectionList;
      ::file::path                              m_pathFilemanagerProject;
      bool                                      m_bRestoring;



      manager_template(::aura::application * papp);
      virtual ~manager_template();

      void load_filemanager_project(const ::file::path & pathFileManagerProject, ::create * pcreate = NULL, ::fs::data * pdata = NULL, data * pfilemanagerdata = NULL, callback * pcallback = NULL);

      void save_filemanager_project();

      sp(manager) find_manager(var varFile);

      sp(manager) restore_manager(var varFile, ::create * pcreate = NULL, ::fs::data * pdata = NULL, data * pfilemanagerdata = NULL, callback * pcallback = NULL);

      sp(manager) open_manager(var varFile, ::create * pcreate = NULL, ::fs::data * pdata = NULL, data * pfilemanagerdata = NULL, callback * pcallback = NULL);

      sp(manager) add_manager(const ::file::path & pathFolder, ::create * pcreate = NULL, ::fs::data * pdata = NULL, data * pfilemanagerdata = NULL, callback * pcallback = NULL);

      bool remove_manager(var varFile);

      bool remove_manager(manager * pmanager);


      sp(manager) open_main(::aura::application * pappOnBehalfOf, ::id id = -1, ::create * pcreate = NULL,::fs::data * pdata = NULL,data * pfilemanagerdata = NULL,callback * pcallback = NULL);
      sp(manager) open(::aura::application * pappOnBehalfOf, ::id id = -1, ::create * pcreate = NULL, ::fs::data * pdata = NULL, data * pfilemanagerdata = NULL, callback * pcallback = NULL);
      sp(manager) open_child(bool bMakeVisible = true,bool bTransparentBackground = false,sp(::user::interaction) pwndParent = NULL,data * pfilemanagerdata = NULL, callback * pcallback = NULL);
      sp(manager) open_child_list(bool bMakeVisible = true,bool bTransparentBackground = false,sp(::user::interaction) pwndParent = NULL,data * pfilemanagerdata = NULL,callback * pcallback = NULL);
      sp(manager) open_folder_selection_list(bool bMakeVisible = true,bool bTransparentBackground = false,sp(::user::interaction) pwndParent = NULL,data * pfilemanagerdata = NULL,callback * pcallback = NULL);


      sp(manager) create_new_document(callback * pinterface, ::create * pcreate);


      sp(::filemanager::data) create_file_manager_data(::create * pcreate = NULL);

      virtual void Initialize(int32_t iTemplate, const char * pszMatter);


   };




} // namespace filemanager




