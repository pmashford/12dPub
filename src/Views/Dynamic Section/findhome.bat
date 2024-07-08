REM find home
cls
@echo off
setlocal enabledelayedexpansion


set "fullpath=C:\GitHub\12dPub\srsc\Views\Dynamic Section\myfile.4dm"

set "outer=!fullpath!" 
:looppath
	
for %%I in ("!outer!") do (
	echo fullpath: !fullpath!
	set "drive=%%~dI"
	set "parentpath=%%~dpI"
	set "parentpath=!parentpath:~0,-1!"
	echo parentpath: !parentpath!
	


	for %%I in ("!parentpath!") do set "foldername=%%~nxI"
	echo foldername: !foldername!
	echo drive: !drive!
	
	if /i "!foldername!"=="!drive!" goto :notfound
	if /i "!foldername!"=="src" (
	    echo Resulting path: !parentpath!
	    goto :found
	) else (
	    set "outer=!parentpath!"
	    goto :looppath
	)
)

:notfound
echo NOT FOUND!
goto :end

:found
echo found
echo Using path: !parentpath!
goto :end

goto :end

echo fullpath=!fullpath!
for %%I in ("%fullpath%") do set "drivepath=%%~dpI"
echo drivepath=!drivepath!
REM walk back to src


set "4dofolder"=



:loop
for %%I in ("!fullpath!") do set "parent=%%~dpI"
echo parent: !parent!
set "parent=!parent:~0,-1!"
echo parent: !parent!
REM set "drive=%%~nxI"
set "drive=!parent:~0,2!"
echo drive: !drive!

for %%I in ("!parent!") do set "foldername=%%~nxI"
echo foldername: !foldername!
echo foldername: !foldername!

if /i "!foldername!"=="src" (
    echo Resulting path: !parent!
    echo drive=!drive!
    goto :end
) else (
    if /i "!foldername!"==%%~dI (
    echo ENDING!
    goto :end
    )
    set "result=!parent!"
    goto :loop
)
:end


endlocal