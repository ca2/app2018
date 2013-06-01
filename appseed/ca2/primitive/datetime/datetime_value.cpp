#include "framework.h"


namespace datetime
{

   value span_strtotime(sp(::ca::application) pcaapp, ::user::str_context * pcontext, const char * pszSpanExpression)
   {
      static id idCalendarDay("calendar:day");
      static id idCalendarDays("calendar:days");
      static id idCalendarWeek("calendar:week");
      static id idCalendarWeeks("calendar:weeks");
      static id idCalendarYear("calendar:year");
      static id idCalendarYears("calendar:years");
      static id idCalendarHour("calendar:hour");
      static id idCalendarHours("calendar:hours");
      static id idCalendarNow("calendar:now");
      UNREFERENCED_PARAMETER(pcaapp);
      value time;
      time.m_bSpan = true;
      string str(pszSpanExpression);
      str.trim();
      str += " ";
      ::ca::property_set set;
      bool bAdd = false;
      bool bMinus = false;
      const char * psz = str;
      string strNumber;
      string strText1;
      for(int32_t i = 0;*psz; psz = ::ca::str::utf8_inc(psz))
      {
         string strChar = ::ca::str::get_utf8_char(psz);
         if(::ca::ch::is_whitespace(psz))
         {
            i++;
            //if(strNumber.has_char() && strText.has_char())
            //{
            //}
            if((bAdd || bMinus) && strNumber.has_char() && strText1.has_char())
            {
               value span;
               span.m_bSpan = true;
               if(strText1 == "day" || strText1 == "days" || strText1 == "dia" || strText1 == "dias"
                  || (pcontext != NULL && pcontext->matches(idCalendarDay, strText1))
                  || (pcontext != NULL && pcontext->matches(idCalendarDays, strText1)))
               {
                  span.m_iDay = atoi(strNumber);
               }
               else if(strText1 == "week" || strText1 == "weeks"
                  || (pcontext != NULL && pcontext->matches(idCalendarWeek, strText1))
                  || (pcontext != NULL && pcontext->matches(idCalendarWeeks, strText1)))
               {
                  span.m_iDay = atoi(strNumber) * 7;
               }
               else if(strText1 == "hour" || strText1 == "hours" || strText1 == "hora" || strText1 == "horas"
                  || (pcontext != NULL && pcontext->matches(idCalendarHour, strText1))
                  || (pcontext != NULL && pcontext->matches(idCalendarHour, strText1)))
               {
                  span.m_iHour = atoi(strNumber);
               }
               else if(strText1 == "year" || strText1 == "years"
                  || (pcontext != NULL && pcontext->matches(idCalendarYear, strText1))
                  || (pcontext != NULL && pcontext->matches(idCalendarYears, strText1)))
               {
                  span.m_iYear = atoi(strNumber);
               }
               else if(strText1 == "now"
                  || (pcontext != NULL && pcontext->matches(idCalendarNow, strText1)))
               {
                  throw "now cannot be span";
               }
               else
               {
                  throw not_implemented(pcaapp);
               }
               strNumber.Empty();
               strText1.Empty();
               if(bAdd)
               {
                  time = time + span;
                  bAdd = false;
               }
               else
               {
                  time = time - span;
                  bMinus = false;
               }
            }
            continue;
         }
         else if(strChar == "+")
         {
            if(bAdd)
            {
               Sys(pcaapp->m_psystem).log().trace("strtotime: invalid char +");
            }
            else if(bMinus)
            {
               Sys(pcaapp->m_psystem).log().trace("strtotime: invalid char + on Minus state");
            }
            bAdd = true;
            bMinus = false;
            strNumber.Empty();
         }
         else if(strChar == "-")
         {
            if(bAdd)
            {
               Sys(pcaapp->m_psystem).log().trace("strtotime: invalid char - on add state");
            }
            else if(bMinus)
            {
               Sys(pcaapp->m_psystem).log().trace("strtotime: invalid char - on Minus state");
            }
            bAdd = false;
            bMinus = true;
            strNumber.Empty();
         }
         else if(::ca::ch::is_digit(psz))
         {
            strNumber += strChar;
         }
         else if(::ca::ch::is_letter(psz))
         {
            strText1 += strChar;
         }
      }
      return time;
   }


   value strtotime(sp(::ca::application) pcaapp, ::user::str_context * pcontext, const char * psz, int32_t & iPath, int32_t & iPathCount)
   {
      ::datetime::time time;
      string str(psz);
      str.trim();
      str += " ";
      ::ca::property_set set;
      bool bBaseTime = false;
      int32_t iStart = 0;

      // if is international date time 2009-04-31 21:45:59
      if(str.get_length() >= 19)
      {
         if(str.Mid(4, 1) == "-"
         && str.Mid(7, 1) == "-"
         && str.Mid(10, 1) == " "
         && str.Mid(13, 1) == ":"
         && str.Mid(16, 1) == ":")
         {
            bBaseTime = true;
            Sys(pcaapp->m_psystem).datetime().international().parse_str(str, set);
            string strWord = str.Mid(19);
            strWord.trim_left();
            strWord = ::ca::str::get_word(strWord, " ");
            if(strWord.CompareNoCase("UTC") == 0
            || strWord.CompareNoCase("GMT") == 0)
            {
               struct tm atm;
               atm.tm_sec = set["second"];
               atm.tm_min = set["minute"];
               atm.tm_hour = set["hour"];
               atm.tm_mday = set["day"].int32();
               atm.tm_mon = set["month"].int32() - 1;        // tm_mon is 0 based
               atm.tm_year = set["year"].int32() - 1900;     // tm_year is 1900 based
               atm.tm_isdst = -1;
               /*time_t now = _time64(NULL);
               time_t nowUtc = mktime(gmtime(&now));
               time_t tDiff = difftime(nowUtc, now);*/
#ifdef WINDOWS
               time = ::datetime::time(_mkgmtime64(&atm));
#else
               time = ::datetime::time(mkgmtime_dup(&atm));
#endif
            }
            else
            {
               time = ::datetime::time(
                  set["year"],
                  set["month"],
                  set["day"],
                  set["hour"],
                  set["minute"],
                  set["second"]);
            }
            iStart = 20;
         }
      }
      // if is international date time 2009-04-31
      if(!bBaseTime && str.get_length() >= 10)
      {
         if(str.Mid(4, 1) == "-"
         && str.Mid(7, 1) == "-")
         {
            bBaseTime = true;
            Sys(pcaapp->m_psystem).datetime().international().parse_str(str, set);
            time = ::datetime::time(
               set["year"],
               set["month"],
               set["day"],
               set["hour"],
               set["minute"],
               set["second"]);
            iStart = 11;
         }
      }
      if(!bBaseTime && (
         ::ca::str::begins_eat(str, "today") ||
         (pcontext != NULL && pcontext->begins_eat(str, "calendar:today"))))
      {
         time = ::datetime::time::get_current_time();
         time = ::datetime::time(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
         bBaseTime = true;
      }
      if(!bBaseTime &&(
         ::ca::str::begins_eat(str, "tomorrow") ||
         (pcontext != NULL && pcontext->begins_eat(str, "calendar:tomorrow"))))
      {
         time = ::datetime::time::get_current_time();
         time = ::datetime::time(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
         time += ::datetime::time_span(1, 0, 0, 0);
         bBaseTime = true;
      }
      if(!bBaseTime &&(
         ::ca::str::begins_eat(str, "now") ||
         (pcontext != NULL && pcontext->begins_eat(str, "calendar:now"))))
      {
         time = ::datetime::time::get_current_time();
         bBaseTime = true;
      }
      stringa stra;
      if(!bBaseTime && cregexp_util::match(stra, str, "/^\\s*((\\d+)\\s*/\\s*(\\d+))((\\d|$)?!)/", true, 4) > 0)
      {
         time = ::datetime::time::get_current_time();
         int32_t i1 = atoi(stra[2]);
         int32_t i2 = atoi(stra[3]);
         int32_t iCount = 0;
         bool bFirst = false;
         if(i1 != i2
         && i1 >= 1 && i1 <= 12
         && i2 >= 1 && i2 <=
            Sys(pcaapp->m_psystem).datetime().get_month_day_count(time.GetYear(), i1))
         {
            bFirst = true;
            iCount++;
         }
         if(i2 >= 1 && i2 <= 12
         && i1 >= 1 && i1 <=
            Sys(pcaapp->m_psystem).datetime().get_month_day_count(time.GetYear(), i2))
         {
            iCount++;
         }
         if(iCount > 0)
         {
            if((iCount == 1 && bFirst) || (iCount == 2 && (iPath % iCount) == 0))
            {
               int32_t iDay = i2;
               int32_t iMonth = i1;
               time = ::datetime::time(time.GetYear(), iMonth, iDay,
                  time.GetHour(), time.GetMinute(), time.GetSecond());
               time = ::datetime::time(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
            }
            else if((iCount == 1 && !bFirst) || (iCount == 2 && (iPath % iCount) == 1))
            {
               int32_t iDay = i1;
               int32_t iMonth = i2;
               time = ::datetime::time(time.GetYear(), iMonth, iDay,
                  time.GetHour(), time.GetMinute(), time.GetSecond());
               time = ::datetime::time(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
            }
            iPath = iPath / iCount;
            iPathCount = iPathCount * iCount;
            bBaseTime = true;
         }
      }
      if(bBaseTime)
      {
         return value(time) + span_strtotime(pcaapp, pcontext, str.Mid(iStart));
      }
      else
      {
         return span_strtotime(pcaapp, pcontext, str.Mid(iStart));
      }
   }


   value::value(const ::datetime::time & time)
   {
      operator =(time);
   }

   value::value()
   {
      m_iYear     = 0;
      m_iMonth    = 0;

      m_iDay      = 0;
      m_iHour     = 0;
      m_iMinute   = 0;
      m_iSecond   = 0;

      m_bSpan     = false;

   }

   value::~value()
   {
   }

   double value::get_years() const
   {
      return m_iYear + m_iMonth / 12.0 +  (m_iDay + (m_iHour +  (m_iMinute + (m_iSecond / 60.0) / 60.0) / 24.0)) / 365.2425;
   }

   double value::get_months() const
   {
      return m_iYear / 12.0 + m_iMonth +  12.0 * (m_iDay + (m_iHour +  (m_iMinute + (m_iSecond / 60.0) / 60.0) / 24.0)) / 365.2425;
   }

   double value::get_days() const
   {
      return (m_iYear * 365.2425 + m_iMonth * 365.2425 / 12.0 + m_iDay) + (m_iHour +  (m_iMinute + (m_iSecond / 60.0) / 60.0) / 24.0);
   }

   double value::get_hours() const
   {
      return (m_iYear * 365.2425 + m_iMonth * 365.2425 / 12.0 + m_iDay) * 24.0 + m_iHour +  (m_iMinute + (m_iSecond / 60.0) / 60.0);
   }

   double value::get_minutes() const
   {
      return (((m_iYear * 365.2425 + m_iMonth * 365.2425 / 12.0 + m_iDay) * 24  + m_iHour) * 60.0) + m_iMinute + (m_iSecond / 60.0);
   }

   double value::get_seconds() const
   {
      return (((m_iYear * 365.2425 + m_iMonth * 365.2425 / 12.0 + m_iDay) * 24.0  + m_iHour) * 60.0 + m_iMinute) * 60.0 + m_iSecond;
   }

   ::datetime::time value::get_time() const
   {
      ASSERT(!m_bSpan);
      try
      {
         return ::datetime::time(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, m_iSecond);
      }
      catch(...)
      {
         return -1;
      }
   }

   ::datetime::time_span value::GetSpan() const
   {
      ASSERT(m_bSpan);
      return ::datetime::time_span((LONG) (m_iDay + m_iMonth * (365.0 * 4.0 + 1.0) /(12.0 *4.0) + m_iYear *(365.0 * 4.0 + 1.0) /(4.0)), m_iHour, m_iMinute, m_iSecond);
   }

   value & value::operator = (const ::datetime::time & time)
   {
      m_bSpan     = false;
      m_iYear     = time.GetYear();
      m_iMonth    = time.GetMonth();
      m_iDay      = time.GetDay();
      m_iHour     = time.GetHour();
      m_iMinute   = time.GetMinute();
      m_iSecond   = time.GetSecond();
      return *this;
   }

   string value::to_string(sp(::ca::application) pcaapp, ::user::str_context * pcontext)
   {
      string str;
      if(m_bSpan)
      {
         stringa stra;
         string strItem;
         if(m_iYear != 0)
         {
            if(abs(m_iYear) == 1)
            {
               strItem.Format("%d year", m_iYear);
            }
            else
            {
               strItem.Format("%d years", m_iYear);
            }
            stra.add(strItem);
         }
         if(m_iMonth != 0)
         {
            if(abs(m_iMonth) == 1)
            {
               strItem.Format("%d month", m_iMonth);
            }
            else
            {
               strItem.Format("%d months", m_iMonth);
            }
            stra.add(strItem);
         }
         if(m_iDay != 0)
         {
            if(abs(m_iDay) == 1)
            {
               strItem.Format("%d day", m_iDay);
            }
            else
            {
               strItem.Format("%d days", m_iDay);
            }
            stra.add(strItem);
         }
         if(m_iHour != 0)
         {
            if(abs(m_iHour) == 1)
            {
               strItem.Format("%d hour", m_iHour);
            }
            else
            {
               strItem.Format("%d hours", m_iHour);
            }
            stra.add(strItem);
         }
         if(m_iMinute != 0)
         {
            if(abs(m_iMinute) == 1)
            {
               strItem.Format("%d minute", m_iMinute);
            }
            else
            {
               strItem.Format("%d minutes", m_iMinute);
            }
            stra.add(strItem);
         }
         if(m_iSecond != 0)
         {
            if(abs(m_iSecond) == 1)
            {
               strItem.Format("%d second", m_iSecond);
            }
            else
            {
               strItem.Format("%d seconds", m_iSecond);
            }
            stra.add(strItem);
         }
         if(stra.get_count() >= 2)
         {
            str = stra.implode(", ", 0, stra.get_count() - 1) + " and " + stra.last_element();
         }
         else if(stra.get_count() == 1)
         {
            str = stra[0];
         }
      }
      else
      {
         ::datetime::time time = get_time();
         if(time.GetSecond() == 0)
         {
            if(time.GetHour() == 0 && time.GetMinute() == 0)
            {
               str = time.Format("%Y-");
               Sys(pcaapp->m_psystem).datetime().get_month_str(pcontext, time.GetMonth());
               str += time.Format("-%d");
            }
            else
            {
               str = time.Format("%Y-");
               str += Sys(pcaapp->m_psystem).datetime().get_month_str(pcontext, time.GetMonth());
               str += time.Format("-%d %H:%M");
            }
         }
         else
         {
            str = time.Format("%Y-");
            str += Sys(pcaapp->m_psystem).datetime().get_month_str(pcontext, time.GetMonth());
            str += time.Format("-%d %H:%M:%S");
         }
      }
      return str;
   }

} // namespace datetime


datetime::value operator + (const datetime::value & val1, const datetime::value & val2)
{
   datetime::value val;
   if(val1.m_bSpan && val2.m_bSpan)
   {
      val.m_bSpan = true;
      int32_t iTotalSecond = val1.m_iSecond + val2.m_iSecond;
      int32_t iTotalMinute = iTotalSecond / 60 + val1.m_iMinute + val2.m_iMinute;
      int32_t iTotalHour = iTotalMinute / 60 + val1.m_iHour + val2.m_iHour;
      int32_t iTotalDay = iTotalHour / 24 + val1.m_iDay + val2.m_iDay;
      val.m_iSecond = iTotalSecond % 60;
      val.m_iMinute = iTotalMinute % 60;
      val.m_iHour   = iTotalHour % 24;
      val.m_iDay    = iTotalDay;
      val.m_iMonth  = val1.m_iMonth + val2.m_iMonth;
      val.m_iYear   = val1.m_iYear + val2.m_iYear;
   }
   else if((val1.m_bSpan && !val2.m_bSpan) || (val2.m_bSpan && !val1.m_bSpan))
   {
      const datetime::value * pdate = NULL;
      const datetime::value * pspan = NULL;
      if(val1.m_bSpan)
      {
         pdate = &val2;
         pspan = &val1;
      }
      else
      {
         pdate = &val1;
         pspan = &val2;
      }
      val = ::datetime::time(pdate->get_time() + pspan->GetSpan());
   }
   else
   {
      throw "cannot add two dates";
   }
   return val;
}

datetime::value operator - (const datetime::value & val1, const datetime::value & val2)
{
   datetime::value val;
   if(val1.m_bSpan && val2.m_bSpan)
   {
      val.m_bSpan = true;
      int32_t iTotalSecond = val1.m_iSecond - val2.m_iSecond;
      int32_t iTotalMinute = iTotalSecond / 60 + val1.m_iMinute - val2.m_iMinute;
      int32_t iTotalHour = iTotalMinute / 60 + val1.m_iHour - val2.m_iHour;
      int32_t iTotalDay = iTotalHour / 24 + val1.m_iDay - val2.m_iDay;
      val.m_iSecond = iTotalSecond % 60;
      val.m_iMinute = iTotalMinute % 60;
      val.m_iHour   = iTotalHour % 24;
      val.m_iDay    = iTotalDay;
   }
   else if(val1.m_bSpan && !val2.m_bSpan)
   {
      throw "cannot subtract a date from a span";
   }
   else if(val2.m_bSpan && !val1.m_bSpan)
   {
      const datetime::value * pdate = &val1;
      const datetime::value * pspan = &val2;
      val = ::datetime::time(pdate->get_time() - pspan->GetSpan());
   }
   else
   {
      val.m_bSpan = true;
      int32_t iTotalSecond = val1.m_iSecond - val2.m_iSecond;
      int32_t iTotalMinute = iTotalSecond / 60 + val1.m_iMinute - val2.m_iMinute;
      int32_t iTotalHour = iTotalMinute / 60 + val1.m_iHour - val2.m_iHour;
      int32_t iTotalDay = iTotalHour / 24 + val1.m_iDay - val2.m_iDay;
      int32_t iTotalMonth = iTotalDay / 31 + val1.m_iMonth - val2.m_iMonth;
      int32_t iTotalYear = iTotalMonth / 12 + val1.m_iYear - val2.m_iYear;
      val.m_iSecond = iTotalSecond % 60;
      val.m_iMinute = iTotalMinute % 60;
      val.m_iHour   = iTotalHour % 24;
      val.m_iDay    = iTotalDay % 31;
      val.m_iMonth  = iTotalMonth % 12;
      val.m_iYear   = iTotalYear;
   }
   return val;
}

datetime::value operator * (const datetime::value & val1, double d)
{
   datetime::value val;
   if(val1.m_bSpan)
   {
      val.m_bSpan = true;
      double dTotalSecond
         = (val1.m_iSecond + (val1.m_iMinute + (val1.m_iHour + (val1.m_iDay * 24.0)) * 60.0) * 60.0) * d;
      double dTotalMinute = dTotalSecond / 60.0;
      double dTotalHour = dTotalMinute / 60.0;
      double dTotalDay = dTotalHour / 24.0;
      val.m_iSecond = (int32_t) fmod(dTotalSecond, 60.0);
      val.m_iMinute = (int32_t) fmod(dTotalMinute, 60.0);
      val.m_iHour   = (int32_t) fmod(dTotalHour, 24.0);
      val.m_iDay    = (int32_t) dTotalDay;
   }
   else
   {
      throw "cannot multiply a date";
   }
   return val;
}

datetime::value operator / (const datetime::value & val1, double d)
{
   datetime::value val;
   if(val1.m_bSpan)
   {
      val.m_bSpan = true;
      double dTotalSecond
         = (val1.m_iSecond
         + (val1.m_iMinute
         + (val1.m_iHour
         + (val1.m_iDay * 24.0)) * 60.0) * 60.0) / d;
      double dTotalMinute = dTotalSecond / 60.0;
      double dTotalHour = dTotalMinute / 60.0;
      double dTotalDay = dTotalHour / 24.0;
      val.m_iSecond = (int32_t) fmod(dTotalSecond, 60.0);
      val.m_iMinute = (int32_t) fmod(dTotalMinute, 60.0);
      val.m_iHour   = (int32_t) fmod(dTotalHour, 24.0);
      val.m_iDay    = (int32_t) dTotalDay;
   }
   else
   {
      throw "cannot multiply a date";
   }
   return val;
}
