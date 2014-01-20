#include "framework.h"


namespace draw2d_xlib
{

   bitmap::bitmap(sp(base_application) papp) :
      element(papp),
      m_ui(papp)
   {

      m_pixmap = None;

   }

   bitmap::~bitmap()
   {

      if(m_pixmap != NULL)
      {

         destroy();

      }

   }

   bool bitmap::CreateBitmap(::draw2d::graphics * pdc, int32_t cx, int32_t cy, UINT nPlanes, UINT nBitcount, const void * pdata, int32_t iStrideParam)
   {

      cy = abs(cy);
      cx = abs(cx);

      if(nPlanes != 1 || nBitcount != 32)
      {

         throw not_implemented(get_app());

      }

      if(m_pixmap != NULL)
      {

         destroy();

      }

      int32_t iStride = cx * 4;

      m_mem.allocate(iStride * cy);

      if(iStrideParam != iStride)
      {

         int32_t iW = cx * 4;

         for(int32_t i = 0; i < cy; i++)
         {

            memcpy(&((byte *) m_mem.get_data())[iStride * i], &((byte *) pdata)[iStrideParam * i], iW);

         }

      }
      else
      {
         memcpy(m_mem.get_data(), pdata, iStride * cy);
      }

      if(m_ui.m_window == NULL)
      {

         rect r(100, 100, 200, 200);

         m_ui.create_window(r);

      }

      if(m_ui.m_window == NULL)
      {

         return false;

      }

      ::oswindow window = m_ui.m_window;

      m_pixmap = XCreatePixmap(window->display(), window->window(), cx, cy, window->m_iDepth);

      if(m_pixmap == None)
      {

         return false;

      }

      m_size.cx = cx;

      m_size.cy = cy;

      return true;

   }


   bool bitmap::CreateBitmapIndirect(::draw2d::graphics * pdc, LPBITMAP lpBitmap)
   {

      return false;

   }


   bool bitmap::CreateDIBSection(::draw2d::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void ** ppdata, int * pstride, HANDLE hSection, uint32_t offset)
   {

      int cy = abs(lpbmi->bmiHeader.biHeight);

      int cx = abs(lpbmi->bmiHeader.biWidth);

      if(lpbmi->bmiHeader.biPlanes != 1 || lpbmi->bmiHeader.biBitCount != 32)
      {

         throw not_implemented(get_app());

      }

      if(m_pixmap != NULL)
      {

         destroy();

      }

      int32_t iStride = cx * 4;

      m_mem.allocate(iStride * cy);

      if(*ppdata != NULL)
      {

         if(cx * 4 != iStride)
         {

            int32_t iW = cx * 4;

            for(int32_t i = 0; i < cy; i++)
            {

               memcpy(&((byte *) m_mem.get_data())[iStride * i], &((byte *) *ppdata)[iW * i], iW);

            }

         }
         else
         {
            memcpy(m_mem.get_data(), *ppdata, iStride * cy);
         }

      }

      if(m_ui.m_window == NULL)
      {

         rect r(100, 100, 200, 200);

         m_ui.create_window(r);

      }

      if(m_ui.m_window == NULL)
      {

         return false;

      }

      ::oswindow window = m_ui.m_window;

      m_pixmap = XCreatePixmap(window->display(), window->window(), cx, cy, window->m_iDepth);

      if(m_pixmap == None)
      {

         return false;

      }


      if(ppdata != NULL)
      {

         *ppdata = (COLORREF *) m_mem.get_data();

      }

      if(pstride != NULL)
      {

         *pstride = iStride;

      }

      m_size.cx = cx;
      m_size.cy = cy;



      return true;

   }


   bool bitmap::CreateDIBitmap(::draw2d::graphics * pdc, const BITMAPINFOHEADER *pbmih, DWORD flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage)
   {
      return FALSE;
   }


   DWORD bitmap::SetBitmapBits(DWORD dwCount, const void * lpBits)
   {

      //return ::SetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;

   }
   DWORD bitmap::GetBitmapBits(DWORD dwCount, LPVOID lpBits) const
   {
      //return ::GetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   }
   bool bitmap::LoadBitmap(const char * lpszResourceName)
   {
   //   return Attach(::LoadBitmap(::core::FindResourceHandle(
   //lpszResourceName, RT_BITMAP), lpszResourceName));
      return FALSE;

   }

   size bitmap::SetBitmapDimension(int32_t nWidth, int32_t nHeight)
   {


      throw not_implemented(get_app());

      //SIZE size;
      //VERIFY(::SetBitmapDimensionEx((HBITMAP)get_handle(), nWidth, nHeight, &size));
      //return size;
      return ::size(0, 0);

   }

   size bitmap::GetBitmapDimension() const
   {

      if(m_pixmap == NULL)
         return ::size(0, 0);

      return m_size;

   }

   bool bitmap::LoadBitmap(UINT nIDResource)
   {
      //return Attach(::LoadBitmap(::core::FindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource)));
      return FALSE;
   }
   bool bitmap::LoadOEMBitmap(UINT nIDBitmap)
   {
      //return Attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap)));
      return FALSE;
   }
   bool bitmap::CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int32_t cx, int32_t cy)
   {

      m_mem.allocate(cx * cy * 4);

      memset(m_mem.get_data(), 0, m_mem.get_size());

      if(!CreateBitmap(pgraphics, cx, cy, 1, 32, (COLORREF *) m_mem.get_data(), cx * sizeof(COLORREF)))
      {

         m_mem.allocate(0);

         return false;

      }


      return true;

/*
      if(m_pdata != NULL)
      {
         memory_free(m_pdata);
         m_pdata = NULL;
      }

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }

      m_pbitmap = new ::Gdiplus::Bitmap(nWidth, nHeight, Gdiplus::PixelOffsetModeHighQuality);
*/

//      return TRUE;

   }
   bool bitmap::CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight)
   {

      return CreateCompatibleBitmap(pgraphics, nWidth, nHeight);

   }


   int32_t bitmap::GetBitmap(BITMAP* pBitMap)
   {
   //   ASSERT(get_handle() != NULL);
     // return ::GetObject(get_handle(), sizeof(BITMAP), pBitMap);
      return 0;
   }


      /////////////////////////////////////////////////////////////////////////////


      void bitmap::dump(dump_context & dumpcontext) const
      {
         object::dump(dumpcontext);

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

      return (void *) m_pixmap;

   }

/*
   void get_surface_size (xlib_surface_t * psurface, LONG * plongWidth, LONG * plongHeight)
	{

      if(plongWidth != NULL)
      {

         *plongWidth = xlib_image_surface_get_width(psurface);

      }

      if(plongHeight != NULL)
      {

         *plongHeight = xlib_image_surface_get_height(psurface);

      }


	}*/


   bool bitmap::attach(void * psurface)
   {

      if(m_pixmap != 0)
      {

         destroy();

         m_pixmap = None;

      }

//      m_pixmap = (xlib_surface_t *) psurface;

  //    get_surface_size((xlib_surface_t *) psurface, &m_size.cx, &m_size.cy);

      return true;

   }


   bool bitmap::destroy()
   {

      if(m_pixmap == NULL)
         return true;

//      if(m_pixmap == xlib_keep::g_xlibsurface)
  //    {
    //     printf("123");
      //}

//      xlib_surface_destroy(m_pixmap);

      XFreePixmap(m_ui.m_window->display(), m_pixmap);

      m_pixmap = NULL;

      return true;

   }


} // namespace draw2d_xlib






