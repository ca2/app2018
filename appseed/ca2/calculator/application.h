#pragma once


namespace ca2
{


   namespace calculator
   {


      class CLASS_DECL_ca2 application :
         virtual public ::ca2::fontopus::application
      {
      public:


         application(void);


         virtual ~application(void);


         bool initialize();



      };


   } // namespace calculator


} // namespace ca2



