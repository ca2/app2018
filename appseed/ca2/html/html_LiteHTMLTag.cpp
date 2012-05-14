#include "framework.h"


lite_html_tag::lite_html_tag(lite_html_tag &rSource, bool bCopy) throw(memory_exception)
   :m_pcollAttr(NULL)
{
   m_strTagName = rSource.m_strTagName;
   if (!bCopy)
   {
      m_pcollAttr = rSource.m_pcollAttr;
      rSource.m_pcollAttr = NULL;
   }
   else if (rSource.m_pcollAttr != NULL)
   {
      /** DEEP COPY BEGIN */
      try
      {
         m_pcollAttr = new LiteHTMLAttributes(*(rSource.m_pcollAttr), true);
      }
      catch(memory_exception * pe)
      {
         ::ca::rethrow(pe);
      }
         
      /** DEEP COPY END */
   }
}

lite_html_tag::~lite_html_tag()
{
   SAFE_DELETE_POINTER(m_pcollAttr);
}

/**
 * lite_html_tag::parseFromStr
 * 
 * @param rStr - string to parse
 * @param bIsOpeningTag - receives true if the tag parsed 
 *        is a opening tag, false otherwise.
 * @param bIsClosingTag - receives true if the tag parsed 
 *        is a closing tag, false otherwise.
 * @param bParseAttrib - determines whether attribute/value 
 *        pairs should be parsed. Default is true.
 *
 * @return number of characters successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
UINT lite_html_tag::parseFromStr(::lite_html_reader * preader, const char * lpszString, 
                              bool &bIsOpeningTag, 
                              bool &bIsClosingTag, 
                              bool bParseAttrib /* = true */)
{
   ASSERT(__is_valid_string(lpszString));

   bool            bClosingTag = false;
   bool            bOpeningTag = false;
   LiteHTMLAttributes   *pcollAttr = NULL;
   string            strTagName;
   UINT            nRetVal = 0U, 
                  nTemp = 0U;
   const char *            lpszBegin = lpszString;
   const char *            lpszEnd = NULL;

   // skip leading white-space characters
   while (::_istspace(*lpszBegin))
      lpszBegin = ::_tcsinc(lpszBegin);

   // HTML tags always begin with a less-than symbol
   if (*lpszBegin != '<')
      return (0U);
   
   // skip tag's opening delimeter '<'
   lpszBegin = ::_tcsinc(lpszBegin);

   // optimization for is_empty opening tags
   if (*lpszBegin == '>')
   {
      ASSERT(strTagName.is_empty());
      ASSERT(pcollAttr == NULL);
      ASSERT(!bClosingTag);
      nRetVal = (UINT) (lpszBegin - lpszString);
      goto LUpdateAndExit;
   }

   // tag names always begin with an alphabet
   if (!::_istalpha(*lpszBegin))
   {
      bClosingTag = (*lpszBegin == '/');
      if (bClosingTag)
         lpszBegin = ::_tcsinc(lpszBegin);
      else
         return (0U);
   }

   bOpeningTag = !bClosingTag;
   lpszEnd = lpszBegin;
   do
   {
      // tag name may contain letters (a-z, A-Z), digits (0-9), 
      // underscores '_', hyphen '-', colons ':', and periods '.'
      if ( (!::_istalnum(*lpszEnd)) && 
          (*lpszEnd != '-') && (*lpszEnd != ':') && 
          (*lpszEnd != '_') && (*lpszEnd != '.') )
      {
         ASSERT(lpszEnd != lpszBegin);
         
         // only white-space characters, a null-character, a 
         // greater-than symbol, or a forward-slash can break 
         // a tag name
         if (*lpszEnd == NULL || ::_istspace(*lpszEnd) || 
            *lpszEnd == '>' || 
            (*lpszEnd == '/' && (!bClosingTag)) )
         {
            break;
         }

         return (0U);   // any other character will fail parsing process
      }

      lpszEnd = ::_tcsinc(lpszEnd);
   }
   while(true);

   // store tag name for later use
   strTagName = string(lpszBegin, lpszEnd - lpszBegin);

   // is this a closing tag?
   if (bClosingTag)
   {
      // in a closing tag, there can be only one symbol after 
      // tag name, i.e., the tag end delimeter '>'. Anything 
      // else will result in parsing failure
      if (*lpszEnd != '>')
         return (0U);

      // skip tag's ending delimeter
      lpszEnd = ::_tcsinc(lpszEnd);

      ASSERT(strTagName.get_length());
      ASSERT(pcollAttr == NULL);
      nRetVal = (UINT) (lpszEnd - lpszString);
      goto LUpdateAndExit;
   }

   // tag contains attribute/value pairs?
   if (*lpszEnd != '>')
   {
      lpszBegin = lpszEnd;
      lpszEnd = NULL;

      // skip white-space characters after tag name
      while (::_istspace(*lpszBegin))
         lpszBegin = ::_tcsinc(lpszBegin);

      nTemp = 0U;
      if (bParseAttrib)   // parse attribute/value pairs?
      {
         ASSERT(pcollAttr == NULL);
         // instantiate collection ...
         if ((pcollAttr = new LiteHTMLAttributes) == NULL)
         {
//            TRACE0("(Error) lite_html_tag::parseFromStr: Out of primitive::memory.\n");
            throw memory_exception();
            return (0U);
         }

         // ... and delegate parsing process
         nTemp = (UINT) pcollAttr->parseFromStr(preader, lpszBegin);
      }

      if (nTemp == 0)   // attribute/value pair parsing is disabled? 
                  //   - OR - 
                  // attribute/value pairs could not be parsed?
      {
         SAFE_DELETE_POINTER(pcollAttr);
         if ((lpszEnd = ::_tcsstr(lpszBegin, "/>")) == NULL)
         {
            if ((lpszEnd = ::_tcschr(lpszBegin, '>')) == NULL)
               return (0U);
         }
      }
      else
      {
         lpszEnd = lpszBegin + nTemp;

         // skip white-space after attribute/value pairs
         while (::_istspace(*lpszEnd))
            lpszEnd = ::_tcsinc(lpszEnd);
         
         // tag's ending delimeter could not be found?
         if (*lpszEnd == NULL)
         {
            SAFE_DELETE_POINTER(pcollAttr);
            return (0U);
         }
      }

      // a tag like this one: <BR/>
      if (*lpszEnd == '/')
      {
         ASSERT(bOpeningTag);
         bClosingTag = true;
         lpszEnd = ::_tcsinc(lpszEnd);
      }
   }

   // HTML tags always end with a greater-than '>' symbol
   if (*lpszEnd != '>')
   {
      SAFE_DELETE_POINTER(pcollAttr);
      return (0U);
   }
   else
      lpszEnd = ::_tcsinc(lpszEnd);

   nRetVal = (UINT) (lpszEnd - lpszString);
   goto LUpdateAndExit;   // just to show the flow-of-control

LUpdateAndExit:
   m_strTagName = strTagName;
   if(strTagName.CompareNoCase("BR") == 0)
   {
      bIsClosingTag = true;
      bIsOpeningTag = true;
   }
   else
   {
      bIsClosingTag = bClosingTag;
      bIsOpeningTag = bOpeningTag;
   }
   m_strTagName.trim_left();   m_strTagName.trim_right();   // just-in-case!
   SAFE_DELETE_POINTER(m_pcollAttr);
   m_pcollAttr = pcollAttr;
   pcollAttr = NULL;

   return (nRetVal);
}

