#ifdef __cplusplus_winrt
void my_debug()
{
}
#else

#include <winapifamily.h>
#include <windows.h>
#include <shlwapi.h>
#include <wrl/client.h>
#include <D2d1_1.h>


#include <DXGI1_2.h>
#include <Dwrite.h>
#include <D3D11.h>
#include <D3D11_1.h>
#include <Dxgi1_3.h>     
#include <Initguid.h>
#include <DXGIDebug.h>
///void my_debug()
//{
//   Microsoft::WRL::ComPtr<IDXGIDebug1> d;
//   HRESULT hr =DXGIGetDebugInterface1(IID_IDXGIDebug1,&d);
//   if(SUCCEEDED(hr))
//   {
//      d->ReportLiveObjects(DXGI_DEBUG_ALL,DXGI_DEBUG_RLO_ALL);
//   }
//}
//#endif

//typedef GUID DXGI_DEBUG_ID;
//
//DEFINE_GUID(DXGI_DEBUG_ALL,0xe48ae283,0xda80,0x490b,0x87,0xe6,0x43,0xe9,0xa9,0xcf,0xda,0x8);
//DEFINE_GUID(DXGI_DEBUG_DX,0x35cdd7fc,0x13b2,0x421d,0xa5,0xd7,0x7e,0x44,0x51,0x28,0x7d,0x64);
//DEFINE_GUID(DXGI_DEBUG_DXGI,0x25cddaa4,0xb1c6,0x47e1,0xac,0x3e,0x98,0x87,0x5b,0x5a,0x2e,0x2a);
//DEFINE_GUID(DXGI_DEBUG_APP,0x6cd6e01,0x4219,0x4ebd,0x87,0x9,0x27,0xed,0x23,0x36,0xc,0x62);
//
//DEFINE_GUID(DXGI_DEBUG_D3D11,0x4b99317b,0xac39,0x4aa6,0xbb,0xb,0xba,0xa0,0x47,0x84,0x79,0x8f);
typedef HRESULT WINAPI FN_DXGIGetDebugInterface(
   REFIID riid,
   void **ppDebug
   );
typedef FN_DXGIGetDebugInterface * PFN_DXGIGetDebugInterface;
class dxgidebug
{
public:
   Microsoft::WRL::ComPtr<IDXGIDebug> d;
   void init()
   {
      HMODULE hmodule = GetModuleHandle("Dxgidebug.dll");
      if(hmodule != NULL)
      {
         PFN_DXGIGetDebugInterface pfn_DXGIGetDebugInterface =(PFN_DXGIGetDebugInterface) ::GetProcAddress(hmodule,"DXGIGetDebugInterface");
         if(pfn_DXGIGetDebugInterface != NULL)
         {
            HRESULT hr =pfn_DXGIGetDebugInterface(IID_IDXGIDebug,&d);
         }

      }

   }
   void debug()
   {
      if(d.Get() != NULL)
      {
         ::OutputDebugString("MY_DEBUG : IDXGIDebug::ReportLiveObjects");
         d->ReportLiveObjects(DXGI_DEBUG_ALL,DXGI_DEBUG_RLO_ALL);
      }
      else
      {
         init();
      }

   }
} g_dxgidebug;

void my_debug()
{
   g_dxgidebug.debug();
}


#endif