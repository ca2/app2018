#pragma once


namespace draw2d
{


   class CLASS_DECL_AURA bitmap :
      virtual public ::draw2d::object
   {
   public:

      int                  m_iStride;

#ifdef WINDOWSEX

      HBITMAP              m_hbitmapGet;

#endif



      bitmap();
      virtual ~bitmap();

#ifdef DEBUG

      virtual void dump(dump_context & dumpcontext) const;

#endif

      virtual bool CreateBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int32_t stride);
      virtual bool CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap);
      virtual bool CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight);
      virtual bool CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight);
//         virtual bool CreateDIBSection(::draw2d::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void ** ppcolorref, int * piScan, HANDLE hSection, uint32_t offset);
      //       virtual bool HostDIBSection(::draw2d::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void * pcolorref, int iScan, HANDLE hSection, uint32_t offset);
      //     virtual bool CreateDIBitmap(::draw2d::graphics * pgraphics, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);
      virtual bool CreateDIBSection(::draw2d::graphics * pgraphics, int cx, int cy, UINT usage, void ** ppcolorref, int * piScan, HANDLE hSection, uint32_t offset);
      virtual bool HostDIBSection(::draw2d::graphics * pgraphics, int cx, int cy, UINT usage, void * pcolorref, int iScan, HANDLE hSection, uint32_t offset);
      virtual bool CreateDIBitmap(::draw2d::graphics * pgraphics, int cx, int cy, uint32_t flInit, const void *pjBits, UINT iUsage);

      virtual bool attach(void * posdata);
      virtual void * detach();

      virtual class size SetBitmapDimension(int32_t nWidth, int32_t nHeight);
      virtual class size set_size(class size size);

      virtual class size GetBitmapDimension() const;
      virtual class size get_size() const;
      virtual class size size() const;

      // Operations
      virtual uint32_t SetBitmapBits(uint32_t dwCount, const void * lpBits);
      virtual uint32_t GetBitmapBits(uint32_t dwCount, LPVOID lpBits) const;



#ifdef WINDOWSEX

      virtual HBITMAP GetHBITMAP();
      virtual void ReleaseHBITMAP(HBITMAP hbitmap);

      virtual HBITMAP _GetHBITMAP();
      virtual void _ReleaseHBITMAP(HBITMAP hbitmap);

#endif


   };


   typedef smart_pointer < bitmap > bitmap_sp;


} // namespace draw2d




#ifdef WINDOWSEX

CLASS_DECL_AURA HBITMAP CreateHBITMAP(COLORREF * pdata,int stride,int cx,int cy);
CLASS_DECL_AURA HBITMAP CreateHBITMAP2(COLORREF * & pdata, int & stride, int cx, int cy);

#endif
