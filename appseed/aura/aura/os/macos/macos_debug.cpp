//
//  macos_debug.cpp
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 2013-09-17.
//
//

//
//  macos_debug.cpp
//  ca
//
//  Created by Lion User on 26/05/2012.
//  Copyright (c) 2012 ca2 Desenvolvimento de Sofware Ltda. All rights reserved.
//
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#define user user_struct
#include <sys/sysctl.h>

int __node_is_debugger_attached()
// Returns true if the current process is being debugged (either
// running under the debugger or has a debugger attached post facto).
{
   int                 junk;
   int                 mib[4];
   struct kinfo_proc   info;
   size_t              size;
   
   // Initialize the flags so that, if sysctl fails for some bizarre
   // reason, we get a predictable result.
   
   info.kp_proc.p_flag = 0;
   
   // Initialize mib, which tells sysctl the info we want, in this case
   // we're looking for information about a specific process ID.
   
   mib[0] = CTL_KERN;
   mib[1] = KERN_PROC;
   mib[2] = KERN_PROC_PID;
   mib[3] = getpid();
   
   // Call sysctl.
   
   size = sizeof(info);
   junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
   assert(junk == 0);
   
   // We're being debugged if the P_TRACED flag is set.
   
   return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
}









DWORD
WINAPI
FormatMessage(
              DWORD dwFlags,
              LPCVOID lpSource,
              DWORD dwMessageId,
              DWORD dwLanguageId,
              LPSTR lpBuffer,
              DWORD nSize,
              va_list *Arguments
              )
{
    
    
    return 0;
    
    
}

void ns_log(const char * pszLog);


void output_debug_string(const char * pszOutputString)
{
   
   string strOutputString(pszOutputString);
   
   strOutputString.replace("\r\n", "\n");
   
   strOutputString.replace("\n", "\r");
   
   unichar32 * pusz = utf8_to_utf32(strOutputString);
   
   wprintf(L"%S", pusz);
   
   memory_free(pusz);
   
}




VOID
WINAPI
output_debug_string(
                   LPCWSTR lpOutputString
                   )
{
    
    
    output_debug_string(string(lpOutputString));
    
    
}


void throw_ns_exception()
{

   _throw(ns_exception());
   
}


