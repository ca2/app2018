#include "framework.h"




file_size_table::get_fs_size::get_fs_size()
{
}

file_size_table::get_fs_size::get_fs_size(const get_fs_size & getfssize)
{
   operator = (getfssize);
}

file_size_table::get_fs_size & file_size_table::get_fs_size::operator = (const get_fs_size & getfssize)
{
   if(this != & getfssize)
   {
      m_strPath         = getfssize.m_strPath;
      m_bPending        = getfssize.m_bPending;
      m_iSize           = getfssize.m_iSize;
      m_bRet            = getfssize.m_bRet;
      m_oswindow            = getfssize.m_oswindow;

   }
   return *this;
}


file_size_table::file_size_table(::aura::application * papp) :
   object(papp)
{
   m_hmap = NULL;
   m_item.m_pitemParent = NULL;
   m_pwndServer = NULL;
   m_oswindowServer = NULL;

   /*   SECURITY_ATTRIBUTES MutexAttributes;
      ZeroMemory( &MutexAttributes, sizeof(MutexAttributes) );
      MutexAttributes.nLength = sizeof( MutexAttributes );
      MutexAttributes.bInheritHandle = FALSE; // object uninheritable

      // declare and initialize a security descriptor
      SECURITY_DESCRIPTOR SD;
      bool bInitOk = InitializeSecurityDescriptor(
                        &SD,
                        SECURITY_DESCRIPTOR_REVISION );
      if ( bInitOk )
      {
         // give the security descriptor a Null Dacl
         // done using the  "TRUE, (PACL)NULL" here
         bool bSetOk = SetSecurityDescriptorDacl( &SD,
                                               TRUE,
                                               (PACL)NULL,
                                               FALSE );
         if ( bSetOk )
         {
            // Make the security attributes point
            // to the security descriptor
            MutexAttributes.lpSecurityDescriptor = &SD;*/
   //m_pmutex = new mutex(FALSE, "Global\\::draw2d::fontopus::file_system_size::7807e510-5579-11dd-ae16-0800200c7784", &MutexAttributes);
   //m_pevExec = new event(FALSE, FALSE, "Global\\::draw2d::fontopus::file_system_size::exec_event::7807e510-5579-11dd-ae16-0800200c7784", &MutexAttributes);
   //m_pevDone = new event(FALSE, FALSE, "Global\\::draw2d::fontopus::file_system_size::done_event::7807e510-5579-11dd-ae16-0800200c7784", &MutexAttributes);
   //m_pmutex = new mutex(FALSE, "Local\\::draw2d::fontopus::file_system_size::7807e510-5579-11dd-ae16-0800200c7784");
   //m_pevExec = new event(FALSE, FALSE, "Local\\::draw2d::fontopus::file_system_size::exec_event::7807e510-5579-11dd-ae16-0800200c7784");
   //m_pevDone = new event(FALSE, FALSE, "Local\\::draw2d::fontopus::file_system_size::done_event::7807e510-5579-11dd-ae16-0800200c7784");
   /*      }
      }*/
   m_pwnd  = new FileSystemSizeWnd(papp);
   m_pwnd->CreateClient();
}

file_size_table::~file_size_table()
{
}



bool file_size_table::check_map()
{
   return true;
}

file_size_table::item::item()
{
   m_iSize = 0;
   m_iStep = 0;
   m_bPending = false;
   m_bPendingLs = true;
   m_pitemParent = NULL;
}

void file_size_table::item::ls(::aura::application * papp, index & iIteration)
{

   if(m_bDir)
   {

      ::file::listing               listing(get_app());

      if(path().is_empty())
      {

         Sess(papp).dir().root_ones(listing);

         for(int32_t i = 0; i < listing.get_size(); i++)
         {
            item item;
            item.m_bPending = true;
            item.m_bDir = true;
            item.m_strName = listing[i];
            item.m_pitemParent = this;
            item.m_iStep = 0;
            m_itema.add(new file_size_table::item(item));
         }
      }
      else
      {

         listing.ls(path());

         for(int32_t i = 0; i < listing.get_size(); i++)
         {

            item item;
            item.m_bPending = listing[i].m_iDir == 1;
            item.m_bPendingLs = listing[i].m_iDir == 1;
            item.m_bDir = listing[i].m_iDir == 1;
            item.m_strName = listing[i].name();
            item.m_pitemParent = this;
            if(listing[i].m_iDir)
            {
               item.m_iStep = 0;
            }
            else
            {
               item.m_iSize = listing[i].m_iSize;
               item.m_iStep = -1;
            }
            m_itema.add(new file_size_table::item(item));
            iIteration++;
         }
      }
      m_bPendingLs = false;

   }
}

string file_size_table::item::path()
{
   if(m_pitemParent == NULL)
      return m_strName;
   else if(m_pitemParent->m_pitemParent == NULL)
      return m_strName;
   else
      return m_pitemParent->path() + "\\" + m_strName;
}

file_size_table::item * file_size_table::item::FindItem(::aura::application * papp, const char * pszPath, index & iIteration)
{
   string strName;
   string strPath(pszPath);
   strPath.replace('/', '\\');

   while(strPath.Left(1) == "\\")
      strPath = strPath.Mid(1);
   strsize iFind = strPath.find('\\');
   strsize iFindName;
   if(iFind < 0)
   {
      iFindName = FindName(papp, strPath, iIteration);
      if(iFindName < 0)
         return NULL;
      return m_itema[iFindName];
   }
   else
   {
      iFindName = FindName(papp, strPath.Left(iFind), iIteration);
      if(iFindName < 0)
         return NULL;
      return m_itema[iFindName]->FindItem(papp, strPath.Mid(iFind + 1), iIteration);

   }
}


index file_size_table::item::FindName(::aura::application * papp, const char * pszName, index & iIteration)
{
   if(m_bPendingLs)
   {
      ls(papp, iIteration);
   }
   for(index i = 0; i < m_itema.get_size(); i++)
   {
      if(m_itema[i]->m_strName == pszName)
         return i;
   }
   return -1;
}

void file_size_table::item::update_size(::aura::application * papp, index & iIteration)
{
   UNREFERENCED_PARAMETER(papp);
   UNREFERENCED_PARAMETER(iIteration);
   if(m_bDir)
   {
      m_iSize = 0;
      m_bPending = false;
      for(int32_t i = 0; i < m_itema.get_size(); i++)
      {
         m_iSize += m_itema[i]->m_iSize;
         if(m_itema[i]->m_bPending || m_itema[i]->m_bPendingLs)
            m_bPending = true;
      }
      if(m_pitemParent != NULL && m_bPending)
         m_pitemParent->m_bPending = true;
   }

}

void file_size_table::item::update_size_recursive(::aura::application * papp, index & iIteration)
{
   if(m_bPendingLs)
   {
      ls(papp, iIteration);
   }
   for(int32_t i = 0; i < m_itema.get_size(); i++)
   {
      m_itema[i]->update_size_recursive(papp, iIteration);
      if(iIteration > 230)
         break;
   }
   update_size(papp, iIteration);
}



DBFileSystemSizeSet::DBFileSystemSizeSet(::aura::application * papp) :
   object(papp), m_table(papp)
{
   m_iMaxIteration = 230;
}

DBFileSystemSizeSet::~DBFileSystemSizeSet()
{
}


bool DBFileSystemSizeSet::get_cache_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending)
{

   return false;

//   single_lock sl(m_table.m_pmutex, FALSE);
//  // Wait for mutex. Once it is obtained, no other client may
//  // communicate with the server
//   if(!sl.lock(duration::zero()))
//      return false;
//   if(!m_table.check_map())
//      return false;
//
//   m_table.m_pwnd->get_fs_size(i64Size, pszPath, bPending);
//   file_size_table::get_fs_size size;
//   FileSystemSizeWnd::size_map::pair * ppair = m_table.m_pwnd->m_map.PLookup(pszPath);
//   if(ppair != NULL)
//   {
//      i64Size     = ppair->m_element2.m_iSize;
//      bPending    = ppair->m_element2.m_bPending;
//      return true;
//   }
//   return  false;
//
//    // Fill shared memory
//    //::ZeroMemory(&m_pMsg, sizeof(TSharedMemory));
//    //m_pMsg->m_dwProcessID = m_dwProcessId;
///*    _tcscpy(m_table.m_pgetfssize->m_szPath, pszPath);
//
//    // Signal server to process this request
//    m_table.m_pevExec->SetEvent();
//    // Wait for server to finish
//    if (m_table.m_pevDone->lock(0))
//    {
//      // Server finished processing data, handle data returned by the Server
//       i64Size = m_table.m_pgetfssize->m_iSize;
//       bPending = m_table.m_pgetfssize->m_bPending;
//       return m_table.m_pgetfssize->m_bRet;
//    }
//    else
//    {
//      // Timeout waiting for response from server
//       return false;
//    }*/

}


bool DBFileSystemSizeSet::get_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending)
{
   index iIteration = 0;
   single_lock sl(m_table.m_pmutex, FALSE);
   if(!sl.lock(duration::zero()))
      return false;
   if(!get_fs_size(i64Size, pszPath, bPending, iIteration))
      return false;

   return true;
}

bool DBFileSystemSizeSet::get_fs_size(int64_t & i64Size,const ::file::path & pszPath,bool & bPending,index & iIteration)
{
   single_lock sl(m_table.m_pmutex, FALSE);
   if(!sl.lock(duration::zero()))
      return false;
   if(iIteration >= m_iMaxIteration)
   {
      bPending = true;
      return true;
   }

   string strTitle = pszPath.name();

   file_size_table::item * pitem = m_table.m_item.FindItem(get_app(), pszPath, iIteration);

   if(pitem != NULL)
   {
      bPending = pitem->m_bPending;
      i64Size = pitem->m_iSize;
      if(!bPending)
      {
         return true;
      }
   }
   else
   {
      return false;
   }
   pitem->update_size_recursive(m_papp, iIteration);
   bPending = pitem->m_bPending;
   i64Size = pitem->m_iSize;
   return true;
}

FileSystemSizeWnd::FileSystemSizeWnd(::aura::application * papp) :
   object(papp)
{
   m_pui = new ::user::interaction(papp);
}


void FileSystemSizeWnd::install_message_routing(::message::sender * pinterface)
{

   m_pui->install_message_routing(pinterface);

   IGUI_MSG_LINK(WM_COPYDATA, pinterface, this, &FileSystemSizeWnd::_001OnCopyData);

}


bool FileSystemSizeWnd::CreateClient()
{

//#ifdef WINDOWS

   m_bServer = false;
   return m_pui->create_message_queue("::draw2d::fontopus::FileSystemSizeWnd::Client");
   /*  sp(::user::interaction) puiMessage = NULL;
      puiMessage = System.ui_from_handle(HWND_MESSAGE);
      return m_pui->create(NULL, "::draw2d::fontopus::FileSystemSizeWnd::Client", 0, rect(0, 0, 0, 0), puiMessage, id()) != FALSE;*/

//#else

   // ::exception::throw_not_implemented(get_app());

//#endif

}

bool FileSystemSizeWnd::CreateServer()
{

#ifdef WINDOWS

   m_bServer = true;
   if(!m_pui->create_window(NULL,"Local\\::draw2d::fontopus::FileSystemSizeWnd::Server",0,null_rect(),System.ui_from_handle(HWND_MESSAGE),id()))
      return false;
   m_pui->SetTimer(100, 100, NULL);
   return true;

#else

   ::exception::throw_not_implemented(get_app());

   return false;

#endif

}

bool FileSystemSizeWnd::get_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending)
{

#ifdef WINDOWSEX

   db_server * pcentral = dynamic_cast < db_server * > (&System.m_psimpledb->db());
   oswindow oswindow = pcentral->m_pfilesystemsizeset->m_table.m_oswindowServer;
   if(oswindow == NULL || ! ::IsWindow(oswindow))
   {
      if(pcentral->m_pfilesystemsizeset->m_table.m_oswindowServer == NULL)
      {
         ClientStartServer();
      }
      return false;
   }
   file_size_table::get_fs_size size;
   size.m_strPath = pszPath;
   size.m_bRet = false;


   memory_file file(get_app());

   {

      writer writer(&file);

      writer(size);

   }

   COPYDATASTRUCT data;
   data.dwData = 0;
   data.cbData = (uint32_t)file.get_length();
   data.lpData = file.get_data();
   ::oswindow oswindowWparam = (::oswindow) m_pui->get_os_data();
   WPARAM wparam = (WPARAM) oswindowWparam;
   if(::SendMessage(oswindow, WM_COPYDATA, wparam, (LPARAM) &data))
   {
      i64Size = m_size.m_iSize;
      bPending = m_size.m_bPending;

      return m_bRet;
   }
   else
   {
      return false;
   }

#else

   ::exception::throw_not_implemented(get_app());

   return false;

#endif

}


void FileSystemSizeWnd::_001OnCopyData(::message::message * pobj)
{

#ifdef WINDOWSEX

   SCAST_PTR(::message::base, pbase, pobj);

   COPYDATASTRUCT * pstruct = (COPYDATASTRUCT *) pbase->m_lparam.m_lparam;
   if(pstruct->dwData == 0)
   {
      //file_size_table::get_fs_size * prec  = (file_size_table::get_fs_size *) pstruct->lpData;
      db_server * pcentral = &System.m_psimpledb->db();
      file_size_table::get_fs_size size;
      memory_file file(get_app(), pstruct->lpData, pstruct->cbData);

      {

         reader reader(&file);

         reader(size);

      }

      cslock sl(&m_cs);

      size.m_oswindow = (oswindow) pbase->m_wparam;

      size.m_bRet =  pcentral->m_pfilesystemsizeset->get_fs_size(
                     size.m_iSize,
                     size.m_strPath,
                     size.m_bPending);
      m_sizea.add(size);
      pbase->set_lresult(1);
   }
   else if(pstruct->dwData == 1)
   {

      file_size_table::get_fs_size size;

      memory_file file(get_app(), pstruct->lpData, pstruct->cbData);

      {

         reader reader(&file);

         reader(size);

      }

      m_bRet = true;

      m_map.set_at(size.m_strPath, size);

      pbase->set_lresult(1);

   }

#else

   ::exception::throw_not_implemented(get_app());

#endif


}

void FileSystemSizeWnd::_001OnTimer(::timer * ptimer)
{

#ifdef WINDOWSEX

   if(ptimer->m_nIDEvent == 100)
   {

      if(m_sizea.get_size() > 0)
      {

         COPYDATASTRUCT data;

         data.dwData = 1;

         memory_file file(get_app());

         writer writer(&file);

         while(m_sizea.get_size() > 0)
         {

            cslock sl(&m_cs);

            file_size_table::get_fs_size & size = m_sizea[0];

            file.Truncate(0);

            writer(size);

            data.cbData = (uint32_t) file.get_length();

            data.lpData = file.get_data();

            ::SendMessage(size.m_oswindow, WM_COPYDATA, (WPARAM) m_pui->get_os_data(), (LPARAM) &data);

            m_sizea.remove_at(0);

         }

      }

   }

#else

   ::exception::throw_not_implemented(get_app());

#endif


}

FileSystemSizeServerThread::FileSystemSizeServerThread(::aura::application * papp) :
   object(papp),
   thread(papp)
{
}

bool FileSystemSizeServerThread::init_thread()
{
   db_server * pcentral = &System.m_psimpledb->db();
   pcentral->m_pfilesystemsizeset->m_table.m_pwndServer->CreateServer();
   return true;
}

void FileSystemSizeWnd::ClientStartServer()
{

#ifdef WINDOWSEX

   db_server * pcentral = &System.m_psimpledb->db();

   if(get_tick_count() - m_dwLastStartTime > 2000)
   {
      m_dwLastStartTime = get_tick_count();

      ::aura::shell_launcher launcher(NULL, NULL, System.dir().module() / "winservice_filesystemsizeapp", NULL, NULL, SW_HIDE);

      launcher.execute();


   }

   pcentral->m_pfilesystemsizeset->m_table.m_oswindowServer = ::FindWindowEx(HWND_MESSAGE, NULL, NULL, "Local\\::draw2d::fontopus::FileSystemSizeWnd::Server");

#else

   ::exception::throw_not_implemented(get_app());

#endif

}


void file_size_table::get_fs_size::io(stream & serialize)
{

   serialize(m_strPath);
   serialize(m_bPending);
   serialize(m_iSize);
   serialize(m_bRet);

//#ifdef WINDOWSEX
//
//   serialize((int_ptr &) m_oswindow);
//
//#else
//
//   _throw(not_implemented(::get_app()));
//
//#endif

}


//void file_size_table::get_fs_size::read(serialize & istream)
//{
//   istream >> m_strPath;
//   istream >> m_bPending;
//   istream >> m_iSize;
//   istream >> m_bRet;
//
////#ifdef WINDOWS
////
////   istream >> (int32_t &) m_oswindow;
////
////#else
////
////   _throw(not_implemented(::get_app()));
////
////#endif
//
//
//}



