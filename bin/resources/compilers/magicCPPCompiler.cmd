::SET VS=C:\VTS
::echo %VS%
::if		exist %VS% echo 0 Folder already exists
::if	not exist %VS% echo 1 Folder does not exist
::pause

::set arg1=%1
::set arg2=%2
::SET VS=C:\Program Files (x86)\Microsoft Visual Studio\2019
::CALL "%VS%\Community\Common7\Tools\VsDevCmd.bat"
::cl "%arg1%" /Fe%arg2%


set arg1="%1"
set arg2="%2"

SET VS="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
if exist %VS% goto Compile

SET VS="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
if exist %VS% goto Compile

SET VS="D:\programs\VS 2022\Common7\Tools\VsDevCmd.bat"
if exist %VS% goto Compile

goto NoCompiler

:NoCompiler
echo ERROR: Compiler not found!
goto CommonExit

:Compile
echo Take compiler from %VS%
CALL %VS%
cl %arg1% /Zc:__cplusplus[/std:c++17] /Fe%arg2% 

:CommonExit