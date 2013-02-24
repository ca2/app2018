#pragma once

namespace userbase
{

   class tab_drop_target_window;
   class single_document_template;

   class CLASS_DECL_ca2 tab_view : 
      virtual public ::userbase::view,
      virtual public ::user::tab,
      virtual public ::user::tab_callback,
      virtual public ::user::view_container
   {
   public:
      
      
      tab_drop_target_window *               m_pdroptargetwindow;
      ::user::view_creator_data *      m_pviewdata;
      ::user::view_creator_data *      m_pviewdataOld;
      ::user::view_creator *                 m_pviewcreator;


      tab_view(::ca::application * papp);
      virtual ~tab_view();



      virtual id get_view_id();

      virtual ::user::interaction * get_view_uie();
      virtual ::user::view_creator_data * get_view_creator_data();
      virtual ::user::document_interface * get_view_document();


      void _000OnDraw(::ca::graphics * pdc);

      void _001OnDropTab(int32_t iPane, e_position eposition);
      
      ::user::interaction* _001GetTabWnd(int32_t iTab);

      bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);


      virtual void set_view_creator(::user::view_creator * pview);
      virtual ::user::view_creator * get_view_creator();


      virtual void _001DropTargetWindowInitialize(::user::tab * pinterface);
      virtual void _001DropTargetWindowRelay(::user::tab * pinterface);
      virtual void _001DropTargetWindowFinalize(::user::tab * pinterface);

      virtual void rotate();

      virtual ::user::view_creator_data * ensure(id id);
      virtual void ensure_tab_by_id(id id);

      virtual void _001OnShowTab(::user::tab * ptab);
      virtual void on_show_view();
      virtual void install_message_handling(::ca::message::dispatch * pinterface);

      void _001OnTabClick(int32_t iTab);


      virtual void on_update(::view * pSender, LPARAM lHint, ::ca::object* pHint);
      virtual bool pre_create_window(CREATESTRUCT& cs);

      DECL_GEN_SIGNAL(_001OnMenuMessage)

      
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnSetFocus)
   };

   class tab_drop_target_window : 
      virtual public ::user::interaction
   {
   public:


      ::user::tab *     m_ptab;
      int32_t               m_iPane;


      tab_drop_target_window(::user::tab * ptab, int32_t iPane);
      virtual ~tab_drop_target_window();


      virtual void install_message_handling(::ca::message::dispatch * pinterface);

      void _001OnDraw(::ca::graphics * pdc);

      DECL_GEN_SIGNAL(_001OnLButtonUp)

   };

} // namespace userbase