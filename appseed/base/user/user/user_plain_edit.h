#pragma once


#include "base/user/user/user_control.h"
#include "base/user/user/user_plain_text_data.h"


class elemental;
class element_2d;

namespace colorertake5
{
   class base_editor;
   class text_lines;
   class file_type;
} // namespace colorertake5


namespace user
{


   class plain_edit_internal;

   class CLASS_DECL_BASE plain_edit :
      virtual public control,
      virtual public ::data::listener,
      virtual public edit_text
   {
   public:

      class error
      {
      public:
         strsize     m_iStart;
         strsize     m_iEnd;
         DWORD       m_dwTime;
         string      m_strMessage;
      };


      e_rect                        m_erectPadding;
      e_int                         m_eintDrawTextFlags;
      DWORD                         m_dwLastDraw;
      array <error> m_errora;
      //WPARAM                        m_dwLastKeyWparam;
      //LPARAM                        m_dwLastKeyLparam;
      sp(::message::key)            m_pmessagekeyLast;
      //size                          m_sizeTotal;

      string                        m_strEmtpyText;
      plain_edit_internal *         m_pinternal;
      bool                          m_bNeedCalcLayout;
      bool                          m_bCalcLayoutHintNoTextChange;

      int                           m_iTabWidth;
      bool                          m_bColorerTake5;
      index                         m_iLineStart;
      index                         m_iLineEnd;
      count                         m_iLineCount;
      //visual::dib_sp                m_dibBk;
      bool                          m_bCustomFrameBefore;
      rect                          m_FullScreenWindowRect;
      //visual::fastblur              m_fastblur;

      string_to_string              m_base64map;

      bool                          m_bPassword;
      bool                          m_bEnterKeyOnPaste;
      bool                          m_bLMouseDown;
      bool                          m_bRMouseDown;
      point                         m_ptSelStart;
      bool                          m_bFocus;
//      bool                          m_bCaretOn;
//      uint32_t                      m_dwLastCaret;
      uint32_t                      m_dwCaretTime;
      index                         m_iLineOffset;
      //stringa                       m_straSep;
      //colorertake5::base_editor *   m_pcolorereditor;
      //colorertake5::text_lines *    m_plines;
      stringa                        m_straLines;
      int32_t                       m_y;
      bool                          m_bGetTextNeedUpdate;
      bool                          m_bNeedScrollUpdate;
      bool                          m_bTabInsertSpaces;
      //strsize                       m_iSelStart;
      //strsize                       m_iSelEnd;
      strsize                       m_iViewOffset; // in bytes
      strsize                       m_iViewSize; // in bytes
      int32_t                       m_iLineHeight;
      index                         m_iColumn;
      int32_t                       m_iColumnX;
      bool                          m_bMultiLine;
      bool                          m_bSendEnterKey;
      bool                          m_bReadOnly;
      point                         m_ptLastCursor;
      ::draw2d::memory_graphics     m_pmemorygraphics;
      //::draw2d::font_sp             m_spfont;

      // Used for whatever it can make faster for large files (scroll for example)
      // keep each line size
      index_array                   m_iaLineLen;
      // Used for whatever it can make faster for large files (scroll for example)
      // keep each line end flag 3 = \r \n     1 = \n  \r = 2
      index_array                   m_iaLineEnd;
      index_array                   m_iaLineBeg;

      bool                          m_bOwnData;
      sp(plain_text_tree)           m_ptree;
      ::data::tree_item *           m_pitem;


      bool                          m_bActionHover;
      array < double_array >        m_daExtent;
      ::file::edit_file::InsertItem *     m_pinsert;
      plain_text_set_sel_command *  m_psetsel;
      bool                          m_bParseDataPacks;

      plain_edit();
      plain_edit(::aura::application * papp);
      virtual ~plain_edit();


      void plain_edit_common_construct();


      //virtual colorertake5::base_editor * colorertake5();


      virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;


      virtual void on_viewport_offset(::draw2d::graphics * pgraphics) override;

      virtual void _001DeleteSel();
      virtual bool _001DeleteSel(bool & bFullUpdate, index & iLineUpdate);

      virtual void _001ReplaceSel(const char * pszText);
      virtual bool _001ReplaceSel(const char * pszText, bool & bFullUpdate, index & iLineUpdate);

      //void on_change_view_size();

      // virtual bool get_font(::draw2d::font_sp & spfont, e_font efont, ::user::interaction * pui) override;
      void _001OnTimer(::timer * ptimer) override;

      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnRButtonDown);
      DECL_GEN_SIGNAL(_001OnRButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnKeyDown);
      DECL_GEN_SIGNAL(_001OnKeyUp);
      DECL_GEN_SIGNAL(_001OnChar);
      DECL_GEN_SIGNAL(_001OnUniChar);
      
      DECL_GEN_SIGNAL(_001OnSetFocus);
      DECL_GEN_SIGNAL(_001OnKillFocus);


      DECL_GEN_SIGNAL(_001OnUpdateEditCut);
      DECL_GEN_SIGNAL(_001OnEditCut);
      DECL_GEN_SIGNAL(_001OnUpdateEditCopy);
      DECL_GEN_SIGNAL(_001OnEditCopy);
      DECL_GEN_SIGNAL(_001OnUpdateEditPaste);
      DECL_GEN_SIGNAL(_001OnEditPaste);
      DECL_GEN_SIGNAL(_001OnUpdateEditDelete);
      DECL_GEN_SIGNAL(_001OnEditDelete);

      virtual DECL_GEN_SIGNAL(_009OnChar);

//      DECL_GEN_SIGNAL(_001OnRButtonUp);

//      DECL_GEN_SIGNAL(_001OnChar);
      DECL_GEN_SIGNAL(_001OnSysChar);

      DECL_GEN_SIGNAL(keyboard_focus_OnKeyDown) override;
      DECL_GEN_SIGNAL(keyboard_focus_OnKeyUp) override;
      DECL_GEN_SIGNAL(keyboard_focus_OnChar) override;

      virtual bool keyboard_focus_is_focusable() override;


      virtual int32_t get_wheel_scroll_delta() override;

      virtual void clipboard_copy();
      virtual void clipboard_paste();

      virtual bool get_line_color(COLORREF & crOverride, const string & strLine);

      virtual void pre_translate_message(::message::message * pobj) override;

      void key_to_char(::message::key * pkey);

      virtual void install_message_routing(::message::sender * pinterface) override;
      virtual void OnDraw(::draw2d::dib * pdib);      // overridden to draw this ::user::impact
      virtual void _001OnInitialUpdate();



      void set_plain_root(plain_text_tree * proot, bool bOwnData);

      void _001OnKeyboardFocusTimer(uint64_t iTimer);

      string get_expanded_line(index iLine, array < strsize * > intptra = array < strsize * >());

      string get_line(index iLine);
      double get_line_extent(index iLine, strsize iChar);

      virtual void _001OnAfterChangeText(::action::context actioncontext);

      virtual void _001OnUpdate(::action::context actioncontext);
      virtual void _001OnSetText(::action::context actioncontext);

      virtual bool has_action_hover() override;
      virtual bool has_text_input() override;


      virtual void on_updata_data(::data::simple_data * pdata, int32_t iHint);


      virtual bool create_control(class ::user::control_descriptor * pdescriptor) override;

      virtual strsize char_hit_test(int32_t x, int32_t y);
      virtual strsize line_char_hit_test(int32_t x, index iLine);

      colorertake5::file_type * colorer_select_type();

      virtual strsize _001GetTextLength() const override;
      virtual void _001GetText(string & str) const override;
      virtual void _001GetSelText(string & str) const override;
      virtual void _001GetText(string & str, index iBeg, index iEnd) const override;

      void _001GetViewSel(strsize &iSelStart, strsize &iSelEnd) const override;

      void _001SetText(const string & str, ::action::context actioncontext) override;
      void _001SetSelText(const char * psz, ::action::context actioncontext) override;
      void _001SetSelEnd(strsize iSelEnd) override;
      void _001SetSel(strsize iSelStart, strsize iSelEnd) override;
      void _001GetSel(strsize & iSelStart, strsize & iSelEnd) const override;

      void _001EnsureVisibleChar(strsize iChar);
      void _001EnsureVisibleLine(index iLine);

      bool should_load_full_file();

      void _001OnCalcLayout(index iLine = -1);
      void _001OnCalcOffset(index iLine = -1);
      //void _001OnCalcLayoutProc(::user::primitive * pview);

      void FileSave();
      void OnFileUpdate();
      void CreateLineIndex();
      void OnLineUpdate(index iLine, ::action::context actioncontext);
      void UpdateLineIndex(index iLine);

      index SelToColumn(strsize iSel);
      index SelToColumnX(strsize iSel, int32_t & x);
      index SelToLine(strsize iSel);
      index SelToLineX(strsize iSel, int32_t & x);
      strsize LineColumnToSel(index iLine, index iColumn);
      strsize LineXToSel(index iLine, int32_t x);
      index CharToLine(strsize iSel);

      void OneLineUp();

      void IndexRegisterDelete(strsize iSel, strsize iCount);
      void IndexRegisterInsert(strsize iSel, const char * lpcszWhat);

      virtual void MacroBegin() override;
      void MacroRecord(sp(plain_text_command) pcommand);
      virtual void MacroEnd() override;

      bool Undo();
      bool Redo();
      bool CanUndo();
      bool CanRedo();
      ::count GetRedoBranchCount();

      DECL_GEN_SIGNAL(_001OnDestroy);
      void VirtualOnSize();
      DECL_GEN_SIGNAL(_001OnPaint);
      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnContextMenu);
      DECL_GEN_SIGNAL(_001OnSetCursor);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnVScroll);
      DECL_GEN_SIGNAL(_001OnHScroll);


      virtual sp(::data::item) on_allocate_item();

      void set_root(plain_text_tree * pdata, bool bOwnData);

      virtual void on_layout() override;

      virtual bool ShowWindow(int32_t nCmdShow) override;

      virtual void on_change_viewport_offset() override;

      virtual size get_total_size() override;

      virtual var get_ex_value();

      virtual void insert_text(string str, bool bForceNewStep);

      virtual void internal_edit_update(bool bFullUpdate, index iLineUpdate);

      virtual void _001EditDelete();

      virtual void _001OnNcDraw(::draw2d::graphics * pgraphics) override;


   };


} // namespace user



