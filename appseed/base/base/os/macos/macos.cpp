#include "framework.h"

//int32_t raw_main_command_line(const char * pszCommandLine, int argc, char *argv[]);
//
//string & get_command_line_string()
//{
//   static string g_strCommandLine;
//   return g_strCommandLine;
//}
//
//wstring & get_command_line_wstring()
//{
//   static wstring g_wstrCommandLine;
//   return g_wstrCommandLine;
//}
//
//
//void SetCommandLineA(const char * psz)
//{
//
//   get_command_line_string()     = psz;
//   get_command_line_wstring()    = psz;
//
//}
//
//void SetCommandLineW(const unichar * pwsz)
//{
//
//   get_command_line_string()     = pwsz;
//      get_command_line_wstring()     = pwsz;
//
//}
//
//
//LPSTR GetCommandLineA()
//{
//
//   return (LPSTR) (LPCSTR) get_command_line_string();
//
//}
//
//LPWSTR GetCommandLineW()
//{
//
//   return (LPWSTR) (LPCWSTR)    get_command_line_wstring() ;
//
//}
//



//void output_debug_string(const char * psz)
//{
//
//   output_debug_string(psz);
//
//}




CLASS_DECL_AURA bool __node_base_pre_init()
{

   
   return true;

}



CLASS_DECL_AURA bool __node_base_pos_init()
{
   
   
   return true;
   
}


CLASS_DECL_AURA bool __node_base_pre_term()
{
  
   return true;
   
}



CLASS_DECL_AURA bool __node_base_pos_term()
{
   


   return true;
   
}





int32_t base_main_command_line(const char * pszParams, int argc, char *argv[])
{
   
   
   pid_t processid;
   
   string strCommandLine;
   
   strCommandLine = argv[0];
   
   if(strCommandLine.find(' ') >= 0)
   {
      
      strCommandLine = "\"" + strCommandLine + "\"";
      
   }
   
   string strP = pszParams;

   strsize iFind = strP.find(':');
   
   if(iFind == 0)
   {
   }
   else
   {
      int iQuote;
      while(iFind > 0)
      {
      
      iQuote = 0;
      const char * psz = &strP[iFind];
      for(; psz >= strP.c_str(); psz = ::str::utf8_dec(strP, psz))
{
         if(*psz == '\"')
         {
            iQuote++;
         }
      }
      
      if(iQuote % 2 == 0)
      {
         break;
         
      }
         iFind = strP.find(':', iFind + 1);
         
      }
      

      
   }
   
   
   string str1;
   
   string str2;
   
   if(iFind >= 0)
   {
      
      str1 = strP.Left(iFind);
      
      if(str1.has_char())
      {
         
         str1 = " " + str1 + " ";
         
      }
      
      str2 = strP.Mid(iFind + 1);
      
      if(str2.has_char())
      {
         
         str2 = " " + str2 + " ";
         
      }
      
   }
   
   string strA;
   
   string strB;
   
   processid = getpid();
   
   printf("%d\n", processid);
   
   string strAddUp;
   
   bool bColon = false;
   
   bool bOverride = false;
   
   for(index i = 1; i < argc; i++)
   {
      
      if(strcmp(argv[i], ":") == 0)
      {
         
         bColon = true;
         
      }
      else
      {
         if(!bColon)
         {
         
            if(argv[i][0] == '-')
            {
            
               bColon = true;
            
            }
            
         }
         
         if(!bColon)
         {
            
            //if(strstr(argv[i], " ") != 0)
            {
               
               strA += " \"" + string(argv[i]) + "\"";
               
            }
//            else
//            {
//            
//               strA += string(" ") + argv[i];
//               
//            }
            
         }
         else
         {
            
            if(::str::begins_ci(argv[i], "app="))
            {
               
               bOverride = true;
               
            }

            strB += string(" ") + argv[i];
            
         }
         
      }
      
   }
   
   if(bOverride)
   {
      
      str2.replace_ci("app=", "fallback_app=");
      
   }
   
   strCommandLine += str1 + strA + " : " + str2 + strB;

   return raw_main_command_line(strCommandLine, argc, argv);

}


