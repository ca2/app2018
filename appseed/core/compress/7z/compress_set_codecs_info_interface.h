#pragma once

namespace libcompress
{

   class CLASS_DECL_CORE set_codecs_info_interface :
      virtual public ::ca::object
   {
   public:
      virtual ::ca::HRes SetCompressCodecsInfo(codecs_info_interface *compressCodecsInfo) = 0;
   };

} // namespace libcompress
