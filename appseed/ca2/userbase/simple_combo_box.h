#pragma once


class CLASS_DECL_ca simple_combo_box :
   public ::userbase::combo_box
{
public:
   simple_combo_box();


   virtual void pre_subclass_window();

   virtual void _001OnDraw(::ax::graphics * pdc);
   virtual ~simple_combo_box();

   BOOL OnEraseBkgnd(::ax::graphics * pgraphics);
};

