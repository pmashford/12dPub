# 12d Public

Publicly visible 12d stuff

Shout me out if there's something helpful or useful

## SETUP NOTES

### 12d Version
* Current setup is for v15
** Hardcoded paths to cc4d.exe are in ./vscode/tasks.json & /build/Make_4do_From_4dm.bat
* The above files should be changed for a newer 12d Version
* Can compile for an older version using a historical *_Vxx.bat and accompanying vscode task

### VSCODE
* There is a .vscode folder ready to go
* From vscode open the base repo folder
* There is a taks to generate a new protypes file

### CPATH
Set the windows environment variable
CPATH
If more that 1 path is required they must be _unix_ colon : seperated ( windows ; does not work , not sure if it's just cc4d.exe)
