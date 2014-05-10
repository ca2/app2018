#pragma once


namespace html
{


   class CLASS_DECL_CORE font :
      virtual public element
   {
   public:
      
      
      index                m_iIndex;
      string               m_strFamily;
      string               m_strWeight;
      string               m_strSize;
      string               m_strTextDecoration;
      ::draw2d::font_sp    m_font;


      font();
      font(const class font & font);
      
      bool create(sp(::base::application) papp);

      bool operator == (const font & font);


   };


} // namespace html





