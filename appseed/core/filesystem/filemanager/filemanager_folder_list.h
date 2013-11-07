#pragma once


#include "::filemanager::folder_list.h"


namespace filemanager
{


   class CLASS_DECL_CORE ::filemanager::folder_list :
      virtual public ::user::view,
      virtual public ::filemanager::::filemanager::folder_list
   {
   public:
      
      
      simple_list_header_control    m_headerctrl;
      sp(::filemanager::manager)        m_pfilemanager;

      
      ::filemanager::folder_list (sp(base_application) papp);
      virtual ~::filemanager::folder_list ();
      
      virtual void install_message_handling(::message::dispatch * pinterface);
      virtual void OnDraw(::draw2d::graphics * pgraphics);      // overridden to draw this ::user::view
      virtual void on_update(sp(::user::view) pSender, LPARAM lHint, object* pHint);
      virtual bool pre_create_window(CREATESTRUCT& cs);

      

      DECL_GEN_SIGNAL(_001OnLButtonDblClk)
      DECL_GEN_SIGNAL(_001OnCancelMode)

#ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
#endif
      virtual public ::user::list,
         virtual public ::filemanager::data_interface
      {
      public:

         enum EMode
         {
            ModeNormal, // a bit ridiculous that folder list
            ModeConfigurationItem,
         };

         ::filemanager::folder_list(sp(base_application) papp);
         virtual ~::filemanager::folder_list();

      public:
         class Folder
         {
         public:
            string      m_strPath;
            string      m_wstrName;
            int32_t          m_iImage;
         };
         class FolderArray :
            protected array < Folder, Folder & >
         {

         public:
            void AddFolder(Folder & folder);
            ::count GetFolderCount();
#ifdef WINDOWSEX
            void clear(LPITEMIDLIST lpiidlPreserve1, LPITEMIDLIST lpiidlPreserve2);
#endif
            void clear();
            Folder & GetFolder(index i);
         };

         virtual void _017Synchronize();

         FolderArray         m_foldera;
         map < HICON, HICON, int32_t, int32_t > m_iconmap;

         ::fs::item & GetFileManagerItem();

         virtual void _001GetItemImage(::user::list_item * pitem);
         void _017UpdateList(const char * lpcszParent);
         virtual void _001CreateImageList(::user::list_column * column);
         void _017UpdateList();
         void _017Browse(const char * lpcsz);
         void _017Browse(const wchar_t * lpcsz);
         virtual void _001GetItemText(::user::list_item * pitem);
         virtual count _001GetItemCount();
         virtual void _001InsertColumns();

         virtual void install_message_handling(::message::dispatch * pinterface);

         virtual COLORREF get_background_color();

   };


} // namespace filemanager




