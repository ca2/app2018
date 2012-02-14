#include "StdAfx.h"
#include "mysql/mysql.h"



namespace simpledb
{


   application::application()
   {
      m_pserver      = NULL;
   }



   bool application::InitializeDataCentral()
   {

      if(is_system())
      {
         /* initialize client library */
         if (mysql_library_init (0, NULL, NULL))
         {
            TRACE("mysql_library_init() failed\n");
            return false;
         }
      }

      m_pserver = new db_server(this);


      m_pserver->add_client(this);

      if(!m_pserver->initialize())
      {
         simple_message_box(NULL, "Could not initialize simpledb.", MB_OK);
         return false;
      }

      return true;
   }

   void application::on_set_locale(const char * lpcsz, bool bUser)
   {
      if(bUser)
      {
         data_set("locale", lpcsz);
      }
      ::database::application::on_set_locale(lpcsz, bUser);
   }

   void application::on_set_style(const char * lpcsz, bool bUser)
   {
      if(bUser)
      {
         data_set("style", lpcsz);
      }
      ::database::application::on_set_style(lpcsz, bUser);
   }

   bool application::FinalizeDataCentral()
   {

      try
      {
         m_pserver->finalize();
      }
      catch(...)
      {
      }

      try
      {
         delete m_pserver;
      }
      catch(...)
      {
      }

      if(is_system())
      {

         m_pserver = NULL;

         try
         {
            mysql_library_end();
         }
         catch(...)
         {
         }

      }

      return true;
   }


   bool application::initialize()
   {

      if(!database::application::initialize())
         return false;

      if(command().m_varTopicQuery["locale"].get_string().has_char())
      {
         string str = command().m_varTopicQuery["locale"];
         set_locale(str, false);
      }

      if(command().m_varTopicQuery["style"].get_string().has_char())
      {
         string str = command().m_varTopicQuery["style"];
         set_style(str, false);
      }

//      if(&AppUser(this) == NULL)
  //       return false;

      if(!InitializeDataCentral())  
      {
         simple_message_box(NULL, "Could not initialize data central"); 
         return false; 
      }
      ::database::client::initialize(this);

//      ::ca2::application_request * prequest = System.get_application_request();

      string str;
      // if system locale has changed (compared to last recorded one by ca2)
      // use the system locale
      if(data_get("system_locale", str))
      {
         if(str.has_char())
         {
            if(str != get_locale())
            {
               try
               {
                  data_set("system_locale", get_locale());
                  data_set("locale", get_locale());
               }
               catch(...)
               {
               }
            }
         }
      }
      else
      {
         data_set("system_locale", get_locale());
      }

      if(command().m_varTopicQuery["locale"].get_string().has_char())
      {
         str = command().m_varTopicQuery["locale"];
         data_set("system_locale", str);
         data_set("locale", str);
         set_locale(str, false);
      }
      else if(command().m_varTopicQuery["lang"].get_string().has_char())
      {
         str = command().m_varTopicQuery["lang"];
         data_set("system_locale", str);
         data_set("locale", str);
         set_locale(str, false);
      }
      else if(data_get("locale", str))
      {
         if(str.has_char())
         {
            set_locale(str, false);
         }
      }
      // if system style has changed (compared to last recorded one by ca2)
      // use the system style
      if(data_get("system_style", str))
      {
         if(str.has_char())
         {
            if(str != get_style())
            {
               try
               {
                  data_set("system_style", get_style());
                  data_set("style", get_style());
               }
               catch(...)
               {
               }
            }
         }
      }
      else
      {
         data_set("system_style", get_style());
      }

      if(command().m_varTopicQuery["style"].get_string().has_char())
      {
         str = command().m_varTopicQuery["style"];
         data_set("system_style", str);
         data_set("style", str);
         set_style(str, false);
      }
      else if(data_get("style", str))
      {
         if(str.has_char())
         {
            set_style(str, false);
         }
      }
      
      // keyboard layout
      if(data_get("keyboard_layout", str) && str.has_char())
      {
         set_keyboard_layout(str, false);
      }
      else
      {
         set_keyboard_layout(NULL, false);
      }

      data_pulse_change("ca2_fontopus_votagus", "savings", NULL);


      {

         gen::international::locale_style localestyle(this);


         localestyle.m_bAddAlternateStyle = true;


         string strLocale;
         string strStyle;

      
         if(Application.directrix().m_varTopicQuery["locale"].has_char() && Application.directrix().m_varTopicQuery["locale"].get_string().CompareNoCase("_std") != 0)
         {
            if(Application.directrix().m_varTopicQuery["style"].has_char() && Application.directrix().m_varTopicQuery["style"].get_string().CompareNoCase("_std") != 0)
            {
               localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["locale"], Application.directrix().m_varTopicQuery["style"]);
            }
            localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["locale"], Application.directrix().m_varTopicQuery["locale"]);
         }
      
      
         if(Application.directrix().m_varTopicQuery["lang"].has_char() && Application.directrix().m_varTopicQuery["lang"].get_string().CompareNoCase("_std") != 0)
         {
            if(Application.directrix().m_varTopicQuery["style"].has_char() && Application.directrix().m_varTopicQuery["style"].get_string().CompareNoCase("_std") != 0)
            {
               localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["lang"], Application.directrix().m_varTopicQuery["style"]);
            }
            localestyle.add_locale_variant(Application.directrix().m_varTopicQuery["lang"], Application.directrix().m_varTopicQuery["lang"]);
         }
      
      
         if(Application.directrix().m_varTopicQuery["style"].has_char() && Application.directrix().m_varTopicQuery["style"].get_string().CompareNoCase("_std") != 0)
         {
            localestyle.add_locale_variant(get_locale(), Application.directrix().m_varTopicQuery["style"]);
         }
         if(get_style().has_char() && get_style().CompareNoCase("_std") != 0 && get_style().CompareNoCase(get_locale()) != 0)
         {
            localestyle.add_locale_variant(get_locale(), Application.directrix().m_varTopicQuery["style"]);
         }
         localestyle.add_locale_variant(get_locale(), get_locale());


         localestyle.add_locale_variant("en", get_style());
         localestyle.add_locale_variant("en", get_locale());
         localestyle.add_locale_variant(get_style(), "en");
         localestyle.add_locale_variant(get_locale(), "en");
         localestyle.add_locale_variant("en", "en");

      
         localestyle.add_locale_variant("_std", get_style());
         localestyle.add_locale_variant("_std", get_locale());
         localestyle.add_locale_variant(get_style(), "_std");
         localestyle.add_locale_variant(get_locale(), "_std");
         localestyle.add_locale_variant("_std", "_std");


         localestyle.finalize();

         str_context()->m_straParamLocale.add(localestyle.m_strLocale);
         str_context()->m_straParamStyle.add(localestyle.m_strStyle);

         for(int i = 0; i < localestyle.m_straLocale.get_size(); i++)
         {
            str_context()->m_straParamLocale.add(localestyle.m_straLocale[i]);
            str_context()->m_straParamStyle.add(localestyle.m_straStyle[i]);
         }

      }

      return true;
   }

   bool application::finalize()
   {

      try
      {
         database::application::finalize();
      }
      catch(...)
      {
      }

      try
      {
         FinalizeDataCentral();
      }
      catch(...)
      {
      }


      return true;


   }

   ::database::server * application::get_data_server()
   {
      return m_pserver;
   }

   db_server & application::db()
   {
      return *m_pserver;
   }

   bool application::set_keyboard_layout(const char * pszPath, bool bUser)
   {
      UNREFERENCED_PARAMETER(pszPath);
      UNREFERENCED_PARAMETER(bUser);
      return false;
   }

   void application::on_set_keyboard_layout(const char * pszPath, bool bUser)
   {
      if(bUser)
      {
         data_set("keyboard_layout", pszPath);
      }
   }


} // namespace application