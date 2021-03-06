#pragma once


namespace visual
{


   class CLASS_DECL_AURA api :
      virtual public ::object
   {
   public:


      api(::aura::application * papp);
      virtual ~api();


      bool open();

      bool close();

      void DrawAndFillBeziers(
         ::draw2d::graphics * pgraphics,
         LPPOINT             lppoints,
         int32_t                  iCount,
         double               dRateX,
         LPPOINT               lppointOffset);

      void DrawBeziers(
         ::draw2d::graphics * pgraphics,
         LPPOINT             lppoints,
         int32_t                  iCount,
         double               dRateX,
         LPPOINT               lppointOffset);

      void DrawAndFillBeziers(
         ::draw2d::graphics * pgraphics,
         array<point_array, point_array &> *
                            lpglyph,
         double               dRateX,
         LPPOINT               lppointOffset);

      void EmbossedTextOut(
         ::draw2d::graphics * pgraphics,
            const RECT &               lpcrect,
            double               dRateX,
            double               dHeight,
            const char *         psz);

      void EmbossedTextOut(
         ::draw2d::graphics * pgraphics,
         const RECT &               lpcrect,
         double               dRateX,
         double               dHeight,
         const char *         psz,
         LPINT                  lpiCharsPositions,
         int32_t                  iCharsPositions,
         int32_t                  iOffset);

      void EmbossedTextOut(
         ::draw2d::graphics * pgraphics,
         const char *      lpcsz,
         int32_t               iLeft,
         int32_t               iTop,
         int32_t               iWidht,
         COLORREF          crText,
         COLORREF          crOutline,
         int32_t               iLen);


      void SimpleTextOut(
         ::draw2d::graphics * pgraphics,
         const RECT &            lpcrect,
         double            dRateX,
         double            dHeight,
         const char *      psz,
         LPINT               lpiCharsPositions,
         int32_t               iCharsPositions,
         int32_t               iOffset);


   };


} // namespace visual



