#pragma once


namespace install
{


   class CLASS_DECL_BASE starter_start :
      virtual public object
   {
   public:


      ::hotplugin::plugin  *           m_pplugin;
      string                           m_strCommandLine;


      UINT start();

   };


   uint32_t _ca2_starter_start(void * pvoid);


} // namespace install




