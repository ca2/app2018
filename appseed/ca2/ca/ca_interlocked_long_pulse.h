#pragma once


namespace ca
{


   class CLASS_DECL_ca2 interlocked_long_pulse
   {
   public:


      interlocked_long *   m_plockedlong;
      long                 m_lLevel;

      
      interlocked_long_pulse(interlocked_long * plockedlong, long lLevel);
      virtual ~interlocked_long_pulse();


   };


} // namespace ca