//
//  ios_ip_enum.h
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 07/09/17.
//
//

/*
 Module : IPENUM.H
 Purpose: Interface for an ca2 API class wrapper for IP address enumeration
 Created: PJN / 21-04-1998
 History: None
 
 Copyright (c) 1998 by PJ Naughter.
 
 
 */


namespace ios
{
   
   
   class CLASS_DECL_AURA ip_enum :
   virtual public ::net::ip_enum
   {
   public:
      
      
      ip_enum(::aura::application * papp);
      virtual ~ip_enum();
      
      
      virtual ::count enumerate(stringa & stra);
      
      
   };
   
   
} // namespace ios




