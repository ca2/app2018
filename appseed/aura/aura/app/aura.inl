#include "aura/aura/aura/aura.h"


#ifdef LINUX

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

const char * g_psz_br_init_symbol_app = "";

#define GET_COMMAND_LINE(RET, ARGC, ARGV) linux_get_command_line(RET, ARGC, ARGV)

class string;

void linux_get_command_line(string & str, int argc, char ** argv);

#endif // LINUX


#ifdef METROWIN

//#include "core/core/core.h"
//
//#undef CoreApplication
//

#undef new

[MTAThread]
int main(Array < String ^ > ^ refstra)
{

   class aura_aura aura;

   ap(aura_main_data) pmaindata = new aura_main_data(refstra);

#ifdef MAIN_STRING

   pmaindata->m_strCommandLine = MAIN_STRING;

#endif

   return (int)aura_aura(pmaindata);

}

#else


#undef new

int main(int argc, char * argv[])
{

//#ifdef LINUX
//
//   signal(SIGCHLD, SIG_IGN);
//
//   {
//
//      int fd = open("/dev/null",O_RDWR, 0);
//
//      if (fd != -1)
//      {
//
//         dup2 (fd, STDIN_FILENO);
//         dup2 (fd, STDOUT_FILENO);
//         dup2 (fd, STDERR_FILENO);
//
//         if (fd > 2)
//         {
//
//            close (fd);
//
//         }
//
//      }
//
//   }
//
//   /*resetting File Creation Mask */
//   umask(0);
//
//#endif

   class aura_aura aura;

   ap(aura_main_data) pmaindata = new aura_main_data(argc, argv);

#ifdef LINUX

   br_init_set_symbol(g_psz_br_init_symbol_app);

#endif


#ifdef MAIN_STRING

   pmaindata->m_strCommandLine = MAIN_STRING;

#endif


#ifdef GET_COMMAND_LINE

   GET_COMMAND_LINE(pmaindata->m_strCommandLine, argc, argv);

#endif // GET_COMMAND_LINE

   return (int)aura_aura(pmaindata);

}


#ifdef WINDOWS

BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   class aura_aura aura;

   ap(aura_main_data) pmaindata = new aura_main_data(hinstance, hPrevInstance, lpCmdLine, nCmdShow);

   return aura_aura(pmaindata);

}

END_EXTERN_C

#endif


#endif



#ifdef WINDOWS

#include "include/applink.cpp"

#endif


#define APP_PRELUDE_LEVEL(prefix) \
extern "C" \
::aura::application * prefix##_get_new_app(::aura::application * papp); \
aura_prelude auraprelude(&prefix##_get_new_app);


#define LIBRARY_PRELUDE(prefix) \
extern "C" \
::aura::library * prefix##_get_new_library(::aura::application * papp); \
aura_prelude auraprelude(&prefix##_get_new_library);


#ifdef LINUX

extern char _binary_data_txt_start;

extern char _binary_data_txt_end;

void linux_get_command_line(string & str, int argc, char ** argv)
{

   char*  p1 = &_binary_data_txt_start;

   char * p2 = &_binary_data_txt_end;

   string strAppId = string(p1, p2 - p1);

   strAppId.trim();

   str = string(argv[0]) + " : " + "app=" + strAppId;

}

#endif

