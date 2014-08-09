#include "framework.h"


namespace filemanager
{


   manager_template::manager_template(sp(::base::application) papp) :
      element(papp)
   {

      m_iTemplate = -1;

      m_pfilelistcallback = NULL;

      m_iNextDocument = 0;

      m_pdoctemplate = NULL;
      m_pdoctemplateChild = NULL;
      m_pdoctemplateChildList = NULL;

   }

   manager_template::~manager_template()
   {
      if (m_pdoctemplate != NULL)
      {
         m_pdoctemplate.release();
      }
   }

   sp(manager) manager_template::open(sp(::create_context) pcreatecontext, ::fs::data * pdata, ::filemanager::data * pfilemanagerdataParam)
   {
      
      sp(::filemanager::data) pfilemanagerdata(pfilemanagerdataParam);

      if (pcreatecontext.is_null())
         pcreatecontext.alloc(allocer());

      if (pfilemanagerdata.is_null())
      {

         pfilemanagerdata = canew(::filemanager::data(get_app()));
         pfilemanagerdata->m_pcallback = &platform().filemanager();
         pfilemanagerdata->m_iTemplate = m_iTemplate;
         pfilemanagerdata->m_iDocument = m_iNextDocument++;
         pfilemanagerdata->m_bTransparentBackground = pcreatecontext == NULL ? true : pcreatecontext->m_bTransparentBackground;
         string strId;
         strId.Format("filemanager(%d)", pfilemanagerdata->m_iDocument);
         pfilemanagerdata->m_strDISection = m_strDISection + "." + strId;
         pfilemanagerdata->m_bFileSize = true;


      }

      pfilemanagerdata->m_pmanagertemplate = this;

      pcreatecontext->oprop("filemanager::data") = pfilemanagerdata;

      sp(manager) pdoc = (m_pdoctemplateMain->open_document_file(pcreatecontext));

      if (pdoc != NULL)
      {

         pdoc->Initialize(pcreatecontext == NULL ? true : pcreatecontext->m_bMakeVisible);

         return pdoc;

      }

      return NULL;

   }

   sp(manager) manager_template::create_new_document(callback * pcallback, sp(::create_context) pcreatecontext)
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


   sp(manager) manager_template::open_child(bool bMakeVisible, bool bTransparentBackground, sp(::user::interaction) pwndParent, ::filemanager::data * pfilemanagerdata)
   {

      if(pfilemanagerdata == NULL)
      {

         pfilemanagerdata = new ::filemanager::data(get_app());

      }

      sp(::create_context) createcontext(allocer());
      
      createcontext->m_bMakeVisible = false;
      createcontext->m_puiParent = pwndParent;
      createcontext->oprop("filemanager::data") = pfilemanagerdata;

      pfilemanagerdata->m_pmanagertemplate = this;
      pfilemanagerdata->m_pcallback = &platform().filemanager();
      pfilemanagerdata->m_pfilemanager = &platform().filemanager();
      pfilemanagerdata->m_iTemplate = m_iTemplate;
      pfilemanagerdata->m_iDocument = m_iNextDocument++;
      pfilemanagerdata->m_bTransparentBackground = bTransparentBackground;

      sp(manager) pdoc = (m_pdoctemplateChild->open_document_file(createcontext));

      if(pdoc == NULL)
         return NULL;

      if(pdoc->get_filemanager_data()->m_strDISection.is_empty())
      {

         string strId;
         strId.Format("%s::(%d)",platform().filemanager().m_idFileManager.str(),pdoc->get_filemanager_data()->m_iDocument);
         pdoc->get_filemanager_data()->m_strDISection = strId;

      }

      pdoc->Initialize(bMakeVisible);

      return pdoc;
      
   }


   sp(manager) manager_template::open_child_list(bool bMakeVisible, bool bTransparentBackground, sp(::user::interaction) pwndParent, ::filemanager::data * pfilemanagerdata)
   {
      UNREFERENCED_PARAMETER(bMakeVisible);
      sp(::create_context) createcontext(allocer());
      createcontext->m_bMakeVisible = false;
      createcontext->m_puiParent = pwndParent;
      sp(manager) pdoc = (m_pdoctemplateChildList->open_document_file(createcontext));
      if (pdoc != NULL)
      {
         //      pdoc->get_filemanager_data()->m_uiMenuBar = m_uiMenuBar;
         //      pdoc->get_filemanager_data()->m_uiToolBar = m_uiToolBar;
         if (pfilemanagerdata == NULL)
         {
            pfilemanagerdata = new ::filemanager::data(get_app());
         }
         pdoc->set_filemanager_data(pfilemanagerdata);
         pdoc->get_filemanager_data()->m_pcallback = &platform().filemanager();
         pdoc->get_filemanager_data()->m_pfilemanager = &platform().filemanager();
         pdoc->get_filemanager_data()->m_pmanager = pdoc;
         pdoc->get_filemanager_data()->m_pmanagerMain = pdoc;
         pdoc->get_filemanager_template() = this;
         pdoc->get_filemanager_data()->m_iTemplate = m_iTemplate;
         pdoc->get_filemanager_data()->m_iDocument = m_iNextDocument++;
         pdoc->get_filemanager_data()->m_bTransparentBackground = bTransparentBackground;

         //pdoc->CreateViews();

         return pdoc;
      }
      return NULL;
   }

   void manager_template::Initialize(int32_t iTemplate, const char * pszMatter)
   {
      m_iTemplate = iTemplate;
      m_pdoctemplateMain = new ::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < main_frame >(),       // main SDI frame window
         System.type_info < tab_view >());

      m_pdoctemplate = new ::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < frame >(),
         System.type_info < view >());

      m_pdoctemplateChild = new ::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < child_frame >(),
         System.type_info < view >());

      m_pdoctemplateChildList = new ::user::multiple_document_template(
         get_app(),
         pszMatter,
         System.type_info < manager >(),
         System.type_info < child_frame >(),
         System.type_info < file_list >());
   }



} // namespace filemanager




