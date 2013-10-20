#pragma once


namespace userfs
{


   class CLASS_DECL_CORE tree_interface :
      public ::user::tree
   {
   public:


      string                        m_strPath;
      critical_section              m_csBrowse;
      bool                          m_bCreateImageList;
      bool                          m_bCreateImageListRedraw;
      int32_t                       m_iAnimate;
      bool                          m_bTimer123;
      stringa                       m_straUpdatePtrFilter;
      stringa                       m_straMissingUpdate;
      bool                          m_bDelayedListUpdate;
      sp(::data::tree_item)           m_pdataitemCreateImageListStep;
      int32_t                       m_iDefaultImage;
      int32_t                       m_iDefaultImageSelected;

         
      tree_interface(sp(base_application) papp);
      virtual ~tree_interface();


      void update_list();

      void _001OnItemExpand(sp(::data::tree_item) pitem);
      void _017UpdateList(const char * lpcsz, sp(::data::tree_item) pitemParent, int32_t iLevel);

      sp(::userfs::document) get_document();
      virtual sp(tree_data) get_fs_tree_data();

      virtual void _001InsertColumns();
      virtual void _001UpdateImageList(sp(::data::tree_item) pitem);

   

      virtual void _017Synchronize();
      void install_message_handling(::message::dispatch * pinterface);
   

      void _StartCreateImageList();
      void _StopCreateImageList();
      void _CreateImageListStep();

      // user::tree
      virtual void _001OnOpenItem(sp(::data::tree_item) pitem);
      virtual void _001OnItemCollapse(sp(::data::tree_item) pitem);


      virtual void _017OpenFolder(sp(::fs::item) item);
      
      virtual COLORREF get_background_color();

      void _017PreSynchronize();
      void TakeAnimationSnapshot();
      virtual void StartAnimation();
      void GetSelectedFilePath(stringa & stra);
      virtual bool _001IsTranslucent();

      void _017Browse(const char * lpcsz, bool bForceUpdate = false);
      void _017UpdateList();
      void _017UpdateZipList(const char * lpcsz, sp(::data::tree_item) pitemParent, int32_t iLevel);
      void _017EnsureVisible(const char * lpcsz);
      
      sp(::data::tree_item) find_item(const char * lpcsz);

      sp(::data::tree_item) find_absolute(const char * lpcsz);
      void clear(const char * lpcszPreserve1, const char * lpcszPreserve2);
      void arrange(::fs::e_arrange earrange);

      DECL_GEN_SIGNAL(_001OnTimer)

   protected:


      void _StartDelayedListUpdate();
      void _StopDelayedListUpdate();
      void _DelayedListUpdate();


   };


} // namespace userfs


