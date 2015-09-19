//#include "framework.h" // from "base/user/user.h"


namespace user
{


   schema::schema()
   {

      m_puserschema = NULL;

   }


   schema::~schema()
   {

   }


   bool schema::get_color(COLORREF & cr, e_color ecolor)
   {

      if(m_puserschema != NULL && m_puserschema != this)
      {

         if(m_puserschema->get_color(cr,ecolor))
            return true;

      }

      if(get_parent_user_schema() != NULL)
      {

         if(get_parent_user_schema()->get_color(cr,ecolor))
            return true;

      }

      if( m_pauraapp != NULL && this != m_pauraapp->m_pbasesession &&m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession != m_puserschema && m_pauraapp->m_pbasesession->m_pschemasimple != m_puserschema)
      {

         if(m_pauraapp->m_pbasesession->get_color(cr,ecolor))
            return true;

      }

      return false;

   }

   bool schema::get_font(::draw2d::font_sp & spfont)
   {

      if(m_puserschema != NULL && m_puserschema != this)
      {

         if(m_puserschema->get_font(spfont))
            return true;

      }

      if(get_parent_user_schema() != NULL)
      {

         if(get_parent_user_schema()->get_font(spfont))
            return true;

      }

      if(m_pauraapp != NULL && this != m_pauraapp->m_pbasesession && m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession != m_puserschema && m_pauraapp->m_pbasesession->m_pschemasimple != m_puserschema)
      {

         if(m_pauraapp->m_pbasesession->get_font(spfont))
            return true;

      }

      return false;

   }

   bool schema::get_translucency(ETranslucency & etranslucency)
   {

      if(m_puserschema != NULL && m_puserschema != this)
      {

         if(m_puserschema->get_translucency(etranslucency))
            return true;

      }

      if(get_parent_user_schema() != NULL)
      {

         if(get_parent_user_schema()->get_translucency(etranslucency))
            return true;

      }

      if(m_pauraapp != NULL && this != m_pauraapp->m_pbasesession && m_pauraapp->m_pbasesession != NULL && m_pauraapp->m_pbasesession != m_puserschema && m_pauraapp->m_pbasesession->m_pschemasimple != m_puserschema)
      {

         if(m_pauraapp->m_pbasesession->get_translucency(etranslucency))
            return true;

      }

      return false;

   }

   COLORREF schema::_001GetColor(e_color ecolor,COLORREF crDefault)
   {

      COLORREF cr;

      if(get_color(cr,ecolor))
         return cr;

      return crDefault;

   }

   COLORREF schema::_001GetColor(e_color ecolor)
   {

      COLORREF cr;

      if(get_color(cr, ecolor))
         return cr;

      if(ecolor == color_text)
      {

         return ARGB(255,0,0,0);

      }
      else if(ecolor == color_text_selected)
      {

         return ARGB(255,255,255,255);

      }
      else if(ecolor == color_text_highlight)
      {

         return ARGB(255,55,105,255);

      }
      else if(ecolor == color_text_selected_highlight)
      {

         return ARGB(255,102,153,255);

      }
      else if(ecolor == color_background)
      {

         if(_001IsTranslucent())
         {

            return ARGB(184,255,255,255);

         }
         else
         {

            return ARGB(255,255,255,255);

         }

      }
      else if(ecolor == color_background_selected)
      {

         if(_001IsTranslucent())
         {

            return ARGB(184,0,0,127);

         }
         else
         {

            return ARGB(255,0,0,127);

         }

      }
      else if(ecolor == color_background_selected)
      {

         if(_001IsTranslucent())
         {

            return ARGB(184,0,0,127);

         }
         else
         {

            return ARGB(255,0,0,127);

         }

      }
      else if(ecolor == color_border_normal)
      {

         return ARGB(255,84,84,77);

      }
      else if(ecolor == color_border_hover)
      {

         return ARGB(255,84,184,77);

      }
      else if(ecolor == color_text_hover)
      {

         return ARGB(255,84,184,255);

      }


      throw invalid_argument_exception(::get_thread_app());

   }


   ETranslucency schema::_001GetTranslucency(ETranslucency etranslucencyDefault)
   {

      ETranslucency etranslucency;

      if(get_translucency(etranslucency))
         return etranslucency;

      return etranslucencyDefault;


   }



   bool schema::_001IsTranslucent()
   {

      ETranslucency etranslucency = _001GetTranslucency();

      return etranslucency == TranslucencyPresent || etranslucency == TranslucencyTotal;

   }


   bool schema::_001IsBackgroundBypass()
   {

      return _001GetTranslucency() == TranslucencyTotal;

   }


   bool schema::_001HasTranslucency()
   {

      ETranslucency etranslucency = _001GetTranslucency();

      return etranslucency == TranslucencyPresent || etranslucency == TranslucencyTotal;

   }


   bool schema::_001IsTransparent()
   {

      return _001GetTranslucency() == TranslucencyTotal;

   }


   ::user::schema * schema::get_user_schema()
   {

      if(m_puserschema != NULL)
      {

         return m_puserschema;

      }

      return get_parent_user_schema();

   }


   ::user::schema * schema::get_parent_user_schema()
   {

      return NULL;

   }


   bool schema::select_layered_frame()
   {


      
      m_puserschema = System.m_pschemaLayeredFrame;

      return true;

   }



   bool schema::select_text_color(::draw2d::graphics * pgraphics,e_color ecolor)
   {

      COLORREF crText;

      if(!get_color(crText,ecolor))
         return false;

      return pgraphics->set_text_color(crText);

   }

   
   bool schema::select_font(::draw2d::graphics * pgraphics)
   {

      ::draw2d::font_sp spfont;

      if(!get_font(spfont))
         return false;

      if(spfont.is_null())
         return false;

      return pgraphics->set_font(spfont);

   }


   bool schema::simple_ui_draw_focus_rect(::user::interaction * pui,::draw2d::graphics * pgraphics)
   {

      if(m_puserschema != NULL && m_puserschema != this)
      {

         if(m_puserschema->simple_ui_draw_focus_rect(pui, pgraphics))
            return true;

      }

      rect rectClient;

      pui->GetClientRect(rectClient);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      int32_t iStyle = 1;

      rectClient.left--;
      rectClient.top--;

      if(m_pauraapp != NULL && (Session.get_keyboard_focus() == pui || pui->has_action_hover()))
      {

         ::draw2d::brush_sp brush(allocer());

         if(pui->has_action_hover() && !pui->has_text_input())
         {

            brush->create_solid(ARGB(184,150,230,180));

            pgraphics->SelectObject(brush);

            pgraphics->FillRectangle(rectClient);

         }

         if(iStyle == 1)
         {

            if(pui->has_action_hover())
            {

               ::draw2d::pen_sp pen(pgraphics,1.0,pui->get_action_hover_border_color());

               pgraphics->DrawRect(rectClient,pen);

            }
            else
            {

               //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(255, 108, 149, 255));
               ::draw2d::pen_sp pen(pgraphics,1.0,ARGB(184,84,84,77));

               pgraphics->DrawRect(rectClient,pen);

            }


            if(Session.get_keyboard_focus() == pui)
            {

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(255, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics,1.0,ARGB(135,120,190,255));

                  pgraphics->DrawRect(rectClient,pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  /*::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);*/

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(84, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics,1.0,ARGB(105,140,200,255));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient,pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true,::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient,1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(72, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics,1.0,ARGB(95,160,210,255));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient,pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true,::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient,1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(60, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics,1.0,ARGB(75,180,220,255));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient,pen);

               }


               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true,::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient,1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(48, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics,1.0,ARGB(55,200,230,255));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient,pen);

               }



               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_sp pathRound(allocer());

                  pathRound->begin_figure(true,::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient,1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(36, 108, 149, 255));
                  ::draw2d::pen_sp pen(pgraphics,1.0,ARGB(45,220,240,255));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient,pen);

               }




               /*
               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

               ::draw2d::path_sp pathRound(allocer());

               pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

               pathRound->add_round_rect(rectClient, 1 * 2);

               pathRound->end_figure(true);

               //::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(24, 108, 149, 255));

               ::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(25, 240, 250, 255));

               pgraphics->SelectObject(pen);

               pgraphics->DrawRect(rectClient, pen);

               }
               */

            }

         }
         else
         {

            ::draw2d::pen_sp pen(pgraphics, 3.0, ARGB(255, 84, 77, 255));

            pgraphics->DrawRect(rectClient, pen);

         }

      }
      else
      {

         ::draw2d::pen_sp pen(pgraphics, 1.0, ARGB(255, 149, 149, 123));

         pgraphics->DrawRect(rectClient, pen);

      }

      return true;

   }

   void schema::_001TabOnDrawSchema01(::draw2d::graphics * pdc,tab * ptab)
   {

   }


   void schema::_001OnTabPaneDrawTitle(::user::tab_pane & pane,::user::tab * ptab,::draw2d::graphics * pdc,LPCRECT lpcrect,::draw2d::brush_sp & brushText)
   {

   }


   void schema::_001OnTabLayout(::user::tab * ptab)
   {

   }


} // namespace user





