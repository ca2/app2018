#pragma once


namespace net
{


   class CLASS_DECL_AXIS email_departament :
      virtual public ::aura::departament
   {
   public:


      email_departament(::aura::application * papp);


      bool utf8_mail(class ::net::email & email);


      bool syntax_is_valid(const char * pszCandidate);


   };


} // namespace net





