#pragma once


namespace calculator
{


   class CLASS_DECL_SPHERE application :
      virtual public ::asphere::application
   {
   public:


      application();


      virtual ~application();


      virtual bool init_instance() override;



   };


} // namespace calculator






