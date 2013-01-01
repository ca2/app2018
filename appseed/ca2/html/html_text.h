#pragma once


namespace html
{


   namespace impl
   {


      class text : 
         virtual public elemental,
         virtual public user::edit_plain_text
      {
      public:


         base_array < size >           m_sizea;
         stringa                       m_straLines;
         stringa                       m_straWordSpace;
         bool                          m_bLink;
         string                        m_strLink;
         bool                          m_bOnAfterChangeText;


         text(::ca::application * papp);

         DECL_GEN_SIGNAL(OnLButtonDown)
         DECL_GEN_SIGNAL(OnMouseMove)
         DECL_GEN_SIGNAL(OnLButtonUp)

         virtual void _001OnGetText();

         virtual void _001OnDraw(data * pdoc);
         virtual void _001OnDraw(::ca::graphics * pgraphics);
         void implement_phase1(data * pdoc, ::html::elemental * pelemental);
         void layout_phase3(data * pdoc);

         virtual void delete_implementation();

         virtual bool get_color(COLORREF & cr);
         virtual bool has_link();
         virtual string link();

         using user::edit_plain_text::hit_test;
         virtual int32_t hit_test(data * pdoc, ::point pt);

         virtual strsize char_hit_test(::ca::graphics * pdc, int32_t x, int32_t y);

         virtual float get_first_line_height();
         virtual float get_last_line_height();

         virtual void _001OnAfterChangeText();


      };

   } // namespace impl



} // namespace html_impl