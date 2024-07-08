# 12d Build

 - These are files used to **build .4dm** to **.4do**

 - The vscode config file _tasks.json_ has a **build** _task_ which _runs_ **Make_4do_From_4dm.bat**.

 - **Ctrl + Shift + B** executes the build process for the current file with focus

 - The **.bat** expects one argument being **qualified_path\filename.4dm**

 - The **.bat** will copy the .4do to **/bin/**  and also _create_ a **.json** file which _stores_ some **info**

 - The **.json** info contains various data at compile time

## Older compiler versions

 - **Make_4do_From_4dm_v14.bat** points to the _v14.00_ installation area and is a simple way to compile to that versions

 - To execute it can be accessed from the **Run Task** area of vscode