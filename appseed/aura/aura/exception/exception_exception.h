#pragma once


namespace exception
{


   class CLASS_DECL_AURA exception :
      virtual public ::object
   {
   public:


      string m_strMessage;


      exception();
      virtual ~exception();
      virtual void Delete();


      virtual const char * what () const throw ();


   };


   void CLASS_DECL_AURA rethrow(exception * pe);


} // namespace exception


