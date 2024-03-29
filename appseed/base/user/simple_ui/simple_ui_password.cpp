#include "framework.h"// from "base/user/user.h"
//#include "base/user/user.h"
//#include "aura/user/colorertake5/colorertake5.h"



namespace simple_ui
{




   password::password(::aura::application * papp) :
      ::object(papp),
      ::user::interaction(papp),
      ::user::plain_edit(papp),
      edit_box(papp)
   {
      m_bPassword = true;
   }

   password::~password()
   {
   }




   void password::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      pgraphics->set_text_rendering_hint(::draw2d::text_rendering_hint_anti_alias);

      COLORREF crBk;
      COLORREF crBkSel;
      COLORREF crSel;
      COLORREF cr;

      cr          = _001GetColor(::user::color_text);
      crBk        = _001GetColor(::user::color_background);
      crSel       = _001GetColor(::user::color_text_selected);
      crBkSel     = _001GetColor(::user::color_background_selected);


      ::job * pjob = pgraphics->m_pjob;

      ::user::print_job * pprintjob = NULL;
      if(pjob != NULL)
         pprintjob = dynamic_cast < ::user::print_job * > (pjob);
      else
         pprintjob = NULL;


      rect rectClient;
      GetClientRect(rectClient);

      if(pprintjob != NULL)
      {
         TRACE("Print Job Is Printing page %d",pprintjob->m_iPrintingPage);
      }

      bool bCaretOn = ((get_tick_count() - m_dwFocusStartTick) % (m_dwCaretTime * 2)) < m_dwCaretTime;


      //rectClient.deflate(0, 0, 1, 1);

      //

      if(pprintjob == NULL)
      {

         ::user::interaction::_001OnDraw(pgraphics);

      }

      /*else if(!Session.savings().is_trying_to_save(::aura::resource_processing)
      && !Session.savings().is_trying_to_save(::aura::resource_blur_background))
      {

      pgraphics->blur(true, 4, rectClient);
      //class imaging & imaging = Application.imaging();
      //rect rectClient;
      //         GetWindowRect(rectClient);
      //       rectClient.offset(rectClient.top_left());
      if(rectClient.size() != m_dibBk->size())
      {
      m_dibBk->create(rectClient.size());
      m_dibBk->Fill(184, 184, 170);
      HMODULE hmodule = ::LoadLibrary("ca2performance.dll");
      ::visual::fastblur *( *pfnNew )(sp(::aura::application)) = (::visual::fastblur *(*)(sp(::aura::application))) ::GetProcAddress(hmodule, "new_fastblur");*/
      /*            m_fastblur.alloc(allocer());
      m_fastblur.initialize(rectClient.size(), 2);
      }
      if(m_fastblur.is_set() && m_fastblur->get_graphics() != NULL)
      {
      m_fastblur->get_graphics()->BitBlt(0, 0, rectClient.width(), rectClient.height(), pgraphics, 0, 0, SRCCOPY);
      m_fastblur.blur();
      imaging.bitmap_blend(
      m_fastblur->get_graphics(),
      null_point(),
      rectClient.size(),
      m_dibBk->get_graphics(),
      null_point(),
      49);
      pgraphics->from(rectClient.size(),
      m_fastblur->get_graphics(),
      null_point(),
      SRCCOPY);
      }*/
      /*}
      else
      {
      class imaging & imaging = Application.imaging();
      //_001DrawBackground(pgraphics, rectClipBox);
      imaging.color_blend(
      pgraphics,
      rectClient,
      RGB(200, 255, 255),
      127);
      }*/

      color ca;
      ca.set_rgb(RGB(227,227,210));
      ca.hls_rate(0.0,-0.33,-0.23);
//      COLORREF crBorder = ca.get_rgb() | (0xff << 24);
      //pgraphics->draw3d_rect(rectClient,crBorder,crBorder);

      if(m_ptree == NULL)
         return;

      /*if(m_iLineHeight == 0)
      {
         pgraphics->OffsetViewportOrg(-m_scrolldata.m_ptScroll.x,m_scrolldata.m_ptScroll.y);
      }
      else
      {
         pgraphics->OffsetViewportOrg(-m_scrolldata.m_ptScroll.x,-(m_scrolldata.m_ptScroll.y % m_iLineHeight));
      }*/


      ::draw2d::region_sp rgn(allocer());

      rectClient.deflate(2,2);

      //ClientToScreen(rectClient);
      rgn->create_rect(rectClient);

      double left = rectClient.left;

      //   pgraphics->SelectClipRgn(&rgn);

      if(has_focus())
      {
         m_bFocus = keyboard_focus_is_focusable();
      }
      else
      {
         m_bFocus = false;
      }

      strsize iSelStart;
      strsize iSelEnd;
      strsize lim = 0;

      ::draw2d::pen_sp penCaret(allocer());

      ::draw2d::brush_sp brushText(allocer());

      penCaret->create_solid(1.0,ARGB(255,0,0,0));


      /*   rectClient.top = m_pt.y;
      rectClient.left = m_pt.x;
      rectClient.bottom = rectClient.top + m_size.cy;
      rectClient.right = rectClient.left + m_size.cx;*/
      //      pgraphics->fill_solid_rect(rectClient, crBk);

//

      double y = rectClient.top;
      _001GetViewSel(iSelStart,iSelEnd);
      strsize iCursor = iSelEnd;
      sort::sort(iSelStart,iSelEnd);
      select_font(pgraphics, ::user::font_plain_edit);
      size size3;
      size3 = pgraphics->GetTextExtent(unitext("gGYIÁ"));
      int32_t iLineHeight = size3.cy;
//      stringa & straLines = m_plines->lines;
      stringa & straLines = m_straLines;
      stringa straLineFeed;
      string strLine;
      string str1;
      string str2;
      string str3;
      string strExtent1;
      string strExtent2;
      string strExtent3;
      index iLineStart = should_load_full_file() ? m_iLineOffset : 0;
      index iLineEnd = should_load_full_file() ? m_iLineCount - 1 : straLines.get_size() - 1;
      iLineEnd = MIN(iLineEnd,straLines.get_upper_bound());
      for(index i = iLineStart; i <= iLineEnd; i++)
      {
         straLineFeed.remove_all();
         straLineFeed.add_lines(straLines[i], false);
         if(straLineFeed.get_size() > 0)
         {
            strLine = straLineFeed[0];
         }
         else
         {
            strLine.Empty();
         }
         stringa stra;
         strsize i1 = iSelStart - lim;
         strsize i2 = iSelEnd - lim;
         strsize i3 = iCursor - lim;
         strsize iStart = MAX(0,i1);
         strsize iEnd = MIN(i2,strLine.get_length());
         ::str::fill(strLine,'*');
         str1 = strLine.Mid(0,iStart);
         str2 = strLine.Mid(iStart,iEnd - iStart);
         str3 = strLine.Mid(iEnd);
         strExtent1 = str1;
         strExtent2 = str2;
         strExtent3 = str3;
         strExtent1.replace("\t","   ");
         strExtent2.replace("\t","   ");
         strExtent3.replace("\t","   ");
         ::str::fill(strExtent1,'*');
         ::str::fill(strExtent2,'*');
         ::str::fill(strExtent3,'*');

         brushText->create_solid(cr);
         pgraphics->SelectObject(brushText);
         pgraphics->text_out(left,y,strExtent1);

         sized size1(0.0,0.0);
         pgraphics->GetTextExtent(size1,strLine,(int32_t)strLine.length(),(int32_t)iStart);
         sized sizeb(0.0,0.0);
         pgraphics->GetTextExtent(sizeb,strLine,iEnd);
         sized size2(0.0,0.0);
         pgraphics->GetTextExtent(size2,strLine,(int32_t)strLine.length(),(int32_t)iEnd);
         size2.cx -= size1.cx;

         if(iEnd > iStart)
         {
            pgraphics->fill_solid_rect_dim((int32_t)(left + size1.cx),(int32_t)y,(int32_t)size2.cx,(int32_t)size2.cy,ARGB(255,120,240,180));
            brushText->create_solid(crSel);
            pgraphics->SelectObject(brushText);
            pgraphics->text_out(left + size1.cx,y,strExtent2);
         }

         brushText->create_solid(cr);
         pgraphics->SelectObject(brushText);
         pgraphics->text_out(left + size1.cx + size2.cx,y,strExtent3);

         //maxcy = MAX(size1.cy, size2.cy);
         //maxcy = MAX(maxcy, size3.cy);
         if(m_bFocus && bCaretOn && i3 == str1.get_length())
         {
            pgraphics->SelectObject(penCaret);
            pgraphics->move_to(left + size1.cx,y);
            pgraphics->line_to(left + size1.cx,y + iLineHeight);
         }
         else if(m_bFocus && bCaretOn && i3 == (str1.get_length() + str2.get_length()))
         {
            pgraphics->SelectObject(penCaret);
            pgraphics->move_to(left + size2.cx + size1.cx,y);
            pgraphics->line_to(left + size2.cx + size1.cx,y + iLineHeight);
         }
         y += iLineHeight;
         lim += straLines[i].get_length();
         //ASSERT(FALSE);
      }

   }










} // namespace simple_ui



