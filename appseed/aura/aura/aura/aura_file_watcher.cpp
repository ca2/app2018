﻿/**
	Copyright (c) 2009 James Wynn (james@jameswynn.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to uxse, cxopy, mxodify, mxerge, pxublish, dxistribute, sxublicense, and/or sxell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
#include "framework.h"


#if defined(WINDOWSEX)
#include "aura/os/windows/windows_file_watcher.h"
#elif defined(MACOS)
#include "aura/os/macos/macos_file_watcher.h"
#elif defined(LINUX)
//#include "aura/os/linux/linux_file_watcher.h"
#elif defined(ANDROID)
//#include "aura/os/android/android_file_watcher.h"
#elif defined(METROWIN)
//#include "aura/os/metrowin/metrowin_file_watcher.h"
#endif


namespace file_watcher
{


   file_watcher::file_watcher(::aura::application * papp) :
      ::object(papp)
   {

      m_bUpdating = false;

      m_pimpl = new os_file_watcher(papp);

   }


   file_watcher::~file_watcher()
   {

      if(m_pimpl != NULL)
      {

         delete m_pimpl;

         m_pimpl = NULL;

      }

   }


   file_watch_id file_watcher::add_watch(const char * directory, file_watch_listener * pwatcher, bool bRecursive, bool bOwn)
   {

      return m_pimpl->add_watch(directory, pwatcher, bRecursive, bOwn);

   }

   void file_watcher::remove_watch(const char * directory)
   {

      m_pimpl->remove_watch(directory);

   }

   void file_watcher::remove_watch(file_watch_id id)
   {

      m_pimpl->remove_watch(id);

   }


   bool file_watcher::update()
   {

      keep_true updating(m_bUpdating);

      return m_pimpl->update();

   }


//   void file_watcher_pool::install_message_routing(::message::sender * psender)
//   {
//      IGUI_MSG_LINK(WM_APP+1000, psender, this, _001OnApp1000)
//   }
//
//   void file_watcher_pool::add_watch(const char * directory, file_watch_listener * pwatcher, bool bRecursive, bool bOwn)
//   {
//
//      add * padd = new add;
//
//      padd->directory = directory;
//      padd->pwatcher = pwatcher;
//      padd->bRecursive = bRecursive;
//      padd->bOwn = bOwn;
//
//      post_thread_message(WM_APP + 1000, 0, padd);
//
//   }
//
//
//   void file_watcher_pool::_001OnApp1000(::message::message * pobj)
//   {
//
//      SCAST_PTR(::message::base, pbase, pobj);
//
//
//      delete padd;
//
//   }
//

   file_watch_listener::file_watch_listener()
   {

      m_pfilewatcherlistenerthread = NULL;

   }

   file_watch_listener::~file_watch_listener()
   {

   }

/// Handles the action file action
/// @param watchid The watch id for the directory
/// @param dir The directory
/// @param filename The filename that was accessed (not full path)
/// @param action Action that was performed
   void file_watch_listener::handle_file_action(file_watch_id idWatch, const char * szDir, const char * szFilename, e_action eaction)
   {

      UNREFERENCED_PARAMETER(idWatch);
      UNREFERENCED_PARAMETER(szDir);
      UNREFERENCED_PARAMETER(szFilename);
      UNREFERENCED_PARAMETER(eaction);

   }



} //namespace file_watcher





