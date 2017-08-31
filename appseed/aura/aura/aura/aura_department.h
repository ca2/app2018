#pragma once



namespace aura
{



   class CLASS_DECL_AURA department :
   virtual public ::message::receiver
   {
   public:


      department(application * papp);
      virtual ~department();


      virtual void construct(::aura::application * papp);
      virtual void connect_to_application_signal();


      virtual bool process_initialize();


      virtual bool initialize();
      virtual bool initialize1();
      virtual bool initialize2();
      virtual bool initialize3();
      virtual bool initialize_application();

      virtual bool finalize();
      virtual int32_t exit_application();


      void on_application_message(::message::message * pobj);

   };




} // namespace aura



