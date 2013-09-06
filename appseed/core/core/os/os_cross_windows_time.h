#pragma once


#ifdef METROWIN

struct timeval
{
   __int32    tv_sec;         /* seconds */
   __int32    tv_usec;        /* microseconds */
};

#endif

CLASS_DECL_ca2 int_bool WINAPI LocalFileTimeToFileTime(const FILETIME *localft, LPFILETIME utcft);
CLASS_DECL_ca2 int_bool WINAPI FileTimeToLocalFileTime(const FILETIME *utcft, LPFILETIME localft);
#ifndef METROWIN
int_bool FileTimeToSystemTime(const FILETIME *ft, LPSYSTEMTIME syst);
#endif
#ifndef METROWIN
int_bool SystemTimeToFileTime(const SYSTEMTIME *syst, LPFILETIME ft);
#endif


#ifndef METROWIN

void GetSystemTimeAsFileTime(LPFILETIME time); /* [out] Destination for the current utc time */
void GetSystemTime(LPSYSTEMTIME systime);

#endif


