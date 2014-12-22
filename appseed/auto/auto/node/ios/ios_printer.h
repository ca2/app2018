#pragma once


namespace ios2
{


   class CLASS_DECL_BASE printer :
      virtual public ::user::printer
   {
   public:


      class CLASS_DECL_BASE document_properties :
         virtual public ::object
      {
      public:


//         DEVMODE *      m_pdevmode;
         HDC            m_hdc;


         document_properties(sp(::aura::application) papp);
         virtual ~document_properties();


  //       virtual bool initialize(::win2::printer * pprinter, DEVMODE * pdevmode = NULL);
         virtual bool initialize(::ios2::printer * pprinter);
         virtual bool close();
         virtual ::draw2d::graphics * create_graphics();

      };


      HANDLE                  m_hPrinter;
      document_properties     m_documentproperties;


      printer(sp(::aura::application) papp);
      virtual ~printer();


      virtual bool open(const char * pszDeviceName);
      virtual ::draw2d::graphics * create_graphics();
      virtual bool is_opened();
      virtual bool close();



   };
   
   
} // namespace iosU






