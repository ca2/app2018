#pragma once



namespace aura
{

   CLASS_DECL_AURA void raw_trace_v(const char *pszFileName,int32_t nLine,uint32_t dwCategory,uint32_t nLevel,const char * pszFmt,va_list args);

   namespace trace
   {
      enum e_status
      {
         status_enabled,
         status_disabled
      };

      enum e_category
      {
         category_General,
         category_COM,
         category_QI,
         category_Registrar,
         category_Refcount,
         category_Windowing,
         category_Controls,
         category_Hosting,
         category_DBClient,
         category_DBProvider,
         category_Snapin,
         category_NotImpl,
         category_Allocation,
         category_Exception,
         category_Time,
         category_Cache,
         category_Stencil,
         category_String,
         category_Map,
         category_Util,
         category_Security,
         category_Sync,
         category_ISAPI,

         category_User,
         category_User2,
         category_User3,
         category_User4,
         category_AppMsg = 500,        // main message pump trace (includes DDE)
         category_WinMsg,        // Windows message tracing
         category_CmdRouting,    // Windows command routing trace
         category_Ole,          // special OLE callback trace
         category_Database,     // special database trace
         category_Internet,     // special Internet client trace
         category_dumpContext,   // traces from dump_context
         category_Memory,      // generic non-kernel memory traces
         category_Html,         // Html traces
         category_Socket      // socket traces

      };


      class trace;


      // Declare a global instance of this class to automatically register a custom trace category at startup
      class CLASS_DECL_AURA category
      {
      public:


         category();
         ~category();


         UINT GetLevel() const NOTHROW;
         void SetLevel(UINT nLevel) NOTHROW;
         e_status GetStatus() const NOTHROW;
         void SetStatus(e_status eStatus) NOTHROW;

         operator uint32_t() const NOTHROW;

         uint32_t      m_dwCategory;
         string            m_strCategory;
         UINT              m_uiLevel;
         e_status          m_estatus;
      };


      class CLASS_DECL_AURA trace
      {
      public:

         ::aura::application * m_papp;

         trace(::aura::application * papp)
         {
            m_papp = papp;
         }

         ~trace()
         {
         }

         category & operator[](uint_ptr dwCategory)
         {
            return m_map[dwCategory];
         }

         void TraceV(const char *pszFileName,int32_t nLine,uint_ptr dwCategory,UINT nLevel,const char * pszFmt,va_list args) const;


         /*bool LoadSettings(const char * pszFileName = NULL) const
         { return 0 != gen_TraceLoadSettings(pszFileName);}
         void SaveSettings(const char * pszFileName = NULL) const
         { gen_TraceSaveSettings(pszFileName);}*/

         map < uint_ptr,uint_ptr,category,category > m_map;


      };



      CLASS_DECL_AURA void __cdecl __trace(const char * pszFormat,...);
      CLASS_DECL_AURA void __cdecl __trace(const unichar * pszFormat,...);
      CLASS_DECL_AURA void __cdecl __trace(uint_ptr dwCategory,UINT nLevel,const char * pszFormat,...);
      CLASS_DECL_AURA void __cdecl __trace(uint_ptr dwCategory,UINT nLevel,const unichar * pszFormat,...);

   } // namespace trace


};  // namespace aura


CLASS_DECL_AURA const char * get_windows_message_name(UINT nMsg);

