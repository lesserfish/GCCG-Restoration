@echo off

echo Welcome to the GCCG Package Manager
:LOOP
echo.
set /p arg= Enter an operation ("q" to quit): 
if "%arg:~0,2%" equ "./" set arg=%arg:~2%
if /i "%arg:~0,12%" equ "gccg_package" set arg=%arg:~13%
if /i "%arg%" equ "q" (goto EXIT) else (perl gccg_package %arg%)
goto LOOP
:EXIT
