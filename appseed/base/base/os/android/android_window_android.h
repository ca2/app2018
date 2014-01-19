#pragma once


class CLASS_DECL_BASE window_android :
   public window_graphics
{
public:


   //cairo_t *                     m_cairoSource;
   //cairo_surface_t *             m_cairosurfaceSource;
   ::primitive::memory                 m_mem;
   size                                m_size;


   window_android();
   virtual ~window_android();


   virtual void create(oswindow window, int64_t cx, int64_t cy, int iStride = -1);
   virtual void destroy();

   virtual void update_window(oswindow window, COLORREF * pOsBitmapData, LPCRECT lpcrect, int iStride = -1);

};
