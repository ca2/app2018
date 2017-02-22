//#include "framework.h"


namespace filemanager
{


   manager_template::manager_template(::aura::application * papp) :
      object(papp)
   {

      m_iTemplate = -1;

      m_pfilelistcallback = NULL;

      m_iNextDocument = 0;

      m_pdoctemplate = NULL;
      m_pdoctemplateChild = NULL;
      m_pdoctemplateChildList = NULL;
      m_pdoctemplateFolderSelectionList = NULL;

   }

   manager_template::~manager_template()
   {

   }


   sp(::filemanager::data) manager_template::create_file_manager_data(sp(::create) pcreatecontext)
   {

      sp(::filemanager::data) pfilemanagerdata(canew(data(get_app())));

      callback * pcallback = NULL;

      if (pcreatecontext.is_set())
      {

         pcallback = pcreatecontext->oprop("filemanager::callback").cast < callback > ();

      }

      pfilemanagerdata->m_pcallback = pcallback != NULL ? pcallback : &Session.filemanager();
      pfilemanagerdata->m_iTemplate = m_iTemplate;
      pfilemanagerdata->m_iDocument = m_iNextDocument++;
      pfilemanagerdata->m_bTransparentBackground = pcreatecontext == NULL ? true : pcreatecontext->m_bTransparentBackground;
      string strId;
      strId.Format("filemanager(%d)", pfilemanagerdata->m_iDocument);
      pfilemanagerdata->m_strDISection = m_strDISection + "." + strId;
      pfilemanagerdata->m_bFileSize = true;
      pfilemanagerdata->m_pmanagertemplate = this;

      return pfilemanagerdata;

   }


   sp(manager) manager_template::open_main(::id id, sp(::create) pcreatecontext,::fs::data * pdata,::filemanager::data * pfilemanagerdata,callback * pcallback)
   {

      ::file::path pathFolder;

      if (pcreatecontext->m_spCommandLine->m_ecommand == ::command_line::command_file_open)
      {

         pathFolder = pcreatecontext->m_spCommandLine->m_varFile;

         if (Application.dir().is(pathFolder))
         {

            pathFolder.m_iDir = 1;

         }
         else
         {

            pathFolder.Empty();

         }

      }


      
      sp(manager) pdoc;

      if (id.int64() < -1 || id.int64() == m_pdoctemplateMain->get_document_count())
      {

         pcreatecontext->oprop("filemanager::template") = this;

         pcreatecontext->oprop("filemanager::data") = pfilemanagerdata;

         pcreatecontext->oprop("filemanager::callback") = pcallback;

         pdoc = m_pdoctemplateMain->open_document_file(pcreatecontext);

         if (pdoc != NULL)
         {

            bool bInitialBrowsePath = true;

            if (pathFolder.m_iDir == 1)
            {

               bInitialBrowsePath = false;

            }

            pdoc->Initialize(pcreatecontext == NULL ? true : pcreatecontext->m_bMakeVisible, bInitialBrowsePath);

         }

      }
      else if (id.int64() < m_pdoctemplateMain->get_document_count())
      {

         pdoc = m_pdoctemplateMain->get_document((index) id);

      }

      if (pdoc != NULL)
      {

         tab_view * ptabview = pdoc->get_typed_view < tab_view >();

         if (pathFolder.m_iDir == 1)
         {

            if (ptabview != NULL)
            {

               ptabview->set_cur_tab_by_id("verifile://" + pathFolder);

            }

         }
         else
         {

            if (ptabview->get_tab_count() <= 0)
            {

               ptabview->set_cur_tab_by_id("0");

            }

         }

      }

      return pdoc;

   }


   sp(manager) manager_template::open(id id, sp(::create) pcreatecontext, ::fs::data * pdata, ::filemanager::data * pfilemanagerdata, callback * pcallback)
   {

      ::file::path pathFolder;

      if (pcreatecontext->m_spCommandLine->m_ecommand == ::command_line::command_file_open)
      {

         pathFolder = pcreatecontext->m_spCommandLine->m_varFile;

         if (Application.dir().is(pathFolder))
         {

            pathFolder.m_iDir = 1;

         }
         else
         {

            pathFolder.Empty();

         }

      }



      sp(manager) pdoc;

      if (id.int64() < -1 || id.int64() == m_pdoctemplateMain->get_document_count())
      {

         pcreatecontext->oprop("filemanager::template") = this;

         pcreatecontext->oprop("filemanager::data") = pfilemanagerdata;

         pcreatecontext->oprop("filemanager::callback") = pcallback;

         pdoc = m_pdoctemplate->open_document_file(pcreatecontext);

         if (pdoc != NULL)
         {

            bool bInitialBrowsePath = true;

            if (pathFolder.m_iDir == 1)
            {

               bInitialBrowsePath = false;

            }

            pdoc->Initialize(pcreatecontext == NULL ? true : pcreatecontext->m_bMakeVisible, bInitialBrowsePath);

         }

      }
      else if (id.int64() < m_pdoctemplateMain->get_document_count())
      {

         pdoc = m_pdoctemplate->get_document((index) id);

      }

      if (pdoc != NULL)
      {

         if (pathFolder.m_iDir == 1)
         {

            pdoc->FileManagerBrowse(pathFolder, ::action::source_user);

         }

      }

      return pdoc;

   }

   sp(manager) manager_template::create_new_document(callback * pcallback, sp(::create) pcreatecontext)
   {

      sp(manager) pdoc = (m_pdoctemplate->create_new_document(pcreatecontext));

      if (pdoc != NULL)
      {

         pdoc->get_filemanager_data()->m_pcallback = pcallback;
         pdoc->get_filemanager_data()->m_pfilemanager = pcallback;
         pdoc->get_filemanager_data()->m_pmanager = pdoc;
         pdoc->get_filemanager_data()->m_pmanagerMain = pdoc;
         pdoc->get_filemanager_template() = this;
         pdoc->get_filemanager_data()->m_iTemplate = m_iTemplate;
         pdoc->get_filemanager_data()->m_iDocument = m_iNextDocument++;

         return pdoc;
      }
      return NULL;
   }


   sp(manager) manager_template::open_child(bool bMakeVisible,bool bTransparentBackground,sp(::user::interaction) pwndParent,::filemanager::data * pfilemanagerdata,callback * pcallback)
   {

      if(pfilemanagerdata == NULL)
      {

         pfilemanagerdata = canew(::filemanager::data(get_app()));

      }

      sp(::create) createcontext(pfilemanagerdata->allocer());
      
      createcontext->m_bMakeVisible = false;
      createcontext->m_puiParent = pwndParent;
      createcontext->oprop("filemanager::data") = pfilemanagerdata;

      pfilemanagerdata->m_pmanagertemplate = this;
      if(pfilemanagerdata->m_pcallback == NULL || pcallback != NULL)
      {
         pfilemanagerdata->m_pcallback = pcallback != NULL ? pcallback : &Session.filemanager();
      }
      pfilemanagerdata->m_pfilemanager = &Session.filemanager();
      pfilemanagerdata->m_iTemplate = m_iTemplate;
      pfilemanagerdata->m_iDocument = m_iNextDocument++;
      pfilemanagerdata->m_bTransparentBackground = bTransparentBackground;

      sp(manager) pdoc = (m_pdoctemplateChild->open_document_file(createcontext));

      if(pdoc == NULL)
         return NULL;

      if(pdoc->get_filemanager_data()->m_strDISection.is_empty())
      {

         string strId;
         strId.Format("%s::(%d)",Session.filemanager().m_idFileManager.str(),pdoc->get_filemanager_data()->m_iDocument);
         pdoc->get_filemanager_data()->m_strDISection = strId;

      }

      pdoc->Initialize(bMakeVisible);

      return pdoc;
      
   }


   sp(manager) manager_template::open_child_list(bool bMakeVisible,bool bTransparentBackground,sp(::user::interaction) pwndParent,::filemanager::data * pfilemanagerdata,callback * pcallback)
   {
      
      UNREFERENCED_PARAMETER(bMakeVisible);

      sp(::create) createcontext(allocer());

      createcontext->m_bMakeVisible = false;

      createcontext->m_puiParent = pwndParent;

      if(pfilemanagerdata == NULL)
      {

         pfilemanagerdata = canew(::filemanager::data(get_app()));

      }

      createcontext->oprop("filemanager::data") = pfilemanagerdata;


      pfilemanagerdata->m_pmanagertemplate = this;
      pfilemanagerdata->m_pcallback = pcallback != NULL ? pcallback : &Session.filemanager();
      pfilemanagerdata->m_pfilemanager = &Session.filemanager();
      pfilemanagerdata->m_iTemplate = m_iTemplate;
      pfilemanagerdata->m_iDocument = m_iNextDocument++;
      pfilemanagerdata->m_bTransparentBackground = bTransparentBackground;


      sp(manager) pdoc = (m_pdoctemplateChildList->open_document_file(createcontext));

      if(pdoc == NULL)
      {

         return NULL;

      }

      if(pdoc == NULL)
         return NULL;

      pdoc->get_filemanager_data()->m_pmanager = pdoc;
      pdoc->get_filemanager_data()->m_pmanagerMain = pdoc;

      if(pdoc->get_filemanager_data()->m_strDISection.is_empty())
      {

         string strId;
         strId.Format("%s::(%d)",Session.filemanager().m_idFileManager.str(),pdoc->get_filemanager_data()->m_iDocument);
         pdoc->get_filemanager_data()->m_strDISection = strId;

      }

      pdoc->Initialize(bMakeVisible);

      return pdoc;

   }


   sp(manager) manager_template::open_folder_selection_list(bool bMakeVisible,bool bTransparentBackground,sp(::user::interaction) pwndParent,::filemanager::data * pfilemanagerdata,callback * pcallback)
   {

      UNREFERENCED_PARAMETER(bMakeVisible);

      sp(::create) createcontext(allocer());

      createcontext->m_bMakeVisible = false;

      createcontext->m_puiParent = pwndParent;

      if(pfilemanagerdata == NULL)
      {

         pfilemanagerdata = canew(::filemanager::data(get_app()));

      }

      createcontext->oprop("filemanager::data") = pfilemanagerdata;


      pfilemanagerdata->m_pmanagertemplate = this;
      pfilemanagerdata->m_pcallback = pcallback != NULL ? pcallback : &Session.filemanager();
      pfilemanagerdata->m_pfilemanager = &Session.filemanager();
      pfilemanagerdata->m_iTemplate = m_iTemplate;
      pfilemanagerdata->m_iDocument = m_iNextDocument++;
      pfilemanagerdata->m_bTransparentBackground = bTransparentBackground;

      sp(manager) pdoc = (m_pdoctemplateFolderSelectionList->open_document_file(createcontext));

      if(pdoc == NULL)
         return NULL;

      pdoc->get_filemanager_data()->m_pmanager = pdoc;

      pdoc->get_filemanager_data()->m_pmanagerMain = pdoc;

      if(pdoc->get_filemanager_data()->m_strDISection.is_empty())
      {

         string strId;

         strId.Format("%s::(%d)",Session.filemanager().m_idFileManager.str(),pdoc->get_filemanager_data()->m_iDocument);

         pdoc->get_filemanager_data()->m_strDISection = strId;

      }

      pdoc->Initialize(bMakeVisible);

      return pdoc;

   }


   void manager_template::Initialize(int32_t iTemplate, const char * pszMatter)
   {

      if (m_pdoctemplateMain.is_set())
         return;
      
      m_iTemplate = iTemplate;

      m_pdoctemplateMain = canew(::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < main_frame >(),       // main SDI frame window
         System.type_info < tab_view >()));

      Application.add_document_template(m_pdoctemplateMain);

      m_pdoctemplate = canew(::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < frame >(),
         System.type_info < view >()));

      Application.add_document_template(m_pdoctemplate);

      m_pdoctemplateChild = canew(::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < child_frame >(),
         System.type_info < view >()));

      Application.add_document_template(m_pdoctemplateChild);

      m_pdoctemplateChildList = canew(::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < child_frame >(),
         System.type_info < file_list >()));

      Application.add_document_template(m_pdoctemplateChildList);

      m_pdoctemplateFolderSelectionList = canew(::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < child_frame >(),
         System.type_info < folder_selection_list_view >()));

      Application.add_document_template(m_pdoctemplateFolderSelectionList);

   }



} // namespace filemanager




