#include "framework.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>


int create_process(const char * _cmd_line, int * pprocessId)
{
   char *   exec_path_name;
   char *	cmd_line;

   cmd_line = (char *) ca2_alloc(strlen(_cmd_line ) + 1 );

   if(cmd_line == NULL)
            return 0;

   strcpy_dup(cmd_line, _cmd_line);

   if((*pprocessId = fork()) == 0)
   {
      // child
      char		*pArg, *pPtr;
      char		*argv[1024 + 1];
      int		 argc;
      if( ( pArg = strrchr_dup( exec_path_name, '/' ) ) != NULL )
         pArg++;
      else
         pArg = exec_path_name;
      argv[0] = pArg;
      argc = 1;

      if( cmd_line != NULL && *cmd_line != '\0' )
      {
         pArg = strtok_r_dup(cmd_line, " ", &pPtr);
         while( pArg != NULL )
         {
            argv[argc] = pArg;
            argc++;
            if( argc >= 1024 )
               break;
            pArg = strtok_r_dup(NULL, " ", &pPtr);
         }
      }
      argv[argc] = NULL;

      execv(exec_path_name, argv);
      free(cmd_line);
      exit( -1 );
   }
   else if(*pprocessId == -1)
   {
      // in parent, but error
      *pprocessId = 0;
      free(cmd_line);
      return 0;
   }
   // in parent, success
   return 1;
}

CLASS_DECL_c int call_async(
                            const char * pszPath, 
                            const char * pszParam, 
                            const char * pszDir,
                            int iShow)
{
    vsstring strCmdLine;
    
    strCmdLine = pszPath;
    if(strlen_dup(pszParam) > 0)
    {
        strCmdLine +=  " ";
        strCmdLine += pszParam;
    }
    
    int processId;
    
    if(!create_process(strCmdLine, &processId))
        return -1;
    
    return 0;
    
}

CLASS_DECL_c DWORD call_sync(
                             const char * pszPath, 
                             const char * pszParam, 
                             const char * pszDir,
                             int iShow,
                             int iRetry, 
                             int iSleep, 
                             int (* pfnOnRetry)(int iTry, uint_ptr dwParam),
                             uint_ptr dwParam)
{

   vsstring strCmdLine;
    
   strCmdLine = pszPath;
   
   if(strlen_dup(pszParam) > 0)
   {
   
      strCmdLine +=  " ";
      strCmdLine += pszParam;
   }
    
   int processId;
    
   if(!create_process(strCmdLine, &processId))
      return -1;
    
    
   while(true)
   {
   
      if(kill(processId, 0) == -1 && errno == ESRCH) // No process can be found corresponding to processId
         break;
   
      sleep(1);
      
   }
   
   return 0;
   
}





