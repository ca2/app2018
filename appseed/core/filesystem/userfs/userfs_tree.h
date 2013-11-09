#pragma once


namespace userfs
{


   class main_view;


   class CLASS_DECL_CORE tree :
      public ::data::tree
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


      tree(sp(base_application) papp);
      virtual ~tree();


      void update_list();

      void _001OnItemExpand(::data::tree_item * pitem);
      void _017UpdateList(const char * lpcsz, ::data::tree_item * pitemParent, int32_t iLevel);

      sp(::userfs::document) get_document();

      virtual void _001InsertColumns();
      virtual void _001UpdateImageList(::data::tree_item * pitem);



      virtual void _017Synchronize();
      void install_message_handling(::message::dispatch * pinterface);


      void _StartCreateImageList();
      void _StopCreateImageList();
      void _CreateImageListStep();

      // user::tree
      virtual void _001OnOpenItem(::data::tree_item * pitem);
      virtual void _001OnItemCollapse(::data::tree_item * pitem);


      virtual void _017OpenFolder(sp(::fs::item) item);

      virtual COLORREF get_background_color();

      void _017PreSynchronize();
      void TakeAnimationSnapshot();
      virtual void StartAnimation(::user::interaction * pui);
      void GetSelectedFilePath(stringa & stra);
      virtual bool _001IsTranslucent();

      void _017Browse(const char * lpcsz, bool bForceUpdate = false);
      void _017UpdateList();
      void _017UpdateZipList(const char * lpcsz, ::data::tree_item * pitemParent, int32_t iLevel);
      void _017EnsureVisible(const char * lpcsz);

      sp(::data::tree_item) find_item(const char * lpcsz);

      sp(::data::tree_item) find_absolute(const char * lpcsz);
      void clear(const char * lpcszPreserve1, const char * lpcszPreserve2);
      void arrange(::fs::e_arrange earrange);

      DECL_GEN_SIGNAL(_001OnTimer)


      void _StartDelayedListUpdate();
      void _StopDelayedListUpdate();
      void _DelayedListUpdate();


      void RenameFile(int32_t iLine, string & str);

      virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg); 

      virtual void on_update(sp(::user::view) pSender, LPARAM lHint, object* pHint);
      virtual bool pre_create_window(CREATESTRUCT& cs);
    
      DECL_GEN_SIGNAL(_001OnLButtonDblClk)
      DECL_GEN_SIGNAL(_001OnContextMenu)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnShellCommand)

   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif


   };


} // namespace userfs



