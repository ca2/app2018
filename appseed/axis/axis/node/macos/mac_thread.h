#pragma once


WINBOOL AfxInternalPreTranslateMessage(MESSAGE* pMsg);


namespace mac
{

   

   
   class CLASS_DECL_mac thread :
      virtual public ::ansios::thread_impl
   {
   public:
      

      
      thread(::aura::application * papp);
      virtual ~thread();
      
      

      
      
   };
 


} // namespace mac




