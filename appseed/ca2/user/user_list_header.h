#pragma once


namespace user
{


   class list;


   class CLASS_DECL_ca2 list_header :
      virtual public database::user::interaction
   {
   public:


      class CLASS_DECL_ca2 item
      {
      public:


         UINT    mask;
         int32_t     cxy;
         LPWSTR  pszText;
         HBITMAP hbm;
         int32_t     cchTextMax;
         int32_t     fmt;
         LPARAM  lParam;
         int32_t     iImage;        // index of bitmap in ImageList
         int32_t     iOrder;
         UINT    type;           // [in] filter type (defined what pvFilter is a pointer to)
         void *  pvFilter;       // [in] fillter data see above
         UINT   state;
      };


      enum EElement
      {
         ElementItem,
         ElementItemBox,
         ElementDivider,
      };


      ::ca::font_sp                 m_font;
      string                        m_strBuffer;
      visual::graphics_extension    m_dcextension;

      bool                          m_bTrack;
      bool                          m_bHover;
      EElement                      m_eelementHover;
      index                         m_iItemHover;
      bool                          m_bLButtonDown;
      EElement                      m_eelementLButtonDown;
      index                         m_iItemLButtonDown;

      list *                        m_plistctrlinterface;


      list_header(::ca::applicationsp papp);
      virtual ~list_header();

      void AddMessageHandling(::ca::message::dispatch * pinterface);
      void SetBaseListCtrlInterface(list * pinterface);

      virtual void _001OnDraw(::ca::graphics * pdc);
      virtual void DrawItem(::ca::draw_item * pdrawitem);


      virtual bool DIDDXColumn(bool bSave, index iColumn);
      virtual bool DIDDXLayout(bool bSave);
      virtual index MapItemToOrder(index iItem);
      virtual index ItemToColumnKey(index iItem);

      virtual int32_t GetDividerWidth();


      bool GetItemRect(
         LPRECT lprect, 
         EElement eelement, 
         index iItem);

      bool GetItemRect(
         LPRECT lprect, 
         EElement eelementLButtonDown, 
         index iItemLButtonDown, 
         EElement eelement, 
         index iItem);

      bool hit_test(
         POINT point, 
         EElement & eelement, 
         index & iItem);

      bool hit_test(
         POINT point, 
         EElement eelementLButtonDown, 
         index iItemLButtonDown, 
         EElement & eelement, 
         index & iItem);

      // Message Handlers
   protected:
      DECL_GEN_VSIGNAL(_001OnLButtonDown)
         DECL_GEN_VSIGNAL(_001OnLButtonUp)
         DECL_GEN_VSIGNAL(_001OnLButtonDblClk)
         DECL_GEN_VSIGNAL(_001OnMouseMove)

   };

} // namespace user



