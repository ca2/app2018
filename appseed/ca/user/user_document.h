#pragma once


class CLASS_DECL_ca document :
   virtual public user::document_interface,
   public ::ca::data_container,
   virtual public ::database::client
{
public:


   document(::ca::application * papp);
   virtual ~document();


};


