call "%VS_VARS%" x86

cl /I"." /I"%VOTAGUS_ROOT%app/seed/metaseed/../../" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../thirdparty/include" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../thirdparty/appseed" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../appseedcore" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../appseedcore/ca2" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../nodeapp/platform/windows/win32" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../nodeapp/platform/windows/win32/include" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../appseed" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../appseed/cube" /I"%VOTAGUS_ROOT%net/netseed/ds/ca2" /I"%VOTAGUS_ROOT%net/netseed/ds/ca2/include" /I"%VOTAGUS_ROOT%time/netnode/library/include" /D "_DEBUG" /ZI /nologo /W3 /WX- /Od /Oy- /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_NETNODESCRIPT_DLL" /D "_WINDLL" /D "_MBCS" /Gm- /EHa /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Yu"StdAfx.h" /Fp"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodescript/netnodescript.pch" /Fa"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodescript/%ITEM_NAME%/" /Fo"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodescript/%ITEM_NAME%/" /Fd"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodescript/%ITEM_NAME%/vcwindows7.1sdk.pdb" /Gd /errorReport:queue  -Zm284 /bigobj /c "%VOTAGUS_ROOT%time/netnode/%ITEM_NAME%.cpp" > "%VOTAGUS_ROOT%time/netnode/%ITEM_NAME%-compile-log.txt"

link /OUT:"%TARGET_PATH%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%VOTAGUS_ROOT%app\seed\metaseed\../../../time/library/%PLATFORM%/" /LIBPATH:"%VOTAGUS_ROOT%app\seed\metaseed\../../thirdparty/library/%LIBPLATFORM%" /DLL "_.lib" "freeimage.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "zlib.lib" %LIBS_LIBS%  /MANIFEST:NO /ManifestFile:"%VOTAGUS_ROOT%app\seed\metaseed\..\..\..\time\intermediate\%PLATFORM%\netnodescript\netnodescript.dll.intermediate.manifest" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /DEF:"%VOTAGUS_ROOT%app/appseed/netnodescript/netnodescript.def" /DEBUG /PDB:"%TARGET_PATH%.pdb" /SUBSYSTEM:WINDOWS /PGD:"%TARGET_PATH%.pgd" /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE "%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodescript/StdAfx.obj" "%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodescript/dllmain.obj"  "%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodescript/%ITEM_NAME%/%ITEM_TITLE%.obj" > "%VOTAGUS_ROOT%time/netnode/%ITEM_NAME%-link-log.txt"
