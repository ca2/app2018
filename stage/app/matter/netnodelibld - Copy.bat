call "%VS_VARS%"

cd "%VOTAGUS_ROOT%appseed\ca2\fontopus\net\ds\common\netnode\"

link /OUT:"%VOTAGUS_ROOT%stage/ca2/fontopus/app/main/front/Debug/%ITEM_NAME%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%VOTAGUS_ROOT%appseedcore/ca2core/fontopus/app/source/main/library/" /LIBPATH:"%VOTAGUS_ROOT%time/ca2/fontopus/app/main/build/library/Debug" /LIBPATH:"%VOTAGUS_ROOT%time/ca2/fontopus/app/main/build/library/Debug/library" /LIBPATH:"%VOTAGUS_ROOT%appseed/ca2/fontopus/app/source/main/library" /DLL /MANIFEST /MANIFESTFILE:"%VOTAGUS_ROOT%time/ca2/fontopus/app/main/build/intermediate/Debug/netnodelibrary/%ITEM_NAME%.dll.intermediate.manifest" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /IMPLIB:"%VOTAGUS_ROOT%time/ca2/fontopus/app/main/build/library/Debug/%ITEM_NAME%.lib"  /DEF:"%VOTAGUS_ROOT%appseed\ca2\fontopus\app\source\main\netnodelibrary/netnode_script.def" /DEBUG /PDB:"%VOTAGUS_ROOT%stage\ca2\fontopus\app\main\front\Debug\%ITEM_NAME%.pdb" /SUBSYSTEM:WINDOWS /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X86 /ERRORREPORT:PROMPT freeimage.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mysqlclientd.lib "%VOTAGUS_ROOT%time\ca2\fontopus\app\main\build\intermediate\Debug\netnodelibrary\StdAfx.obj" "%VOTAGUS_ROOT%time\ca2\fontopus\app\main\build\intermediate\Debug\netnodelibrary\dllmain.obj" %OBJS% > "%VOTAGUS_ROOT%time\ca2\fontopus\net\ds\common\netnode\%ITEM_NAME%-link-log.txt"
