#pragma once


namespace process
{


   class CLASS_DECL_AURA pipe :
      virtual public ::object
   {
   public:

      
      char *               m_pchBuf;
      string               m_strRead;

      bool                 m_bInherit;
      bool                 m_bBlock;


      pipe(sp(::aura::application) papp);
      virtual ~pipe();


      virtual bool create(bool bBlock = true, bool bInherit = false);


      virtual bool not_inherit_read();
      virtual bool not_inherit_write();

      virtual bool write(const char * psz);
      virtual string read();
      virtual string one_pass_read();


#ifdef WINDOWS

      void readex();

#endif



   };


} // namespace process


































