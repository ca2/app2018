#pragma once


namespace draw2d_direct2d
{


   //////////////////////////////////////////////////////////////////////
   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////

   class CLASS_DECL_DRAW2D_DIRECT2D dib :
      virtual public ::draw2d::dib
   {
   public:

      HRESULT                       m_hrEndDraw;
      BITMAPINFO                    m_info;

      ::draw2d::bitmap_sp           m_spbitmap;
      ::draw2d::graphics_sp         m_spgraphics;

      ::draw2d::bitmap_sp           m_spbitmapMap;
      ::draw2d::graphics_sp         m_spgraphicsMap;

      bool                          m_bTrans;

      //float                                        m_dpi;
      //::comptr<ID2D1DeviceContext>        m_pdevicecontext;
      //mutex                                        m_mutexDc;



      dib(::aura::application * papp);
      virtual void construct(int32_t cx, int32_t cy);
      virtual ~dib();


      void map(bool bApplyAlphaTransform = true) const;
      void unmap() const;


      virtual ::draw2d::graphics * get_graphics() const;
      virtual ::draw2d::bitmap_sp get_bitmap() const;
      virtual ::draw2d::bitmap_sp detach_bitmap();


      virtual COLORREF * get_data() const;

      void stretch_dib(::draw2d::dib * pdib);

      bool dc_select(bool bSelect = true);





      bool create(class size size);
      bool create(int32_t iWidth, int32_t iHeight);
      bool create(::draw2d::graphics * pgraphics);
      bool Destroy();



      bool from(::draw2d::graphics * pgraphics);
      bool from(point ptDest, ::draw2d::graphics * pgraphics, point pt, class size sz);

      bool to(::draw2d::graphics * pgraphics, point pt, class size size, point ptSrc);

      void SetIconMask(::visual::icon * picon, int32_t cx, int32_t cy);


//#if defined(WINDOWSEX)
      virtual bool update_window(::aura::draw_interface * pwnd,::message::message * pobj,bool bTransferBuffer = true);
      //virtual bool print_window(window * pwnd, ::message::message * pobj);
//#endif


      //bool color_blend(COLORREF cr, BYTE bAlpha);
      //bool Blend(::draw2d::dib *pdib, ::draw2d::dib * pdibA, int32_t A);
      //void BitBlt(::draw2d::dib * pdib, int32_t op);
      //COLORREF GetAverageColor();
      //int32_t cos(int32_t i, int32_t iAngle);
      //int32_t sin(int32_t i, int32_t iAngle);
      //int32_t cos10(int32_t i, int32_t iAngle);
      //int32_t sin10(int32_t i, int32_t iAngle);

      //bool is_rgb_black();
      //void xor(::draw2d::dib * pdib);

      //void ToAlpha(int32_t i);
      //void ToAlphaAndFill(int32_t i, COLORREF cr);
      //void GrayToARGB(COLORREF cr);

      //void from_alpha();
      //void mult_alpha(::draw2d::dib * pdibWork, bool bPreserveAlpha = true);
      //void set_rgb(int32_t R, int32_t G, int32_t B);

      //void rotate(::draw2d::dib * pdib, LPCRECT lpcrect, double dAngle, double dScale);
      //void rotate(::draw2d::dib * pdib, double dAngle, double dScale);
      //void Rotate034(::draw2d::dib * pdib, double dAngle, double dScale);
      //void RadialFill(BYTE a, BYTE r, BYTE g, BYTE b, int32_t x, int32_t y, int32_t iRadius);
      //void RadialFill(
      //   BYTE a1, BYTE r1, BYTE g1, BYTE b1, // center colors
      //   BYTE a2, BYTE r2, BYTE g2, BYTE b2, // border colors
      //   int32_t x, int32_t y, int32_t iRadius);

      //uint32_t GetPixel(int32_t x, int32_t y);
      //void Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask);
      //void channel_mask(BYTE uchFind, BYTE uchSet, BYTE uchUnset, visual::rgba::echannel echannel);
      //void transparent_color(color color);
      //void DivideRGB(int32_t iDivide);
      //void DivideARGB(int32_t iDivide);
      //void DivideA(int32_t iDivide);
      //virtual void fill_channel(int32_t C, visual::rgba::echannel echannel);
      //void Fill (int32_t A, int32_t R, int32_t G, int32_t B );
      //void Fill ( int32_t R, int32_t G, int32_t B );
      //void FillGlass ( int32_t R, int32_t G, int32_t B, int32_t A );
      //void FillStippledGlass ( int32_t R, int32_t G, int32_t B );
      //void Invert();
      //void channel_invert(visual::rgba::echannel echannel);
      //void channel_multiply(visual::rgba::echannel echannel, double dRate);

      //void Map (int32_t ToRgb, int32_t FromRgb );

      /*void copy( ::draw2d::dib *dib );
      void Paste ( ::draw2d::dib *dib );

      void Blend ( ::draw2d::dib *dib, int32_t A );
      void Darken ( ::draw2d::dib *dib );
      void Difference ( ::draw2d::dib *dib );
      void Lighten ( ::draw2d::dib *dib );
      void Multiply ( ::draw2d::dib *dib );
      void Screen ( ::draw2d::dib *dib );

      void copy ( ::draw2d::dib *dib, int32_t x, int32_t y );
      void PasteRect ( ::draw2d::dib *dib, int32_t x, int32_t y );

      void fill_rect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B );
      void FillGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B, int32_t A );
      void FillStippledGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B );

      void BlendRect ( ::draw2d::dib *dib, int32_t x, int32_t y, int32_t A );
      void DarkenRect ( ::draw2d::dib *dib, int32_t x, int32_t y );
      void DifferenceRect ( ::draw2d::dib *dib, int32_t x, int32_t y );
      void LightenRect ( ::draw2d::dib *dib, int32_t x, int32_t y );
      void MultiplyRect ( ::draw2d::dib *dib, int32_t x, int32_t y );
      void ScreenRect ( ::draw2d::dib *dib, int32_t x, int32_t y );

      void Line ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B );
      void LineGlass ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B, int32_t A );*/

      //void create_frame(class size size, int32_t iFrameCount);
      //void set_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount);
      //void set_frame1(void * lpdata, int32_t iFrame, int32_t iFrameCount);
      //void get_frame(void * lpdata, int32_t iFrame, int32_t iFrameCount);
      //void xor_dib_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount);


      //double pi();

      virtual bool defer_realize(::draw2d::graphics * pgraphics) const;
      virtual bool realize(::draw2d::graphics * pgraphics) const;
      virtual bool unrealize() const;
      virtual bool is_realized() const;


      //virtual bool update_window(::user::interaction_impl * pwnd, ::message::message * pobj);
      //virtual bool print_window(::user::interaction_impl * pwnd, ::message::message * pobj);


      virtual bool blend(point ptDst, ::draw2d::dib * pdibSrc, point ptSrc, class size size, byte bA);

      virtual void tint(::draw2d::dib * pdib, int32_t R, int32_t G, int32_t B) override;

   };


} // namespace draw2d_direct2d




