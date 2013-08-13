//
//  c_os_simple_graphics.h
//  c
//
//  Created by Thomas Borregaard Sørensen on 9/30/12.
//
//

#ifdef __MM
#else
typedef CGRect NSRect;
#endif

class CLASS_DECL_ca os_simple_graphics
{
public:
   
#ifdef __MM
   NSGraphicsContext *     m_nsgc;
#else
   void *                  m_pnsgc;
   
#endif
   
   //Display *               m_pdisplay;
   int                     m_iScreen;
   //Drawable                m_d;
   //GC                      m_gc;
   //Colormap                m_colormap;
   simple_pen              m_pen;
   simple_brush            m_brush;
   os_simple_font          m_font;
   simple_bitmap           m_bitmap;
   /*   oswindow                    m_hwnd;
    HBITMAP                 m_hbitmapOld;
    HPEN                    m_hpenOld;
    HFONT                   m_hfontOld;
    HBRUSH                  m_hbrushOld;
    int                     m_iType;
    PAINTSTRUCT             m_ps;*/
   int                     m_iType;
   bool                    m_bForeColor;
   bool                    m_bBackColor;
#ifdef __MM
   NSColor *               m_nscolorFore;
   NSColor *               m_nscolorBack;
#else
   void *                  m_pnscolorFore;
   void *                  m_pnscolorBack;
#endif
   COLORREF                m_crTextColor;
   bool                    m_bGotOriginalClipPath;
   NSRect                  m_rectOriginalClip;
   
   
   os_simple_graphics();
   ~os_simple_graphics();
   
   
   // aim to be all-platoform - but if there is no equivalent because no op : create empty method
   bool create();
   bool destroy();
   bool bit_blt(int x, int y, int cx, int cy, simple_graphics & gSrc, int x1, int y1, DWORD rop);
   bool alpha_blend(int x, int y, int cx, int cy, simple_graphics & gSrc, int x1, int y1, int cx1, int cy1, BLENDFUNCTION bf);
   bool create_from_bitmap(simple_bitmap & b);
   bool select(simple_font & font);
   bool select(simple_brush & brush);
   bool select(simple_pen & brush);
   bool text_out(int x, int y, const char * pszUtf8, int iLen = -1);
   bool blend_bitmap_data(int x, int y, int cx, int cy, COLORREF * pdata);
   SIZE get_text_extent(const char * psz, int iLen = -1);
   //bool set_text_color(COLORREF cr);
   
   
   bool is_null();
   bool set_alpha_mode(::draw2d::e_alpha_mode emode);

   
   
   
   bool rectangle(LPCRECT lpcrect);
   bool draw_line(int x1, int y1, int x2, int y2, simple_pen & pen);
   bool draw_rect(LPCRECT lpcrect, simple_pen & pen);
   bool fill_rect(LPCRECT lpcrect, simple_brush & brush);
   void fill_solid_rect(LPCRECT lpRect, COLORREF clr);


   bool fill_polygon(POINT * p, int iCount, ::draw2d::e_fill_mode);
   
   
   bool draw_path(simple_path & path, simple_pen & pen);
   bool fill_path(simple_path & path, simple_brush & brush);

   
   bool replace_clip(simple_path & path);
   bool exclude_clip(simple_path & path);
   bool replace_clip(const RECT & r);
   bool remove_clip();
   
   
   
   // may be multi-platform
   bool create(simple_graphics & g);
   bool create_from_screen();
   bool set_offset(int x, int y);
   bool offset(int x, int y);
   
   
   // platform-specific
   bool create(HDC hdc);
   bool from_entire_window(oswindow hwnd);
   bool from_window(oswindow hwnd);
   bool from_window_paint(oswindow hwnd, LPRECT lprectPaint = NULL);
   bool reference_os_data(HDC hdc);
//   XFontStruct * get_font(simple_font & font);
//   XFontStruct * get_font();
   void set(simple_brush & brush);
   void set(simple_pen & pen);
#ifdef __MM
   NSColor * alloc_color(COLORREF cr);
   bool free_color(NSColor * nscolor);
#else
   void * alloc_color(COLORREF cr);
   bool free_color(void * pnscolor);
#endif
   void set_foreground(COLORREF cr);
   void set_background(COLORREF cr);
   void defer_get_original_clip_path();

};
