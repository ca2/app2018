/**
	Main header for the file_watcher class. Declares all implementation
	classes to reduce compilation overhead.

	@author James Wynn
	@date 4/15/2009

	Copyright (c) 2009 James Wynn (james@jameswynn.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
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


	/// Type for a watch id
	typedef uint64_t file_watch_id;
	typedef array < file_watch_id > file_watch_array;

	// forward declarations
	class file_watcher_impl;
	class file_watch_listener;
   class listener_thread;

	/// Base exception class
	/// @class Exception
	class CLASS_DECL_AURA exception
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
   class CLASS_DECL_AURA file_watch_listener
   {
   public:


      ::file_watcher::listener_thread *   m_pfilewatcherlistenerthread;


      file_watch_listener();
      virtual ~file_watch_listener();

      /// Handles the action file action
      /// @param watchid The watch id for the directory
      /// @param dir The directory
      /// @param filename The filename that was accessed (not full path)
      /// @param action Action that was performed
      virtual void handle_file_action(file_watch_id watchid, const char * dir, const char * filename, e_action action);


   }; // class file_watch_listener

   template < typename PRED >
   class pred_file_watch_listener :
      virtual public file_watch_listener
   {
   public:


      PRED m_pred;

      pred_file_watch_listener(PRED pred):
         m_pred(pred)
      {
      }

      /// Handles the action file action
      /// @param watchid The watch id for the directory
      /// @param dir The directory
      /// @param filename The filename that was accessed (not full path)
      /// @param action Action that was performed
      virtual void handle_file_action(file_watch_id watchid, const char * dir, const char * filename, e_action action)
      {

         m_pred(watchid, dir, filename, action);

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

      bool  m_bUpdating;



		file_watcher(::aura::application * papp);
		virtual ~file_watcher();

		/// Add a directory watch
		/// @exception file_not_found_exception Thrown when the requested directory does not exist
      file_watch_id add_watch(const char * directory, file_watch_listener * pwatcher, bool bRecursive, bool bOwn = false);

      template < typename PRED >
      file_watch_id pred_add_watch(const char * directory, PRED pred, bool bRecursive)
      {

         return add_watch(directory, new pred_file_watch_listener < PRED >(pred), bRecursive, true);

      }

		/// Remove a directory watch. This is a brute force search O(nlogn).
		void remove_watch(const char * directory);

      void remove_watch(file_watch_id id);

		/// Updates the watcher. Must be called often.
		virtual bool update();


	};//end file_watcher


   //class CLASS_DECL_AURA file_watcher_pool :
   //   virtual public ::thread
   //{
   //public:

   //   class CLASS_DECL_AURA add
   //   {
   //   public:
   //      
   //      string directory;
   //      file_watch_listener * pwatcher;
   //      bool bRecursive;
   //      bool bOwn;
   //   };


   //   map < file_watch_id, file_watch_id, file_watcher *, file_watcher * > m_pool;

   //   file_watcher_pool() {}
   //   virtual ~file_watcher_pool() {}

   //   virtual void install_message_routing(::message::sender * psender);

   //   /// Add a directory watch
   //   /// @exception file_not_found_exception Thrown when the requested directory does not exist
   //   file_watch_id add_watch(const char * directory, file_watch_listener * pwatcher, bool bRecursive, bool bOwn = false);

   //   template < typename PRED >
   //   file_watch_id pred_add_watch(const char * directory, PRED pred, bool bRecursive)
   //   {

   //      return add_watch(directory, new pred_file_watch_listener < PRED >(pred), bRecursive, true);

   //   }


   //   int32_t run();

   //};

} // namespace file_watcher




