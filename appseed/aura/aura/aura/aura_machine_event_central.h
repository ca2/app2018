#pragma once


class CLASS_DECL_AURA machine_event_central :
   virtual public simple_thread
{
public:


   machine_event     m_machineevent;
   bool              m_bInitialized;


   machine_event_central(::aura::application * papp);
   virtual ~machine_event_central();


   void read(machine_event_data * pdata);
   void write(machine_event_data * pdata);

   bool initialize();
   void run();

   bool is_close_application();

   void command(sp(::xml::node) pnodeCommand);

};


