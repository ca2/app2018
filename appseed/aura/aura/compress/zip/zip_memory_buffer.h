#pragma once


namespace zip
{


   class CLASS_DECL_AURA memory_file :
      public ::memory_file

   {
   public:


      void *               m_pvoidFileFuncDef;


      memory_file(::aura::application * papp);
      memory_file(::aura::application * papp, ::primitive::memory_base * pmemory);
      virtual ~memory_file();


      void common_construct();



   };


} // namespace zip



