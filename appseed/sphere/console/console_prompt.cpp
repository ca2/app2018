#include "framework.h"


namespace console
{


   prompt::prompt(::aura::application * papp) :
      element(papp)
   {


      ::user::single_document_template* pdoctemplate;

      pdoctemplate = new ::user::single_document_template(
         get_app(),
         "system/form",
         System.type_info < prompt_document >(),
         System.type_info < prompt_frame >(),
         System.type_info < prompt_impact >());
      
      Application.add_document_template(pdoctemplate);

      m_pdoctemplate = pdoctemplate;

      

   }


   prompt_impact & prompt::impact(bool bShow, bool bCreateMini)
   {


      /*if(m_pdoctemplate->get_document() == NULL)
      {
         
         m_pdoctemplate->open_document_file(NULL,false);

      }*/

      manual_reset_event ev(get_app());

      Application.post_thread_message(WM_APP + 3243, (bShow ? 1 : 0) | (bCreateMini ? 2 : 0), &ev);

      ev.wait();

      return *m_pdoctemplate->get_document()->get_typed_view < prompt_impact >();

   }

   int prompt::getch()
   {

      return impact().getch();

   }

   void prompt::ungetch(int c)
   {

      impact().ungetch(c);

   }

} // namespace console



















