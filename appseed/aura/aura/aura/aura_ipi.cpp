#include "framework.h"


namespace aura
{



   ipi::ipi(::aura::application * papp,const string & strApp):
      ::object(papp),
      m_rx(papp)
   {
      
      m_id = "::aura::ipi";
      
      defer_create_mutex();
      
      //children_add(&m_rx);

      m_strApp          = strApp;

      int iPid = System.os().get_pid();

      defer_add_module(System.file().module(), iPid);

      ::file::path p;

#ifndef METROWIN

      p = module_path_from_pid(iPid);

#endif

      defer_add_module(p, iPid);

      m_rx.m_preceiver  = this;

      string strKey = key(strApp, iPid);

      if(!m_rx.create(strKey))
      {
         
         _throw(::resource_exception(papp));
         
      }

   }


   ipi::~ipi()
   {


   }
   
   
   void ipi::safe_pre_term()
   {
      
      ::object::safe_pre_term();
      
      m_rx.safe_pre_term();
      
   }


   var ipi::call(const string & strApp,int iPid, const string & strObject,const string & strMember,var_array & va, duration duration)
   {

      ::aura::ipc::tx & txc = tx(strApp,iPid);

      string strVara = str_from_va(va);

      bool bSendOk = txc.send("call " + strObject + "." + strMember + " : " + strVara, duration);

      return bSendOk;

   }


   int_map < var > ipi::scall(bool bAutoLaunch, const string & strApp,const string & strObject,const string & strMember,var_array & va, duration duration)
   {

      int_map < var > map;

      int_array iaPid = get_pid(strApp);

      if(iaPid.get_count() <= 0)
      {

         if(!bAutoLaunch)
         {

            return map;

         }

         start(strApp);

         iaPid = get_pid(strApp);

      }

      for(int iPid : iaPid)
      {

         map[iPid] = call(strApp,iPid,strObject,strMember,va, duration);

      }

      return map;

   }

   int_map < var > ipi::call(const string & strApp,const string & strObject,const string & strMember,var_array & va, duration durationTimeout)
   {

      int_map < var > map;

      int_array iaPid = get_pid(strApp);

      for(int iPid : iaPid)
      {

         map[iPid] = call(strApp, iPid, strObject, strMember, va, durationTimeout);

      }

      return map;

   }
   int_map < var > ipi::call(const stringa & straApp, const string & strObject, const string & strMember, var_array & va, duration durationTimeout)
   {

      int_map < var > map;

      stringa straApp2;

      int_array iaPid2;

      for (index i = 0; i < straApp.get_size(); i++)
      {

         int_array iaPid = get_pid(straApp[i]);

         for (index j = 0; j < iaPid.get_size(); j++)
         {

            int iPid = iaPid[j];

            if (iaPid2.add_unique(iPid))
            {

               string strApp = straApp[i];

               straApp2.add(strApp);

            }

         }

      }

      for (index i = 0; i < straApp2.get_size(); i++)
      {

         int iPid = iaPid2[i];

         string strApp = straApp2[i];

         map[iPid] = call(strApp, iPid, strObject, strMember, va, durationTimeout);

      }

      return map;

   }


   int_map < var > ipi::ecall(const string & strApp,int_array iaExcludePid, const string & strObject,const string & strMember,var_array & va, duration durationTimeOut)
   {

      int_map < var > map;

      int_array iaPid = get_pid(strApp);

      iaPid -= iaExcludePid;

      for(int iPid : iaPid)
      {

         map[iPid] = call(strApp,iPid,strObject,strMember,va, durationTimeOut);

      }

      return map;

   }


   bool ipi::start(const string & strApp)
   {

      synch_lock sl1(m_pmutex);

      mutex * pmutex = m_mapAppMutex[strApp];

      if(pmutex == NULL)
      {

         pmutex = canew(mutex(get_app()));

         m_mapAppMutex[strApp] = pmutex;

      }

      sl1.unlock();

      synch_lock sl(pmutex);

      int_array iaPid = get_pid(strApp);

      if(iaPid.get_count() > 0)
      {

         return true;

      }


      //throw todo(get_app());

      ::aura::app_launcher launcher(process_platform_dir_name2(), strApp);

      int iPid = -1;

      {

         int_array ia = get_pid(strApp);

         if(ia.get_count() <= 0)
         {

            launcher.start();

            int iStep = 0;

            int iSubStep;

            while(iStep < 8 && ::get_thread_run())
            {

               iStep++;

               ia = get_pid(strApp);

               if(ia.get_count() > 0)
               {

                  goto started;

               }

               for(iSubStep = 0; (iSubStep < (iStep + 1) * 10) && ::get_thread_run(); iSubStep++)
               {

                  Sleep(100);

               }

            }

            return false;

         }

started:

         iPid = ia[0];

      }

      string strKey = strApp + ":" + ::str::from(iPid);

      if(m_txmap[strKey].is_null())
      {

         m_txmap[strKey] = canew(::aura::ipc::tx(get_app()));

      }

      return m_txmap[strKey]->open(key(strApp,iPid));

   }


   bool ipi::connect(const string & strApp, int iPid)
   {

      string strKey = strApp + ":" + ::str::from(iPid);

      if(m_txmap[strKey].is_null())
      {

         m_txmap[strKey] = canew(::aura::ipc::tx(get_app()));

      }

      if(m_txmap[strKey]->is_tx_ok())
      {

         return true;

      }
      else
      {

         return m_txmap[strKey]->open(key(strApp, iPid));

      }

   }


   ::aura::ipc::tx & ipi::tx(const string & strApp, int iPid)
   {

      string strKey = strApp + ":" + ::str::from(iPid);

      if(m_txmap[strKey].is_null())
      {

         m_txmap[strKey] = canew(::aura::ipc::tx(get_app()));

      }

      if(!m_txmap[strKey]->is_tx_ok())
      {

         connect(strApp, iPid);

      }

      return *m_txmap[strKey];

   }


   string ipi::key(const string &strApp, int iPid)
   {

      string strKey;

#ifdef WINDOWS

#ifdef METROWIN

      string strAppId(strApp);

      strAppId.replace("\\","-");

      strAppId.replace("/","-");

      strAppId.replace("-","-");

      strKey = strAppId;

#else

      strKey = "::ca2::fontopus::ccvotagus::" + strApp + ":" + iPid;

#endif

#else

#ifdef LINUX

      strKey = ::dir::system() / "ipi" / strApp / ::str::from(iPid);

#else

#ifdef APPLE_IOS

      string strAppId(strApp);

      strAppId.replace("\\","-");

      strAppId.replace("/","-");

      strAppId.replace("-","-");

      strKey = "core-" + strAppId;

#else


      strKey = ::dir::system() / "ipi" / strApp / ::str::from(iPid);

#endif

#endif

#endif

      return strKey;


   }

   string ipi::str_from_va(var_array & va)
   {

      memory_file m(get_app());

      serialize serialize(&m);

      m.m_nOpenFlags = ::file::mode_write;

      serialize.stream_array(va);

      return m.get_primitive_memory()->to_hex();

      //if(va.get_count() <= 0)
      //{

      //   return ";";

      //}

      //string str;

      //for(var & v : va)
      //{

      //   str += v.get_string();

      //   str += ";";

      //}

      //return str;

   }

   void ipi::on_receive(::aura::ipc::rx * prx,const char * pszMessage)
   {

      string str(pszMessage);

      if(!::str::begins_eat(str,"call "))
         return;

      strsize iFind = str.find(":");

      string str1;
      string strObject;
      string strMember;
      stringa stra;
      var_array va;


      if(iFind < 0 || iFind > 3)
      {
         if(iFind > 3)
         {
            str1 = str.Left(iFind);
         }
         else
         {
            str1 = str;
         }

         str1.trim();

         strsize iFind2 = str1.find(".");

         if(iFind2 < 0)
         {

            return;

         }

         strObject = str1.Left(iFind2);
         strMember = str1.Mid(iFind2 + 1);
         if(iFind < 0)
         {
            on_call(strObject,strMember,va);
            return;
         }
      }
      else
      {
         return;
      }

      str1 = str.Mid(iFind + 1);



      memory_file m(get_app());

      str1.trim();

      m.get_primitive_memory()->from_hex(str1);

      m.seek_to_begin();

      serialize is(&m);

      m.m_nOpenFlags = ::file::mode_read;

      is.stream_array(va);

      //return
      //stra.explode(";",str1);

      //if(stra.has_elements())
      //{

      //   stra.remove_last();

      //}

      //stra.trim();

      //va = stra;

      on_call(strObject,strMember,va);

   }


   void ipi::on_call(const string & strObject,const string & strMember,var_array & va)
   {

      if(strObject == "application")
      {

         if(strMember == "on_exclusive_instance_local_conflict")
         {

            Application.on_exclusive_instance_local_conflict(va[0],va[1],va[2]);

         }
         else if (strMember == "on_new_instance")
         {

            on_new_instance(va[0], va[1]);

         }

      }

   }

   void ipi::on_new_instance(const string & strModule, int iPid)
   {

      defer_add_module(strModule, iPid);

      Application.on_new_instance(strModule, iPid);

   }


   int_array ipi::get_pid(const string & strApp)
   {

      int_array iaPid;

#if defined(LINUX) || defined(MACOS)


      iaPid = app_get_pid(strApp);


#else

#if defined(METROWIN)

      iaPid.add(1);

#else

      stringa stra;

      ::file::path pathModule;

      pathModule = ::dir::system() / "ipi";

      pathModule /= strApp + ".module_list";

      string strModuleList = file_as_string_dup(pathModule);

      stra.add_lines(strModuleList);

repeat:

      if (stra.get_count() > 32)
      {

         stra.remove_at(0, 16);

      }

      stringa stra2;

      int_array iaPid2;

      for (auto & str : stra)
      {

         if (str.has_char())
         {

            stringa a;

            a.explode("|", str);

            if (a.get_size() >= 2)
            {

               stra2.add_unique_ci(a[0]);

               string strPath = module_path_from_pid(atoi_dup(a[1]));

               if (strPath.has_char())
               {

                  if (strPath.compare_ci(a[0]) == 0)
                  {

                     iaPid.add(atoi_dup(a[1]));

                  }

               }

            }

         }

      }

      if (iaPid.get_count() <= 0 && stra.get_size() > 32)
      {

         goto repeat;

      }
      //for(auto & str : stra2)
      //{

      //   if(str.has_char())
      //   {

      //          iaPid.add_unique(module_path_get_pid(str));
//
      //   }

      //}

#endif
#endif
      return iaPid;

   }


   void ipi::defer_add_module(const string & strModule, int iPid)
   {

#ifndef METROWIN

      ::file::path pathModule;

      m_straModule.remove_all();

      pathModule = ::dir::system() / "ipi";

      pathModule /= m_strApp + ".module_list";

      ::file::path pathPid = module_path_from_pid(iPid);

      string strModuleList = file_as_string_dup(pathModule);

      m_straModule.add_lines(strModuleList);

      stringa stra2;

      int_array iaPid2;

      for (index i = 0; i < m_straModule.get_count();)
      {

         string str = m_straModule[i];

         str.trim();

         bool bOk = false;

         if (str.has_char())
         {

            stringa a;

            a.explode("|", str);

            if (a.get_size() >= 2)
            {

               stra2.add_unique_ci(a[0]);

               string strPath = module_path_from_pid(atoi_dup(a[1]));

               if (strPath.has_char())
               {

                  if (strPath.compare_ci(a[0]) == 0)
                  {

                     bOk = true;

                  }

               }

            }

         }

         if (bOk)
         {

            i++;

         }
         else
         {

            m_straModule.remove_at(i);

         }

      }



      stringa straUnique;

      forallref(m_straModule)
      {

         straUnique.add_unique_ci(item);

      }

      m_straModule = straUnique;

      ::file::path pathThisModule = System.file().module();

      string strItem;

      if (pathPid.has_char())
      {
         strItem = pathPid + "|" + ::str::from(iPid);

      }
      else
      {

         strItem = strModule + "|" + ::str::from(iPid);
      }


      m_straModule.add_unique_ci(strItem);

      strModuleList = m_straModule.implode("\n");

      file_put_contents_dup(pathModule,strModuleList);

#endif

   }

} // namespace aura


