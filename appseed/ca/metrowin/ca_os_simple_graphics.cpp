#include "framework.h"


os_simple_graphics::os_simple_graphics()
{

   m_pdc       = NULL;

   m_player    = NULL;

   m_pclip     = NULL;

}


os_simple_graphics::~os_simple_graphics()
{

   if(m_pdc != NULL)
   {

      destroy();

   }

}

bool os_simple_graphics::reference_os_data(ID2D1DeviceContext * pdc)
{

   if(m_pdc != NULL)
   {
    
      destroy();

   }
   
   m_pdc = pdc;

   m_iType = 1;

   return true;

}

bool os_simple_graphics::create_device()
{
   if(m_iType != 0)
      destroy();

   GetD2D1Factory1()->CreateDevice(TlsGetDXGIDevice(), &m_pd);

   m_pd->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pdc);

   if(m_pdc == NULL)
   {
      m_pd->Release();
      m_pdc = NULL;
      return false;
   }

   return true;

}

bool os_simple_graphics::create_from_bitmap(simple_bitmap & b)
{

   if(m_iType != 0)
      destroy();

   if(!create_device())
      return false;

   m_pdc->SetTarget(b.m_pbitmap);

   m_iType = 3;
   
   return true;

}

bool os_simple_graphics::from_window( Windows::UI::Core::CoreWindow ^ w)
{
   // Allocate a descriptor.
   DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
   swapChainDesc.Width = 0;                                     // use automatic sizing
   swapChainDesc.Height = 0;
   swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // this is the most common swapchain format
   swapChainDesc.Stereo = false; 
   swapChainDesc.SampleDesc.Count = 1;                          // don't use multi-sampling
   swapChainDesc.SampleDesc.Quality = 0;
   swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   swapChainDesc.BufferCount = 2;                               // use double buffering to enable flip
   swapChainDesc.Scaling = DXGI_SCALING_NONE;
   swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all Metro style apps must use this SwapEffect
   swapChainDesc.Flags = 0;


   TlsGetDXGIDevice()->GetAdapter(&m_pad);

   m_pad->GetParent(IID_PPV_ARGS(&m_pfax));

   // Get the final swap chain for this window from the DXGI factory.
   /*m_pfax->CreateSwapChainForCoreWindow(
      TlsGetD3D11Device1()->Get(), 
      reinterpret_cast<IUnknown*>(w), 
      &swapChainDesc, 
      nullptr,    // allow on all displays
      &m_swapChain);*/

        // Ensure that DXGI doesn't queue more than one frame at a time.
   //TlsGetDXGIDevice()->SetMaximumFrameLatency(1)

   return true;

}




/*bool os_simple_graphics::detach_bitmap()
{

   if(m_iType != 0)
      destroy();
   
   if(m_hdc == NULL)
      return false;

   if(m_hbitmapOld == NULL)
      return false;

   if(m_iType != 3)
      return false;

   HBITMAP hbitmap = (HBITMAP) ::SelectObject(m_hdc, m_hbitmapOld);

   m_iType = 1;
      
   return hbitmap != NULL;

}*/

/*bool os_simple_graphics::from_entire_window(HWND hwnd)
{

   if(m_iType != 0)
      destroy();

   m_hdc = ::GetWindowDC(hwnd);

   if(m_hdc == NULL)
      return false;

   m_hwnd = hwnd;

   m_iType = 2;
   
   return true;

}


bool os_simple_graphics::from_window(HWND hwnd)
{

   if(m_iType != 0)
      destroy();

   m_hdc = ::GetDC(hwnd);

   if(m_hdc == NULL)
      return false;

   m_hwnd = hwnd;

   m_iType = 2;
   
   return true;

}

bool os_simple_graphics::from_window_paint(HWND hwnd, LPRECT lprectPaint)
{

   if(m_iType != 0)
      destroy();

    m_hdc = BeginPaint(hwnd, &m_ps);

    if(m_hdc == NULL)
       return false;
    
    m_hwnd = hwnd;

    if(lprectPaint != NULL)
    {
    
       *lprectPaint = m_ps.rcPaint;

    }

    m_iType = 4;

    return true;
    
}*/

/*
bool os_simple_graphics::reference_os_data(HDC hdc)
{
   
   m_iType = 5;

   m_hdc = hdc;

   return true;

}
*/

bool os_simple_graphics::create()
{
   
   //os_simple_graphics g;

   //g.from_window(nullptr);
   
   //return create(g);

   return create_device();
   
}

/*bool os_simple_graphics::create(os_simple_graphics & g)
{
   
   return reference_os_da(g.m_hdc);
   
}

bool os_simple_graphics::create_from_screen()
{
   
   return from_window(NULL);
   
}*/

bool os_simple_graphics::select(simple_font & font)
{

   m_font = font;

   return font.m_pformat != NULL;

}

bool os_simple_graphics::select(simple_brush & brush)
{
   
   m_brush = brush;

   return brush.m_pbrush != NULL || brush.m_plineargradientbrush != NULL || brush.m_psolidbrush != NULL;

}

bool os_simple_graphics::select(simple_pen & pen)
{
   
   m_pen = pen;

   return m_pen.get_os_brush() != NULL;

}

bool os_simple_graphics::destroy()
{
   
   if(m_player != NULL)
   {
      m_pdc->PopLayer();
      m_player->Release();
      m_player = NULL;
   }

   if(m_pclip != NULL)
   {
      m_pclip->Release();
      m_pclip = NULL;
   }

   if(m_pdc != NULL)
   {
      m_pdc->Release();
      m_pdc = NULL;
   }


   return true;

}

bool os_simple_graphics::set_offset(int x, int y)
{
   
   m_pdc->SetTransform(D2D1::Matrix3x2F::Translation((FLOAT) x, (FLOAT) y));

   return true;

}

bool os_simple_graphics::offset(int x, int y)
{

   D2D1::Matrix3x2F m;

   m_pdc->GetTransform(&m);

   m = m * D2D1::Matrix3x2F::Translation((FLOAT) x, (FLOAT) y);

   m_pdc->SetTransform(&m);

   return true;

}


bool os_simple_graphics::bit_blt(int x, int y, int cx, int cy, simple_graphics & gSrc, int x1, int y1, DWORD rop)
{
   
   m_pdc->DrawImage(gSrc.m_bitmap.m_pbitmap, D2D1::Point2F((FLOAT) x, (FLOAT) y), D2D1::RectF((FLOAT) x1, (FLOAT) y1, (FLOAT) (x1 + cx), (FLOAT) (y1 + cy)));
   ///return ::BitBlt(m_hdc, x, y, cx, cy, gSrc.m_hdc, x1, y1, rop) != FALSE;

   return true;

}

bool os_simple_graphics::blend_bitmap_data(int x, int y, int cx, int cy, COLORREF * pdata)
{
   
   try
   {
   
      simple_bitmap b;

      b.create_from_data(cx, cy, pdata, (simple_graphics & ) *this);

      m_pdc->DrawImage(b.m_pbitmap, D2D1::Point2F((FLOAT) x, (FLOAT) y), D2D1::RectF(0.f, 0.f, (float) cx, (float) cy));

      return true;

   }
   catch(...)
   {

   }

   return false;

}

SIZE os_simple_graphics::get_text_extent(const char * psz, int iLen)
{

   if(iLen < 0)
      iLen = strlen(psz);

   vsstring str(psz, iLen);

   wstring wstr(str);

   SIZE size;

   IDWriteTextLayout * playout = NULL;

   HRESULT hr = TlsGetWriteFactory()->CreateTextLayout(
      wstr,                // The string to be laid out and formatted.
      wstr.get_length(),   // The length of the string.
      m_font.m_pformat,    // The text format to apply to the string (contains font information, etc).
      1024.f * 1024.f,               // The width of the layout box.
      1024.f * 1024.f,        // The height of the layout box.
      &playout  // The IDWriteTextLayout interface pointer.
      );

   if(playout == NULL || FAILED(hr))
   {

      size.cx = 0;

      size.cy = 0;

   }
   else
   {
      
      DWRITE_TEXT_METRICS m;
      
      playout->GetMetrics(&m);

      size.cx = (LONG) m.width;

      size.cy = (LONG) m.height;

   }

   return size;

}


bool os_simple_graphics::rectangle(LPCRECT lpcrect)
{

   bool bOk1 = true;

   if(m_brush.m_pbrush != NULL)
   {

      bOk1 = fill_rect(lpcrect, m_brush);

   }

   bool bOk2 = true;

   if(m_pen.get_os_brush() != NULL)
   {

      bOk2 = draw_rect(lpcrect, m_pen);

   }
   
   return bOk1 && bOk2;

}


bool os_simple_graphics::draw_rect(LPCRECT lpcrect, simple_pen & pen)
{

   if(&pen == NULL)
      return true;

   if(pen.get_os_brush() == NULL)
      return true;

   D2D1_RECT_F r;

   r.left      = (FLOAT) lpcrect->left;
   r.top       = (FLOAT) lpcrect->top;
   r.right     = (FLOAT) lpcrect->right;
   r.bottom    = (FLOAT) lpcrect->bottom;

   m_pdc->DrawRectangle(r, pen.get_os_brush(), (FLOAT) m_pen.m_iWidth);

   return true;

}

bool os_simple_graphics::fill_rect(LPCRECT lpcrect, simple_brush & brush)
{
   
   if(&brush == NULL)
      return true;

   if(brush.get_os_data() == NULL)
      return true;

   D2D1_RECT_F r;

   r.left      = (FLOAT) lpcrect->left;
   r.top       = (FLOAT) lpcrect->top;
   r.right     = (FLOAT) lpcrect->right;
   r.bottom    = (FLOAT) lpcrect->bottom;

   m_pdc->FillRectangle(r, brush.get_os_data());

   return true;

}

/*bool os_simple_graphics::set_text_color(COLORREF cr)
{
   
   m_brush
   return ::SetTextColor(m_hdc, cr) != FALSE;

}
*/

bool os_simple_graphics::set_alpha_mode(::draw2d::e_alpha_mode emode)
{

   switch(emode)
   {
   
   case ::draw2d::alpha_mode_blend:
   
      m_pdc->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

      break;

   case ::draw2d::alpha_mode_set:
   
      m_pdc->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);

      break;

   };

   return false;

}

/*bool os_simple_graphics::alpha_blend(int x, int y, int cx, int cy, os_simple_graphics & gSrc, int x1, int y1, int cx1, int cy1, BLENDFUNCTION bf)
{

   m_pdc->DrawImage(gSrc.m_bitmap.m_pbitmap, D2D1::Point2F(x, y), D2D1::RectF(x1, y1, x1 + cx, y1 + cy));

   return true;

}*/

void os_simple_graphics::fill_solid_rect(LPCRECT lpcrect, COLORREF clr)
{
   
   simple_solid_brush b((simple_graphics &) *this, clr);

   fill_rect(lpcrect, b);
   
}

bool os_simple_graphics::draw_path(simple_path & path, simple_pen & pen)
{
   
   m_pdc->DrawGeometry(path.get_os_data(), pen.get_os_brush(), (FLOAT) pen.m_iWidth);

   return true;

}

bool os_simple_graphics::fill_path(simple_path & path, simple_brush & brush)
{
   
   m_pdc->FillGeometry(path.get_os_data(), brush.get_os_data());

   return true;

}

bool os_simple_graphics::fill_polygon(LPPOINT lpa, int iCount, ::draw2d::e_fill_mode emode)
{

   simple_path path;

   path.begin_figure(true, ::draw2d::fill_mode_winding);

   path.add_lines(lpa, iCount);

   path.end_figure(true);

   fill_path(path, m_brush);

   return true;

}

bool os_simple_graphics::text_out(int x, int y, const char * pszUtf8, int iSize)
{

   vsstring str(pszUtf8, iSize);

   wstring wstr(str);

   D2D1_RECT_F rect;

   rect.left = (FLOAT) x;
   rect.top = (FLOAT) y;
   rect.right = x + (1024.f * 1024.f);
   rect.bottom = y + (1024.f * 1024.f);

   m_pdc->DrawText(wstr, wstr.get_length(), m_font.m_pformat, &rect, m_brush.m_pbrush);

   return true;

   /*WCHAR * pwsz = utf8_to_16(pszUtf8);
   bool b = TextOutW(m_hdc, x, y, pwsz, (int) wcslen_dup(pwsz)) != FALSE;
   delete  [] pwsz;
   return b;*/
}



bool os_simple_graphics::draw_line(int x1, int y1, int x2, int y2, simple_pen & pen)
{

   D2D1_POINT_2F p1;
   p1.x = (FLOAT) x1;
   p1.y = (FLOAT) y1;

   D2D1_POINT_2F p2;
   p2.x = (FLOAT) x2;
   p2.y = (FLOAT) y2;

   m_pdc->DrawLine(p1, p2, pen.get_os_brush(), (FLOAT) pen.m_iWidth);

   return true;
}

bool os_simple_graphics::replace_clip(const RECT & rect)
{
   
   if(m_player != NULL)
   {

      m_pdc->PopLayer();
      m_player->Release();
      m_player = NULL;
   }

   if(m_pclip != NULL)
   {
      m_pclip->Release();
      m_pclip = NULL;
   }

   simple_path path;

   path.begin_figure(true, ::draw2d::fill_mode_winding);

   path.add_rect(rect);

   path.end_figure(true);

   if(path.get_os_data() == NULL)
      return true;

   path.get_os_data()->AddRef();

   m_pclip = path.get_os_data();

   HRESULT hr = m_pdc->CreateLayer(NULL, &m_player);

   if(FAILED(hr) || m_player == NULL)
   {
      m_pclip->Release();
      m_pclip = NULL;
      return false;
   }

   m_pdc->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), m_pclip), m_player);

   return true;

}


bool os_simple_graphics::replace_clip(simple_path & path)
{
   
   if(m_player != NULL)
   {

      m_pdc->PopLayer();
      m_player->Release();
      m_player = NULL;
   }

   if(m_pclip != NULL)
   {
      m_pclip->Release();
      m_pclip = NULL;
   }

   if(path.get_os_data() == NULL)
      return true;

   path.get_os_data()->AddRef();

   m_pclip = path.get_os_data();

   HRESULT hr = m_pdc->CreateLayer(NULL, &m_player);

   if(FAILED(hr) || m_player == NULL)
   {
      m_pclip->Release();
      m_pclip = NULL;
      return false;
   }

   m_pdc->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), m_pclip), m_player);

   return true;

}


bool os_simple_graphics::exclude_clip(simple_path & path)
{

   if(path.get_os_data() == NULL)
      return true;

   if(m_player != NULL)
   {

      m_pdc->PopLayer();
      m_player->Release();
      m_player = NULL;

   }

   ID2D1Factory * pfactory = NULL;

   m_pdc->GetFactory(&pfactory);

   HRESULT hr;

   if(m_pclip == NULL)
   {
      ID2D1RectangleGeometry * prect;

      hr = pfactory->CreateRectangleGeometry(D2D1::InfiniteRect(), &prect);

      if(FAILED(hr) || m_pclip == NULL)
      {
         pfactory->Release();
         return false;
      }

      hr = pfactory->CreatePathGeometry(&m_pclip);

      if(FAILED(hr) || m_pclip == NULL)
      {
         pfactory->Release();
         prect->Release();
         return false;
      }

      ID2D1GeometrySink * psink = NULL;

      hr = m_pclip->Open(&psink);

      if(FAILED(hr) || psink == NULL)
      {
         pfactory->Release();
         prect->Release();
         m_pclip->Release();
         m_pclip = NULL;
         return false;
      }

      hr = prect->Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION_LINES, NULL, psink);

      psink->Close();
      psink->Release();
      prect->Release();

   }


   ID2D1PathGeometry * pclipNew = NULL;

   hr = pfactory->CreatePathGeometry(&pclipNew);

   if(FAILED(hr) || pclipNew == NULL)
   {
      pfactory->Release();
      m_pclip->Release();
      m_pclip = NULL;
      return false;
   }

   ID2D1GeometrySink * psink = NULL;

   hr = m_pclip->Open(&psink);

   if(FAILED(hr) || psink == NULL)
   {
      pfactory->Release();
      m_pclip->Release();
      m_pclip = NULL;
      pclipNew->Release();
      return false;
   }
   
   
   m_pclip->CombineWithGeometry(path.get_os_data(), D2D1_COMBINE_MODE_EXCLUDE, NULL, psink);

   psink->Close();
   psink->Release();
   pfactory->Release();
   m_pclip->Release();
   m_pclip = pclipNew;


   hr = m_pdc->CreateLayer(NULL, &m_player);

   if(FAILED(hr) || m_player == NULL)
   {
      m_pclip->Release();
      m_pclip = NULL;
      return false;
   }

   m_pdc->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), m_pclip), m_player);

   return true;

}



bool os_simple_graphics::is_null()
{

   return m_pdc != NULL;

}


