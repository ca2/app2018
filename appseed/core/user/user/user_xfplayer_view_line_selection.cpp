#include "framework.h"

XfplayerViewLineSelection::XfplayerViewLineSelection()
{
   m_estate = state_initial;

}

XfplayerViewLineSelection::~XfplayerViewLineSelection()
{

}

void XfplayerViewLineSelection::relay_event(XfplayerViewLine & viewline, signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);

   UINT message = pbase->m_uiMessage;

   if(message != WM_MOUSEMOVE
      || message != WM_TIMER
      || message != WM_LBUTTONDOWN
      || message != WM_LBUTTONUP)
   {
      return;
   }

   index iLine;
   strsize iChar;
   if((message == WM_MOUSEMOVE && GetState() == StateTracking)
      || message == WM_LBUTTONDOWN
      || message == WM_LBUTTONUP)
   {
      bool bInside;
      point ptCursor(
         LOWORD(pbase->m_lparam),  // horizontal position of cursor
         HIWORD(pbase->m_lparam));  // vertical position of cursor

      rect rectPlacement;
      viewline.GetPlacement(rectPlacement);
      bInside = rectPlacement.contains(ptCursor) != 0;
      if(!bInside && GetState() == StateTracking)
      {
         if(message == WM_LBUTTONUP
            || message == WM_MOUSEMOVE)
         {
            if(ptCursor.y < rectPlacement.top
               ||
               (ptCursor.y <= rectPlacement.bottom &&
               ptCursor.x < rectPlacement.left))
            {
               SetSelBefore(viewline);
               //                   viewline.get_interaction()->_001RedrawWindow();
            }
            else
            {
               SetSelAfter(viewline);
               //                viewline.get_interaction()->_001RedrawWindow();
            }
            if(message == WM_LBUTTONUP)
            {
               OnSelEvent(viewline, EventEnd);
            }
         }
         return;
      }
      else if(bInside)
      {
         //uint32_t fwKeys = pbase->m_wparam; // key flags
         if(message == WM_LBUTTONDOWN)
         {
            if(viewline.CalcChar(ptCursor, iChar))
            {
               iLine = viewline.m_iIndex;
               m_item.SetLineStart(iLine);
               m_item.SetCharStart(iChar);
               m_item.SetLineEnd(iLine);
               m_item.SetCharEnd(iChar);
               //                m_etype = TypeMaskStartHere;
            }
            else
            {
               m_item.SetLineStart(-1);
               m_item.SetCharStart(-1);
               m_item.SetLineEnd(-1);
               m_item.SetCharEnd(-1);
            }
            //                viewline.get_interaction()->_001RedrawWindow();
            OnSelEvent(viewline, EventStart);
            pbase->m_bRet = true;
            return;
         }
         else if(
            (message == WM_LBUTTONUP
            || message == WM_MOUSEMOVE)
            && GetState() == StateTracking)
         {
            if(viewline.CalcChar(ptCursor, iChar))
            {
               iLine = viewline.m_iIndex;
               m_item.SetLineEnd(iLine);
               m_item.SetCharEnd(iChar);
               //                m_item.NormalizeSel();
               //                m_etype |= TypeMaskEndHere;
               //viewline.get_interaction()->_001RedrawWindow();
            }
            else
            {
               if(ptCursor.x < viewline.m_iaPosition[0])
               {
                  SetSelBefore(viewline);
               }
               {
                  SetSelAfter(viewline);
               }
               //viewline.get_interaction()->_001RedrawWindow();

            }
            if(message == WM_LBUTTONUP)
            {
               OnSelEvent(viewline, EventEnd);
               if(m_iLineStartSource == m_item.GetLineStart()
                  && m_iLineEndSource == m_item.GetLineEnd()
                  && m_iCharStartSource == m_item.GetCharStart()
                  && m_iCharEndSource == m_item.GetCharEnd())
               {
                  string str;
                  point pt;
                  System.get_cursor_pos(&pt);
                  if(viewline.GetLink(str, pt))
                  {
                     //usersp(::user::impact) pview = viewline.get_interaction();
                     //pview->on_link_click(str);
                  }
               }
            }
            pbase->m_bRet = true;
            return;
         }
         else if(message == WM_MOUSEMOVE)
         {
            point pt;
            System.get_cursor_pos(&pt);
            viewline.UpdateHover(pt);
         }
      }
   }
   else if(message == WM_TIMER)
   {
      uint_ptr nIDEvent = pbase->m_wparam;
      if(nIDEvent == ::timer::ID_HOVER)
      {
         point pt;
         System.get_cursor_pos(&pt);
         viewline.UpdateHover(pt);
         if(!viewline.IsInHover())
         {
            sp(::user::interaction) pwnd = viewline.get_interaction();
            pwnd->KillTimer(::timer::ID_HOVER);
         }
      }
   }
}

void XfplayerViewLineSelection::OnSelEvent(XfplayerViewLine & viewline, XfplayerViewLineSelection::e_event eevent)
{
   UNREFERENCED_PARAMETER(viewline);
   switch(eevent)
   {
   case EventStart:
      m_iLineStartSource  = m_item.GetLineStart();
      m_iLineEndSource      = m_item.GetLineEnd();
      m_iCharStartSource   = m_item.GetCharStart();
      m_iCharEndSource      = m_item.GetCharEnd();
      m_estate               = StateTracking;

      //         viewline.GetTemplate()->OnSelStart(&viewline);
      break;
   case EventEnd:
      //NormalizeSel(viewline);
      //         m_item.NormalizeSel();
      Select(
         m_item.GetLineStart(),
         m_item.GetLineEnd(),
         m_item.GetCharStart(),
         m_item.GetCharEnd(), false);
      m_estate = state_initial;
      //      viewline.GetTemplate()->OnSelEnd(&viewline);

      break;
   default:
      ASSERT(FALSE);
   }
}

bool XfplayerViewLineSelection::SetSelBefore(XfplayerViewLine & viewline)
{
   /*   if(m_etype & TypeMaskEndHere)
   {
   if(m_etype & TypeMaskStartHere)
   {
   m_etype &= ~TypeMaskStartHere;*/
   m_item.SetLineStart(viewline.m_iIndex);
   m_item.SetCharStart(0);
   /*      }
   }
   else if(m_etype &  TypeMaskStartHere)
   {
   m_etype &= ~TypeMaskStartHere;
   m_etype |= TypeMaskEndHere;
   m_item.SetLineStart(viewline.GetFirstLine());
   m_item.SetCharStart(0);
   }
   else
   {
   string str;
   viewline.m_tokenaMain.element_at(viewline.GetLineCount() - 1).GetText(str);
   m_item.SetLineStart(viewline.GetFirstLine());
   m_item.SetCharStart(0);
   m_item.SetLineEnd(viewline.GetFirstLine() + viewline.GetLineCount() - 1);
   m_item.SetCharEnd(viewline.m_tokenaMain.last_element().GetCharCount() - 1);
   }*/
   return true;
}

bool XfplayerViewLineSelection::SetSelAfter(XfplayerViewLine & viewline)
{
   if(viewline.m_iaPosition.get_count() > 0)
   {
      m_item.SetLineEnd(viewline.m_iIndex);
      m_item.SetCharEnd(viewline.m_iaPosition.get_upper_bound());
   }

   /*   if(m_etype & TypeMaskEndHere)
   {
   if(m_etype & TypeMaskStartHere)
   {*/
   /*         string str;
   viewline.m_tokenaMain.element_at(viewline.GetLineCount() - 1).GetText(str);
   //         m_etype &= ~TypeMaskEndHere;
   m_item.SetLineEnd(viewline.GetFirstLine() + viewline.m_tokenaMain.get_size() - 1);
   m_item.SetCharEnd(str.get_length());*/
   /*      }
   else
   {
   string str;
   viewline.m_tokenaMain.element_at(viewline.GetLineCount() - 1).GetText(str);
   m_etype &= ~TypeMaskEndHere;
   m_etype |= TypeMaskStartHere;
   m_item.SetLineEnd(viewline.GetFirstLine() + viewline.m_tokenaMain.get_size() - 1);
   m_item.SetCharEnd(str.get_length() - 1);
   }
   }
   else if(m_etype &  TypeMaskStartHere)
   {
   }
   else
   {
   string str;
   viewline.m_tokenaMain.element_at(viewline.GetLineCount() - 1).GetText(str);
   m_item.SetLineStart(viewline.GetFirstLine());
   m_item.SetCharStart(0);
   m_item.SetLineEnd(viewline.GetFirstLine() + viewline.m_tokenaMain.get_size() - 1);
   m_item.SetCharEnd(str.get_length() - 1);
   }*/
   return true;
}

void XfplayerViewLineSelection::NormalizeSel(XfplayerViewLine & viewline)
{
   UNREFERENCED_PARAMETER(viewline);
   ASSERT(FALSE);
   /*   string str;
   if(m_item.m_iLineStart < 0)
   {
   }
   else if(m_item.m_iLineStart >= viewline.m_tokenaMain.get_size())
   {
   m_item.m_iLineStart = viewline.m_tokenaMain.get_size() - 1;
   if(m_item.m_iLineStart < 0)
   {
   m_item.m_iCharStart = -1;
   }
   else
   {
   m_item.m_iCharStart = 0;
   }
   }
   if(m_item.m_iLineStart < 0)
   {
   }
   else
   {
   viewline.m_tokenaMain[m_item.m_iLineStart].GetText(str);
   if(m_item.m_iCharStart >= str.get_length())
   {
   if(str.is_empty())
   {
   m_item.m_iCharStart = 0;
   }
   else
   {
   m_item.m_iCharStart = str.get_length() - 1;
   }
   }
   if(m_item.m_iLineEnd < 0)
   {
   m_item.m_iLineEnd = m_item.m_iLineStart;
   m_item.m_iCharEnd = m_item.m_iCharStart;
   }
   else if(m_item.m_iCharEnd < 0)
   {
   m_item.m_iCharEnd = m_item.m_iCharStart;
   }

   if(m_item.m_iLineEnd >= viewline.m_tokenaMain.get_size())
   {
   m_item.m_iLineEnd = viewline.m_tokenaMain.get_size() - 1;
   if(m_item.m_iLineEnd < 0)
   {
   m_item.m_iCharEnd = -1;
   }
   else
   {
   viewline.m_tokenaMain[m_item.m_iLineEnd].GetText(str);
   if(str.is_empty())
   {
   m_item.m_iCharEnd = 0;
   }
   else
   {
   m_item.m_iCharEnd = str.get_length() - 1;
   }
   }
   }
   viewline.m_tokenaMain[m_item.m_iLineEnd].GetText(str);
   if(m_item.m_iCharEnd >= str.get_length())
   {
   if(str.is_empty())
   {
   m_item.m_iCharEnd = 0;
   }
   else
   {
   m_item.m_iCharEnd = str.get_length() - 1;
   }
   }
   }*/

}

XfplayerViewLineSelection & XfplayerViewLineSelection::operator =(XfplayerViewLineSelection & selection)
{
   m_estate = selection.m_estate;
   m_etype = selection.m_etype;
   return *this;
}


XfplayerViewLineSelectionItem::XfplayerViewLineSelectionItem()
{
   m_iLineStart = -1;
   m_iLineEnd = -1;
   m_iCharStart = -1;
   m_iCharEnd = -1;
}
XfplayerViewLineSelectionItem::XfplayerViewLineSelectionItem(
   index      iLineStart,
   index      iLineEnd,
   strsize      iCharStart,
   strsize      iCharEnd
   )
{
   m_iLineStart = iLineStart;
   m_iLineEnd = iLineEnd;
   m_iCharStart = iCharStart;
   m_iCharEnd = iCharEnd;
}
XfplayerViewLineSelectionItem::~XfplayerViewLineSelectionItem()
{
}

XfplayerViewLineSelectionItem & XfplayerViewLineSelectionItem::operator =(const XfplayerViewLineSelectionItem & item)
{
   m_iLineStart   = item.m_iLineStart;
   m_iLineEnd      = item.m_iLineEnd;
   m_iCharStart   = item.m_iCharStart;
   m_iCharEnd      = item.m_iCharEnd;
   return *this;
}

void XfplayerViewLineSelection::Select(
   index      iLineStart,
   index      iLineEnd,
   strsize      iCharStart,
   strsize      iCharEnd,
   bool    bMerge)
{
   UNREFERENCED_PARAMETER(bMerge);
   for(index i = 0; i < m_itema.get_size(); i++)
   {
      if(m_itema.element_at(i).Intersect(iLineStart, iLineEnd))
      {
         m_itema.element_at(i).SetLineStart(iLineStart);
         m_itema.element_at(i).SetLineEnd(iLineEnd);
         m_itema.element_at(i).SetCharStart(iCharStart);
         m_itema.element_at(i).SetCharEnd(iCharEnd);
         return;
      }
   }
   m_itema.remove_all();
   m_itema.add(
      XfplayerViewLineSelectionItem(
      iLineStart,
      iLineEnd,
      iCharStart,
      iCharEnd));
}

bool XfplayerViewLineSelection::OnLButtonDown(XfplayerViewLine & viewline, UINT user, point pt)
{
   UNREFERENCED_PARAMETER(user);
   point pt1(pt);
   viewline.get_interaction()->ScreenToClient(&pt1);

   bool bInside;

   index iLine;
   strsize iChar;
   rect rectPlacement;
   viewline.GetPlacement(rectPlacement);
   bInside = rectPlacement.contains(pt1) != 0;
   if(!bInside && GetState() == StateTracking)
   {
      return false;
   }
   else if(bInside)
   {
      //             uint32_t fwKeys = user; // key flags
      if(viewline.CalcChar(pt1, iChar))
      {
         iLine = viewline.m_iIndex;
         m_item.SetLineStart(iLine);
         m_item.SetCharStart(iChar);
         m_item.SetLineEnd(iLine);
         m_item.SetCharEnd(iChar);
         //                m_etype = TypeMaskStartHere;
         m_estate = StateTracking;
      }
      else
      {
         m_item.SetLineStart(-1);
         m_item.SetCharStart(-1);
         m_item.SetLineEnd(-1);
         m_item.SetCharEnd(-1);
      }
      //               viewline.get_interaction()->_001RedrawWindow();
      OnSelEvent(viewline, EventStart);
      return true;
   }

   return false;
}

bool XfplayerViewLineSelection::OnMouseMove(XfplayerViewLine & viewline, UINT user, point pt)
{
   UNREFERENCED_PARAMETER(user);
   if(!viewline.IsVisible())
      return false;

   bool bInside;

   point pt1(pt);
   viewline.get_interaction()->ScreenToClient(&pt1);
   index iLine;
   strsize iChar;

   rect rectPlacement;
   viewline.GetPlacement(rectPlacement);
   bInside = rectPlacement.contains(pt1) != 0;
   if(!bInside && GetState() == StateTracking)
   {
      if(pt1.y < rectPlacement.top
         ||
         (pt1.y <= rectPlacement.bottom &&
         pt1.x < rectPlacement.left))
      {
         SetSelBefore(viewline);
         //             viewline.get_interaction()->_001RedrawWindow();
      }
      else
      {
         SetSelAfter(viewline);
         //          viewline.get_interaction()->_001RedrawWindow();
      }
      return false;
   }
   else if(bInside && GetState() == StateTracking)
   {
      //uint32_t fwKeys = user; // key flags
      if(GetState() == StateTracking)
      {
         if(viewline.m_iaPosition.get_size() <= 0)
            return false;
         if(viewline.CalcChar(pt1, iChar))
         {
            iLine = viewline.m_iIndex;
            m_item.SetLineEnd(iLine);
            m_item.SetCharEnd(iChar);
            //m_item.NormalizeSel();
            //             m_etype |= TypeMaskEndHere;
            //               viewline.get_interaction()->_001RedrawWindow();
         }
         else
         {
            if(pt1.x < viewline.m_iaPosition[0])
            {
               SetSelBefore(viewline);
            }
            {
               SetSelAfter(viewline);
            }
            //                viewline.get_interaction()->_001RedrawWindow();

         }
         //OnSelEvent(viewline, EventEnd);
         if(m_iLineStartSource == m_item.GetLineStart()
            && m_iLineEndSource == m_item.GetLineEnd()
            && m_iCharStartSource == m_item.GetCharStart()
            && m_iCharEndSource == m_item.GetCharEnd())
         {
            string str;
            if(viewline.GetLink(str, pt) == ::user::line_hit_link)
            {
               //                   usersp(::user::impact) pview = viewline.get_interaction();
               //                 pview->on_link_click(str);
            }
         }
         return true;
      }
   }

   return false;
}

bool XfplayerViewLineSelection::OnLButtonUp(XfplayerViewLine & viewline, UINT user, point pt)
{
   UNREFERENCED_PARAMETER(user);
   if(!viewline.IsVisible())
      return false;

   bool bInside;

   point pt1(pt);
   viewline.get_interaction()->ScreenToClient(&pt1);
   index iLine;
   strsize iChar;

   rect rectPlacement;
   viewline.GetPlacement(rectPlacement);
   bInside = rectPlacement.contains(pt1) != 0;
   if(!bInside && GetState() == StateTracking)
   {
      if(pt1.y < rectPlacement.top
         ||
         (pt1.y <= rectPlacement.bottom &&
         pt1.x < rectPlacement.left))
      {
         SetSelBefore(viewline);
         //             viewline.get_interaction()->_001RedrawWindow();
      }
      else
      {
         SetSelAfter(viewline);
         //          viewline.get_interaction()->_001RedrawWindow();
      }
      return false;
   }
   else if(bInside)
   {
      //uint32_t fwKeys = user; // key flags
      if(GetState() == StateTracking)
      {
         if(viewline.m_iaPosition.get_size() <= 0)
            return false;
         if(viewline.CalcChar(pt1, iChar))
         {
            iLine = viewline.m_iIndex;
            m_item.SetLineEnd(iLine);
            m_item.SetCharEnd(iChar);

            //             m_etype |= TypeMaskEndHere;
            //               viewline.get_interaction()->_001RedrawWindow();
         }
         else
         {
            if(pt1.x < viewline.m_iaPosition[0])
            {
               SetSelBefore(viewline);
            }
            {
               SetSelAfter(viewline);
            }
            //                viewline.get_interaction()->_001RedrawWindow();

         }
         OnSelEvent(viewline, EventEnd);
         if(m_iLineStartSource == m_item.GetLineStart()
            && m_iLineEndSource == m_item.GetLineEnd()
            && m_iCharStartSource == m_item.GetCharStart()
            && m_iCharEndSource == m_item.GetCharEnd())
         {
            string str;
            if(viewline.GetLink(str, pt) == ::user::line_hit_link)
            {
               //                   usersp(::user::impact) pview = viewline.get_interaction();
               //                 pview->on_link_click(str);
            }
         }
         return true;
      }
   }
   if(GetState() == StateTracking)
      m_estate = state_initial;
   return false;
}

bool XfplayerViewLineSelection::OnTimer(XfplayerViewLine & viewline, UINT user)
{
   UINT nIDEvent = user;
   if(nIDEvent == ::timer::ID_HOVER)
   {
      if(viewline.IsInHover())
      {
         point pt;
         GetCursorPos(&pt);
         viewline.get_interaction()->ScreenToClient(&pt);
         viewline.UpdateHover(pt);
         if(!viewline.IsInHover())
         {
            rect rectPlacement;
            viewline.GetPlacement(rectPlacement);
            viewline.get_interaction()->_001RedrawWindow();
         }
         //         sp(window) pwnd = viewline.GetWnd();
         //         pwnd->KillTimer(::timer::ID_HOVER);
      }
   }
   return false;
}

void XfplayerViewLineSelectionItem::NormalizeSel()
{
   if(m_iLineStart > m_iLineEnd)
   {
      ::core::swap(m_iLineEnd, m_iLineStart);
      ::core::swap(m_iCharEnd, m_iCharStart);
   }
   else if(m_iLineStart == m_iLineEnd
      && m_iCharStart > m_iCharEnd)
   {
      ::core::swap(m_iCharEnd, m_iCharStart);
   }

}




void XfplayerViewLineSelection::GetNormalSelection(index & iLineStart, strsize & iCharStart, index & iLineEnd, strsize & iCharEnd)
{
   if(m_item.GetLineStart() > m_item.GetLineEnd())
   {
      iLineStart = m_item.GetLineEnd();
      iCharStart = m_item.GetCharEnd();
      iLineEnd = m_item.GetLineStart();
      iCharEnd = m_item.GetCharStart();
   }
   else if(m_item.GetLineStart() == m_item.GetLineEnd())
   {
      iLineStart = m_item.GetLineStart();
      iLineEnd = m_item.GetLineEnd();
      if(m_item.GetCharStart() > m_item.GetCharEnd())
      {
         iCharStart = m_item.GetCharEnd();
         iCharEnd = m_item.GetCharStart();
      }
      else
      {
         iCharStart = m_item.GetCharStart();
         iCharEnd = m_item.GetCharEnd();
      }
   }
   else
   {
      iLineStart = m_item.GetLineStart();
      iCharStart = m_item.GetCharStart();
      iLineEnd = m_item.GetLineEnd();
      iCharEnd = m_item.GetCharEnd();
   }
}

index XfplayerViewLineSelectionItem::GetLineStart()
{
   return m_iLineStart;
}

index XfplayerViewLineSelectionItem::GetLineEnd()
{
   return m_iLineEnd;
}

strsize XfplayerViewLineSelectionItem::GetCharStart()
{
   return m_iCharStart;
}

strsize XfplayerViewLineSelectionItem::GetCharEnd()
{
   return m_iCharEnd;
}

void XfplayerViewLineSelectionItem::SetLineStart(index iLine)
{
   m_iLineStart = iLine;
}

void XfplayerViewLineSelectionItem::SetLineEnd(index iLine)
{
   m_iLineEnd = iLine;
}

void XfplayerViewLineSelectionItem::SetCharStart(strsize iChar)
{
   m_iCharStart = iChar;
}

void XfplayerViewLineSelectionItem::SetCharEnd(strsize iChar)
{
   m_iCharEnd = iChar;
}

bool XfplayerViewLineSelectionItem::Intersect(XfplayerViewLine &viewline)
{
   return Intersect(viewline.m_iIndex, viewline.m_iaPosition.get_upper_bound());
}



XfplayerViewLineSelection::e_state XfplayerViewLineSelection::GetState()
{
   return m_estate;
}


bool XfplayerViewLineSelectionItem::Intersect(index iFirstLine, index iLastLine)
{

   return max(iFirstLine, m_iLineStart) <= min(iLastLine, m_iLineEnd);

}

bool XfplayerViewLineSelection::get_item(XfplayerViewLineSelectionItem &item, XfplayerViewLine &viewline)
{
   if(m_item.Intersect(viewline))
   {
      item = m_item;
      item.NormalizeSel();
      return true;
   }
   for(index iItem = 0; iItem < m_itema.get_size(); iItem++)
   {
      XfplayerViewLineSelectionItem & itemTest = m_itema.element_at(iItem);
      if(itemTest.Intersect(viewline))
      {
         item = itemTest;
         item.NormalizeSel();
         return true;
      }

   }
   return false;
}
