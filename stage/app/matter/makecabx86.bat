@echo off
mkdir ..\..\..\..\time\iexca2\
mkdir ..\..\..\..\time\iexca2\x86
del ..\..\..\..\time\iexca2\x86\iexca2.cab
del ..\..\..\..\time\iexca2\x86\iexca2.dll
del ..\..\..\..\time\iexca2\x86\app-install.exe
copy ..\..\..\..\stage\x86\iexca2.dll ..\..\..\..\time\iexca2\x86\iexca2.dll
copy ..\..\..\..\stage\x86\app-install.exe ..\..\..\..\time\iexca2\x86\app-install.exe
..\..\..\thirdparty\binary\signtool sign /f "C:\cecyn1.at.hotmail.com\ccvotagus\cgcl\2011-05-ca2.p12" /p none ..\..\..\..\time\iexca2\x86\iexca2.dll
..\..\..\thirdparty\binary\signtool sign /f "C:\cecyn1.at.hotmail.com\ccvotagus\cgcl\2011-05-ca2.p12" /p none ..\..\..\..\time\iexca2\x86\app-install.exe
IF NOT EXIST ..\..\..\..\time\iexca2\x86\iexca2.cab ECHO Deleted iexca2.cab
..\..\..\thirdparty\binary\cabarc N ..\..\..\..\time\iexca2\x86\iexca2.cab iexca2.inf ..\..\..\..\time\iexca2\x86\iexca2.dll ..\..\..\..\time\iexca2\x86\app-install.exe
IF NOT EXIST ..\..\..\..\time\iexca2\x86\iexca2.cab ECHO ERROR WAS HAPPEND!!! 
IF EXIST ..\..\..\..\time\iexca2\x86\iexca2.cab ECHO Copy was successfully
..\..\..\thirdparty\binary\signtool sign /f "C:\cecyn1.at.hotmail.com\ccvotagus\cgcl\2011-05-ca2.p12" /p none ..\..\..\..\time\iexca2\x86\iexca2.cab