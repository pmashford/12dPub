@echo off
setlocal enabledelayedexpansion

set "fullpath=c:\github\12dpub\src\dir1\dir2\dirwe\file.4dm"

:: Remove everything after \src\ (including \src\)
set "pathbeforesrc=%fullpath:\src\=" & rem %"

for %%I in ("!fullpath!") do (
	echo fullpath: !fullpath!
	set "filePath=%%~dpI"
	set "fileBase=%%~nI"
	set "fileBaseExt=%%~nxI"
)
set "fileTarget=%pathbeforesrc%\bin\%fileBase%.4do"

echo fullpath : %fullpath%
echo filePath : %filePath%
echo fileBase : %fileBase%
echo fileBaseExt : %fileBaseExt%
echo fileTarget : %fileTarget%

endlocal