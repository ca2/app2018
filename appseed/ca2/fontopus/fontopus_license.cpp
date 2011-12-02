#include "StdAfx.h"

namespace fontopus
{

   license::license(::ax::application * papp) :
      ax(papp)
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
      class validate authuser(get_app(), "system\\user\\authenticate.xhtml", true, bInteractive);
      info.m_bLicensed = authuser.get_license(pszId);
      m_mapInfo.set_at(pszId, info);
      return info.m_bLicensed;
   }

   count license::clear_all_cache()
   {
      count count = m_mapInfo.get_count();
      m_mapInfo.remove_all();
      return count;
   }

   bool license::clear_cache(const char * pszId)
   {
      return m_mapInfo.remove_key(pszId) != FALSE;
   }


} // namespace fontopus



