#include "framework.h"
#include "dynamic_source.h"


namespace dynamic_source
{


script_instance::script_instance(script * pscript) :
   ::object(pscript->get_app())
{
   m_pscript = pscript;
   m_pscriptScriptInstance = pscript;
   m_pscriptScriptInstance->m_scriptinstanceptra.add(this);
}

script_instance::~script_instance()
{
}

void script_instance::destroy()
{

   single_lock sl(m_pscriptScriptInstance->m_pmutex);
   
   try
   {

      sl.lock(minutes(1.0));

   }
   catch(...)
   {

   }

   try
   {

      m_pscriptScriptInstance->m_scriptinstanceptra.remove(this);

   }
   catch(...)
   {

   }

}



} // namespace dynamic_source



