﻿#include "framework.h"



namespace draw2d
{


   font::enum_item::enum_item() :
      m_ecs(cs_none)
   {

   }


   font::enum_item::enum_item(string strName, e_cs ecs) :
      m_strFile(strName),
      m_strName(strName),
      m_ecs(ecs)
   {

   }


   font::enum_item::enum_item(string strFile, string strName, e_cs ecs) :
      m_strFile(strFile),
      m_strName(strName),
      m_ecs(ecs)
   {

   }


   font::enum_item::~enum_item()
   {

   }


   font::font()
   {

      m_strFontFamilyName  = FONT_SERIF;
      m_dFontSize          = 17.0;
      m_dFontWidth         = 1.0;
      m_eunitFontSize      = ::draw2d::unit_point;
      m_iFontWeight        = 400;
      m_bItalic            = false;
      m_bUnderline         = false;
      m_bStrikeout         = false;
      m_ecs                = cs_none;
      m_etextrendering = text_rendering_undefined;
   }

   font::font(const font & font)
   {

      operator = (font);
   }

   font::~font()
   {

   }

#ifdef DEBUG


   void font::dump(dump_context & dumpcontext) const
   {
      UNREFERENCED_PARAMETER(dumpcontext);
      ::exception::throw_interface_only(get_app());
   }

#endif
   bool font::create_pixel_font(const char * lpszFacename, double dSize, int32_t iWeight, bool bItalic, bool bUnderline, bool bStrikeOut, double dWidth)
   {

      synch_lock sl(m_pmutex);

#ifdef WINDOWS
      if(stricmp(lpszFacename, FONT_SANS) == 0)
      {
         m_strFontFamilyName = FONT_SANS;
      }
      else
      {
         m_strFontFamilyName     = lpszFacename;
      }
#else
      m_strFontFamilyName     = lpszFacename;

#endif

      m_dFontSize             = dSize;
      m_dFontWidth            = 1.0;
      m_eunitFontSize         = ::draw2d::unit_pixel;
      m_iFontWeight           = iWeight;
      m_bItalic               = bItalic;
      m_bUnderline            = bUnderline;
      m_bStrikeout            = bStrikeOut;
      m_bUpdated              = false;

      return true;

   }

   bool font::create_point_font(const char * lpszFacename, double dSize, int32_t iWeight, bool bItalic, bool bUnderline, bool bStrikeOut, double dWidth)
   {

      synch_lock sl(m_pmutex);

      m_strFontFamilyName     = lpszFacename;
#ifdef MACOS
      m_dFontSize             = dSize * 1.2;
#elif defined(VSNORD)
      m_dFontSize             = dSize * 1.6;
#else
      m_dFontSize             = dSize;
#endif
      m_dFontWidth            = 1.0;
      m_eunitFontSize         = ::draw2d::unit_point;
      m_iFontWeight           = iWeight;
      m_bItalic               = bItalic;
      m_bUnderline            = bUnderline;
      m_bStrikeout            = bStrikeOut;
      m_bUpdated              = false;

      return true;

   }


   font & font::operator = (const font & fontSrc)
   {

      synch_lock sl(m_pmutex);

      if(this != &fontSrc)
      {
         m_strFontFamilyName     = fontSrc.m_strFontFamilyName;
         m_dFontSize             = fontSrc.m_dFontSize;
         m_dFontWidth            = fontSrc.m_dFontWidth;
         m_eunitFontSize         = fontSrc.m_eunitFontSize;
         m_iFontWeight           = fontSrc.m_iFontWeight;
         m_bItalic               = fontSrc.m_bItalic;
         m_bUnderline            = fontSrc.m_bUnderline;
         m_bStrikeout            = fontSrc.m_bStrikeout;
         m_bUpdated              = false;
      }

      return *this;

   }


   void font::set_family_name(const char * pszFamilyName)
   {

      synch_lock sl(m_pmutex);

      m_strFontFamilyName  = pszFamilyName;
      m_bUpdated           = false;


   }

   void font::set_size(double dSize, e_unit eunit)
   {

      synch_lock sl(m_pmutex);

      m_dFontSize       = dSize;
      m_eunitFontSize   = eunit;
      m_bUpdated        = false;


   }

   void font::set_bold(bool bBold)
   {

      synch_lock sl(m_pmutex);

      if(bBold)
      {
         m_iFontWeight  = 700;
      }
      else
      {
         m_iFontWeight  = 400;
      }

      m_bUpdated     = false;

   }

   void font::set_italic(bool bItalic)
   {

      synch_lock sl(m_pmutex);

      m_bItalic      = bItalic;
      m_bUpdated     = false;

   }

   void font::set_underline(bool bUnderline)
   {

      synch_lock sl(m_pmutex);

      m_bUnderline   = bUnderline;
      m_bUpdated     = false;

   }

   void font::set_strikeout(bool bStrikeout)
   {


      synch_lock sl(m_pmutex);

      m_bStrikeout   = bStrikeout;
      m_bUpdated     = false;

   }


   font::e_cs font::get_cs(graphics * pgraphics)
   {

      if (m_ecs == cs_none)
      {

         m_ecs = calc_cs(pgraphics);

      }

      return m_ecs;

   }


   font::e_cs font::calc_cs(graphics * pgraphics)
   {

      return cs_default;

   }


   string font::get_sample_text(e_cs ecs)
   {

      if (ecs == cs_chinesebig5)
      {

         return unitext("示例文本");

      }
      else if (ecs == cs_gb2312)
      {

         return unitext("示例文本");

      }
      else if (ecs == cs_shiftjis)
      {

         return unitext("サンプルテキスト");

      }
      else if (ecs == cs_hebrew)
      {

         return unitext("טקסט לדוגמה");

      }
      else if (ecs == cs_arabic)
      {

         return unitext("نص بسيط");

      }
      else if (ecs == font::cs_greek)
      {

         return unitext("Δείγμα κειμένου");

      }
      else if (ecs == font::cs_turkish)
      {

         return unitext("Örnek yazı");

      }
      else if (ecs == font::cs_vietnamese)
      {

         return unitext("văn bản mẫu");

      }
      else if (ecs == font::cs_thai)
      {

         return unitext("ตัวอย่างข้อความ");

      }
      else if (ecs == font::cs_easteurope)
      {

         return unitext("Sample Text");

      }
      else if (ecs == font::cs_russian)
      {

         return unitext("Образец текста");

      }
      else if (ecs == font::cs_johab)
      {

         return unitext("샘플 텍스트");

      }
      else if (ecs == font::cs_hangul)
      {

         return unitext("샘플 텍스트");

      }

      return "";

   }


   string font::get_sample_text(::draw2d::graphics * pgraphics)
   {

      auto ecs = get_cs(pgraphics);

      string str = get_sample_text(ecs);

      if (str.has_char())
      {

         return str;

      }

      return m_strFontFamilyName;

   }



} // namespace draw2d


