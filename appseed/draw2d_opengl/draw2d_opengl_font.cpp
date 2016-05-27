#include "framework.h"


namespace draw2d_opengl
{


   font::font(::aura::application * papp) :
      ::object(papp)
   {

      //m_pfont     = NULL;
      m_bUpdated = false;
      m_hdcFont = NULL;
      m_hfont = NULL;
      m_baseFont = 0;


   }


   font::font(const font & font) :
      ::object(font.get_app())
   {

      //m_pfont     = NULL;
      m_bUpdated = false;

      ::draw2d::font::operator = (font);

   }


   font::~font()
   {

      if (m_hdcFont != NULL)
      {

         ::DeleteDC(m_hdcFont);
         ::DeleteObject(m_hfont);
         glDeleteLists(m_baseFont, 256);

      }

   }


   void font::construct(const ::draw2d::font & fontParam)
   {
      //class font & font = const_cast < ::draw2d_opengl::font & > (dynamic_cast < const ::draw2d_opengl::font & > (fontParam));
      //if(font.m_pfont == NULL)
      //{
      //   if(m_pfont != NULL)
      //   {
      //      delete m_pfont;
      //      m_pfont = NULL;
      //   }
      //}
      //else
      //{
      //   m_pfont = font.m_pfont->Clone();
      //}
   }


   void font::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

   }




   void * font::get_os_data() const
   {

      if (m_hdcFont == NULL || !m_bUpdated)
      {
         if (m_hdcFont != NULL)
         {
            ::DeleteDC(m_hdcFont);
            ::DeleteObject(m_hfont);
            glDeleteLists(m_baseFont, 256);
         }


         //m_bFont = true;

         ((font *)this)->m_hdcFont = ::CreateCompatibleDC(NULL);

         ((font *)this)->m_baseFont = glGenLists(256);								// Storage For 256 Characters

         ((font *)this)->m_hfont = CreateFont(-m_dFontSize,							// Height Of Font
            0,								// Width Of Font
            0,								// Angle Of Escapement
            0,								// Orientation Angle
            m_iFontWeight,						// Font Weight
            m_bItalic ? TRUE : FALSE,							// Italic
            m_bUnderline ? TRUE : FALSE,							// Underline
            m_bStrikeout ? TRUE : FALSE,							// Strikeout
            ANSI_CHARSET,					// Character Set Identifier
            OUT_TT_PRECIS,					// Output Precision
            CLIP_DEFAULT_PRECIS,			// Clipping Precision
            ANTIALIASED_QUALITY,			// Output Quality
            FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
            m_strFontFamilyName);				// Font Name

         ::SelectObject(m_hdcFont, m_hfont);							// Selects The Font We Created

         wglUseFontOutlines(m_hdcFont,							// Select The Current DC
            0,								// Starting Character
            255,							// Number Of Display Lists To Build
            m_baseFont,							// Starting Display Lists
            0.0f,							// Deviation From The True Outlines
            0.2f,							// Font Thickness In The Z Direction
            WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
            ((font *)this)->m_gmf);							// Address Of Buffer To Recieve Data

                                       //::DeleteDC(hDC);

      }

      return m_hfont;

   }




   //   if(m_pfont != NULL)
   //   {
   //      ((font *) this)->m_bUpdated = true;
   //   }

   //   return (void *) (plusplus::Font *) m_pfont;

   //}

} // namespace draw2d_opengl