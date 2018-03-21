#include "framework.h"


namespace aura
{


   void application::_001EnableShellOpen()
   {

   }


   string application::get_version()
   {

      return "1.0.0";

   }


   bool application::impl_process_init()
   {

      return true;

   }


   bool application::impl_init1()
   {

      return true;

   }


   bool application::impl_init2()
   {

      return true;

   }


   bool application::impl_init3()
   {

      return true;

   }

   void application::impl_process_term()
   {

      set_os_data(NULL);

   }


   void application::impl_term1()
   {

   }


   void application::impl_term2()
   {

   }


   void application::impl_term3()
   {

   }


   void application::ShowWaitCursor(bool bShow)
   {


   }


   void application::get_time(struct timeval *p)
   {

      gettimeofday(p, NULL);

   }


   void application::set_env_var(const string & var,const string & value)
   {

      setenv(var, value, 1);

   }


   IDTHREAD application::get_thread_id()
   {

      return ::pthread_self();

   }


   bool application::os_on_start_application()
   {

      return true;

   }


   bool application::process_command(::command::command * pdata)
   {

      m_pcommand =  pdata;

      string strCmdLine          = pdata->m_strCommandLine;

      SetCurrentHandles();

      __init_thread();

      return true;

   }


   string application::draw2d_get_default_library_name()
   {

      return "draw2d_cairo";

   }


   string application::multimedia_audio_get_default_library_name()
   {

      return "audio_opensles";

   }


   string application::multimedia_audio_mixer_get_default_library_name()
   {

      return "audio_mixer_opensles";

   }


   string application::veriwell_multimedia_music_midi_get_default_library_name()
   {

      return "";

   }


} // namespace android




