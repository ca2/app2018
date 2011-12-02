#pragma once


#include "ca/simpleuser/simpleuser.h"


namespace fs
{

   enum e_arrange
   {
      arrange_by_name,
   };

   enum e_flag
   {
      FlagInZip,
      FlagFolder,
      FlagFolderEx001,
      FlagFolderEx001Calc,
      FlagHasSubFolder,
      FlagHasSubFolderUnknown,
   };

} // namespace fs


#include "fs_item.h"
#include "fs_item_array.h"

#include "fs_data.h"
#include "fs_set.h"
#include "ifs_file.h"
#include "ifs.h"
#include "native_fs.h"


#include "fs_application.h"


