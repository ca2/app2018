#include "framework.h"

namespace user
{

   network_configuration::network_configuration(::aura::application * papp) :
      object(papp)
   {
//      m_pdoc   = NULL;
      //    m_pview  = NULL;
   }

   network_configuration::~network_configuration()
   {
   }

   bool network_configuration::initialize(sp(::user::interaction) puiParent)
   {
      /*m_pdoc = Application.create_form(this, puiParent);
      if(m_pdoc != NULL)
      {
         m_pview = m_pdoc->get_typed_view < ::user::form > ();
         m_pview->m_pcallback = this;
         return true;
      }*/
      return false;
   }

   bool network_configuration::initialize_child(sp(::user::interaction) puiParent)
   {
      /*
      m_pdoc = Session.user()->create_child_form(this, puiParent);
      if(m_pdoc != NULL)
      {
         m_pview = m_pdoc->get_typed_view < ::user::form > ();
         m_pview->m_pcallback = this;
         return true;
      }*/
      return false;
   }

   void network_configuration::on_show()
   {
      /*
      if(!m_pdoc->on_open_document(Application.dir().matter("system/network/configuration/proxy.xhtml")))
      {
         return;
      }
      xml::node node(get_app());
      if(node.load(System.file_as_string(&System, System.dir().appdata("proxy.xml"))))
      {
         string strProxy = node.attr("server");
         int32_t iProxyPort = node.attr("port");
         sp(::user::interaction) pui = m_pview->get_child_by_name("server");
         sp(::user::primitive) ptext =  (pui);
         ptext->_001SetText(strProxy, false);
         pui = m_pview->get_child_by_name("port");
         ptext =  (pui);
         ptext->_001SetText(::str::from(iProxyPort), false);
      }*/
   }




} // namespace user
