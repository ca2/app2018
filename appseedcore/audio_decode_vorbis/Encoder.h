#pragma once

namespace audio_decode_lame
{

   class encoder :
      public audio_decode::encoder
   {
   public:
      encoder(::ca::application * papp);
   public:
      ~encoder(void);

//      lame_global_flags *  gf;
      ex1::file *          m_pfile;
      primitive::memory               m_memoryLeft;
      primitive::memory               m_memoryRight;
      primitive::memory               m_memory;

      virtual void      EncoderSetSamplesPerSec(UINT user);
      virtual void      EncoderSetChannelCount(UINT user);
      virtual void      EncoderSetBitsPerSample(UINT user);
      virtual bool      EncoderSetFile(ex1::file * pfile);
      virtual bool      EncoderInitialize();
      //virtual bool      EncoderGetAttribute(EAttribute attribute, LPTSTR lpcszUTF8, int iSize);
      
      // return number of bytes written
      virtual void      EncoderWriteBuffer(LPVOID lpvoidBuffer, UINT uiBufferSize);

      virtual bool      EncoderFinalize();

   };

} // namespace audio_decode_lame
