#pragma once


namespace html
{


   namespace impl
   {


      class CLASS_DECL_ca2 table_row : public elemental
      {
      public:


         index                               m_iRow;
         int32_t                             m_cyMin;
         int32_t                             m_cyMax;
         lemon_array < cell::holder >        m_cellholdera;


         table_row();
         virtual ~table_row();


         void _001OnDraw(data * pdata);

         table * get_table();

         void set_cell(index iCol, index iRow, cell * pcell);


         virtual void implement_phase1(data * pdoc, ::html::elemental * pelemental);
         virtual void implement_phase2(data * pdoc);
         virtual void layout_phase3(data * pdata);
         virtual void final_layout(data * pdata);


         virtual bool use_in_final_layout(::html::impl::elemental * pimplChild);

      };


   } // namespace impl


} // namespace html

