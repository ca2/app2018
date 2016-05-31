//#include "framework.h"


//#include <pthread.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <sys/ptrace.h>
//#include <sys/wait.h>

int32_t __node_is_debugger_attached()
{

   return gdb_check();

}



/*
int32_t gdb_check()
{
  int32_t pid = fork();
  int32_t status;
  int32_t res;

  if (pid == -1)
    {
      perror("fork");
      return -1;
    }

  if (pid == 0)
    {
      int32_t ppid = getppid();

      // Child
      if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) == 0)
        {
          // Wait for the parent to stop and continue it
          waitpid(ppid, NULL, 0);
          ptrace(PTRACE_CONT, NULL, NULL);

          // Detach
          ptrace(PTRACE_DETACH, getppid(), NULL, NULL);

          // We were the tracers, so gdb is not present
          res = 0;
        }
      else
        {
  // Trace failed so gdb is present
          res = 1;
        }
      exit(res);
    }
  else
    {
      waitpid(pid, &status, 0);
      res = WEXITSTATUS(status);
    }
  return res;
}
*/



static void*
test_trace(void* ignored)
{
	return (void*)ptrace(PTRACE_TRACEME, 0, NULL, NULL);
}

int32_t
gdb_check(void)
{
	pthread_attr_t attr;
	void* result;
	pthread_t thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (pthread_create(&thread, &attr, test_trace, NULL) != 0) {
		pthread_attr_destroy(&attr);
		return false;
	}
	pthread_attr_destroy(&attr);
	if (pthread_join(thread, &result) != 0) {
		return false;
	}

	return result != NULL;
}



//
//  macos_debug.cpp
//  ace
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


VOID
WINAPI
OutputDebugStringA(
                   LPCSTR lpOutputString
                   )
{


    //fprintf(stderr, "%s", lpOutputString);
    //fflush(stderr);
    printf("%s", lpOutputString);
    //fflush(stdout);


}




VOID
WINAPI
OutputDebugStringW(
                   LPCWSTR lpOutputString
                   )
{


    OutputDebugStringA(string(lpOutputString));


}
