#pragma once


namespace draw2d_opengl
{


   class CLASS_DECL_DRAW2D_OPENGL font : 
      virtual public ::draw2d::font
   {
   public:

      
      HDC                           m_hdcFont;
      HFONT                         m_hfont;
      GLuint	                     m_baseFont;				// Base Display List For The Font Set

#ifdef WINDOWS
      GLYPHMETRICSFLOAT             m_gmf[256];	// Storage For Information About Our Outline Font Characters
#endif



      font(::aura::application * papp);
      font(const font & font);
      virtual ~font();


      virtual void * get_os_data() const;


      virtual void construct(const ::draw2d::font & fontParam);

      
      virtual void dump(dump_context & dumpcontext) const;





   };


} // namespace draw2d_opengl



