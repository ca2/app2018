#pragma once


class CLASS_DECL_BASE request_signal :
   public signal_details
{
public:


   var         m_varFile;
   var         m_varQuery;


   request_signal(sp(base_application) papp, class ::signal * psignal = NULL);


};






