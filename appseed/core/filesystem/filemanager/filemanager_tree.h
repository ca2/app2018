#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE tree :
      virtual public ::userfs::tree,
      virtual public ::filemanager::impact
   {
   public:


      enum e_message
      {

         MessageMainPost,

      };

      enum EMessageMainPost
      {

         MessageMainPostCreateImageListItemRedraw,

      };

      enum e_step
      {
         step_start,
         step_image_visible,
         step_has_subdir_visible,
         step_image_hidden,
         step_has_subdir_hidden,
         step_end,
      };



      context_menu            m_contextmenu;
      

#ifdef WINDOWSEX
      map < EFolder, EFolder, IShellFolder *, IShellFolder *> m_mapFolder;
#endif


      tree(::aura::application * papp);
      virtual ~tree();


      virtual void assert_valid() const override;
      virtual void dump(dump_context & dumpcontext) const override;

      virtual void _001InsertColumns() override;
      //virtual void _polishing_step(::data::tree_item * pitem, bool bLowLatency, e_step estep);

      void _001OnTimer(::timer * ptimer) override;


      void install_message_routing(::message::sender * pinterface) override;

      // tree_polishing
      //void _polishing_start(::user::tree * pusertree);
      //void _polishing_run(::data::tree_item * pitem, ::user::tree * pusertree, bool bLowLatency);
      //bool _polishing_step(::data::tree_item * pitem, bool bLowLatency);

      // user::tree
      virtual void _001OnOpenItem(::data::tree_item * pitem, ::action::context actioncontext) override;
      virtual void _001OnItemExpand(::data::tree_item * pitem, ::action::context actioncontext) override;
      virtual void _001OnItemCollapse(::data::tree_item * pitem, ::action::context actioncontext) override;


      virtual void _017OpenFolder(::fs::item * pitem, ::action::context actioncontext) override;

#ifdef WINDOWSEX

      int32_t MapToCSIDL(EFolder efolder);

      IShellFolder * _001GetFolder(EFolder efolder);

#endif


      void TakeAnimationSnapshot();
      virtual void StartAnimation();
      DECL_GEN_SIGNAL(_001OnMainPostMessage);
      void GetSelectedFilePath(stringa & stra);
      virtual bool _001IsTranslucent() override;

      
      virtual void browse_sync(::action::context actioncontext) override;
      virtual void knowledge(const ::file::path & strPath,::action::context actioncontext,bool bOnlyParent = false);
      virtual void filemanager_tree_insert(const ::file::path & strPath,::file::listing & listing, ::action::context actioncontext,bool bOnlyParent = false, bool bVoidTreeDataChangeEvent = true);
      void _017EnsureVisible(const ::file::path & path, ::action::context actioncontext);

      ::data::tree_item * find_item(const ::file::path & path, bool bPathFromItemFromOwnTree = false, ::data::tree_item * pitemStart = NULL);







      void RenameFile(int32_t iLine, string & str, ::action::context actioncontext);

      virtual void on_update(::user::impact * pSender, LPARAM lHint, object* pHint) override;


      DECL_GEN_SIGNAL(_001OnLButtonDblClk);
      DECL_GEN_SIGNAL(_001OnContextMenu);
      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnShellCommand);


      virtual void on_merge_user_tree(::user::tree * pusertree) override;
      virtual void on_bind_user_tree(::user::tree * pusertree) override;


   };


} // namespace filemanager



