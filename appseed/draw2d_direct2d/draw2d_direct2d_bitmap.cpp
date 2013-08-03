#include "framework.h"


#undef new


namespace draw2d_direct2d
{


   bitmap::bitmap(::ca2::application * papp) :
      ca2(papp)
   { 


   }

   bitmap::~bitmap()
   { 


   }

   bool bitmap::CreateBitmap(::draw2d::graphics * pgraphics, int cx, int cy, UINT nPlanes, UINT nBitcount, const void * lpBits, int stride)
   { 

      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = cx;
      size.height = cy;

      D2D1_BITMAP_PROPERTIES props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
      props.dpiX = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      props.dpiY = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      //props.colorContext = NULL;

      //if(ppdata != NULL)
      {
        // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(COLORREF), props, &m_pbitmap);
      }
      //else
      //{
      HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, lpBits, stride, props, &m_pbitmap);

      //}

      if(FAILED(hr))
         return false;

      zero(&m_map, sizeof(m_map));
  //    m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);
//
      //if(ppdata != NULL)
        // *ppdata = (COLORREF *) m_map.bits;


      return true;

   }

   bool bitmap::CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap)
   { 
      return FALSE;
   }


   bool bitmap::CreateDIBSection(::draw2d::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, int * stride, HANDLE hSection, uint32_t offset)
   { 

      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = abs(lpbmi->bmiHeader.biWidth);
      size.height = abs(lpbmi->bmiHeader.biHeight);

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
      props.dpiX = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      props.dpiY = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      props.colorContext = NULL;

      //m_memory.allocate(size.width * size.height * sizeof(COLORREF));
      HRESULT hr;
      //if(ppvBits == NULL || *ppvBits == NULL)
      {
        // hr = METROWIN_DC(pgraphics)->m_pdc->CreateBitmap(size, NULL, 0, props, &m_pbitmap);
      }
      //else
      {
         //hr = METROWIN_DC(pgraphics)->m_pdevicecontext->CreateBitmap(size, m_memory.get_data(), size.width * sizeof(COLORREF), props, &m_pbitmap1);
      }
      hr = ((ID2D1DeviceContext *) pgraphics->get_os_data())->CreateBitmap(size, NULL,  0, props, &m_pbitmap1);

      if(FAILED(hr) || m_pbitmap1 == NULL)
         return FALSE;

      m_pbitmap = m_pbitmap1;


      return true;

   }


   bool bitmap::CreateDIBitmap(::draw2d::graphics * pgraphics, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage)
   { 
      return FALSE;
   }


   uint32_t bitmap::SetBitmapBits(uint32_t dwCount, const void * lpBits)
   { 

      //return ::SetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;

   }
   uint32_t bitmap::GetBitmapBits(uint32_t dwCount, LPVOID lpBits) const
   { 
      //return ::GetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   }
   bool bitmap::LoadBitmap(const char * lpszResourceName)
   { 
      //   return Attach(::LoadBitmap(::ca2::FindResourceHandle(
      //lpszResourceName, RT_BITMAP), lpszResourceName));
      return FALSE;

   }

   size bitmap::SetBitmapDimension(int nWidth, int nHeight)
   {


      throw not_implemented(get_app());

      //SIZE size;
      //VERIFY(::SetBitmapDimensionEx((HBITMAP)get_handle(), nWidth, nHeight, &size));
      //return size;
      return class ::size(0, 0);

   }

   size bitmap::GetBitmapDimension() const
   {

      if(m_pbitmap == NULL)
         return class size(0, 0);

      D2D1_SIZE_U size = m_pbitmap->GetPixelSize();

      return ::size(size.width, size.height);

   }

   bool bitmap::LoadBitmap(UINT nIDResource)
   { 
      //return Attach(::LoadBitmap(::ca2::FindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource))); 
      return FALSE;
   }
   bool bitmap::LoadOEMBitmap(UINT nIDBitmap)
   { 
      //return Attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap))); 
      return FALSE;
   }
   bool bitmap::CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight)
   {


      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = nWidth;
      size.height = nHeight;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
      props.dpiX = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      props.dpiY = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      //if(ppdata != NULL)
      {
        // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(COLORREF), props, &m_pbitmap);
      }
      //else
//      ID2D1Bitmap1 * pbitmap1;
      {
          ((ID2D1DeviceContext *) pgraphics->get_os_data())->CreateBitmap(size, NULL, size.width * sizeof(COLORREF), props, &m_pbitmap1);
      }
      m_pbitmap = m_pbitmap1;

      //if(ppdata != NULL)
        // *ppdata = (COLORREF *) m_map.bits;


      return true;

/*
      if(m_pdata != NULL)
      {
         ca2_free(m_pdata);
         m_pdata = NULL;
      }

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }

      m_pbitmap = new ::Gdiplus::Bitmap(nWidth, nHeight, Gdiplus::PixelOffsetModeHighQuality);

      return TRUE;*/

   }
   bool bitmap::CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight)
   { 


      if(m_pbitmap != NULL)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = nWidth;
      size.height = nHeight;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
      props.dpiX = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      props.dpiY = ::Windows::Graphics::Display::DisplayProperties::LogicalDpi;
      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      //if(ppdata != NULL)
      {
        // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(COLORREF), props, &m_pbitmap);
      }
      //else
      {
          ((ID2D1DeviceContext *) pgraphics->get_os_data())->CreateBitmap(size, NULL, size.width * sizeof(COLORREF), &props, &m_pbitmap1);
      }
      m_pbitmap = m_pbitmap1;
      //m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      //if(ppdata != NULL)
        // *ppdata = (COLORREF *) m_map.bits;


      return true;

   }

/*
   int bitmap::GetBitmap(BITMAP* pBitMap)
   { 
      //   ASSERT(get_handle() != NULL);
      // return ::GetObject(get_handle(), sizeof(BITMAP), pBitMap); 
      return 0;
   }
   */

   /////////////////////////////////////////////////////////////////////////////


   void bitmap::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

      /*         if (get_handle() == NULL)
      return;

      if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_BITMAP)
      {
      // not a valid object
      dumpcontext << "has ILLEGAL HBITMAP!";
      return;
      }*/

      /*BITMAP bm;
      VERIFY(GetObject(sizeof(bm), &bm));
      dumpcontext << "bm.bmType = " << bm.bmType;
      dumpcontext << "\nbm.bmHeight = " << bm.bmHeight;
      dumpcontext << "\nbm.bmWidth = " << bm.bmWidth;
      dumpcontext << "\nbm.bmWidthBytes = " << bm.bmWidthBytes;
      dumpcontext << "\nbm.bmPlanes = " << bm.bmPlanes;
      dumpcontext << "\nbm.bmBitsPixel = " << bm.bmBitsPixel;
      */
      dumpcontext << "\n";
   }




   void * bitmap::get_os_data() const
   {

      return (void *) m_pbitmap.Get();

   }

   void * bitmap::get_os_data_ex(int i) const
   {

      if(i == data_bitmap)
      {

         return (void *) m_pbitmap.Get();

      }
      else if(i == data_bitmap1)
      {

         return (void *) m_pbitmap1.Get();

      }

      return ::draw2d_direct2d::object::get_os_data_ex(i);

   }

   bool bitmap::attach(void * hbitmap)
   {

      if(m_pbitmap != NULL)
      {

         destroy();

      }

      m_pbitmap = (ID2D1Bitmap *) hbitmap;

      m_pbitmap.As(&m_pbitmap1);

      return true;

   }

   void * bitmap::detach()
   {

      m_pbitmap1 = nullptr;

      return m_pbitmap.Detach();

   }


   bool bitmap::destroy()
   {

      m_pbitmap   = nullptr;

      m_pbitmap1  = nullptr;

      return true;

   }

   


} // namespace draw2d_direct2d