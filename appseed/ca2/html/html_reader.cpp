#include "framework.h"


namespace html
{


   reader::reader()
   {
      m_ptag = ::null();
      m_ptagMain = ::null();
   }

   reader::~reader()
   {

      if(m_ptagMain != ::null())
      {
         delete m_ptagMain;
      }

      m_ptag      = ::null();
      m_ptagMain  = ::null();

   }

   void reader::BeginParse(uint_ptr dwAppData, bool &bAbort)
   {
      UNREFERENCED_PARAMETER(dwAppData);
      m_ptag = ::null();
      m_ptagMain = ::null();
      bAbort = false;
   }

   void reader::StartTag(lite_html_tag *pTag, uint_ptr dwAppData, bool &bAbort)
   {
      UNREFERENCED_PARAMETER(dwAppData);
      if(m_ptag == ::null())
      {
         m_ptag = new ::html::tag(::null());
         m_ptagMain = m_ptag;
      }
      else
      {
         ::html::tag * ptag = new ::html::tag(m_ptag);
         m_ptag->baseptra().add(ptag);
         m_ptag = ptag;
      }
      m_ptag->set_name(pTag->getTagName());
      if(pTag->getAttributes() != ::null())
      {
         for(int32_t i = 0; i < pTag->getAttributes()->getCount(); i++)
         {
             m_ptag->attra().add(canew(attribute()));
            attribute * pattr = m_ptag->attra().last_sp();
            pattr->set_name(pTag->getAttributes()->getAttribute(i).getName().make_lower());
            pattr->set_value(pTag->getAttributes()->getAttribute(i).getValue());
         }
      }
      if(m_ptag->get_parent() != ::null() && pTag->getTagName() == "visual")
      {
         m_ptag = m_ptag->get_parent();
      }
      bAbort = false;
   }

   void reader::EndTag(lite_html_tag *pTag, uint_ptr dwAppData, bool &bAbort)
   {
      UNREFERENCED_PARAMETER(pTag);
      UNREFERENCED_PARAMETER(dwAppData);
      if(m_ptag->get_parent() != ::null() && pTag->getTagName() != "visual")
      {
         m_ptag = m_ptag->get_parent();
      }

      bAbort = false;
   }

   void reader::Characters(const string &rText, uint_ptr dwAppData, bool &bAbort)
   {
      UNREFERENCED_PARAMETER(dwAppData);
      if(m_ptag != ::null())
      {
         ::html::value * pvalue = new ::html::value(m_ptag);
         m_ptag->baseptra().add(pvalue);
         pvalue->set_value(rText);
      }
      bAbort = false;
   }

   void reader::Comment(const string &rComment, uint_ptr dwAppData, bool &bAbort)
   {
      UNREFERENCED_PARAMETER(rComment);
      UNREFERENCED_PARAMETER(dwAppData);
      bAbort = false;
   }

   void reader::EndParse(uint_ptr dwAppData, bool bIsAborted)
   {
      UNREFERENCED_PARAMETER(dwAppData);
      UNREFERENCED_PARAMETER(bIsAborted);
   }

   tag * reader::detach_main_tag()
   {

      tag * ptagMain    = m_ptagMain;


      m_ptagMain        = ::null();
      m_ptag            = ::null();


      return ptagMain;

   }


} // namespace html



