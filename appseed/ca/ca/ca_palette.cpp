#include "framework.h"


namespace ca
{


   bool palette::CreatePalette(LPLOGPALETTE lpLogPalette)
   {
      UNREFERENCED_PARAMETER(lpLogPalette);
      throw interface_only_exception(get_app());
   }

   bool palette::CreateHalftonePalette(::ca::graphics * pgraphics)
   {
      UNREFERENCED_PARAMETER(pgraphics);
      throw interface_only_exception(get_app());
   }

   UINT palette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
   {
      UNREFERENCED_PARAMETER(nStartIndex);
      UNREFERENCED_PARAMETER(nNumEntries);
      UNREFERENCED_PARAMETER(lpPaletteColors);
      throw interface_only_exception(get_app());
   }

   UINT palette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
   {
      UNREFERENCED_PARAMETER(nStartIndex);
      UNREFERENCED_PARAMETER(nNumEntries);
      UNREFERENCED_PARAMETER(lpPaletteColors);
      throw interface_only_exception(get_app());
   }

   void palette::AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
   {
      UNREFERENCED_PARAMETER(nStartIndex);
      UNREFERENCED_PARAMETER(nNumEntries);
      UNREFERENCED_PARAMETER(lpPaletteColors);
      throw interface_only_exception(get_app());
   }

   UINT palette::GetNearestPaletteIndex(COLORREF crColor) const
   {
      UNREFERENCED_PARAMETER(crColor);
      throw interface_only_exception(get_app());
   }

   bool palette::ResizePalette(UINT nNumEntries)
   {
      UNREFERENCED_PARAMETER(nNumEntries);
      throw interface_only_exception(get_app());
   }

   int palette::GetEntryCount()
   {
      throw interface_only_exception(get_app());
   }


} // namespace ca








