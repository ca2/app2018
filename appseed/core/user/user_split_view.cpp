#include "framework.h"

namespace user
{

   split_view::split_view(sp(base_application) papp) :
      element(papp),
      
      split_layout(papp),
      place_holder_container(papp)
   {
   }

   split_view::~split_view()
   {
   }

   void split_view::install_message_handling(::message::dispatch * pinterface)
   {
      view::install_message_handling(pinterface);
      USER_MESSAGE_LINK(message_create, pinterface, this, &split_view::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &split_view::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &split_view::_001OnShowWindow);
   }



   /////////////////////////////////////////////////////////////////////////////
   // split_view diagnostics

   #ifdef DEBUG
   void split_view::assert_valid() const
   {
      view::assert_valid();
   }

   void split_view::dump(dump_context & dumpcontext) const
   {
      view::dump(dumpcontext);
   }
   #endif //DEBUG

   void split_view::_001OnCreate(signal_details * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj)



   }

   void split_view::on_update(sp(::user::view) pSender, LPARAM lHint, object* phint)
   {

      if (phint != NULL)
      {

         view_update_hint * pupdatehint = dynamic_cast < view_update_hint * > (phint);


         if(pupdatehint->m_ehint == view_update_hint::hint_open_document
            || pupdatehint->m_ehint == view_update_hint::hint_create_views)
         {

            try
            {

               on_create_views();

            }
            catch (...)
            {

               string strMessage;

               strMessage.Format("split_view::on_create_views failed to create views for split view %s", typeid(this).raw_name());

               Application.simple_message_box_timeout(this, strMessage, seconds(10), MB_ICONEXCLAMATION);

            }

            layout();

         }

      }


   }


   void split_view::_001OnSize(signal_details * pobj)
   {
      SCAST_PTR(::message::size, psize, pobj)
      psize->previous();
      //layout();
   }

   bool split_view::pre_create_window(CREATESTRUCT& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

      return view::pre_create_window(cs);
   }


   void split_view::on_create_views()
   {


   }

   void split_view::_001OnDraw(::draw2d::graphics *pdc)
   {

      int32_t i;
      rect rect;
      COLORREF cr = ARGB(184, 92, 92, 80);
      //COLORREF crBack = ARGB(77, 230, 230, 210);
      //pdc->set_alpha_mode(::draw2d::alpha_mode_blend);
      for(i = 0; i < get_pane_count(); i++)
      {
         CalcPaneRect(i, &rect);

        // pdc->FillSolidRect(rect, crBack);


         // Top
         pdc->FillSolidRect(
            rect.left,
            rect.top,
            rect.width(),
            m_cyBorder,
            cr);

         // Right
         pdc->FillSolidRect(
            rect.right - m_cxBorder,
            rect.top,
            m_cxBorder,
            rect.height(),
            cr);

         // Bottom
         pdc->FillSolidRect(
            rect.left,
            rect.bottom - m_cyBorder,
            rect.width(),
            m_cyBorder,
            cr);

         // Left
         pdc->FillSolidRect(
            rect.left,
            rect.top,
            m_cxBorder,
            rect.height(),
            cr);
      }
   }


   void split_view::_001OnShowWindow(signal_details * pobj)
   {
      SCAST_PTR(::message::show_window, pshowwindow, pobj)
      pshowwindow->previous();
      layout();
   }

} // namespace user
