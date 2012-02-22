#pragma once

class CRegExp;

class ::database::selection;
class window_redraw_interface;
class image_list;

namespace visual
{
   class CBuffer;
}

namespace userbase
{
   namespace _list
   {
      enum EElement
      {
         ElementItem,
         ElementImage,
         ElementText,
         ElementSubItem,
         ElementGroupImage,
         ElementGroupItemText,
      };
   }
}

namespace user
{


   class list_cache_interface;
   class list_header;
   class scroll_bar;
   class list_data;
   class list;
   class list_column_array;


   class CLASS_DECL_ca2 list_column
   {
   public:


      index                            m_iKey;
      index                            m_iKeyVisible;
      index                            m_iKeyNonVisible;


      bool                             m_bVisible;
      index                            m_iNextGlobalOrderKey;
      ::user::list_column_array *      m_pcontainer;
      index                            m_iOrder;
      id                               m_uiText;
      UINT                             m_uiSmallBitmap;
      COLORREF                         m_crSmallMask;
      int                              m_iSmallImageWidth;
      index                            m_iSubItem;
      image_list *                     m_pil;
      image_list *                     m_pilHover;
      ::visual::icon_int_map           m_mapIcon;
      size                             m_sizeIcon;
      bool                             m_bIcon;
      int                              m_iWidth;
      bool                             m_bCustomDraw;

      // form list attributes
      bool                             m_bEditOnSecondClick;
      index                            m_iControl;
      ::database::id                   m_datakey;




      list_column();
      list_column(const list_column & pcolumn);
      virtual ~list_column();


      static index CompareOrderSectEndNonVisible(list_column * columna, list_column * columnb);
      static index CompareOrder(list_column * columna, list_column * columnb);

      static index CompareKey(list_column * columna, list_column * columnb);

      bool operator ==(const ::user::list_column & pcolumn) const;
      list_column & operator = (const list_column & column);

   };

   class CLASS_DECL_ca2 list_column_array :
      public array_ptr_alloc < ::user::list_column >
   {
   public:


      index               m_iFirstGlobalOrderKey;
      list         *       m_plist;


      list_column_array(::ca::application * papp);


      void Initialize(list * plist);
      index MapConfigIdToKey(const ::database::id & key);





      index add(list_column & column);

      void OnChange();

      void remove(index iColumn);


      ::count NonVisibleGetCount();
      index NonVisibleMapSubItemToColumn(index iSubItem);
      void ShowSubItem(index iSubItem, bool bShow);

      index _001GetSubItemKey(index iSubItem);

      list_column * _001GetBySubItem(index iSubItem);
      list_column * _001GetByKey(index iKey);
      list_column * _001GetByConfigId(const ::database::id & key);
      list_column * _001GetVisible(index iKeyVisible);
      list_column * _001GetNonVisible(index iKeyNonVisible);

      list_column * GlobalOrderGetPrevious(index iKey);
      list_column * GlobalOrderGetNext(index iKey);

      index VisibleMapSubItemToColumn(index iSubItem);
      ::count VisibleGetCount();

      ::count get_count();


      void remove_all();
      void GlobalToVisibleOrder();

      void VisibleToGlobalOrder(index iKeyA, index iKeyB);

      void VisibleToGlobalOrder();

      void DISaveOrder();

      void DILoadOrder();

      index VisibleGetOrderFromKey(index iKey);

      index OrderToKey(index iOrder);

   };


   class CLASS_DECL_ca2 list_item :
      virtual public ::radix::object
   {
   public:

      list *         m_plist;
      index          m_iGroup;
      index          m_iItem;
      index          m_iDisplayItem;
      index          m_iColumn;
      index          m_iColumnKey;
      index          m_iOrder;
      index          m_iSubItem;
      index          m_iListItem;
      string         m_strText;
      COLORREF       m_cr;
      int            m_iState;
      index          m_iImage;
      bool           m_bOk;

      index          m_iGroupTopIndex;
      index          m_iGroupCount;

      list_column *  m_pcolumn;


      list_item(list * plist);

   };

   class CLASS_DECL_ca2 draw_list_item :
      virtual public list_item,
      virtual public draw_context
   {
   public:

      rect                 m_rectGroup;
      rect                 m_rectItem;
      rect                 m_rectSubItem;
      rect                 m_rectListItem;
      rect                 m_rectImage;
      rect                 m_rectText;
      rect *               m_prectClient;


      index                m_iGroupRectGroup;

      index                m_iItemRectItem;

      index                m_iWidthColumn;
      int                  m_iColumnWidth;
      list_column *        m_pcolumnWidth;

      index                m_iSubItemRectItem;
      index                m_iSubItemRectSubItem;
      index                m_iSubItemRectOrder;
      index                m_iSubItemRectColumn;
      list_column *        m_pcolumnSubItemRect;


      index                m_iListItemRectItem;
      index                m_iListItemRectSubItem;
      index                m_iListItemRectListItem;

      ::ca::font *         m_pfont;
      int                  m_iDrawTextFlags;

      draw_list_item(list * plist);


      image_list * get_image_list();
      bool draw_image();
      bool draw_group_image();
      void update_item_color();
      void set_text_color();
      void draw_text();

   };


   class CLASS_DECL_ca2 list :
      virtual public scroll_view,
      public ::sort::compare_interface
   {
   public:

      enum EView
      {
         ViewList,
         ViewReport,
         ViewIcon,
      };

      enum ItemState
      {
         ItemStateHover = 1,
         ItemStateSelected = 2,
      };

      enum e_flag
      {
         flag_auto_arrange,
         flag_align_to_grid,
      };




      class Range;
      class ::user::list_column_array;

      class CListItemRange
      {
      public:
         void Set(index iLBoundListItem, index iUBoundListItem);
         index      m_iLBound;
         index      m_iUBound;
      };

      class CLASS_DECL_ca2 CSubItemRange
      {

      public:
         CSubItemRange();
         CSubItemRange(CSubItemRange & subitemrange);

         index      m_iLBound;
         index      m_iUBound;
      protected:
         CListItemRange    m_listitemrange;


      public:
         void Set(index iLBoundSubItem, index iUBoundSubItem, index iLBoundListItem, index iUBoundListItem);
         bool HasSubItem(index iSubItem) const;

         CSubItemRange & operator =(const CSubItemRange & subitemrange);

      };

      class CLASS_DECL_ca2 ItemRange
      {
         friend class Range;
      public:
         ItemRange();
         ItemRange(ItemRange & itemrange);
      protected:
         index      m_iLBound;
         index      m_iUBound;
         CSubItemRange  m_subitemrange;





      public:
         void Offset(index iOffset);
         void Set(index iLBoundItem, index iUBoundItem, index iLBoundSubItem, index iUBoundSubItem, index iLBoundListItem, index iUBoundListItem);
         void SetLBound(index iLBoundItem);
         void SetUBound(index iUBoundItem);
         bool HasSubItem(index iSubItem) const;
         index GetLBound() const;
         index GetUBound() const;
         ItemRange & operator =(const ItemRange & itemrange);
         bool HasItem(index iItem) const;
   //      int get_count();
   //      int get_item(int iItemIndex);

      };

      class CLASS_DECL_ca2 Range
      {
      protected:
         base_array < ItemRange > m_itemrangea;
      public:
         Range(Range & range);
         Range();
         Range & operator = (const Range & range);

         bool HasItem(index iItem) const;
         bool HasSubItem(index iItem, index iSubItem) const;
         bool RemoveItem(index iItem);
         bool OnRemoveItem(index iItem);
         void clear();
         void add_item(const ItemRange & itemrange);
         ItemRange & ItemAt(index iIndex);
         ::count get_item_count() const;
      };




      class CLASS_DECL_ca2 list_layout :
         virtual public ex1::byte_serializable
      {
      public:
         list_layout();
         virtual ~list_layout();

         index_array   m_iaDisplayToStrict;
         int            m_iWidth;

         virtual void write(::ex1::byte_output_stream & ostream);
         virtual void read(::ex1::byte_input_stream & istream);
      };

      class CLASS_DECL_ca2 icon_layout :
         virtual public ex1::byte_serializable
      {
      public:
         icon_layout();
         virtual ~icon_layout();

         index_biunique   m_iaDisplayToStrict;
         int            m_iWidth;

         virtual void write(::ex1::byte_output_stream & ostream);
         virtual void read(::ex1::byte_input_stream & istream);
      };

   public:

      bool        m_bHoverSelect;
      bool        m_bMultiSelect;


      static const UINT MESSAGE_ENDCOLUMNHEADERDRAG;
      static const UINT MESSAGE_COLUMNHEADERTRACK;
      static const UINT MESSAGE_ENDCOLUMNHEADERTRACK;




      // Simple Filter Implementation
      // Base List Side

      enum EFilterState
      {
         FilterStateNoFilter,
         FilterStateSetup,
         FilterStateFilter,
      };

      EFilterState         m_efilterstate;
      index_biunique *     m_piaFilterIcon;
      index_array *        m_piaFilterList;

      index    m_iItemDrag;
      index    m_iItemDrop;
      bool     m_bDrag;


      

      // Sort
      bool                 m_bSort;

      bool                 m_bEmboss;

      class CSortInfoItem
      {
      public:
         index    m_iSubItem;
         bool     m_bAscendent;
      };

      class CSortInfo
      {
      public:
         base_array < CSortInfoItem, CSortInfoItem & > m_itema;
      };

      CSortInfo   m_sortinfo;







      bool                          m_bSortEnable;
      bool                          m_bHeaderCtrl;
      bool                          m_bSingleColumnMode;
      list_cache_interface *        m_pcache;
      list_header *                 m_pheaderctrl;

      LOGFONT                       m_logfont;
      visual::graphics_extension    m_dcextension;

      COLORREF                      m_crText;
      COLORREF                      m_crTextSelected;
      COLORREF                      m_crTextFocused;
      COLORREF                      m_crTextHighlight;
      COLORREF                      m_crTextSelectedHighlight;
      COLORREF                      m_crDefaultBackgoundColor;

      index                         m_iClick;

      index                         m_iItemFocus;

      bool                          m_bLockViewUpdate;
      int                           m_iItemHeight;
      int                           m_iItemWidth;

      short                         m_iWheelDelta;

      index                         m_iItemHover;
      index                         m_iSubItemHover;

      index                         m_iLastItemSel;
      index                         m_iLastSubItemSel;
      index                         m_iItemEnter;
      index                         m_iSubItemEnter;
      index                         m_iMouseFlagEnter;
      index                         m_iItemSel;
      index                         m_iSubItemSel;


      Range                         m_rangeSelection;
      Range                         m_rangeHighlight;



      index                         m_iShiftFirstSelection;
      UINT                          m_uiLButtonUpFlags;
      point                         m_ptLButtonUp;
      UINT                          m_uiRButtonUpFlags;
      point                         m_ptRButtonUp;
      CRegExp                       m_reFilter1;
      int                           m_iFilter1Step;
      bool                          m_bFilter1;

      bool                          m_bTopText;
      string                        m_strTopText;
      rect                          m_rectTopText;
      list_data *                   m_pdata;
      ::ca::font_sp                 m_font;
      ::ca::font_sp                 m_fontHover;
      ::ca::pen_sp                  m_penFocused;
      ::ca::pen_sp                  m_penHighlight;
      EView                         m_eview;
      flags < e_flag >              m_flags;
      icon_layout                   m_iconlayout;
      list_layout                   m_listlayout;


      index                         m_iTopIndex;
      index                         m_iTopGroup;
      count                         m_nDisplayCount;
      count                         m_nItemCount;
      count                         m_nGroupCount;


      image_list *                  m_pilGroup;
      image_list *                  m_pilGroupHover;
      bool                          m_bGroup;
      bool                          m_bLateralGroup;
      int                           m_iLateralGroupWidth;
      int                           m_iGroupMinHeight;
      index                         m_iGroupHover;

      draw_list_item *              m_pdrawlistitem;

      ::user::list_column_array     m_columna;

      list(::ca::application * papp);
      virtual ~list();



      int _001CalcItemWidth(::ca::graphics * pdc, index iItem, index iSubItem);
      int _001CalcItemWidth(::ca::graphics * pdc, ::ca::font * pfont, index iItem, index iSubItem);





      virtual void install_message_handling(::gen::message::dispatch * pinterface);

      list_data * GetDataInterface();
      void UpdateHover();
      ::ca::font * _001GetFont();
      ::ca::font * _001GetFontHover();
      ::ca::pen * _001GetPenFocused();
      ::ca::pen * _001GetPenHighlight();
      virtual void PreSubClassWindow();
      virtual void _OnDraw(::ca::graphics * pdc);
      void _001MaximizeColumnWidth(index iColumn);
      int _001CalcItemWidth(index iItem, index iSubItem);
      int _001CalcColumnWidth(index iColumn);
      int _001CalcListWidth();
      virtual void _001OnSort();

      // Sort
      virtual index _001Compare(index iItem1, index iItem2);
      virtual index _001Compare(index iItem1, index iItem2, index iSubItem);
      virtual index _001DisplayCompare(index iDisplayItem1, index iDisplayItem2);


      virtual bool is_valid_display_item(index iDisplayItem);
      virtual bool is_valid_strict_item(index iStrictItem);


      class size get_item_size();

      void _001OnSort(index iSubItem);
      void _001OnListHeaderItemClick(index iHeaderItem);
      void _001OnListHeaderItemDblClk(index iHeaderItem);
      void Filter1(string & str);
      bool Filter1Step();


      index _001CalcDisplayTopIndex();
      count _001CalcDisplayItemCount();
      int _001GetGroupHeight(index iGroup);


      void FilterInclude(int_array & base_array);
      void FilterInclude(index iItem);
      void FilterExcludeAll();
      void FilterClose();
      void FilterApply();
      void FilterBegin();
      index StrictToDisplay(index iStrict);
      index DisplayToStrict(index iDisplay);
      void _001Select(index iItem, index iSubItem);
      void _001Highlight(index iItem, bool bRedraw);
      void _001RemoveSelection();
      virtual bool _001OnRemoveItem(index iItem);
      bool _001RemoveItem(index iItem, bool bRedraw = true);
      void _001EnsureVisible(index iItem, bool bRedraw = true);
      void _001EnsureVisible(index iItem, Range & rangeRedraw);
      void _001ItemScroll(index iItem, bool bRedraw = true);
      index _001ConfigIdToColumnKey(const ::database::id & key);
      index _001ConfigIdToSubItem(const ::database::id & key);
      virtual bool _001HasConfigId(const ::database::id & key);
      void _001GetSelection(::database::id & key, ::database::selection & selection);

      void _001SetSingleColumnMode(bool bHeaderCtrl);
      bool _001InsertColumn(::user::list_column & column);
      void SetDataInterface(list_data * pinterface);
      void CacheHint();
      void SetCacheInterface(list_cache_interface * pinterface);
      //void AddMessageHandling(::gen::message::dispatch * pinterface);
      void _001ShowTopText(bool bShow = true);
      void _001LayoutTopText();
      void _001SetTopText(const wchar_t * lpcwsz);
      bool DIDDXHeaderLayout(bool bSave);
      virtual void DIOnSectionSet();


      virtual void data_get_DisplayToStrict();
      virtual void data_set_DisplayToStrict();
      virtual id data_get_current_sort_id();
      virtual id data_get_sort_id(EView eview);
      virtual id data_get_current_list_layout_id();

      virtual bool query_drop(index iDisplayDrop, index iDisplayDrag);
      virtual bool do_drop(index iDisplayDrop, index iDisplayDrag);
      virtual bool defer_drop(index iDisplayDrop, index iDisplayDrag);



      bool CreateHeaderCtrl();

      virtual void _001OnDraw(::ca::graphics *pdc);

      virtual void _001DrawGroups(draw_list_item * pdrawitem, index iGroupFirst, index iGroupLast, index iItemFirst, index iItemLast);

      virtual void _001DrawGroup(draw_list_item * pdrawitem);

      virtual void _001DrawItems(draw_list_item * pdrawitem, index iItemFirst, index iItemLast);

      virtual void _001DrawItem(draw_list_item * pdrawitem);

      virtual void _001DrawSubItem(draw_list_item * pdrawitem);

      virtual void _001GetItemImage(list_item * pitem);

      virtual void _001GetItemText(list_item * pitem);

      virtual void _001GetItemColor(list_item * pitem);

      virtual void _001SearchGetItemText(list_item * pitem);

      virtual ::count _001GetGroupItemCount(index iGroup);

      virtual ::count _001GetGroupMetaItemCount(index iGroup);

      virtual void _001GetGroupText(list_item * pitem);

      virtual void _001GetGroupImage(list_item * pitem);

      virtual void _001InsertColumns();

      ::count _001GetColumnCount();




      void layout();

      void SetScrollSizes();

      virtual bool _001OnUpdateItemCount(DWORD dwFlags = 0);

      void _001GetViewRect(LPRECT lprect);
      void _001GetViewClientRect(LPRECT lprect);


      virtual void _001OnInitialize();

      void _001AddColumn(list_column & pcolumn);

      void _001GetGroupRect(draw_list_item * pitem);
      void _001GetItemRect(draw_list_item * pitem);
      void _001GetSubItemRect(draw_list_item * pitem);
      void _001GetElementRect(draw_list_item * pitem, userbase::_list::EElement eelement);

      virtual void _001OnColumnChange();

      void _001SetColumnWidth(index iColumn, int iWidth);

      void _001GetColumnWidth(draw_list_item * pdrawitem);

      index _001MapSubItemToOrder(index iSubItem);

      index _001MapOrderToSubItem(index iOrder);

      index _001MapOrderToColumn(index iOrder);

      index _001MapColumnToOrder(index iColumn);

      index _001MapSubItemToColumn(index iSubItem);

      index _001MapColumnToSubItem(index iColumn);

      void _001DeleteColumn(index iColumn);


      virtual ::count _001GetItemCount();
      virtual ::count _001GetGroupCount();
      bool _001HitTest_(point point, index &iItem, index &iSubItem, index &iListItem, userbase::_list::EElement &eelement);
      bool _001HitTest_(point point, index &iItem, index &iSubItem);
      bool _001HitTest_(POINT pt, index &iItemParam);


      bool _001DisplayHitTest(point point, index &iItem, index &iSubItem, index &iListItem, userbase::_list::EElement &eelement);
      bool _001DisplayHitTest(point point, index &iItem, index &iSubItem);
      bool _001DisplayHitTest(POINT pt, index &iItemParam);

      virtual void _001OnAfterSort();


      void LayoutHeaderCtrl();

      void auto_arrange(bool bAutoArrange = true);

      bool get_auto_arrange();

      DECL_GEN_SIGNAL(_001OnSize)
      DECL_GEN_SIGNAL(_001OnMouseLeave)
      DECL_GEN_SIGNAL(_001OnMouseMove)
      DECL_GEN_SIGNAL(_001OnMouseWheel)
      DECL_GEN_SIGNAL(_001OnLButtonDown)
      DECL_GEN_SIGNAL(_001OnLButtonUp)
      DECL_GEN_SIGNAL(_001OnLButtonDblClk)
      DECL_GEN_SIGNAL(_001OnRButtonDown)
      DECL_GEN_SIGNAL(_001OnKeyDown)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnTimer)

      DECL_GEN_SIGNAL(_001OnUpdateListViewAutoArrange)

      DECL_GEN_SIGNAL(_001OnListViewAutoArrange)


      virtual void _001OnClick(UINT uiFlags, point point);
      virtual void _001OnRightClick(UINT uiFlags, point point);

      void _001GetSelection(Range & selection);


      virtual bool _001IsEditing();


      ::count _001GetSelectedItemCount();

      id _001GetColumnTextId(index iColumn);

      void HeaderCtrlLayout();

      index HeaderCtrlMapColumnToOrder(index iColumn);

      bool _001OnHeaderCtrlEndDrag(WPARAM wparam, LPARAM lparam);


      bool _001OnHeaderCtrlEndTrack(WPARAM wparam, LPARAM lparam);

      bool _001OnHeaderCtrlTrack(WPARAM wparam, LPARAM lparam);

      void _001ShowSubItem(index iSubItem, bool bShow = true);
      void DISaveOrder();

      void DILoadOrder();


      virtual void _001OnSelectionChange();

      virtual void _001UpdateColumns();


      void _001RemoveAllColumns();


      bool TwiHasTranslucency();

      void _001SetBackBuffer(visual::CBuffer *ptwb);

      void _001CreateImageList(list_column * pcolumn);

      bool _001IsItemVisible(index iItem);

      void _001ClearSelection();

      void _001SetSelection(const Range &range);

      void _001AddSelection(const ItemRange & itemrange);

      index set_cur_sel(index iSel);
      index get_cur_sel();

      void _001SetHighlightRange(Range & range);

      void _001SetView(EView eview);

      EView _001GetView();

      bool keyboard_focus_is_focusable();

      DECL_GEN_SIGNAL(_001OnVScroll)
      DECL_GEN_SIGNAL(_001OnHScroll)

   };

} // namespace user
