#include "framework.h"
#include <float.h>


namespace html
{


   namespace impl
   {


      table::table()
      {

         m_iBorder         = 1;

         m_iCellSpacing    = 2;

         m_iCellPadding    = 2;

      }


      table::~table()
      {

      }


      void table::implement_phase1(data * pdata, ::html::elemental * pelemental)
      {

         m_bHasChar = true;

         elemental::implement_phase1(pdata, pelemental);

         if(m_pelemental->m_pbase->get_type() !=:: html::base::type_tag)
         {

            return;

         }

         tag * ptag = m_pelemental->get_tag();

         if(ptag != NULL)
         {

            if(ptag->get_attr_value("border").has_char())
            {
               m_iBorder         = (float) ::str::to_uint(ptag->get_attr_value("border"));
            }

            if(ptag->get_attr_value("cellspacing").has_char())
            {
               m_iCellSpacing    = (float) ::str::to_uint(ptag->get_attr_value("cellspacing"));
            }

            if(ptag->get_attr_value("cellpadding").has_char())
            {
               m_iCellPadding    = (float) ::str::to_uint(ptag->get_attr_value("cellpadding"));
            }

         }

         bool bTableBorder = m_iBorder > 0;

         if(m_border.left == 0.f && bTableBorder)
         {
            m_border.left = m_iBorder;
            m_border.crLeft = ARGB(255, 192, 192, 192);
            m_border.styleLeft = ::html::border::style_solid;
         }
         if(m_border.top == 0.f && bTableBorder)
         {
            m_border.top = m_iBorder;
            m_border.crTop = ARGB(255, 192, 192, 192);
            m_border.styleTop = ::html::border::style_solid;
         }
         if(m_border.right == 0.f && bTableBorder)
         {
            m_border.right = m_iBorder;
            m_border.crRight = ARGB(255, 84, 84, 84);
            m_border.styleRight = ::html::border::style_solid;
         }
         if(m_border.bottom == 0.f && bTableBorder)
         {
            m_border.bottom = m_iBorder;
            m_border.crBottom = ARGB(255, 84, 84, 84);
            m_border.styleBottom = ::html::border::style_solid;
         }

      }

      
      float table::calc_width()
      {

         string strWidth = m_pelemental->m_style.m_propertyset["width"].get_string();

         strWidth.trim();

//         if (false && str::ends_eat(strWidth, "%"))
//         {
//
//            strWidth.trim();
//
//            float percent = var(strWidth).get_float();
//
//            float cx = m_bound.get_cx();
//
//            cx *= percent / 100.0f;
//
//            return cx;
//
//
//         }
         
         float cx = 0.f;

         for (int32_t i = 0; i < m_columna.get_size(); i++)
         {

            cx += m_columna[i].m_cxMax;

         }

         return cx;

      }


      void table::layout_phase0(data * pdata)
      {

         for (index j = 0; j < m_rowptra.get_size(); j++)
         {

            m_rowptra[j]->m_pelemental->layout_phase0(pdata);

         }

         float cxMax = 0;

         float cxMin = 0;

         for(index i = 0; i < m_cellholdera.get_size(); i++)
         {

            cxMax += MAX(0, m_columna[i].m_cxMax);

            cxMin += MAX(0, m_columna[i].m_cxMin);

         }

         m_cxMax = cxMax;

         m_cxMin = cxMin;

      }


      void table::layout_phase1_end(data * pdata)
      {

         //elemental::layout_phase1_end(pdata);

         for (index i = 0; i < m_columna.get_count(); i++)
         {

            m_columna[i].m_cx = 0;

         }

         index iCol;

         cell * pcell;

         for (index i = 0; i < m_cellholdera.get_count(); i++)
         {

            for (index j = 0; j < m_cellholdera[i].get_count(); j++)
            {

               cell::holder & holder = m_cellholdera[i][j];

               pcell = holder.m_pcell;

               if (pcell == NULL)
                  continue;

               iCol = holder.m_iCol;

               if (iCol < 0)
                  continue;

               if (iCol >= m_columna.get_size())
                  continue;

               m_columna[iCol].m_cx = MAX(pcell->m_box.get_cx(), m_columna[iCol].m_cx);

            }

         }


         float cx = m_iCellSpacing;

         float cy = m_iCellSpacing;

         float x = m_box.left + m_iCellSpacing;

         for (int32_t i = 0; i < m_columna.get_size(); i++)
         {

            m_columna[i].m_x = x;

            x += m_columna[i].m_cx + m_iCellSpacing;

            cx += m_columna[i].m_cx + m_iCellSpacing;

         }

         for (int32_t i = 0; i < m_rowptra.get_size(); i++)
         {

            //   cy += m_rowptra[i]->m_box.get_cy() + m_iCellSpacing + 1;
            cy += m_rowptra[i]->m_box.get_cy() + m_iCellSpacing;

         }

         m_box.right =
            m_box.left
            + cx
            + m_margin.left
            + m_margin.right
            + m_border.left
            + m_border.right;
//            + m_padding.left
  //          + m_padding.right;

         m_box.bottom =
            m_box.top
            + cy
            + m_margin.top
            + m_margin.bottom
            + m_border.top
            + m_border.bottom;
//            + m_padding.top
  //          + m_padding.bottom;

         pdata->m_layoutstate1.m_cya.last() = m_box.get_cy();

         pdata->m_layoutstate1.m_cxMax.last() = MAX(m_box.get_cx(), pdata->m_layoutstate1.m_cxMax.last());

      }


      void table::layout_phase2(data * pdata)
      {

         UNREFERENCED_PARAMETER(pdata);

      }


      void table::layout_phase3(data * pdata)
      {
         
         UNREFERENCED_PARAMETER(pdata);

         set_xy(pdata);

//         float cx = m_iCellSpacing;

         float cy = m_iCellSpacing;

         float x = m_box.left + m_iCellSpacing + m_border.left + m_margin.left;

         for (int32_t i = 0; i < m_columna.get_size(); i++)
         {

            m_columna[i].m_x = x;

            x += m_columna[i].m_cx + m_iCellSpacing;

         }

         for (int32_t i = 0; i < m_rowptra.get_size(); i++)
         {

            //   cy += m_rowptra[i]->m_box.get_cy() + m_iCellSpacing + 1;
            cy += m_rowptra[i]->m_box.get_cy() + m_iCellSpacing;

         }



      }


      void table::layout_phase3_end(data * pdata)
      {

      }


      table::column::column()
      {

         m_iCol   = -1;
         m_cxMax  = -2;
         m_cxMin  = -2;

      }


      table::column::column(index iCol)
      {

         m_iCol   = iCol;
         m_cxMax  = -2;
         m_cxMin  = -2;

      }


      void table::set_cell(index iCol, index iRow, cell * pcell)
      {

         m_cellholdera.element_at_grow(iCol).element_at_grow(iRow).m_iCol     = iCol;
         m_cellholdera.element_at_grow(iCol).element_at_grow(iRow).m_iRow     = iRow;
         m_cellholdera.element_at_grow(iCol).element_at_grow(iRow).m_pcell    = pcell;

      }


      void table::_001OnDraw(data * pdata)
      {

         if(m_pelemental->m_pbase->get_type() == ::html::base::type_value)
            return;

         elemental::_001OnDraw(pdata);

         /*if(m_iBorder > 0)
         {

         pdata->m_pdc->Draw3dRect(get_x() - m_iBorder - 2, get_y() - m_iBorder - 2, 
         get_cx() + ((m_iBorder + 2) * 2), get_cy() + ((m_iBorder + 2) * 2), ARGB(255, 184, 184, 184), ARGB(255, 84, 84, 84));

         }*/
      }

   } // namespace impl

} // namespace html
