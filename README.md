# README

### Brief
Tisp lang will be a tiny, general-purpose programming language. It will have strong, static typing and run on a bytecode VM.

### Initial Tooling
 - Git (MacOS)
 - CMake 3.27.7 & CTest (later!)
 - Apple Clang

### Other Docs
 - [Tisp Grammar](grammar.md)

### Setup
 - Clone the repo onto your system.
 - Run `rm -vi ./compile_commands.json` since the paths are incorrect. Ensure it's the correct file to remove. I've accidentally committed this file.
 - Run `cmake --fresh .` to rebuild the project building files.
 - Run `make all`.

### TODO:
 - Fix parser failure on test00: the token advancing is too fast so it thinks that the defun keyword is part of the import.
