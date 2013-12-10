#include "framework.h"


#if defined(MACOS)

void openURL(const string &url_str);

#endif

#if defined(LINUX) || defined(MACOS)
#include <dlfcn.h>
#endif


UINT base_application::APPM_LANGUAGE = WM_APP + 117;
WPARAM base_application::WPARAM_LANGUAGE_UPDATE = 1;


base_application::base_application() :
m_allocer(this),
m_mutexMatterLocator(this),
m_mutexStr(this)
{

#ifdef WINDOWS

   m_hinstance                = NULL;

#endif

   if (m_pbaseapp == NULL)
   {

      m_pbaseapp              = this;

   }

   if (m_pbaseapp != NULL)
   {

      m_pbasesystem           = m_pbaseapp->m_pbasesystem;

      m_hinstance             = m_pbaseapp->m_hinstance;

   }
   else
   {

      m_pbasesystem           = NULL;

   }

   m_pplaneapp                = NULL;

   m_pbasesession             = NULL;

   m_nSafetyPoolSize          = 512;        // default size

   m_bIfs                     = true;

   m_dir.set_app(this);
   m_file.set_app(this);
   m_http.set_app(this);


   m_psignal                  = canew(class signal());

   m_pcommandthread           = canew(::command_thread(this));

   m_psavings                 = canew(class ::core::savings(this));
   m_pmath                    = canew(math::math(this));
   m_pgeometry                = canew(geometry::geometry(this));

   m_bZipIsDir                = true;

   m_pmapKeyPressed           = NULL;

   m_bLicense                 = true;

   // initialize wait cursor state
   m_iWaitCursorCount         = 0;
   m_hcurWaitCursorRestore    = NULL;

   m_bBaseProcessInitialize   = false;
   m_bBaseInitializeInstance  = false;


}

base_application::~base_application()
{
}


void base_application::assert_valid() const
{
   thread::assert_valid();


   if (System.GetThread() != (thread*)this)
      return;     // only do subset if called from different thread

   ASSERT(System.GetThread() == this);
   //ASSERT(afxCurrentInstanceHandle == m_hInstance);

   /*      if (m_pdocmanager != NULL)
   ASSERT_VALID(m_pdocmanager);*/
}

void base_application::dump(dump_context & dumpcontext) const
{

   thread::dump(dumpcontext);

#ifdef WINDOWS

   dumpcontext << "m_hinstance = " << (void *)m_hinstance;

#endif

   dumpcontext << "\nm_strCmdLine = " << m_strCmdLine;
   dumpcontext << "\nm_nCmdShow = " << m_nCmdShow;
   dumpcontext << "\nm_bHelpMode = " << m_strAppName;
   //   dumpcontext << "\nm_bHelpMode = " << m_bHelpMode;
   // dumpcontext << "\nm_pszHelpFilePath = " << m_pszHelpFilePath;
   //dumpcontext << "\nm_pszProfileName = " << m_pszProfileName;

#ifdef WINDOWS
   //   dumpcontext << "\nm_hDevMode = " << (void *)m_hDevMode;
   // dumpcontext << "\nm_hDevNames = " << (void *)m_hDevNames;
#endif

   //dumpcontext << "\nm_dwPromptContext = " << (UINT)m_dwPromptContext;
   //      dumpcontext << "\nm_eHelpType = " << m_eHelpType;


   /*      if (m_pdocmanager != NULL)
   m_pdocmanager->dump(dumpcontext);*/

   //dumpcontext << "\nm_nWaitCursorCount = " << m_iWaitCursorCount;
   dumpcontext << "\nm_hcurWaitCursorRestore = " << (void *)m_hcurWaitCursorRestore;
   //dumpcontext << "\nm_nNumPreviewPages = " << m_nNumPreviewPages;

   /*   ___THREAD_STATE* pState = __get_thread_state();
   dumpcontext << "\nm_msgCur = {";
   dumpcontext << "\n\toswindow = " << (void *)pState->m_msgCur.oswindow;
   dumpcontext << "\n\tmessage = " << (UINT)pState->m_msgCur.message;
   dumpcontext << "\n\twParam = " << (UINT)pState->m_msgCur.wParam;
   dumpcontext << "\n\tlParam = " << (void *)pState->m_msgCur.lParam;
   dumpcontext << "\n\ttime = " << pState->m_msgCur.time;
   dumpcontext << "\n\tpt = " << point(pState->m_msgCur.pt);
   dumpcontext << "\n}";
   */
   dumpcontext << "\n";
}


int32_t base_application::simple_message_box(sp(::user::interaction) puiOwner, const char * pszMessage, UINT fuStyle)
{

#if defined(WINDOWSEX)

   return MessageBoxW(puiOwner->get_safe_handle(), wstring(pszMessage), wstring(m_strAppName), fuStyle);
   //return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
   //   wstring(pszMessage), wstring(m_strAppName), fuStyle);

#elif  defined(LINUX) || defined(MACOS) || defined(ANDROID)

   return MessageBox(puiOwner->get_safe_handle(), pszMessage, m_strAppName, fuStyle);
   //   return MessageBox((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

#else

   return MessageBox(puiOwner->get_safe_handle(), pszMessage, m_strAppName, fuStyle);

#endif

}

/*
int32_t application::simple_message_box(sp(::user::interaction) puiOwner, const char * pszMessage, UINT fuStyle)
{

#if defined(WINDOWSEX)

   return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
      wstring(pszMessage), wstring(m_strAppName), fuStyle);

#elif  defined(LINUX) || defined(MACOS) || defined(ANDROID)

   return MessageBox((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

#else

   return MessageBox(m_psystem->m_posdata->m_pui->get_handle(), pszMessage, m_strAppName, fuStyle);

#endif

}
*/

int32_t base_application::simple_message_box(const char * pszMessage, UINT fuStyle)
{

#if defined(WINDOWSEX)

   return MessageBoxW(NULL, wstring(pszMessage), wstring(m_strAppName), fuStyle);
   //return MessageBoxW((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())),
   //   wstring(pszMessage), wstring(m_strAppName), fuStyle);

#elif  defined(LINUX) || defined(MACOS) || defined(ANDROID)

   return MessageBox(NULL, pszMessage, m_strAppName, fuStyle);
   //   return MessageBox((puiOwner == NULL ? NULL : (puiOwner->get_wnd() == NULL ? NULL : puiOwner->get_handle())), pszMessage, m_strAppName, fuStyle);

#else

   return MessageBox(NULL, pszMessage, m_strAppName, fuStyle);

#endif

}

string base_application::message_box(const string & pszMatter, property_set & propertyset)
{

   simple_message_box(pszMatter, 0);

   return "";

}

string base_application::load_string(id id)
{
   string str;
   if (!load_string(str, id))
   {
      return (const string &)id;
   }
   return str;
}

bool base_application::load_string(string & str, id id)
{
   if (!load_cached_string(str, id, true))
   {
      return false;
   }
   return true;
}

bool base_application::load_cached_string(string & str, id id, bool bLoadStringTable)
{
   ::xml::document doc(this);
   if (!doc.load(id))
   {
      return load_cached_string_by_id(str, id, "", bLoadStringTable);
   }
   sp(::xml::node) pnodeRoot = doc.get_root();
   if (pnodeRoot->get_name() == "string")
   {
      string strId = pnodeRoot->attr("id");
      string strValue = pnodeRoot->get_value();
      return load_cached_string_by_id(str, strId, strValue, bLoadStringTable);
   }
   str = doc.get_name();
   return true;
}

bool base_application::load_cached_string_by_id(string & str, id id, const string & pszFallbackValue, bool bLoadStringTable)
{

   synch_lock sl(&m_mutexStr);

   string strId(*id.m_pstr);
   string strTable;
   string strString;
   string_to_string * pmap = NULL;
   index iFind = 0;
   if ((iFind = strId.find(':')) <= 0)
   {
      strTable = "";
      strString = strId;
   }
   else
   {
      strTable = strId.Mid(0, iFind);
      strString = strId.Mid(iFind + 1);
   }
   if (m_stringtableStd.Lookup(strTable, pmap))
   {
      if (pmap->Lookup(strString, str))
      {
         return true;
      }
   }
   else if (m_stringtable.Lookup(strTable, pmap))
   {
      if (pmap->Lookup(strString, str))
      {
         return true;
      }
   }
   else if (bLoadStringTable)
   {
      load_string_table(strTable, "");
      return load_cached_string_by_id(str, id, pszFallbackValue, false);
   }
   if (pszFallbackValue.is_empty())
      str = strId;
   else
      str = pszFallbackValue;
   return true;
}

void base_application::load_string_table(const string & pszApp, const string & pszId)
{

   synch_lock sl(&m_mutexStr);

   string strApp(pszApp);
   string strMatter;
   string strLocator;

   if (strApp.is_empty())
   {
      strLocator = System.dir().appmatter_locator(this);
   }
   else
   {
      strLocator = System.dir().appmatter_locator(strApp);
   }

   if (strMatter.is_empty())
   {
      strMatter = "stringtable.xml";
   }
   else if (System.file().extension(strMatter) != "xml")
   {
      strMatter += ".xml";
   }

   string strTableId = strApp;

   if (pszId.has_char() && *pszId != '\0')
   {
      strTableId += "\\";
      strTableId += pszId;
   }

   ::xml::document doc(get_app());
   string strFilePath = System.dir().matter_from_locator(App(this).str_context(), strLocator, strMatter);
   if (!System.file().exists(strFilePath, this))
   {
      try
      {
         if (m_stringtable[pszId] != NULL)
            delete m_stringtable[pszId];
      }
      catch (...)
      {
      }
      m_stringtable.set_at(pszId, new string_to_string);
      return;
   }
   string strFile = Application.file().as_string(strFilePath);
   if (!doc.load(strFile))
      return;
   string_to_string * pmapNew = new string_to_string;
   for (int32_t i = 0; i < doc.get_root()->children().get_count(); i++)
   {
      string strId = doc.get_root()->child_at(i)->attr("id");
      string strValue = doc.get_root()->child_at(i)->get_value();
      pmapNew->set_at(strId, strValue);
   }

   string_to_string * pmapOld = m_stringtable[strTableId];

   m_stringtable[strTableId] = NULL;

   if (pmapOld != NULL)
   {

      try
      {

         delete pmapOld;

      }
      catch (...)
      {

      }

   }

   m_stringtable[strTableId] = pmapNew;
   ASSERT(m_stringtable[strTableId] == pmapNew);
}




void base_application::load_string_table()
{
   load_string_table("", "");
}


sp(element) base_application::alloc(sp(type) info)
{
   return System.alloc(this, info);
}

sp(element) base_application::alloc(const  id & idType)
{
   return System.alloc(this, idType);
}

bool base_application::is_system()
{

   return false;

}

bool base_application::is_session()
{

   return false;

}


bool base_application::is_serviceable()
{

   return false;

}

//string base_application::matter_as_string(const char * pszMatter, const char * pszMatter2 = NULL);
//string base_application::dir_matter(const char * pszMatter, const char * pszMatter2 = NULL);
bool base_application::is_inside_time_dir(const char * pszPath)
{
   throw not_implemented(this);
   return false;
}

bool base_application::file_is_read_only(const char * pszPath)
{
   throw not_implemented(this);
   return false;
}

string base_application::file_as_string(var varFile)
{

   if (::str::begins_ci(varFile.get_string(), "http://")
      || ::str::begins_ci(varFile.get_string(), "https://"))
   {
      return Application.http().get(varFile.get_string());
   }
   else if (::str::begins_ci(varFile["url"].get_string(), "http://")
      || ::str::begins_ci(varFile["url"].get_string(), "https://"))
   {
      return Application.http().get(varFile["url"].get_string());
   }
   else
   {
      return file_as_string_dup(varFile.get_string());
   }

}

string base_application::dir_path(const char * psz1, const char * psz2, const char * psz3)
{
   return ::dir::path(psz1, psz2, psz3);
}

string base_application::dir_name(const char * psz)
{
   return ::dir::name(psz);
}

bool base_application::dir_mk(const char * psz)
{
   return ::dir::mk(psz);
}

string base_application::file_title(const char * psz)
{
   return ::file_title_dup(psz);
}
string base_application::file_name(const char * psz)
{
   return ::file_name_dup(psz);
}

bool base_application::app_map_lookup(const char * psz, void * & p)
{
   return m_appmap.Lookup(psz, p) != FALSE;
}

void base_application::app_map_set(const char * psz, void * p)
{
   m_appmap.set_at(psz, p);
}


sp(::command_thread) base_application::command_central()
{
   return m_pcommandthread;
}

sp(::command_thread) base_application::command_thread()
{
   return m_pcommandthread;
}

sp(::command_thread) base_application::command()
{
   return m_pcommandthread;
}

sp(::command_thread) base_application::guideline()
{
   return m_pcommandthread;
}

sp(::command_thread) base_application::directrix()
{
   return m_pcommandthread;
}

sp(::command_thread) base_application::axiom()
{
   return m_pcommandthread;
}

bool base_application::verb()
{
   axiom()->run();
   return true;
}

sp(::command_thread) base_application::creation()
{
   return m_pcommandthread;
}


::file::binary_buffer_sp base_application::file_get_file(var varFile, uint32_t uiFlags)
{

   return NULL;

}


application_signal_details::application_signal_details(sp(base_application) papp, class ::signal * psignal, e_application_signal esignal) :
element(papp),
::signal_details(psignal)
{

   m_esignal = esignal;
   m_bOk = true;

}



::fontopus::user * base_application::safe_get_user()
{

   if (m_pfontopus == NULL)
      return NULL;

   return m_pfontopus->m_puser;

}


bool base_application::open_link(const string & strLink, const string & pszTarget)
{
   if (is_system())
   {
#ifdef WINDOWSEX
      string strUrl = strLink;
      if (!::str::begins_ci(strUrl, "http://")
         && !::str::begins_ci(strUrl, "https://"))
      {
         strUrl = "http://" + strUrl;
      }
      ::ShellExecuteA(NULL, "open", strUrl, NULL, NULL, SW_SHOW);
      return true;
#elif defined METROWIN
#pragma push_macro("System")
#undef System
      ::Windows::Foundation::Uri ^ uri = ref new ::Windows::Foundation::Uri(strLink);
      ::Windows::System::LauncherOptions ^ options = ref new ::Windows::System::LauncherOptions();
      options->TreatAsUntrusted = false;
      bool success = ::wait(::Windows::System::Launcher::LaunchUriAsync(uri, options));
#pragma pop_macro("System")
#elif defined(LINUX)
      ::system("xdg-open " + strLink);
      return true;
#elif defined(MACOS)
      openURL(strLink);
      return true;
#else
      throw not_implemented(get_app());
#endif
   }
   else
   {
      return System.open_link(strLink, pszTarget);
   }

   return false;

}


::user::interaction_ptr_array & base_application::frames()
{
   return *m_pframea;
}

void base_application::add_frame(sp(::user::interaction) pwnd)
{
   m_pframea->add_unique(pwnd);
}

void base_application::remove_frame(sp(::user::interaction) pwnd)
{
   m_pframea->remove(pwnd);
   if (GetMainWnd() == pwnd)
   {
      if (m_pframea->get_size() > 0)
      {
         SetMainWnd(m_pframea->element_at(0));
      }
      else
      {
         SetMainWnd(NULL);
      }
   }
}


thread * base_application::GetThread()
{

   if (m_pimpl == NULL)
      return NULL;

   return m_pimpl->GetThread();

}


bool base_application::is_key_pressed(::user::e_key ekey)
{

   if (is_session())
   {
      if (m_pmapKeyPressed == NULL)
      {
         m_pmapKeyPressed = new ::map < ::user::e_key, ::user::e_key, bool, bool >;
      }
      bool bPressed = false;
      if (ekey == ::user::key_shift)
      {
         m_pmapKeyPressed->Lookup(::user::key_shift, bPressed);
         if (bPressed)
            goto ret;
         m_pmapKeyPressed->Lookup(::user::key_lshift, bPressed);
         if (bPressed)
            goto ret;
         m_pmapKeyPressed->Lookup(::user::key_rshift, bPressed);
         if (bPressed)
            goto ret;
      }
      else if (ekey == ::user::key_control)
      {
         m_pmapKeyPressed->Lookup(::user::key_control, bPressed);
         if (bPressed)
            goto ret;
         m_pmapKeyPressed->Lookup(::user::key_lcontrol, bPressed);
         if (bPressed)
            goto ret;
         m_pmapKeyPressed->Lookup(::user::key_rcontrol, bPressed);
         if (bPressed)
            goto ret;
      }
      else if (ekey == ::user::key_alt)
      {
         m_pmapKeyPressed->Lookup(::user::key_alt, bPressed);
         if (bPressed)
            goto ret;
         m_pmapKeyPressed->Lookup(::user::key_lalt, bPressed);
         if (bPressed)
            goto ret;
         m_pmapKeyPressed->Lookup(::user::key_ralt, bPressed);
         if (bPressed)
            goto ret;
      }
      else
      {
         m_pmapKeyPressed->Lookup(ekey, bPressed);
      }
   ret:
      return bPressed;
   }
   else if (m_pbasesession != NULL)
   {
      return Sess(this).is_key_pressed(ekey);
   }
   else if (m_pbasesystem != NULL)
   {
      if (m_pbasesystem == this)
      {
         throw "not expected";
         return false;
      }
      return Sys(this).is_key_pressed(ekey);
   }
   else
   {
      throw "not expected";
   }

}

void base_application::set_key_pressed(::user::e_key ekey, bool bPressed)
{
   if (is_session())
   {
      if (m_pmapKeyPressed == NULL)
      {
         m_pmapKeyPressed = new ::map < ::user::e_key, ::user::e_key, bool, bool >;
      }
      (*m_pmapKeyPressed)[ekey] = bPressed;
   }
   else if (m_pbasesession != NULL)
   {
      return Sess(this).set_key_pressed(ekey, bPressed);
   }
   else if (m_pbasesystem != NULL)
   {

      if (m_pbasesystem == this)
      {

         return;

      }

      return Sys(this).set_key_pressed(ekey, bPressed);

   }
   else
   {
      throw "not expected";
   }
}



#ifdef METROWIN
sp(::user::interaction) base_application::window_from_os_data(void * pdata)
{

   return m_pimpl->window_from_os_data(pdata);

}

sp(::user::interaction) base_application::window_from_os_data_permanent(void * pdata)
{

   return m_pimpl->window_from_os_data_permanent(pdata);

}
#else
sp(::user::window) base_application::window_from_os_data(void * pdata)
{

   return m_pimpl->window_from_os_data(pdata);

}

sp(::user::window) base_application::window_from_os_data_permanent(void * pdata)
{

   return m_pimpl->window_from_os_data_permanent(pdata);

}
#endif



void base_application::DoWaitCursor(int32_t nCode)
{

   if (nCode < 0)
   {

      m_iWaitCursorCount = 0;
      ShowWaitCursor(false);

   }
   else if (nCode == 0)
   {

      if (m_iWaitCursorCount > 0)
      {
         m_iWaitCursorCount--;
      }

      if (m_iWaitCursorCount > 0)
      {

         ShowWaitCursor(true);

      }

      m_iWaitCursorCount = 0;
      ShowWaitCursor(false);

   }
   else
   {

      if (m_iWaitCursorCount < 0)
      {
         m_iWaitCursorCount = 0;
      }

      m_iWaitCursorCount++;

      ShowWaitCursor(true);

   }



}

void base_application::ShowWaitCursor(bool bShow)
{

   if (m_pimpl == NULL)
      return;

   m_pimpl->ShowWaitCursor(bShow);

}



void base_application::construct()
{

}










void base_application::LockTempMaps()
{

   throw interface_only_exception(this);

}


bool base_application::UnlockTempMaps(bool bDeleteTemps)
{

   throw interface_only_exception(this);

}


void base_application::TermThread(HINSTANCE hInstTerm)
{

   throw interface_only_exception(this);

}


/*#ifdef METROWIN
sp(::user::interaction) base_application::window_from_os_data(void * pdata)
{

throw interface_only_exception(this);

}


sp(::user::interaction) base_application::window_from_os_data_permanent(void * pdata)
{

throw interface_only_exception(this);

}


#endif*/

sp(::user::window) base_application::FindWindow(const char * lpszClassName, const char * lpszWindowName)
{

   throw interface_only_exception(this);

}


sp(::user::window) base_application::FindWindowEx(oswindow oswindowParent, oswindow oswindowChildAfter, const char * lpszClass, const char * lpszWindow)
{

   throw interface_only_exception(this);

}


string base_application::get_version()
{

   throw interface_only_exception(this);

}


void base_application::set_thread(thread * pthread)
{

   m_pimpl->set_thread(pthread);

}


void base_application::SetCurrentHandles()
{

   m_pimpl->SetCurrentHandles();

}


void base_application::set_env_var(const string & var, const string & value)
{

   throw interface_only_exception(this);

}


bool base_application::set_main_init_data(::core::main_init_data * pdata)
{

   throw interface_only_exception(this);

}




bool base_application::_001OnDDECommand(const char * lpcsz)
{

   throw interface_only_exception(this);

}


void base_application::_001EnableShellOpen()
{

   throw interface_only_exception(this);

}


sp(::user::object) base_application::_001OpenDocumentFile(var varFile)
{

   throw interface_only_exception(this);

}


void base_application::_001OnFileNew(signal_details * pobj)
{

   throw interface_only_exception(this);

}


::user::printer * base_application::get_printer(const char * pszDeviceName)
{

   throw interface_only_exception(this);

}


bool base_application::update_module_paths()
{


   if (is_system())
   {

      if (!m_pimpl->update_module_paths())
         return false;

      if (m_pimpl->m_strCa2ModuleFolder.is_empty())
         m_pimpl->m_strCa2ModuleFolder = m_pimpl->m_strModuleFolder;

      m_strModulePath = m_pimpl->m_strModulePath;
      m_strModuleFolder = m_pimpl->m_strModuleFolder;
      m_strCa2ModulePath = m_pimpl->m_strCa2ModulePath;
      m_strCa2ModuleFolder = m_pimpl->m_strCa2ModuleFolder;

   }
   else
   {

      m_strModulePath = System.m_strModulePath;
      m_strModuleFolder = System.m_strModuleFolder;

   }

   return true;


}



string base_application::veriwell_multimedia_music_midi_get_default_library_name()
{

   if (m_pimpl == NULL)
      return "";

   return m_pimpl->veriwell_multimedia_music_midi_get_default_library_name();

}



string base_application::multimedia_audio_mixer_get_default_library_name()
{

   if (m_pimpl == NULL)
      return "";

   return m_pimpl->multimedia_audio_mixer_get_default_library_name();

}



string base_application::multimedia_audio_get_default_library_name()
{

   if (m_pimpl == NULL)
      return "";

   return m_pimpl->multimedia_audio_get_default_library_name();

}



string base_application::draw2d_get_default_library_name()
{

   if (m_pimpl == NULL)
      return "draw2d_cairo";

   return m_pimpl->draw2d_get_default_library_name();

}




::fontopus::user * base_application::get_user()
{

   return m_pfontopus->get_user();

}

bool base_application::get_auth(const string & pszForm, string & strUsername, string & strPassword)
{
   UNREFERENCED_PARAMETER(pszForm);
   UNREFERENCED_PARAMETER(strUsername);
   UNREFERENCED_PARAMETER(strPassword);
   return false;
}


/*::fontopus::user * application::create_user(const string & pszLogin)
{
return NULL;
}*/

::fontopus::user * base_application::create_current_user()
{
   return NULL;
   /*   string str = get_current_user_login();
   return create_user(str);*/
}

/*string application::get_current_user_login()
{
return "";
}*/


void base_application::set_locale(const string & lpcsz, bool bUser)
{
   m_strLocale = lpcsz;
   on_set_locale(lpcsz, bUser);
}

void base_application::set_schema(const string & lpcsz, bool bUser)
{
   m_strSchema = lpcsz;
   on_set_schema(lpcsz, bUser);
}

void base_application::on_set_locale(const string & lpcsz, bool bUser)
{
   UNREFERENCED_PARAMETER(bUser);
   UNREFERENCED_PARAMETER(lpcsz);
   //System.appa_load_string_table();
}

void base_application::on_set_schema(const string & lpcsz, bool bUser)
{
   UNREFERENCED_PARAMETER(bUser);
   UNREFERENCED_PARAMETER(lpcsz);
   //System.appa_load_string_table();
}



void base_application::process(machine_event_data * pdata)
{
   if (pdata->m_fixed.m_bRequestCloseApplication)
   {
      _001CloseApplication();
   }
}


void base_application::_001CloseApplication()
{

   throw todo(get_app());

}





sp(::user::interaction) base_application::release_capture_uie()
{

#if defined(LINUX)

   oswindow oswindowCapture = ::GetCapture();
   if (oswindowCapture == NULL)
      return NULL;
   return oswindowCapture->get_user_interaction()->release_capture();

#elif defined(WINDOWS)

   oswindow oswindowCapture = ::GetCapture();
   if (oswindowCapture == NULL)
      return NULL;
   return System.window_from_os_data(oswindowCapture)->release_capture();

#elif defined(MACOS)

   oswindow oswindowCapture = ::GetCapture();
   if (oswindowCapture == NULL)
      return NULL;
   return oswindowCapture->get_user_interaction()->release_capture();

#else

   throw not_implemented(get_app());

#endif

}


sp(::user::interaction) base_application::get_capture_uie()
{

#ifdef METROWIN

   oswindow oswindowCapture = ::GetCapture();

   if (oswindowCapture == NULL)
      return NULL;

   ::user::interaction * pui = oswindowCapture->window();

   if (pui == NULL)
      return NULL;

   return pui->get_capture();

#elif defined(WINDOWS)

   oswindow oswindowCapture = ::GetCapture();

   if (oswindowCapture == NULL)
      return NULL;

   ::user::window * pwindow = System.window_from_os_data(oswindowCapture).cast < ::user::window >();

   if (pwindow == NULL)
      return NULL;

   return pwindow->get_capture();

#else

   //      throw not_implemented(get_app());

   oswindow oswindowCapture = ::GetCapture();

   if (oswindowCapture == NULL)
      return NULL;

   return ::GetCapture()->get_user_interaction()->m_pimpl.cast < ::user::window >()->get_capture();

#endif

}



math::math & base_application::math()
{
   return *m_pmath;
}




::count base_application::get_monitor_count()
{

   return System.get_monitor_count();

}


bool base_application::get_monitor_rect(index iMonitor, LPRECT lprect)
{

   return System.get_monitor_rect(iMonitor, lprect);

}


::count base_application::get_desk_monitor_count()
{

   return System.get_desk_monitor_count();

}



bool base_application::get_desk_monitor_rect(index iMonitor, LPRECT lprect)
{

   return System.get_desk_monitor_rect(iMonitor, lprect);

}



bool base_application::is_licensed(const char * pszId, bool bInteractive)
{

   if (directrix()->m_varTopicQuery.has_property("install"))
      return true;

   if (directrix()->m_varTopicQuery.has_property("uninstall"))
      return true;

   return license().has(pszId, bInteractive);

}

string base_application::get_license_id()
{

   return m_strAppId;

}




string base_application::get_locale()
{
   return m_strLocale;
}

string base_application::get_schema()
{
   return m_strSchema;
}


::user::str_context * base_application::str_context()
{

   return m_puserstrcontext;

}


string base_application::get_locale_schema_dir()
{

   return System.dir().simple_path(get_locale(), get_schema());

}

string base_application::get_locale_schema_dir(const string & strLocale)
{

   if (strLocale.is_empty())
      return System.dir().simple_path(get_locale(), get_schema());
   else
      return System.dir().simple_path(strLocale, get_schema());

}

string base_application::get_locale_schema_dir(const string & strLocale, const string & strSchema)
{
   if (strLocale.is_empty())
   {
      if (strSchema.is_empty())
         return System.dir().simple_path(get_locale(), get_schema());
      else
         return System.dir().simple_path(get_locale(), strSchema);
   }
   else
   {
      if (strSchema.is_empty())
         return System.dir().simple_path(strLocale, get_schema());
      else
         return System.dir().simple_path(strLocale, strSchema);
   }
}



void base_application::get_cursor_pos(LPPOINT lppoint)
{
   if (is_system())
   {
      if (m_bSessionSynchronizedCursor)
      {
         ::GetCursorPos(&m_ptCursor);
      }
      if (lppoint != NULL)
      {
         *lppoint = m_ptCursor;
      }
   }
   else if (is_session())
   {
      if (m_bSessionSynchronizedCursor)
      {
         System.get_cursor_pos(&m_ptCursor);
      }
      if (lppoint != NULL)
      {
         *lppoint = m_ptCursor;
      }
   }
   else
   {
      Session.get_cursor_pos(lppoint);
   }
}



sp(::user::interaction) base_application::get_focus_guie()
{

#ifdef METROWIN

   return GetFocus()->window();

#elif defined(WINDOWSEX) || defined(LINUX)

   sp(::user::window) pwnd = System.window_from_os_data_permanent(::GetFocus());
   if (pwnd != NULL)
   {
      if (System.get_active_guie()->get_safe_handle() == pwnd->get_safe_handle()
         || ::user::window_util::IsAscendant(System.get_active_guie()->get_safe_handle(), pwnd->get_safe_handle()))
      {
         return pwnd;
      }
      else
      {
         return NULL;
      }
   }
   pwnd = System.window_from_os_data(::GetFocus());
   if (pwnd != NULL)
   {
      if (System.get_active_guie()->get_safe_handle() == pwnd->get_safe_handle()
         || ::user::window_util::IsAscendant(System.get_active_guie()->get_safe_handle(), pwnd->get_safe_handle()))
      {
         return pwnd;
      }
      else
      {
         return NULL;
      }
   }
   return NULL;

#else

   return System.get_active_guie();

#endif

}



uint32_t base_application::get_thread_id()
{
   return m_pimpl->get_thread_id();
}




sp(::user::interaction) base_application::get_active_guie()
{

#if defined(WINDOWSEX) || defined(LINUX) || defined(MACOS)

   return window_from_os_data(::GetActiveWindow());

#else

   if (frames().get_size() <= 0)
      return NULL;

   return frames()(0);

#endif

}


geometry::geometry & base_application::geometry()
{

   return *m_pgeometry;

}


#ifndef METROWIN

void base_application::get_time(timeval *p)
{
   m_pimpl->get_time(p);
}

#endif


bool base_application::do_prompt_file_name(var & varFile, UINT nIDSTitle, uint32_t lFlags, bool bOpenFileDialog, sp(::user::impact_system) ptemplate, sp(::user::object) pdocument)
{

   UNREFERENCED_PARAMETER(varFile);
   UNREFERENCED_PARAMETER(nIDSTitle);

   return false;

}

string CLASS_DECL_BASE base_application::show_auth_window(LPRECT lprect, string & strUsername, string & strSessId, string & strServerId, string & strLoginUrl, string strFontopusServer)
{

   return ::fontopus::show_auth_window(this, lprect, strUsername, strSessId, strServerId, strLoginUrl, strFontopusServer);

}




int_bool base_application::get_temp_file_name_template(char * szRet, ::count iBufferSize, const char * pszName, const char * pszExtension, const char * pszTemplate)
{

#ifdef METROWIN

   string str(::Windows::Storage::ApplicationData::Current->TemporaryFolder->Path);

#else

   char lpPathBuffer[MAX_PATH * 4];

   uint32_t dwRetVal = GetTempPath(sizeof(lpPathBuffer), lpPathBuffer);

   if (dwRetVal > sizeof(lpPathBuffer) || (dwRetVal == 0))
   {

      return FALSE;

   }

   string str(lpPathBuffer);

#endif

   char bufItem[64];

   string strRelative;

   SYSTEMTIME st;

   memset_dup(&st, 0, sizeof(st));

   GetSystemTime(&st);

   itoa_dup(bufItem, st.wYear, 10);
   zero_pad(bufItem, 4);
   strRelative += bufItem;

   itoa_dup(bufItem, st.wMonth, 10);
   zero_pad(bufItem, 2);
   strRelative += "-";
   strRelative += bufItem;

   itoa_dup(bufItem, st.wDay, 10);
   zero_pad(bufItem, 2);
   strRelative += "-";
   strRelative += bufItem;

   itoa_dup(bufItem, st.wHour, 10);
   zero_pad(bufItem, 2);
   strRelative += " ";
   strRelative += bufItem;

   itoa_dup(bufItem, st.wMinute, 10);
   zero_pad(bufItem, 2);
   strRelative += "-";
   strRelative += bufItem;

   itoa_dup(bufItem, st.wSecond, 10);
   zero_pad(bufItem, 2);
   strRelative += "-";
   strRelative += bufItem;

   string strRet;

   for (int32_t i = 0; i < (1024 * 1024); i++)
   {
      strRet = System.dir().path(str, strRelative + "-" + hex::lower_from(i + 1), string(pszName) + string(".") + pszExtension);
      if (pszTemplate != NULL)
      {
         if (System.install().is_file_ok(szRet, pszTemplate))
            return true;
      }
      if (file_exists_dup(szRet))
      {
         try
         {

            m_pbasesystem->file().del(szRet);

         }
         catch (...)
         {

            return false;

         }

         return true;

      }
      else
      {
         return true;
      }
   }
   return FALSE;


}


int_bool base_application::get_temp_file_name(char * szRet, ::count iBufferSize, const char * pszName, const char * pszExtension)
{

   return get_temp_file_name_template(szRet, iBufferSize, pszName, pszExtension, NULL);

}



void base_application::get_screen_rect(LPRECT lprect)
{
#ifdef METROWIN
   if (m_bSessionSynchronizedScreen)
   {
      System.get_monitor_rect(0, m_rectScreen);
   }
#elif defined(LINUX)
   if (m_bSessionSynchronizedScreen)
   {
      System.get_monitor_rect(0, m_rectScreen);
   }
#elif defined(MACOS)
   if (m_bSessionSynchronizedScreen)
   {
      System.get_monitor_rect(0, m_rectScreen);
   }
#else
   if (m_bSessionSynchronizedScreen)
   {
      oswindow oswindowDesktop = ::GetDesktopWindow();
      ::GetWindowRect(oswindowDesktop, &m_rectScreen);
   }
#endif
   if (lprect != NULL)
   {
      *lprect = m_rectScreen;
   }
}




#if defined(MACOS)

void openURL(const string &url_str);


void openURL(const string &url_str) {
   CFURLRef url = CFURLCreateWithBytes (
      NULL,                        // allocator
      (UInt8*)url_str.c_str(),     // URLBytes
      url_str.length(),            // length
      kCFStringEncodingASCII,      // encoding
      NULL                         // baseURL
      );
   LSOpenCFURLRef(url,0);
   CFRelease(url);
}
#endif


void base_application::Ex1OnFactoryExchange()
{

   System.factory().creatable_large < ::file::exception >();

   static base_library library(this);

   if (!library.open("os"))
      throw "failed to do factory exchange";

   PFN_ca2_factory_exchange pfn_ca2_factory_exchange = library.get < PFN_ca2_factory_exchange >("ca2_factory_exchange");

   pfn_ca2_factory_exchange(this);

}

::core::savings & base_application::savings()
{

   return *m_psavings;

}


string base_application::get_ca2_module_folder()
{
   single_lock sl(&m_mutex, true);
   return m_strCa2ModuleFolder;
}

string base_application::get_ca2_module_file_path()
{

   string strModuleFileName;

#ifdef WINDOWSEX

   char lpszModuleFilePath[MAX_PATH + 1];

   if (GetModuleFileName(::GetModuleHandleA("core.dll"), lpszModuleFilePath, MAX_PATH + 1))
   {

      strModuleFileName = lpszModuleFilePath;

   }

#elif defined(METROWIN)

   throw todo(this);

#else

#ifdef RTLD_DI_LINKMAP

   {

      void * handle = dlopen("core.so", 0);

      if (handle == NULL)
         return false;

      link_map * plm;

      dlinfo(handle, RTLD_DI_LINKMAP, &plm);

      strModuleFileName = plm->l_name;

      dlclose(handle);

      //         m_strCa2ModuleFolder = dir::name(strModuleFileName);

   }

#else

   {

      char * pszCurDir = getcwd(NULL, 0);

      string strCurDir = pszCurDir;

      free(pszCurDir);

      if (App(this).file().exists(System.dir().path(strCurDir, "core.dylib")))
      {
         m_strCa2ModuleFolder = strCurDir;
         goto finishedCa2Module;
      }


      if (App(this).file().exists(System.dir().path(m_strModuleFolder, "core.dylib")))
      {
         m_strCa2ModuleFolder = m_strModuleFolder;
         goto finishedCa2Module;
      }

      strModuleFileName = App(this).dir().pathfind(getenv("LD_LIBRARY_PATH"), "core.dylib", "rfs"); // readable - normal file - non zero sized

   }

finishedCa2Module:;

#endif

#endif

   return strModuleFileName;


}

string base_application::get_module_folder()
{
   return m_strModuleFolder;
}

string base_application::get_module_file_path()
{

#ifdef WINDOWSEX

   char lpszModuleFilePath[MAX_PATH + 1];

   GetModuleFileName(NULL, lpszModuleFilePath, MAX_PATH + 1);

   string strModuleFileName(lpszModuleFilePath);

   return strModuleFileName;

#elif defined(METROWIN)

   return "m_app.exe";

#else

   char * lpszModuleFilePath = br_find_exe_dir("app");

   if (lpszModuleFilePath == NULL)
      return "";

   string strModuleFileName(lpszModuleFilePath);

   free(lpszModuleFilePath);

   return strModuleFileName;

#endif

}


string base_application::get_module_title()
{
   return file_title(get_module_file_path());
}

string base_application::get_module_name()
{
   return file_name(get_module_file_path());
}


::visual::icon * base_application::set_icon(object * pobject, ::visual::icon * picon, bool bBigIcon)
{

   ::visual::icon * piconOld = get_icon(pobject, bBigIcon);

   if (bBigIcon)
   {

      pobject->oprop("big_icon").operator =((sp(element)) picon);

   }
   else
   {

      pobject->oprop("small_icon").operator =((sp(element)) picon);

   }

   return piconOld;

}


::visual::icon * base_application::get_icon(object * pobject, bool bBigIcon) const
{

   if (bBigIcon)
   {

      return const_cast <object *> (pobject)->oprop("big_icon").cast < ::visual::icon >();

   }
   else
   {

      return const_cast <object *> (pobject)->oprop("small_icon").cast < ::visual::icon >();

   }

}



bool base_application::final_handle_exception(::exception::exception & e)
{
   UNREFERENCED_PARAMETER(e);
   //linux      exit(-1);

   if (!is_system())
   {

      // get_app() may be it self, it is ok...
      if (Sys(get_app()).final_handle_exception((::exception::exception &) e))
         return true;


   }

   return false;
}


::fontopus::fontopus * base_application::create_fontopus()
{

   return canew(::fontopus::fontopus(this));

}


bool base_application::init_main_data(::core::main_init_data * pdata)
{

   return false;

}




int32_t base_application::main()
{

   TRACE(string(typeid(*this).name()) + " main");;

   try
   {

      if (!main_start())
      {
         m_bReady = true;
         TRACE("application::start failure");
         return m_iReturnCode;
      }

   }
   catch (::exit_exception &)
   {

      System.os().post_to_all_threads(WM_QUIT, 0, 0);

      goto exit_application;

   }
   catch (...)
   {

      goto exit_application;

   }


   try
   {

      TRACE(string(typeid(*this).name()) + " on_run");;
      m_iReturnCode = 0;
      m_bReady = true;
      m_p->m_bReady = true;
      thread::m_bReady = true;
      thread::m_p->m_bReady = true;
      m_bRun = true;
      m_p->m_bRun = true;
      thread::m_bRun = true;
      thread::m_p->m_bRun = true;
      m_iReturnCode = on_run();
      if (m_iReturnCode != 0)
      {
         ::OutputDebugStringW(L"application::main on_run termination failure");
      }

   }
   catch (::exit_exception &)
   {

      System.os().post_to_all_threads(WM_QUIT, 0, 0);

      goto exit_application;

   }
   catch (...)
   {

      goto exit_application;

   }

   try
   {

      if (is_system())
      {

         System.os().post_to_all_threads(WM_QUIT, 0, 0);

         Sleep(1984 + 1977);

      }

   }
   catch (...)
   {

   }

exit_application:

   try
   {

      m_iReturnCode = exit();

   }
   catch (::exit_exception &)
   {

      System.os().post_to_all_threads(WM_QUIT, 0, 0);

      m_iReturnCode = -1;

   }
   catch (...)
   {

      m_iReturnCode = -1;

   }

   return m_iReturnCode;

}




bool base_application::main_start()
{

   TRACE(string(typeid(*this).name()) + " main_start");;
   try
   {

      m_dwAlive = ::get_tick_count();
      TRACE(string(typeid(*this).name()) + "pre_run");;
      int32_t m_iReturnCode = pre_run();
      if (m_iReturnCode != 0)
      {
         m_bReady = true;
         TRACE("application::main pre_run failure");
         return false;
      }

      TRACE(string(typeid(*this).name()) + " initial_check_directrix");;
      if (!initial_check_directrix())
      {
         exit();
         m_iReturnCode = -1;
         m_bReady = true;
         ::OutputDebugStringW(L"exiting on check directrix");
         return false;
      }


      TRACE(string(typeid(*this).name()) + " os_native_bergedge_start");;
      m_dwAlive = ::get_tick_count();
      if (!os_native_bergedge_start())
      {
         exit();
         m_iReturnCode = -1;
         m_bReady = true;
         ::OutputDebugStringW(L"application::main os_native_bergedge_start failure");
         return false;
      }

      return true;
   }
   catch (::exit_exception &)
   {

      System.os().post_to_all_threads(WM_QUIT, 0, 0);

   }

   return false;

}




int32_t base_application::on_run()
{
   int32_t m_iReturnCode = 0;

   try
   {
      application_signal_details signal(this, m_psignal, application_signal_start);
      m_psignal->emit(&signal);
   }
   catch (...)
   {
   }

   thread * pthread = System.GetThread();

   install_message_handling(pthread->m_p);
   try
   {
      try
      {
         m_bReady = true;
         if (m_peventReady != NULL)
            m_peventReady->SetEvent();
      }
      catch (...)
      {
      }
   run:
      try
      {
         m_iReturnCode = run();

      }
      catch (::exit_exception & e)
      {

         throw e;

      }
      catch (const ::exception::exception & e)
      {
         if (on_run_exception((::exception::exception &) e))
            goto run;
         if (GetMainWnd() != NULL)
         {
            TRACE(::core::trace::category_AppMsg, 0, "Warning: Destroying non-NULL GetMainWnd()\n");
            try
            {
               GetMainWnd()->DestroyWindow();
            }
            catch (::exception::exception &)
            {
            }
            SetMainWnd(NULL);
         }
         if (final_handle_exception((::exception::exception &) e))
            goto run;
         if (GetMainWnd() != NULL)
         {
            TRACE(::core::trace::category_AppMsg, 0, "Warning: Destroying non-NULL GetMainWnd()\n");
            try
            {
               GetMainWnd()->DestroyWindow();
            }
            catch (::exit_exception & e)
            {

               throw e;

            }
            catch (::exception::exception &)
            {
            }
            SetMainWnd(NULL);
         }
         try
         {
            m_iReturnCode = exit();
         }
         catch (::exit_exception & e)
         {

            throw e;

         }
         catch (...)
         {
            m_iReturnCode = -1;
         }
         goto InitFailure;
      }
   }
   catch (::exit_exception & e)
   {

      throw e;

   }
   catch (...)
   {
      // linux-like exit style on crash, differently from general windows error message approach
      // to prevent or correct from crash, should:
      // - look at dumps - to do;
      // - look at trace and log - always doing;
      // - look at debugger with the visual or other tool atashed - to doing;
      // - fill other possibilities;
      // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
      // - ...
      // - ..
      // - .
      // - .
      // - .
      // - .
      // -  .
      // - ...
      // - ...
      // - ...
      // to pro-activia and overall benefits workaround:
      // - stateful applications:
      //      - browser urls, tabs, entire history, in the ca2computing cloud;
      //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
      //           - not directly related but use date and title to name document;
      //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
      //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
      //
      // - ex-new-revolut-dynamic-news-self-healing
      //      - pre-history, antecendentes
      //            - sometimes we can't recover from the last state
      //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
      //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
      //           - reification :
      //           - if the document is corrupted, try to open the most of it
      //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
      //              done, for example search in the web for a proper solution?
      //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
      //           - import a little as pepper for the meal, prodevian technology into estamira, so gaming experience relativity can open ligh
      //               speed into cartesian dimensions of
      //               core, estamira and prodevian. Take care not to flood prodevian brand black ink over the floor of the estamira office...
      //               black letters, or colorful and pink are accepted and sometimes desired, for example, hello kity prodevian, pirarucu games,
      //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
      //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
      //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
      //               - new version with bug fixes;
      //      - new versions
      //      - automatic updates
      //      - upgrades
      //      - rearrangemntes
      //      - downgrade in the form of retro
      // - ...
      // - ..
      // - .
      // - .
      // - .
      // - .
      // -  .
      // - ...
      // - ...
      // - ...

   }
InitFailure:
   try
   {
      if (m_peventReady != NULL)
         m_peventReady->SetEvent();
   }
   catch (...)
   {
   }
   try
   {
      thread * pthread = thread::m_p;
      if (pthread != NULL && pthread->m_pbReady != NULL)
      {
         *pthread->m_pbReady = true;
      }
   }
   catch (...)
   {
   }
   /*try
   {
   thread * pthread = dynamic_cast < thread * > (this);
   ::SetEvent((HANDLE) pthread->m_peventReady);
   }
   catch(...)
   {
   }*/

   // let translator run undefinetely
   /*if(is_system())
   {
   translator::detach();
   }*/

   return m_iReturnCode;
}




int32_t base_application::pre_run()
{

   //      m_dir.m_psystem      = m_psystem;
   //    m_file.m_psystem     = m_psystem;

#ifdef WINDOWSEX

   MESSAGE msg;

   // Create Windows Message Queue
   ::PeekMessageA(&msg, NULL, 0, 0xffff, 0);

   if (!is_system() && (bool)oprop("SessionSynchronizedInput"))
   {
      ::AttachThreadInput(GetCurrentThreadId(), (uint32_t)System.thread::m_p->get_os_int(), TRUE);
   }

#endif
   /*

   if(is_system())
   {
   translator::attach();
   }
   */
   m_iReturnCode = 0;

   m_dwAlive = ::get_tick_count();

   // App global initializations (rare)
   if (!InitApplication())
      goto InitFailure;

   m_dwAlive = ::get_tick_count();
   // Perform specific initializations
   //#if !defined(DEBUG) || defined(WINDOWS)
   try
   {
      try
      {
         //#endif
         if (!process_initialize())
         {
            if (GetMainWnd() != NULL)
            {
               TRACE(::core::trace::category_AppMsg, 0, "Warning: Destroying non-NULL GetMainWnd()\n");
               GetMainWnd()->DestroyWindow();
            }
            goto InitFailure;
         }
         //#if !defined(DEBUG) || defined(WINDOWS)
      }
      catch (::exit_exception & e)
      {

         throw e;

      }
      catch (const ::exception::exception &)
      {
         if (GetMainWnd() != NULL)
         {
            GetMainWnd()->DestroyWindow();
            SetMainWnd(NULL);
         }
         goto InitFailure;
      }
      //#endif
      m_dwAlive = ::get_tick_count();
      //#if !defined(DEBUG) || defined(WINDOWS)
      try
      {
         //#endif
         if (!initialize_instance())
         {
            if (GetMainWnd() != NULL)
            {
               TRACE(::core::trace::category_AppMsg, 0, "Warning: Destroying non-NULL GetMainWnd()\n");
               GetMainWnd()->DestroyWindow();
            }

            //#if !defined(DEBUG) || defined(WINDOWS)
            try
            {
               //#endif
               exit();
               //#if !defined(DEBUG) || defined(WINDOWS)
            }
            catch (...)
            {
            }
            //#endif
            goto InitFailure;
         }
         //#if !defined(DEBUG) || defined(WINDOWS)
      }
      catch (::exit_exception & e)
      {

         throw e;

      }
      catch (const ::exception::exception & e)
      {
         if (on_run_exception((::exception::exception &) e))
            goto run;
         if (GetMainWnd() != NULL)
         {
            TRACE(::core::trace::category_AppMsg, 0, "Warning: Destroying non-NULL GetMainWnd()\n");
            try
            {
               GetMainWnd()->DestroyWindow();
            }
            catch (::exit_exception & e)
            {

               throw e;

            }
            catch (::exception::exception &)
            {
            }
            SetMainWnd(NULL);
         }
         if (final_handle_exception((::exception::exception &) e))
            goto run;
         if (GetMainWnd() != NULL)
         {
            TRACE(::core::trace::category_AppMsg, 0, "Warning: Destroying non-NULL GetMainWnd()\n");
            try
            {
               GetMainWnd()->DestroyWindow();
            }
            catch (::exit_exception & e)
            {

               throw e;

            }
            catch (::exception::exception &)
            {
            }
            SetMainWnd(NULL);
         }
         try
         {
            m_iReturnCode = exit();
         }
         catch (...)
         {
            m_iReturnCode = -1;
         }
         if (m_iReturnCode == 0)
            m_iReturnCode = -1;
         goto InitFailure;
      }
   }
   catch (::exit_exception & e)
   {

      throw e;

   }
   catch (...)
   {
      // linux-like exit style on crash, differently from general windows error message approach
      // to prevent or correct from crash, should:
      // - look at dumps - to do;
      // - look at trace and log - always doing;
      // - look at debugger with the visual or other tool atashed - to doing;
      // - fill other possibilities;
      // - restart and send information in the holy back, and stateful or self-heal as feedback from below;
      // - ...
      // - ..
      // - .
      // - .
      // - .
      // - .
      // -  .
      // - ...
      // - ...
      // - ...
      // to pro-activia and overall benefits workaround:
      // - stateful applications:
      //      - browser urls, tabs, entire history, in the ca2computing cloud;
      //      - uint16_t - html document to simplify here - with all history of undo and redos per document optimized by cvs, svn, syllomatter;
      //           - not directly related but use date and title to name document;
      //      - save forms after every key press in .undo.redo.form file parallel to appmatter / form/undo.redo.file parity;
      //      - last ::ikaraoke::karaoke song and scoring, a little less motivated at time of writing;
      //
      // - ex-new-revolut-dynamic-news-self-healing
      //      - pre-history, antecendentes
      //            - sometimes we can't recover from the last state
      //            - to start from the beggining can be too heavy, waity, worky, bory(ing)
      //      - try to creativetily under-auto-domain with constrained-learning, or heuristcally recover from restart, shutdown, login, logoff;
      //           - reification :
      //           - if the document is corrupted, try to open the most of it
      //           - if can only detect that the document cannot be opened or damaged, should creatively workarounds as it comes, as could it be
      //              done, for example search in the web for a proper solution?
      //           - ::ikaraoke::karaoke file does not open? can open next? do it... may animate with a temporary icon...
      //           - import a little as pepper for the meal, prodevian technology into estamira, so gaming experience relativity can open ligh
      //               speed into cartesian dimensions of
      //               core, estamira and prodevian. Take care not to flood prodevian brand black ink over the floor of the estamira office...
      //               black letters, or colorful and pink are accepted and sometimes desired, for example, hello kity prodevian, pirarucu games,
      //               I think no one likes to be boring, but a entire background in black... I don't know... only for your personal office, may be...
      //           - could an online colaborator investigate crashes promptly in a funny way, and make news and jokes? Like terra and UOL for the real world?
      //               - new crash, two documents lost, weathers fault, too hot, can't think, my mother was angry with me, lead to buggy code;
      //               - new version with bug fixes;
      //      - new versions
      //      - automatic updates
      //      - upgrades
      //      - rearrangemntes
      //      - downgrade in the form of retro
      // - ...
      // - ..
      // - .
      // - .
      // - .
      // - .
      // -  .
      // - ...
      // - ...
      // - ...

   }
   //#endif
   goto run;
InitFailure:
   if (m_iReturnCode == 0)
      m_iReturnCode = -1;
run:
   return m_iReturnCode;
}



bool base_application::InitApplication()
{

   return true;

}



bool base_application::initial_check_directrix()
{

   if (directrix()->m_varTopicQuery.has_property("install"))
   {

      if (!on_install())
         return false;

      string strId = m_strAppId;

      xxdebug_box("on_install1", strId, 0);

      if (strId.is_empty())
         strId = m_strAppName;

      if (strId.has_char() && command()->m_varTopicQuery.has_property("app") && strId == command()->m_varTopicQuery["app"])
      {

         system_add_app_install(strId);

      }
      else if (strId.has_char() && command()->m_varTopicQuery.has_property("session_start") && strId == command()->m_varTopicQuery["session_start"])
      {

         system_add_app_install(strId);

      }
      else if (m_strInstallToken.has_char())
      {

         system_add_app_install(m_strInstallToken);

      }

   }
   else if (directrix()->m_varTopicQuery.has_property("uninstall"))
   {

      if (!on_uninstall())
         return false;

      System.install().remove_spa_start(m_strInstallType, m_strInstallToken);

   }

   return true;

}


bool base_application::on_install()
{

   return true;

}


bool base_application::on_uninstall()
{

   return true;

}


bool base_application::system_add_app_install(const char * pszId)
{

   string strId(pszId);
   string strSystemLocale = System.m_strLocale;
   string strSystemSchema = System.m_strSchema;
   string strLocale = command()->m_varTopicQuery["locale"];
   string strSchema = command()->m_varTopicQuery["schema"];

   System.install().remove_spa_start(m_strInstallType, strId);
   System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, strSystemLocale, m_strSchema);
   System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, strSystemLocale, strSystemSchema);
   System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, m_strLocale, m_strSchema);
   System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, m_strLocale, strSchema);
   System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, strLocale, m_strSchema);
   System.install().add_app_install(System.command()->m_varTopicQuery["build_number"], m_strInstallType, strId, strLocale, strSchema);

   return true;

}



bool base_application::os_native_bergedge_start()
{

   return true;

}











//   void application::construct(const char *pszId)
//   {
//      //if(m_strAppName.has_char())
//      //   return;
//      //m_strAppName.Empty();
//      //m_strId.Empty();
//      if (pszId == NULL)
//      {
//#ifdef WINDOWSEX
//         wstring wstr = ::GetCommandLineW();
//         string str = ::str::international::unicode_to_utf8(wstr);
//         strsize iFind = str.find(" : ");
//         if (iFind >= 0)
//         {
//            iFind = str.find("app=", iFind);
//            if (iFind >= 0)
//            {
//               strsize iEnd = str.find(" ", iFind);
//               if (iEnd < 0)
//               {
//                  m_strId = str.Mid(iFind + 4);
//               }
//               else
//               {
//                  m_strId = str.Mid(iFind + 4, iEnd - iFind - 4);
//               }
//               ::str::begins_eat(m_strId, "\"");
//               ::str::ends_eat(m_strId, "\"");
//            }
//         }
//#endif
//      }
//      else
//      {
//         m_strId = pszId;
//      }
//      if (m_strId.is_empty())
//         m_strId = "mplite";
//      construct();
//      if (m_strAppName.is_empty())
//      {
//         if (m_strAppId.has_char())
//            m_strAppName = m_strAppId;
//         else if (m_strInstallToken.has_char())
//            m_strAppName = m_strInstallToken;
//      }
//   }


//void application::construct()
//{
//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }

//   ::application::construct();

//}




//void application::_001OnFileNew()
//{
//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }
//   ::application::_001OnFileNew(NULL);
//}


//bool application::bergedge_start()
//{
//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }
//   return ::application::bergedge_start();
//}



//bool application::on_install()
//{
//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }
//   return ::application::on_install();
//}

//bool application::on_uninstall()
//{
//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }
//   return ::application::on_uninstall();
//}


//void application::on_request(sp(::create_context) pcreatecontext)
//{
//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }
//   return ::application::on_request(pcreatecontext);


//}

//bool application::is_serviceable()
//{


//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }
//   return ::application::is_serviceable();
//}

//service_base * application::allocate_new_service()
//{

//   return NULL;

//}


//sp(::user::object) application::_001OpenDocumentFile(var varFile)
//{
//   string strId = m_strId;
//   char chFirst = '\0';
//   if (strId.get_length() > 0)
//   {
//      chFirst = strId[0];
//   }
//   return ::application::_001OpenDocumentFile(varFile);

//}


int32_t base_application::run()
{

   if ((command()->m_varTopicQuery.has_property("install")
      || command()->m_varTopicQuery.has_property("uninstall"))
      &&
      ((is_session() && command()->m_varTopicQuery["session_start"] == "session")))
   {
   }
   else if (!is_system() && !is_session())
   {
      if (command()->m_varTopicQuery.has_property("install")
         || command()->m_varTopicQuery.has_property("uninstall"))
      {

      }
      else if (command()->m_varTopicQuery.has_property("service"))
      {
         create_new_service();
         ::service_base::serve(*m_pservice);
      }
      else if (command()->m_varTopicQuery.has_property("run") || is_serviceable())
      {
         create_new_service();
         m_pservice->Start(0);
         return ::thread::run();
      }
      else
      {
         return ::thread::run();
      }
   }
   else
   {
      return ::thread::run();
   }

   return 0;

}




sp(::base_application) base_application::assert_running(const char * pszAppId)
{


   sp(::base_application) papp = NULL;


   try
   {

      bool bFound = false;

      for (int32_t i = 0; i < System.m_appptra.get_count(); i++)
      {
         try
         {

            papp = System.m_appptra(i);

            if (papp->m_strAppName == pszAppId)
            {
               bFound = true;
               break;
            }

         }
         catch (...)
         {
         }

      }

      bool bCreate = !bFound;

      if (bFound)
      {

         bool bRunning = false;

         try
         {
            if (papp->is_running())
            {
               bRunning = true;
            }
         }
         catch (...)
         {
         }

         if (!bRunning)
         {

            try
            {
               papp->post_thread_message(WM_QUIT);
            }
            catch (...)
            {
            }
            try
            {
               papp.release();
            }
            catch (...)
            {
            }

            bCreate = true;

         }


      }

      if (bCreate)
      {

         sp(::create_context) spcreatecontext(allocer());

         papp = Session.start_application("application", pszAppId, spcreatecontext);

      }

   }
   catch (const ::exit_exception & e)
   {

      throw e;

   }
   catch (const ::exception::exception & e)
   {

      if (!Application.on_run_exception((::exception::exception &) e))
         throw exit_exception(get_app());

   }
   catch (...)
   {

      papp = NULL;

   }


   return papp;

}





typedef  void(*PFN_ca2_factory_exchange)(sp(base_application) papp);




/*::file::binary_buffer_sp application::friendly_get_file(var varFile, UINT nOpenFlags)
{

try
{

return m_file.get_file(varFile, nOpenFlags);

}
catch (::file::exception & e)
{

string strMessage = e.get_message();

App(this).simple_message_box(NULL, strMessage, MB_OK);

return NULL;

}

}
*/



::user::user * base_application::create_user()
{

   return canew(::user::user(this));

}





bool base_application::is_installing()
{

   return directrix()->has_property("install");

}


bool base_application::is_uninstalling()
{

   return directrix()->has_property("uninstall");

}


bool base_application::create_new_service()
{

   if (m_pservice != NULL)
      return false;

   m_pservice = allocate_new_service();

   if (m_pservice == NULL)
      return false;

   return true;

}



bool base_application::create_service()
{

   return System.os().create_service(this);

}

bool base_application::remove_service()
{

   return System.os().remove_service(this);

}

bool base_application::start_service()
{

   return System.os().start_service(this);

}

bool base_application::stop_service()
{

   return System.os().stop_service(this);

}


void base_application::on_service_request(sp(::create_context) pcreatecontext)
{

   if (!is_serviceable())
      return;

   if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("create_service"))
   {
      create_service();
   }
   else if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("start_service"))
   {
      start_service();
   }
   else if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("stop_service"))
   {
      stop_service();
   }
   else if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("remove_service"))
   {
      remove_service();
   }

}



/*   void application::defer_initialize_twf()
   {
   if (System.m_ptwf == NULL && (System.m_bShouldInitializeGTwf && m_bShouldInitializeGTwf && m_bInitializeProDevianMode))
   {
   System.create_twf();
   }
   }


   */

/*

sp(base_application) application::instantiate_application(const char * pszType, const char * pszId, application_bias * pbias)
{

sp(base_application) papp = NULL;

string strId(pszId);

if (strId.CompareNoCase("session") == 0)
{

::plane::session * psession = new ::plane::session(this);

papp = psession;

psession->construct();

if (m_psystem != NULL && m_psystem->m_psession == NULL)
{

m_psystem->m_psession = psession;

}

psession->m_strAppId = "session";

}
else
{

string strNewId;

if (strId == "bergedge")
{

strNewId = "app/core/bergedge";

}
else if (strId == "cube")
{

strNewId = "app/core/cube";

}
else
{

strNewId = strId;

}

papp = System.get_new_app(this, pszType, strNewId);

if (papp == NULL)
return NULL;

papp->m_pplaneapp->m_psession = m_psession;
papp->m_pplaneapp->m_pbasesession = m_psession;

/*if(pbaseapp->m_bService)
{

App(pbaseapp).m_puiInitialPlaceHolderContainer  = NULL;

}*/

/*    if (m_psystem != NULL && m_psystem->m_psession == NULL)
    {

    m_psystem->m_psession = m_psession;

    }

    if (papp != NULL)
    {

    if (strId == "bergedge"
    || strId == "cube")
    {

    papp->m_pplaneapp->m_strAppId = strId;

    }

    if (papp->m_pplaneapp->m_strInstallToken.is_empty())
    {

    papp->m_pplaneapp->m_strInstallToken = papp->m_pplaneapp->m_strAppId;

    }

    }

    }

    //pbaseapp->m_pbaseapp                               = this;
    papp->m_pplaneapp->m_psystem = m_psystem;

    papp->m_pbasesystem = m_pbasesystem;

    papp->m_pplaneapp->command_central()->consolidate(command_central());

    papp->m_pplaneapp->m_bSessionSynchronizedCursor = m_bSessionSynchronizedCursor;

    if (pbias != NULL)
    {

    papp->m_pplaneapp->propset().merge(pbias->m_set);

    }
    else
    {

    papp->m_pplaneapp->oprop("SessionSynchronizedInput") = true;
    papp->m_pplaneapp->oprop("NativeWindowFocus") = true;

    }

    if ((papp == NULL || papp->m_pplaneapp->m_strAppId != strId)
    &&
    (!Application.command()->m_varTopicQuery.has_property("install")
    && !Application.command()->m_varTopicQuery.has_property("uninstall")))
    {

    TRACE("Failed to instantiate %s, going to try installation through ca2_cube_install", strId);

    string strCommandLine;

    strCommandLine = " : app=" + strId;
    strCommandLine += " locale=" + string(Application.str_context()->m_plocaleschema->m_idLocale);
    strCommandLine += " style=" + string(Application.str_context()->m_plocaleschema->m_idSchema);
    strCommandLine += " install";

    System.install().start(strCommandLine);

    throw installing_exception(get_app());

    return NULL;

    }

    return papp;

    }*/

/*
   sp(base_application) application::create_application(const char * pszType, const char * pszId, bool bSynch, application_bias * pbias)
   {

   sp(base_application) pbaseapp = instantiate_application(pszType, pszId, pbias);

   if (pbaseapp == NULL)
   return NULL;

   sp(base_application) papp = (pbaseapp);

   if (!papp->m_pplaneapp->start_application(bSynch, pbias))
   {
   try
   {
   pbaseapp.release();
   }
   catch (...)
   {
   }
   return NULL;
   }


   return pbaseapp;

   }

   */
//////////////////////////////////////////////////////////////////////////////////////////////////
// System/System
//
/*sp(::user::object) application::place_hold(sp(::user::interaction) pui)
{

return NULL;

}
*/
/*
   count application::get_monitor_count()
   {

   return 0;

   }

   bool application::get_monitor_rect(index i, LPRECT lprect)
   {

   return false;

   }

   count application::get_desk_monitor_count()
   {

   return 0;

   }

   bool application::get_desk_monitor_rect(index i, LPRECT lprect)
   {

   return false;

   }

   */
/*
   void application::set_title(const char * pszTitle)
   {

   Session.set_app_title(m_strInstallType, m_strAppName, pszTitle);

   }


   //////////////////////////////////////////////////////////////////////////////////////////////////
   // Session/Session
   //
   sp(::bergedge::view) application::get_view()
   {

   return NULL;

   }

   sp(::bergedge::document) application::get_document()
   {

   return NULL;

   }
   */

void base_application::fill_locale_schema(::str::international::locale_schema & localeschema, const char * pszLocale, const char * pszSchema)
{


   localeschema.m_idaLocale.remove_all();
   localeschema.m_idaSchema.remove_all();


   string strLocale(pszLocale);
   string strSchema(pszSchema);


   localeschema.m_idLocale = pszLocale;
   localeschema.m_idSchema = pszSchema;


   localeschema.add_locale_variant(strLocale, strSchema);
   localeschema.add_locale_variant(get_locale(), strSchema);
   localeschema.add_locale_variant(__id(std), strSchema);
   localeschema.add_locale_variant(__id(en), strSchema);


   localeschema.finalize();


}


void base_application::fill_locale_schema(::str::international::locale_schema & localeschema)
{


   localeschema.m_idaLocale.remove_all();
   localeschema.m_idaSchema.remove_all();


   //localeschema.m_bAddAlternateStyle = true;


   string strLocale;
   string strSchema;

   strLocale = get_locale();
   strSchema = get_schema();

   if (Application.directrix()->m_varTopicQuery["locale"].has_char() && Application.directrix()->m_varTopicQuery["locale"].get_string().CompareNoCase("_std") != 0)
   {
      strLocale = Application.directrix()->m_varTopicQuery["locale"];
   }

   if (Application.directrix()->m_varTopicQuery["schema"].has_char() && Application.directrix()->m_varTopicQuery["schema"].get_string().CompareNoCase("_std") != 0)
   {
      strSchema = Application.directrix()->m_varTopicQuery["schema"];
   }


   localeschema.m_idLocale = strLocale;
   localeschema.m_idSchema = strSchema;


   localeschema.add_locale_variant(strLocale, strSchema);
   localeschema.add_locale_variant(get_locale(), strSchema);
   localeschema.add_locale_variant(__id(std), strSchema);
   localeschema.add_locale_variant(__id(en), strSchema);


   localeschema.finalize();


}



bool base_application::initialize()
{

   application_signal_details signal(this, m_psignal, application_signal_initialize);
   m_psignal->emit(&signal);
   if (!signal.m_bOk)
      return false;

   //m_pcalculator = new ::calculator::calculator(this);

   //m_pcalculator->construct(this);

   //if (!m_pcalculator->initialize())
   //   return false;


   //m_pcolorertake5 = new ::colorertake5::colorertake5(this);

   //m_pcolorertake5->construct(this);

   //if (!m_pcolorertake5->initialize())
   //   return false;

   m_dwAlive = ::get_tick_count();

   if (is_system())
   {
      if (guideline()->m_varTopicQuery.propset().has_property("save_processing"))
      {
         System.savings().save(::core::resource_processing);
      }
      if (guideline()->m_varTopicQuery.propset().has_property("save_blur_back"))
      {
         System.savings().save(::core::resource_blur_background);
      }
      if (guideline()->m_varTopicQuery.propset().has_property("save_transparent_back"))
      {
         System.savings().save(::core::resource_translucent_background);
      }
   }

   if (directrix()->m_varTopicQuery.propset().has_property("install"))
   {
      // core level app install
      if (!Ex2OnAppInstall())
         return false;
   }
   else if (directrix()->m_varTopicQuery.propset().has_property("uninstall"))
   {
      // core level app uninstall
      if (!Ex2OnAppUninstall())
         return false;
   }
   else
   {
#ifdef WINDOWSEX
      // when this process is started in the context of a system account,
      // for example, this code ensure that the process will
      // impersonate a loggen on ::fontopus::user
      HANDLE hprocess;
      HANDLE htoken;

      hprocess = ::GetCurrentProcess();

      if (!OpenProcessToken(
         hprocess,
         TOKEN_ALL_ACCESS,
         &htoken))
         return false;

      if (!ImpersonateLoggedOnUser(htoken))
      {
         TRACELASTERROR();
         return false;
      }
#endif
   }


   m_dwAlive = ::get_tick_count();

   if (is_session())
   {

      Session.m_spcopydesk.create(allocer());

      if (!Session.m_spcopydesk->initialize())
         return false;

   }

   if (is_system()
      && command_thread()->m_varTopicQuery["app"] != "core_netnodelite"
      && command_thread()->m_varTopicQuery["app"] != "app-core/netnode_dynamic_web_server"
      && command_thread()->m_varTopicQuery["app"] != "app-gtech/alarm"
      && command_thread()->m_varTopicQuery["app"] != "app-gtech/sensible_service")
   {
      System.http().defer_auto_initialize_proxy_configuration();
   }




   //      if(!::cubebase::application::initialize())
   //       return false;


   //      m_puserbase = new ::user::user();

   //    m_puserbase->construct(this);

   //  if(!m_puserbase->initialize())
   //return false;

   //m_pfilemanager = canew(::filemanager::filemanager(this));

   //application::m_pfilemanager = m_pfilemanager;

   //m_pfilemanager->construct(this);

   //if (!m_pfilemanager->initialize())
   //   return false;

   //m_pusermail = canew(::usermail::usermail(this));

   //m_pusermail->construct(this);

   //if (!m_pusermail->initialize())
   //   return false;

   //m_dwAlive = ::get_tick_count();





   m_dwAlive = ::get_tick_count();

   //m_splicense(new class ::fontopus::license(this));


   //if (!is_system())
   //{
   //   System.register_bergedge_application(this);
   //}


   //m_dwAlive = ::get_tick_count();

   //ensure_app_interest();

   //application_signal_details signal(this, m_psignal, application_signal_initialize);
   //m_psignal->emit(&signal);
   //if (!signal.m_bOk)
   //   return false;
   //return true;


   if (!is_installing() && !is_uninstalling() && !is_system() && m_pbasesession != NULL)
   {

      if (!user()->keyboard().initialize())
         return false;

   }


   if (m_bIfs)
   {
      if (m_pbasesession != NULL && &Session != NULL)
      {
         if (m_spfsdata.is_null())
            m_spfsdata = new ::fs::set(this);
         ::fs::set * pset = dynamic_cast <::fs::set *> ((class ::fs::data *) m_spfsdata);
         pset->m_spafsdata.add(Session.m_pifs);
         pset->m_spafsdata.add(Session.m_prfs);
         stringa stra;
         pset->root_ones(stra);
      }
   }

   m_dwAlive = ::get_tick_count();

   /*      if (!is_system())
         {
         System.register_bergedge_application(this);
         }*/



   //if (!m_spuserfs->initialize())
   //   return false;


   //if (!m_simpledb.initialize())
   //   return false;

   if (!m_spuser->initialize())
      return false;

   //if (!m_spuserex->initialize())
   //   return false;

   //if (!m_phtml->initialize())
   //   return false;

   //if (!is_system() && !is_session() && !is_installing() && !is_uninstalling())
   //{

   //   string str;
   //   // if system locale has changed (compared to last recorded one by core)
   //   // use the system locale
   //   if (data_get("system_locale", str))
   //   {
   //      if (str.has_char())
   //      {
   //         if (str != get_locale())
   //         {
   //            try
   //            {
   //               data_set("system_locale", get_locale());
   //               data_set("locale", get_locale());
   //            }
   //            catch (...)
   //            {
   //            }
   //         }
   //      }
   //   }
   //   else
   //   {
   //      data_set("system_locale", get_locale());
   //   }

   //   if (command()->m_varTopicQuery["locale"].get_string().has_char())
   //   {
   //      str = command()->m_varTopicQuery["locale"];
   //      data_set("system_locale", str);
   //      data_set("locale", str);
   //      set_locale(str, false);
   //   }
   //   else if (command()->m_varTopicQuery["lang"].get_string().has_char())
   //   {
   //      str = command()->m_varTopicQuery["lang"];
   //      data_set("system_locale", str);
   //      data_set("locale", str);
   //      set_locale(str, false);
   //   }
   //   else if (data_get("locale", str))
   //   {
   //      if (str.has_char())
   //      {
   //         set_locale(str, false);
   //      }
   //   }
   //   // if system schema has changed (compared to last recorded one by core)
   //   // use the system schema
   //   if (data_get("system_schema", str))
   //   {
   //      if (str.has_char())
   //      {
   //         if (str != get_schema())
   //         {
   //            try
   //            {
   //               data_set("system_schema", get_schema());
   //               data_set("schema", get_schema());
   //            }
   //            catch (...)
   //            {
   //            }
   //         }
   //      }
   //   }
   //   else
   //   {
   //      data_set("system_schema", get_schema());
   //   }

   //   if (command()->m_varTopicQuery["schema"].get_string().has_char())
   //   {
   //      str = command()->m_varTopicQuery["schema"];
   //      data_set("system_schema", str);
   //      data_set("schema", str);
   //      set_schema(str, false);
   //   }
   //   else if (data_get("schema", str))
   //   {
   //      if (str.has_char())
   //      {
   //         set_schema(str, false);
   //      }
   //   }

   //   // keyboard layout
   //   if (data_get("keyboard_layout", str) && str.has_char())
   //   {
   //      user()->set_keyboard_layout(str, false);
   //   }
   //   else
   //   {
   //      user()->set_keyboard_layout(NULL, false);
   //   }

   //   data_pulse_change("ca2", "savings", NULL);


   //   App(this).fill_locale_schema(*str_context()->m_plocaleschema);


   //   Sys(this).appa_load_string_table();

   //}


   return true;

}

bool base_application::process_initialize()
{

   if (m_bBaseProcessInitialize)
      return true;

   m_bBaseProcessInitialize = true;

   if (is_installing() || is_uninstalling())
   {

      if (is_system())
      {

         
         System.install().trace().initialize();
      }

   }


   /*      if (::thread::m_p.is_null())
         {

         ::thread::m_p.create(allocer());

         ::thread::m_p->m_p = this;

         }

         if (m_pimpl.is_null())
         {

         m_pimpl.create(allocer());

         m_pimpl->m_pimpl = this;

         }*/

   if (is_system())
   {
      System.factory().cloneable_large < stringa >();
      System.factory().cloneable_large < ::primitive::memory >();
      System.factory().cloneable_large < int_array >();
      //System.factory().cloneable_large < property > ();
   }

   m_pframea = new ::user::interaction_ptr_array(this);

   if(is_system())
   {

      Ex1OnFactoryExchange();

   }

   thread::s_bAllocReady = true;

   if (thread::m_p == NULL)
   {

      thread::m_p.create(allocer());
      thread::m_p->m_p = this;

   }

   m_pimpl.create(allocer());
   m_pimpl->construct();
   m_pimpl->m_pimpl = this;

   if (::get_thread() == NULL)
   {
      set_thread(dynamic_cast <thread *> (this));
   }

   if (!update_module_paths())
      return false;

   m_spfs = canew(::fs::fs(this));

   if (m_spfs == NULL)
      return false;

   m_spfs->construct(this);


   if (!m_spfs->initialize())
      return false;

   if (!ca_process_initialize())
      return false;

   if (is_system())
   {
      draw2d_factory_exchange();
   }

   if (!m_pimpl->process_initialize())
      return false;




   m_pfontopus = create_fontopus();

   if (m_pfontopus == NULL)
      throw simple_exception(this, "could not create fontopus for base_application (base_application::construct)");

   m_pfontopus->construct(this);

   m_spuser = create_user();

   if (m_spuser == NULL)
      return false;

   m_spuser->construct(this);

   //m_spuserex = create_userex();

   //if (m_spuserex == NULL)
   //   return false;

   //m_spuserex->construct(this);

   //m_spuserfs = create_userfs();

   //if (m_spuserfs == NULL)
   //   return false;

   //m_spuserfs->construct(this);

   //m_phtml = create_html();

   //if (m_phtml == NULL)
   //   return false;

   //m_phtml->construct(this);

   m_psockets = canew(::sockets::sockets(this));

   m_psockets->construct(this);

   if (!m_psockets->initialize1())
      throw simple_exception(this, "could not initialize (1) sockets for base_application (base_application::construct)");


   if (!m_psockets->initialize())
      throw simple_exception(this, "could not initialize sockets for base_application (base_application::construct)");


   return true;

}

bool base_application::initialize1()
{



   m_splicense = new class ::fontopus::license(this);



   if (!is_system())
   {
      if (m_spfsdata.is_null())
         m_spfsdata = new ::fs::set(this);
      ::fs::set * pset = dynamic_cast <::fs::set *> ((class ::fs::data *) m_spfsdata);
      pset->m_spafsdata.add(new ::fs::native(this));
      stringa stra;
      pset->root_ones(stra);
   }


   if (fontopus()->m_puser == NULL &&
      (Application.directrix()->m_varTopicQuery.has_property("install")
      || Application.directrix()->m_varTopicQuery.has_property("uninstall")))
   {

      if (fontopus()->create_system_user("system") == NULL)
         return false;

   }


   //m_puinteraction = canew(::uinteraction::uinteraction(this));

   //m_puinteraction->construct(this);

   //if (!m_puinteraction->initialize())
   //   return false;

   /*
   if(is_system())
   {
   m_strFontopusServer     = System.get_fontopus_server("http://account.ca2.cc/get_fontopus", this, 8);
   if(m_strFontopusServer.is_empty())
   m_strFontopusServer = "server.ca2.cc";
   else
   {
   m_strFontopusServer.replace("account", "server");
   }
   m_strMatterUrl          = "http://" + m_strFontopusServer + "/matter/";
   m_strMatterSecureUrl    = "https://" + m_strFontopusServer + "/matter/";
   }
   else
   {
   m_strFontopusServer     = System.m_strFontopusServer;
   m_strMatterUrl          = System.m_strMatterUrl;
   m_strMatterSecureUrl    = System.m_strMatterSecureUrl;
   }
   */


   m_dwAlive = ::get_tick_count();


   m_strMatterLocator = System.dir().appmatter_locator(this);


   m_puserstrcontext = canew(::user::str_context(this));
   if (m_puserstrcontext == NULL)
      return false;


   if (!ca_initialize1())
      return false;

   string strLocaleSystem;

   string strSchemaSystem;

   string strPath = System.dir().appdata("langstyle_settings.xml");

   if (Application.file().exists(strPath))
   {

      string strSystem = Application.file().as_string(strPath);

      ::xml::document docSystem(get_app());

      if (docSystem.load(strSystem))
      {

         if (docSystem.get_child("lang") != NULL)
         {

            strLocaleSystem = docSystem.get_child("lang")->get_value();

         }

         if (docSystem.get_child("style") != NULL)
         {

            strSchemaSystem = docSystem.get_child("style")->get_value();

         }

      }

   }



   string strLocale;

   string strSchema;

#ifdef METROWIN

   stringa stra;

   try
   {

      stra.explode("-", ::Windows::Globalization::ApplicationLanguages::PrimaryLanguageOverride);

   }
   catch (long)
   {


   }

   strLocale = stra[0];

   strSchema = stra[0];

#elif defined(WINDOWS)
   LANGID langid = ::GetUserDefaultLangID();
#define SPR_DEUTSCH LANG_GERMAN
   if (langid == LANG_SWEDISH)
   {
      strLocale = "se";
      strSchema = "se";
   }
   else if (langid == MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN))
   {
      strLocale = "pt-br";
      strSchema = "pt-br";
   }
   else if (PRIMARYLANGID(langid) == SPR_DEUTSCH)
   {
      strLocale = "de";
      strSchema = "de";
   }
   else if (PRIMARYLANGID(langid) == LANG_ENGLISH)
   {
      strLocale = "en";
      strSchema = "en";
   }
   else if (PRIMARYLANGID(langid) == LANG_JAPANESE)
   {
      strLocale = "jp";
      strSchema = "jp";
   }
   else if (PRIMARYLANGID(langid) == LANG_POLISH)
   {
      strLocale = "pl";
      strSchema = "pl";
   }
#endif

   if (strLocale.is_empty())
      strLocale = "se";

   if (strSchema.is_empty())
      strSchema = "se";

   if (strLocaleSystem.has_char())
      strLocale = strLocaleSystem;

   if (strSchemaSystem.has_char())
      strSchema = strSchemaSystem;

   if (Sys(this).directrix()->m_varTopicQuery["locale"].get_string().has_char())
      strLocale = Sys(this).directrix()->m_varTopicQuery["locale"];

   if (Sys(this).directrix()->m_varTopicQuery["schema"].get_string().has_char())
      strSchema = Sys(this).directrix()->m_varTopicQuery["schema"];

   if (App(this).directrix()->m_varTopicQuery["locale"].get_string().has_char())
      strLocale = App(this).directrix()->m_varTopicQuery["locale"];

   if (App(this).directrix()->m_varTopicQuery["schema"].get_string().has_char())
      strSchema = App(this).directrix()->m_varTopicQuery["schema"];



   set_locale(strLocale, false);
   set_schema(strSchema, false);


   str_context()->localeschema().m_idaLocale.add(strLocale);
   str_context()->localeschema().m_idaSchema.add(strSchema);

   //Sleep(15 * 1000);



   if (!m_pimpl->initialize1())
      return false;


   if (!m_spuser->initialize1())
      return false;

   if (!m_spuser->initialize2())
      return false;

   //if (!m_spuserex->initialize1())
   //   return false;
   //if (!m_spuserex->initialize2())
   //   return false;

   //m_simpledb.construct(this);

   //if (!m_simpledb.initialize2())
   //   return false;


   return true;

}


bool base_application::initialize2()
{

   if (!m_pimpl->initialize2())
      return false;

   if (!ca_initialize2())
      return false;

   fill_locale_schema(*str_context()->m_plocaleschema);

   return true;

}


bool base_application::initialize3()
{

   if (!m_pimpl->initialize3())
      return false;

   if (!ca_initialize3())
      return false;

   return true;

}


bool base_application::initialize_instance()
{

   if (m_bBaseInitializeInstance)
      return true;

   m_bBaseInitializeInstance = true;

   if (!is_system())
   {
      if (!check_exclusive())
         return false;
   }

   m_dwAlive = ::get_tick_count();


   if (!initialize1())
      return false;

   string strWindow;
   if (m_strAppName.has_char())
      strWindow = m_strAppName;
   else
      strWindow = typeid(*this).name();

#ifndef METROWIN

   if (!create_message_queue(this, strWindow))
   {
      TRACE("Fatal error: could not initialize application message window (name=\"%s\").", strWindow);
      return false;
   }

#endif

   m_dwAlive = ::get_tick_count();

   if (!initialize2())
      return false;

   m_dwAlive = ::get_tick_count();

   if (!initialize3())
      return false;

   m_dwAlive = ::get_tick_count();

   try
   {
      if (!initialize())
         return false;
   }
   catch (const char * psz)
   {
      if (!strcmp(psz, "You have not logged in! Exiting!"))
      {
         return false;
      }
      return false;
   }

   if (!m_pfontopus->initialize_instance())
      return false;

   return true;


}


int32_t base_application::exit_instance()
{

   try
   {

      try
      {
         if (is_session())
         {
            if (Session.m_spcopydesk.is_set())
            {
               Session.m_spcopydesk->finalize();
               Session.m_spcopydesk.release();
            }
            Session.m_splicense.release();
         }
      }
      catch (...)
      {
      }


      /*      try
      {
      if(m_plemonarray != NULL)
      {
      delete m_plemonarray;
      }
      }
      catch(...)
      {
      }
      m_plemonarray = NULL;
      */


      try
      {
         if (m_pmath != NULL)
         {
            delete m_pmath;
         }
      }
      catch (...)
      {
      }
      m_pmath = NULL;



      try
      {
         if (m_pgeometry != NULL)
         {
            delete m_pgeometry;
         }
      }
      catch (...)
      {
      }
      m_pgeometry = NULL;



      try
      {
         if (m_psavings != NULL)
         {
            delete m_psavings;
         }
      }
      catch (...)
      {
      }
      m_psavings = NULL;


      m_pcommandthread.release();

      release_exclusive();

      if (!destroy_message_queue())
      {
         TRACE("Could not finalize message window");
      }

      application_signal_details signal(this, m_psignal, application_signal_exit_instance);
      try
      {
         m_psignal->emit(&signal);
      }
      catch (...)
      {
      }

      //try
      //{
      //   if (!is_system())
      //   {
      //      System.unregister_bergedge_application(this);
      //   }
      //}
      //catch (...)
      //{
      //}

      /*try
      {
      ::release(smart_pointer <thread>::m_p);
      }
      catch(...)
      {
      }*/


      if (is_system())
      {

         //         try
         //       {
         //        if(m_spfilesystem.m_p != NULL)
         //      {
         //       ::core::del(m_spfilesystem.m_p);
         //  }
         //         }
         //       catch(...)
         //     {
         //   }
      }



      try
      {

         thread         * pthread = thread::m_p.detach();

         if (pthread != NULL)
         {

            try
            {
               // avoid calling CloseHandle() on our own thread handle
               // during the thread destructor
               // avoid thread object data auto deletion on thread termination,
               // letting thread function terminate
               pthread->m_bAutoDelete = false;

               pthread->set_os_data(NULL);

               pthread->set_run(false);

            }
            catch (...)
            {

            }

         }

      }
      catch (...)
      {

      }

      try
      {

         ::base_application   * papp = m_pimpl.detach();

         if (papp != NULL && papp != this && !papp->is_system())
         {

            try
            {

               papp->exit_instance();

            }
            catch (...)
            {

            }

         }

      }
      catch (...)
      {

      }

      return 0;
      /*      int32_t nReturnValue=0;
      if(__get_current_message())
      {
      nReturnValue=static_cast<int32_t>(__get_current_message()->wParam);
      }*/
      //      return nReturnValue; // returns the value from PostQuitMessage


   }
   catch (...)
   {

      m_iReturnCode = -1;

   }

   try
   {

      if (System.appptra().get_count() <= 1)
      {

         if (System.thread::get_os_data() != NULL)
         {
            System.post_thread_message(WM_QUIT);

         }

      }

   }
   catch (...)
   {

      m_iReturnCode = -1;

   }

   return m_iReturnCode;

}









bool base_application::is_running()
{
   return is_alive();
}

service_base * base_application::allocate_new_service()
{

   return NULL;

}







bool base_application::ca_initialize2()
{

   application_signal_details signal(this, m_psignal, application_signal_initialize2);
   m_psignal->emit(&signal);
   return signal.m_bOk;

}


bool base_application::ca_initialize3()
{

   application_signal_details signal(this, m_psignal, application_signal_initialize3);
   m_psignal->emit(&signal);
   if (!signal.m_bOk)
      return false;

   return true;

}










bool base_application::check_exclusive()
{

#ifdef METROWIN

   return true;

#endif

   bool bSetOk;

   LPSECURITY_ATTRIBUTES lpsa = NULL;

   bool bResourceException = false;

#ifdef WINDOWSEX

   bSetOk = false;

   SECURITY_ATTRIBUTES MutexAttributes;
   ZeroMemory(&MutexAttributes, sizeof(MutexAttributes));
   MutexAttributes.nLength = sizeof(MutexAttributes);
   MutexAttributes.bInheritHandle = FALSE; // object uninheritable
   // declare and initialize a security descriptor
   SECURITY_DESCRIPTOR SD;
   bool bInitOk = InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION) != FALSE;
   if (bInitOk)
   {
      // give the security descriptor a Null Dacl
      // done using the  "TRUE, (PACL)NULL" here
      bSetOk = SetSecurityDescriptorDacl(&SD,
         TRUE,
         (PACL)NULL,
         FALSE) != FALSE;
   }

   if (bSetOk)
   {

      MutexAttributes.lpSecurityDescriptor = &SD;

      lpsa = &MutexAttributes;

   }

#else

   bSetOk = true;

#endif

   if (bSetOk)
   {
      // Make the security attributes point
      // to the security descriptor
      bResourceException = false;
      try
      {
         m_pmutexGlobal = new mutex(this, FALSE, get_global_mutex_name(), lpsa);
      }
      catch (resource_exception &)
      {
         try
         {
            m_pmutexGlobal = new mutex(this, FALSE, get_global_mutex_name());
         }
         catch (resource_exception &)
         {
            bResourceException = true;
         }
      }

      if (m_eexclusiveinstance == ExclusiveInstanceGlobal
         && (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException))
      {
         // Should in some way activate the other instance, but this is global, what to do? do not know yet.
         //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine.<br><br>Exiting this new instance.");
         TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine.<br><br>Exiting this new instance.");
         on_exclusive_instance_conflict(ExclusiveInstanceGlobal);
         return false;
      }
      if (m_eexclusiveinstance == ExclusiveInstanceGlobalId)
      {
         bResourceException = false;
         try
         {
            m_pmutexGlobalId = new mutex(this, FALSE, get_global_id_mutex_name(), lpsa);
         }
         catch (resource_exception &)
         {
            try
            {
               m_pmutexGlobalId = new mutex(this, FALSE, get_global_id_mutex_name());
            }
            catch (resource_exception &)
            {
               bResourceException = true;
            }
         }
         if (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException)
         {
            // Should in some way activate the other instance
            //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
            TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "with the id \"" + get_local_mutex_id() + "\" <br><br>seems to be already running at the same machine<br>Only one instance of this application can run globally: at the same machine with the same id.<br><br>Exiting this new instance.");
            on_exclusive_instance_conflict(ExclusiveInstanceGlobalId);
            return false;
         }
      }
      bResourceException = false;
      try
      {
         m_pmutexLocal = new mutex(this, FALSE, get_local_mutex_name(), lpsa);
      }
      catch (resource_exception &)
      {
         try
         {
            m_pmutexLocal = new mutex(this, FALSE, get_local_mutex_name());
         }
         catch (resource_exception &)
         {
            bResourceException = true;
         }
      }
      if (m_eexclusiveinstance == ExclusiveInstanceLocal && (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException))
      {
         // Should in some way activate the other instance
         //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
         TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
         on_exclusive_instance_conflict(ExclusiveInstanceLocal);
         return false;
      }
      if (m_eexclusiveinstance == ExclusiveInstanceLocalId)
      {
         bResourceException = false;
         try
         {
            m_pmutexLocalId = new mutex(this, FALSE, get_local_id_mutex_name(), lpsa);
         }
         catch (resource_exception &)
         {
            try
            {
               m_pmutexLocalId = new mutex(this, FALSE, get_local_id_mutex_name());
            }
            catch (resource_exception &)
            {
               bResourceException = true;
            }
         }
         if (::GetLastError() == ERROR_ALREADY_EXISTS || bResourceException)
         {
            // Should in some way activate the other instance
            //System.simple_message_box("A instance of the application:<br><br>           - " + string(m_strAppName) + "<br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same account.<br><br>Exiting this new instance.");
            TRACE("A instance of the application:<br><br>           - " + string(m_strAppName) + "with the id \"" + get_local_mutex_id() + "\" <br><br>seems to be already running at the same account.<br>Only one instance of this application can run locally: at the same ac::count with the same id.<br><br>Exiting this new instance.");
            on_exclusive_instance_conflict(ExclusiveInstanceLocalId);
            return false;
         }
      }
   }
   else
   {
      return false;
   }

   return true;

}

bool base_application::release_exclusive()
{
   if (m_pmutexGlobal.is_set())
   {
      m_pmutexGlobal.release();
   }
   if (m_pmutexGlobalId.is_set())
   {
      m_pmutexGlobalId.release();
   }
   if (m_pmutexLocal.is_set())
   {
      m_pmutexLocal.release();
   }
   if (m_pmutexLocalId.is_set())
   {
      m_pmutexLocalId.release();
   }
   return true;
}



bool base_application::ca_process_initialize()
{
   application_signal_details signal(this, m_psignal, application_signal_process_initialize);
   m_psignal->emit(&signal);
   return true;
}

bool base_application::ca_initialize1()
{
   application_signal_details signal(this, m_psignal, application_signal_initialize1);
   m_psignal->emit(&signal);
   return signal.m_bOk;
}



bool base_application::ca_finalize()
{
   application_signal_details signal(this, m_psignal, application_signal_finalize);
   try
   {
      m_psignal->emit(&signal);
   }
   catch (...)
   {
   }




   return signal.m_bOk;
}



string base_application::get_local_mutex_name(const char * pszAppName)
{
   string strMutex;
   strMutex.Format("Local\\ca2_application_local_mutex:%s", pszAppName);
   return strMutex;
}

string base_application::get_local_id_mutex_name(const char * pszAppName, const char * pszId)
{
   string strId(pszId);
   string strMutex;
   strMutex.Format("Local\\ca2_application_local_mutex:%s, id:%s", pszAppName, strId);
   return strMutex;
}

string base_application::get_global_mutex_name(const char * pszAppName)
{
   string strMutex;
   strMutex.Format("Global\\ca2_application_global_mutex:%s", pszAppName);
   return strMutex;
}

string base_application::get_global_id_mutex_name(const char * pszAppName, const char * pszId)
{
   string strId(pszId);
   string strMutex;
   strMutex.Format("Global\\ca2_application_global_mutex:%s, id:%s", pszAppName, strId);
   return strMutex;
}

string base_application::get_local_mutex_name()
{
   return get_local_mutex_name(get_mutex_name_gen());
}

string base_application::get_local_id_mutex_name()
{
   return get_local_id_mutex_name(get_mutex_name_gen(), get_local_mutex_id());
}

string base_application::get_global_mutex_name()
{
   return get_global_mutex_name(get_mutex_name_gen());
}

string base_application::get_global_id_mutex_name()
{
   return get_global_id_mutex_name(get_mutex_name_gen(), get_global_mutex_id());
}




void base_application::on_exclusive_instance_conflict(EExclusiveInstance eexclusive)
{
   if (eexclusive == ExclusiveInstanceLocal)
   {
      on_exclusive_instance_local_conflict();
   }
}

void base_application::on_exclusive_instance_local_conflict()
{
}


string base_application::get_mutex_name_gen()
{
   return m_strAppName;
}

string base_application::get_local_mutex_id()
{
   return command()->m_varTopicQuery["local_mutex_id"];
}

string base_application::get_global_mutex_id()
{
   return command()->m_varTopicQuery["global_mutex_id"];
}

::mutex * base_application::get_local_mutex()
{
   return m_pmutexLocal;
}

::mutex * base_application::get_global_mutex()
{
   return m_pmutexGlobal;
}



bool base_application::Ex2OnAppInstall()
{
   return true;
}

bool base_application::Ex2OnAppUninstall()
{
   return true;
}




void base_application::draw2d_factory_exchange()
{

   string strLibrary = draw2d_get_default_library_name();

   if (strLibrary.is_empty())
      strLibrary = "draw2d_cairo";

   base_library & library = System.m_libraryDraw2d;

   if (library.is_opened())
      return;

   if (!library.open(strLibrary))
   {
      if (strLibrary != "draw2d_cairo")
      {
         if (!library.open("draw2d_cairo"))
         {
            throw "failed to do draw2d factory exchange";
         }
      }
      else
      {
         throw "failed to do draw2d factory exchange";
      }
   }


   PFN_ca2_factory_exchange pfn_ca2_factory_exchange = library.get < PFN_ca2_factory_exchange >("ca2_factory_exchange");

   pfn_ca2_factory_exchange(this);

}



bool base_application::update_appmatter(::sockets::socket_handler & h, ::sockets::http_session * & psession, const char * pszRoot, const char * pszRelative)
{

   ::str::international::locale_schema localeschema(this);

   fill_locale_schema(localeschema);

   bool bIgnoreStdStd = string(pszRoot) == "app" && (string(pszRelative) == "main" || string(pszRelative) == "bergedge");

   //update_appmatter(h, psession, pszRoot, pszRelative, localeschema.m_idLocale, localeschema.m_idSchema);

   for (int32_t i = 0; i < localeschema.m_idaLocale.get_count(); i++)
   {
      if (localeschema.m_idaLocale[i] == __id(std) && localeschema.m_idaSchema[i] == __id(std) && bIgnoreStdStd)
         continue;
      update_appmatter(h, psession, pszRoot, pszRelative, localeschema.m_idaLocale[i], localeschema.m_idaSchema[i]);
      System.install().m_progressApp()++;
   }


   return true;

}

bool base_application::update_appmatter(::sockets::socket_handler & h, ::sockets::http_session * & psession, const char * pszRoot, const char * pszRelative, const char * pszLocale, const char * pszStyle)
{

   string strLocale;
   string strSchema;
   TRACE("update_appmatter(root=%s, relative=%s, locale=%s, style=%s)", pszRoot, pszRelative, pszLocale, pszStyle);
   string strRelative = System.dir().path(System.dir().path(pszRoot, "appmatter", pszRelative), App(this).get_locale_schema_dir(pszLocale, pszStyle)) + ".zip";
   string strFile = System.dir().element(strRelative);
   string strUrl;
   if (_ca_is_basis())
   {
      strUrl = "http://basis.spaignition.api.server.ca2.cc/download?authnone&version=basis&stage=";
   }
   else
   {
      strUrl = "http://stage.spaignition.api.server.ca2.cc/download?authnone&version=stage&stage=";
   }

   strUrl += System.url().url_encode(strRelative);


   if (psession == NULL)
   {

      while (true)
      {

         property_set setEmpty(get_app());

         psession = System.http().open(h, System.url().get_server(strUrl), System.url().get_protocol(strUrl), setEmpty, NULL, NULL);

         if (psession != NULL)
            break;

         Sleep(184);

      }

   }

   property_set set;

   set["get_memory"] = "";

   psession = System.http().request(h, psession, strUrl, set);

   ::file::memory_buffer file(get_app(), set["get_memory"].cast < primitive::memory >());

   if (set["get_memory"].cast < primitive::memory >() != NULL && set["get_memory"].cast < primitive::memory >()->get_size() > 0)
   {

      zip::Util util;

      string strDir = strFile;

      ::str::ends_eat_ci(strDir, ".zip");

      try
      {

         util.extract_all(strDir, &file);

      }
      catch (...)
      {

         // spa app-install.exe would recover by retrying or someone would fix the resource packaging problem and then zip extraction at least should work.

         return false;

      }

      //System.compress().extract_all(strFile, this);

   }

   return true;

}







void base_application::on_set_scalar(e_scalar escalar, int64_t iValue)
{

   if (escalar == scalar_download_size)
   {

//      m_content_ptr = iValue;

   }
   else
   {

      return ::int_scalar_source::on_set_scalar(escalar, iValue);

   }

}


void base_application::get_scalar_minimum(e_scalar escalar, int64_t & i)
{

   if (escalar == scalar_download_size)
   {

      i = 0;

   }
   else
   {

      ::int_scalar_source::get_scalar_minimum(escalar, i);

   }

}

void base_application::get_scalar(e_scalar escalar, int64_t & i)
{

   if (escalar == scalar_download_size)
   {

      i = 0;

   }
   else
   {

      ::int_scalar_source::get_scalar(escalar, i);

   }

}

void base_application::get_scalar_maximum(e_scalar escalar, int64_t & i)
{

   if (escalar == scalar_download_size)
   {

      i = 0;

   }
   else
   {

      ::int_scalar_source::get_scalar_minimum(escalar, i);

   }

}


int32_t base_application::simple_message_box_timeout(sp(::user::interaction) pwndOwner, const char * pszMessage, ::duration durationTimeOut, UINT fuStyle)
{
   UNREFERENCED_PARAMETER(durationTimeOut);
   return simple_message_box(pwndOwner, pszMessage, fuStyle);
}



service_base * base_application::get_service()
{
   
   return m_pservice;

}





void base_application::message_queue_message_handler(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   if (pbase->m_uiMessage == WM_TIMER)
   {
      SCAST_PTR(::message::timer, ptimer, pobj);
      if (ptimer->m_nIDEvent == 123)
      {
         m_spuiMessage->KillTimer(ptimer->m_nIDEvent);
         frames().send_message_to_descendants(::base_application::APPM_LANGUAGE);
         System.appa_load_string_table();
      }
   }
}
