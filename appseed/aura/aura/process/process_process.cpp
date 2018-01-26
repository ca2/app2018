#include "framework.h"


void prepare_argc_argv(int & argc, char ** argv, char * cmd_line);


namespace process
{


   exit_status::exit_status()
   {

      m_iExitCode = 0;
      m_iExitSignal = 0;
      m_iExitStop = 0;

   }


   process::process(::aura::application * papp):
      object(papp),
      m_pipe(papp)
   {

      m_bPiped = false;
      m_iPid = 0;

   }


   process::~process()
   {


   }


   bool process::create_child_process(const char * pszCmdLine,bool bPiped,const char * pszDir,int32_t iCa2Priority)
   {

      if(bPiped)
      {

         if(!m_pipe.create(false, true))
            return false;

      }

      m_bPiped = bPiped;

      return true;

   }


   bool process::write(const char * psz)
   {

      return m_pipe.m_sppipeIn->write(psz);

   }

   string process::read(bool bUntilExit)
   {

      UNREFERENCED_PARAMETER(bUntilExit);

      return m_pipe.m_sppipeOut->read();

   }


   void process::wait_until_exit(int32_t iWaitMax)
   {

      uint32_t dwStartTime = ::get_tick_count();

      int32_t i = 1;

      while(true)
      {

         if(has_exited())
         {

            break;

         }

         if(iWaitMax >= 0 && get_tick_count() > dwStartTime + iWaitMax)
         {

            break;

         }

         Sleep(100);

         i++;

      }

   }


   bool process::has_exited()
   {

      return true;

   }


   void process::synch_elevated(const char * pszCmdLine,int iShow,const ::duration & durationTimeOut,bool * pbTimeOut)
   {

   }


   bool process::kill()
   {

      return false;

   }


   string app_id_to_app_name(string strId)
   {

      string strName;

      for (index i = 0; i < strId.length(); i++)
      {

         if (strId[i] == '-' || strId[i] == '/' || strId[i] == '\\')
         {

            strName += "_";

         }
         else
         {

            strName += strId[i];

         }

      }

      return strName;

   }


} // namespace process




