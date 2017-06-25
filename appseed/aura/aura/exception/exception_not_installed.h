#pragma once


class CLASS_DECL_AURA not_installed : 
   virtual public ::exception::base
{
public:


   string      m_strAppId;
   string      m_strAppType;
   string      m_strConfiguration;
   string      m_strPlatform;
   string      m_strLocale;
   string      m_strSchema;


   not_installed(::aura::application * papp, const char * pszAppId, const char * pszAppType);
   not_installed(::aura::application * papp, const char * pszAppId, const char * pszAppType, const char * pszConfiguration, const char * pszPlatform, const char * pszLocale, const char * pszSchema);
   not_installed(const not_installed & e);
   virtual ~not_installed();


};
