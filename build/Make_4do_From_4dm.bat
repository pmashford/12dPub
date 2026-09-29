@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Error: No argument provided.
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

rem put this repo's \include\ first on the cc4d include path, keeping any user CPATH (unix-style ':' separator, not ';')
if defined CPATH (set "CPATH=!repohome!\include:!CPATH!") else (set "CPATH=!repohome!\include")

echo fullpath : %fullpath%
echo filePath : %filePath%
echo fileBase : %fileBase%
echo fileBaseExt : %fileBaseExt%
echo fileTarget : %fileTarget%
echo repoBin : %repoBin%
echo filePathRelative : %filePathRelative%
echo CPATH : %CPATH%

set "where=C:\Program Files\12d\12dmodel\15.00\nt.x64"
if not exist "%where%\cc4d.exe" (
    echo Error: cc4d.exe not found in "%where%"
    exit /b 1
)

echo compiling %fullpath% using %where%

REM START code to get QUOTED_CC4D_VERSION_DATA (send errors to file to capture the version data for cc4d.exe)
set "ERRORFILE=%TEMP%\cc4d_version_%RANDOM%.txt"
"%where%\cc4d.exe" 2> "%ERRORFILE%"
set "QUOTED_CC4D_VERSION_DATA="
for /f "delims=" %%a in ('type "%ERRORFILE%" ^| findstr "^Version"') do (
    set "QUOTED_CC4D_VERSION_DATA=%%a"
)

for /f "tokens=2,4,6 delims= " %%a in ("%QUOTED_CC4D_VERSION_DATA%") do (
    set "MACRO_VERSION_4D=%%a"
    set "MACRO_LAST_OPCODE_4D=%%b"
    set "MACRO_LAST_LIBRARY_CODE_4D=%%c"
)

echo:
echo CC4D.exe version information accessable from custom environment variable QUOTED_CC4D_VERSION_DATA
echo where QUOTED_CC4D_VERSION_DATA = "%QUOTED_CC4D_VERSION_DATA%"
echo where MACRO_VERSION_4D = "%MACRO_VERSION_4D%"
echo where MACRO_LAST_OPCODE_4D = "%MACRO_LAST_OPCODE_4D%"
echo where MACRO_LAST_LIBRARY_CODE_4D = "%MACRO_LAST_LIBRARY_CODE_4D%"
echo:
del "%ERRORFILE%"
REM END code to get QUOTED_CC4D_VERSION_DATA

cd /d "%filePath%"

rem compiler errors go to NAME.4dl next to the source (typed below so vscode's problem matcher still sees them)
set "logfile=%filePath%%fileBase%.4dl"
if exist "%logfile%" del "%logfile%"

rem pass in macros with -D
set mycmd="%where%\cc4d.exe" "%fullpath%" -allow_old_calls -log "%logfile%" -D"QUOTED_CC4D_VERSION_DATA=\"\\\"%QUOTED_CC4D_VERSION_DATA%\\\"\"" -DMACRO_VERSION_4D=%MACRO_VERSION_4D% -DMACRO_LAST_OPCODE_4D=%MACRO_LAST_OPCODE_4D% -DMACRO_LAST_LIBRARY_CODE_4D=%MACRO_LAST_LIBRARY_CODE_4D%
%mycmd%
set "ccexit=%errorlevel%"
echo %mycmd%

ECHO "========================================================="
ECHO " COMPILER RESULTS
ECHO "========================================================="
if exist "%logfile%" type "%logfile%"
ECHO "========================================================="

rem cc4d deletes the old .4do on a failed compile, so dont touch /bin/ (leaves the last good build there)
if not "%ccexit%"=="0" (
    echo COMPILE FAILED ^(cc4d exit code %ccexit%^) - bin not updated
    endlocal
    exit /b 1
)

echo COMPILE OK : %filePath%%fileBase%.4do (%date% %time: =%)

rem SKIP ALL THIS IF THE MACRO FILENAME CONTAINS test, WE DONT WANT TO COPY THESE TO /bin/
set "skipreason="
if not x%fileBase:test=%==x%fileBase% set "skipreason=test"
if not x%fileBase:mashy_lib=%==x%fileBase% set "skipreason=mashy_lib"
if defined skipreason (
    echo SKIPPED    : bin not updated ^(filename contains "%skipreason%"^)
    goto end
)

copy /y "%filePath%\%fileBase%.4do" "%repoBin%" >nul || (echo COPY FAILED : %repoBin%\%fileBase%.4do & endlocal & exit /b 1)
echo COPIED     : %repoBin%\%fileBase%.4do
copy /y "%filePath%\%fileBase%.4do" "%repoBin%\_macro_hot_off_the_press.4do" >nul || (echo COPY FAILED : %repoBin%\_macro_hot_off_the_press.4do & endlocal & exit /b 1)
echo COPIED     : %repoBin%\_macro_hot_off_the_press.4do

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
echo WROTE      : %infofile%

:end
echo DONE
@ECHO ON

@endlocal
