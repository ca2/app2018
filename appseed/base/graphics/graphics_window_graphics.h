#pragma once


class CLASS_DECL_BASE window_graphics
{
public:


   int32_t        cx;
   int32_t        cy;
   int32_t        scan;


   window_graphics();
   virtual ~window_graphics();


   virtual void create(oswindow window, int64_t cx, int64_t cy, int iStride = -1);
   virtual void destroy();


   virtual void update_window(oswindow window, COLORREF * pOsBitmapData, LPCRECT lpcrect, int iStride = -1);


   static void update_window(window_graphics * & pdata, oswindow window, COLORREF * pOsBitmapData, LPCRECT lpcrect, int iStride = -1);

   virtual void copy_colorref(COLORREF * pcolorrefDst, COLORREF * pcolorrefSrc, int iStrideSrc);

};



