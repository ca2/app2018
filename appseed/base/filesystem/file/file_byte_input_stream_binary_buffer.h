#pragma once


namespace file
{


   class CLASS_DECL_BASE byte_input_stream_binary_buffer :
      virtual public byte_input_stream
   {
   public:

      binary_buffer_sp     m_spbinarybuffer;


      byte_input_stream_binary_buffer(sp(::base::application) papp, const char * pszFilePath, uint32_t uiFlags = 0);
      virtual ~byte_input_stream_binary_buffer();

   };


} // namespace file


