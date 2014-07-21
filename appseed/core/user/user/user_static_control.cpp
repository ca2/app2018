#include "framework.h"


namespace user
{


   static_control::static_control(sp(::base::application) papp) :
      element(papp)
   {

      m_etype = type_text;

      m_bLButtonDown = false;

      m_crText = ARGB(255, 0, 0, 0);

      m_crHover = ARGB(255,84,84,84);

      m_bHover = false;

   }


   static_control::~static_control()
   {

      DestroyWindow();

   }


   
   bool static_control::create(sp(::user::interaction) pParentWnd, id id)
   {

      return ::user::control::create(pParentWnd, id);

   }


   void static_control::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::control::install_message_handling(pdispatch);

      IGUI_WIN_MSG_LINK(WM_CREATE, pdispatch, this, &static_control::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pdispatch, this, &static_control::_001OnLButtonDown);
      IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pdispatch, this, &static_control::_001OnLButtonUp);
      IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pdispatch, this, &static_control::_001OnMouseLeave);
      IGUI_WIN_MSG_LINK(WM_MOUSELEAVE, pdispatch, this, &static_control::_001OnMouseMove);

   }

   void static_control::_001OnCreate(signal_details * pobj)
   {

      pobj->previous();

      if (pobj->m_bRet)
         return;

   }

   void static_control::_001OnLButtonDown(signal_details * pobj)
   {

//      SCAST_PTR(::message::mouse, pmouse, pobj);
       UNREFERENCED_PARAMETER(pobj);

      m_bLButtonDown = true;

   }


   void static_control::_001OnLButtonUp(signal_details * pobj)
   {
      
//      SCAST_PTR(::message::mouse, pmouse, pobj);
       UNREFERENCED_PARAMETER(pobj);

      if (m_bLButtonDown)
      {
         
         m_bLButtonDown = false;

         ::user::control_event ev;
         ev.m_puie = this;
         ev.m_eevent = ::user::event_button_clicked;
         ev.m_actioncontext = ::action::source_user;
         
         BaseOnControlEvent(&ev);

      }


   }

   void static_control::_001OnMouseMove(signal_details * pobj)
   {

//      SCAST_PTR(::message::mouse, pmouse, pobj);
       UNREFERENCED_PARAMETER(pobj);
 }


   void static_control::_001OnMouseLeave(signal_details * pobj)
   {

//      SCAST_PTR(::message::mouse, pleave, pobj);
       UNREFERENCED_PARAMETER(pobj);

      m_bLButtonDown = false;

   }

   void static_control::_001OnDraw(::draw2d::graphics * pdc)
   {

      COLORREF crText;

      if(m_bHover)
      {

         point pt;
         
         session().get_cursor_pos(&pt);

         ::rect rectWindow;

         GetWindowRect(rectWindow);

         if(rectWindow.contains(pt))
         {

            crText = m_crHover;

         }
         else
         {

            crText = m_crText;

         }

      }
      else
      {

         crText = m_crText;

      }

      pdc->set_text_color(crText);

      pdc->SelectObject(GetFont());

      pdc->TextOut(0, 0, get_window_text());

   }


   bool static_control::OnChildNotify(::message::base * pbase)
   {
      
      if (pbase->m_uiMessage != WM_DRAWITEM)
          return ::user::interaction::OnChildNotify(pbase);

#ifdef WINDOWSEX
      
//      DrawItem((LPDRAWITEMSTRUCT)pbase->m_lparam);

#endif

      return true;

   }


   void static_control::set_icon(::visual::icon * picon)
   {

      if(picon == NULL)
         return;

      m_etype = type_icon;

      m_picon = picon;

   }


   ::visual::icon * static_control::get_icon()
   {

      return m_etype == type_icon ? m_picon : NULL;

   }


   HENHMETAFILE static_control::SetEnhMetaFile(HENHMETAFILE hMetaFile)
   {

      //ASSERT(IsWindow());

      //return (HENHMETAFILE)send_message( STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);

      return NULL;

   }


   HENHMETAFILE static_control::GetEnhMetaFile()
   {

      //ASSERT(IsWindow());

      //return (HENHMETAFILE)send_message( STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L);

      return NULL;

   }


   void static_control::set_bitmap(::draw2d::bitmap * pbitmap)
   {


      if(pbitmap == NULL)
         return;

      m_etype = type_icon;

      m_pbitmap = pbitmap;


   }


   ::draw2d::bitmap * static_control::get_bitmap()
   {

      return m_etype == type_bitmap ? m_pbitmap : NULL;

   }


   void static_control::set_cursor(::visual::cursor * pcursor)
   {

      if(pcursor == NULL)
         return;

      m_etype = type_cursor;

      m_pcursor = pcursor;

   }


   ::visual::cursor *  static_control::get_cursor()
   {

      return m_etype == type_cursor ? m_pcursor : NULL;

   }

   ::user::static_control::e_type static_control::get_type()
   {

      return m_etype;

   }


} // namespace user







