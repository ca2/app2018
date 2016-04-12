#pragma once


class CLASS_DECL_AXIS image_list :
   virtual public ::object
{
public:


   class CLASS_DECL_AXIS info
   {
   public:


      rect                 m_rect;
      ::draw2d::dib *      m_pdib;

   };


   size                 m_size;
   int32_t              m_iSize;
   int32_t              m_iGrow;
   ::draw2d::dib_sp     m_spdib;


   ::draw2d::dib_sp     m_spdibWork;
   ::draw2d::dib_sp     m_spdibWork2;
   ::draw2d::dib_sp     m_spdibWork3;




   image_list(::aura::application * papp);
   image_list(const image_list & imagelist);
   virtual ~image_list();


   using ::object::create;
   bool create(int32_t cx, int32_t cy);
   bool create(int32_t cx, int32_t cy, UINT nFlags, int32_t nInitial, int32_t nGrow);
   bool realize(::draw2d::graphics * pgraphics) const;
    //bool create(const char * lpszBitmapID, int32_t cx, int32_t nGrow, COLORREF crMask);

   int32_t add(::visual::icon * picon);
   int32_t add_icon(const char * lpcsz);
   int32_t add_dib(::draw2d::dib * pdib, int x, int y);
   int32_t add_icon_os_data(void * pvoid);
   int32_t add_matter_icon(const char * lpcszMatter);
   int32_t add_file(const char * lpcsz);
   int32_t add_matter(const char * lpcsz, ::aura::application * papp = NULL);
   int32_t add_std_matter(const char * lpcsz);

   bool draw(::draw2d::graphics * pdc, int32_t iImage, point pt, int32_t iFlag);
   bool draw(::draw2d::graphics * pdc, int32_t iImage, point pt, int32_t iFlag, BYTE alpha);
   bool draw(::draw2d::graphics * pdc, int32_t iImage, point pt, size sz, point ptOffset, int32_t iFlag);
   int32_t get_image_count() const;
   

   bool get_image_info(int32_t nImage, info * pinfo) const;

   void remove_all();

   image_list & operator = (const image_list & imagelist);

   bool _grow();
   int32_t _get_alloc_count();

};





