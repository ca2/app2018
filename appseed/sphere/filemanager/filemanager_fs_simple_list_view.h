#pragma once

class image_list;

namespace filemanager
{

   namespace fs
   {

      namespace simple
      {

         class view;

         enum EItemType
         {
            ItemTypeFolder,
            ItemTypeArtist,
            ItemTypeSong,
         };
         class Item
         {
         public:
            int64_t     m_iParent;
            int64_t     m_iFolder;
            string      m_strTitle;
            string      m_strFileName;
            string      m_strExtension;
            int32_t         m_iId;
            EItemType   m_etype;
            int32_t         m_iImage;

            bool IsFolder();
         };

         class ItemArray :
            public array_ptr_alloc < Item, Item & >
         {
         public:
            //int32_t FindAbsolute(const char * lpszId);
         };


         class view;

         class CLASS_DECL_ca2 list_view :
            public simple_list_view
         {
         public:

            enum EFieldIndex
            {
               FieldIndexNone = -1,
               FieldIndexId = 0,
               FieldIndexTitle,
               FieldIndexArtist,
               FieldIndexFileName,
               FieldIndexFilePath,
            };

            enum ESubItem
            {
               SubItemId,
               SubItemTitle,
               SubItemArtist,
               SubItemFileName,
               SubItemFilePath,
            };

            list_view(::ca::application * papp);
            ::filemanager::fs::simple::view * m_pserver;

            void install_message_handling(::gen::message::dispatch * pinterface);
       
            ::user::list_cache        m_cache;
            bool                 m_bKickActive;

            ItemArray         m_itema; 

            image_list *          m_pil;
            int64_t m_iParentFolder;


            int32_t m_iIconFolder;
            int32_t m_iIconArtist;
            int32_t m_iIconSong;

            class BuildHelper
            {
            public:
               BuildHelper();
               ::index      m_iTopIndex;
               ::count      m_iDisplayItemCount;
               ::index      m_iStep;
               bool     m_bActive;
            };

            BuildHelper          m_buildhelper;

            static UINT c_cdecl ThreadProcFillTask(LPVOID lpParameter);

            class FillTask
            {
            public:
               //FillTask(MAlbumView * pview, const wchar_t * lpcsz);
         //      list_view *      m_pview;
               string             m_strFile;
               UINT              m_uiTimer;

            };


            void parse(const char * lpszSource);
            void KickBuild(int32_t iItem = -1);
         
            void PostFillTask(string & strFile, uint_ptr uiTimer);

            virtual void _001OnSort();
            bool GetSongPath(string & str, index iItem);

            virtual void _001GetItemImage(::user::list_item * pitem);
            virtual void _001InsertColumns();
            virtual void _001GetItemText(::user::list_item * pitem);
            virtual count _001GetItemCount();
            virtual void _001SearchGetItemText(::user::list_item * pitem);

            virtual void on_update(::view * pSender, LPARAM lHint, ::radix::object* pHint);

            virtual void _001OnClick(UINT nFlags, point point);

            virtual ~list_view();
         #ifdef DEBUG
            virtual void assert_valid() const;
            virtual void dump(dump_context & dumpcontext) const;
         #endif

            DECL_GEN_SIGNAL(_001OnLButtonDblClk)
            DECL_GEN_SIGNAL(_001OnTimer)
            DECL_GEN_SIGNAL(_001OnSize)
            DECL_GEN_SIGNAL(_001OnContextMenu)
            //DECL_GEN_SIGNAL(_001OnAlbumExecutePlay)
            //DECL_GEN_SIGNAL(_001OnUpdateAlbumExecutePlay)
            DECL_GEN_SIGNAL(_001OnExecutePrevious)
            DECL_GEN_SIGNAL(_001OnUpdateExecutePrevious)
            DECL_GEN_SIGNAL(_001OnExecuteNext)
            DECL_GEN_SIGNAL(_001OnUpdateExecuteNext)
            DECL_GEN_SIGNAL(_001OnEraseBkgnd)
            DECL_GEN_SIGNAL(_001OnFillTaskResponse)

         };


      } // simple


   } // namespace fs


} // namespace filemanager

