#pragma once


class CLASS_DECL_ACE not_supported_exception : 
   virtual public interface_only_exception
{
public:
   
   
   not_supported_exception(const not_supported_exception & e);
   not_supported_exception(::ace::application * papp, const char * pszTip = NULL);
   virtual ~not_supported_exception();


};
