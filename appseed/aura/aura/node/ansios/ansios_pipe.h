#pragma once


namespace ansios
{


   class CLASS_DECL_AURA pipe :
      virtual public ::process::pipe
   {
   public:


      char *               m_pchBuf;
      string               m_strRead;

      int32_t                  m_fd[2];


      pipe(::aura::application * papp);
      virtual ~pipe();


      virtual bool create(bool bBlock = true, bool bInherit = false);


      virtual bool not_inherit_read();
      virtual bool not_inherit_write();

      virtual bool write(const char * psz);
      virtual string read();
      virtual string one_pass_read();



   };


} // namespace core





































