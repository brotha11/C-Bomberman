@echo off
setlocal EnableDelayedExpansion

echo === Compiling C-Bomberman for Windows ===

:: Variables
set CC=gcc
set NAME=C-Bomberman.exe
set CFLAGS=-Iinclude -Llib
set LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

:: Execute game after compiling
set RUN_AFTER_COMPILE=true

:: Look for .c files (Folders with subfolders are done bellow)
set SRC=
for %%F in (
    src\audio\*.c
    src\blocks\*.c
    src\bombs\*.c
    src\entities\*.c
    src\game\*.c
    src\graphics\*.c
    src\input\*.c
    src\pickups\*.c
    src\rooms\*.c
) do (
    set SRC=!SRC! %%F
)

:: Include subdirectories

:: Modes
for /R src\modes %%F in (*.c) do (
    set SRC=!SRC! %%F
)

:: Compile
echo Compiling...
%CC% %CFLAGS% -o %NAME% !SRC! %LIBS%

if %errorlevel% neq 0 (
    echo Compilation error
) else (
    echo Compilation succesfully done: %NAME%
    
    if /i "%RUN_AFTER_COMPILE%"=="true" (
        echo Running game...
        .\%NAME%
    )
)

endlocal
pause