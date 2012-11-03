#include "framework.h"

namespace userbase
{

   menu_button::menu_button(::ca::application * papp) :
      ::user::interaction(papp),
      ::user::button(papp),
      button(papp),
      ca(papp)
   {
      m_pitem = NULL;
   }

   menu_button::~menu_button()
   {
   }


   void menu_button::_001OnDraw(::ca::graphics * pdc)
   {
      button::_001OnDraw(pdc);
      rect rectClient;
      m_pguie->GetClientRect(rectClient);
      if(m_pitem != NULL && m_pitem->m_bPopup)
      {
         ::ca::brush_sp br(get_app(), RGB(0, 0, 0));
         ::ca::pen_sp pen(get_app());
         pen->create_solid(1, RGB(0, 0, 0));
         pdc->SelectObject(pen);
         pdc->SelectObject(br);
         rect rectPopupArrow;
         rectPopupArrow.left = rectClient.right - 9;
         rectPopupArrow.right = rectClient.right - 4;
         rectPopupArrow.top = ((rectClient.top + rectClient.bottom) / 2) - 4;
         rectPopupArrow.bottom = ((rectClient.top + rectClient.bottom) / 2) + 4;
         point_array pta;
         pta.add(point(rectPopupArrow.left, rectPopupArrow.bottom));
         pta.add(point(rectPopupArrow.right, (rectPopupArrow.bottom + rectPopupArrow.top) / 2));
         pta.add(point(rectPopupArrow.left, rectPopupArrow.top));
         pta.add(point(rectPopupArrow.left, rectPopupArrow.bottom));
         pdc->Polygon(pta.get_data(), (int) pta.get_size());
      }
      _001DrawCheck(pdc);
   }

   void menu_button::_001Layout()
   {
      rect rect;
      class rect rectClient;
      if(m_pguie == NULL)
         return;
      m_pguie->GetClientRect(rectClient);

      rect = rectClient;
      rect.left   += 3 + 16;
      rect.top    += 3;
      m_rectText = rect;

      rect = rectClient;
      rect.left   += 3;
      rect.top    += 3;
      rect.right  -= 3;
      rect.bottom -= 3;
      m_rectCheckBox = rect;
   }


   void menu_button::_001DrawCheck(::ca::graphics * pdc)
   {
      UINT uiImage = 0xffffffffu;
      if(m_pitem != NULL)
      {
         uiImage = BaseMenuCentral::GetMenuCentral(get_app())->CommandToImage(m_pitem->m_id);
      }
      if(uiImage != 0xffffffffu)
      {
         rect rectImage = m_rectCheckBox;
         rect rectImageBorder = rectImage;
         rectImageBorder.inflate(2, 2);
         IMAGEINFO ii;
         image_list * pimagelist;
         if(!_001IsWindowEnabled())
         {
            pimagelist = BaseMenuCentral::GetMenuCentral(get_app())->MenuV033GetImageListHueLight();
         }
         else
         {
            pimagelist = BaseMenuCentral::GetMenuCentral(get_app())->MenuV033GetImageList();
         }
         pimagelist->get_image_info(uiImage, &ii);
         rect rectImageInfo(ii.rcImage);
         rectImage.offset(1, 1);
         rectImage.top     = rectImage.bottom - rectImageInfo.height();
         rectImage.right   = rectImage.left + rectImageInfo.width();
         if(m_echeck == check::checked)
         {
            pdc->FillSolidRect(rectImageBorder, RGB(127, 127, 127));
            pdc->Draw3dRect(rectImageBorder, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
         }
         pimagelist->draw(
            pdc,
            uiImage,
            rectImage.top_left(),
            0);
      }
      else
      {
         userbase::GetUfeSchema(get_app())->DrawCheck(m_echeck, m_rectCheckBox, pdc);
      }
   }

   menu_button_cmd_ui::menu_button_cmd_ui(::ca::application * papp) :
      ca(papp),
      cmd_ui(papp)
   {
   }


   void menu_button_cmd_ui::Enable(bool bOn)
   {
      m_bEnableChanged = TRUE;
      menu_button* pbutton = dynamic_cast < menu_button * > (m_pOther);
      pbutton->_001EnableWindow(bOn != FALSE);
      /*ASSERT(pToolBar != NULL);
      ASSERT_KINDOF(simple_toolbar, pToolBar);
      ASSERT(m_nIndex < m_nIndexMax);*/

      /*UINT nNewStyle = pToolBar->GetGetButtonStyle(m_nIndex) & ~TBBS_DISABLED;
      if (!bOn)
      {
         nNewStyle |= TBBS_DISABLED;
         // If a button is currently pressed and then is disabled
         // COMCTL32.DLL does not unpress the button, even after the mouse
         // button goes up!  We work around this bug by forcing TBBS_PRESSED
         // off when a button is disabled.
         nNewStyle &= ~TBBS_PRESSED;
      }
      ASSERT(!(nNewStyle & TBBS_SEPARATOR));
      pToolBar->SetButtonStyle(m_nIndex, nNewStyle);*/
   }

   void menu_button_cmd_ui::SetCheck(check::e_check echeck)
   {
      ASSERT(echeck == check::checked || echeck == check::unchecked || echeck == check::tristate); // 0=>off, 1=>on, 2=>indeterminate
      menu_button* pbutton = dynamic_cast < menu_button *  > (m_pOther);
      pbutton->_001SetCheck(echeck, true);
   /*   ASSERT(pToolBar != NULL);
      ASSERT_KINDOF(simple_toolbar, pToolBar);
      ASSERT(m_nIndex < m_nIndexMax);

      UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) &
               ~(TBBS_CHECKED | TBBS_INDETERMINATE);
      if (nCheck == 1)
         nNewStyle |= TBBS_CHECKED;
      else if (nCheck == 2)
         nNewStyle |= TBBS_INDETERMINATE;
      ASSERT(!(nNewStyle & TBBS_SEPARATOR));
      pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);*/
   }

   void menu_button_cmd_ui::SetText(const char *)
   {
      // ignore it
   }

} // namespace userbase
