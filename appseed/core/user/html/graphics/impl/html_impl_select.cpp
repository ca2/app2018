#include "framework.h"


namespace html
{


   namespace impl
   {


      select::select(data * pdata)
      {

         m_iFont = -1;
         m_pcombo = new ::user::combo_box(pdata->get_app());
         m_pcombo->m_ulFlags &= ~::object::flag_auto_delete;
         m_pcombo->m_bMultiLine = false;

      }


      select::~select()
      {

         try
         {

            if (m_pcombo->IsWindow())
            {

               m_pcombo->DestroyWindow();

            }

         }
         catch (...)
         {

         }

         try
         {

            m_pcombo.release();

         }
         catch (...)
         {

         }

      }


      void select::implement_phase1(data * pdata, ::html::elemental * pelemental)
      {

         m_bHasChar = true;

         elemental::implement_phase1(pdata, pelemental);

         if(pelemental->m_pbase->get_type() == ::html::base::type_tag && !m_pcombo->IsWindow())
         {
            m_pcombo->oprop("parent_lock_data") = (::data::data *) pdata;
            m_pcombo->create_window(null_rect(),pdata->m_pui,100);
            pdata->on_create_interaction(m_pcombo);
            m_pcombo->m_bPassword = pelemental->m_propertyset["type"].compare_value_ci("password") == 0;
            m_pcombo->m_strName = pelemental->m_pbase->get_tag()->get_attr_value("name");
            m_pcombo->m_id = pelemental->m_pbase->get_tag()->get_attr_value("id");
            m_pcombo->m_edatamode = ::user::combo_box::data_mode_string;

            for (index i = 0; i < pelemental->m_pbase->get_tag()->baseptra().get_count(); i++)
            {
               if(pelemental->m_pbase->get_tag()->baseptra()[i]->get_type() != ::html::base::type_tag)
                  continue;
               string strText = pelemental->m_pbase->get_tag()->baseptra()[i]->get_tag()->baseptra()[0]->get_value()->get_value();
               string strValue = pelemental->m_pbase->get_tag()->baseptra()[i]->get_tag()->get_attr_value("value");
               m_pcombo->AddString(strText, strValue);
            }
            
            m_pcombo->_001SetText(pelemental->m_pbase->get_tag()->get_attr_value("value"), ::action::source::add(::action::source_data, ::action::source_load));

            m_pcombo->m_puserschema = pelemental;

            m_pcombo->m_bEdit = false;

         }

         pdata->m_focusptra.add_unique(m_pcombo);

         m_cxMax = 200;

         m_cxMin = 200;

      }


      void select::layout_phase1(data * pdata)
      {

         if(m_pelemental->m_pbase->get_type() != ::html::base::type_tag)
            return;

         string strSize = m_pelemental->m_pbase->get_tag()->get_attr_value("size");

         int iSize = 20;

         if (strSize.has_char())
            iSize = MAX(1, atoi(strSize));
      
         m_box.set_cxy(iSize * 10.f, 23.f);

      }


      void select::layout_phase1_end(data * pdata)
      {

         UNREFERENCED_PARAMETER(pdata);

      }


      
      void select::layout_phase3(data * pdata)
      {

         
         elemental::layout_phase3(pdata);
         if(m_pelemental->m_pbase->get_type() != ::html::base::type_tag)
            return;


         m_pcombo->SetWindowPos(0, (int32_t)m_box.left, (int32_t)m_box.top, (int32_t)m_box.get_cx(), (int32_t)m_box.get_cy(), SWP_NOREDRAW);

      }


      void select::_001OnDraw(data * pdata)
      {

         ::draw2d::graphics * pgraphics = pdata->m_pgraphics;

         rect rectWindow;
         m_pcombo->GetWindowRect(rectWindow);
         m_pcombo->ScreenToClient(rectWindow);
         ::point ptPreviousViewportOrg = pgraphics->GetViewportOrg();
         pgraphics->SetViewportOrg(rectWindow.top_left());
         m_pcombo->_000OnDraw(pgraphics);
         pgraphics->SetViewportOrg(ptPreviousViewportOrg);

      }


      void select::on_change_layout(data * pdata)
      {

         if(m_pelemental->m_pbase->get_type() != ::html::base::type_tag)
            return;

         UNREFERENCED_PARAMETER(pdata);
         m_pcombo->SetWindowPos(0, (int32_t)m_box.left, (int32_t)m_box.top, (int32_t)m_box.get_cx(), (int32_t)m_box.get_cy(), SWP_NOREDRAW);
      }

      void select::set_pos(data * pdoc,float x,float y,float cx,float cy)
      {

         m_box.set_pos_dim(x,y,m_box.get_cx(),m_box.get_cy());
         on_change_layout(pdoc);

      }

   }

} // namespace html

