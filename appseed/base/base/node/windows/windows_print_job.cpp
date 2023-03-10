#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"
//#include "windows.h"


namespace windows
{


   print_job::print_job(::aura::application * papp) :
      ::object(papp),
      ::job(papp),
      ::user::job(papp),
      ::user::print_job(papp),
      thread(papp)
   {

   }


   print_job::~print_job()
   {

   }


   void print_job::run()
   {

      if (!m_phprinter->is_opened())
      {
         // TODO:
         // m_printer.open(&callback_form);
         _throw(simple_exception(get_app(), "printer not opened"));
      }

      if (m_pui == NULL)
      {
         _throw(simple_exception(get_app(), "a view should collaborate"));
      }

      ::draw2d::graphics * pgraphics = m_phprinter->create_graphics();

      // xxx
      ::draw2d::dib * pdib = NULL;


      DOCINFO docinfo;
      memset(&docinfo, 0, sizeof(docinfo));
      docinfo.cbSize = sizeof(docinfo);
      pgraphics->StartDocA(&docinfo);

      int32_t iPageCount = m_pui->get_total_page_count(this);
      int32_t iPageEnd = -1;
      if (m_iPageStart < 0)
         m_iPageStart = 0;
      if (m_iPageCount >= 0)
      {
         iPageEnd = MIN(m_iPageStart + m_iPageCount - 1, iPageCount - 1);
      }
      else
      {
         iPageEnd = iPageCount - 1;
      }

      pgraphics->m_pjob = this;
      for (m_iPrintingPage = m_iPageStart; m_iPrintingPage <= iPageEnd; m_iPrintingPage++)
      {
         pgraphics->StartPage();
         if (m_bCancel)
         {
            pgraphics->AbortDoc();

            m_error.set_if_not_set();

            return;
         }
         m_pui->_001OnDraw(pgraphics);
         if (m_bCancel)
         {
            pgraphics->AbortDoc();

            m_error.set_if_not_set();

            return;

         }
         pgraphics->EndPage();
         if (m_bCancel)
         {
            pgraphics->AbortDoc();
            m_error.set_if_not_set();

            return;

         }

      }

      pgraphics->EndDoc();


      //return 0;

   }


} // namespace windows





