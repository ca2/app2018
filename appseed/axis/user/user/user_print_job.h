#pragma once


namespace user
{


   class CLASS_DECL_BASE print_job :
      virtual public ::print_job
   {
   public:

      
      sp(::aura::printer)       m_phprinter;


      print_job(::aura::application * papp);
      virtual ~print_job();


      virtual int32_t run();


   };


} // namespace user



