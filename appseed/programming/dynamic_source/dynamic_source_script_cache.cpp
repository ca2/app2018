#include "framework.h"
#include "dynamic_source.h"


namespace dynamic_source
{


   script_cache::script_cache(::aura::application * papp) :
      ::object(papp)
   {

   }

   script_cache::~script_cache()
   {

   }


   script * script_cache::get(const char * lpcszName)
   {

      string strName(lpcszName);

#ifdef WINDOWS

      strName.replace("/", "\\");

#endif

      single_lock sl(&m_cs, TRUE);

      strsp(script)::pair * ppair = m_map.PLookup(strName);

      if (ppair != NULL && ppair->m_element2.is_set() && !ppair->m_element2->ShouldBuild())
      {

         return (script *)ppair->m_element2;

      }

      sp(script) pscript = canew(ds_script(get_app()));

      pscript->m_pmanager = m_pmanager;

      pscript->m_strName = strName;

      cache(pscript);

      return pscript;

   }


   script * script_cache::register_script(const char * lpcszName, script * pscript)
   {

      string strName(lpcszName);

#ifdef WINDOWS
      strName.replace("/", "\\");
#endif

      single_lock sl(&m_cs, TRUE);

      strsp(script)::pair * ppair = m_map.PLookup(strName);

      if(ppair != NULL)
      {

         ppair->m_element2 = pscript;

         return pscript;

      }

      pscript->m_pmanager = m_pmanager;

      pscript->m_strName = strName;

      cache(pscript);

      return pscript;

   }

   script_instance * script_cache::create_instance(const char * lpcszName, script * & pscript)
   {

      pscript = NULL;

      if(::str::begins(lpcszName, "netnode://"))
      {
         single_lock sl(&m_cs, TRUE);
         pscript  = get(lpcszName);
         sl.unlock();
         return pscript->create_instance();
      }

      //::output_debug_string(lpcszName);
      string strName(lpcszName);
      strName = m_pmanager->real_path(lpcszName);
      //::output_debug_string(strName);
      if(strName.is_empty())
         strName =  m_pmanager->real_path(string(lpcszName) + ".ds");
      //::output_debug_string(strName);
      strName.replace("\\", "/");
      single_lock sl(&m_cs, TRUE);
      m_pmanager->include_has_script(strName);
      pscript  = get(strName);
      sl.unlock();
      return pscript->create_instance();
   }


   void script_cache::cache(script * pscript)
   {

      single_lock sl(&m_cs, TRUE);

      m_map.set_at(pscript->m_strName, pscript);

   }


   void script_cache::set_all_out_of_date()
   {
      return;
//   single_lock sl(&m_cs, TRUE);
//   sp(script) pscript;
//   string strName;
//   POSITION pos = m_map.get_start_position();
//   while(pos != NULL)
//   {
//      m_map.get_next_assoc(pos, strName, pscript);
//
//      ds_script * pdsscript = dynamic_cast < ds_script * > (pscript.m_p);
//      if(pdsscript != NULL)
//      {
//         pdsscript->m_bShouldBuild = true;
//      }
//
//   }
   }


   void script_cache::set_out_of_date(string str)
   {

      if (str.is_empty())
      {

         return;

      }

      ::file::path pathChanged = str;

      single_lock sl(&m_cs, TRUE);

      for (auto & pair : m_map)
      {

         sp(ds_script) pdsscript = pair.m_element2;

         if (pdsscript != NULL)
         {

            ::file::path path = pdsscript->m_strSourcePath;

            if (path.ends_ci(pathChanged))
            {

               pdsscript->m_bShouldBuild = true;

            }

         }

      }

   }

} // namespace dynamic_source


