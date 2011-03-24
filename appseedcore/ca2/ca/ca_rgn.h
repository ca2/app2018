#pragma once

namespace ca
{

   class CLASS_DECL_ca rgn : 
      virtual public graphics_object
   {
   public:

      virtual BOOL CreateRectRgn(int x1, int y1, int x2, int y2);
      virtual BOOL CreateRectRgnIndirect(LPCRECT lpRect);
      virtual BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2);
      virtual BOOL CreateEllipticRgnIndirect(LPCRECT lpRect);
      virtual BOOL CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
      virtual BOOL CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
            int nCount, int nPolyFillMode);
      virtual BOOL CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
      virtual BOOL CreateFromPath(::ca::graphics * pgraphics);
      virtual BOOL CreateFromData(const XFORM* lpXForm, int nCount,
         const RGNDATA* pRgnData);

   // Operations
      virtual void SetRectRgn(int x1, int y1, int x2, int y2);
      virtual void SetRectRgn(LPCRECT lpRect);
      virtual int CombineRgn(const ::ca::rgn* pRgn1, const ::ca::rgn* pRgn2, int nCombineMode);
      virtual int CopyRgn(const ::ca::rgn* pRgnSrc);
      virtual BOOL EqualRgn(const ::ca::rgn* pRgn) const;
      virtual int OffsetRgn(int x, int y);
      virtual int OffsetRgn(POINT point);
      virtual int GetRgnBox(LPRECT lpRect) const;
      virtual int GetRgnBox(rect64 * lpRect) const;
      virtual BOOL PtInRegion(int x, int y) const;
      virtual BOOL PtInRegion(POINT point) const;
      virtual BOOL RectInRegion(LPCRECT lpRect) const;
      virtual int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;

   };

   typedef smart_pointer < rgn > rgn_sp;

} // namespace ca