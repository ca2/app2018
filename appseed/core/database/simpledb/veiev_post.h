#pragma once


class db_server;


class CLASS_DECL_CORE veiev_post :
   public db_set
{
public:


   veiev_post(db_server * pdatacentral);
   virtual ~veiev_post();


   /*class CLASS_DECL_CORE record
   {
   public:
      var sender;
      var recipient;
      var sent;
      var send_time;
      var index;
      var message;
   };*/


   bool write(var record);
   var last();
   var current();


   var get_page(::index iPage, ::count iMessageCountPerPage);
   int64_t get_count();


};



