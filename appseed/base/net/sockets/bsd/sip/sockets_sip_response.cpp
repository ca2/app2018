#include "framework.h" // #include "base/net/sockets/bsd/sockets.h"

namespace sip
{

   // --------------------------------------------------------------------------------------
   response::response(::aura::application * papp, const string & version) :
   element(papp),
   transaction(papp),
   m_memfileBody(papp )
   {
      UNREFERENCED_PARAMETER(version);
   }


   // --------------------------------------------------------------------------------------
   response::response(const response& src) :
   element(((response &)src).get_app()),
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

      ::sip::transaction::operator=(src);

      return *this;
   }

   // --------------------------------------------------------------------------------------
   /*void response::SetFile( const string & path )
   {
      m_file = smart_pointer<IFile>(new File);
      m_file -> fopen( path, "rb" );
   }*/

   // --------------------------------------------------------------------------------------
   void response::clear()
   {
      transaction::clear();
      file().Truncate(0);
   //   m_file = smart_pointer<IFile>(new MemFile);
   }

} // namespace sip


