#pragma once


namespace fs
{

   class main_view;

   class CLASS_DECL_ca list :
      virtual public list_interface
   {
   public:


      simple_list_header_control    m_headerctrl;
      simple_scroll_bar             m_scrollbarVert;
      simple_scroll_bar             m_scrollbarHorz;


      list(::ca::application * papp);
      virtual ~list();


      void _001OnClick(UINT uiFlags, point point);

      virtual void _001InstallMessageHandling(::user::win::message::dispatch * pdispatch);
      
      virtual void on_update(::view * pSender, LPARAM lHint, ::radix::object* pHint);
      virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnLButtonDblClk)
      DECL_GEN_SIGNAL(_001OnCancelMode)

#ifdef _DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
#endif

   };


} // namespace fs

