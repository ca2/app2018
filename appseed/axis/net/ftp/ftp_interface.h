#pragma once


namespace ftp
{


   class CLASS_DECL_AXIS iinterface :
      virtual public object
   {
   protected:


      iinterface() {}
      virtual ~iinterface() {}


   private: // prevent copy action

      iinterface(const iinterface&);


      const iinterface& operator=(const iinterface&);

   };


} // namespace ftp



