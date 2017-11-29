#pragma once


namespace aura
{

   class CLASS_DECL_AURA app:
      virtual public ::aura::application
   {
   public:


      int32_t                       m_argc;
      TCHAR **						m_argv;

      MESSAGE                       m_msg;


      app();
      virtual ~app();

      virtual void main() override;

      virtual void body();

      virtual int32_t simple_app_pre_run();

      virtual bool intro();
      virtual int32_t refrain();
      virtual bool end();


   };


} // namespace aura




