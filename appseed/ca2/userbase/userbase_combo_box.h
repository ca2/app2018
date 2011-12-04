#pragma once


namespace userbase
{

   class CLASS_DECL_ca2 combo_box :
      virtual public ::user::combo_box
   {
   public:



      combo_box();
      virtual ~combo_box();


      virtual bool create(::user::interaction* puiParent, id id);

      int get_count();
      int get_cur_sel();
      int set_cur_sel(int nSelect);
      LCID GetLocale();
      LCID SetLocale(LCID nNewLocale);
      int GetTopIndex();
      int SetTopIndex(int nIndex);
      int InitStorage(int nItems, UINT nBytes);
      void SetHorizontalExtent(UINT nExtent);
      UINT GetHorizontalExtent();
      int SetDroppedWidth(UINT nWidth);
      int GetDroppedWidth();

   #if(WINVER >= 0x0500)
      BOOL GetComboBoxInfo(PCOMBOBOXINFO pcbi);
   #endif   // WINVER >= 0x0500

      // for edit control
      DWORD GetEditSel();
      BOOL LimitText(int nMaxChars);
      BOOL SetEditSel(int nStartChar, int nEndChar);

      // for combobox item
      DWORD_PTR GetItemData(int nIndex);
      int SetItemData(int nIndex, DWORD_PTR dwItemData);
      void * GetItemDataPtr(int nIndex);
      int SetItemDataPtr(int nIndex, void * pData);
      int GetLBText(int nIndex, LPTSTR lpszText);
      void GetLBText(int nIndex, string & rString);
      int GetLBTextLen(int nIndex);

      int SetItemHeight(int nIndex, UINT cyItemHeight);
      int GetItemHeight(int nIndex);
      int FindStringExact(int nIndexStart, const char * lpszFind);
      int SetExtendedUI(BOOL bExtended = TRUE);
      BOOL GetExtendedUI();
      void GetDroppedControlRect(LPRECT lprect);
      BOOL GetDroppedState();

   // Operations
      // for drop-down combo boxes
      void ShowDropDown(BOOL bShowIt = TRUE);

      // manipulating listbox items
      int AddString(const char * lpszString);
      int DeleteString(UINT nIndex);
      int InsertString(int nIndex, const char * lpszString);
      void ResetContent();
      int Dir(UINT attr, const char * lpszWildCard);

      // selection helpers
      int FindString(int nStartAfter, const char * lpszString);
      int SelectString(int nStartAfter, const char * lpszString);

      // Clipboard operations
      void clear();
      void copy();
      void Cut();
      void Paste();

   // Overridables (must override draw, measure and compare for owner draw)
#ifdef WINDOWS
      virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
      virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
      virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
      virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
#endif

      virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
   };


} // namespace userbase
