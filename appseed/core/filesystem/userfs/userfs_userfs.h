#pragma once


namespace userfs
{


   class CLASS_DECL_CORE userfs :
      virtual public ::aura::department
   {
   public:


      userfs(::aura::application * papp);
      virtual ~userfs();

      bool initialize();


   };



} // namespace userfs



