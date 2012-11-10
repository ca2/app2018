#include "framework.h"


namespace filehandler
{


   application::application()
   {
      m_ptemplateMenu   = NULL;
      m_ppaneview       = NULL;
   }


   application::~application()
   {
   }


   void application::construct()
   {

      m_strAppName         = "default_file_handler";
      m_strBaseSupportId   = "default_file_handler";
      m_strInstallToken    = "default_file_handler";
      m_bLicense           = false;

   }

   bool application::initialize_instance()
   {


      System.factory().creatable_small < pane_view >();
      System.factory().creatable_small < document >();
      System.factory().creatable_small < frame >();
      System.factory().creatable_small < view >();

      if(!cube2::application::initialize_instance())
         return false;



      ::userbase::single_document_template* pdoctemplate = new ::userbase::single_document_template(
         this,
         "html/frame",
         System.type_info < document > (),
         System.type_info < frame > (),       // main SDI frame ::ca::window
         System.type_info < pane_view > ());
      userbase::application::add_document_template(pdoctemplate);
      m_ptemplateMenu = pdoctemplate;


      return TRUE;
   }

   int application::exit_instance()
   {

      int iExitCode = 0;

      try
      {

         iExitCode = ::cubebase::application::exit_instance();

      }
      catch(...)
      {

         iExitCode = -1;

      }

      return  iExitCode;

   }

   void application::on_request(::ca::create_context * pcreatecontext)
   {

      if(m_ppaneview == NULL)
      {
         m_ptemplateMenu->open_document_file(pcreatecontext);
      }

      if(m_ppaneview != NULL && pcreatecontext->m_spCommandLine->m_varFile.get_type() == var::type_string)
      {
         m_ppaneview->set_cur_tab_by_id("default_file_handler://" + pcreatecontext->m_spCommandLine->m_varFile.get_string());
      }

   }






} // namespace filehandler




