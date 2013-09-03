#pragma once


class CLASS_DECL_ca2 clock :
   public clock_interface
{
protected:


   point    m_point;
   size     m_size;


public:


   ::draw2d::font    m_font;
   ::draw2d::font    m_fontHour;
   ::draw2d::font    m_fontMinute;
   ::draw2d::font    m_fontSpin;


   clock(sp(base_application) papp);

   void GetRect(LPRECT lprect, enum EElement eelement);

   void _001OnDraw(::draw2d::graphics * pdc);

   void set_pos(point pt, size sz);



};