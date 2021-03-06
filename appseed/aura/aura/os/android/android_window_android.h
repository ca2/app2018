#pragma once


class CLASS_DECL_AURA window_android :
   public window_graphics
{
public:


   //cairo_t *                     m_cairoSource;
   //cairo_surface_t *             m_cairosurfaceSource;
   memory                 m_mem;
   size                                m_size;


   window_android(::aura::application * papp);
   virtual ~window_android();


   virtual void create_window_graphics_(int64_t cx, int64_t cy, int iStride = -1);
   virtual void destroy_window_graphics_();

   virtual void update_window(oswindow interaction_impl, COLORREF * pOsBitmapData, const RECT & rect, int iStride = -1);

};
