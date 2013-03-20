#include "framework.h"


namespace userbase
{


   bool base_static::create(::user::interaction* pParentWnd, id id)
   {

      return ::user::control::create(pParentWnd, id);

   }


   base_static::~base_static()
   {

      DestroyWindow();

   }


   // Derived class is responsible for implementing all of these handlers
   //   for owner/self draw controls
#ifdef WINDOWSEX


   void base_static::DrawItem(LPDRAWITEMSTRUCT)
   {

      ASSERT(FALSE);

   }


#endif


   bool base_static::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
      LRESULT* pResult)
   {
      UNREFERENCED_PARAMETER(message);
      UNREFERENCED_PARAMETER(wParam);
   // trans   if (message != WM_DRAWITEM)
         // return ::user::interaction::OnChildNotify(message, wParam, lParam, pResult);

      ASSERT(pResult == NULL);       // no return value expected
#ifdef WINDOWSEX
      UNUSED(pResult); // unused in release builds
      DrawItem((LPDRAWITEMSTRUCT)lParam);
#endif
      return TRUE;
   }


   base_static::base_static(::ca::application * papp) :
      ca(papp)
   {
      
      m_etype = type_text;

   }


   void base_static::set_icon(::visual::icon * picon)
   {
      
      if(picon == NULL)
         return;

      m_etype = type_icon;

      m_picon = picon;

   }


   ::visual::icon * base_static::get_icon()
   {

      return m_etype == type_icon ? m_picon : NULL;

   }


   HENHMETAFILE base_static::SetEnhMetaFile(HENHMETAFILE hMetaFile)
   {

      //ASSERT(IsWindow());

      //return (HENHMETAFILE)send_message( STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);

      return NULL;

   }


   HENHMETAFILE base_static::GetEnhMetaFile()
   {

      //ASSERT(IsWindow());

      //return (HENHMETAFILE)send_message( STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L);

      return NULL;

   }


   void base_static::set_bitmap(::ca::bitmap * pbitmap)
   {

      
      if(pbitmap == NULL)
         return;

      m_etype = type_icon;

      m_pbitmap = pbitmap;


   }


   ::ca::bitmap * base_static::get_bitmap()
   {

      return m_etype == type_bitmap ? m_pbitmap : NULL;

   }


   void base_static::set_cursor(::visual::cursor * pcursor)
   {

      if(pcursor == NULL)
         return;

      m_etype = type_cursor;

      m_pcursor = pcursor;

   }


   ::visual::cursor *  base_static::get_cursor()
   {

      return m_etype == type_cursor ? m_pcursor : NULL;

   }

   ::userbase::base_static::e_type base_static::get_type()
   {
      
      return m_etype;

   }


} // namespace userbase







