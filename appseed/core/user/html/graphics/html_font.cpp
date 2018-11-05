#include "framework.h"

namespace html
{

   font::font()
   {
   }

   font::font(const class font & font)
   {
      m_strFamily             = font.m_strFamily;
      m_strSize               = font.m_strSize;
      m_strWeight             = font.m_strWeight;
      m_strTextDecoration     = font.m_strTextDecoration;
   }


   bool font::create(::aura::application * papp)
   {

      set_app(papp);

      m_font.alloc(allocer());

      int32_t iWeight;

      m_strWeight.trim();

      if(m_strWeight.compare_ci("bold") == 0)
      {
         iWeight = 800;
      }
      else if (m_strWeight.is_empty())
      {
         iWeight = 400;
      }
      else
      {
         iWeight = MAX(100, atoi(m_strWeight));
      }

      double dSize;

      m_strSize.trim();

      if(m_strSize.is_empty())
      {
         dSize = 11;
      }
      else
      {
         dSize = strtod(m_strSize, NULL);
      }



      stringa stra;
      stra.add_tokens(m_strFamily,",", false);
      for(int32_t i = 0; i < stra.get_size(); i++)
      {
         stra[i].trim();
         if(stra[i].compare_ci("fixed-width") == 0)
         {
         }
         else if(stra[i].compare_ci("sans-serif") == 0)
         {
         }
         else if(stra[i].compare_ci("serif") == 0)
         {
         }
      }
//      strsize iLen;
      string strName;
      if(stra.get_count() > 0)
      {
         strName = stra[0];
//         iLen = MIN(sizeof(lf.lfFaceName) - 1, stra[0].get_length());
  //       strncpy(lf.lfFaceName, stra[0], iLen);
    //     lf.lfFaceName[iLen] = '\0';
         strName.trim();
      }
      else
      {
         strName = FONT_SANS;
      }


      bool bUnderline = ::str::find_ci("underline", m_strTextDecoration) >= 0;

      m_font->create_point_font(strName, dSize, iWeight, false, bUnderline);

      return true;
   }

   bool font::operator == (const font & font)
   {
      return !m_strFamily.compare_ci(font.m_strFamily)
          && !m_strSize.compare_ci(font.m_strSize)
          && !m_strWeight.compare_ci(font.m_strWeight)
          && !m_strTextDecoration.compare_ci(font.m_strTextDecoration);
   }

} // namespace html
