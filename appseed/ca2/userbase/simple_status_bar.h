#pragma once

class CLASS_DECL_ca2 simple_status_bar :
   public ::userbase::status_bar
{
public:
   simple_status_bar(::ca::applicationsp papp);

   virtual void _001OnDraw(::ca::graphics * pdc);
   virtual ~simple_status_bar();

};
