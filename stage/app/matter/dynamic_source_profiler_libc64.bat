call "%VS_VARS%" 

cl /I"." /I"%VOTAGUS_ROOT%app" /I"%VOTAGUS_ROOT%app/thirdparty/include/x64" /I"%VOTAGUS_ROOT%app/thirdparty/include" /I"%VOTAGUS_ROOT%app/thirdparty/appseed" /I"%VOTAGUS_ROOT%app/appseedcore" /I"%VOTAGUS_ROOT%app/appseedcore/ca2" /I"%VOTAGUS_ROOT%app/nodeapp/windows" /I"%VOTAGUS_ROOT%app/nodeapp/windows/include" /I"%VOTAGUS_ROOT%app/appseed" /I"%VOTAGUS_ROOT%app/appseed/cube" /I"%NETNODE_ROOT%net/netseed/ds/ca2" /I"%NETNODE_ROOT%net/netseed/ds/ca2/include" /I"%VOTAGUS_ROOT%time/dynamic_source/library/include" /Zi /nologo /O2 /Ob2 /Oi /Ot /Oy /W3 /WX- /D "_NETNODELITE_LIBRARY_DLL" /D "_WINDLL" /D "_MBCS" /GF- /Gm- /EHa /MD /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /openmp-   /Yu"framework.h" /Fp"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_library/netnodelite_library.pch" /Fa"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_library/%ITEM_DIR%" /Fo"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_library/%ITEM_DIR%" /Fd"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_library/vc110.pdb" /Gd /analyze-  /errorReport:queue   /Zm384 /bigobj /c "%VOTAGUS_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%VOTAGUS_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 
