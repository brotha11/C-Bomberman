@echo off
setlocal EnableDelayedExpansion

:: Due to how this .bat is made, if you modify a .h file, it is better to rebuild the program to avoid random errors.

if not defined BUILD_SCRIPT_STARTED (
    set BUILD_SCRIPT_STARTED=1
    echo === C-Bomberman Build Script ===
)

:: Configuración
set CC=gcc
set NAME=C-Bomberman.exe
set CFLAGS=-Iinclude -Llib -c
set LDFLAGS=-Llib
set LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
set SRC_DIR=src
set OBJ_DIR=obj
set RUN_AFTER_COMPILE=false

:: Procesar argumentos
if /i "%1"=="clean" goto clean
if /i "%1"=="rebuild" goto rebuild
if /i "%1"=="build" goto build
if /i "%1"=="run" goto run
if /i "%1"=="full" goto full

echo Usage: %0 [build|clean|rebuild|run]
exit /b 1

:clean
echo Cleaning...
if exist "%OBJ_DIR%" (
    del /q "%OBJ_DIR%\*.o"
)
if exist "%NAME%" (
    del /q "%NAME%"
)
echo Clean complete.
exit /b 0

:rebuild
call %0 clean
call %0 build
exit /b 0

:run
echo Running game...
start "" "%NAME%"
exit /b 0

:full
call %0 rebuild
call %0 run
exit /b 0

:build
:: Crear carpeta obj si no existe
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

:: Compilar .c archivos
echo Compiling .c files...
for /R "%SRC_DIR%" %%F in (*.c) do (
    set "SRC=%%~fF"
    set "OBJ=%OBJ_DIR%\%%~nF.o"
    set "FILENAME=%%~nxF"
    
    call :check_compile
)

:: Enlazar
echo Linking...
"%CC%" -o "%NAME%" "%OBJ_DIR%\*.o" %LDFLAGS% %LIBS%
if errorlevel 1 (
    echo Linking error
    exit /b 1
) else (
    echo Compilation successful: %NAME%
    if /i "%RUN_AFTER_COMPILE%"=="true" (
        call %0 run
    )
)
exit /b 0

:check_compile
if not exist "%OBJ%" goto :do_compile
for %%A in ("%SRC%") do set SRC_DATE=%%~tA
for %%B in ("%OBJ%") do set OBJ_DATE=%%~tB
if "!SRC_DATE!" gtr "!OBJ_DATE!" (
    goto :do_compile
) else (
    echo - Skipping: %FILENAME%
    goto :eof
)

:do_compile
echo - Compiling: %FILENAME%
"%CC%" %CFLAGS% "%SRC%" -o "%OBJ%"
if errorlevel 1 (
    echo Error compiling %FILENAME%
    exit /b 1
)
goto :eof
