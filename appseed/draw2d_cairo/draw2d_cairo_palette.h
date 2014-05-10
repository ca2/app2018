#pragma once


namespace draw2d_cairo
{

   class CLASS_DECL_DRAW2D_CAIRO palette :
      virtual public ::draw2d_cairo::object,
      virtual public ::draw2d::palette
   {
   public:

      palette(sp(::base::application) papp);
      virtual ~palette();


//      bool CreatePalette(LPLOGPALETTE lpLogPalette);
//      bool CreateHalftonePalette(::draw2d::graphics * pgraphics);

   // Attributes
/*      operator HPALETTE() const;
      int32_t GetEntryCount();
      UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
            LPPALETTEENTRY lpPaletteColors) const;
      UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
            LPPALETTEENTRY lpPaletteColors);

   // Operations
      void AnimatePalette(UINT nStartIndex, UINT nNumEntries,
            LPPALETTEENTRY lpPaletteColors);
      UINT GetNearestPaletteIndex(COLORREF crColor) const;
//      bool ResizePalette(UINT nNumEntries);

   // Implementation*/
   };


} // namespace draw2d_cairo

