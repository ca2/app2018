#pragma once


namespace visual
{


   class CLASS_DECL_AURA graphics_extension :
      virtual public ::object
   {
   public:


      graphics_extension(::aura::application * papp);
      virtual ~graphics_extension();


      void GetTextExtent(::draw2d::graphics * pgraphics, const char * lpcwsz, array < size > & sizea);
      void GetTextExtent(::draw2d::graphics * pgraphics, const char * lpcwsz, size & size);
      void GetTextExtent(::draw2d::graphics * pgraphics, const char * lpcwsz, strsize iCount, size & size);

      bool text_out(::draw2d::graphics * pgraphics, int32_t x, int32_t y, const char * lpcwsz, strsize iCount,size & s);

      //void FillSolidRect(HDC hdc, const RECT64 * lpRect, COLORREF clr);
      //void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr);
      //void FillSolidRect(HDC hdc, int32_t x, int32_t y, int32_t cx, int32_t cy, COLORREF clr);

      //static int32_t _FindPrefixV1(const unichar * lpcsz);

   };


} // namespace visual



