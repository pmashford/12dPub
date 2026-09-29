# 12d Public

12dPL macros and the **mashy_lib** include library for 12d Model, with a ready-to-go VS Code setup.

12dPL (the 12d Programming Language, formerly 4DML) is the C-like macro language built into 12d Model. Macro sources are `.4dm` files, includable headers are `.H`/`.h`/`.4d`, and `cc4d.exe` compiles them to `.4do` files that you run inside 12d Model.

Shout me out if there's something helpful or useful.

## Directories
* `/.vscode/` : VS Code setup (build tasks, problem matchers, IntelliSense)
* `/bin/`     : compiled `.4do` files built from `/src/`, each with a `.json` of build info
* `/build/`   : batch files that VS Code uses to call `cc4d.exe` and turn a `.4dm` into a `.4do`
* `/include/` : the mashy_lib headers (`mashy_lib_*.H`), plus headers from the 12d forums under `/include/12d/`
* `/src/`     : macro source, grouped by theme
* `/test/`    : experimental and prototype macros

## Setup

### 12d version
* The current setup is for **12d Model v15**.
* The path to `cc4d.exe` is hardcoded in three places. Change all three for a newer or different 12d Model version:
  * `build/Make_4do_From_4dm.bat`
  * the prototypes task in `.vscode/tasks.json`
  * the `set_ups` include path in `.vscode/c_cpp_properties.json`
* `build/Make_4do_From_4dm_v14.bat` targets v14.

### VS Code
* Use **Open Folder...** and select the base repo folder. VS Code offers to install the C/C++ extension it needs.
* **Ctrl+Shift+B** compiles the file you have open (task **12dPL: Compile macro (v15)**). Compiler errors show up in the Problems panel.
* A successful compile copies the `.4do` to `/bin/`. A failed compile leaves the last good build there.
* The task **12dPL: Regenerate prototypes file (v15)** rebuilds `/include/prototypesv15.4dm`, which gives most of the highlighting and IntelliSense for 12dPL.

### Includes (CPATH)
* No setup is needed. The build batch files put this repo's `/include/` folder on `CPATH` for each compile.
* If you already have your own `CPATH` (for example, one pointing at another repo's include folder), it's still searched after this repo's.
* If you set `CPATH` yourself, separate multiple paths with a **Unix** colon `:`. Windows `;` doesn't work with `cc4d.exe`.

## Disclaimer

12d and 12d Model are trademarks of 12d Solutions Pty Ltd. This repo is not affiliated with or endorsed by 12d Solutions.
