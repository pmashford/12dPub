rem textpad parameters $File $FileDir $BaseName "C:\Users\Public\12d\MY_USER\TEMP"
@echo off 

cls 

setlocal 
enabledelayedexpansion

set cpath=X:\12d\MACROS\_INCLUDES

if exist "C:\Program Files\12d\12dmodel\15.00\nt.x64\12d.exe" set where=C:\Program Files\12d\12dmodel\15.00\nt.x64

path %where%;%path% 
REM set CPLUS_INCLUDE_PATH=X:/12d/MACROS/_INCLUDES

echo. 
echo compiling %3.4dm using %where% 
echo. 



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

rem -macro_version 801 // is the only flag that really works... 801 was v8 (64bit? only?)

rem "%where%\cc4d.exe" -log X:\12d\Temp\temp.4dl %1  -allow_old_calls -D"QUOTED_CC4D_VERSION_DATA=\"\\\"%QUOTED_CC4D_VERSION_DATA%\\\"\""
rem "%where%\cc4d.exe" -list X:\12d\Temp\templist.4dl -log X:\12d\Temp\temp.4dl %1  -allow_old_calls -D"QUOTED_CC4D_VERSION_DATA=\"\\\"%QUOTED_CC4D_VERSION_DATA%\\\"\""
set mycmd="%where%\cc4d.exe" -macro_version 801 -log X:\12d\Temp\temp.4dl %1  -allow_old_calls -D"QUOTED_CC4D_VERSION_DATA=\"\\\"%QUOTED_CC4D_VERSION_DATA%\\\"\""
%mycmd%
echo %mycmd%

echo. 
echo updating macros 
echo. 

copy %2\%3.4do %4  
copy %2\%3.4do %4\_macro_hot_off_the_press.4do
rem copy %2\%3.4do C:\Users\Public\12d\TEMP\_macro_hot_off_the_press.4do

ECHO "=========================================================" 
ECHO " COMPILER RESULTS 
ECHO "========================================================="
type "X:\12d\Temp\temp.4dl"
ECHO "=========================================================" 
@ECHO ON

endlocal 