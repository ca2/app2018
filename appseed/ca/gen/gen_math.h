#pragma once


namespace gen
{


   namespace math
   {


      CLASS_DECL_ca int32_t ceil_div(int32_t num, int32_t den);
      CLASS_DECL_ca int64_t ceil_div(int64_t num, int64_t den);


      class CLASS_DECL_ca math :
         virtual public ::radix::object
      {
      private:
         double         dPi;
//#if WINVER >= 0x500
#ifdef WINDOWSEX
         HCRYPTPROV     hCryptProv;
         HCRYPTKEY      hOriginalKey;
         HCRYPTKEY      hDuplicateKey;
#endif
//#endif
         DWORD          dwMode;
         BYTE           pbData[16];
         rng            m_rng;

      public:

         math(::ca::application * papp);
         virtual ~math();


         int64_t RandRange(int64_t iMin, int64_t iMax);


         int32_t LinearMap(int32_t iMin, int32_t iMax, int32_t iValue, int32_t iValueMin, int32_t iValueMax);
         uint64_t gen_rand();
         void gen_rand(void * buf, DWORD dwLen);
         uint32_t rnd();
         int32_t random_context_entropy(int32_t iMin, int32_t iMax, int32_t iLevel = 3);
         int32_t rand_max();
         double LinearMap(
            double dMin, double dMax,
            double dValue,
            double dValueMin,
            double dValueMax);
         double GetPi();

         static bool IsPowerOfTwo(uint32_t uiValue);
         static uint32_t ReverseBits (uint32_t index, uint32_t NumBits );
      //   template <class N> static inline void MaxClip(N * pnClipValue, N nMaxValue);
      //   template <class N> static inline void MinClip(N * pnClipValue, N nMinValue);
         static inline void MaxClip(double * pnClipValue, double nMaxValue);
         static inline void MinClip(double * pnClipValue, double nMinValue);

         int32_t RangeRate(int32_t iMin, int32_t iMax, double dRate);



      };

      /*template <class N> inline void math::MaxClip(N * pnClipValue, N nMaxValue)
      {
         if(*pnClipValue > nMaxValue)
            *pnClipValue = nMaxValue;
      }

      template <class N> inline void math::MinClip(N * pnClipValue, N nMinValue)
      {
         if(*pnClipValue < nMinValue)
            *pnClipValue = nMinValue;
      }*/

      inline void math::MaxClip(double * pnClipValue, double nMaxValue)
      {
         if(*pnClipValue > nMaxValue)
            *pnClipValue = nMaxValue;
      }

      inline void math::MinClip(double * pnClipValue, double nMinValue)
      {
         if(*pnClipValue < nMinValue)
            *pnClipValue = nMinValue;
      }


   } // namespace math


} // namespace gen


