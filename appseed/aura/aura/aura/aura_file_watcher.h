/**
	Main header for the file_watcher class. Declares all implementation
	classes to reduce compilation overhead.

	@author James Wynn
	@date 4/15/2009

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
#pragma once


namespace file_watcher
{


   class watch;
   class file_watcher;


   /// Type for a watch id
   using id = i64;

   typedef array < id > file_watch_array;

   // forward declarations
   class file_watcher_impl;
   class listener;

   /// Base exception class
   /// @class Exception
   class CLASS_DECL_AURA exception :
      virtual public ::exception::exception
   {
   public:

      string    m_strMessage;
      exception(const char *  message) :
         m_strMessage(message)
      {
      }

   };

   /// Exception thrown when a file is not found.
   /// @class file_not_found_exception
   class CLASS_DECL_AURA file_not_found_exception : public exception
   {
   public:
      file_not_found_exception()
         : exception("File not found")
      {}

      file_not_found_exception(const char * filename)
         : exception("File not found (" + string(filename) + ")")
      {}
   };

   /// Actions to listen for. Rename will send two events, one for
   /// the deletion of the old file, and one for the creation of the
   /// new file.
   enum e_action
   {
      /// Sent when a file is created or renamed
      action_add = 1,
      /// Sent when a file is deleted or renamed
      action_delete = 2,
      /// Sent when a file is modified
      action_modify = 4

   };

   /// Basic interface for listening for file events.
   /// @class file_watch_listener
   class CLASS_DECL_AURA listener :
      virtual public ::object
   {
   public:


      watch *        m_pwatch;


      listener();
      virtual ~listener();

      /// Handles the action file action
      /// @param watchid The watch id for the directory
      /// @param dir The directory
      /// @param filename The filename that was accessed (not full path)
      /// @param action Action that was performed
      virtual void handle_file_action(id id, const char * dir, const char * filename, e_action action);


   }; // class file_watch_listener


   template < typename PRED >
   class pred_listener :
      virtual public listener
   {
   public:


      PRED m_pred;

      pred_listener(PRED pred):
         m_pred(pred)
      {
      }

      /// Handles the action file action
      /// @param watchid The watch id for the directory
      /// @param dir The directory
      /// @param filename The filename that was accessed (not full path)
      /// @param action Action that was performed
      virtual void handle_file_action(id id, const char * dir, const char * filename, e_action action)
      {

         m_pred(id, dir, filename, action);

      }


   }; // class file_watch_listener


   /// Listens to files and directories and dispatches events
   /// to notify the parent program of the changes.
   /// @class file_watcher
   class CLASS_DECL_AURA file_watcher :
      virtual public object
   {
   private:

      /// The implementation
      file_watcher_impl       * m_pimpl;

   public:


      file_watcher(::aura::application * papp);
      virtual ~file_watcher();


      /// Add a directory watch
      /// @exception file_not_found_exception Thrown when the requested directory does not exist
      id add_watch(const char * directory, listener * pwatcher, bool bRecursive, bool bOwn = false);

      template < typename PRED >
      id pred_add_watch(const char * directory, PRED pred, bool bRecursive)
      {

         return add_watch(directory, canew(pred_listener < PRED >(pred)), bRecursive, true);

      }

      /// Remove a directory watch. This is a brute force search O(nlogn).
      void remove_watch(const char * directory);

      void remove_watch(id id);


   };//end file_watcher


} // namespace file_watcher



