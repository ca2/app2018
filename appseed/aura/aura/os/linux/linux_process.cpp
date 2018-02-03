#include "framework.h"

#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <spawn.h>
#include <sys/stat.h>


int create_process6(const char * _cmd_line, int * pprocessId);

CLASS_DECL_AURA void dll_processes(uint_array & dwa, stringa & straProcesses, const char * pszDll)
{

   UNREFERENCED_PARAMETER(dwa);
   UNREFERENCED_PARAMETER(straProcesses);
   UNREFERENCED_PARAMETER(pszDll);

}




int32_t create_process(const char * pszCommandLine, int32_t * pprocessId)
{

   stringa stra;

   stra = get_c_args_for_c(pszCommandLine);

   pointer_array < char * > argv;

   for(auto & str : stra)
   {

      argv.add((char *) str.c_str());

   }

   argv.add(NULL);

   pid_t pid = 0;

   int status = posix_spawn(&pid, argv[0], NULL, NULL, argv.get_data(), environ);

   int iError = errno;

   char * pszError = strerror(iError);

   if (status == 0)
   {

      if(pprocessId != NULL)
      {

         *pprocessId = pid;

      }

      return 1;

   }
   else
   {

      return 0;

   }

}


int32_t create_process3(const char * _cmd_line, int32_t * pprocessId)
{

   char *   exec_path_name;

   char *	cmd_line;

   char *	cmd_line2;

   cmd_line = strdup(_cmd_line);

   if(cmd_line == NULL)
   {

      return 0;

   }

   char *      argv[1024 + 1];

   int32_t		argc = 0;

   prepare_argc_argv(argc, argv, cmd_line);

   pid_t pid;

   int status;

   status = posix_spawn(&pid, argv[0], NULL, NULL, argv, environ);

   free(cmd_line);

   if (status == 0)
   {

      return 1;

   }
   else
   {

      return 0;

   }

}


int32_t daemonize_process(const char * pszCommandLine, int32_t * pprocessId)
{

   stringa stra;

   stra = get_c_args_for_c(pszCommandLine);

   char ** argv = (char **) malloc(sizeof(char *) * (stra.get_size() + 1));

   int argc = 0;

   for(auto & str : stra)
   {

      argv[argc] = strdup((char *) str.c_str());

      argc++;

   }

   argv[argc] = NULL;

   pid_t pid;

   pid = fork();

   if (pid == -1)
   {

      printf("fork error\n");

      char ** pargv = argv;

      while(*pargv != NULL)
      {

         free(*pargv);

         pargv++;

      }

      free(argv);

      return 0;

   }
   else if(pid > 0)
   {

      return 1;

   }

   daemon(0, 0);

//   signal(SIGCHLD, SIG_IGN);
//
//   umask(0);
//
//   int sid = setsid();
//
//   if (sid < 0)
//   {
//
//      exit(EXIT_FAILURE);
//
//   }
//
//    /* Change the current working directory.  This prevents the current
//       directory from being locked; hence not being able to remove it. */
//   if ((chdir("/")) < 0)
//   {
//
//      exit(EXIT_FAILURE);
//
//   }
//
//   /* Redirect standard files to /dev/null */
//   freopen( "/dev/null", "r", stdin);
//   freopen( "/dev/null", "w", stdout);
//   freopen( "/dev/null", "w", stderr);

   int iExitCode = execv(argv[0], argv);

   char ** pargv = argv;

   while(*pargv != NULL)
   {

      free(*pargv);

      pargv++;

   }

   free(argv);

   exit(iExitCode);


   return 0;

}


int32_t create_process4(const char * pszCommandLine, int32_t * pprocessId)
{

   stringa stra;

   stra = get_c_args_for_c(pszCommandLine);

   char ** argv = (char **) malloc(sizeof(char *) * (stra.get_size() + 1));

   int argc = 0;

   for(auto & str : stra)
   {

      argv[argc] = strdup((char *) str.c_str());

      argc++;

   }

   argv[argc] = NULL;

   if((*pprocessId = fork()) == 0)
   {

      execv(argv[0], argv);

      int status = 0;

      wait(&status);

      char ** pargv = argv;

      while(*pargv != NULL)
      {

         free(*pargv);

         pargv++;

      }

      free(argv);

      exit(status);

   }
   else if(*pprocessId == -1)
   {

      *pprocessId = 0;

      char ** pargv = argv;

      while(*pargv != NULL)
      {

         free(*pargv);

         pargv++;

      }

      free(argv);

      return 0;

   }

   return 1;

}


CLASS_DECL_AURA int32_t call_async(const char * pszPath, const char * pszParam, const char * pszDir, int32_t iShow, bool bPrivileged, unsigned int * puiPid)
{

   string strCmdLine;

   strCmdLine = pszPath;

   if(strlen_dup(pszParam) > 0)
   {

      strCmdLine +=  " ";

      strCmdLine += pszParam;

   }

   int32_t processId;

   if(!create_process(strCmdLine, &processId))
   {

      if(puiPid != NULL)
      {

         *puiPid = -1;

      }

      return -1;

   }

   if(puiPid != NULL)
   {

      *puiPid = processId;

   }

   return 0;

}


CLASS_DECL_AURA DWORD call_sync(const char * pszPath, const char * pszParam, const char * pszDir, int32_t iShow, int32_t iRetry, int32_t iSleep, PFNCALLSYNCONRETRY pfnOnRetry, uint_ptr dwParam, unsigned int * puiPid)
{

   string strCmdLine;

   strCmdLine = pszPath;

   if(strlen_dup(pszParam) > 0)
   {

      strCmdLine +=  " ";

      strCmdLine += pszParam;

   }

   int32_t processId;

   if(!create_process(strCmdLine, &processId))
   {

      if(puiPid != NULL)
      {

         *puiPid = -1;

      }

      return -1;

   }

   while(true)
   {

      if(kill(processId, 0) == -1 && errno == ESRCH) // No process can be found corresponding to processId
      {

         break;

      }

      sleep(1);

   }

   if(puiPid != NULL)
   {

      *puiPid = processId;

   }

   return 0;

}


string module_path_from_pid(unsigned int iPid)
{

   struct stat sb;

   int iSize;

   string str;

   str = "/proc/" + ::str::from(iPid) + "/exe";

   memory mem;

   ssize_t s;

   bool iTry;

   if(lstat(str, &sb) == -1)
   {

retry:

      iSize = 1024 * 4;

      iTry = 1;

      sb.st_size = iSize - 1;

   }
   else
   {

      iSize = sb.st_size + 1;

      iTry = 0;

   }
#if MEMDLEAK
   mem.m_strTag = "memory://function=module_path_from_pid";
#endif
   mem.allocate(iSize);

   s = readlink (str, (char *) mem.get_data(), iSize);

   if(s > sb.st_size)
   {

      if(iTry <= 0)
      {

         goto retry;

      }
      else
      {

         return "";

      }

   }

   mem.get_data()[s] = '\0';

   return (const char *) mem.get_data();

}


int_array module_path_get_pid(const char * pszPath)
{

   int_array ia;

   ::file::patha stra;

   ::dir::ls_dir(stra, "/proc/");

   for(auto & strPid : stra)
   {

      int iPid = atoi(strPid.title());

      if(iPid > 0)
      {

         string strPath =module_path_from_pid(iPid);

         if(strPath	 == pszPath)
         {

            ia.add(iPid);

         }

      }

   }

   return ia;

}


int_array app_get_pid(const char * psz)
{

   int_array ia;

   ::file::patha stra;

   ::dir::ls_dir(stra, "/proc/");

   string str(psz);

   str = "app=" + str;

   string strApp(psz);

   strApp.replace("-", "_");

   strApp.replace("/", "_");

   for(auto & strPid : stra)
   {

      int iPid = atoi(strPid.title());

      if(iPid > 0)
      {

         ::file::path path = module_path_from_pid(iPid);

         if(path.has_char())
         {

            output_debug_string(path + "\n");

         }

         if(path.title() == strApp)
         {

            ia.add(iPid);

         }
         else
         {

            stringa straCmdLine = cmdline_from_pid(iPid);

            string strCmdLine;

            strCmdLine = straCmdLine.implode(" ");

            if(straCmdLine.find_first(str) > 0)
            {

               ia.add(iPid);

            }

         }

      }

   }

   return ia;

}


stringa cmdline_from_pid(unsigned int iPid)
{

   stringa stra;

   string str;

   str = "/proc/" + ::str::from(iPid) + "/cmdline";

   memory mem = file_as_memory_dup(str);

   string strArg;

   char ch;

   for(int i = 0; i < mem.get_size(); i++)
   {

      ch = (char) mem.get_data()[i];

      if(ch == '\0')
      {

         stra.add(strArg);

         strArg.Empty();

      }
      else
      {

         strArg += ch;

      }


   }

   if(strArg.has_char())
   {

      stra.add(strArg);

   }

   return stra;

   /* the easiest case: we are in linux */
//    ssize_t s = readlink (str, path, iSize);

   //  if(s == -1)
   //{
   // return "";
   //}

   //path[s] = '\0';

   //return path;

}


bool shell_execute_sync(const char * pszFile, const char * pszParams, ::duration durationTimeout )
{

   return call_sync(pszFile, pszParams, ::file::path(pszFile).folder(), 0, false, durationTimeout.get_total_milliseconds());

}


CLASS_DECL_AURA bool is_shared_library_busy(uint32_t processid, const stringa & stra)
{

   return false;

}


CLASS_DECL_AURA bool is_shared_library_busy(const stringa & stra)
{

   return false;

}


CLASS_DECL_AURA int32_t ca2_main();



int create_process2(const char * pszCommandLine, int * pprocessId)
{

   stringa stra;

   stra = get_c_args_for_c(pszCommandLine);

   char ** argv = (char **) malloc(sizeof(char *) * (stra.get_size() + 1));

   int argc = 0;

   for(auto & str : stra)
   {

      argv[argc] = strdup((char *) str.c_str());

      argc++;

   }

   argv[argc] = NULL;

   pid_t pid = 0;

   if((pid = fork()) == 0) // child
   {

      int iExitCode = execv(argv[0], argv);

      char ** pargv = argv;

      while(*pargv != NULL)
      {

         free(*pargv);

         pargv++;

      }

      free(argv);

      exit(iExitCode);

   }
   else if(pid == -1) // in parent, but error
   {

      char ** pargv = argv;

      while(*pargv != NULL)
      {

         free(*pargv);

         pargv++;

      }

      free(argv);

      return 0;

   }

   if(pprocessId != NULL)
   {

      *pprocessId = pid;

   }

   return 1;

}




