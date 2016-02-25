#pragma once


namespace html
{


   class CLASS_DECL_CORE data :
      virtual public signalizable,
      public ::data::data
   {
   public:


      class CLASS_DECL_CORE image :
         virtual public ::object
      {
      public:


         index                   m_iIndex;
         string                  m_strPath;
         ::visual::dib_sp        m_spdib;


         image(::aura::application * papp);
         virtual bool load_image();

      };


      class CLASS_DECL_CORE load_image
      {
      public:
         
         
         image *     m_pimage;
         data *      m_pdocument;


      };


      class CLASS_DECL_CORE layout_state1
      {
      public:

         float_array       m_cxa;
         float_array       m_cxMax;
         float_array       m_cya;
         bool                          m_bHasChar;
         float                         m_cy;


         void reset();


      };


      class CLASS_DECL_CORE layout_state2
      {
      public:
         int32_t      m_iBody;
         float    m_y;
         float    m_cy;
         float    m_x;
         float    m_cx;
         //         bool     m_bLastBlockX;
         //         bool     m_bLastCellX;
         bool     m_bLastBlock;
         bool     m_bLastCell;
         //bool     m_bHasChar;


         //ptr_array < impl::table > m_tableptraStack;
         //ptr_array < impl::table_row > m_tablerowptraStack;


         void reset();


      };


      class CLASS_DECL_CORE layout_state3
      {
      public:
         int32_t                       m_iBody;
         float_array       m_yParent;
         float                         m_y;
//         float_array       m_cyStack;
         float_array       m_xParent;
         float_array       m_cya;
  //       int                           m_iy;
         float                         m_x;
         float                         m_cx;
         //         bool     m_bLastBlockX;
         //         bool     m_bLastCellX;
         bool     m_bLastBlock;
         bool     m_bLastCell;
         //bool     m_bHasChar;


         //ptr_array < impl::table > m_tableptraStack;
         //ptr_array < impl::table_row > m_tablerowptraStack;


         void reset();


      };

      ::user::form_callback *    m_pcallback;
      sp(::user::interaction)      m_pui;
      ::draw2d::graphics *           m_pdc;
      box                        m_box;
      string                     m_strTitle;
      layout_state1               m_layoutstate1;
      layout_state2               m_layoutstate2;
      layout_state3               m_layoutstate3;

      ptr_array < impl::table >  m_tableptra;

      smart_pointer_array < image>
                                 m_imagea;

      smart_pointer_array < font >
                                 m_fonta;

      ::user::interaction_ptra   m_focusptra;

      style_sheet_array          m_stylesheeta;
      ::file::path               m_strPathName;
      user::interaction_spa           m_uiptra;
      tag *                      m_ptag;
      elemental                  m_elemental;
      bool                       m_bImplemented;


      bool                       m_bImplement;
      bool                       m_bLayout;


      http::cookies *            m_pcookies;
      ::fontopus::user *         m_puser;
      string                     m_strUser;
      string                     m_strPassword;
      string                     m_strLicense;

      property_set          m_propertyset;
      property_set          m_propset;
      string                     m_strSource;

      sp(::html_form)              m_pform;
      bool                       m_bEdit;
      elemental *                m_pElementalSelStart;
      elemental *                m_pElementalSelEnd;

      bool                       m_bHasChar;

      bool                       m_bDrawFirstBody;



      data(::aura::application * papp);
      virtual ~data();

      virtual bool open_document(var varFile);
      virtual bool open_link(const char * pszPath);

      virtual void OnBeforeNavigate2(var & varFile, uint32_t nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, bool* pbCancel);

      virtual ::user::form * get_form();
      virtual sp(::user::interaction) get_frame();


      //virtual bool is_locked();

      void load(const char *);
      void implement(::draw2d::dib * pdib);
      void layout(::draw2d::dib * pdib);
      void _001OnDraw(::draw2d::dib * pdib);

      void implement_and_layout(html_form * pform);

      DECL_GEN_SIGNAL(_001OnKeyDown);

      int32_t get_image_index(const char * pszUrl);
      void load_image_asynch(image * pimage);
      static UINT c_cdecl LoadImageProc(LPVOID lpParam);
      bool load_image(image * pimage);

      void on_image_loaded(image * pimage);


      elemental * get_element_by_name(id id);
      elemental * get_element_by_id(id id);

      void delete_contents();
      void destroy();
      void delete_implementation();

      virtual bool contains(sp(::user::interaction) pui);

      virtual bool on_create_interaction(sp(::user::interaction) pui);


      font * get_font(elemental * pelemental);

   protected:


      int32_t create_font(elemental * pelemental);


   private:


      data(const data & ) :
         ::data::data(NULL),
         m_elemental(NULL),
         m_uiptra(NULL)
      {
      }


   };


} // namespace html



