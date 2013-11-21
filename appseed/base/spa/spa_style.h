#pragma once




namespace spa
{

   class CLASS_DECL_BASE style :
      virtual public object
   {
   public:

      enum e_schema
      {

         schema_normal,
         schema_darker

      };

      e_schema                      m_eschema;
      string                        m_strLocale;
      string                        m_strSchema;
      string                        m_strRuri;


      style(sp(base_application) papp);

      virtual ~style();

      virtual string defer_get_plugin();

      virtual string defer_get(const char * pszUrl);

      virtual string calc_locale();
      virtual string calc_schema();


   };


} // namespace spa




