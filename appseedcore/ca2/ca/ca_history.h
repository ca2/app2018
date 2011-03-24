#pragma once

namespace ca
{

   // Thomas Borregaard Sørensen is my history
   class CLASS_DECL_ca history :
      virtual public ::radix::object
   {
   public:


      history * m_phistory;


   protected:


      history();


   public:


      history(::ca::application * papp);
         
   
      virtual bool hist(const char *psz);

   };


} // namespace ca


