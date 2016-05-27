#pragma once



class CLASS_DECL_CORE simple_tab_view :
   virtual public ::user::impact,
   virtual public ::user::tab
{
public:
   simple_tab_view(::aura::application * papp);
   virtual ~simple_tab_view();



   virtual void _001OnDraw(::draw2d::graphics * pgraphics);

#ifdef DEBUG
   virtual void assert_valid() const;
#ifndef _WIN32_WCE
   virtual void dump(dump_context & dumpcontext) const;
#endif
#endif

   DECL_GEN_SIGNAL(_001OnSize);
   void layout();

   void _001OnCreate(signal_details * pobj);

   virtual bool GetClientRect(LPRECT lprect);

   virtual void install_message_handling(::message::dispatch * pinterface);

   DECL_GEN_SIGNAL(_001OnLButtonUp);
   DECL_GEN_SIGNAL(_001OnMouseMove);
   DECL_GEN_SIGNAL(_001OnMouseLeave);

};


