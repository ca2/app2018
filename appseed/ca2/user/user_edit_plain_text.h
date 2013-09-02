#pragma once


class keyboard_focus;
class element_2d;


namespace user
{


   class CLASS_DECL_ca2 edit_plain_text :
      virtual public scroll_view,
      public colorertake5::base_editor,
      public ::ca2::data_listener,
      public ::ca2::tree
   {
   public:


      //WPARAM                        m_dwLastKeyWparam;
      //LPARAM                        m_dwLastKeyLparam;
      ::ca2::message::key            m_keymessageLast;



      bool                          m_bKeyPressed;
      bool                          m_bColorerTake5;
      int32_t                           m_iLineCount;
      visual::dib_sp                m_dibBk;
      bool                          m_bCustomFrameBefore;
      rect                          m_FullScreenWindowRect;
      visual::fastblur              m_fastblur;

      bool                       m_bPassword;

      bool                       m_bMouseDown;
      point                      m_ptSelStart;
      bool                       m_bFocus;
      bool                       m_bCaretOn;
      uint32_t                      m_dwLastCaret;
      uint32_t                      m_dwCaretTime;
      index                      m_iLineOffset;
      stringa                    m_straSep;
      colorertake5::text_lines   m_lines;
      int32_t                        m_y;
      bool                       m_bGetTextNeedUpdate;
      bool                       m_bNeedScrollUpdate;

      strsize                    m_iSelStart;
      strsize                    m_iSelEnd;
      strsize                    m_iViewOffset; // in bytes
      strsize                    m_iViewSize; // in bytes
      int32_t                        m_iLineHeight;
      index                      m_iColumn;
      bool                       m_bMultiLine;
      bool                       m_bSendEnterKey;
      bool                       m_bReadOnly;

      // Used for whatever it can make faster for large files (scroll for example)
      // keep each line size
      index_array                m_iaLineIndex;
      // Used for whatever it can make faster for large files (scroll for example)
      // keep each line end flag 3 = \r \n     1 = \n  \r = 2
      index_array                m_iaLineEndIndex;
      count_array                m_iaCLineIndex;

      plain_text_data *          m_pdata;
      bool                       m_bOwnData;





      edit_plain_text(sp(::application) papp);

      virtual ~edit_plain_text();


      virtual void _001OnDraw(::draw2d::graphics * pdc);




      virtual DECL_GEN_SIGNAL(_002OnLButtonDown)
      virtual DECL_GEN_SIGNAL(_002OnLButtonUp)
      virtual DECL_GEN_SIGNAL(_002OnRButtonDown)
      virtual DECL_GEN_SIGNAL(_002OnRButtonUp)
      virtual DECL_GEN_SIGNAL(_002OnMouseMove)
      virtual DECL_GEN_SIGNAL(_002OnKeyDown)
      virtual DECL_GEN_SIGNAL(_002OnKeyUp)
      virtual DECL_GEN_SIGNAL(_002OnTimer)
      virtual DECL_GEN_SIGNAL(_002OnChar)

      virtual DECL_GEN_SIGNAL(_001OnUpdateEditFocusCopy)
      virtual DECL_GEN_SIGNAL(_001OnEditFocusCopy)
      virtual DECL_GEN_SIGNAL(_001OnUpdateEditFocusPaste)
      virtual DECL_GEN_SIGNAL(_001OnEditFocusPaste)

      DECL_GEN_VSIGNAL(_009OnChar)

      DECL_GEN_SIGNAL(_001OnHScroll)

      DECL_GEN_SIGNAL(_001OnRButtonUp)

      DECL_GEN_SIGNAL(_001OnChar)
      DECL_GEN_SIGNAL(_001OnSysChar)

      virtual DECL_GEN_SIGNAL(keyboard_focus_OnKeyDown)
      virtual DECL_GEN_SIGNAL(keyboard_focus_OnKeyUp)
      virtual DECL_GEN_SIGNAL(keyboard_focus_OnChar)

      virtual bool keyboard_focus_is_focusable();
      virtual bool keyboard_focus_OnSetFocus();



      void clipboard_copy();
      void clipboard_paste();



      virtual void pre_translate_message(signal_details * pobj);

      void key_to_char(::ca2::message::key * pkey);

      virtual void install_message_handling(::ca2::message::dispatch * pinterface);
      virtual void OnDraw(::draw2d::graphics * pgraphics);      // overridden to draw this ::user::view
      DECL_GEN_SIGNAL(_001OnInitialUpdate)



      static UINT ThreadProcScrollSize(LPVOID lpvoid);

      void set_plain_text_data(plain_text_data * pdata, bool bOwnData);

      void _001OnKeyboardFocusTimer(int32_t iTimer);

      virtual void _001OnAfterChangeText();

      virtual void _001OnUpdate();
      virtual void _001OnSetText();





      virtual void on_updata_data(::data * pdata, int32_t iHint);


      virtual bool create_control(class ::user::control::descriptor * pdescriptor);

      virtual strsize char_hit_test(::draw2d::graphics * pdc, int32_t x, int32_t y);

      colorertake5::file_type * colorer_select_type();

      virtual void _001GetText(string & str);
      virtual void _001GetSelText(string & str);

      void _001GetViewSel(strsize &iSelStart, strsize &iSelEnd);

      void _001SetText(const char * psz);
      void _001SetSelText(const char * psz);
      void _001SetSel(strsize iSelStart, strsize iSelEnd);

      bool should_load_full_file();

      void _001OnCalcLayout(::draw2d::graphics * pdc);
      void _001OnCalcLayoutProc(::user::elemental * pview, ::draw2d::graphics * pdc);

      void FileSave();
      void OnFileUpdate();
      void CreateLineIndex();

      index SelToColumn(strsize iSel);
      index SelToLine(strsize iSel);
      index SelToLineX(strsize iSel, int32_t & x);
      strsize LineColumnToSel(index iLine, index iColumn);
      strsize LineXToSel(index iLine, int32_t x);

      void OneLineUp();

      void IndexRegisterDelete(strsize iSel, strsize iCount);
      void IndexRegisterInsert(strsize iSel, const char * lpcszWhat);

      void MacroBegin();
      void MacroRecord(sp(plain_text_data::Command) pcommand);
      void MacroEnd();

      bool Undo();
      bool Redo();
      bool CanUndo();
      bool CanRedo();
      ::count GetRedoBranchCount();

      DECL_GEN_SIGNAL(_001OnDestroy)
      void VirtualOnSize();
      DECL_GEN_SIGNAL(_001OnPaint)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnContextMenu)
      DECL_GEN_SIGNAL(_001OnSetCursor)


      virtual sp(::ca2::tree_item_data) on_allocate_item();


   };


} // namespace user



