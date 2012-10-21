#pragma once


namespace visual
{


   class CLASS_DECL_ca glyph
   {
   public:


      UINT   m_ui;

      base_array <point_array, point_array &> m_pointsets;

#ifdef WINDOWSEX

      GLYPHMETRICS   m_gm;

#endif


      glyph();
      virtual ~glyph();


      void DrawGlyph(::ca::graphics * pdc, bool bFill, double dRateX, LPPOINT lppointoffset);

#ifdef WINDOWSEX

      void Initialize(LPTTPOLYGONHEADER lpph, DWORD cbSize, int iFontHiHeight);

#endif

      void GetGlyphRect(int x, int y, LPRECT lpRect);

      void clear();

      glyph & operator = (const glyph & glyph);

      bool operator <= (const glyph & glyph) const;


   };


} // namespace visual



