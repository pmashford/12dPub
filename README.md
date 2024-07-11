# 12d Public

Publicly visible 12d stuff

## Directories
* /.vscode/ : vscode setup
* /bin/     : some 4do's compile from /src/
* /build/   : batch file used by vscode to call cc4d.exe and 4dm->4do
* /include/ : Over time I'll add various functions from my librarys
* /src/     : Over time I'll add various macro source

Shout me out if there's something helpful or useful

## REPO NOTES

* This is a place to store some of my macro source code and setup

## SETUP NOTES

### 12d Version
* Current setup is for **v15**
* Hardcoded paths to **cc4d.exe** are in __./vscode/tasks.json__ & __/build/Make_4do_From_4dm.bat__
* The above files should be changed for a newer/different 12d Model Version

### VSCODE
* There is a .vscode folder ready to go
* From vscode use **Open Folder...** and select the base repo folder
* There is a task to generate a new protypes file
* the prototypes.4dm file within /include/ gets most of the highlighting for the 4dm language

### CPATH
* Set the windows environment variable **CPATH** to be equal to **drive:/full/path/12dPub/include**
* If more that 1 path is required they must be **unix** colon **:** seperated ( windows ; _does not work_ , not sure if it's just cc4d.exe)
