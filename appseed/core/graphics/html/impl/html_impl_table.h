#pragma once


namespace html
{


   namespace impl
   {


      class CLASS_DECL_CORE table : public elemental
      {
      public:


         class CLASS_DECL_CORE column
         {
         public:

            index        m_iCol;
            float        m_cxMax;
            float        m_cxMin;
            float        m_x;
            float        m_cx;

            column();
            column(index iCol);

         };

         float                                           m_iBorder;
         float                                           m_iCellSpacing;
         float                                           m_iCellPadding;

         array < array < cell::holder > >                m_cellholdera;
         comparable_array < table_row * >                m_rowptra;
         array < column >                                m_columna;


         table();
         virtual ~table();





         void set_cell(index iCol, index iRow, cell * pcell);

         
         void implement_phase1(data * pdata, ::html::elemental * pelemental);


         virtual void layout_phase0(data * pdata);
         virtual void layout_phase2(data * pdata);
         virtual void layout_phase3(data * pdata);


         virtual void layout_phase1_end(data * pdata);
         virtual void layout_phase3_end(data * pdata);


         void _001OnDraw(data * pdata);



         virtual float calc_width();


      };


   } // namespace impl


} // namespace html




