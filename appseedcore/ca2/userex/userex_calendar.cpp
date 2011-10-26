#include "StdAfx.h"

calendar::calendar(::ca::application * papp) :
   ca(papp),
   m_font(papp),
   m_fontYear(papp),
   m_fontMonth(papp),
   m_fontSpin(papp),
   calendar_interface(papp)
{
   class time time = time::get_current_time();
   m_iYear = time.GetYear();
   m_iMonth = time.GetMonth();
}

void calendar::_001OnDraw(::ca::graphics * pdc)
{
   pdc->SelectObject(m_font);
   class time timeNow = time::get_current_time();
   int iMonth = m_iMonth;
   int iYear = m_iYear;
   class time time(iYear, iMonth, 1, 0, 0, 0);
   time_span timespan(1, 0, 0, 0);
   COLORREF crBorder;
   rect rectDay;
   int iDay;
   for(iDay = 1; iDay <= 7; iDay++)
   {
      GetRectDay(iDay, 0, rectDay);
      //crBorder = RGB(184, 184, 177);
      //pdc->Draw3dRect(rectDay, crBorder, crBorder);
      rectDay.deflate(m_iColWidth / 10, m_iLineHeight / 10);
      string strDay;
      strDay.Format("%s", GetTinyWeekDay(pdc->str_context(), iDay));
      pdc->DrawText(strDay, rectDay, DT_BOTTOM | DT_RIGHT);
   }
   for(iDay = 1; iDay <= 33; iDay++)
   {
      if((timeNow.GetDay() == iDay &&
         timeNow.GetMonth() == iMonth &&
         timeNow.GetYear() == iYear)
         ||
         (iDay == m_time.GetDay() &&
         iMonth == m_time.GetMonth() &&
         iYear == m_time.GetYear()))
      {
         time += timespan;
         if(time.GetMonth() != iMonth)
            break;
         continue;
      }
      GetRectDay(time, rectDay);
      crBorder = RGB(184, 184, 177);
      pdc->Draw3dRect(rectDay, crBorder, crBorder);
      rectDay.deflate(m_iColWidth / 5, m_iLineHeight / 5);
      string strDay;
      strDay.Format("%d", iDay);
      pdc->DrawText(strDay, rectDay, DT_BOTTOM | DT_RIGHT);
      time += timespan;
      if(time.GetMonth() != iMonth)
         break;
   }
   if(timeNow.GetMonth() == iMonth
   && timeNow.GetYear() == iYear)
   {
      crBorder = RGB(84, 84, 77);
      GetRectDay(timeNow, rectDay);
      rectDay.inflate(m_iColWidth / 10, m_iColWidth / 10);
      pdc->FillSolidRect(rectDay, RGB(220, 220, 210));
      pdc->Draw3dRect(rectDay, crBorder, crBorder);
      rectDay.deflate(1, 1);
      pdc->Draw3dRect(rectDay, crBorder, crBorder);
      rectDay.deflate(m_iColWidth / 5, m_iLineHeight / 5);
      string strDay;
      strDay.Format("%d", timeNow.GetDay());
      pdc->DrawText(strDay, rectDay, DT_BOTTOM | DT_RIGHT);
   }
   class time timeEmp = m_time;
   for(int iDay = timeEmp.GetDay(); time.GetYear() == iYear 
      && time.GetMonth() == iMonth &&
      (m_time.GetDay() == iDay || (
      m_bRange && time <= m_timeEnd)); time += timespan)
   {
      crBorder = RGB(240, 120, 52);
      GetRectDay(m_time, rectDay);
      rectDay.inflate(m_iColWidth / 10, m_iColWidth / 10);
      pdc->Draw3dRect(rectDay, crBorder, crBorder);
      rectDay.deflate(1, 1);
      pdc->Draw3dRect(rectDay, crBorder, crBorder);
      rectDay.deflate(m_iColWidth / 5, m_iLineHeight / 5);
      string strDay;
      strDay.Format("%d", timeEmp.GetDay());
      pdc->DrawText(strDay, rectDay, DT_BOTTOM | DT_RIGHT);
   }
   
   pdc->SelectObject(m_fontYear);
   class rect rect;
   GetRect(rect, ElementYearTitle);
   string strYear;
   strYear.Format("%d", iYear);
   pdc->DrawText(strYear, rect, DT_CENTER | DT_VCENTER);

   pdc->SelectObject(m_fontMonth);
   GetRect(rect, ElementMonthTitle);
   string strMonth;
   strMonth = GetMonth(pdc->str_context(), iMonth);
   pdc->DrawText(strMonth, rect, DT_CENTER | DT_VCENTER);

   pdc->SelectObject(m_fontSpin);
   GetRect(rect, ElementPreviousYear);
   pdc->DrawText("<<", rect, DT_CENTER | DT_VCENTER);
   GetRect(rect, ElementNextYear);
   pdc->DrawText(">>", rect, DT_CENTER | DT_VCENTER);
   GetRect(rect, ElementPreviousMonth);
   pdc->DrawText("<", rect, DT_CENTER | DT_VCENTER);
   GetRect(rect, ElementNextMonth);
   pdc->DrawText(">", rect, DT_CENTER | DT_VCENTER);
}

void calendar::_001GetHtml(::html::file * pfile)
{
   try
   {
      // pszSchema can be ignored if the object has only one implemented schema
      class time timeNow = time::get_current_time();
      int iMonth = m_iMonth;
      int iYear = m_iYear;

      int iWeekDay;
      
      class time time(iYear, iMonth, 1, 0, 0, 0);
      time_span timespan(1, 0, 0, 0);
      class time timeLastDayOfMonth((iMonth == 12) ? (iYear + 1) : iYear, (iMonth == 12) ? 1 : (iMonth + 1), 1, 0, 0, 0);
      timeLastDayOfMonth -= time_span(1, 0, 0, 0);
      int iFirstDayOfWeek = time.GetDayOfWeek();
//      int iFirstWeek;
      int iLastDayOfWeek = timeLastDayOfMonth.GetDayOfWeek();
      int iLastDayPreviousMonth = (time - time_span(1, 0, 0, 0)).GetDay();
      rect rectDay;
      int iDay;
      pfile->print("<table cellpadding=\"0\" cellspacing=\"0\">");
      if(pfile->m_strSchema.find("<no-week-bar-title>") < 0)
      {
         pfile->print("<tr>");
         if(pfile->m_strSchema.find("<left-week-of-the-year>") >= 0)
         {
            pfile->print("<td>");
            pfile->print("V");
            pfile->print("</td>");
         }
         for(iWeekDay = 1; iWeekDay <= 7; iWeekDay++)
         {
            int iDayOfWeek = iWeekDay;
            if(pfile->m_strSchema.find("<monday-first>")>=0)
            {
               iDayOfWeek++;
               if(iDayOfWeek > 7)
                  iDayOfWeek = 1;
            }
            pfile->print("<td>");
            pfile->printf("%s", GetTinyWeekDay(pfile->str_context(), iDayOfWeek));
            pfile->print("</td>");
         }
         pfile->print("</tr>");
      }


      int iDayCount = timeLastDayOfMonth.GetDay();
      int iLineCount = (iDayCount + iFirstDayOfWeek - 1) / 7;
      if(((iDayCount + iFirstDayOfWeek - 1) % 7) > 0)
         iLineCount++;

      iDay = 1;
      for(int iWeek = 1; iWeek <= iLineCount; iWeek++)
      {
         pfile->print("<tr>");
         if(pfile->m_strSchema.find("<left-week-of-the-year>") >= 0)
         {
            int w ;       if(pfile->m_strSchema.find("<monday-first>")>=0)
      {
         w = atoi(System.datetime().strftime("%V", class ::time(iYear, iMonth, iDay, 0, 0, 0).get_time()));
      }
      else
      {
         w = atoi( class ::time(iYear, iMonth, iDay, 0, 0, 0).Format("%U"));
      }

            pfile->print("<td>");
            pfile->print(gen::str::itoa(w));
            pfile->print("</td>");
         }
         for(int iWeekDay = 1; iWeekDay <=7; iWeekDay++)
         {
            int iDayOfWeek = iWeekDay;
            if(pfile->m_strSchema.find("<monday-first>")>=0)
            {
               iDayOfWeek++;
            }

            pfile->print("<td>");

            if((!m_bRange &&
               iMonth == m_time.GetMonth()
               && iYear == m_time.GetYear( )
               && iDay == m_time.GetDay()) ||
               (m_bRange &&
               class time(iYear, iMonth, iDay, 23, 59, 59) >= m_time
               && class time(iYear, iMonth, iDay, 0, 0, 0) <= m_timeEnd))
            {
               pfile->print("<div class=\""+ pfile->m_strStyle + "calendar-sel\">");
            }
            else if((timeNow.GetDay() == iDay &&
               timeNow.GetMonth() == iMonth &&
               timeNow.GetYear() == iYear)
               ||
               (iDay == m_time.GetDay() &&
               iMonth == m_time.GetMonth() &&
               iYear == m_time.GetYear()))
            {
               pfile->print("<div class=\""+ pfile->m_strStyle + "calendar-today\">");
            }
            else if(iWeek == 1 && iDayOfWeek < iFirstDayOfWeek ||
               iWeek == iLineCount && iDayOfWeek > iLastDayOfWeek)
            {
               pfile->print("<div class=\""+ pfile->m_strStyle + "calendar-out-of-month-day\">");
            }
            else
            {
               if(iDayOfWeek == 1 || iDayOfWeek == 8)
               {
                  pfile->print("<div class=\""+ pfile->m_strStyle + "calendar-sunday\">");
               }
               else
               {
                  pfile->print("<div class=\""+ pfile->m_strStyle + "calendar-day\">");
               }
            }
            if(iWeek == 1 && iDayOfWeek < iFirstDayOfWeek)
            {
               pfile->printf("%d", iLastDayPreviousMonth + iDayOfWeek - iFirstDayOfWeek + 1);
               iDay = 1;
            }
            else if(iWeek == iLineCount && iDayOfWeek > iLastDayOfWeek)
            {
               pfile->printf("%d", iDayOfWeek - iLastDayOfWeek);
            }
            else
            {
               pfile->printf("%d", iDay);
               iDay++;
            }
            pfile->print("</div></td>");
         }
         pfile->print("</tr>");
      }
      pfile->print("</table>");
   }
   catch(...)
   {
      //-----\/----- implement diagnostic print for html file;
      pfile->dprint("error calendar::_001GetHtml");
      // worst case: mal-formed calendar display
   }

}


void calendar::GetRectDay(class time & time, LPRECT lprect)
{
   int iWeekDay = time.GetDayOfWeek();
   class time timeMonth(m_iYear, m_iMonth, 1, 0, 0, 0);
   int iWeek = get_week_of_month(time);
   GetRectDay(iWeekDay, iWeek + 1, lprect);
}

void calendar::GetRectDay(int iWeekDay, int iLine, LPRECT lprect)
{
   lprect->left = m_point.x + m_iColWidth * (iWeekDay - 1);
   lprect->right = lprect->left + m_iColWidth + 1;
   lprect->top = m_point.y + m_iLineHeight * iLine;
   lprect->bottom = lprect->top + m_iLineHeight + 1;
}

void calendar::GetRect(LPRECT lprect, enum EElement eelement)
{
   if(eelement == ElementMonthTitle)
   {
      lprect->left = m_point.x + m_iColWidth * 1;
      lprect->right = lprect->left + m_iColWidth * 2 + 1;
      lprect->top = m_point.y + m_iLineHeight * 7;
      lprect->bottom = lprect->top + m_iLineHeight + 1;
   }
   else
   {
      switch(eelement)
      {
      case ElementPreviousYear:
         GetRectDay(5, 7, lprect);
         return;
      case ElementPreviousMonth:
         GetRectDay(1, 7, lprect);
         return;
      case ElementNextMonth:
         GetRectDay(4, 7, lprect);
         return;
      case ElementNextYear:
         GetRectDay(7, 7, lprect);
         return;
      case ElementYearTitle:
         GetRectDay(6, 7, lprect);
         return;
      }
   }
}



void calendar::set_pos(point pt, size sz)
{
   m_point = pt;
   m_size = sz;
   m_iColWidth = m_size.cx / 7;
   m_iLineHeight = m_size.cy / 8;
}




calendar::EElement calendar::hit_test(point pt)
{
   if(hit_test(ElementNextMonth, pt))
      return ElementNextMonth;
   if(hit_test(ElementNextYear, pt))
      return ElementNextYear;
   if(hit_test(ElementPreviousMonth, pt))
      return ElementPreviousMonth;
   if(hit_test(ElementPreviousYear, pt))
      return ElementPreviousYear;
   return ElementNull;
}

bool calendar::time_hit_test(class time & timeRet, point pt)
{
   int iMonth = m_iMonth;
   int iYear = m_iYear;
   class time time(iYear, iMonth, 1, 0, 0, 0);
   time_span timespan(1, 0, 0, 0);
   rect rectDay;
   int iDay;
   for(iDay = 1; iDay <= 33; iDay++)
   {
      GetRectDay(time, rectDay);
      if(rectDay.contains(pt))
      {
         timeRet = time;
         return true;
      }
      time += timespan;
      if(time.GetMonth() != iMonth)
         break;
   }
   return false;
}

bool calendar::hit_test(EElement eelement, point pt)
{
   class rect rect;
   GetRect(rect, eelement);
   return rect.contains(pt) != FALSE;
}