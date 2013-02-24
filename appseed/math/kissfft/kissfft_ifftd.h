#pragma once


namespace kissfft
{


   class CLASS_DECL_CA2_MATH ifftd :
      virtual public ::ca::object
   {
   public:

      int32_t                     m_iSize;
      kiss_fftr_cfg           m_cfg;


      ifftd(int32_t iSize);
      virtual ~ifftd();

      void ifft(kiss_fft_cpx * pIn, double * pOut);

   };


} // namespace kissfft

