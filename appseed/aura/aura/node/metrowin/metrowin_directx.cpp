#include "framework.h"
//#include "base/user/user.h"
//#include "metrowin.h"
#include "app/appseed/draw2d_direct2d/draw2d_direct2d.h"

using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;
using namespace D2D1;

extern CLASS_DECL_AURA spa(::draw2d::dib) * g_pdiba;

namespace metrowin
{


   directx_base::directx_base(::aura::application * papp):
      m_papp(papp),
      m_dpi(-1.0f),
      m_mutexDc(papp)
   {

      m_pimpl = NULL;

      m_b3D = false;

      m_dpiIni = 1.0f;
      m_dpi = -999.0f;
      m_bInitialized = false;
      m_bInit = false;

   }


   // Initialize the DirectX resources required to run.
   void directx_base::Initialize(CoreWindow^ window,float dpi)
   {

      m_window = window;

      m_dpiIni = dpi;

      CreateDeviceIndependentResources();

      m_bInit = true;


   }


   bool directx_base::defer_init()
   {

      ::draw2d::lock draw2dlock;

      if (m_bInitialized)
      {

         return true;

      }

      if (!m_bInit)
      {

         return false;

      }

      CreateDeviceResources();

      SetDpi(m_dpiIni);

      m_bInitialized = true;

      //m_dib.alloc(m_papp->allocer());
      //m_dib->create(1000, 1000);


      return true;

   }


   // Recreate all device resources and set them back to the current state.
   void directx_base::HandleDeviceLost()
   {

      ::draw2d::lock draw2dlock;

      // Reset these member variables to ensure that SetDpi recreates all resources.
      float dpi = m_dpi;
      m_dpi = -1.0f;
      m_windowBounds.Width = 0;
      m_windowBounds.Height = 0;
      m_swapChain = nullptr;

      CreateDeviceResources();

      SetDpi(dpi);

   }


   // These are the resources required independent of the device.
   void directx_base::CreateDeviceIndependentResources()
   {

      ::draw2d::lock draw2dlock;

      D2D1_FACTORY_OPTIONS options;
      ZeroMemory(&options,sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
      // If the project is in a debug build, enable Direct2D debugging via SDK Layers.
      options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif


      ::metrowin::throw_if_failed(
      DWriteCreateFactory(
      DWRITE_FACTORY_TYPE_SHARED,
      __uuidof(IDWriteFactory),
      &m_dwriteFactory
      )
      );

      ::metrowin::throw_if_failed(
      CoCreateInstance(
      CLSID_WICImagingFactory,
      nullptr,
      CLSCTX_INPROC_SERVER,
      IID_PPV_ARGS(&m_wicFactory)
      )
      );






















































      //// Create a DirectWrite text format object.
      //::metrowin::throw_if_failed(
      //m_dwriteFactory->CreateTextFormat(
      //L"Gabriola",
      //nullptr,
      //DWRITE_FONT_WEIGHT_REGULAR,
      //DWRITE_FONT_STYLE_NORMAL,
      //DWRITE_FONT_STRETCH_NORMAL,
      //64.0f,
      //L"en-US", // locale
      //&m_textFormat
      //)
      //);

      //// Center the text horizontally.
      //::metrowin::throw_if_failed(
      //m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)
      //);

      //// Center the text vertically.
      //::metrowin::throw_if_failed(
      //m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER)
      //);





   }

   // These are the resources that depend on the device.
   void directx_base::CreateDeviceResources()
   {

      ::draw2d::lock draw2dlock;

      m_d3dDevice = global_draw_get_d3d11_device1();

      m_d3dContext = global_draw_get_d3d11_device_context1();

      m_d2dDevice = global_draw_get_d2d1_device();

   }

   // Helps track the DPI in the helper class.
   // This is called in the dpiChanged event handler in the view class.
   void directx_base::SetDpi(float dpi)
   {
      // Only handle window size changed if there is no pending DPI change.

      m_window->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this,dpi]()
      {
         OnChangeDpi(dpi);

         //         ::aura::system::g_p->m_psystem->m_possystemwindow->m_bWindowSizeChange = true;

      }));

   }
   void directx_base::OnChangeDpi(float dpi)
   {
      ::draw2d::lock draw2dlock;

      if (dpi != m_dpi)
      {
         // Save the DPI of this display in our class.
         m_dpi = dpi;

         System.m_dpi = dpi;

         m_size.cx = (LONG)m_window->Bounds.Width;

         m_size.cy = (LONG)m_window->Bounds.Height;

         // Often a DPI change implies a window size change. In some cases Windows will issue
         // both a size changed event and a DPI changed event. In this case, the resulting bounds
         // will not change, and the window resize code will only be executed once.
         OnWindowSizeChange();
      }
   }

   // This routine is called in the event handler for the view SizeChanged event.
   void directx_base::UpdateForWindowSizeChange()
   {

      // Only handle window size changed if there is no pending DPI change.

      m_window->Dispatcher->RunAsync(CoreDispatcherPriority::Normal,ref new Windows::UI::Core::DispatchedHandler([this]()
      {
         OnWindowSizeChange();

//         ::aura::system::g_p->m_psystem->m_possystemwindow->m_bWindowSizeChange = true;

      }));

   }

   //::Windows::Graphics::Display::DisplayInformation ^ displayinformation = ::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

   //if(m_dpi != displayinformation->LogicalDpi)
   //{
   //   return;
   //}
   void directx_base::OnWindowSizeChange()
   {

      if (m_size.cx != m_windowBounds.Width || m_size.cy != m_windowBounds.Height)
      {

         ::draw2d::lock lock;

         CreateWindowSizeDependentResources();

         System.m_possystemwindow->m_pui->SetWindowPos(ZORDER_TOP, 0, 0, m_size.cx, m_size.cy, SWP_SHOWWINDOW);

         if (System.handler()->m_varTopicQuery.has_property("client_only"))
         {

            for (int i = 0; i < System.m_possystemwindow->m_pui->m_uiptraChild.get_count(); i++)
            {

               if (System.m_possystemwindow->m_pui->m_uiptraChild[i]->IsWindowVisible())
               {

                  System.m_possystemwindow->m_pui->m_uiptraChild[i]->SetWindowPos(ZORDER_TOP, 0, 0, m_size.cx, m_size.cy, SWP_SHOWWINDOW);

                  System.m_possystemwindow->m_pui->m_uiptraChild[i]->set_need_layout();

                  System.m_possystemwindow->m_pui->m_uiptraChild[i]->set_need_redraw();

               }

            }

         }

         //Render();

         //Present();

      }

   }


   // Allocate all memory resources that change on a window SizeChanged event.
   void directx_base::CreateWindowSizeDependentResources()
   {

      ::draw2d::device_lock devicelock;

      // Store the window bounds so the next time we get a SizeChanged event we can
      // avoid rebuilding everything if the size is identical.
      m_windowBounds.Width = (float) m_size.cx;
      m_windowBounds.Height = (float) m_size.cy;

      if(m_swapChain != nullptr)
      {

         ID3D11RenderTargetView * nullViews[] = { nullptr };
         m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
         m_d3dRenderTargetView = nullptr;
         m_pd2d1devicecontext->SetTarget(nullptr);
         m_d2dTargetBitmap = nullptr;
         m_d3dDepthStencilView = nullptr;
         m_d3dContext->Flush();

         m_pd2d1devicecontext = nullptr;
         m_d2dTargetBitmap = nullptr;
         m_d3dRenderTargetView = nullptr;
         m_d3dDepthStencilView = nullptr;
         //m_windowSizeChangeInProgress = true;


         HRESULT hr;
         m_d3dContext->Flush();
         m_d3dContext->ClearState();
         m_d2dDevice->ClearResources();
         {
            Microsoft::WRL::ComPtr < ID3D11CommandList > pcommandlist;
            hr = m_d3dContext->FinishCommandList(FALSE, &pcommandlist);
            if (SUCCEEDED(hr))
            {
            }
         }

         // If the swap chain already exists, resize it.
         hr = m_swapChain->ResizeBuffers(
              0,
              0, // If you specify zero, DXGI will use the width of the client area of the target window.
              0, // If you specify zero, DXGI will use the height of the client area of the target window.
              DXGI_FORMAT_UNKNOWN, // Set this value to DXGI_FORMAT_UNKNOWN to preserve the existing format of the back buffer.
              0);

         if(hr == DXGI_ERROR_DEVICE_REMOVED)
         {
            // If the device was removed for any reason, a new device and swapchain will need to be created.
            HandleDeviceLost();

            // Everything is set up now. Do not continue execution of this method.
            return;
         }
         else if(hr == DXGI_ERROR_INVALID_CALL)
         {
            // i1;
            //  return;
            TRACE("directx_base::CreateWindowSizeDependentResources(1) DXGI_ERROR_INVALID_CALL");
         }
         else
         {
            ::metrowin::throw_if_failed(hr);
         }
      }
      else
      {
         // Otherwise, create a new one using the same adapter as the existing Direct3D device.
         DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
         //swapChainDesc.Width = m_size.cx;                                     // Use automatic sizing.
         //swapChainDesc.Height = m_size.cy;
         swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // This is the most common swap chain format.
         swapChainDesc.Stereo = false;
         swapChainDesc.SampleDesc.Count = 1;                          // Don't use multi-sampling.
         swapChainDesc.SampleDesc.Quality = 0;
         swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
         swapChainDesc.BufferCount = 2;                               // Use double-buffering to minimize latency.
         swapChainDesc.Scaling = DXGI_SCALING_NONE;
         swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Metro style apps must use this SwapEffect.
         swapChainDesc.Flags = 0;

         ComPtr<IDXGIDevice1> dxgiDevice;
         ::metrowin::throw_if_failed(
         m_d3dDevice.As(&dxgiDevice)
         );

         ComPtr<IDXGIAdapter> dxgiAdapter;
         ::metrowin::throw_if_failed(
         dxgiDevice->GetAdapter(&dxgiAdapter)
         );

         ComPtr<IDXGIFactory2> dxgiFactory;
         ::metrowin::throw_if_failed(
         dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
         );

         CoreWindow ^ window = m_window.Get();

         ::metrowin::throw_if_failed(
         dxgiFactory->CreateSwapChainForCoreWindow(
         m_d3dDevice.Get(),
         reinterpret_cast<IUnknown*>(window),
         &swapChainDesc,
         nullptr,
         &m_swapChain
         )
         );

         // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
         // ensures that the application will only render after each VSync, minimizing power consumption.
         ::metrowin::throw_if_failed(dxgiDevice->SetMaximumFrameLatency(1));

      }

      if (m_b3D)
      {

         // Create a Direct3D render target view of the swap chain back buffer.
         ComPtr<ID3D11Texture2D> backBuffer;
         ::metrowin::throw_if_failed(
         m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
         );

         ::metrowin::throw_if_failed(
         m_d3dDevice->CreateRenderTargetView(
         backBuffer.Get(),
         nullptr,
         &m_d3dRenderTargetView
         )
         );

         // Cache the rendertarget dimensions in our helper class for convenient use.
         D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
         backBuffer->GetDesc(&backBufferDesc);
         m_renderTargetSize.Width = static_cast<float>(backBufferDesc.Width);
         m_renderTargetSize.Height = static_cast<float>(backBufferDesc.Height);

         // Create a depth stencil view for use with 3D rendering if needed.
         CD3D11_TEXTURE2D_DESC depthStencilDesc(
         DXGI_FORMAT_D24_UNORM_S8_UINT,
         backBufferDesc.Width,
         backBufferDesc.Height,
         1,
         1,
         D3D11_BIND_DEPTH_STENCIL
         );

         ComPtr<ID3D11Texture2D> depthStencil;
         ::metrowin::throw_if_failed(
         m_d3dDevice->CreateTexture2D(
         &depthStencilDesc,
         nullptr,
         &depthStencil
         )
         );

         auto viewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);
         ::metrowin::throw_if_failed(
         m_d3dDevice->CreateDepthStencilView(
         depthStencil.Get(),
         &viewDesc,
         &m_d3dDepthStencilView
         )
         );

         // Set the 3D rendering viewport to target the entire window.
         CD3D11_VIEWPORT viewport(
         0.0f,
         0.0f,
         static_cast<float>(backBufferDesc.Width),
         static_cast<float>(backBufferDesc.Height)
         );

         m_d3dContext->RSSetViewports(1, &viewport);

      }

      // Create a Direct2D target bitmap associated with the
      // swap chain back buffer and set it as the current target.
      D2D1_BITMAP_PROPERTIES1 bitmapProperties =
      BitmapProperties1(
      D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),
      m_dpi,
      m_dpi
      );


      ::draw2d_direct2d::throw_if_failed(
      global_draw_get_d2d1_device()->CreateDeviceContext(
      //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
      D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
      &m_pd2d1devicecontext
      )
      );


      ComPtr<IDXGISurface> dxgiBackBuffer;
      ::metrowin::throw_if_failed(
      m_swapChain->GetBuffer(0,IID_PPV_ARGS(&dxgiBackBuffer))
      );

      ::metrowin::throw_if_failed(
      m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(
      dxgiBackBuffer.Get(),
      &bitmapProperties,
      &m_d2dTargetBitmap
      )
      );

      m_pd2d1devicecontext->SetTarget(m_d2dTargetBitmap.Get());

   }


   void directx_base::Present()
   {

      HRESULT hr = S_OK;

      {

         if (!defer_init())
         {

            return;

         }

         try
         {


            // The application may optionally specify "dirty" or "scroll" rects to improve efficiency
            // in certain scenarios.  In this sample, however, we do not utilize those features.
            DXGI_PRESENT_PARAMETERS parameters = {};

            parameters.DirtyRectsCount = 0;

            parameters.pDirtyRects = nullptr;

            parameters.pScrollRect = nullptr;

            parameters.pScrollOffset = nullptr;

            if (m_swapChain == nullptr)
            {

               return;

            }

            {

               ::draw2d::device_lock devicelock;

               // The first argument instructs DXGI to block until VSync, putting the application
               // to sleep until the next VSync. This ensures we don't waste any cycles rendering
               // frames that will never be displayed to the screen.
               hr = m_swapChain->Present1(1, 0, &parameters);

               if (m_d3dContext.Get())
               {

                  if (m_d3dRenderTargetView.Get())
                  {

                     // Discard the contents of the render target.
                     // This is a valid operation only when the existing contents will be entirely
                     // overwritten. If dirty or scroll rects are used, this call should be removed.
                     m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

                  }

                  if (m_d3dDepthStencilView.Get())
                  {

                     // Discard the contents of the depth stencil.
                     m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());

                  }

               }

               g_pdiba->remove_all();

            }

            // If the device was removed either by a disconnect or a driver upgrade, we
            // must recreate all device resources.
            if (hr == DXGI_ERROR_DEVICE_REMOVED)
            {

               HandleDeviceLost();

            }
            else
            {

               ::metrowin::throw_if_failed(hr);

            }

         }
         catch (...)
         {

         }

      }

      //if (m_windowSizeChangeInProgress)
      //{

      //   m_windowSizeChangeInProgress = false;

      //   m_window->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]()
      //   {

      // A window size change has been initiated and the app has just completed presenting
      // the first frame with the new size. Notify the resize manager so we can short
      // circuit any resize animation and prevent unnecessary delays.
      //      CoreWindowResizeManager::GetForCurrentView()->NotifyLayoutCompleted();

      //   }));

      //}

   }

   void directx_base::ValidateDevice()
   {
      ::draw2d::lock draw2dlock;
      // The D3D Device is no longer valid if the default adapter changes or if
      // the device has been removed.

      // First, get the information for the adapter related to the current device.

      ComPtr<IDXGIDevice1> dxgiDevice;
      ComPtr<IDXGIAdapter> deviceAdapter;
      DXGI_ADAPTER_DESC deviceDesc;
      ::metrowin::throw_if_failed(m_d3dDevice.As(&dxgiDevice));
      ::metrowin::throw_if_failed(dxgiDevice->GetAdapter(&deviceAdapter));
      ::metrowin::throw_if_failed(deviceAdapter->GetDesc(&deviceDesc));

      // Next, get the information for the default adapter.

      ComPtr<IDXGIFactory2> dxgiFactory;
      ComPtr<IDXGIAdapter1> currentAdapter;
      DXGI_ADAPTER_DESC currentDesc;
      ::metrowin::throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
      ::metrowin::throw_if_failed(dxgiFactory->EnumAdapters1(0,&currentAdapter));
      ::metrowin::throw_if_failed(currentAdapter->GetDesc(&currentDesc));

      // If the adapter LUIDs don't match, or if the device reports that it has been removed,
      // a new D3D device must be created.

      if((deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart) ||
            (deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart) ||
            FAILED(m_d3dDevice->GetDeviceRemovedReason()))
      {
         // Release references to resources related to the old device.
         dxgiDevice = nullptr;
         deviceAdapter = nullptr;

         // Create a new device and swap chain.
         HandleDeviceLost();
      }
   }


   HRESULT directx_base::Render()
   {
      ::draw2d::lock draw2dlock;

      if(!defer_init())
         return E_FAIL;




      m_pd2d1devicecontext->BeginDraw();

      D2D1_COLOR_F cr;
      cr.a = 1.0f;
      cr.b = 0xcc / 255.f;
      cr.r = 0xcc / 255.f;
      cr.g = 0xcc / 255.f;
      m_pd2d1devicecontext->Clear(cr);


      m_pd2d1devicecontext->SetTransform(D2D1::Matrix3x2F::Identity());

      ::draw2d::graphics_sp dc(get_app()->allocer());

      dc->attach((ID2D1DeviceContext *)m_pd2d1devicecontext.Get());

      sp(::user::interaction_impl) pimpl = Sys(::aura::system::g_p).m_possystemwindow->m_pui->m_pimpl;

      if (pimpl.is_set())
      {

         pimpl->_001UpdateBuffer();

         sp(::window_double_buffer) pbuffer = pimpl->m_spgraphics;

         if (pbuffer.is_set())
         {

            size sz = pbuffer->get_buffer()->get_size();

            ::draw2d::graphics * pgraphics = pbuffer->get_buffer()->get_graphics();

            dc->from(sz, pgraphics, SRCCOPY);

         }

      }

      //_001UpdateBuffer();

      //Sys(::aura::system::g_p).m_possystemwindow->m_pui->_000OnDraw(m_dib->get_graphics());

      //dc->from(m_dib->get_size(), m_dib->get_graphics(), SRCCOPY);
      //dc->from(m_dib->get_size(), m_dib->get_graphics(), SRCCOPY);

      //sl.lock();

      dc->detach();


      // We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
      // is lost. It will be handled during the next call to Present.
      HRESULT hr = m_pd2d1devicecontext->EndDraw();

      if(FAILED(hr))
      {

         if(hr == D2DERR_RECREATE_TARGET)
         {

            trace_hr("directx_base::Render, EndDraw", hr);

         }
         else if(hr == D2DERR_WRONG_STATE)
         {

            trace_hr("directx_base::Render, EndDraw",hr);

         }
         else
         {

            trace_hr("directx_base::Render, EndDraw",hr);

         }

      }

      return hr;

   }


} // namespace metrowin



