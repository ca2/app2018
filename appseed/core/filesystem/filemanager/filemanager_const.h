#pragma once


namespace filemanager
{

   enum e_flag
   {

      flag_zip_compressed,
      flag_folder,
      flag_folder_ex001,
      flag_folder_ex001_calc,
      flag_has_sub_folder,
      flag_has_sub_folder_unknown

   };

   
   enum e_attribute
   {

      attribute_name

   };

   enum EFolder
   {


   FolderMyComputer,


   };


} // namespace filenamager


#define FILE_MANAGER_ID_FILE_NAME 1




#define FILEMANAGER_SHELL_COMMAND_FIRST 0x1000
#define FILEMANAGER_SHELL_COMMAND_LAST 0x2000


