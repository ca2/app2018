#include "StdAfx.h"


namespace ca2
{


   namespace filehandler
   {


      document::document(::ca::application * papp) :
         ca(papp),
         ::userbase::document(papp)
      {
      }

      bool document::on_new_document()
      {
         if (!::userbase::document::on_new_document())
            return FALSE;

   //      if(!get_data()->on_open_document("rtp://10.10.100.97:19847"))
     //    {
       //     delete_contents();
         //   return FALSE;
         //}

         //update_all_views(NULL, 19847);

         return TRUE;
      }

      bool document::on_open_document(var varFile) 
      {
         return TRUE;
      }


      document::~document()
      {
      }



      #ifdef _DEBUG
      void document::assert_valid() const
      {
         ::userbase::document::assert_valid();
      }

      void document::dump(dump_context & dumpcontext) const
      {
         ::userbase::document::dump(dumpcontext);
      }
      #endif //_DEBUG


   } // namespace filehandler


} // namespace mail



