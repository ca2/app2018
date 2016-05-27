#include "framework.h"

void resizeBilinear(memory & m, int w2, int h2, int * pixels, int w, int h);

#ifdef WINDOWS
#include <windowsx.h>
#endif

extern WNDCLASSEX g_wcl;


#ifdef LINUX
#define WIDTH 3200
#define HEIGHT 1800

const int sbAttrib[] = { GLX_DOUBLEBUFFER, 0, GLX_RED_SIZE, 1,GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,GLX_ALPHA_SIZE, 1, GLX_DEPTH_SIZE, 16,None };
int pbAttrib[] = { GLX_PBUFFER_WIDTH, WIDTH,GLX_PBUFFER_HEIGHT, HEIGHT,GLX_PRESERVED_CONTENTS, True,None };
#endif

//extern CLASS_DECL_AXIS thread_int_ptr < DWORD_PTR > t_time1;
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.00f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.00f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.00f };
//GLfloat LightPosition2[] = { -5.0f, -5.0f, 32.0f, 1.00f };

#ifdef WINDOWS
typedef BOOL(WINAPI * PFNWGLDESTROYPBUFFERARBPROC) (HPBUFFERARB hPbuffer);
typedef BOOL(WINAPI * PFNWGLQUERYPBUFFERARBPROC) (HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
typedef HDC(WINAPI * PFNWGLGETPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer);
typedef HPBUFFERARB(WINAPI * PFNWGLCREATEPBUFFERARBPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
typedef int (WINAPI * PFNWGLRELEASEPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer, HDC hDC);

PFNWGLDESTROYPBUFFERARBPROC                       wglDestroyPbufferARB;
PFNWGLQUERYPBUFFERARBPROC                         wglQueryPbufferARB;
PFNWGLGETPBUFFERDCARBPROC                         wglGetPbufferDCARB;
PFNWGLCREATEPBUFFERARBPROC                        wglCreatePbufferARB;
PFNWGLRELEASEPBUFFERDCARBPROC                     wglReleasePbufferDCARB;


typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef BOOL(WINAPI * PFNWGLGETPIXELFORMATATTRIBFVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL(WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);

PFNWGLCHOOSEPIXELFORMATARBPROC                    wglChoosePixelFormatARB;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC               wglGetPixelFormatAttribfvARB;
PFNWGLGETPIXELFORMATATTRIBIVARBPROC               wglGetPixelFormatAttribivARB;


#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

#endif

#include <assert.h>
#ifdef WINDOWS
#include <tchar.h>
#endif // WINDOWS
#ifdef  assert
#define verify(expr) if(!expr) assert(0)
#else verify(expr) expr
#endif

namespace draw2d_gl2d
{


   bitmap::bitmap(::aura::application * papp) :
      ::object(papp)
   { 

      m_bTexture = false;
      m_bPBuffer = false;
      m_bFlashed = false;

      m_memOut.m_bAligned = true;
      m_memIn.m_bAligned = true;

      //m_pbitmap   = NULL;
      m_iStride   = 0;

   }

   bitmap::~bitmap()
   { 

      //::aura::del(m_pbitmap);

   }

   bool bitmap::CreateBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int32_t stride)
   { 

      UNREFERENCED_PARAMETER(pgraphics);

      //::aura::del(m_pbitmap);

      //m_pbitmap = new ::plusplus::Bitmap(nWidth, nHeight, plusplus::PixelOffsetModeHighQuality);

      return TRUE;
   
   }

   bool bitmap::CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap)
   { 

      UNREFERENCED_PARAMETER(pgraphics);

      return FALSE;
   }


   bool bitmap::CreateDIBSection(::draw2d::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, int * stride, HANDLE hSection, uint32_t offset)
   { 

      UNREFERENCED_PARAMETER(pgraphics);

//      ::aura::del(m_pbitmap);

      m_sizeOut.cx = lpbmi->bmiHeader.biWidth;
      m_sizeOut.cy = -lpbmi->bmiHeader.biHeight;

      m_iStride = 4 * lpbmi->bmiHeader.biWidth;

      m_memOut.allocate(abs(m_iStride * lpbmi->bmiHeader.biHeight));

      if(m_memOut.get_data() == NULL)
         return false;

      //m_pbitmap = new plusplus::Bitmap(abs(lpbmi->bmiHeader.biWidth), abs(lpbmi->bmiHeader.biHeight),m_iStride, PixelFormat32bppPARGB, (BYTE *)m_mem.get_data());

      //if(m_pbitmap == NULL)
      //{
      //   return FALSE;
      //}

      if(ppvBits != NULL)
      {
         *ppvBits = m_memOut.get_data();
      }

      if(stride != NULL)
      {
         *stride = lpbmi->bmiHeader.biWidth * sizeof(COLORREF);
      }

      return TRUE;

   }


   bool bitmap::CreateDIBitmap(::draw2d::graphics * pdc, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage)
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
   //   return attach(::LoadBitmap(::core::FindResourceHandle(
   //lpszResourceName, RT_BITMAP), lpszResourceName));
      return FALSE;
   
   }
   
   size bitmap::SetBitmapDimension(int32_t nWidth, int32_t nHeight)
   {


      ::exception::throw_not_implemented(get_app());

      //SIZE size;
      //VERIFY(::SetBitmapDimensionEx((HBITMAP)get_handle(), nWidth, nHeight, &size));
      //return size;
      return class ::size(0, 0);

   }

   size bitmap::GetBitmapDimension() const
   {
      
      //if(m_pbitmap == NULL)
      //   return class size(0, 0);

      //return class size(m_pbitmap->GetWidth(), m_pbitmap->GetHeight());

      return m_sizeOut;

   }

   bool bitmap::LoadBitmap(UINT nIDResource)
   { 
      //return attach(::LoadBitmap(::core::FindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource))); 
      return FALSE;
   }
   bool bitmap::LoadOEMBitmap(UINT nIDBitmap)
   { 
      //return attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap))); 
      return FALSE;
   }
   bool bitmap::CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight)
   {

//      ::aura::del(m_pbitmap);

  //    m_pbitmap = new ::plusplus::Bitmap(nWidth, nHeight, plusplus::PixelOffsetModeHighQuality);

      return TRUE;
   
   }
   bool bitmap::CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight)
   { 

//      ::aura::del(m_pbitmap);

  //    m_pbitmap = new ::plusplus::Bitmap(nWidth, nHeight, plusplus::PixelOffsetModeHighQuality);

      return TRUE;
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
      
//      return (void *) (plusplus::Bitmap *) m_pbitmap;
      return m_memOut.get_data();

   }

   bool bitmap::attach(void * posdata)
   {
      //::aura::del(m_pbitmap);
      //
      //m_pbitmap = (plusplus::Bitmap *) posdata;


      return true;

   }

   void * bitmap::detach()
   {

      //plusplus::Bitmap * pbitmap = m_pbitmap;

      //m_pbitmap = NULL;

//      return m_pbitmap;
      return NULL;

   }


   HBITMAP bitmap::_GetHBITMAP()
   {

      //if(m_pbitmap == NULL)
      //   return NULL;
      //
      //HBITMAP hbitmap = NULL;

      //plusplus::Color colorBk(0,0,0,0);

      //m_pbitmap->GetHBITMAP(colorBk,&hbitmap);

      //return hbitmap;
      return NULL;

   }


   void bitmap::_ReleaseHBITMAP(HBITMAP hbitmap)
   {

      ::DeleteObject(hbitmap);
      

   }

   void bitmap::destroy_bitmap()
   {

      m_bTexture = false;
      m_bPBuffer = false;

      if (g_hDC)
      {
         if (g_hRC)
         {
            wglMakeCurrent(g_hDC, 0);
            wglDeleteContext(g_hRC);
            g_hRC = 0;
         }

         DeleteDC(g_hDC);
         g_hDC = 0;
      }


   }

   void bitmap::create_texture(int iResampleQuality)
   {

      m_sizeIn.cx = 1;
      m_sizeIn.cy = 1;

      while (m_sizeIn.cx < m_sizeOut.cx)
      {

         m_sizeIn.cx *= 2;

      }

      while(m_sizeIn.cy < m_sizeOut.cy)
      {

         m_sizeIn.cy *= 2;

      }


      resizeBilinear(m_memIn, m_sizeIn.cx, m_sizeIn.cy, (int*)m_memOut.get_data(), m_sizeOut.cx, m_sizeOut.cy);
      
      glGenTextures(1, &m_texture);

      GLenum e = glGetError();

      // Create Nearest Filtered Texture
      glBindTexture(GL_TEXTURE_2D, m_texture);
      e = glGetError();
      if (iResampleQuality == 2)
      {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         e = glGetError();
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
         e = glGetError();
      }
      else if (iResampleQuality == 1)
      {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         e = glGetError();
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         e = glGetError();
      }
      else
      {

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         e = glGetError();
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         e = glGetError();

      }
      
      glTexImage2D(GL_TEXTURE_2D, 0, 4, m_sizeIn.cx, m_sizeIn.cy, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_memIn.get_data());
      e = glGetError();



   }

#ifdef WINDOWS
   LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
   {
      static TCHAR szBuffer[32] = { 0 };

      switch (msg)
      {
      case WM_CREATE:
         //         timeBeginPeriod(1);
         //SetTimer(hWnd, 1, 1000, 0);
         return 0;

      case WM_DESTROY:
         //KillTimer(hWnd, 1);
         //       timeEndPeriod(1);
         //PostQuitMessage(0);
         return 0;

      case WM_KEYDOWN:
         //if (wParam == VK_ESCAPE)
         {
            //SendMessage(hWnd, WM_CLOSE, 0, 0);
           // return 0;
         }
         break;

      case WM_NCHITTEST:
         return HTCAPTION;   // allows dragging of the window

      case WM_TIMER:
         //         _stprintf(szBuffer, _TEXT("%d FPS"), g_frames);
         //SetWindowText(hWnd, szBuffer);
         //         g_frames = 0;
         return 0;

      default:
         break;
      }

      return DefWindowProc(hWnd, msg, wParam, lParam);
   }
#endif


   bool bitmap::flash()
   {

      if (!m_bPBuffer && m_sizeOut.area() > 0)
      {

         MSG msg;
          

            g_hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, g_wcl.lpszClassName,
               _T("GL Layered Window Demo"), 0, 0, 0, 0,
               0, 0, 0, g_wcl.hInstance, 0);

            if (g_hWnd == NULL)
            {

               return false;

            }

            bool bLayout = true;

            while (true)
            {
#ifdef WINDOWS
               if (PeekMessage(&msg, g_hWnd, 0, 0, PM_REMOVE))
               {


                  TranslateMessage(&msg);
                  DispatchMessage(&msg);

                  if (msg.message == WM_NCDESTROY)
                     break;

               }
               else
#else
               Sleep(5);
#endif
               {
                  if (bLayout)
                  {
                     bLayout = false;
                     if (Init())
                     {
                        //initSC();
                        //resizeSC();
                        if (m_sizeOut.area() > 0)
                        {
                           //if (height == 0)										// Prevent A Divide By Zero By
                           //{
                           //   height = 1;										// Making Height Equal One
                           //}

                           glViewport(0, 0, m_sizeOut.cx, m_sizeOut.cy);						// Reset The Current Viewport

                           glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
                           glLoadIdentity();									// Reset The Projection Matrix

                                                                     // Calculate The Aspect Ratio Of The Window
                           gluPerspective(45.0f, (GLfloat)m_sizeOut.cx / (GLfloat)m_sizeOut.cy, 0.1f, 100.0f);

                           glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
                           glLoadIdentity();									// Reset The Modelview Matrix
                           glTranslatef(-1.0, -1.0, 0);
                           glScalef(2.0/(float)m_sizeOut.cx, 2.0/(float)m_sizeOut.cy, 0);

                        }

                     }
                     break;
                  }

               }
            }

            g_hWnd = NULL;


         m_bPBuffer = true;

      }

      wglMakeCurrent(g_hPBufferDC, g_hPBufferRC);
      m_bFlashed = true;

   }

   void bitmap::defer_reveal()
   {

      if (m_bFlashed)
      {

         m_bFlashed = false;

         //wglMakeCurrent(g_hDC, g_hRC);

         wglMakeCurrent(g_hPBufferDC, g_hPBufferRC);
         GLenum e = glGetError();

//         glFlush();

         //m_mem.allocate(cxDIB * cyDIB * 4);
         //m_mem.zero();
         glPixelStorei(GL_PACK_ALIGNMENT, 1);
         e = glGetError();

         COLORREF * pdata = (COLORREF *) m_memOut.get_data();

         glReadPixels(0, 0, m_sizeOut.cx, m_sizeOut.cy, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pdata);
          e = glGetError();

         output_debug_string("error " + ::str::from((int)e));

      }

   }


   bool bitmap::InitGL()
   {

#ifdef WINDOWS
      // Even though we aren't going to be rendering the scene to the window
      // we still need to create a dummy rendering context in order to load the
      // pbuffer extensions and to create our pbuffer.

      PIXELFORMATDESCRIPTOR pfd = { 0 };

      // Don't bother with anything fancy here. This is just a dummy rendering
      // context so just ask for the bare minimum.
      pfd.nSize = sizeof(pfd);
      pfd.nVersion = 1;
      pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 32;
      pfd.cDepthBits = 32;
      pfd.iLayerType = PFD_MAIN_PLANE;

      if (!(g_hDC = GetDC(g_hWnd)))
         return false;

      int pf = ChoosePixelFormat(g_hDC, &pfd);

      if (!SetPixelFormat(g_hDC, pf, &pfd))
         return false;

      if (!(g_hRC = wglCreateContext(g_hDC)))
         return false;

      if (!wglMakeCurrent(g_hDC, g_hRC))
         return false;

      if (!InitGLExtensions())
         return false;

      if (!InitPBuffer())
         return false;

      // Deactivate the dummy rendering context now that the pbuffer is created.
      wglMakeCurrent(g_hDC, 0);
      ::DeleteDC(g_hDC);
      g_hDC = 0;



      return true;
#else



      if (!(dpy = XOpenDisplay(NULL)))
      {
         fprintf(stderr, "could not open display");
         return false;
      }

      fprintf(stdout, "Info:GLX Extensions:%s\n", glXQueryExtensionsString(dpy, DefaultScreen(dpy)));

      if (!glXQueryExtension(dpy, &dummy, &dummy))
      {
         fprintf(stderr, "Error:GLX extensions not supported");
         return false;
      }

      fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), sbAttrib, &nElements);
      fprintf(stdout, "Info:Number of FBConfigs: %d\n", nElements);
      if (nElements == 0)
      {
         fprintf(stderr, "Error: No valid framebuffers configurations found\n");
         return false;
      }
      /*
      * For simplicities sake, select the first. This however may not be the right one
      * for the purpose of an example this will suffice.
      */
      vi = glXGetVisualFromFBConfig(dpy, fbc[0]);

      if (!(WinCtx = glXCreateContext(dpy, vi,
         None, /* no sharing of display lists */
         True /* direct rendering if possible */
         )))
      {
         fprintf(stderr, "Cound not create rendering context\n");
         return false;
      }

      PBuffer = glXCreatePbuffer(dpy, fbc[0], pbAttrib);
      PBufferCtx = glXCreateNewContext(dpy, fbc[0], GLX_RGBA_TYPE, 0, GL_TRUE);

      cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
      swa.colormap = cmap;
      swa.border_pixel = 0;
      swa.event_mask = ExposureMask | ButtonPressMask | StructureNotifyMask;
      win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, WIDTH, HEIGHT,
         0, vi->depth, InputOutput, vi->visual,
         CWBorderPixel | CWColormap | CWEventMask,
         &swa);

      glXMakeContextCurrent(dpy, PBuffer, PBuffer, PBufferCtx);

#endif
   }

   bool bitmap::Init()
   {
      if (!InitGL())
      {
         Cleanup();
         return false;
      }

      //if (!d->create(cxDIB, cyDIB))

      //   //      if (!ImageCreate(&g_image, cxDIB, cyDIB))
      //{
      //   Cleanup();
      //   return false;
      //}
      ::DestroyWindow(g_hWnd);
      return true;
   }


   bool bitmap::InitGLExtensions()
   {

#ifdef WINDOWS
#define GPA(x) wglGetProcAddress(x)

      // WGL_ARB_pbuffer.
      wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)GPA("wglDestroyPbufferARB");
      wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)GPA("wglQueryPbufferARB");
      wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)GPA("wglGetPbufferDCARB");
      wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)GPA("wglCreatePbufferARB");
      wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)GPA("wglReleasePbufferDCARB");

      // WGL_ARB_pixel_format.
      wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)GPA("wglChoosePixelFormatARB");
      wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)GPA("wglGetPixelFormatAttribfvARB");
      wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)GPA("wglGetPixelFormatAttribivARB");

#undef GPA

      if (!wglDestroyPbufferARB || !wglQueryPbufferARB || !wglGetPbufferDCARB || !wglCreatePbufferARB || !wglReleasePbufferDCARB)
      {
         MessageBox(0, _T("Required extension WGL_ARB_pbuffer not supported"), _T("Error"), MB_ICONSTOP);
         return false;
      }

      if (!wglChoosePixelFormatARB || !wglGetPixelFormatAttribfvARB || !wglGetPixelFormatAttribivARB)
      {
         MessageBox(0, _T("Required extension WGL_ARB_pixel_format not supported"), _T("Error"), MB_ICONSTOP);
         return false;
      }
#endif
      return true;
   }

   bool bitmap::InitPBuffer()
   {
#ifdef WINDOWS
      // Create a pbuffer for off-screen rendering. Notice that since we aren't
      // going to be using the pbuffer for dynamic texturing (i.e., using the
      // pbuffer containing our rendered scene as a texture) we don't need to
      // request for WGL_BIND_TO_TEXTURE_RGBA_ARB support in the attribute list.

      int attribList[] =
      {
         WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // allow rendering to the pbuffer
         WGL_SUPPORT_OPENGL_ARB,  TRUE,      // associate with OpenGL
         WGL_DOUBLE_BUFFER_ARB,   FALSE,     // single buffered
         WGL_RED_BITS_ARB,   8,              // minimum 8-bits for red channel
         WGL_GREEN_BITS_ARB, 8,              // minimum 8-bits for green channel
         WGL_BLUE_BITS_ARB, 8,              // minimum 8-bits for blue channel
         WGL_ALPHA_BITS_ARB, 8,              // minimum 8-bits for alpha channel
         WGL_DEPTH_BITS_ARB, 16,             // minimum 16-bits for depth buffer
         0
      };

      int format = 0;
      UINT matchingFormats = 0;

      if (!wglChoosePixelFormatARB(g_hDC, attribList, 0, 1, &format, &matchingFormats))
      {
         MessageBox(0, _T("wglChoosePixelFormatARB() failed"), _T("Error"), MB_ICONSTOP);
         return false;
      }

      if (!(g_hPBuffer = wglCreatePbufferARB(g_hDC, format, m_sizeOut.cx, m_sizeOut.cy, 0)))
      {
         MessageBox(0, _T("wglCreatePbufferARB() failed"), _T("Error"), MB_ICONSTOP);
         return false;
      }

      if (!(g_hPBufferDC = wglGetPbufferDCARB(g_hPBuffer)))
      {
         MessageBox(0, _T("wglGetPbufferDCARB() failed"), _T("Error"), MB_ICONSTOP);
         return false;
      }

      if (!(g_hPBufferRC = wglCreateContext(g_hPBufferDC)))
      {
         MessageBox(0, _T("wglCreateContext() failed for PBuffer"), _T("Error"), MB_ICONSTOP);
         return false;
      }
#endif
      return true;
   }


   //int pitch = d->m_iScan / 4;
   //int iScan = cxDIB * 4;
   //for (int i = 0; i < cyDIB; ++i)
   //{
   //   memcpy(&d->m_pcolorref[pitch * i], &m_mem.get_data()[((cyDIB - 1) - i) * iScan], iScan);
   //}

   void bitmap::Cleanup()
   {
      if (m_texture)
      {
         glDeleteTextures(1, &m_texture);
         m_texture = 0;
      }

#ifdef WINDOWS
      if (g_hPBuffer)
      {
         wglDeleteContext(g_hPBufferRC);
         wglReleasePbufferDCARB(g_hPBuffer, g_hPBufferDC);
         wglDestroyPbufferARB(g_hPBuffer);
         g_hPBufferRC = 0;
         g_hPBufferDC = 0;
         g_hPBuffer = 0;
      }
      if (g_hDC)
      {
         if (g_hRC)
         {
            wglMakeCurrent(g_hDC, 0);
            wglDeleteContext(g_hRC);
            g_hRC = 0;
         }

         DeleteDC(g_hDC);
         g_hDC = 0;
      }
#else
      //      if (g_hPBuffer)
      //      {
      //         glDeleteContext(g_hPBufferRC);
      //         wglReleasePbufferDCARB(g_hPBuffer, g_hPBufferDC);
      //         glDestroyPbufferARB(g_hPBuffer);
      //         g_hPBufferRC = 0;
      //         g_hPBufferDC = 0;
      //         g_hPBuffer = 0;
      //      }
#endif


      //      ImageDestroy(&g_image);
   }




} // namespace draw2d_gl2d


void resizeBilinear(memory & m, int w2, int h2, int * pixels, int w, int h)
{

   //memory m;
   m.allocate(sizeof(int) * w2* h2);
   //int[] temp = new int[w2*h2];
   int * temp = (int *)m.get_data();
   int a, b, c, d, x, y, index;
   float x_ratio = ((float)(w - 1)) / w2;
   float y_ratio = ((float)(h - 1)) / h2;
   float x_diff, y_diff, blue, red, green;
   int offset = 0;
   for (int i = 0; i < h2; i++) {
      for (int j = 0; j < w2; j++) {
         x = (int)(x_ratio * j);
         y = (int)(y_ratio * i);
         x_diff = (x_ratio * j) - x;
         y_diff = (y_ratio * i) - y;
         index = (y*w + x);
         a = pixels[index];
         b = pixels[index + 1];
         c = pixels[index + w];
         d = pixels[index + w + 1];

         // blue element
         // Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
         blue = (a & 0xff)*(1 - x_diff)*(1 - y_diff) + (b & 0xff)*(x_diff)*(1 - y_diff) +
            (c & 0xff)*(y_diff)*(1 - x_diff) + (d & 0xff)*(x_diff*y_diff);

         // green element
         // Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
         green = ((a >> 8) & 0xff)*(1 - x_diff)*(1 - y_diff) + ((b >> 8) & 0xff)*(x_diff)*(1 - y_diff) +
            ((c >> 8) & 0xff)*(y_diff)*(1 - x_diff) + ((d >> 8) & 0xff)*(x_diff*y_diff);

         // red element
         // Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
         red = ((a >> 16) & 0xff)*(1 - x_diff)*(1 - y_diff) + ((b >> 16) & 0xff)*(x_diff)*(1 - y_diff) +
            ((c >> 16) & 0xff)*(y_diff)*(1 - x_diff) + ((d >> 16) & 0xff)*(x_diff*y_diff);

         temp[offset++] =
            0xff000000 | // hardcode alpha
            ((((int)red) << 16) & 0xff0000) |
            ((((int)green) << 8) & 0xff00) |
            ((int)blue);

      }

   }


}