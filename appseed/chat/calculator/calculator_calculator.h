#pragma once


namespace calculator
{


   class calculator;


   class CLASS_DECL_CA2_MATH calculator :
      virtual public ::aura::department
   {
   public:


      calculator(::aura::application * papp);
      virtual ~calculator();


      bool initialize();


   };


} // namespace calculator


