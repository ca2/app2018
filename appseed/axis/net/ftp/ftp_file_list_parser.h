////////////////////////////////////////////////////////////////////////////////
//
// Code taken from D. J. Bernstein (C ==> C++)
//
// ftpparse.c, ftpparse.h: library for parsing FTP LIST responses
// 20001223
// D. J. Bernstein, djb@cr.yp.to
// http://cr.yp.to/ftpparse.html
//
// Commercial use is fine, if you let me know what programs you're using this in.
//
// Currently covered formats:
// EPLF.
// UNIX ls, with or without gid.
// Microsoft FTP Service.
// Windows NT FTP Server.
// VMS.
// WFTPD.
// NetPresenz (Mac).
// NetWare.
// MSDOS.
//
// Definitely not covered: 
// Long VMS filenames, with information split across two lines.
// NCSA Telnet FTP server. Has LIST = NLST (and bad NLST for directories).
//
////////////////////////////////////////////////////////////////////////////////

#pragma once



namespace ftp
{


   class file_status;

   class ifile_list_parser : 
      virtual public iinterface
   {
   public:
      virtual ~ifile_list_parser() {}
      virtual bool Parse(file_status& ftpFileStatus, const string& strLineToParse) = 0;
   };



   /// Implements the parsing of the string returned by the LIST command.
   class file_list_parser : 
      virtual public ifile_list_parser
   {
   public:

      time_t        m_tmBase;       // time() value on this OS at the beginning of 1970 TAI
      long          m_lCurrentYear; // approximation to current year
      static const TCHAR* m_Months[12];

      file_list_parser();
      virtual ~file_list_parser();

      bool Parse(file_status& ftpFileStatus, const string& strLineToParse);

      bool IsEPLS(LPCTSTR pszLine);
      bool ParseEPLF(file_status& ftpFileStatus, LPCTSTR pszLine, int iLength);

      bool IsUNIXStyleListing(LPCTSTR pszLine);
      bool ParseUNIXStyleListing(file_status& ftpFileStatus, LPCTSTR pszLine, int iLength);

      bool IsMultiNetListing(LPCTSTR pszLine);
      bool ParseMultiNetListing(file_status& ftpFileStatus, LPCTSTR pszLine, int iLength);

      bool IsMSDOSListing(LPCTSTR pszLine);
      bool ParseMSDOSListing(file_status& ftpFileStatus, LPCTSTR pszLine, int iLength);

      bool CheckMonth(LPCTSTR pszBuffer, LPCTSTR pszMonthName) const;
      int  GetMonth(LPCTSTR pszBuffer, int iLength) const;
      bool GetLong(LPCTSTR pszLong, int iLength, long& lResult) const;
      long GetYear(time_t time) const;

      long ToTAI(long lYear, long lMonth, long lMDay) const;
      long GuessTAI(long lMonth, long lMDay);

   };


} // namespace ftp





