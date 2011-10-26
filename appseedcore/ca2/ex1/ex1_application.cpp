#include "StdAfx.h"

namespace ex1
{

   application::application()
   {
   }

   application::~application()
   {
   }

typedef  void (* PFN_ca2_factory_exchange)(::ca::application * papp);

   void application::Ex1OnFactoryExchange()
   {
   #ifdef WIN32
      HMODULE hmodule = ::LoadLibraryA("os.dll");
      PFN_ca2_factory_exchange pfn_ca2_factory_exchange = (PFN_ca2_factory_exchange) ::GetProcAddress(hmodule, "ca2_factory_exchange");
      pfn_ca2_factory_exchange(this);
   #else
      return NULL; // not implemented... yet!! you may start!!
   #endif
   }


   void application::on_request(::ca::create_context * pcreatecontext)
   {
      ::radix::application::on_request(pcreatecontext);
   }


} // namespace ex1