//
//  apple_debug.cpp
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 06/01/18.
//

#include "apple_debug.h"

void throw_ns_exception()
{
   
   _throw(ns_exception());
   
}


char * mm_error_string(OSStatus status);

char * mm_error_description(OSStatus status);


string apple_error_string(OSStatus status)
{
   
   return str::from_strdup(mm_error_string(status));
   
}


string apple_error_description(OSStatus status)
{
   
   return str::from_strdup(mm_error_description(status));
   
}


