#pragma once


namespace net
{


   class CLASS_DECL_CORE email_departament :
      virtual public base_departament
   {
   public:


      email_departament(sp(base_application) papp);


      bool utf8_mail(class ::net::email & email);


      bool syntax_is_valid(const char * pszCandidate);


   };


} // namespace net





