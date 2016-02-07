//#include "framework.h"


command_thread::command_thread(::aura::application * papp) :
   object(papp),
   m_mutex(papp),
   m_ev(papp)
{

   m_varTopicQuery.propset().set_app(papp);

}

command_thread::~command_thread()
{
}


var command_thread::run()
{
   single_lock sl(&m_mutex);
   sp(::create) pcreatecontext;
   sp(primitive::command) spcommand;
   while(m_ptra.get_size() > 0)
   {
      spcommand = NULL;
      sl.lock();
      try
      {
         spcommand = m_ptra.element_at(0);
         m_ptra.remove_at(0);
      }
      catch(...)
      {
      }
      sl.unlock();

      if(spcommand.is_null())
         continue;

      pcreatecontext = NULL;
      try
      {
         pcreatecontext = spcommand;
      }
      catch(...)
      {
         pcreatecontext = NULL;
      }

      if(pcreatecontext != NULL)
      {
         try
         {
            ::output_debug_string("command_thread::run - on_request : appid : " + pcreatecontext->m_spCommandLine->m_strApp + "\n\n");
            on_request(pcreatecontext);
         }
         catch(::exit_exception & e)
         {

            throw e;

         }
         catch(::exception::exception & e)
         {

            if(!Application.on_run_exception(e))
               throw exit_exception(get_app());

         }
         catch(...)
         {
         }
      }


      sl.lock();
      try
      {
         m_ptraHistory.add(spcommand);
         m_straHistory.add(spcommand->get_description());
      }
      catch(...)
      {
      }
      sl.unlock();

   }

   m_ev.ResetEvent();

   return true;

}


void command_thread::request_create(sp(::create) pline)
{

   single_lock sl(&m_mutex, TRUE);

   m_ptra.add(pline);

   m_ev.SetEvent();

}


void command_thread::on_request(sp(::create) pline)
{
   try
   {
      Application.request_create(pline);
   }
   catch(::exit_exception & e)
   {

      throw e;

   }
   catch(::exception::exception & e)
   {

      if(!Application.on_run_exception(e))
         throw exit_exception(get_app());

   }
   catch(...)
   {
   }
}

void command_thread::consolidate(::create * pcreatecontext)
{
   m_spcommandline = pcreatecontext->m_spCommandLine;
   if(!pcreatecontext->m_spCommandLine->m_varFile.is_empty())
   {
      if(m_varTopicFile != pcreatecontext->m_spCommandLine->m_varFile)
      {
         if(m_varTopicFile.is_empty())
         {
            m_varTopicFile = pcreatecontext->m_spCommandLine->m_varFile;
         }
         else
         {
            m_varTopicFile.stra().add(pcreatecontext->m_spCommandLine->m_varFile.stra());
         }
      }
   }
   m_varTopicQuery.propset().merge(pcreatecontext->m_spCommandLine->m_varQuery.propset());
}

void command_thread::consolidate(::command_thread * pthread)
{

   if(pthread == NULL || pthread == this)
      return;

   if(!pthread->m_varTopicFile.is_empty())
   {
      m_varTopicFile.stra().add_unique(pthread->m_varTopicFile.stra());
   }
   m_varTopicQuery.propset().merge(pthread->m_varTopicQuery.propset());

}

bool command_thread::is_thread()
{
   return true;
}

var & command_thread::property(const char * pszKey)
{
   string strKey(pszKey);
   if(strKey.CompareNoCase("file") == 0)
   {
      if(m_varTopicFile.is_empty())
      {
         if(!m_varTopicQuery["file"].is_empty())
         {
            m_varTopicFile = m_varTopicQuery["file"];
         }
      }
      return m_varTopicFile;
   }
   else
   {
      return m_varTopicQuery[pszKey];
   }
}

bool command_thread::has_property(const char * pszKey)
{
   string strKey(pszKey);
   if(strKey.CompareNoCase("file") == 0)
   {
      if(m_varTopicFile.is_empty())
      {
         if(m_varTopicQuery["file"].is_empty())
         {
            return false;
         }
      }
      return true;
   }
   else
   {
      return m_varTopicQuery.has_property(pszKey);
   }

}




