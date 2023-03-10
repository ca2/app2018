#include "framework.h"
#ifdef WINDOWSEX
#include "aura/aura/node/windows/windows_registry.h"
#endif

#include <sys/stat.h>


#if defined(LINUX)
#include <ctype.h>
#endif


DWORD RunSilent(const char* strFunct, char* strstrParams);


namespace introjection
{


   library::library(::aura::application * papp):
      object(papp),
      m_plibrary(canew(::aura::library(papp))),
      m_memfileError(papp)
   {

      ZERO(m_filetime);

   }


   library::~library()
   {


   }


   compiler::compiler(::aura::application * papp):
      ::object(papp),
      m_mutex(papp)
   {


#ifdef WINDOWS
      {

         ::file::path path;

         path = ::dir::system() / "config/programming/vs.txt";

         m_strVs = Application.file().as_string(path);

         m_strVs.trim();

         if (m_strVs == "2015")
         {

            m_strVsTools = "140";

         }
         else if (m_strVs == "2017")
         {

            m_strVsTools = "141";

         }
         else
         {

            string strMessage;

            strMessage = "There is a hole here. You should fill it with fullfillment. Missing f**k " + path;

            ::MessageBox(NULL, strMessage, strMessage, MB_OK);

         }

      }

#endif

      ::file::path path;

      path = THIS_FILE;

      m_pathProjectDir = path.folder();

#if MEMDLEAK

      m_strDynamicSourceConfiguration = "basis";
      m_strDynamicSourceStage = "time";


#elif defined(DEBUG)

      m_strDynamicSourceConfiguration = "basis";
      m_strDynamicSourceStage = "time";

#else

      m_strDynamicSourceConfiguration = "profiler";
      m_strDynamicSourceStage = "profiler";

#endif

      m_strDynamicSourceStageFolder = System.dir().install() / m_strDynamicSourceStage;

   }


   compiler::~compiler()
   {

   }


   void compiler::initialize(const char * pszApp)
   {

      m_strApp = pszApp;

      prepare_compile_and_link_environment();

   }


   void compiler::prepare_compile_and_link_environment()
   {

      Application.dir().mk(::dir::system() / "introjection\\symbols");

      ::file::path strVars;

#ifndef METROWIN
#ifdef WINDOWSEX

      if (m_strVs == "2015")
      {

         strVars = getenv("VS140COMNTOOLS");

      }
#endif
#endif


#ifdef WINDOWSEX

      if (m_strVs == "2017")
      {

         m_strEnv = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat";

         ::windows::registry::Key key;

         if (key.OpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\SxS\\VS7", false))
         {

            string strPath;

            if (key.QueryValue("15.0", strPath))
            {

               ::file::path path = strPath;

               m_strEnv = path / "VC/Auxiliary/Build/vcvarsall.bat";

            }

         }

         m_strVCVersion = VS2017_CURRENT_BUILD;

      }
      else if (m_strVs == "2015")
      {

         m_strEnv = strVars.up(2);
         m_strEnv = m_strEnv / "vc\\vcvarsall.bat";
         //m_strEnv = ".\\vc_vars.bat";

      }

      m_strTime = System.dir().install() / "time";

      //m_strEnv = "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Bin\\SetEnv.cmd";

      //m_strSdk1 = "windows7.1sdk";
      if (m_strVs == "2015")
      {

         m_strSdk1 = "vc140";

      }
      else if (m_strVs == "2017")
      {

         m_strSdk1 = "vc141";

      }

#endif

#ifdef OS64BIT
#ifdef LINUX
      m_strPlat1     = "64";
      m_strPlatform = "x64";
      m_strStagePlatform = "x64";
      m_strLibPlatform = "x64/";
#else
      m_strPlat1     = "64";
      m_strPlat2 = "x86_amd64";
      //m_strPlat2 = "amd64";
      m_strPlatform = "x64";
      m_strStagePlatform = "x64";
      m_strLibPlatform = "x64/";
#endif
#else
      m_strPlat1     = "32";
      m_strPlat2 = " x86";
      m_strPlatform = "Win32";
      m_strStagePlatform = "x86";
      m_strLibPlatform = "x86/";
#endif

      //System.file().lines(m_straSync, "C:\\core\\database\\text\\introjection\\syncer.txt", get_app());
#if defined(LINUX)
      prepare1(m_strDynamicSourceConfiguration + "_cl" + m_strPlat1 + ".bash",
               m_strDynamicSourceConfiguration + "_cl" + m_strPlat1 + ".bash");
      prepare1(m_strDynamicSourceConfiguration + "_libc" + m_strPlat1 + ".bash",
               m_strDynamicSourceConfiguration + "_libc" + m_strPlat1 + ".bash");
      prepare1(m_strDynamicSourceConfiguration + "_libl" + m_strPlat1 + ".bash",
               m_strDynamicSourceConfiguration + "_libl" + m_strPlat1 + ".bash");
#else
      prepare1(m_strPlat1,m_strPlat1);
      //prepare1(m_strDynamicSourceConfiguration  + "_cl" + m_strPlat1 + ".bat",
      //   m_strDynamicSourceConfiguration  + "_cl" + m_strPlat1 + ".bat");
      //prepare1(m_strDynamicSourceConfiguration  + "_libc" + m_strPlat1 + ".bat",
      //   m_strDynamicSourceConfiguration  + "_libc" + m_strPlat1 + ".bat");
      //prepare1(m_strDynamicSourceConfiguration  + "_libl" + m_strPlat1 + ".bat",
      //   m_strDynamicSourceConfiguration  + "_libl" + m_strPlat1 + ".bat");
#endif
      System.dir().mk(System.dir().install() / m_strDynamicSourceStage / "front",get_app());

      //#ifdef WINDOWS
      //      string vars1batSrc;
      //      string vars2batSrc;
      //      string vars1batDst;
      //      string vars2batDst;
      //      vars1batSrc = System.dir().install()/"nodeapp/stage/introjection/vc_vars.bat";
      //      vars2batSrc = System.dir().install()/"nodeapp/stage/introjection/vc_vars_query_registry.bat";
      //      vars1batDst = System.dir().install()/ m_strDynamicSourceStage / "front"/"vc_vars.bat";
      //      vars2batDst = System.dir().install()/m_strDynamicSourceStage /"front"/"vc_vars_query_registry.bat";
      //      try
      //      {
      //         Application.file().copy(vars1batDst, vars1batSrc, false);
      //      }
      //      catch(...)
      //      {
      //      }
      //      try
      //      {
      //         Application.file().copy(vars2batDst, vars2batSrc, false);
      //      }
      //      catch(...)
      //      {
      //      }
      //
      //#endif


#ifdef METROWIN

      _throw(todo(get_app()));

#elif defined(LINUX)

#else

      var var = System.process().get_output("\"" + m_strEnv + "\" " + m_strPlat2 + " 10.0.16299.0");

      TRACE("%s", var.get_string());

#endif

      string str;
      string strItem;

      strItem = System.dir().install() / m_strDynamicSourceStage / m_strStagePlatform;
      str = str + strItem + ";";

      strItem = System.dir().install() / m_strDynamicSourceStage / m_strStagePlatform / "introjection\\library";
      str = str + strItem + ";";
#ifdef WINDOWSEX
      uint32_t dwSize = GetEnvironmentVariable("PATH",NULL,0);
      LPTSTR lpsz = new char[dwSize + 1];
      dwSize = GetEnvironmentVariable("PATH",lpsz,dwSize + 1);
      str += lpsz;
      delete lpsz;
#elif defined(METROWIN)

      _throw(todo(get_app()));

#else
      str += getenv("PATH");
#endif
      bool bResult;
#ifdef WINDOWSEX
      //bResult = SetEnvironmentVariable("PATH",str) != FALSE;
#elif defined(METROWIN)

      _throw(todo(get_app()));

#elif defined(LINUX)
#else
      bResult = setenv("PATH",str,TRUE);
#endif
      TRACE("compiler::prepare_compile_and_link_environment SetEnvironmentVariable return bool %d",bResult);


   }





   void compiler::prepare1(const char * lpcszSource,const char * lpcszDest)
   {
#ifdef WINDOWS
      //Sleep(15000);

      string strBuildCmd = m_strEnv;

      if (m_strVs == "2015")
      {

         strBuildCmd = "\"" + strBuildCmd + "\" " + m_strPlat2;

      }
      else if (m_strVs == "2017")
      {

         strBuildCmd = "\"" + strBuildCmd + "\" " + m_strPlat2 + " " VS2017_CURRENT_BUILD;

      }

      ::process::process_sp process(allocer());


      ::file::path pathEnvTxt;

      pathEnvTxt = dir::system() / "env.txt";

      file_put_contents_dup(::dir::system() / "env1.bat",::dir::system() / "env.bat > \"" + pathEnvTxt + "\"");

      file_put_contents_dup(::dir::system() / "env.bat","@call " + strBuildCmd + "\r\n@set");

      RunSilent(::dir::system() / "env1.bat","");

      string strLog;

      strLog = file_as_string_dup(::dir::system() / "env.txt");

      stringa stra;

      stra.add_lines(strLog);

      //Sleep(10000);

#ifdef WINDOWSEX
      EnvVarValArray arrEnvVarVal;
      // Seperate values and variables
      CProcessEnvReader::SeparateVariablesAndValues(stra,arrEnvVarVal);

      string_to_string map;

      for(auto pair : arrEnvVarVal)
      {
         map[pair.m_element1] = pair.m_element2;
         TRACE("%s : %s", pair.m_element1, pair.m_element2);
         SetEnvironmentVariable(pair.m_element1,pair.m_element2);
      }



#endif
#endif

      ::file::path strFolder;
      strFolder = System.dir().install();
      if(!::str::ends(strFolder,"/") && !::str::ends(strFolder,"\\"))
         strFolder += "/";
      string strTemplate;
      string strSource = "nodeapp/time/dynamic_source/";
      strSource += lpcszSource;

      ::file::path pathN = m_pathProjectDir;
      pathN -= 3;
      string strN = pathN;
      strN.replace("\\", "/");
      strN += "/";


      //#ifdef DEBUG
      strTemplate = strFolder / strSource;
      //#else
      // strTemplate = strFolder, "app/time/core/fontopus/app/main/matter/dynamic_source_cl.bat", false);
      //#endif
      string str;
      str = Application.file().as_string(strTemplate);
      /*string strVars = getenv("VS100COMNTOOLS");
      System.file().path().eat_end_level(strVars, 2, "/");
      strVars += "vc/bin/vcvars32.bat";*/
      str.replace("%VS_VARS%",m_strEnv);
      str.replace("%VS_VARS_PLAT2%",m_strPlat2);

      string strV(System.dir().install());
      strV.replace("\\","/");
      if(!::str::ends(strV,"/") && !::str::ends(strV,"\\"))
         strV += "/";
      str.replace("%CA2_ROOT%",strV);
      str.replace("%PROJECT_DIR%", m_pathProjectDir);
      str.replace("%NETNODE_ROOT%",strN);
      str.replace("%SDK1%",m_strSdk1);
      string strDest = m_strDynamicSourceStage / "front" / lpcszDest;
      ::file::path strCmd;
      //#ifdef DEBUG
      strCmd = strFolder / strDest;
      //#else
      // strCmd = strFolder, "app\\time\\core\\fontopus\\app\\main\\front\\dynamic_source_cl.bat", false);
      //#endif
      Application.dir().mk(strCmd.folder());
      //Application.file().put_contents_utf8(strCmd, str);
      Application.file().put_contents(strCmd,str);
      Application.dir().mk(m_strTime / "dynamic_source");

   }





   ::aura::library * compiler::compile(string strFilePath,bool & bNew)
   {

      if (m_strApp.is_empty())
      {

         _throw(simple_exception(get_app(), "call compiler::initialize"));

      }

      sp(library) & lib = m_lib[strFilePath];

      if(lib.is_null())
      {

         lib = canew(library(get_app()));

      }

      if(get_file_time(strFilePath) == lib->m_filetime)
      {

         bNew = false;

         return lib->m_plibrary;

      }

      lib->m_plibrary->close();

      bNew = true;

      lib->m_filetime = get_file_time(strFilePath);

      ::file::path strName(strFilePath);

      lib->m_pathScript = strName;

#ifdef WINDOWS

      strName.replace("/","\\");

#endif

      string str;

      ::file::path strClog;

      ::file::path strLlog;

      ::file::path strScript;

      strScript = strName.title();

      ::file::path strTransformName = strName;

      if(Application.file().exists(strName))
      {

         strTransformName.replace(":","");

         ::str::ends_eat_ci(strTransformName,".cpp");

      }
      else
      {

         return lib->m_plibrary;

      }

      string strLibTitle = lib->m_pathScript.title();

#ifdef MACOS

      string strCmd = strFilePath;
      string strLCmd;
      string strDCmd;

      ::str::ends_eat_ci(strCmd, ".cpp");
      string strTargetPath;
      string strSrcName = ::file::path(strCmd).title();
      strTargetPath =  System.dir().module() / "lib" + strSrcName;
      string strDsymPath = (System.dir().module().folder(3)) /  "lib" + strSrcName;
      string strDdPath = System.dir().module().folder(7);
      string strBuildFolderPath = System.dir().module().folder(5);
      string strSrcFolder = ::file::path(strCmd).folder();
      string strFramework = m_strFramework;
      strClog = strCmd + "-osxc.txt";
      strLlog = strCmd + "-osxl.txt";
      strLCmd = strCmd + "-osxl";
      strDCmd = strCmd + "-osxd";
      string strLfl = strCmd + "-osx";
      strCmd += "-osxc";

      {

         string str2 = Application.file().as_string(strLfl+".LinkFileList");
         str2.replace("%TARGET_PATH%", strTargetPath);
         str2.replace("%DSYM_PATH%", strDsymPath);
         str2.replace("%DERIVED_DATA%", strDdPath);
         str2.replace("%FRAMEWORK%", strFramework);
         str2.replace("%BUILD_FOLDER%", strBuildFolderPath);
         str2.replace("%SRC_FOLDER%", strSrcFolder);
         str2.replace("%SRC_NAME%", strSrcName);
         Application.file().put_contents(strLfl + "2.LinkFileList", str2);

      }

      ::dir::mk("/var/tmp/ca2/intermediate");

#else


      //strName.replace("/", "\\");
      //string strFolder;
      //strFolder = System.dir().install();
      //::file::path strB;
      ::file::path strO;
      ::file::path strP;
      ::file::path strL;
      ::file::path strE;
      //::file::path strSVI;
      //::file::path strSVP;
      //::file::path strSPC;
      //::file::path strDVI;
      ::file::path strDVP;
      //::file::path strDPC;
      //::file::path strSO1;
      //::file::path strSO2;
      //::file::path strDO1;
      //::file::path strDO2;
      ::file::path strObj;


      /*string strScript(strName);
      strScript.replace("\\", ",");
      strScript.replace("/", ",");
      strScript = "ca2" + m_pmanager->m_strNamespace + "_script." + strScript;*/

      ::file::path strSourceDir;
      strSourceDir = strName.folder();

      //if(!Application.file().exists(lib->m_strSourcePath))
      //{
      //   lib->m_memfileError << "<pre>";
      //   str.Format("Source File : \"%s\" does not exist",lib->m_strSourcePath);
      //   lib->m_memfileError << str;
      //   lib->m_memfileError << "</pre>";
      //   return;
      //}


      //      string strTime = m_strTime;


      string strCppPath;

      //strCppPath = lib->m_pathScript;



      strClog = lib->m_pathScript + "-compile-log.txt";
      strLlog = lib->m_pathScript + "-link-log.txt";
      ::file::path strDynamicSourceScriptFolder = m_strTime / "intermediate" / m_strPlatform / m_strDynamicSourceConfiguration / string("introjection");

      //#ifdef DEBUG
#ifdef LINUX
      strO = strDynamicSourceScriptFolder / strTransformName.name() / strTransformName + ".bash";
      strObj = strDynamicSourceScriptFolder / strTransformName / strTransformName.name() + ".o";
#else
      //strB = m_strDynamicSourceStageFolder / "front\\introjection\\BuildBat" / strTransformName.name() / strTransformName + ".bat";
      strP = m_strDynamicSourceStageFolder / m_strStagePlatform / "introjection" / strTransformName.sibling(strScript.name()) + ".pdb";
      strL = m_strDynamicSourceStageFolder / m_strStagePlatform / "introjection" / strTransformName.sibling(strScript.name()) + ".lib";
      strE = m_strDynamicSourceStageFolder / m_strStagePlatform / "introjection" / strTransformName.sibling(strScript.name()) + ".exp";
      //strCppPath = m_strDynamicSourceStageFolder / "introjection" / strTransformName.sibling(strScript.name()) + ".cpp";
      strCppPath = strName;

      //strDVI = strDynamicSourceScriptFolder / strTransformName / m_strSdk1 + ".idb";
      strDVP = strDynamicSourceScriptFolder / strTransformName / m_strSdk1 + ".pdb";
      //strDPC = strDynamicSourceScriptFolder / strTransformName / m_pmanager->m_strNamespace + "_dynamic_source_script.pch";
      //strDO1 = strDynamicSourceScriptFolder / strTransformName / "framework.obj";
      //strDO2 = strDynamicSourceScriptFolder / strTransformName / m_pmanager->m_strNamespace + "_dynamic_source_script.obj";
      //strSVI = strDynamicSourceScriptFolder / m_strSdk1 + ".idb";
      //strSVP = strDynamicSourceScriptFolder / m_strSdk1 + ".pdb";
      //strSPC = strDynamicSourceScriptFolder / m_pmanager->m_strNamespace + "_dynamic_source_script.pch";
      //strSO1 = strDynamicSourceScriptFolder / "framework.obj";
      //strSO2 = strDynamicSourceScriptFolder / m_pmanager->m_strNamespace + "_dynamic_source_script.obj";

      strObj = strDynamicSourceScriptFolder / strTransformName / strTransformName.name() + ".obj";

      strO = strDynamicSourceScriptFolder / strTransformName.name() / strTransformName + ".bat";


#endif
      //lib->m_strBuildBat = strB;
      //m_pathScript = m_pmanager->get_script_path(strName);
      //#else
      // lib->m_strLibraryPath.Format(System.dir().install(m_strDynamicSourceStage /" Release\\%s.dll"), strName);
      //#endif

      try
      {
         if(Application.file().exists(strO))
         {
            Application.file().del(strO);
         }
      }
      catch(...)
      {
      }
      try
      {
         if(Application.file().exists(strObj))
         {
            Application.file().del(strObj);
         }
      }
      catch(...)
      {
      }
#ifndef LINUX
      try
      {
         if(Application.file().exists(strP))
         {
            Application.file().del(strP);
         }
      }
      catch(...)
      {
      }
      try
      {
         if(Application.file().exists(strL))
         {
            Application.file().del(strL);
         }
      }
      catch(...)
      {
      }
      try
      {
         if(Application.file().exists(strE))
         {
            Application.file().del(strE);
         }
      }
      catch(...)
      {
      }
      //try
      //{
      //   if(Application.file().exists(strDPC))
      //   {
      //      Application.file().del(strDPC);
      //   }
      //}
      //catch(...)
      //{
      //}
      //try
      //{
      //   if(Application.file().exists(strDVP))
      //   {
      //      Application.file().del(strDVP);
      //   }
      //}
      //catch(...)
      //{
      //}
      //try
      //{
      //   if(Application.file().exists(strDVI))
      //   {
      //      Application.file().del(strDVI);
      //   }
      //}
      //catch(...)
      //{
      //}
      //try
      //{
      //   if(Application.file().exists(strDO1))
      //   {
      //      Application.file().del(strDO1);
      //   }
      //}
      //catch(...)
      //{
      //}
      //try
      //{
      //   if(Application.file().exists(strDO2))
      //   {
      //      Application.file().del(strDO2);
      //   }
      //}
      //catch(...)
      //{
      //}
#endif
      try
      {
         if(Application.file().exists(strClog))
         {
            Application.file().del(strClog);
         }
      }
      catch(...)
      {
      }
      try
      {
         if(Application.file().exists(strLlog))
         {
            Application.file().del(strLlog);
         }
      }
      catch(...)
      {
      }
      //::DeleteFile(lib->m_strBuildBat);
      //try
      //{
      //   if(Application.file().exists(lib->m_pathScript + ".old"))
      //   {
      //      Application.file().del(lib->m_pathScript + ".old");
      //   }
      //}
      //catch(string strError)
      //{
      //   TRACE0(strError + "\n");
      //}
      try
      {
         //if(Application.file().exists(lib->m_pathScript))
         //{
         //   Application.file().copy(strCppPath,lib->m_pathScript);
         //}
      }
      catch(string strError)
      {

         TRACE("%s", strError + "\n");

      }
      //try
      //{
      //   if(Application.file().exists(lib->m_pathScript + ".old"))
      //   {
      //      Application.file().del(lib->m_pathScript + ".old");
      //   }
      //}
      //catch(string strError)
      //{
      //   TRACE0(strError + "\n");
      //}
#ifndef LINUX

      //      Application.dir().mk(strDVI.folder());
      //      Application.dir().mk(lib->m_strBuildBat.folder());
      //try
      //{
      //   //         Application.file().copy(strDVI, strSVI, false);
      //}
      //catch(...)
      //{
      //}
      //try
      //{
      //   Application.file().copy(strDVP, strSVP, false);
      //}
      //catch(...)
      //{
      //}
      //try
      //{
      //   Application.file().copy(strDPC, strSPC, false);
      //}
      //catch(...)
      //{

      //   lib->m_bHasTempOsError = true;

      //   return;

      //}
      //try
      //{
      //   Application.file().copy(strDO1,strSO1,false);
      //}
      //catch(...)
      //{

      //   lib->m_bHasTempOsError = true;

      //   return;

      //}
      //try
      //{
      //   Application.file().copy(strDO2,strSO2,false);
      //}
      //catch(...)
      //{

      //   lib->m_bHasTempOsError = true;

      //   return;

      //}




#endif

      Application.dir().mk(lib->m_pathScript.folder());
      Application.dir().mk(strL.folder());
      Application.dir().mk(m_strTime / "intermediate" / m_strPlatform / m_strDynamicSourceConfiguration / m_strApp / strTransformName);

      ::file::path pathN = m_pathProjectDir;

      pathN -= 3;

      string strN = pathN;

      strN.replace("\\", "/");

      strN += "/";

      string strBuildCmd;

#ifdef LINUX
      strBuildCmd = System.dir().install() / "nodeapp/stage/introjection" / m_strApp / (m_strDynamicSourceConfiguration + "_c" + m_strPlat1 + ".bash");
#elif defined(__APPLE__)
      strBuildCmd.Format(System.dir().install() / "nodeapp/stage/introjection" / m_strApp / (m_strDynamicSourceConfiguration + "_c" + m_strPlat1 + ".bat"));
#else
      strBuildCmd.Format(System.dir().install() / "nodeapp/stage/introjection" / m_strApp / m_strVsTools / (m_strDynamicSourceConfiguration + "_c" + m_strPlat1 + ".bat"));
#endif

      str = Application.file().as_string(strBuildCmd);
      str.replace("%SOURCE%",::str::replace("\\","/",string(strName)));
      str.replace("%ITEM_NAME%",::str::replace("\\","/",string(strTransformName)));
      str.replace("%ITEM_TITLE%",strTransformName.name());
      str.replace("%ITEM_DIR%",::str::replace("\\","/",string(strTransformName.folder())) + "/");
      str.replace("%LIBS_LIBS%",m_strLibsLibs);
      str.replace("%VS_VARS%",m_strEnv);
      str.replace("%VS_VARS_PLAT2%",m_strPlat2);

      string strElem = System.dir().install();

      strElem.replace("\\","/");

      strElem += "/";
      string strHmhLctvWildPdbPath;
      string strRndTitle;
      System.math().gen_rand_alnum(strRndTitle.GetBufferSetLength(64),64);
      strRndTitle.ReleaseBuffer();
      strHmhLctvWildPdbPath = ::file::path(::dir::system() / "netnodelite\\symbols") / strRndTitle;
      strHmhLctvWildPdbPath.replace("\\","/");
      Application.dir().mk(::file::path(strHmhLctvWildPdbPath).folder());
      str.replace("%HMH_LCTVWILD_PDB_PATH%",strHmhLctvWildPdbPath);

      str.replace("%CA2_ROOT%",strElem);
      str.replace("%PROJECT_DIR%", m_pathProjectDir);
      str.replace("%CONFIGURATION_NAME%",m_strDynamicSourceConfiguration);
      str.replace("%CONFIGURATION%",m_strDynamicSourceConfiguration);
      str.replace("%PLATFORM%",m_strPlatform);
      str.replace("%STAGEPLATFORM%",m_strStagePlatform);

      str.replace("%SDK1%",m_strSdk1);
      string strT2 = lib->m_pathScript;
      strT2.replace("\\",".");
      strT2.replace("/",".");
      strT2.replace(":","_");

#ifdef LINUX

      string strTargetPath =  System.dir().install() /"time" / m_strPlatform / m_strDynamicSourceConfiguration / lib->m_pathScript.title();
      ::str::ends_eat_ci(strTargetPath,".cpp");
      ::str::ends_eat_ci(strTargetPath,".so");

#else

      string strTargetPath = System.dir().install() / "time" / m_strPlatform / m_strDynamicSourceConfiguration / strT2 ;
      ::str::ends_eat_ci(strTargetPath, ".cpp");
      ::str::ends_eat_ci(strTargetPath,".dll");

#endif

      strTargetPath += "-"+strRndTitle;

      str.replace("%TARGET_PATH%",strTargetPath);

      ::process::process_sp process(allocer());

#ifdef LINUX

      file_put_contents_dup("/tmp/introj.bash", str);

      chmod("/tmp/introj.bash", S_IRWXU | S_IRWXG | S_IRWXO);

      process->create_child_process("/tmp/introj.bash",true,NULL,::multithreading::priority_highest);

#else

      process->create_child_process(str,true,m_pathProjectDir,::multithreading::priority_highest);

#endif

#endif

      string strLog;

      bool bTimeout = false;

#ifdef MACOS

      {

         ::chdir(strSrcFolder);
         strSrcFolder.trim();
         strSrcFolder.replace(" ", "\\ ");
         string str2 = Application.file().as_string(strCmd);
         str2.replace("%TARGET_PATH%", strTargetPath);
         str2.replace("%DSYM_PATH%", strDsymPath);
         str2.replace("%DERIVED_DATA%", strDdPath);
         str2.replace("%FRAMEWORK%", strFramework);
         str2.replace("%BUILD_FOLDER%", strBuildFolderPath);
         str2.replace("%SRC_FOLDER%", strSrcFolder);
         str2.replace("%SRC_NAME%", strSrcName);
         Application.file().put_contents(strCmd + "2", str2);

         ::system(str2);

      }

      strLog= file_as_string_dup(strClog);

#else

      uint32_t dwStart = ::get_tick_count();

      uint32_t dwExitCode;

      while(::get_thread_run())
      {

         strLog += process->read();

         if (process->has_exited())
         {

            break;

         }

         Sleep(100);

         if(::get_tick_count() - dwStart > 840 * 1000) // 14 minutes
         {

            bTimeout = true;

            break;

         }

      }

      strLog += process->read();

      if(bTimeout)
      {

         process->kill();

      }

#endif



#ifdef LINUX
      if(!bTimeout)
#else
      if(!bTimeout && strLog.has_char())
#endif
      {

#ifdef LINUX

         //Sleep(2000);

#endif

         str = strLog;

         str.trim();

         if(str.has_char())
         {
#ifndef MACOS
            Application.file().put_contents_utf8(strClog,strLog);
#endif
            lib->m_memfileError << "<pre>";

            lib->m_memfileError << "Compiling...\n";
            lib->m_memfileError << lib->m_pathScript;
            lib->m_memfileError << "\n";
            if(bTimeout)
            {
               lib->m_memfileError << "error: Timeout during compilation (If there are the compilation or link errors about the file \"" + lib->m_pathScript + "\" following this message, they may be out-of-date)";
            }
            str.replace("\r\n","\n");
            lib->m_memfileError << str;

         }

         string strBuildCmd;

#ifndef MACOS

#ifdef LINUX
         strBuildCmd.Format(System.dir().install() / "nodeapp\\stage\\introjection" / m_strApp / (m_strDynamicSourceConfiguration + "_l" + m_strPlat1 + ".bash"));
#else
         strBuildCmd.Format(System.dir().install() / "nodeapp\\stage\\introjection" / m_strApp / m_strVsTools / (m_strDynamicSourceConfiguration + "_l" + m_strPlat1 + ".bat"));
#endif

         str = Application.file().as_string(strBuildCmd);


         str.replace("%ITEM_NAME%",::str::replace("\\","/",string(strTransformName)));
         str.replace("%ITEM_TITLE%",strTransformName.name());
         str.replace("%ITEM_DIR%",::str::replace("\\","/",string(strTransformName.folder())) + "/");
         str.replace("%LIBS_LIBS%",m_strLibsLibs);
         str.replace("%VS_VARS%",m_strEnv);
         str.replace("%VS_VARS_PLAT2%",m_strPlat2);
         str.replace("%HMH_LCTVWILD_PDB_PATH%",strHmhLctvWildPdbPath);

         str.replace("%CA2_ROOT%",strElem);
         str.replace("%PROJECT_DIR%", m_pathProjectDir);
         str.replace("%CONFIGURATION_NAME%",m_strDynamicSourceConfiguration);
         str.replace("%CONFIGURATION%",m_strDynamicSourceConfiguration);
         str.replace("%PLATFORM%",m_strPlatform);
         str.replace("%STAGEPLATFORM%",m_strStagePlatform);
         //      str.replace("%LIBPLATFORM%", m_strLibPlatform);
         str.replace("%SDK1%",m_strSdk1);
         //string strTargetPath = lib->m_pathScript;
         //strTargetPath.replace("\\",".");
         //strTargetPath.replace("/",".");
         //::str::ends_eat_ci(strTargetPath,".cpp");
         //#ifdef LINUX
         //         ::str::ends_eat_ci(strTargetPath,".so");
         //#else
         //         ::str::ends_eat_ci(strTargetPath,".dll");
         //#endif
         //         strTargetPath = System.dir().install() /
         str.replace("%TARGET_PATH%",strTargetPath);
         //strBuildCmd = lib->m_strBuildBat;
         //Application.file().put_contents_utf8(strBuildCmd, str);

         //Application.file().put_contents(strBuildCmd,str);

         //str.replace("\\", "/");

         bTimeout = false;

         ::process::process_sp process(allocer());

         //         set_thread_priority(::multithreading::priority_highest);

#ifdef LINUX

         file_put_contents_dup("/tmp/introl.bash", str);

         chmod("/tmp/introl.bash", S_IRWXU | S_IRWXG | S_IRWXO);

         process->create_child_process("/tmp/introl.bash",true,NULL,::multithreading::priority_highest);

#else

         process->create_child_process(str,true,NULL,::multithreading::priority_highest);
#endif
#else

         {

            string str2 = Application.file().as_string(strLCmd);
            str2.replace("%TARGET_PATH%", strTargetPath);
            str2.replace("%DSYM_PATH%", strDsymPath);
            str2.replace("%DERIVED_DATA%", strDdPath);
            str2.replace("%FRAMEWORK%", strFramework);
            str2.replace("%BUILD_FOLDER%", strBuildFolderPath);
            str2.replace("%SRC_FOLDER%", strSrcFolder);
            str2.replace("%SRC_NAME%", strSrcName);
            Application.file().put_contents(strLCmd + "2", str2);
            //::system(strLCmd + "2");
            ::system(str2);

            if(!::str::begins_ci(System.dir().module(), "/Applications/"))
            {
               string str2 = Application.file().as_string(strDCmd);
               str2.replace("%TARGET_PATH%", strTargetPath);
               str2.replace("%DSYM_PATH%", strDsymPath);
               str2.replace("%DERIVED_DATA%", strDdPath);
               str2.replace("%FRAMEWORK%", strFramework);
               str2.replace("%BUILD_FOLDER%", strBuildFolderPath);
               str2.replace("%SRC_FOLDER%", strSrcFolder);
               str2.replace("%SRC_NAME%", strSrcName);
               Application.file().put_contents(strDCmd + "2", str2);
               //              ::system(strDCmd + "2");
               ::system(str2);
            }

         }

#endif

         string strLog;

#ifdef MACOS

         strLog= file_as_string_dup(strLlog);

#else

         while(true)
         {

            strLog += process->read();

            if(process->has_exited())
               break;

            Sleep(100);

            if(::get_tick_count() - dwStart > 840 * 1000) // 14 minutes
            {

               bTimeout = true;

               break;

            }

         }

         strLog += process->read();

#endif

         if(!bTimeout && strLog.has_char())
         {

#ifdef LINUX

            //Sleep(2000);

#endif


            str = strLog;

            str.trim();

            if(str.has_char())
            {

#ifndef MACOS

               Application.file().put_contents_utf8(strLlog,strLog);

#endif

               lib->m_memfileError << "Linking...\n";
               str.replace("\r\n","\n");
               lib->m_memfileError << str;
               lib->m_memfileError << "</pre>";


            }

            lib->m_strError = lib->m_memfileError.to_string();

            lib->m_strError.trim();

         }

      }

#ifndef LINUX

#ifndef MACOS

      try
      {

         Application.file().del(strDVP);

      }
      catch(...)
      {

      }

#endif

#endif

#ifdef WINDOWS

      lib->m_plibrary->open(strTargetPath + ".dll");

#elif defined(MACOS)

      lib->m_plibrary->open(strTargetPath + ".dylib");

#else

      lib->m_plibrary->open(strTargetPath + ".so");

#endif

      lib->m_plibrary->open_ca2_library(strLibTitle);

      return lib->m_plibrary;

   }

}







DWORD RunSilent(const char* strFunct, char* strstrParams)
{

#ifdef WINDOWSEX

   STARTUPINFO StartupInfo;
   PROCESS_INFORMATION ProcessInfo;
   char Args[4096];
   char *pEnvCMD = NULL;
   char *pDefaultCMD = "CMD.EXE";
   ULONG rc;

   memset(&StartupInfo, 0, sizeof(StartupInfo));
   StartupInfo.cb = sizeof(STARTUPINFO);
   StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
   StartupInfo.wShowWindow = SW_HIDE;

   Args[0] = 0;

   pEnvCMD = getenv("COMSPEC");

   if (pEnvCMD)
   {

      strcpy(Args, pEnvCMD);
   }
   else
   {
      strcpy(Args, pDefaultCMD);
   }

   // "/c" option - Do the command then terminate the command window
   strcat(Args, " /c ");
   //the application you would like to run from the command window
   strcat(Args, strFunct);
   strcat(Args, " ");
   //the parameters passed to the application being run from the command window.
   strcat(Args, strstrParams);

   if (!CreateProcess(NULL, Args, NULL, NULL, FALSE,
                      CREATE_NEW_CONSOLE,
                      NULL,
                      NULL,
                      &StartupInfo,
                      &ProcessInfo))
   {
      return get_last_error();
   }

   WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
   if (!GetExitCodeProcess(ProcessInfo.hProcess, &rc))
      rc = 0;

   CloseHandle(ProcessInfo.hThread);
   CloseHandle(ProcessInfo.hProcess);

   return rc;

#else

   string strCmdLine;

   strCmdLine = strFunct;
   if (strlen_dup(strstrParams) > 0)
   {
      strCmdLine += " ";
      strCmdLine += strstrParams;
   }

   int32_t processId;

   if (!create_process(strCmdLine, &processId))
      return -1;


   while (true)
   {

      if (kill(processId, 0) == -1 && errno == ESRCH) // No process can be found corresponding to processId
         break;
      sleep(millis(23));
   }

   return 0;
#endif

}
