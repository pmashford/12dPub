@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Error: No argument provided.
    echo Error: No argument provided. > X:\12d\Temp\temp.4dl
    exit /b 1
)

set "fullpath=%~1"
set "fullpath2=%~1"

:: Remove everything after \src\ (including \src\)
set "repohome=%fullpath2:\src\=" & rem %"
:: Remove everything after \src\ (but keep \src\)
set "filePathRelative=%fullpath2:*\src\=\src\%"


for %%I in ("!fullpath!") do (
	set "filePath=%%~dpI"
	set "fileBase=%%~nI"
	set "fileBaseExt=%%~nxI"
)

set "repoBin=!repohome!\bin"

echo fullpath : %fullpath%
echo filePath : %filePath%
echo fileBase : %fileBase%
echo fileBaseExt : %fileBaseExt%
echo fileTarget : %fileTarget%
echo repoBin : %repoBin%
echo filePathRelative : %filePathRelative%

if exist "C:\Program Files\12d\12dmodel\15.00\nt.x64\12d.exe" set where=C:\Program Files\12d\12dmodel\15.00\nt.x64

echo compiling %fullpath% using %where% 

REM START code to get QUOTED_CC4D_VERSION_DATA 
set ERRORFILE=C:\TEMP\temp_cmd_redirect_errors.txt
"%where%\cc4d.exe" 2> %ERRORFILE%
set "QUOTED_CC4D_VERSION_DATA="
for /f "delims=" %%a in ('type %ERRORFILE% ^| findstr "^Version"') do (
    set "QUOTED_CC4D_VERSION_DATA=%%a"
)
echo:
echo CC4D.exe version information accessable from custom environment variable QUOTED_CC4D_VERSION_DATA
echo where QUOTED_CC4D_VERSION_DATA = "%QUOTED_CC4D_VERSION_DATA%"
echo:
del %ERRORFILE%
REM END code to get QUOTED_CC4D_VERSION_DATA 

cd /d "%filePath%"

rem ! DONT LOG TO FILE, LET VSCODE DO FROM STDERR? set mycmd="%where%\cc4d.exe" -log X:\12d\Temp\temp.4dl "%fullpath%"  -allow_old_calls -D"QUOTED_CC4D_VERSION_DATA=\"\\\"%QUOTED_CC4D_VERSION_DATA%\\\"\""
set mycmd="%where%\cc4d.exe" "%fullpath%" -allow_old_calls -D"QUOTED_CC4D_VERSION_DATA=\"\\\"%QUOTED_CC4D_VERSION_DATA%\\\"\""
%mycmd%
echo %mycmd%

rem SKIP ALL THIS IF THE MACRO FILENAME CONTAINS test, WE DONT WANT TO COPY THESE TO /bin/
if not x%fileBase:test=%==x%fileBase% goto end

echo. 
echo updating macros 
echo. 

copy "%filePath%\%fileBase%.4do" "%repoBin%"
copy "%filePath%\%fileBase%.4do" "%repoBin%\_macro_hot_off_the_press.4do"

set "infofile=%repoBin%\%fileBase%.json"
echo { > "%infofile%"
echo "file": "%fileBase%.4do", >> "%infofile%"
echo "compileDate": "%date:~-10%" , >> "%infofile%"
echo "compileTime": "%time: =%" , >> "%infofile%"
echo "sourceBasename": "%fileBaseExt%" , >> "%infofile%"
echo "sourceDirname": "%filePathRelative:\=/%", >> "%infofile%"
echo "compiler": "%where:\=/%/cc4d.exe", >> "%infofile%"
echo "compilerInfo": "%QUOTED_CC4D_VERSION_DATA:\=/%" >> "%infofile%"
echo } >> "%infofile%"

:end

ECHO "=========================================================" 
ECHO " COMPILER RESULTS 
ECHO "========================================================="
type "X:\12d\Temp\temp.4dl"
ECHO "=========================================================" 
@ECHO ON

endlocal 