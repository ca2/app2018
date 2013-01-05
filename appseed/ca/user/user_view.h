#pragma once


namespace user
{


   class view_creator_data;


} // namespace user


class view_update_hint;


class CLASS_DECL_ca view :
   virtual public database::user::interaction
{
public:

   sp(::user::document_interface)    m_spdocument;

   view();
   virtual ~view() = 0;

   user::document_interface * get_document() const;

   template < class DOCUMENT >
   ::ca::data * get_data();

   template < class DOCUMENT >
   DOCUMENT * get_typed_document();


   virtual void install_message_handling(::gen::message::dispatch * pinterface);

   virtual bool IsSelected(const ::radix::object* pDocItem) const; // support for OLE

   // OLE scrolling support (used for drag/drop as well)
   virtual bool OnScroll(UINT nScrollCode, UINT nPos, bool bDoScroll = TRUE);
   virtual bool OnScrollBy(size sizeScroll, bool bDoScroll = TRUE);

   // OLE drag/drop support
/*   virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject,
      uint32_t dwKeyState, point point);
   virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,
      uint32_t dwKeyState, point point);
   virtual void OnDragLeave();
   virtual bool OnDrop(COleDataObject* pDataObject,
      DROPEFFECT dropEffect, point point);
   virtual DROPEFFECT OnDropEx(COleDataObject* pDataObject,
      DROPEFFECT dropDefault, DROPEFFECT dropList, point point);
   virtual DROPEFFECT OnDragScroll(uint32_t dwKeyState, point point);*/

   virtual void OnPrepareDC(::ca::graphics * pgraphics, CPrintInfo* pInfo = NULL);

   //virtual void OnInitialUpdate(); // called first time after construct
   DECL_GEN_VSIGNAL(_001OnInitialUpdate) // called first time after construct


   // Activation
   virtual void OnActivateView(bool bActivate, view * pActivateView,
               view * pDeactiveView);
   virtual void OnActivateFrame(UINT nState, frame_window* pFrameWnd);

   // General drawing/updating
   virtual void on_update(view * pSender, LPARAM lHint, ::radix::object* pHint);
   virtual void OnDraw(::ca::graphics * pgraphics);
   virtual void OnViewUpdateHint(view * pSender, LPARAM lHint, view_update_hint * pHint);



    ::user::interaction * create_view(::ca::type_info info, user::document_interface * pdoc = NULL, ::user::interaction * pwndParent = NULL, ::id id = ::id(), ::user::interaction * pviewLast = NULL);
   static ::user::interaction * s_create_view(::ca::type_info info, user::document_interface * pdoc, ::user::interaction * pwndParent, id id, ::user::interaction * pviewLast = NULL);
   static ::user::interaction * s_create_view(::ca::create_context * pContext, ::user::interaction * pwndParent, id id);

   template < class VIEW >
   VIEW * create_view(user::document_interface * pdoc = NULL, ::user::interaction * pwndParent = NULL, ::id id = ::id(), ::user::interaction * pviewLast = NULL);

   template < class VIEW >
   VIEW * create_view(::user::interaction * pwndParent, ::id id = ::id(), ::user::interaction * pviewLast = NULL);

   template < class VIEW >
   VIEW * create_view(::user::view_creator_data * pcreatordata, ::user::interaction * pviewLast = NULL);


   static user::document_interface * get_document(::user::interaction * pguie);

   virtual void dump(dump_context &) const;
   virtual void assert_valid() const;

   // Advanced: for implementing custom print preview
/*   bool DoPrintPreview(UINT nIDResource, view * pPrintView,
         ::ca::type_info pPreviewViewClass, CPrintPreviewState* pState);*/

   virtual void CalcWindowRect(LPRECT lpClientRect,
      UINT nAdjustType = adjustBorder);
   virtual CScrollBar* GetScrollBarCtrl(int32_t nBar) const;


   virtual void on_draw_view_nc(::ca::graphics * pdc);
   virtual void on_draw_view(::ca::graphics * pdc, spa(::ca::data) spadata);
   virtual void defer_draw_view(::ca::graphics * pdc);

   virtual void _001OnDraw(::ca::graphics * pdc);


   virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);

   virtual bool pre_create_window(CREATESTRUCT& cs);
   virtual void PostNcDestroy();

   // friend classes that call protected view overridables
   friend class user::document_interface;
   friend class document_template;
   friend class CPreviewView;
   friend class frame_window;
   friend class CMDIFrameWnd;
   friend class CMDIChildWnd;
   friend class CSplitterWnd;
   friend class COleServerDoc;
   friend class CDocObjectServer;
   friend class SimpleMDIFrameWindow;
   friend class SimpleMDIChildWindow;

   DECL_GEN_SIGNAL(_001OnCreate)
   DECL_GEN_SIGNAL(_001OnDestroy)
   void OnPaint();
   //int32_t OnMouseActivate(::ca::window* pDesktopWnd, UINT nHitTest, UINT message);
   // commands
   void OnUpdateSplitCmd(cmd_ui* pCmdUI);
   bool OnSplitCmd(UINT nID);
   void OnUpdateNextPaneMenu(cmd_ui* pCmdUI);
   bool OnNextPaneCmd(UINT nID);

   // not mapped commands - must be mapped in derived class
   void OnFilePrint();
   void OnFilePrintPreview();

   // TODO: could return a kind of - also TODO - JOB object in case of assynchronous call
   virtual void collaborate(::ca::job * pjob);
   virtual int32_t  get_total_page_count(::ca::job * pjob);

   DECL_GEN_SIGNAL(_001OnView)
   DECL_GEN_SIGNAL(_001OnLButtonDown)
   DECL_GEN_SIGNAL(_001OnLButtonUp)
   DECL_GEN_SIGNAL(_001OnMouseMove)
};

class view_update_hint :
   virtual public ::radix::object
{
public:
   enum etype
   {
      TypeOpenDocument,
   };
public:
   view_update_hint(::ca::application * papp);
   etype m_etype;
};


