
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cd /d "C:\Users\1234\Desktop\Projects\FYP\GenerateEXE\Dev\Generate-EXE\updated"
cl /c /Fo:Layout01.obj Layout01.c
rc Layout01.rc
link /DLL /DEF:Layout01.def /OUT:C:\Users\1234\Desktop\Projects\FYP\GenerateEXE\Dev\Generate-EXE\compiled\Layout01.dll Layout01.obj Layout01.res
