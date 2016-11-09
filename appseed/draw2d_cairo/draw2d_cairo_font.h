#pragma once


namespace draw2d_cairo
{


   class CLASS_DECL_DRAW2D_CAIRO font :
      virtual public ::draw2d::font
   {
   public:


      FT_Face                    m_ft;
      cairo_font_face_t *        m_pface;
      cairo_user_data_key_t      m_keyDone;
      cairo_scaled_font_t *      m_pfont;


      font(::aura::application * papp);
      virtual ~font();


      virtual bool destroy();


      virtual void * get_os_data() const;

      virtual void dump(dump_context & dumpcontext) const;


      virtual ::draw2d::font::e_cs get_cs(::draw2d::graphics * pgraphics) override;

   };


} // namespace draw2d_cairo



