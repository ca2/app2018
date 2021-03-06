#include "framework.h"


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         appearance::CTool001::CTool001(::aura::application * papp) :
            object(papp)
         {
         }

         appearance::appearance(WorkSet * pworkset) :
            object(pworkset->get_app())
         {
            m_ptool001 = new CTool001(get_app());
            //m_pwndframeworkdownupinterface = this;
            m_bEnabled = true;
            m_pworkset = pworkset;
            m_bUseNc = false;
            m_nTransparency = Transparent;

            m_picon = NULL;

         }

         appearance::~appearance()
         {
            ::aura::del(m_ptool001);
         }

         sp(::user::interaction) appearance::get_window()
         {

            return m_pworkset->GetRegionWindow();

         }


         bool appearance::update()
         {

            if (m_pworkset == NULL)
            {

               return false;

            }

            if(m_pworkset->m_pframeschema == NULL)
            {

               return false;

            }

            //try
            //{

            //   if (m_pworkset->m_pframeschema->m_puserstyle.is_null())
            //   {

            //      m_pworkset->m_pframeschema->m_puserstyle = m_pworkset->GetWndDraw()->m_puserstyle;

            //   }

            //}
            //catch (...)
            //{

            //}

            try
            {

               m_pworkset->m_pframeschema->on_initialize_appearance();

               return true;

            }
            catch (...)
            {

            }

            return false;

         }


         void appearance::Enable(bool bEnable)
         {

            m_bEnabled = bEnable;

            if (m_pworkset != NULL)
            {

               if (m_pworkset->m_pframeschema != NULL)
               {

                  m_pworkset->m_pframeschema->title_bar_layout(false);

               }

            }

         }


         bool appearance::IsEnabled()
         {
            return m_bEnabled;
         }

         void appearance::SetAppearance(::user::e_appearance emode)
         {

            if (m_pworkset == NULL)
               return;

            m_emode = emode;

         }


         void appearance::SetTransparency(EAppearanceTransparency nTransparency)
         {

            m_nTransparency = nTransparency;

         }


         void appearance::SetAppearance()
         {

            sp(::user::interaction) pwnd = get_window();

            if (pwnd == NULL)
               return;
            if (pwnd->WfiIsZoomed())
               SetAppearance(appearance_zoomed);
            else if (pwnd->WfiIsIconic())
               SetAppearance(appearance_iconic);
            else
               SetAppearance(appearance_normal);

         }


         ::user::e_appearance appearance::GetAppearance()
         {

            return m_emode;

         }


         bool appearance::IsZoomed()
         {

            return GetAppearance() == appearance_zoomed;

         }

         bool appearance::IsDocked()
         {

            return ::user::is_docking_appearance(GetAppearance());

         }

         bool appearance::IsFullScreen()
         {

            return GetAppearance() == appearance_full_screen;

         }


         bool appearance::IsMinimal()
         {

            return GetAppearance() == appearance_minimal;

         }


         bool appearance::IsIconic()
         {

            return GetAppearance() == appearance_iconic;

         }


         //bool appearance::WndFrameworkDownUpGetUpEnable()
         //{

         //   CWorkSetDownUpInterface * pinterface = m_pwndframeworkdownupinterface;
         //   //dynamic_cast<CWorkSetDownUpInterface *>(get_draw_window());
         //   if (pinterface != NULL
         //      && pinterface != this)
         //   {
         //      return pinterface->WndFrameworkDownUpGetUpEnable();
         //   }
         //   else
         //      return false;
         //}

         //bool appearance::WndFrameworkDownUpGetDownEnable()
         //{

         //   CWorkSetDownUpInterface * pinterface = m_pwndframeworkdownupinterface;
         //   //dynamic_cast<CWorkSetDownUpInterface *>(get_draw_window());
         //   if (pinterface != NULL
         //      && pinterface != this)
         //   {
         //      return pinterface->WndFrameworkDownUpGetDownEnable();
         //   }
         //   else
         //      return false;
         //}


         //void appearance::frame_Attach()
         //{
         //   CWorkSetDownUpInterface * pinterface = m_pwndframeworkdownupinterface;
         //   if (pinterface != NULL
         //      && pinterface != this)
         //   {
         //      return pinterface->frame_Attach();
         //   }
         //}

         //void appearance::frame_Detach()
         //{
         //   CWorkSetDownUpInterface * pinterface = m_pwndframeworkdownupinterface;
         //   if (pinterface != NULL
         //      && pinterface != this)
         //   {
         //      return pinterface->frame_Detach();
         //   }
         //}


         //void appearance::SetDownUpInterface(CWorkSetDownUpInterface *pinterface)
         //{
         //   m_pwndframeworkdownupinterface = pinterface;
         //}

         void appearance::UpdateAppearance(bool bFullScreen)
         {
            sp(::user::interaction) pwnd = m_pworkset->get_draw_window();

            if (pwnd->WfiIsIconic())
            {
               m_emode = appearance_iconic;
            }
            else if (pwnd->WfiIsZoomed())
            {
               m_emode = appearance_zoomed;
            }
            else if (bFullScreen)
            {
               m_emode = appearance_full_screen;
            }
            else
            {
               m_emode = appearance_normal;
            }
         }


         void appearance::CTool001::Update(::draw2d::graphics * pgraphics, const RECT & rect,
                                           COLORREF crHighlight,
                                           COLORREF crLight,
                                           COLORREF crBody,
                                           COLORREF crShadow,
                                           COLORREF crDkShadow)
         {

            UNREFERENCED_PARAMETER(crLight);

            m_penHighlight->create_solid(1, crHighlight);
            m_penLight->create_solid(1, crHighlight);
            m_penShadow->create_solid(1, crShadow);
            m_penDkShadow->create_solid(1, crDkShadow);
            m_brushNull->create_null();
            m_brushBody->create_solid(crBody);

            m_colorbezieraOutsideBorder.remove_all();
            ::rect rectClient(rect);
            ::rect rectA(rectClient);
            rectA.deflate(3, 3, 3, 3);
            ::rect rectB(rectClient);
            rectB.deflate(18, 18, 18, 18);
            ::rect rectC(rectClient);
            rectC.deflate(7, 7, 7, 7);

            const int32_t ciStart = 4;
            const int32_t ciCurve = 2;


            CColorBezier bezier;




            // top left corner

            // top left corner - bottom left corner
            bezier.m_pointset.add(rectA.left, rectB.top + ciStart);
            bezier.m_pointset.add(rectA.left, rectB.top + ciCurve);
            bezier.m_pointset.add(rectA.left + ciCurve, rectB.top);
            bezier.m_pointset.add(rectA.left + ciStart, rectB.top);
            bezier.m_ppen = m_penHighlight;
            m_colorbezieraOutsideBorder.add(bezier);


            // top left corner - bottom horizontal
            bezier.clear();
            bezier.m_pointset.add(rectA.left + ciStart, rectB.top);
            bezier.m_pointset.add(rectC.left - ciStart, rectB.top);
            bezier.m_pointset.add(rectC.left - ciStart, rectB.top);
            bezier.m_pointset.add(rectC.left - ciStart, rectB.top);
            bezier.m_ppen = m_penLight;
            m_colorbezieraOutsideBorder.add(bezier);

            // top left corner - bottom middle corner
            bezier.clear();
            bezier.m_pointset.add(rectC.left - ciStart, rectB.top);
            bezier.m_pointset.add(rectC.left - ciCurve, rectB.top);
            bezier.m_pointset.add(rectC.left, rectB.top - ciCurve);
            bezier.m_pointset.add(rectC.left, rectB.top - ciStart);
            bezier.m_ppen = m_penDkShadow;
            m_colorbezieraOutsideBorder.add(bezier);

            // top left corner - middle vertical
            bezier.clear();
            bezier.m_pointset.add(rectC.left, rectB.top - ciStart);
            bezier.m_pointset.add(rectC.left, rectC.top + ciStart);
            bezier.m_pointset.add(rectC.left, rectC.top + ciStart);
            bezier.m_pointset.add(rectC.left, rectC.top + ciStart);
            bezier.m_ppen = m_penLight;
            m_colorbezieraOutsideBorder.add(bezier);

            // top left corner - middle corner
            bezier.clear();
            bezier.m_pointset.add(rectC.left, rectC.top + ciStart);
            bezier.m_pointset.add(rectC.left, rectC.top + ciCurve);
            bezier.m_pointset.add(rectC.left + ciCurve, rectC.top);
            bezier.m_pointset.add(rectC.left + ciStart, rectC.top);
            bezier.m_ppen = m_penHighlight;
            m_colorbezieraOutsideBorder.add(bezier);

            // top left corner - middle horizontal
            bezier.clear();
            bezier.m_pointset.add(rectC.left + ciStart, rectC.top);
            bezier.m_pointset.add(rectB.left - ciStart, rectC.top);
            bezier.m_pointset.add(rectB.left - ciStart, rectC.top);
            bezier.m_pointset.add(rectB.left - ciStart, rectC.top);
            bezier.m_ppen = m_penLight;
            m_colorbezieraOutsideBorder.add(bezier);

            // top left corner - bottom middle corner
            bezier.clear();
            bezier.m_pointset.add(rectB.left - ciStart, rectC.top);
            bezier.m_pointset.add(rectB.left - ciCurve, rectC.top);
            bezier.m_pointset.add(rectB.left, rectC.top - ciCurve);
            bezier.m_pointset.add(rectB.left, rectC.top - ciStart);
            bezier.m_ppen = m_penDkShadow;
            m_colorbezieraOutsideBorder.add(bezier);

            // top left corner - right vertical
            bezier.clear();
            bezier.m_pointset.add(rectB.left, rectC.top - ciStart);
            bezier.m_pointset.add(rectB.left, rectA.top + ciStart);
            bezier.m_pointset.add(rectB.left, rectA.top + ciStart);
            bezier.m_pointset.add(rectB.left, rectA.top + ciStart);
            bezier.m_ppen = m_penLight;
            m_colorbezieraOutsideBorder.add(bezier);


         }

         void appearance::CTool001::draw(::draw2d::graphics * pgraphics)
         {



            pgraphics->SelectObject(m_brushBody);

            ::draw2d::pen_sp pen(allocer());

            pen->create_solid(0, RGB(255, 255, 255));

            pgraphics->SelectObject(pen);

            point pt(0, 0);

            System.visual().api().DrawAndFillBeziers(
            pgraphics,
            m_pointsetBody.get_data(),
            (int32_t)m_pointsetBody.get_size(),
            1.0,
            &pt);

            for (int32_t i = 0; i < m_colorbezieraOutsideBorder.get_size(); i++)
            {
               CColorBezier & bezier = m_colorbezieraOutsideBorder[i];
               bezier.draw(*m_brushNull, pgraphics);
            }

         }

         void appearance::CTool001::CColorBezier::draw(::draw2d::brush & brush, ::draw2d::graphics * pgraphics)
         {
            pgraphics->SelectObject(&brush);
            pgraphics->SelectObject(m_ppen);
            point pt(0, 0);

            Sys(pgraphics->get_app()).visual().api().DrawAndFillBeziers(
            pgraphics,
            m_pointset.get_data(),
            (int32_t)m_pointset.get_size(),
            1.0,
            &pt);

         }

         void  appearance::CTool001::CColorBezier::clear()
         {
            m_pointset.remove_all();
         }

         bool appearance::IsNotifyIconEnabled()
         {
            return m_pworkset->IsNotifyIconEnabled();
         }


         sp(::user::wndfrm::frame::control_box_button) appearance::get_box_button(e_button ebutton)
         {

            ::exception::throw_interface_only(get_app());

            return NULL;

         }


      } // namespace frame


   } // namespace wndfrm


} // namespace user





