#pragma once


namespace visual
{


   class CLASS_DECL_BASE cursor :
      virtual public ::object
   {
   public:


      e_cursor                m_ecursor;
      ::draw2d::dib_sp        m_dib;
      //::draw2d::dib_sp        m_dibWork;
      //::draw2d::dib_sp        m_dibWork2;
      size                    m_szHotspotOffset;


      cursor(sp(base_application) papp);

      bool to(::draw2d::graphics * pgraphics, point pt);


   };


   typedef sp(cursor) cursor_sp;


} // namespace visual


