#include "framework.h"


namespace html
{


   namespace impl
   {


      namespace user
      {


         class CLASS_DECL_CORE check_box:
            virtual public ::user::check_box
         {
         public:

            check_box(::aura::application * papp): object(papp),::user::check_box(papp) {}
            virtual ~check_box() {}

            virtual void _001OnClip(::draw2d::graphics * pgraphics) {}
         };

      } // namespace user


      input_checkbox::input_checkbox(data * pdata)
      {

         m_pcheckbox = canew( user::check_box(pdata->get_app()));
         
         m_pcheckbox->m_pform = pdata->m_pform;

      }


      input_checkbox::~input_checkbox()
      {

         try
         {

            if(m_pcheckbox->IsWindow())
            {

               m_pcheckbox->DestroyWindow();

            }

         }
         catch(...)
         {

         }

         try
         {

            m_pcheckbox.release();

         }
         catch(...)
         {

         }

      }


      void input_checkbox::implement_phase1(data * pdata, ::html::elemental * pelemental)
      {

         m_bHasChar = true;

         if(!m_pcheckbox->IsWindow())
         {
            m_pcheckbox->create_window(null_rect(),pdata->m_pui,100);
            pdata->on_create_interaction(m_pcheckbox);
             bool bCheck = pelemental->m_pbase->get_tag()->get_attr("checked") != NULL;
             if(bCheck)
             {
                m_pcheckbox->_001SetCheck(check::checked, ::action::source::add(::action::source_data, ::action::source_load));
             }
             else
             {
                m_pcheckbox->_001SetCheck(check::unchecked, ::action::source::add(::action::source_data, ::action::source_load));
             }
         }
         elemental::implement_phase1(pdata, pelemental);

         m_pcheckbox->m_id = pelemental->m_pbase->get_tag()->get_attr_value("id");
         m_pcheckbox->m_strName = pelemental->m_pbase->get_tag()->get_attr_value("name");
         m_pcheckbox->m_id = pelemental->m_pbase->get_tag()->get_attr_value("id");
         if(pdata->m_pform != NULL)
         {
            ::user::control_event ev;
            ev.m_puie = m_pcheckbox;
            ev.m_actioncontext = ::action::source::add(::action::source_data, ::action::source_load);
            ev.m_eevent = ::user::event_initialize_control;
            ev.m_uiEvent = 0;
            pdata->m_pform->BaseOnControlEvent(&ev);
         }
      }

      bool input_checkbox::layout_phase1(data * pdata)
      {

         m_box.set_cxy(23, 23);

         return true;

      }


      void input_checkbox::layout_phase3(data * pdata)
      {

         elemental::layout_phase3(pdata);
         
         m_pcheckbox->SetWindowPos(0, (int32_t) m_box.left, (int32_t) m_box.top, (int32_t) m_box.get_cx(), (int32_t) m_box.get_cy(), SWP_NOREDRAW);

      }


      void input_checkbox::_001OnDraw(data * pdata)
      {

         ::draw2d::graphics * pgraphics = pdata->m_pgraphics;

         //rect rectWindow;
         //m_pcheckbox->GetWindowRect(rectWindow);
         ::point ptPreviousViewportOrg = pgraphics->GetViewportOrg();
         pgraphics->OffsetViewportOrg((int32_t) m_box.left, (int32_t) m_box.top);
         m_pcheckbox->_000OnDraw(pdata->m_pgraphics);
         pgraphics->SetViewportOrg(ptPreviousViewportOrg);
      }

      void input_checkbox::on_change_layout(data * pdata)
      {
         UNREFERENCED_PARAMETER(pdata);
         m_pcheckbox->SetWindowPos(0, (int32_t) m_box.left, (int32_t) m_box.top, (int32_t) m_box.get_cx(), (int32_t) m_box.get_cy(), SWP_NOREDRAW);
      }

   }

}
