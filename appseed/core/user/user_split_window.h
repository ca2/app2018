#pragma once

class CLASS_DECL_CORE BaseSplitWnd :
   virtual public ::user::split_layout
{
public:
   BaseSplitWnd(sp(base_application) papp);

   virtual void _001OnDraw(::draw2d::graphics * pdc);

   virtual bool pre_create_window(CREATESTRUCT& cs);

   virtual ~BaseSplitWnd();

};

