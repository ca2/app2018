#include "framework.h"

namespace fontopus
{

   license::license(sp(::ca::application) papp) :
      ca(papp)
   {
   }

   license::~license()
   {
   }

   bool license::has(const char * pszId, bool bInteractive)
   {
      class info info;
      if(m_mapInfo.Lookup(pszId, info))
      {
         return info.m_bLicensed;
      }
      else
      {
         return check(pszId, bInteractive);
      }
   }

   bool license::check(const char * pszId, bool bInteractive)
   {

      class info info;
      m_mapInfo.Lookup(pszId, info);
      info.m_strId = pszId;

      bool bLicensed = false;

      if(m_papp != ::null() && m_papp->m_psession != ::null() && m_papp->m_psession->m_pbergedge != ::null())
      {
         bLicensed = App(m_papp->m_psession->m_pbergedgeInterface).m_pfontopus->check_license(pszId, bInteractive);
      }
      else
      {
         bLicensed = Application.m_pfontopus->check_license(pszId, bInteractive);   
      }
      
      info.m_bLicensed = bLicensed;
      m_mapInfo.set_at(pszId, info);
      return info.m_bLicensed;

   }

   ::count license::clear_all_cache()
   {
      ::count count = m_mapInfo.get_count();
      m_mapInfo.remove_all();
      return count;
   }

   bool license::clear_cache(const char * pszId)
   {
      return m_mapInfo.remove_key(pszId) != FALSE;
   }


} // namespace fontopus



