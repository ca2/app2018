#pragma once


namespace user
{

   class CLASS_DECL_CORE job :
      virtual public ::job
   {
   public:


      // pointer holder use intention:
      // cascading ph - pointer holder - references would avoid this view
      // and other ph's referenced directly or indirectly by m_pview
      // to be deleted while the job is not finished or canceled.
      sp(::user::view)        m_pview;

      job(sp(base_application) papp);
      virtual ~job();


      virtual int32_t run() = 0;

   };

} // namespace user
