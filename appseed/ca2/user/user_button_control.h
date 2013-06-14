#pragma once

class image_list;


class CLASS_DECL_ca2 BaseButtonControl :
   virtual public ::user::button
{
public:


   sp(image_list)      m_pimagelistNormal;
   sp(image_list)      m_pimagelistItemHover;
   sp(image_list)      m_pimagelistSubItemHover;
   int32_t               m_iImageNormal;
   int32_t               m_iImageItemHover;
   int32_t               m_iImageSubItemHover;


   BaseButtonControl(sp(::ca2::application) papp);
   virtual ~BaseButtonControl();


   virtual void _003CustomWindowProc(::ca2::signal_object * pobj);


   virtual bool pre_create_window(CREATESTRUCT& cs);
   

   virtual void install_message_handling(::ca2::message::dispatch * pdispatch);

   void UpdateDrawingObjects();
   void UpdateWndRgn();
   void _001OnDraw(::ca2::graphics * pdc);

   

};

