#include "framework.h"

#include <wincodec.h>
#ifdef METROWIN
#include <ShCore.h>
#endif

namespace visual
{


   bool dib_sp::write_to_file(::file::file_sp pfile, save_image * psaveimage)
   {

      if (pfile.is_null())
      {

         return false;

      }

      if (is_null())
      {

         alloc(pfile->allocer());

      }

      if (is_null())
      {

         return false;

      }

      ::draw2d::dib * pdib = m_p;

      if (pdib == NULL)
      {

         return NULL;

      }

#ifdef METROWIN

      Windows::Storage::Streams::InMemoryRandomAccessStream ^ randomAccessStream = ref new Windows::Storage::Streams::InMemoryRandomAccessStream();

      //::wait(randomAccessStream->WriteAsync(get_os_buffer()));

      comptr < IStream > pstream;

      ::CreateStreamOverRandomAccessStream(randomAccessStream, IID_PPV_ARGS(&pstream.get()));

#else

      comptr < IStream > pstream = LIBCALL(shlwapi, SHCreateMemStream)(NULL, NULL);

#endif

      //m_spmemfile->Truncate(0);

      //m_spmemfile->seek_to_begin();

      comptr < IWICImagingFactory > piFactory = NULL;
      comptr < IWICBitmapEncoder > piEncoder = NULL;
      comptr < IWICBitmapFrameEncode > piBitmapFrame = NULL;
      comptr < IPropertyBag2 > pPropertybag = NULL;

      comptr < IWICStream > piStream = NULL;
      UINT uiWidth = pdib->size().cx;
      UINT uiHeight = pdib->size().cy;

      HRESULT hr = CoCreateInstance(
                   CLSID_WICImagingFactory,
                   NULL,
                   CLSCTX_INPROC_SERVER,
                   IID_IWICImagingFactory,
                   (LPVOID*)&piFactory);

      if (SUCCEEDED(hr))
      {
         hr = piFactory->CreateStream(&piStream.get());
      }

      if (SUCCEEDED(hr))
      {
         hr = piStream->InitializeFromIStream(pstream);
      }

      if (SUCCEEDED(hr))
      {
         switch (psaveimage->m_eformat)
         {
         case ::visual::image::format_bmp:
            hr = piFactory->CreateEncoder(GUID_ContainerFormatBmp, NULL, &piEncoder.get());
            break;
         case ::visual::image::format_gif:
            hr = piFactory->CreateEncoder(GUID_ContainerFormatGif, NULL, &piEncoder.get());
            break;
         case ::visual::image::format_jpeg:
            hr = piFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &piEncoder.get());
            break;
         case ::visual::image::format_png:
            hr = piFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &piEncoder.get());
            break;
         default:
            break;
         }
      }

      if (SUCCEEDED(hr))
      {
         hr = piEncoder->Initialize(piStream, WICBitmapEncoderNoCache);
      }

      if (SUCCEEDED(hr))
      {
         hr = piEncoder->CreateNewFrame(&piBitmapFrame.get(), &pPropertybag.get());
      }

      if (SUCCEEDED(hr))
      {
         //if(m_bJxr)
         //{
         //   //PROPBAG2 option ={0};
         //   //option.pstrName = L"ImageQuality";
         //   //VARIANT varValue;
         //   //VariantInit(&varValue);
         //   //varValue.vt = VT_R4;
         //   //varValue.fltVal = 0.49f;
         //   PROPBAG2 option ={0};
         //   option.pstrName = L"UseCodecOptions";
         //   VARIANT varValue;
         //   VariantInit(&varValue);
         //   varValue.vt = VT_BOOL;
         //   varValue.boolVal = -1;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = pPropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"Quality";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_UI1;
         //   varValue.bVal = 184;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = pPropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"Subsampling";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_UI1;
         //   varValue.bVal = 1;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = pPropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"Overlap";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_UI1;
         //   varValue.bVal = 2;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = pPropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"StreamOnly";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_BOOL;
         //   varValue.boolVal = -1;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = pPropertybag->Write(1,&option,&varValue);
         //   }
         //}
         if (psaveimage->m_eformat == ::visual::image::format_jpeg)
         {
            PROPBAG2 option = { 0 };
            option.pstrName = L"ImageQuality";
            VARIANT varValue;
            VariantInit(&varValue);
            varValue.vt = VT_R4;
            varValue.fltVal = MAX(0.f, MIN(1.f, psaveimage->m_iQuality / 100.0f));
            if (SUCCEEDED(hr))
            {
               hr = pPropertybag->Write(1, &option, &varValue);
            }
         }
         if (SUCCEEDED(hr))
         {
            hr = piBitmapFrame->Initialize(pPropertybag);
         }
      }

      if (SUCCEEDED(hr))
      {
         hr = piBitmapFrame->SetSize(uiWidth, uiHeight);
      }

      WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
      if (SUCCEEDED(hr))
      {
         hr = piBitmapFrame->SetPixelFormat(&formatGUID);
      }
      pdib->map();
      if (SUCCEEDED(hr))
      {
         if (IsEqualGUID(formatGUID, GUID_WICPixelFormat32bppBGRA))
         {
            if (SUCCEEDED(hr))
            {
               hr = piBitmapFrame->WritePixels(uiHeight, pdib->m_iScan, uiHeight*pdib->m_iScan, (BYTE *)pdib->m_pcolorref);
            }
         }
         else
         {

            comptr <IWICBitmap> pbitmap;

            if (SUCCEEDED(hr))
            {
               hr = piFactory->CreateBitmapFromMemory(
                    pdib->size().cx,
                    pdib->size().cy,
                    GUID_WICPixelFormat32bppBGRA,
                    pdib->m_iScan,
                    pdib->m_iScan * pdib->size().cy,
                    (BYTE *)pdib->m_pcolorref,
                    &pbitmap.get()
                    );
            }

            comptr<IWICFormatConverter> pconverter;

            if (SUCCEEDED(hr))
            {

               hr = piFactory->CreateFormatConverter(&pconverter.get());

            }



            if (SUCCEEDED(hr))
            {

               hr = pconverter->Initialize(pbitmap, formatGUID, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);
            }

            //Step 4: Create render target and D2D bitmap from IWICBitmapSource
            //UINT width=0;
            //UINT height=0;
            //if(SUCCEEDED(hr))
            //{
            //   hr = pbitmap->GetSize(&width,&height);
            //}

            //pdib->create(width,height);

            if (SUCCEEDED(hr))
            {
               hr = piBitmapFrame->WriteSource(pconverter, NULL);
            }


            //for(int k = 0; k < height; k++)
            //{
            //   memcpy(&pb[k * iStride],&mem.get_data()[(height - 1 - k) * iStride],iStride);
            //}

         }
      }



      if (SUCCEEDED(hr))
      {
         hr = piBitmapFrame->Commit();
      }

      if (SUCCEEDED(hr))
      {
         hr = piEncoder->Commit();
      }

      //if(piFactory)
      //   piFactory->Release();

      //if(piBitmapFrame)
      //   piBitmapFrame->Release();

      //if(piEncoder)
      //   piEncoder->Release();

      //if(piStream)
      //   piStream->Release();



      STATSTG stg;
      ZERO(stg);
      pstream->Stat(&stg, STATFLAG_NONAME);
      LARGE_INTEGER l;
      l.QuadPart = 0;
      pstream->Seek(l, STREAM_SEEK_SET, NULL);


      memory mem(pdib->get_app());

      mem.allocate(1024 * 1024);

      ULONG ulPos = 0;
      ULONG ulRead;
      ULONGLONG ul;
      do
      {

         ulRead = 0;

         ul = stg.cbSize.QuadPart - ulPos;

         pstream->Read(mem.get_data(), (ULONG)MIN(ul, mem.get_size()), &ulRead);

         if (ulRead > 0)
         {

            pfile->write(mem.get_data(), ulRead);

            ulPos += ulRead;

         }

      }
      while (ulRead > 0 && stg.cbSize.QuadPart - ulPos > 0);

      //pstream->Release();

      return true;

   }


} // namespace visual



