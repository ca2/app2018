//#include "framework.h"
//#include "windows_window_gdi.h"



window_gdi::window_gdi(::aura::application * papp) :
   object(papp),
   window_graphics(papp),
   window_buffer(papp)
{

   m_hbitmap         = NULL;
   m_pcolorref       = NULL;
   m_hdc             = NULL;
   m_hwnd            = NULL;
   m_pMap


}


window_gdi::~window_gdi()
{

}

CHAR szName[] = "Local\\ca2screen-%d";
void window_gdi::create_window_graphics(int64_t cxParam, int64_t cyParam, int iStrideParam)
{

   destroy_window_graphics();

   if (m_pimpl == NULL)
   {

      return;

   }

   HWND hwnd = (HWND) m_pimpl->get_os_data();

   if (hwnd == NULL)
   {

      return;

   }

   ZERO(m_bitmapinfo);

   int iStride = iStrideParam;

   m_bitmapinfo.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
   m_bitmapinfo.bmiHeader.biWidth         = (LONG)iStrideParam / 4;
   m_bitmapinfo.bmiHeader.biHeight        = (LONG) -cyParam;
   m_bitmapinfo.bmiHeader.biPlanes        = 1;
   m_bitmapinfo.bmiHeader.biBitCount      = 32;
   m_bitmapinfo.bmiHeader.biCompression   = BI_RGB;
   m_bitmapinfo.bmiHeader.biSizeImage     = (LONG) (iStrideParam * cyParam);

   m_hbitmap = CreateDIBSection(NULL, &m_bitmapinfo, DIB_RGB_COLORS, (void **) &m_pcolorref, NULL, 0);

   if (pBuf != NULL)
   {

      UnmapViewOfFile(pBuf);


   }

   if (hMapFile != NULL)
   {
      CloseHandle(hMapFile);

   }

   char szName2[2048];

   sprintf(szName2, szName, (INT_PTR) hwnd);

   hMapFile = CreateFileMapping(
      INVALID_HANDLE_VALUE,    // use paging file
      NULL,                    // default security
      PAGE_READWRITE,          // read/write access
      0,                       // maximum object size (high-order DWORD)
      10240 * 10240 * 4 ,                // maximum object size (low-order DWORD)
      szName2);                 // name of mapping object

   if (hMapFile != NULL)
   {
      pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
         FILE_MAP_ALL_ACCESS, // read/write permission
         0,
         0,
         10240 * 10240 * 4);

      if (pBuf == NULL)
      {

         CloseHandle(hMapFile);

      }
   }



   m_hdc = ::CreateCompatibleDC(NULL);

   m_hbitmapOld = (HBITMAP) ::SelectObject(m_hdc, m_hbitmap);

   m_hdcScreen = ::GetDCEx(hwnd,NULL,DCX_WINDOW);

   if(m_hdcScreen == NULL)
   {

      // If it has failed to get interaction_impl
      // owned device context, try to get
      // a device context from the cache.
      //m_hdcScreen = ::GetDCEx(interaction_impl,NULL,DCX_CACHE | DCX_CLIPSIBLINGS | DCX_WINDOW);
      m_hdcScreen = ::GetDCEx(hwnd,NULL,DCX_CACHE | DCX_WINDOW);

      // If no device context could be retrieved,
      // nothing can be drawn at the screen.
      // The function failed.
      if(m_hdcScreen == NULL)
         return;

      m_bOwnDC = false;

   }
   else
   {

      m_bOwnDC = true;

   }

   window_graphics::create_window_graphics(cxParam, cyParam, iStride);

}


void window_gdi::destroy_window_graphics()
{

   if(m_hdcScreen != NULL)
   {

      ::ReleaseDC(m_pimpl->m_oswindow,m_hdcScreen);

   }

   if(m_hdc != NULL)
   {

      ::SelectObject(m_hdc, m_hbitmapOld);

      ::DeleteDC(m_hdc);

      m_hdc = NULL;

      m_hbitmapOld = NULL;

   }


   if(m_hbitmap != NULL)
   {

      ::DeleteObject(m_hbitmap);

      m_hbitmap = NULL;

   }

   window_graphics::destroy_window_graphics();

}


void window_gdi::update_window(COLORREF * pcolorref,int cxParam,int cyParam,int iStride)
{

   if (cxParam <= 0 || cyParam <= 0)
   {

      return;

   }

   rect rectWindow = m_pimpl->m_rectParentClient;

   bool bLayered = (::GetWindowLong(m_pimpl->m_oswindow,GWL_EXSTYLE) & WS_EX_LAYERED) != 0;

   try
   {

      ::draw2d::copy_colorref(cxParam,cyParam,m_pcolorref,m_iScan,pcolorref,iStride);

   }
   catch(...)
   {

   }

   if (pBuf != NULL)
   {
      try
      {


         int64_t * p = (int64_t *) pBuf;

         *p++ = cxParam;
         *p++ = cyParam;
         *p++ = m_iScan;

         ::draw2d::copy_colorref(cxParam, cyParam, (COLORREF *) p, sizeof(COLORREF) * cxParam, m_pcolorref, m_iScan);

         //memset(p, 0, sizeof(COLORREF) * cxParam *cyParam / 2);

      }
      catch (...)
      {

      }

   }

   ::SetWindowLongPtr(m_pimpl->m_oswindow, GWLP_USERDATA, (LONG) m_hdc);

   if(bLayered)
   {

      POINT pt;

      pt.x = rectWindow.left;

      pt.y = rectWindow.top;

      SIZE sz;

      sz.cx = width(&rectWindow);

      sz.cy = height(&rectWindow);

      POINT ptSrc = { 0 };

      BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

      bool bOk = ::UpdateLayeredWindow(m_pimpl->m_oswindow, m_hdcScreen, &pt, &sz, m_hdc, &ptSrc, RGB(0, 0, 0), &blendPixelFunction, ULW_ALPHA) != FALSE;

   }
   else
   {

      ::BitBlt(m_hdcScreen, 0, 0, m_cx, m_cy,  m_hdc, 0, 0, SRCCOPY);

   }

}


