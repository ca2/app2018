#pragma once


CLASS_DECL_c ::Windows::Storage::StorageFolder ^ get_os_folder(const char * lpcszDirName);
CLASS_DECL_c ::Windows::Storage::StorageFile ^ get_os_file(const char * lpcszFileName, dword dwDesiredAcces, dword dwShareMode, LPSECURITY_ATTRIBUTES lpSA, dword dwCreationDisposition, dword dwFlagsAndAttributes, HANDLE hTemplateFile);
CLASS_DECL_c bool get_file_time(::Windows::Storage::StorageFile ^ file, LPFILETIME lpCreationTime, LPFILETIME lpItemTime, LPFILETIME lpLastWriteTime);

