@echo off
mkdir build
pushd build

rem ServerFiles
rem ClientFiles
rem CommonIncludes
rem ClientLibsPath
rem CommonLibs
rem ClientLibs
rem ClientIncludes

set ServerFiles=..\TCPServer\*.cpp ..\common\*.cpp
set ClientFiles=..\TCPClient\*.cpp ..\TCPClient\glad.c ..\common\*.cpp
set CommonIncludes=/I ..\common
set ClientIncludes=/I ..\Libs\include
set ClientLibsPath=/LIBPATH:"..\Libs\lib\"
set ClientLibs=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glfw3.lib
set CommonLibs=Ws2_32.lib

rem ServerCompile
rem ClientCompile
set ServerCompile=cl /DDEBUG /Feserver.exe -FC -Zi %ServerFiles% /I ..\TCPServer %CommonIncludes% %CommonLibs%
set ClientCompile=cl /DDEBUG /Feclient.exe /MD -FC -Zi %ClientFiles% /I ..\TCPClient %CommonIncludes% %ClientIncludes% /link %ClientLibsPath%  %CommonLibs% %ClientLibs% /NODEFAULTLIB:MSVCRTD -subsystem:windows

IF "%1"=="c" GOTO CLIENT_COMPILE
%ServerCompile%
IF "%1"=="s" GOTO END
:CLIENT_COMPILE
%ClientCompile%
:END
popd 