/*
Module : IPENUM.H
Purpose: Interface for an ca2 API class wrapper for IP address enumeration
Created: PJN / 21-04-1998
History: None

Copyright (c) 1998 by PJ Naughter.


*/


namespace linux
{


   class CLASS_DECL_LINUX ip_enum :
      virtual public ::net::ip_enum
   {
   public:


      ip_enum(sp(::base::application) papp);
      virtual ~ip_enum();


      virtual ::count enumerate(stringa & stra);


   };


} // namespace linux




