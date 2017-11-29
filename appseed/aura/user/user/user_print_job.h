#pragma once


namespace user
{


   class CLASS_DECL_AURA print_job :
      virtual public ::user::job
   {
      public:

#pragma once


         int32_t      m_iPageStart;
         int32_t      m_iPageCount;
         int32_t      m_iPrintingPage;




         sp(::aura::printer)       m_phprinter;


         print_job(::aura::application * papp);
         virtual ~print_job();


         virtual void run();


   };


} // namespace user



