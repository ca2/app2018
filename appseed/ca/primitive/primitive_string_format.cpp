#include "framework.h"



void string_format::construct(string_format_printer * pprinter, string_format_printer::PRINTER pfnPrinter, void * pvoidPrinter)
{

   m_pszBuffer                   = NULL;
   m_iSize                       = 0;
   m_iLength                     = 0;

   m_pprinter                    = pprinter;
   m_pfnPrinter                  = pfnPrinter;
   m_pvoidPrinter                = pvoidPrinter;

   m_estate                      = state_initial;

   m_bLeftJustify                = false;
   m_bForceShowSign              = false;
   m_bSharp                      = false;
   m_bZeroPadding                = false;


   m_iWidth                      = -1;
   m_iPrecision                  = -1;
   m_chLength                    = '\0';
   m_chSpec                      = '\0';

}


string_format::~string_format()
{

   if(m_pszBuffer != NULL)
   {

      ca2_free(m_pszBuffer);

   }

}


void string_format::allocate_add_up(int iLenAddUp)
{

   if(m_iSize <= 0)
   {

      m_iSize = ((iLenAddUp + 1 + 1023) & ~1024);

      m_pszBuffer = (char *) ca2_alloc(m_iSize);


      if(m_pszBuffer == NULL)
         throw memory_exception();

   }
   else
   {

      m_iSize = ((m_iLength + iLenAddUp + 1 + 1023) & ~1024);

      m_pszBuffer = (char *) ca2_realloc(m_pszBuffer, m_iSize);

      if(m_pszBuffer == NULL)
         throw memory_exception();

   }

   memset(&m_pszBuffer[m_iLength], 0, m_iSize - m_iLength);

}


void string_format::format(int const & i)
{
   // TODO: use specs
   append(gen::str::from(i));

}

void string_format::format(int64_t const & i)
{
   // TODO: use specs
   append(gen::str::from(i));

}

void string_format::format(const void * const & p)
{
   // TODO: use specs
   append(gen::str::from((uint_ptr)p));

}



void string_format::format(const char * const & psz)
{

   append(psz);

}


bool string_format::parse(const char * & s)
{
   if(m_estate == state_initial)
   {
      while(*s != '\0')
      {
         if(*s == '-')
         {
            m_bLeftJustify = true;
         }
         else if(*s == '+')
         {
            m_bForceShowSign = true;
         }
         else if(*s == '#')
         {
            m_bSharp = true;
         }
         else if(*s == '0')
         {
            m_bZeroPadding = true;
         }
         else
         {
            break;
         }
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception("unfineshed argument specifier");
      }
      const char * start = s;
      while(*s != '\0')
      {
         if(*s == '*')
         {
            s++;
            m_estate = state_waiting_width;
            return true;
         }
         else if(isdigit_dup(*s))
         {
         }
         else
         {
            break;
         }
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception("unfineshed argument specifier");
      }
      if(s > start)
      {
         m_iWidth = natoi_dup(start, s - start);
      }
      m_estate = state_parse_precision;
   }
   if(m_estate == state_parse_precision)
   {
      if(*s == '.')
      {
         s++;
      }
      const char * start = s;
      while(*s != '\0')
      {
         if(*s == '*')
         {
            s++;
            m_estate = state_waiting_precision;
            return true;
         }
         else if(isdigit_dup(*s))
         {
         }
         else
         {
            break;
         }
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception("unfineshed argument specifier");
      }
      if(s > start)
      {
         m_iPrecision = natoi_dup(start, s - start);
      }
      m_estate = state_parse_length;
   }
   if(m_estate == state_parse_length)
   {
      if(*s == 'l' || *s == 'L' || *s == 'h')
      {
         m_chLength = *s;
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception("unfineshed argument specifier");
      }
      if(*s == 'c' || *s == 'd' || *s == 'i'
      || *s == 'e' || *s == 'E' || *s == 'f'
      || *s == 'g' || *s == 'G' || *s == 'o'
      || *s == 'g' || *s == 'G' || *s == 's'
      || *s == 'u' || *s == 'x' || *s == 'X'
      || *s == 'p')
      {
         m_chLength = *s;
         s++;
      }
      else
      {
         throw simple_exception("unfineshed format specifier");
      }
      return false;
   }
   else
   {
      throw "invalid state";
   }
}
