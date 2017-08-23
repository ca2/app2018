#pragma once


class db_server;
class FileSystemSizeWnd;


class CLASS_DECL_BASE file_size_table :
   virtual public ::object
{
public:


   class get_fs_size :
      public ::file::serializable
   {
   public:


      string   m_strPath;
      bool     m_bPending;
      int64_t  m_iSize;
      bool     m_bRet;
      oswindow     m_oswindow;


      get_fs_size();
      get_fs_size(const get_fs_size & getfssize);


      get_fs_size & operator = (const get_fs_size & getfssize);


      virtual void write(::file::ostream & ostream) const;
      virtual void read(::file::istream & istream);


   };


   class item :
      virtual public object
   {
   public:


      item *      m_pitemParent;
      string      m_strName;
      bool        m_bPending;
      int64_t     m_iSize;
      index       m_iStep;
      bool        m_bDir;
      bool        m_bPendingLs;

      spa(item)   m_itema;


      item();


      void update_size(::aura::application * papp, index & iIteration);
      void update_size_recursive(::aura::application * papp, index & iIteration);

      item * FindItem(::aura::application * papp, const char * pszPath, index & iIteration);
      index FindName(::aura::application * papp, const char * pszName, index & iIteration);

      string path();

      void ls(::aura::application * papp, index & iIteration);
   };



   item                    m_item;
   HANDLE                  m_hmap;
   class get_fs_size *     m_pgetfssize;
   FileSystemSizeWnd *     m_pwnd;
   FileSystemSizeWnd *     m_pwndServer;
   oswindow                    m_oswindowServer;
   ::event *               m_pevExec;
   ::event *               m_pevDone;
   ::mutex *               m_pmutex;





   file_size_table(::aura::application * papp);
   ~file_size_table();

   bool check_map();

};



class CLASS_DECL_BASE DBFileSystemSizeSet :
   virtual public ::object
{
public:


   file_size_table            m_table;
   index                     m_iMaxIteration;


   DBFileSystemSizeSet(::aura::application * papp);
   ~DBFileSystemSizeSet();



   bool get_cache_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending);
   bool get_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending);


   bool get_fs_size(int64_t & i64Size, const ::file::path & pszPath, bool & bPending, index & iIteration);


};





class CLASS_DECL_BASE FileSystemSizeServerThread :
   virtual public thread
{
public:

   FileSystemSizeServerThread(::aura::application * papp);

   bool initialize_thread() override;

};




class CLASS_DECL_BASE FileSystemSizeWnd :
   public ::message::receiver
{
public:


   class size_map :
      public string_map < file_size_table::get_fs_size, file_size_table::get_fs_size >
   {
   public:
   };


   uint32_t                            m_dwLastStartTime;
   bool                             m_bServer;
   file_size_table::get_fs_size     m_size;
   bool                             m_bRet;
   critical_section                 m_cs;
   array < file_size_table::get_fs_size, file_size_table::get_fs_size & > m_sizea;

   size_map                         m_map;
   ::user::interaction *            m_pui;

   FileSystemSizeWnd(::aura::application * papp);

   bool CreateClient();
   bool CreateServer();



   void ClientStartServer();
   bool get_fs_size(int64_t & i64Size, const char * pszPath, bool & bPending);

   void install_message_routing(::message::sender * pinterface);

   DECL_GEN_SIGNAL(_001OnCopyData);
   void _001OnTimer(::timer * ptimer);


};


