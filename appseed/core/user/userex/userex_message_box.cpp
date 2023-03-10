#include "framework.h"


namespace userex // ca8 + cube
{


   message_box::message_box(::aura::application * papp) :
      object(papp),
      html_form(papp),
      dialog(papp),
      wait_message_dialog(papp)
   {
      
   }

   
   void message_box::on_show(const char * pszMatter, property_set * ppropertyset)
   {
      
      if(!ppropertyset->has_property("application_name"))
      {
       
         ppropertyset->operator[]("application_name") = System.m_strAppName;
         
      }
      
      if(ppropertyset->has_property("simple_message_box_timeout_ms"))
      {
         
         m_dwDelay = ppropertyset->operator[]("simple_message_box_timeout_ms");
         
         if(m_dwDelay > 0)
         {
            
         }
         
      }
      
      wait_message_dialog::on_show(pszMatter, ppropertyset);
      
   }
   

   bool message_box::on_timeout()
   {
      m_strResponse = "ok";
      return wait_message_dialog::on_timeout();
   }

   void message_box::on_timer_soft_reload(uint32_t dwTimeout)
   {
      //m_pdocument->get_html_data()->m_propertyset["simple_message_box_timeout"] = (int32_t) ((m_pdocument->get_html_data()->m_propertyset["simple_message_box_timeout_ms"]) / 1000);
      wait_message_dialog::on_timer_soft_reload(dwTimeout);
   }


} // namespace userex // ca8 + cube



namespace core
{

   string application::message_box(const char * pszMatter, property_set & propertyset)
   {
      ::userex::message_box box(this);
      box.show(pszMatter, &propertyset);
      return box.m_strResponse;
   }


} // namespace core
