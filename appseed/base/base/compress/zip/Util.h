#pragma once


namespace zip
{


   class CLASS_DECL_BASE Util
   {
   public:


      Util();
      virtual ~Util();


      void ls(sp(::axis::application) papp, const char * lpszFileName, bool bRecursive, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, stringa * pstraRelative = NULL, bool_array * pbaIsDir = NULL, int64_array * piaSize = NULL, e_extract eextract = extract_first);
      void ls_dir(sp(::axis::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL);
      bool extract(sp(::axis::application) papp, const char * lpszFileName, const char * lpszExtractFileName);
      bool extract_all(const char * pszDir, ::file::buffer_sp  pfile);
      bool exists(sp(::axis::application) papp, const char * pszPath);
      bool IsUnzipable(sp(::axis::application) papp, const char * lpszFileName);
      bool HasSubFolder(sp(::axis::application) papp, const char * lpszFileName);


   };


} // namespace zip



