#pragma once


class CLASS_DECL_AURA memory_exception :
   virtual public simple_exception
{
public:

   memory_exception();
   memory_exception(::aura::application * papp);
   memory_exception(::aura::application * papp, const char * pszMessage);
   virtual ~memory_exception();


};


#if defined(SOLARIS)


namespace std
{


   class CLASS_DECL_AURA bad_alloc :
      virtual public memory_exception
   {
   public:


      bad_alloc() :
         object(get_app()),
         ::call_stack(get_app()),
         ::exception::base(get_app()),
         simple_exception(get_app()),
         memory_exception(get_app())
      {

      }

      virtual ~bad_alloc() {}


   };


}

#endif



