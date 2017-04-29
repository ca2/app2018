#pragma once


namespace draw3d
{


   class CLASS_DECL_BASE ortho:
      virtual public ::object
   {
   public:

      
      ::draw2d::graphics * m_pdc;

      
      int_array draw_ortho_plain_wire_frame(::box * pbox, int iHint,int bA,bool bFirst);
      int_array draw_ortho_plain_fill(::box * pbox, int iHint,int bA,bool bFirst,::draw2d::dib * pdib);


   };


} // namespace draw3d