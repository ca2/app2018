#include "framework.h"
#include <GdiPlus.h>


void simple_ui::draw_this(HDC hdc)
{

   Gdiplus::Graphics graphics2(hdc);

   graphics2.SetCompositingMode(Gdiplus::CompositingModeSourceOver);

   //Gdiplus::Color crOut(0, 255, 255, 233);

   //Gdiplus::Color crIn(84 + 49, 255, 255, 233);

   Gdiplus::Color crOut;

   Gdiplus::Color crIn;

   if(get_plugin()->m_eschema == ::hotplugin::plugin::schema_darker)
   {

      crOut.SetValue(Gdiplus::Color::MakeARGB(0, 84, 84, 77));

      crIn.SetValue(Gdiplus::Color::MakeARGB(84 + 49, 84, 84, 77));

   }
   else
   {

      crOut.SetValue(Gdiplus::Color::MakeARGB(0, 255, 255, 233));

      crIn.SetValue(Gdiplus::Color::MakeARGB(84 + 49, 255, 255, 233));

   }

   int iBorderH = min(height(&m_rect) / 2, 49);

   Gdiplus::LinearGradientBrush br1(Gdiplus::Point(0, 0), Gdiplus::Point(0, iBorderH), crOut, crIn);

   graphics2.FillRectangle(&br1, m_rect.left, m_rect.top, width(&m_rect), iBorderH);

   Gdiplus::SolidBrush br(crIn);

   graphics2.FillRectangle(&br, m_rect.left, m_rect.top + iBorderH, width(&m_rect), height(&m_rect) - (iBorderH * 2));

   Gdiplus::LinearGradientBrush br2(Gdiplus::Point(0, m_rect.bottom - iBorderH), Gdiplus::Point(0, m_rect.bottom), crIn, crOut);

   graphics2.FillRectangle(&br2, m_rect.left, m_rect.bottom - iBorderH, width(&m_rect), iBorderH);


}


void simple_ui::draw_focus_rect(HDC hdc)
{

   Gdiplus::Graphics graphics2(hdc);

   int iStyle = 1;

   if(get_focus() == this)
   {

      if(iStyle == 1)
      {

         RECT rect;

         rect = m_rect;

         {
         
            Gdiplus::Pen pen(Gdiplus::Color(255, 108, 149, 255), 1.0f);

            graphics2.DrawRectangle(&pen, rect.left, rect.top, width(&rect), height(&rect));

         }

         rect.left--;
         rect.right++;
         rect.top--;
         rect.bottom++;

         {
         
            Gdiplus::GraphicsPath pathRound;

            Gdiplus::Rect rectRound(rect.left, rect.top, width(&rect), height(&rect));
            
            graphics_round_rect::GetRoundRectPath(&pathRound, rectRound, 1);

            Gdiplus::Pen pen(Gdiplus::Color(84, 108, 149, 255), 1.0f);

            graphics2.DrawPath(&pen, &pathRound);

         }

         rect.left--;
         rect.right++;
         rect.top--;
         rect.bottom++;

         {

            Gdiplus::GraphicsPath pathRound;

            Gdiplus::Rect rectRound(rect.left, rect.top, width(&rect), height(&rect));
            
            graphics_round_rect::GetRoundRectPath(&pathRound, rectRound, 2);

            Gdiplus::Pen pen(Gdiplus::Color(72, 108, 149, 255), 1.0f);

            graphics2.DrawPath(&pen, &pathRound);

         }

         rect.left--;
         rect.right++;
         rect.top--;
         rect.bottom++;

         {

            Gdiplus::GraphicsPath pathRound;

            Gdiplus::Rect rectRound(rect.left, rect.top, width(&rect), height(&rect));
            
            graphics_round_rect::GetRoundRectPath(&pathRound, rectRound, 3);

            Gdiplus::Pen pen(Gdiplus::Color(60, 108, 149, 255), 1.0f);

            graphics2.DrawPath(&pen, &pathRound);

         }


         rect.left--;
         rect.right++;
         rect.top--;
         rect.bottom++;

         {

            Gdiplus::GraphicsPath pathRound;

            Gdiplus::Rect rectRound(rect.left, rect.top, width(&rect), height(&rect));
            
            graphics_round_rect::GetRoundRectPath(&pathRound, rectRound, 4);

            Gdiplus::Pen pen(Gdiplus::Color(48, 108, 149, 255), 1.0f);

            graphics2.DrawPath(&pen, &pathRound);

         }



         rect.left--;
         rect.right++;
         rect.top--;
         rect.bottom++;

         {

            Gdiplus::GraphicsPath pathRound;

            Gdiplus::Rect rectRound(rect.left, rect.top, width(&rect), height(&rect));
            
            graphics_round_rect::GetRoundRectPath(&pathRound, rectRound, 5);

            Gdiplus::Pen pen(Gdiplus::Color(36, 108, 149, 255), 1.0f);

            graphics2.DrawPath(&pen, &pathRound);

         }





         rect.left--;
         rect.right++;
         rect.top--;
         rect.bottom++;

         {

            Gdiplus::GraphicsPath pathRound;

            Gdiplus::Rect rectRound(rect.left, rect.top, width(&rect), height(&rect));
            
            graphics_round_rect::GetRoundRectPath(&pathRound, rectRound, 6);

            Gdiplus::Pen pen(Gdiplus::Color(24, 108, 149, 255), 1.0f);

            graphics2.DrawPath(&pen, &pathRound);

         }


      }
      else
      {
         
         Gdiplus::Pen pen(Gdiplus::Color(255, 84, 77, 255), 3.0f);

         graphics2.DrawRectangle(&pen, m_rect.left, m_rect.top, width(&m_rect), height(&m_rect));

      }

   }
   else
   {

      Gdiplus::Pen pen(Gdiplus::Color(255, 149, 149, 123), 1.0f);

      graphics2.DrawRectangle(&pen, m_rect.left, m_rect.top, width(&m_rect), height(&m_rect));

   }


}