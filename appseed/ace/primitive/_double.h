#pragma once



namespace math
{


   class CLASS_DECL_CORE _double :
      virtual public value_impl
   {
   public:

      virtual void set_value_string(const char * psz);
      virtual string get_value_string();


   };


} // namespace math



