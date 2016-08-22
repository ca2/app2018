#pragma once


class CLASS_DECL_BASE window_gdi :
   virtual public window_buffer
{
public:


   HBITMAP                       m_hbitmap;
   HBITMAP                       m_hbitmapOld;
   BITMAPINFO                    m_bitmapinfo;
   COLORREF *                    m_pcolorref;
   HDC                           m_hdc;
   HDC                           m_hdcScreen;
   bool                          m_bOwnDC;
   HANDLE                        m_hmutex;


   window_gdi(::aura::application * papp);
   virtual ~window_gdi();


   virtual void create_window_graphics(int64_t cx, int64_t cy, int iStride = -1);
   virtual void destroy_window_graphics();


   virtual void update_window(COLORREF * pcolorref,int cxParam,int cyParam,int iStride = -1);


};
