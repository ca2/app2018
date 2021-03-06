#pragma once


namespace draw2d
{


   class CLASS_DECL_AURA pen :
      virtual public ::draw2d::object
   {
   public:


      enum e_type
      {

         type_null,
         type_solid,
         type_dot,
         type_brush,
         type_dash

      };


      enum e_line_cap
      {

         line_cap_flat,
         line_cap_round,
         line_cap_square,

      };

      enum e_align
      {

         align_center,
         align_inset,

      };


      enum e_line_join
      {

         line_join_miter,
         line_join_bevel,
         line_join_round,
         line_join_miter_clipped

      };


      e_type            m_etype;
      e_line_cap        m_elinecapBeg;
      e_line_cap        m_elinecapEnd;
      e_line_join       m_elinejoin;
      e_align           m_ealign;
      double            m_dWidth;
      COLORREF          m_cr;
      sp(brush)         m_br;


      pen();
      virtual ~pen();


#ifdef DEBUG

      virtual void dump(dump_context & dumpcontext) const;

#endif


      virtual bool create_null();
      virtual bool create_solid(double dWidth, COLORREF crColor);
      virtual bool create_brush(double dWidth, ::draw2d::brush * pbrush);

      virtual e_line_cap get_beg_cap();
      virtual bool set_beg_cap(e_line_cap ebegcap);

      virtual e_line_cap get_end_cap();
      virtual bool set_end_cap(e_line_cap eendcap);

      virtual e_line_join get_line_join();
      virtual bool set_line_join(e_line_join elinejoin);

      pen & operator = (const pen & penSrc);



   };


   class CLASS_DECL_AURA pen_sp :
      public smart_pointer < pen >
   {
   public:

      pen_sp()
      {
      }

      pen_sp(pen * ppen) :
         smart_pointer < pen > (ppen)
      {
      }

      pen_sp(const smart_pointer < pen > & p) :
         smart_pointer < pen > (p)
      {
      }

      pen_sp(const ::aura::allocatorsp & allocer) :
         smart_pointer < pen > (allocer)
      {
      }

      pen_sp(const ::aura::allocatorsp & allocer, double dWidth, COLORREF crColor);
      /*
            pen_sp(::aura::application * papp, int32_t nPenStyle, int32_t nWidth, const LOGBRUSH* pLogBrush,
               int32_t nStyleCount = 0, const uint32_t* lpStyle = NULL) :
               smart_pointer < pen > (papp)
            {
               m_p->construct(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle);
            }
      */


      pen_sp & operator = (::draw2d::pen * ppen)
      {

         smart_pointer < pen >::operator = (ppen);

         return *this;

      }


   };


} // namespace draw2d





