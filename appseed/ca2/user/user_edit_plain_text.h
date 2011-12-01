#pragma once

class keyboard_focus;
class element_2d;

namespace user
{

   class CLASS_DECL_ca edit_plain_text :
      virtual public scroll_view,
      virtual public text_interface,
      virtual public colorertake5::base_editor,
      virtual public ex1::tree,
      virtual public ca::data_listener
   {
   public:


      DWORD                      m_dwLastKeyWparam;
      DWORD                      m_dwLastKeyLparam;


      bool                       m_bKeyPressed;
      bool                       m_bColorerTake5;
      int                        m_iLineCount;
      visual::dib_sp             m_dibBk;
      bool                       m_bCustomFrameBefore;
      rect                       m_FullScreenWindowRect;
      visual::fastblur           m_fastblur;

      bool                       m_bPassword;

      bool                        m_bMouseDown;
      point                        m_ptSelStart;
      bool                       m_bFocus;
      bool                       m_bCaretOn;
      DWORD                      m_dwLastCaret;
      DWORD                      m_dwCaretTime;
      int                        m_iLineOffset;
      stringa                    m_straSep;
      colorertake5::text_lines   m_lines;
      int                        m_y;
      bool                       m_bGetTextNeedUpdate;
      bool                       m_bNeedScrollUpdate;

      int                        m_iSelStart;
      int                        m_iSelEnd;
      int                        m_iViewOffset; // in bytes
      int                        m_iViewSize; // in bytes
      int                        m_iLineHeight;
      int                        m_iColumn;
      bool                       m_bMultiLine;
      bool                       m_bReadOnly;

      // Used for whatever it can make faster for large files (scroll for example)
      // keep each line size
      int_array                  m_iaLineIndex; 
      // Used for whatever it can make faster for large files (scroll for example)
      // keep each line end flag 3 = \r \n     1 = \n  \r = 2
      int_array                  m_iaLineEndIndex;
      int_array                  m_iaCLineIndex; 
      
      plain_text_data *          m_pdata;
      bool                       m_bOwnData;





      edit_plain_text(::ca::application * papp);

      virtual ~edit_plain_text();


      virtual void _001OnDraw(::ca::graphics * pdc);




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



      virtual void pre_translate_message(gen::signal_object * pobj);

      void key_to_char(WPARAM wparam, LPARAM lparam);

      virtual void install_message_handling(::gen::message::dispatch * pinterface);
      virtual void OnDraw(::ca::graphics * pgraphics);      // overridden to draw this ::view
      DECL_GEN_SIGNAL(_001OnInitialUpdate)



      static UINT ThreadProcScrollSize(LPVOID lpvoid);

      void set_plain_text_data(plain_text_data * pdata, bool bOwnData);

      void _001OnKeyboardFocusTimer(int iTimer);

      virtual void _001OnAfterChangeText();

      virtual void _001OnUpdate();
      virtual void _001OnSetText();


      void MacroBegin();
      void MacroRecord(plain_text_data::Command * pcommand);
      void MacroEnd();



      virtual void on_updata_data(::ca::data * pdata, int iHint);


      virtual bool create_control(class user::control::descriptor * pdescriptor);

      virtual int char_hit_test(::ca::graphics * pdc, int x, int y);

      colorertake5::file_type * colorer_select_type();

      void _001GetText(string & str) const; 
      void _001GetSelText(string & str) const;

      void _001GetViewSel(int &iSelStart, int &iSelEnd);

      void _001SetText(const char * psz);
      void _001SetSelText(const char * psz);
      void _001SetSel(int iSelStart, int iSelEnd);

      bool should_load_full_file();

      void _001OnCalcLayout(::ca::graphics * pdc);
      void _001OnCalcLayoutProc(user::elemental * pview, ::ca::graphics * pdc);

      void FileSave();
      void OnFileUpdate();
      void CreateLineIndex();

      int SelToColumn(int iSel);
      int SelToLine(int iSel);
      int LineColumnToSel(int iLine, int iColumn);

      void OneLineUp();

      void IndexRegisterDelete(int iSel, int iCount);
      void IndexRegisterInsert(int iSel, const char * lpcszWhat);

      bool Undo();
      bool Redo();
      bool CanUndo();
      bool CanRedo();
      int GetRedoBranchCount();

      DECL_GEN_SIGNAL(_001OnDestroy)
      void VirtualOnSize();
      DECL_GEN_SIGNAL(_001OnPaint)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnContextMenu)
      DECL_GEN_SIGNAL(_001OnSetCursor)

      virtual ex1::tree_item_data * on_allocate_item();
      virtual void on_delete_item(ex1::tree_item_data * pitem);

   };

} // namespace user