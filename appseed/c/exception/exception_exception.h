#pragma once


namespace exception
{


   class CLASS_DECL_c exception :
      virtual public base
   {
   public:


      virtual ~exception();
      virtual void Delete();


   };


   void CLASS_DECL_c rethrow(exception * pe);


} // namespace exception