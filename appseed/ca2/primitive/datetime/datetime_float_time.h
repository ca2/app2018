#pragma once


#include <math.h>


typedef double FLOAT_DATE;

#ifndef WINDOWS

typedef struct
{

   SYSTEMTIME st;
   USHORT wDayOfYear;

} UDATE;


#define VTDATEGRE_MIN -657434 /* Minimum possible Gregorian date: 1/1/100 */
#define VTDATEGRE_MAX 2958465 /* Maximum possible Gregorian date: 31/12/9999 */


CLASS_DECL_ca2 HRESULT   VarUdateFromDate(FLOAT_DATE dateIn, ULONG dwFlags, UDATE *lpUdate);
CLASS_DECL_ca2 WINBOOL   FileTimeToLocalFileTime(const FILETIME *utcft, LPFILETIME localft );
CLASS_DECL_ca2 WINBOOL   FileTimeToSystemTime(const FILETIME * ft, LPSYSTEMTIME syst);


#endif


CLASS_DECL_ca2 int32_t       SystemTimeToFloatTime(LPSYSTEMTIME lpSt, double *pDateOut);
CLASS_DECL_ca2 int32_t       FloatTimeToSystemTime(double dateIn, LPSYSTEMTIME lpSt);
CLASS_DECL_ca2 HRESULT   FloatTimeFromStr(const char * pstr, LCID lcid, ULONG dwFlags, FLOAT_DATE * pdateOut);
CLASS_DECL_ca2 HRESULT   FloatTimeFromUdate(UDATE *pUdateIn, ULONG dwFlags, FLOAT_DATE *pDateOut);
CLASS_DECL_ca2 HRESULT   FloatTimeFromUdateEx(UDATE *pUdateIn, LCID lcid, ULONG dwFlags, FLOAT_DATE *pDateOut);


namespace datetime
{

   // Attributes
   enum FloatDateTimeStatus
   {
      error = -1,
      valid = 0,
      invalid = 1,    // Invalid span (out of range, etc.)
      null = 2,       // Literally has no value
   };


   inline WINBOOL convert_system_time_to_float_time(const SYSTEMTIME & systimeSrc, double * pVarDtTm)
   {
	   ENSURE(pVarDtTm!=::null());
	   //Convert using ::SystemTimeToVariantTime and store the result in pVarDtTm then
	   //convert variant time back to system time and compare to original system time.
	   WINBOOL ok = ::SystemTimeToFloatTime(const_cast<SYSTEMTIME*>(&systimeSrc), pVarDtTm);
	   SYSTEMTIME sysTime;
	   memset(&sysTime, 0, sizeof(SYSTEMTIME));

	   ok = ok && ::FloatTimeToSystemTime(*pVarDtTm, &sysTime);
	   ok = ok && (systimeSrc.wYear == sysTime.wYear &&
			   systimeSrc.wMonth == sysTime.wMonth &&
			   systimeSrc.wDay == sysTime.wDay &&
			   systimeSrc.wHour == sysTime.wHour &&
			   systimeSrc.wMinute == sysTime.wMinute &&
			   systimeSrc.wSecond == sysTime.wSecond);

      return ok;

   }


   class CLASS_DECL_ca2 float_time_span
   {
   private:
      static const long maxDaysInSpan  = 3615897L;
      // Constructors
   public:
      float_time_span() RELEASENOTHROW;

      float_time_span(double dblSpanSrc) RELEASENOTHROW;
      float_time_span(
         LONG lDays,
         int32_t nHours,
         int32_t nMins,
         int32_t nSecs) RELEASENOTHROW;


      double m_span;
      FloatDateTimeStatus m_status;

      void SetStatus(FloatDateTimeStatus status) RELEASENOTHROW;
      FloatDateTimeStatus GetStatus() const RELEASENOTHROW;

      double GetTotalDays() const RELEASENOTHROW;    // span in days (about -3.65e6 to 3.65e6)
      double GetTotalHours() const RELEASENOTHROW;   // span in hours (about -8.77e7 to 8.77e6)
      double GetTotalMinutes() const RELEASENOTHROW; // span in minutes (about -5.26e9 to 5.26e9)
      double GetTotalSeconds() const RELEASENOTHROW; // span in seconds (about -3.16e11 to 3.16e11)

      LONG GetDays() const RELEASENOTHROW;       // component days in span
      LONG GetHours() const RELEASENOTHROW;      // component hours in span (-23 to 23)
      LONG GetMinutes() const RELEASENOTHROW;    // component minutes in span (-59 to 59)
      LONG GetSeconds() const RELEASENOTHROW;    // component seconds in span (-59 to 59)

      // Operations
      float_time_span& operator=(double dblSpanSrc) RELEASENOTHROW;

      bool operator==(const float_time_span& dateSpan) const RELEASENOTHROW;
      bool operator!=(const float_time_span& dateSpan) const RELEASENOTHROW;
      bool operator<(const float_time_span& dateSpan) const RELEASENOTHROW;
      bool operator>(const float_time_span& dateSpan) const RELEASENOTHROW;
      bool operator<=(const float_time_span& dateSpan) const RELEASENOTHROW;
      bool operator>=(const float_time_span& dateSpan) const RELEASENOTHROW;

      // DateTimeSpan math
      float_time_span operator+(const float_time_span& dateSpan) const RELEASENOTHROW;
      float_time_span operator-(const float_time_span& dateSpan) const RELEASENOTHROW;
      float_time_span& operator+=(const float_time_span dateSpan) RELEASENOTHROW;
      float_time_span& operator-=(const float_time_span dateSpan) RELEASENOTHROW;
      float_time_span operator-() const RELEASENOTHROW;

      operator double() const RELEASENOTHROW;

      void SetDateTimeSpan(
         LONG lDays,
         int32_t nHours,
         int32_t nMins,
         int32_t nSecs) RELEASENOTHROW;

      // formatting
      string Format(LPCTSTR pFormat) const;
      string Format(UINT nID) const;

      // Implementation
      void CheckRange();

      static const double FLOAT_TIME_HALF_SECOND;
   };

   class CLASS_DECL_ca2 float_time
   {
      // Constructors
   public:
      static float_time WINAPI GetCurrentTime() RELEASENOTHROW;

      float_time() RELEASENOTHROW;

      float_time(FLOAT_DATE dtSrc) RELEASENOTHROW;
#ifndef MACOS
      float_time(__time32_t timeSrc) RELEASENOTHROW;
#endif
      float_time(__time64_t timeSrc) RELEASENOTHROW;

      float_time(const SYSTEMTIME& systimeSrc) RELEASENOTHROW;
      float_time(const FILETIME& filetimeSrc) RELEASENOTHROW;

      float_time(
         int32_t nYear,
         int32_t nMonth,
         int32_t nDay,
         int32_t nHour,
         int32_t nMin,
         int32_t nSec) RELEASENOTHROW;
      float_time(WORD wDosDate, WORD wDosTime) RELEASENOTHROW;
#ifdef _ATL_USE_WINAPI_FAMILY_DESKTOP_APP
      float_time(const DBTIMESTAMP& dbts) RELEASENOTHROW;
      _Success_(return != false) bool GetAsDBTIMESTAMP(DBTIMESTAMP& dbts) const RELEASENOTHROW;
#endif // _ATL_USE_WINAPI_FAMILY_DESKTOP_APP


      FLOAT_DATE m_dt;
      FloatDateTimeStatus m_status;

      void SetStatus(FloatDateTimeStatus status) RELEASENOTHROW;
      FloatDateTimeStatus GetStatus() const RELEASENOTHROW;

      bool GetAsSystemTime(SYSTEMTIME& sysTime) const RELEASENOTHROW;
      bool GetAsUDATE(UDATE& udate) const RELEASENOTHROW;

      int32_t GetYear() const RELEASENOTHROW;
      // Month of year (1 = January)
      int32_t GetMonth() const RELEASENOTHROW;
      // Day of month (1-31)
      int32_t GetDay() const RELEASENOTHROW;
      // Hour in day (0-23)
      int32_t GetHour() const RELEASENOTHROW;
      // Minute in hour (0-59)
      int32_t GetMinute() const RELEASENOTHROW;
      // Second in minute (0-59)
      int32_t GetSecond() const RELEASENOTHROW;
      // Day of week (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
      int32_t GetDayOfWeek() const RELEASENOTHROW;
      // Days since start of year (1 = January 1)
      int32_t GetDayOfYear() const RELEASENOTHROW;

      // Operations
      float_time& operator=(FLOAT_DATE dtSrc) RELEASENOTHROW;
#ifndef MACOS
      float_time& operator=(const __time32_t& timeSrc) RELEASENOTHROW;
#endif
      float_time& operator=(const __time64_t& timeSrc) RELEASENOTHROW;

      float_time& operator=(const SYSTEMTIME& systimeSrc) RELEASENOTHROW;
      float_time& operator=(const FILETIME& filetimeSrc) RELEASENOTHROW;
      float_time& operator=(const UDATE& udate) RELEASENOTHROW;

      bool operator==(const float_time& date) const RELEASENOTHROW;
      bool operator!=(const float_time& date) const RELEASENOTHROW;
      bool operator<(const float_time& date) const RELEASENOTHROW;
      bool operator>(const float_time& date) const RELEASENOTHROW;
      bool operator<=(const float_time& date) const RELEASENOTHROW;
      bool operator>=(const float_time& date) const RELEASENOTHROW;

      // DateTime math
      float_time operator+(float_time_span dateSpan) const RELEASENOTHROW;
      float_time operator-(float_time_span dateSpan) const RELEASENOTHROW;
      float_time& operator+=(float_time_span dateSpan) RELEASENOTHROW;
      float_time& operator-=(float_time_span dateSpan) RELEASENOTHROW;

      // DateTimeSpan math
      float_time_span operator-(const float_time& date) const RELEASENOTHROW;

      operator FLOAT_DATE() const RELEASENOTHROW;

      int32_t SetDateTime(
         int32_t nYear,
         int32_t nMonth,
         int32_t nDay,
         int32_t nHour,
         int32_t nMin,
         int32_t nSec) RELEASENOTHROW;
      int32_t SetDate(int32_t nYear, int32_t nMonth, int32_t nDay) RELEASENOTHROW;
      int32_t SetTime(int32_t nHour, int32_t nMin, int32_t nSec) RELEASENOTHROW;
#ifndef MACOS
      bool ParseDateTime(const char * lpszDate, uint32_t dwFlags = 0, LCID lcid = LANG_USER_DEFAULT) RELEASENOTHROW;
#endif
#ifdef _ATL_USE_WINAPI_FAMILY_DESKTOP_APP
      // formatting
      string Format(uint32_t dwFlags = 0, LCID lcid = LANG_USER_DEFAULT) const;
      string Format(LPCTSTR lpszFormat) const;
      string Format(UINT nFormatID) const;
#endif // _ATL_USE_WINAPI_FAMILY_DESKTOP_APP

   protected:
      static double WINAPI DoubleFromDate(FLOAT_DATE date) RELEASENOTHROW;
      static FLOAT_DATE WINAPI DateFromDouble(double f) RELEASENOTHROW;

      void CheckRange();
      WINBOOL ConvertSystemTimeToFloatTime(const SYSTEMTIME& systimeSrc);
   };



   inline float_time_span::float_time_span() RELEASENOTHROW : m_span(0), m_status(valid)
   {
   }

   inline float_time_span::float_time_span(double dblSpanSrc) RELEASENOTHROW :
   m_span(dblSpanSrc), m_status(valid)
   {
      CheckRange();
   }

   inline float_time_span::float_time_span(
      LONG lDays,
      int32_t nHours,
      int32_t nMins,
      int32_t nSecs) RELEASENOTHROW
   {
      SetDateTimeSpan(lDays, nHours, nMins, nSecs);
   }

   inline void float_time_span::SetStatus(FloatDateTimeStatus status) RELEASENOTHROW
   {
      m_status = status;
   }

   inline FloatDateTimeStatus float_time_span::GetStatus() const RELEASENOTHROW
   {
      return m_status;
   }

   inline double float_time_span::GetTotalDays() const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      return (double)LONGLONG(m_span + (m_span < 0 ?
         -FLOAT_TIME_HALF_SECOND : FLOAT_TIME_HALF_SECOND));
   }

   inline double float_time_span::GetTotalHours() const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      return (double)LONGLONG((m_span + (m_span < 0 ?
         -FLOAT_TIME_HALF_SECOND : FLOAT_TIME_HALF_SECOND)) * 24);
   }

   inline double float_time_span::GetTotalMinutes() const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      return (double)LONGLONG((m_span + (m_span < 0 ?
         -FLOAT_TIME_HALF_SECOND : FLOAT_TIME_HALF_SECOND)) * (24 * 60));
   }

   inline double float_time_span::GetTotalSeconds() const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      return (double)LONGLONG((m_span + (m_span < 0 ?
         -FLOAT_TIME_HALF_SECOND : FLOAT_TIME_HALF_SECOND)) * (24 * 60 * 60));
   }

   inline LONG float_time_span::GetDays() const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      return LONG(GetTotalDays());
   }

   inline LONG float_time_span::GetHours() const RELEASENOTHROW
   {
      double dPartialDayHours = GetTotalHours() - (GetTotalDays() * 24);
      return LONG(dPartialDayHours) % 24;
   }

   inline LONG float_time_span::GetMinutes() const RELEASENOTHROW
   {
      double dPartialHourMinutes = GetTotalMinutes() - (GetTotalHours() * 60);
      return LONG(dPartialHourMinutes) % 60;
   }

   inline LONG float_time_span::GetSeconds() const RELEASENOTHROW
   {
      double dPartialMinuteSeconds = GetTotalSeconds() - (GetTotalMinutes() * 60);
      return LONG(dPartialMinuteSeconds) % 60;
   }

   inline float_time_span& float_time_span::operator=(double dblSpanSrc) RELEASENOTHROW
   {
      m_span = dblSpanSrc;
      m_status = valid;
      CheckRange();
      return *this;
   }

   inline bool float_time_span::operator==(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      if(GetStatus() == dateSpan.GetStatus())
      {
         if(GetStatus() == valid)
         {
            // it has to be in precision range to say that it as equal
            if (m_span + FLOAT_TIME_HALF_SECOND > dateSpan.m_span &&
               m_span - FLOAT_TIME_HALF_SECOND < dateSpan.m_span)
            {
               return true;
            }
            else
            {
               return false;
            }
         }

         return (GetStatus() == null);
      }

      return false;
   }

   inline bool float_time_span::operator!=(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      return !operator==(dateSpan);
   }

   inline bool float_time_span::operator<(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      if( (GetStatus() == valid) && (GetStatus() == dateSpan.GetStatus()) )
         return m_span < dateSpan.m_span;

      return false;
   }

   inline bool float_time_span::operator>(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      if( (GetStatus() == valid) && (GetStatus() == dateSpan.GetStatus()) )
         return m_span > dateSpan.m_span ;

      return false;
   }

   inline bool float_time_span::operator<=(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      return operator<(dateSpan) || operator==(dateSpan);
   }

   inline bool float_time_span::operator>=(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      return operator>(dateSpan) || operator==(dateSpan);
   }

   inline float_time_span float_time_span::operator+(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      float_time_span dateSpanTemp;

      // If either operand Null, result Null
      if (GetStatus() == null || dateSpan.GetStatus() == null)
      {
         dateSpanTemp.SetStatus(null);
         return dateSpanTemp;
      }

      // If either operand Invalid, result Invalid
      if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
      {
         dateSpanTemp.SetStatus(invalid);
         return dateSpanTemp;
      }

      // Add spans and validate within legal range
      dateSpanTemp.m_span = m_span + dateSpan.m_span;
      dateSpanTemp.CheckRange();

      return dateSpanTemp;
   }

   inline float_time_span float_time_span::operator-(
      const float_time_span& dateSpan) const RELEASENOTHROW
   {
      float_time_span dateSpanTemp;

      // If either operand Null, result Null
      if (GetStatus() == null || dateSpan.GetStatus() == null)
      {
         dateSpanTemp.SetStatus(null);
         return dateSpanTemp;
      }

      // If either operand Invalid, result Invalid
      if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
      {
         dateSpanTemp.SetStatus(invalid);
         return dateSpanTemp;
      }

      // Subtract spans and validate within legal range
      dateSpanTemp.m_span = m_span - dateSpan.m_span;
      dateSpanTemp.CheckRange();

      return dateSpanTemp;
   }

   inline float_time_span& float_time_span::operator+=(
      const float_time_span dateSpan) RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      *this = *this + dateSpan;
      CheckRange();
      return *this;
   }

   inline float_time_span& float_time_span::operator-=(
      const float_time_span dateSpan) RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      *this = *this - dateSpan;
      CheckRange();
      return *this;
   }

   inline float_time_span float_time_span::operator-() const RELEASENOTHROW
   {
      return -this->m_span;
   }

   inline float_time_span::operator double() const RELEASENOTHROW
   {
      return m_span;
   }

   inline void float_time_span::SetDateTimeSpan(
      LONG lDays,
      int32_t nHours,
      int32_t nMins,
      int32_t nSecs) RELEASENOTHROW
   {
      // set date span by breaking into fractional days (all input ranges valid)
      m_span = lDays + ((double)nHours)/24 + ((double)nMins)/(24*60) +
         ((double)nSecs)/(24*60*60);
      m_status = valid;
      CheckRange();
   }

   inline void float_time_span::CheckRange()
   {
      if(m_span < -maxDaysInSpan || m_span > maxDaysInSpan)
      {
         m_status = invalid;
      }
   }

   /////////////////////////////////////////////////////////////////////////////
   // float_time
   /////////////////////////////////////////////////////////////////////////////

   inline float_time WINAPI float_time::GetCurrentTime() RELEASENOTHROW
   {

#ifdef WINDOWS

      return float_time(::_time64(::null()));

#else

      return float_time(::time(::null()));

#endif

   }

   inline float_time::float_time() RELEASENOTHROW :
   m_dt( 0 ), m_status(valid)
   {
   }

   inline float_time::float_time(FLOAT_DATE dtSrc) RELEASENOTHROW :
   m_dt( dtSrc ), m_status(valid)
   {
   }
#ifndef MACOS
   inline float_time::float_time(__time32_t timeSrc) RELEASENOTHROW :
   m_dt( 0 ), m_status(valid)
   {
      *this = timeSrc;
   }
#endif

   inline float_time::float_time(__time64_t timeSrc) RELEASENOTHROW :
   m_dt( 0 ), m_status(valid)
   {
      *this = timeSrc;
   }

   inline float_time::float_time(const SYSTEMTIME& systimeSrc) RELEASENOTHROW :
   m_dt( 0 ), m_status(valid)
   {
      *this = systimeSrc;
   }

   inline float_time::float_time(const FILETIME& filetimeSrc) RELEASENOTHROW :
   m_dt( 0 ), m_status(valid)
   {
      *this = filetimeSrc;
   }

   inline float_time::float_time(
      int32_t nYear,
      int32_t nMonth,
      int32_t nDay,
      int32_t nHour,
      int32_t nMin,
      int32_t nSec) RELEASENOTHROW
   {
      SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);
   }

#ifdef _ATL_USE_WINAPI_FAMILY_DESKTOP_APP
   inline float_time::float_time(
      WORD wDosDate,
      WORD wDosTime) RELEASENOTHROW
   {
      m_status = ::DosDateTimeToVariantTime(wDosDate, wDosTime, &m_dt) ?
valid : invalid;
   }
#endif // _ATL_USE_WINAPI_FAMILY_DESKTOP_APP

   inline void float_time::SetStatus(FloatDateTimeStatus status) RELEASENOTHROW
   {
      m_status = status;
   }

   inline FloatDateTimeStatus float_time::GetStatus() const RELEASENOTHROW
   {
      return m_status;
   }

   inline bool float_time::GetAsSystemTime(SYSTEMTIME& sysTime) const RELEASENOTHROW
   {
      return GetStatus() == valid && ::FloatTimeToSystemTime(m_dt, &sysTime);
   }

   inline bool float_time::GetAsUDATE(UDATE &udate) const RELEASENOTHROW
   {
      return SUCCEEDED(::VarUdateFromDate(m_dt, 0, &udate));
   }

   inline int32_t float_time::GetYear() const RELEASENOTHROW
   {
      SYSTEMTIME st;
      return GetAsSystemTime(st) ? st.wYear : error;
   }

   inline int32_t float_time::GetMonth() const RELEASENOTHROW
   {
      SYSTEMTIME st;
      return GetAsSystemTime(st) ? st.wMonth : error;
   }

   inline int32_t float_time::GetDay() const RELEASENOTHROW
   {
      SYSTEMTIME st;
      return GetAsSystemTime(st) ? st.wDay : error;
   }

   inline int32_t float_time::GetHour() const RELEASENOTHROW
   {
      SYSTEMTIME st;
      return GetAsSystemTime(st) ? st.wHour : error;
   }

   inline int32_t float_time::GetMinute() const RELEASENOTHROW
   {
      SYSTEMTIME st;
      return GetAsSystemTime(st) ? st.wMinute : error;
   }

   inline int32_t float_time::GetSecond() const RELEASENOTHROW
   {
      SYSTEMTIME st;
      return GetAsSystemTime(st) ? st.wSecond : error;
   }

   inline int32_t float_time::GetDayOfWeek() const RELEASENOTHROW
   {
      SYSTEMTIME st;
      return GetAsSystemTime(st) ? st.wDayOfWeek + 1 : error;
   }

   inline int32_t float_time::GetDayOfYear() const RELEASENOTHROW
   {
      UDATE udate;
      return GetAsUDATE(udate) ? udate.wDayOfYear : error;
   }

   inline float_time& float_time::operator=(FLOAT_DATE dtSrc) RELEASENOTHROW
   {
      m_dt = dtSrc;
      m_status = valid;
      return *this;
   }

#ifndef MACOS
   inline float_time& float_time::operator=(const __time32_t& timeSrc) RELEASENOTHROW
   {
      return operator=(static_cast<__time64_t>(timeSrc));
   }
#endif

#ifndef _ATL_USE_WINAPI_FAMILY_DESKTOP_APP
   inline bool GetAsSystemTimeHelper(const __time64_t& timeSrc, SYSTEMTIME& timeDest)
   {
      struct tm ttm;

#ifdef WINDOWS
      if (_localtime64_s(&ttm, &timeSrc) != 0)
      {
         return false;
      }

#else

      ttm = *localtime(&timeSrc);

      if(errno != 0)
      {
         return false;
      }

#endif

      timeDest.wYear = (WORD) (1900 + ttm.tm_year);
      timeDest.wMonth = (WORD) (1 + ttm.tm_mon);
      timeDest.wDayOfWeek = (WORD) ttm.tm_wday;
      timeDest.wDay = (WORD) ttm.tm_mday;
      timeDest.wHour = (WORD) ttm.tm_hour;
      timeDest.wMinute = (WORD) ttm.tm_min;
      timeDest.wSecond = (WORD) ttm.tm_sec;
      timeDest.wMilliseconds = 0;

      return true;

   }

#endif // _ATL_USE_WINAPI_FAMILY_DESKTOP_APP


   inline float_time& float_time::operator=(const __time64_t& timeSrc) RELEASENOTHROW
   {

#ifndef _ATL_USE_WINAPI_FAMILY_DESKTOP_APP

      SYSTEMTIME st;

      m_status = GetAsSystemTimeHelper(timeSrc, st) && ConvertSystemTimeToFloatTime(st) ? valid : invalid;

#else

      SYSTEMTIME st;
      CTime tmp(timeSrc);

      m_status = tmp.GetAsSystemTime(st) && ConvertSystemTimeToFloatTime(st) ? valid : invalid;

#endif // _ATL_USE_WINAPI_FAMILY_DESKTOP_APP

      return *this;

   }


   inline float_time &float_time::operator=(const SYSTEMTIME &systimeSrc) RELEASENOTHROW
   {

      m_status = ConvertSystemTimeToFloatTime(systimeSrc) ?	valid : invalid;

      return *this;

   }


   

   inline WINBOOL float_time::ConvertSystemTimeToFloatTime(const SYSTEMTIME& systimeSrc)
   {

      return convert_system_time_to_float_time(systimeSrc,&m_dt);

   }


   inline float_time &float_time::operator=(const UDATE &udate) RELEASENOTHROW
   {

      m_status = (S_OK == FloatTimeFromUdate((UDATE*)&udate, 0, &m_dt)) ? valid : invalid;

      return *this;

   }


   inline bool float_time::operator==(const float_time& date) const RELEASENOTHROW
   {

      if(GetStatus() == date.GetStatus())
      {

         if(GetStatus() == valid)
         {

            // it has to be in precision range to say that it as equal
            if (m_dt + float_time_span::FLOAT_TIME_HALF_SECOND > date.m_dt && m_dt - float_time_span::FLOAT_TIME_HALF_SECOND < date.m_dt)
            {

               return true;

            }
            else
            {

               return false;

            }
         }

         return (GetStatus() == null);

      }

      return false;

   }


   inline bool float_time::operator!=(const float_time& date) const RELEASENOTHROW
   {

      return !operator==(date);

   }

   inline bool float_time::operator<(const float_time& date) const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(date.GetStatus() == valid);
      if( (GetStatus() == valid) && (GetStatus() == date.GetStatus()) )
         return( DoubleFromDate( m_dt ) < DoubleFromDate( date.m_dt ) );

      return false;
   }

   inline bool float_time::operator>(const float_time& date) const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(date.GetStatus() == valid);
      if( (GetStatus() == valid) && (GetStatus() == date.GetStatus()) )
         return( DoubleFromDate( m_dt ) > DoubleFromDate( date.m_dt ) );

      return false;
   }

   inline bool float_time::operator<=(const float_time& date) const RELEASENOTHROW
   {
      return operator<(date) || operator==(date);
   }

   inline bool float_time::operator>=(const float_time& date) const RELEASENOTHROW
   {
      return operator>(date) || operator==(date);
   }

   inline float_time float_time::operator+(float_time_span dateSpan) const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      return( float_time( DateFromDouble( DoubleFromDate( m_dt )+(double)dateSpan ) ) );
   }

   inline float_time float_time::operator-(float_time_span dateSpan) const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      return( float_time( DateFromDouble( DoubleFromDate( m_dt )-(double)dateSpan ) ) );
   }

   inline float_time& float_time::operator+=(float_time_span dateSpan) RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      m_dt = DateFromDouble( DoubleFromDate( m_dt )+(double)dateSpan );
      return( *this );
   }

   inline float_time& float_time::operator-=(float_time_span dateSpan) RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(dateSpan.GetStatus() == valid);
      m_dt = DateFromDouble( DoubleFromDate( m_dt )-(double)dateSpan );
      return( *this );
   }

   inline float_time_span float_time::operator-(const float_time& date) const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      ASSERT(date.GetStatus() == valid);
      return DoubleFromDate(m_dt) - DoubleFromDate(date.m_dt);
   }

   inline float_time::operator FLOAT_DATE() const RELEASENOTHROW
   {
      ASSERT(GetStatus() == valid);
      return( m_dt );
   }

   inline int32_t float_time::SetDateTime(
      int32_t nYear,
      int32_t nMonth,
      int32_t nDay,
      int32_t nHour,
      int32_t nMin,
      int32_t nSec) RELEASENOTHROW
   {
      SYSTEMTIME st;
      ::memset(&st, 0, sizeof(SYSTEMTIME));

      st.wYear = WORD(nYear);
      st.wMonth = WORD(nMonth);
      st.wDay = WORD(nDay);
      st.wHour = WORD(nHour);
      st.wMinute = WORD(nMin);
      st.wSecond = WORD(nSec);

      m_status = ConvertSystemTimeToFloatTime(st) ? valid : invalid;
      return m_status;
   }

   inline int32_t float_time::SetDate(int32_t nYear, int32_t nMonth, int32_t nDay) RELEASENOTHROW
   {
      return SetDateTime(nYear, nMonth, nDay, 0, 0, 0);
   }

   inline int32_t float_time::SetTime(int32_t nHour, int32_t nMin, int32_t nSec) RELEASENOTHROW
   {
      // set date to zero date - 12/30/1899
      return SetDateTime(1899, 12, 30, nHour, nMin, nSec);
   }

   inline double WINAPI float_time::DoubleFromDate(FLOAT_DATE date) RELEASENOTHROW
   {
      // We treat it as positive from -FLOAT_TIME_HALF_SECOND because of numeric errors
      // If value is positive it doesn't need conversion
      if(date > -float_time_span::FLOAT_TIME_HALF_SECOND)
      {
         return date;
      }

      // If negative, must convert since negative dates not continuous
      // (examples: -1.25 to -.75, -1.50 to -.50, -1.75 to -.25)
      double fTemp = ceil(date);

      return fTemp - (date - fTemp);
   }

   inline FLOAT_DATE WINAPI float_time::DateFromDouble(double f) RELEASENOTHROW
   {
      // We treat it as positive from -FLOAT_TIME_HALF_SECOND because of numeric errors
      // If value is positive it doesn't need conversion
      if(f > -float_time_span::FLOAT_TIME_HALF_SECOND )
      {
         return f;
      }

      // If negative, must convert since negative dates not continuous
      // (examples: -.75 to -1.25, -.50 to -1.50, -.25 to -1.75)
      double fTemp = floor(f); // fTemp is now whole part

      return fTemp + (fTemp - f);
   }

   inline void float_time::CheckRange()
   {
      // About year 100 to about 9999
      if(m_dt > VTDATEGRE_MAX || m_dt < VTDATEGRE_MIN)
      {
         SetStatus(invalid);
      }
   }

#ifndef MACOS
   inline bool float_time::ParseDateTime(const char * lpszDate, uint32_t dwFlags, LCID lcid) RELEASENOTHROW
   {

      const char * pszDate = ( lpszDate == ::null() ) ? "" : lpszDate;

      HRESULT hr;

      if (FAILED(hr = FloatTimeFromStr(pszDate, lcid, dwFlags, &m_dt )))
      {
         if (hr == DISP_E_TYPEMISMATCH)
         {
            // Can't convert string to date, set 0 and invalidate
            m_dt = 0;
            m_status = invalid;
            return false;
         }
         else if (hr == DISP_E_OVERFLOW)
         {
            // Can't convert string to date, set -1 and invalidate
            m_dt = -1;
            m_status = invalid;
            return false;
         }
         else
         {
//            ATLTRACE(atlTraceTime, 0, _T("\nCOleDateTime VarDateFromStr call failed.\n\t"));
            // Can't convert string to date, set -1 and invalidate
            m_dt = -1;
            m_status = invalid;
            return false;
         }
      }

      m_status = valid;
      return true;
   }
   
#endif
   

#ifdef _ATL_USE_WINAPI_FAMILY_DESKTOP_APP

   inline string float_time_span::Format(LPCTSTR pFormat) const
   {
      // If null, return empty string
      if (GetStatus() == null)
         return _T("");

      CTimeSpan tmp(GetDays(), GetHours(), GetMinutes(), GetSeconds());
      return tmp.Format(pFormat);
   }

#if defined(_UNICODE) || !defined(_CSTRING_DISABLE_NARROW_WIDE_CONVERSION)
   inline string float_time::Format(
      uint32_t dwFlags,
      LCID lcid) const
   {
      // If null, return empty string
      if (GetStatus() == null)
         return _T("");

      // If invalid, return DateTime global string
      if (GetStatus() == invalid)
      {
         string str;
         if(str.LoadString(ATL_IDS_DATETIME_INVALID))
            return str;
         return szInvalidDateTime;
      }

      CComBSTR bstr;
      if (FAILED(::VarBstrFromDate(m_dt, lcid, dwFlags, &bstr)))
      {
         string str;
         if(str.LoadString(ATL_IDS_DATETIME_INVALID))
            return str;
         return szInvalidDateTime;
      }

      string tmp = string(bstr);
      return tmp;
   }
#endif

   inline string float_time::Format(LPCTSTR pFormat) const
   {
      ATLENSURE_THROW(pFormat != ::null(), E_INVALIDARG);

      // If null, return empty string
      if(GetStatus() == null)
         return _T("");

      // If invalid, return DateTime global string
      if(GetStatus() == invalid)
      {
         string str;
         if(str.LoadString(ATL_IDS_DATETIME_INVALID))
            return str;
         return szInvalidDateTime;
      }

      UDATE ud;
      if (S_OK != VarUdateFromDate(m_dt, 0, &ud))
      {
         string str;
         if(str.LoadString(ATL_IDS_DATETIME_INVALID))
            return str;
         return szInvalidDateTime;
      }

      struct tm tmTemp;
      tmTemp.tm_sec	= ud.st.wSecond;
      tmTemp.tm_min	= ud.st.wMinute;
      tmTemp.tm_hour	= ud.st.wHour;
      tmTemp.tm_mday	= ud.st.wDay;
      tmTemp.tm_mon	= ud.st.wMonth - 1;
      tmTemp.tm_year	= ud.st.wYear - 1900;
      tmTemp.tm_wday	= ud.st.wDayOfWeek;
      tmTemp.tm_yday	= ud.wDayOfYear - 1;
      tmTemp.tm_isdst	= 0;

      string strDate;
      LPTSTR lpszTemp = strDate.GetBufferSetLength(256);
      _tcsftime(lpszTemp, strDate.GetLength(), pFormat, &tmTemp);
      strDate.ReleaseBuffer();

      return strDate;
   }

   inline string float_time_span::Format(UINT nFormatID) const
   {
      string strFormat;
      if (!strFormat.LoadString(nFormatID))
         AtlThrow(E_INVALIDARG);
      return Format(strFormat);
   }

   inline string float_time::Format(UINT nFormatID) const
   {
      string strFormat;
      ATLENSURE(strFormat.LoadString(nFormatID));
      return Format(strFormat);
   }

   inline float_time::float_time(const DBTIMESTAMP& dbts)
   {
      SYSTEMTIME st;
      ::ZeroMemory(&st, sizeof(SYSTEMTIME));

      st.wYear = WORD(dbts.year);
      st.wMonth = WORD(dbts.month);
      st.wDay = WORD(dbts.day);
      st.wHour = WORD(dbts.hour);
      st.wMinute = WORD(dbts.minute);
      st.wSecond = WORD(dbts.second);

      m_status = ::SystemTimeToVariantTime(&st, &m_dt) ? valid : invalid;
   }

   inline _Success_(return != false) bool float_time::GetAsDBTIMESTAMP(DBTIMESTAMP& dbts) const
   {
      UDATE ud;
      if (S_OK != VarUdateFromDate(m_dt, 0, &ud))
         return false;

      dbts.year = (SHORT) ud.st.wYear;
      dbts.month = (USHORT) ud.st.wMonth;
      dbts.day = (USHORT) ud.st.wDay;
      dbts.hour = (USHORT) ud.st.wHour;
      dbts.minute = (USHORT) ud.st.wMinute;
      dbts.second = (USHORT) ud.st.wSecond;
      dbts.fraction = 0;

      return true;
   }

#endif // _ATL_USE_WINAPI_FAMILY_DESKTOP_APP

}	// namespace datetime
