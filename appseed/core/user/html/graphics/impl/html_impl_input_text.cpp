#include "framework.h"


namespace html
{


   namespace impl
   {


      input_text::input_text(data * pdata, const string & strType, const string & strUnit)
      {

         m_iFont = -1;
         m_strType = strType;
         m_strUnit = strUnit;
         if(strType.compare_ci("calculator") == 0)
         {
            m_pedit = canew(::calculator::plain_edit_view(pdata->get_app()));
            m_pedit.cast <::calculator::plain_edit_view>()->m_strFormat = strUnit;
            m_pedit.cast <::calculator::plain_edit_view>()->m_pcallback = this;
         }
         else
         {
            m_pedit = canew(::user::plain_edit(pdata->get_app()));
         }

         m_pedit->m_bMultiLine      = false;

         m_pedit->m_pform = pdata->m_pform;

      }


      input_text::~input_text()
      {

         try
         {

            if(m_pedit->IsWindow())
            {

               m_pedit->DestroyWindow();

            }

         }
         catch(...)
         {

         }

      }


      void input_text::implement_phase1(data * pdata, ::html::elemental * pelemental)
      {

         m_bHasChar = true;

         elemental::implement_phase1(pdata, pelemental);
         if(!m_pedit->IsWindow())
         {
            m_pedit->create_window(null_rect(),pdata->m_pui,2000 + pdata->m_uiptra.get_count());
            pdata->on_create_interaction(m_pedit);
            m_pedit->m_bPassword = pelemental->m_propertyset["type"].compare_value_ci("password") == 0;
            m_pedit->m_strName = pelemental->m_pbase->get_tag()->get_attr_value("name");
            m_pedit->m_id = pelemental->m_pbase->get_tag()->get_attr_value("id");
            m_pedit->_001SetText(pelemental->m_pbase->get_tag()->get_attr_value("value"), ::action::source::add(::action::source_data, ::action::source_load));
            m_pedit->m_puserstyle = pelemental;


         }
         pdata->m_focusptra.add_unique(m_pedit);
         m_cxMax = 200;
         m_cxMin = 200;
      }


      bool input_text::layout_phase1(data * pdata)
      {

         string strSize = m_pelemental->m_pbase->get_tag()->get_attr_value("size");

         int iSize = 20;

         if (strSize.has_char())
            iSize = MAX(1, atoi(strSize));

         m_box.set_cxy(iSize * 10.f, 23.f);

         return true;

      }


      void input_text::layout_phase3(data * pdata)
      {

         elemental::layout_phase3(pdata);

         m_pedit->SetWindowPos(0, (int32_t) m_box.left, (int32_t) m_box.top, (int32_t) m_box.get_cx(), (int32_t) m_box.get_cy(), SWP_NOREDRAW);

      }


      void input_text::_001OnDraw(data * pdata)
      {

         ::draw2d::graphics * pgraphics = pdata->m_pgraphics;

         rect rectWindow;

         m_pedit->GetWindowRect(rectWindow);

         m_pedit->get_wnd()->ScreenToClient(rectWindow);

         ::draw2d::savedc savedc(pgraphics);

         pgraphics->SetViewportOrg(rectWindow.top_left());

         m_pedit->_000OnDraw(pgraphics);

      }


      void input_text::on_change_layout(data * pdata)
      {

         UNREFERENCED_PARAMETER(pdata);

         m_pedit->SetWindowPos(0, (int32_t) m_box.left, (int32_t) m_box.top, (int32_t) m_box.get_cx(), (int32_t) m_box.get_cy(), SWP_NOREDRAW);

      }

   }

} // namespace html

