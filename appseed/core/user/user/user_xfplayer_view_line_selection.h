#pragma once

class xfplayer_view_line;

class CLASS_DECL_CORE XfplayerViewLineSelectionItem  
{
protected:

   index               m_iLineStart;
   index               m_iLineEnd;
   strsize               m_iCharStart;
   strsize               m_iCharEnd;

public:
   index GetLineStart();
   index GetLineEnd();
   index GetCharStart();
   index GetCharEnd();
   void SetLineStart(index iLine);
   void SetLineEnd(index iLine);
   void SetCharStart(strsize iChar);
   void SetCharEnd(index iChar);



      // Construction / Destruction
public:
   void NormalizeSel();
   bool Intersect(xfplayer_view_line & viewline);
   bool Intersect(index iLineStart, index iLineEnd);
   XfplayerViewLineSelectionItem();
   XfplayerViewLineSelectionItem(
      index      iLineStart,
      index      iLineEnd,
      strsize      iCharStart,
      strsize      iCharEnd
      );
   virtual ~XfplayerViewLineSelectionItem();

   XfplayerViewLineSelectionItem & operator =(const XfplayerViewLineSelectionItem & item);
};


class CLASS_DECL_CORE XfplayerViewLineSelection :
   virtual object
{
public:
   enum e_state
   {
      state_initial = 1,
      StateTracking = 2
   };
      
   enum e_event
   {
      EventStart = 1,
      EventEnd = 2
   };
      
   enum ETypeMask
   {
//      TypeMaskStartHere = 1,
//      TypeMaskEndHere = 2,
   };

public:
   array < XfplayerViewLineSelectionItem, XfplayerViewLineSelectionItem> m_itema;

   XfplayerViewLineSelectionItem m_item;
      

   index               m_iLineStartSource;
   index               m_iLineEndSource;
   strsize               m_iCharStartSource;
   strsize               m_iCharEndSource;

   int32_t               m_etype; 
   e_state            m_estate;

      // Conxtruction / Destruction
public:
   bool get_item(XfplayerViewLineSelectionItem & item, xfplayer_view_line & viewline);
   e_state GetState();
   XfplayerViewLineSelection();
   virtual ~XfplayerViewLineSelection();


   virtual void relay_event(xfplayer_view_line & viewline, signal_details * pobj);
   virtual void OnSelEvent(xfplayer_view_line & viewline, e_event eselevent);
   bool SetSelBefore(xfplayer_view_line & viewline);
   bool SetSelAfter(xfplayer_view_line & viewline);
   void NormalizeSel(xfplayer_view_line & viewline);
   void GetNormalSelection(index & iLineStart, strsize & iCharStart, index & iLineEnd, strsize & iCharEnd);

   void Select(
      index      iLineStart,
      index      iLineEnd,
      strsize      iCharStart,
      strsize      iCharEnd,
      bool   bMerge);



   XfplayerViewLineSelection & operator =(XfplayerViewLineSelection & selection);

   bool OnMouseMove(xfplayer_view_line & viewline, UINT user, point pt);
   bool OnLButtonDown(xfplayer_view_line & viewline, UINT user, point pt);
   bool OnLButtonUp(xfplayer_view_line & viewline, UINT user, point pt);
   bool OnTimer(xfplayer_view_line & viewline, UINT user);

};

