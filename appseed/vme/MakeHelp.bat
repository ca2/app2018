@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by VME.HPJ. >"hlp\vme.hm"
echo. >>"hlp\vme.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\vme.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\vme.hm"
echo. >>"hlp\vme.hm"
echo // Prompts (IDP_*) >>"hlp\vme.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\vme.hm"
echo. >>"hlp\vme.hm"
echo // Resources (IDR_*) >>"hlp\vme.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\vme.hm"
echo. >>"hlp\vme.hm"
echo // Dialogs (IDD_*) >>"hlp\vme.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\vme.hm"
echo. >>"hlp\vme.hm"
echo // Frame Controls (IDW_*) >>"hlp\vme.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\vme.hm"
REM -- Make help for Project VME


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\vme.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\vme.hlp" goto :Error
if not exist "hlp\vme.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\vme.hlp" Debug
if exist Debug\nul copy "hlp\vme.cnt" Debug
if exist Release\nul copy "hlp\vme.hlp" Release
if exist Release\nul copy "hlp\vme.cnt" Release
echo.
goto :done

:Error
echo hlp\vme.hpj(1) : error: Problem encountered creating help file

:done
echo.
