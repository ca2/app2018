#pragma once

class CLASS_DECL_ca2 clock_interface :
   virtual public object
{
protected:


   ::datetime::time_span    m_timespan;


public:


   enum EElement
   {
      ElementNull,
      ElementPreviousHour,
      ElementPreviousMinute,
      ElementNextMinute,
      ElementNextHour,
      ElementHourTitle,
      ElementMinuteTitle,
      ElementClock,
   };


   clock_interface(sp(::application) papp);


   virtual void GetRect(LPRECT lprect, enum EElement eelement);


   virtual void set_time(const ::datetime::time & time);
   virtual void get_time(::datetime::time & time);

   virtual void _001OnDraw(::draw2d::graphics * pdc);


   virtual void previous_minute();
   virtual void next_minute();
   virtual void next_hour();
   virtual void previous_hour();

   virtual EElement hit_test(point pt);
   virtual bool hit_test(EElement eelement, point pt);
  

};