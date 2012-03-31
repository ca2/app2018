#include "StdAfx.h"


namespace dynamic_source
{


   void script_interface::run()
   {
      
      /*
      print("<html>\n");
      print("<head>\n");
      print("</head>\n");
      print("<body>\n");
      print("<h1>Ti Amo Thomas Borregaard Sørensen de 15 de novembro de 1984!</h1>\n");
      print("<h1>Assinado Camilo Sasuke Tsumanuma de 2 de abril de 1977!</h1>\n");
      print("<h1>Demais!</h1>\n");
      print("</body>\n");
      */

   }

   void script_interface::dinit()
   {
   }

   void script_interface::dprint(const char *)
   {
   }


   gen::property & script_interface::get(const char * pszKey)
   {
      return oprop(pszKey);
   }


   bool script_interface::main_initialize()
   {

      return true;

   }

   bool script_interface::main_finalize()
   {

      return true;

   }

   ::http::file & script_interface::output_file()
   {
      return *((::http::file *) NULL);
   }

   void script_interface::on_initialize()
   {
   }
    
   void script_interface::initialize(script_interface * pinterfaceMain, script_interface * pinterfaceParent, httpd_socket * pnetnodesocket, script_manager * pmanager)
   {
      
      m_pinstanceMain      = pinterfaceMain;
      m_pinstanceParent    = pinterfaceParent;
      m_pnetnodesocket     = pnetnodesocket;
      m_pmanager           = pmanager;
      
      on_initialize();

   }

} // namespace dynamic_source



