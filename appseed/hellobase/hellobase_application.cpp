#include "framework.h"


namespace hellobase
{


   application::application()
   {

      m_ptemplateHelloBaseMain = NULL;

      m_strAppName = "app/hellobase";
      m_strLibraryName = "app/hellobase";
      m_strBaseSupportId = "ca2_flag";
      m_bLicense = false;

      m_etype = type_normal;

      m_strHelloBaseDefault = "Hello Base!!";
      m_strAlternateHelloBaseDefault = "Hi Base!!";


   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

      System.factory().creatable_small < ::hellobase::document >();
      System.factory().creatable_small < ::hellobase::main_frame >();
      System.factory().creatable_small < ::user::button_view >();
      System.factory().creatable_small < ::hellobase::view >();

      if (!::base::application::init_instance())
      {

         return false;

      }

      if (!::helloaxis::application::init_instance())
      {

         return false;

      }

      string str = handler()->m_varTopicQuery["hellobase"];

      if (str.has_char())
      {

         m_strHelloBase = str;

      }

      set_local_data_key_modifier();

      ::user::single_document_template* pDocTemplate;


      pDocTemplate = canew(::user::single_document_template(
                           this,
                           "main",
                           System.type_info < document >(),
                           System.type_info < main_frame >(),
                           System.type_info < view >()));
      m_ptemplateHelloBaseMain = pDocTemplate;


      return true;

   }


   int32_t application::exit_application()
   {

      ::helloaxis::application::exit_application();

      return ::base::application::exit_application();

   }


   void application::on_request(::create * pcreate)
   {

#if 0

#ifdef DEBUG

      output_debug_string("DEBUG build? (basis)");

      ASSERT(FALSE);

#else

      output_debug_string("RELEASE build? (stage)");

      ASSERT(FALSE);

#endif

#endif

      if (m_ptemplateHelloBaseMain->get_document_count() == 0)
      {

         m_ptemplateHelloBaseMain->request_create(pcreate);

      }


      output_debug_string("\nfinished hellobase::on_request");

   }


   string application::preferred_userschema()
   {

      return "core";

   }


   string application::get_helloaura()
   {

      return "Hello Base!!";

   }


} // namespace hellobase




extern "C"
::aura::library * app_hellobase_get_new_library(::aura::application * papp)
{

   return new ::aura::single_application_library < ::hellobase::application >(papp, "app/hellobase");

}




