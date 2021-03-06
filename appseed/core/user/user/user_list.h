#pragma once

class cregexp;

namespace database
{

   class selection;

} // namespace database

class window_redraw_interface;
class image_list;
class simple_list_data;

namespace visual
{
   class CBuffer;
}

namespace user
{


   class list_header;
   class scroll_bar;
   class list;
   class list_column_array;


   class CLASS_DECL_CORE list_column :
      virtual public object
   {
   public:

      // Always indicate what maybe "superfluous" information
      // in this case
      //
      // Cache:
      //
      // int                           m_iKey;



      bool                             m_bVisible;
      index                            m_iNextGlobalOrderKey;
      ::user::list_column_array *      m_pcontainer;
      index                            m_iOrder;
      id                               m_uiText;
      UINT                             m_uiSmallBitmap;
      COLORREF                         m_crSmallMask;
      int32_t                          m_iSmallImageWidth;
      index                            m_iSubItem;
      index                            m_iColumn;
      sp(image_list)                   m_pil;
      sp(image_list)                   m_pilHover;
      ::visual::icon_int_map           m_mapIcon;
      size                             m_sizeIcon;
      bool                             m_bIcon;
      int32_t                          m_iWidth;
      bool                             m_bCustomDraw;
      ::draw2d::dib_sp                 m_dibHeader;

      // form list attributes
      bool                             m_bEditOnSecondClick;
      index                            m_iControl;
      ::database::key                  m_datakey;


      list_column();
      list_column(const list_column & pcolumn);
      virtual ~list_column();


      static index CompareOrderSectEndNonVisible(const sp(list_column) & columna, const sp(list_column) & columnb);
      static index CompareOrder(const sp(list_column) & columna, const sp(list_column) & columnb);

      static index CompareKey(const sp(list_column) & columna, const sp(list_column) & columnb);

      bool operator ==(const ::user::list_column & pcolumn) const;
      list_column & operator = (const list_column & column);


      ::index        get_index();
      ::index        get_visible_index();

   };

   class CLASS_DECL_CORE list_column_array :
      public smart_pointer_array < ::user::list_column >
   {
   public:


      index               m_iFirstGlobalOrderKey;
      list         *       m_plist;


      list_column_array(::aura::application * papp);


      void Initialize(list * plist);
      //index MapConfigIdToKey(const ::database::key & key);





      index add(list_column & column);

      void OnChange();

      void remove(index iColumn);


      //::count NonVisibleGetCount();
      //index NonVisibleMapSubItemToColumn(index iSubItem);
      void ShowSubItem(index iSubItem, bool bShow);

      index subitem_index(index iSubItem);
      index subitem_visible_index(index iSubItem);




      //list_column * GlobalOrderGetPrevious(index iKey);
      //list_column * GlobalOrderGetNext(index iKey);

      //index VisibleMapSubItemToColumn(index iSubItem);
      ::count VisibleGetCount();

      ::count get_count();


      void remove_all();

      void DISaveOrder();

      void DILoadOrder();

      index visible_index_order(index iIndex);

      index order_index(index iOrder);

//      index subitem_index(index iSubItem);
      index config_id_index(const ::database::key & key);

//      index subitem_visible_index(index iSubItem);
      index config_id_visible_index(const ::database::key & key);
      index visible_index(index iKeyVisible);

      ::index        get_index(list_column * pcolumn);
      ::index        get_visible_index(list_column * pcolumn);
      list_column *  get_visible(index iIndex);
      list_column *  get_by_index(index iIndex);
      list_column *  get_by_subitem(index iSubItem);
      list_column *  get_by_config_id(const ::database::key & key);

   };


   class CLASS_DECL_CORE list_item :
      virtual public mesh_item
   {
   public:

      list *         m_plist;
      list_column *  m_pcolumn;


      list_item(list * plist);

   };

   class CLASS_DECL_CORE draw_list_item :
      virtual public draw_mesh_item,
      virtual public list_item
   {
   public:



      //list_column *        m_pcolumnWidth;

      //list_column *        m_pcolumnSubItemRect;


      draw_list_item(list * plist);


      sp(image_list) get_image_list();
      bool draw_image();
      bool draw_group_image();
      void update_item_color();
      void set_text_color();
      void draw_text();

   };


   class CLASS_DECL_CORE list :
      virtual public mesh
   {
   public:


      int                              m_iImageSpacing;
      bool                             m_bMorePlain;
      bool                             m_bAutoCreateListHeader;
      bool                             m_bHeaderCtrl;
      bool                             m_bSingleColumnMode;

      sp(list_header)                  m_plistheader;

      draw_list_item *                 m_pdrawlistitem;

      ::user::list_column_array        m_columna;

      sp(simple_list_data)             m_psimplelistdata;


      int_ptr_map < ::visual::fastblur > m_mapBlur;
      int_ptr_map < string >           m_mapText;
      int_ptr_map < COLORREF >           m_mapBackColor;
      int_ptr_map < ::visual::fastblur > m_mapIconBlur;
      double                           m_dIconSaturation;
      int                              m_iIconBlur;
      int                              m_iIconBlurRadius;
      int                              m_iTextSpreadRadius;
      int                              m_iTextBlurRadius;
      int                              m_iTextBlur;
      rect                             m_rectSpot;
      ::draw2d::dib_sp                 m_dibSpot;
      ::draw2d::dib_sp                 m_dibTime;




      list();
      virtual ~list();



      virtual int32_t _001CalcItemWidth(::draw2d::graphics * pgraphics, index iItem, index iSubItem);
      virtual int32_t _001CalcItemWidth(::draw2d::graphics * pgraphics, ::draw2d::font * pfont, index iItem, index iSubItem);
      virtual int32_t _001CalcItemHeight(int iBaseHeight);
      virtual int32_t _001GetDrawTextFlags(EView eview);






      virtual void install_message_routing(::message::sender * pinterface) override;

      //mesh_data * GetDataInterface();
      void UpdateHover() override;
      //::draw2d::font * _001GetFont();
      //::draw2d::font * _001GetFontHover();
      //::draw2d::pen * _001GetPenFocused();
      //::draw2d::pen * _001GetPenHighlight();
      virtual void PreSubClassWindow() override;
      virtual void _OnDraw(::draw2d::graphics * pgraphics) override;
      void _001MaximizeColumnWidth(index iColumn) override;
      int32_t _001CalcItemWidth(index iItem, index iSubItem) override;
      int32_t _001CalcColumnWidth(index iColumn) override;
      int32_t _001CalcListWidth();
      virtual void _001OnSort() override;


      virtual void _001OnBeforeDeleteRange(range & range) override;
      virtual void _001OnDeleteRange(range & range) override;
      virtual void _001DeleteRange(range & range) override;

      // Sort
      virtual index _001Compare(index iItem1, index iItem2) override;
      virtual index _002Compare(index iItem1, index iItem2, index iSubItem) override;
      virtual index _001DisplayCompare(index iDisplayItem1, index iDisplayItem2) override;


      virtual bool is_valid_display_item(index iDisplayItem) override;
      virtual bool is_valid_strict_item(index iStrictItem) override;


      class size get_item_size();

      void _001OnSort(index iSubItem);
      void _001OnListHeaderItemClick(index iHeaderItem);
      void _001OnListHeaderItemDblClk(index iHeaderItem);
      void Filter1(string & str);
      bool Filter1Step();


      index _001CalcDisplayTopIndex();
      count _001CalcDisplayItemCount();
      int32_t _001GetGroupHeight(index iGroup);


      void FilterInclude(int_array & array);
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
      virtual bool _001OnRemoveItem(index iItem) override;
      bool _001RemoveItem(index iItem, bool bRedraw = true);
      void _001EnsureVisible(index iItem, e_align ealign = align_vertical_center, bool bRedraw = true);
      void _001EnsureVisible(index iItem, range & rangeRedraw);
      void _001ItemScroll(index iItem, bool bRedraw = true);
      index config_id_index(const ::database::key & key);
      index _001ConfigIdToSubItem(const ::database::key & key);
      virtual bool _001HasConfigId(const ::database::key & key) override;
      void _001GetSelection(::database::key & key, ::database::selection & selection);

      void _001SetSingleColumnMode(bool bHeaderCtrl);
      bool _001InsertColumn(::user::list_column & column);
      void SetDataInterface(mesh_data * pinterface) override;
      void CacheHint();
      //void SetCacheInterface(mesh_cache_interface * pinterface);
      //void AddMessageHandling(::message::sender * pinterface);
      void _001ShowTopText(bool bShow = true);
      void _001LayoutTopText();
      void _001SetTopText(const unichar * lpcwsz);
      bool DIDDXHeaderLayout(bool bSave);


      virtual void data_get_DisplayToStrict() override;
      virtual void data_set_DisplayToStrict() override;
      virtual id data_get_current_sort_id() override;
      virtual id data_get_sort_id(EView eview) override;
      virtual id data_get_current_list_layout_id();

      virtual bool query_drop(index iDisplayDrop, index iDisplayDrag) override;
      virtual bool do_drop(index iDisplayDrop, index iDisplayDrag) override;
      virtual bool defer_drop(index iDisplayDrop, index iDisplayDrag) override;



      bool CreateHeaderCtrl();

      virtual void _001OnNcDraw(::draw2d::graphics * pgraphics) override;

      virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;

      virtual void _001DrawGroups(draw_list_item * pdrawitem, index iGroupFirst, index iGroupLast, index iItemFirst, index iItemLast);

      virtual void _001DrawGroup(draw_list_item * pdrawitem);

      virtual void _001DrawItems(draw_list_item * pdrawitem, index iItemFirst, index iItemLast);

      virtual void _001DrawItem(draw_list_item * pdrawitem);

      virtual void _001DrawSubItem(draw_list_item * pdrawitem);

      //virtual void _001GetItemImage(list_item * pitem);

      //virtual void _001GetItemText(list_item * pitem);

      //virtual void _001GetItemColor(list_item * pitem);

      //virtual void _001SearchGetItemText(list_item * pitem);

      virtual ::count _001GetGroupItemCount(index iGroup) override;

      virtual ::count _001GetGroupMetaItemCount(index iGroup) override;

      //virtual void _001GetGroupText(list_item * pitem);

      //virtual void _001GetGroupImage(list_item * pitem);

      virtual void _001InsertColumns();

      ::count _001GetColumnCount() override;


      virtual ::user::list_header * create_list_header();
      virtual ::user::mesh_data * create_mesh_data() override;

      void on_layout() override;

      virtual bool _001OnUpdateItemCount(uint32_t dwFlags = 0) override;

      void on_change_view_size() override;


      virtual void _001OnInitialize() override;


      virtual void _001AddColumn(list_column & pcolumn);
      virtual void _001OnAddColumn(list_column * pcolumn);

      virtual void _001GetGroupRect(draw_list_item * pitem);
      virtual void _001GetItemRect(draw_list_item * pitem);
      virtual void _001GetSubItemRect(draw_list_item * pitem);
      virtual void _001GetElementRect(draw_list_item * pitem, ::user::mesh::e_element eelement);

      virtual void _001OnColumnChange();

      virtual bool _001SetColumnWidth(index iColumn, int32_t iWidth) override;

      virtual void _001GetColumnWidth(draw_list_item * pdrawitem);

      virtual index _001MapSubItemToOrder(index iSubItem) override;

      virtual index _001MapOrderToSubItem(index iOrder) override;

      virtual index _001MapOrderToColumn(index iOrder) override;

      virtual index _001MapColumnToOrder(index iColumn) override;

      index _001MapSubItemToColumn(index iSubItem);

      index _001MapColumnToSubItem(index iColumn);

      void _001DeleteColumn(index iColumn);


      virtual ::count _001GetItemCount() override;
      virtual ::count _001GetGroupCount() override;
      bool _001HitTest_(point point, index &iItem, index &iSubItem, index &iListItem, ::user::mesh::e_element &eelement) override;
      bool _001HitTest_(point point, index &iItem, index &iSubItem) override;
      bool _001HitTest_(POINT pt, index &iItemParam) override;


      bool _001DisplayHitTest(point point, index &iItem, index &iSubItem, index &iListItem, ::user::mesh::e_element &eelement) override;
      bool _001DisplayHitTest(point point, index &iItem, index &iSubItem) override;
      bool _001DisplayHitTest(POINT pt, index &iItemParam) override;

      virtual void _001OnAfterSort() override;


      void LayoutHeaderCtrl();

      void auto_arrange(bool bAutoArrange = true);

      bool get_auto_arrange();

      virtual void on_create_draw_item() override;

      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnLButtonDblClk);
      DECL_GEN_SIGNAL(_001OnRButtonDown);
      DECL_GEN_SIGNAL(_001OnKeyDown);
      DECL_GEN_SIGNAL(_001OnCreate);
      void _001OnTimer(::timer * ptimer) override;

      DECL_GEN_SIGNAL(_001OnUpdateListViewAutoArrange);

      DECL_GEN_SIGNAL(_001OnListViewAutoArrange);


      virtual bool _001OnItemClick(index iItem);

      virtual bool _001OnClick(uint_ptr uiFlags, point point) override;
      virtual bool _001OnRightClick(uint_ptr uiFlags, point point) override;

      virtual void _001GetSelection(range & selection);
      virtual index _001GetCurItem();

      virtual void _001SelectItem(index iItem);

      virtual bool _001IsEditing() override;


      ::count _001GetSelectedItemCount();
      ::count _001GetSelectedItems(index_array & ia);


      id _001GetColumnTextId(index iColumn);

      void HeaderCtrlLayout();

      index HeaderCtrlMapColumnToOrder(index iColumn);

      bool _001OnHeaderCtrlEndDrag(WPARAM wparam, LPARAM lparam);


      bool _001OnHeaderCtrlEndTrack(WPARAM wparam, LPARAM lparam);

      bool _001OnHeaderCtrlTrack(WPARAM wparam, LPARAM lparam);

      void _001ShowSubItem(index iSubItem, bool bShow = true);
      void DISaveOrder();

      void DILoadOrder();


      virtual void _001OnSelectionChange() override;

      virtual void _001UpdateColumns();


      void _001RemoveAllColumns();


      //bool TwiHasTranslucency();

      void _001SetBackBuffer(visual::CBuffer *ptwb);

      void _001CreateImageList(list_column * pcolumn);

      bool _001IsItemVisible(index iItem);

      void _001ClearSelection();

      void _001SetSelection(const range &range);

      void _001AddSelection(const item_range & itemrange);

      index set_cur_sel(index iSel);
      index get_cur_sel();

      void set_cur_sel(const index_array & iaSel);
      void get_cur_sel(index_array & iaSel);

      void _001SetHighlightRange(range & range);

      //void _001SetView(EView eview, bool bLayout = true);

      EView _001GetView();


      virtual int32_t get_wheel_scroll_delta() override;

      virtual void on_change_viewport_offset() override;

      bool keyboard_focus_is_focusable() override;

      DECL_GEN_SIGNAL(_001OnVScroll);
      DECL_GEN_SIGNAL(_001OnHScroll);

      virtual void data_update_visible_subitem() override;

      virtual void defer_create_mesh_data() override;

      virtual void on_viewport_offset(::draw2d::graphics * pgraphics) override;

      virtual void _001OnClip(::draw2d::graphics * pgraphics) override;

      virtual bool GetClientRect(LPRECT lprect) override;

      virtual void update_icon_list_view_sort();

   };


} // namespace user




