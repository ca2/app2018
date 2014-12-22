#pragma once


struct FIBITMAP;


namespace visual
{

   namespace image
   {

      enum e_format
      {
         format_png,
         format_bmp,
         format_gif,
         format_jpeg,
      };

   } // namespace image

   class CLASS_DECL_AXIS save_image
   {
   public:


      image::e_format         m_eformat;
      int                     m_iQuality;

      save_image();

   };


   class CLASS_DECL_AXIS dib_sp :
      public ::draw2d::dib_sp
   {
   public:

      dib_sp();
      dib_sp(const allocatorsp & allocer);
      virtual ~dib_sp();





      virtual bool load_from_file(var varFile, bool bCache = true);
      virtual bool load_from_matter(const char * pszMatter);
      virtual bool read_from_file(::file::buffer_sp  pfile);


      virtual bool save_to_file(var varFile, save_image * psaveimage = NULL);
      virtual bool write_to_file(::file::buffer_sp  pfile, save_image * psaveimage = NULL);

#ifdef AXIS_FREEIMAGE
      virtual bool from(::draw2d::graphics * pgraphics, FIBITMAP * pfibitmap, bool bUnloadFI);
#endif
      

   };


} // namespace visual


CLASS_DECL_AXIS void draw_freetype_bitmap(::draw2d::dib * m_p,int32_t dx,int32_t dy,void * pftbitmap,int x,int y);
CLASS_DECL_AXIS void draw_freetype_bitmap(::draw2d::dib * m_p,int32_t dx,int32_t dy,void * pftbitmap,int x,int y,byte a,byte r,byte g,byte b);












