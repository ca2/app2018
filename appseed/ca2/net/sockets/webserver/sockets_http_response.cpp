#include "framework.h"

namespace http
{

   // --------------------------------------------------------------------------------------
   response::response(sp(::ca2::application) papp, const string & version) :
   ca2(papp),
   transaction(papp),
   m_memfileBody(papp )
   {
      UNREFERENCED_PARAMETER(version);
   }


   // --------------------------------------------------------------------------------------
   response::response(const response& src) :
   ::ca2::ca2(((response &)src).get_app()),
   transaction(src)
   , m_memfileBody(((response &)src).get_app() )
   {
      m_memfileBody = src.m_memfileBody;
   }


   // --------------------------------------------------------------------------------------
   response::~response()
   {
   }


   // --------------------------------------------------------------------------------------
   response& response::operator=(const response& src)
   {
      m_memfileBody        = src.m_memfileBody;

      transaction::operator=(src);

      return *this;
   }

   // --------------------------------------------------------------------------------------
   /*void response::SetFile( const string & path )
   {
      m_file = std::auto_ptr<IFile>(new File);
      m_file -> fopen( path, "rb" );
   }*/

   // --------------------------------------------------------------------------------------
   void response::clear()
   {
      transaction::clear();
      file().Truncate(0);
   //   m_file = std::auto_ptr<IFile>(new MemFile);
   }

} // namespace http 


