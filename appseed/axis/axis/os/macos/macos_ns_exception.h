//
//  macos_ns_exception.h
//  axis
//
//  Created by Thomas Borregaard Sørensen on 18/10/13.
//
//


#pragma once


#if defined(__cplusplus) 

class ns_exception
{
public:

   int m_iCode;
   
   ns_exception();
   ~ns_exception();

};

#endif



void InstallUncaughtExceptionHandler();
void InstallNullExceptionHandler();




