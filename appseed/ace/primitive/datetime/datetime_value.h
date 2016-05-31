#pragma once


namespace datetime
{


   class CLASS_DECL_ACE value
   {
   public:

      int32_t            m_iYear;
      int32_t            m_iMonth;
      int32_t            m_iDay;
      int32_t            m_iHour;
      int32_t            m_iMinute;
      int32_t            m_iSecond;
      bool           m_bSpan;

      value(const ::datetime::time & time);
      value();
      ~value();

      // ano gregoriano m�dio de 365,2425 dias solares
      double get_years() const;
      // baseado no ano gregoriano m�dio de 365,2425 dias solares * 12.0
      double get_months() const;
      double get_days() const;
      double get_hours() const;
      double get_minutes() const;
      double get_seconds() const;

      ::datetime::time get_time() const;
      ::datetime::time_span GetSpan() const;
      value & operator = (const ::datetime::time & time);

   };



} // namespace datetime


CLASS_DECL_ACE datetime::value operator + (const datetime::value & val1, const datetime::value & val2);
CLASS_DECL_ACE datetime::value operator - (const datetime::value & val1, const datetime::value & val2);
CLASS_DECL_ACE datetime::value operator * (const datetime::value & val1, double d);
CLASS_DECL_ACE datetime::value operator / (const datetime::value & val1, double d);



