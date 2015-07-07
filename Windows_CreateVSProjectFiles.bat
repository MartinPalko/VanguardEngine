@echo off

SET projectFileDirectory="Intermediate\VisualStudio\"
SET cmakeExecutableRelativeProjectDir=..\..\CMake\Windows\bin\cmake.exe
SET sourceRootRelativeProjectDir="..\.."


REM Change directory to where we want the project files put.
if not exist %projectFileDirectory% mkdir %projectFileDirectory%
cd %projectFileDirectory%

REM /b has it start without creating a new window, so we can see the output.
REM /w waits until cmake is finished before calling pause.
REM start /WAIT /b %cmakeExecutableRelativeProjectDir% -G "Visual Studio 12 2013 Win64" %sourceRootRelativeProjectDir%
start /WAIT /b %cmakeExecutableRelativeProjectDir% -G "Visual Studio 12 2013" %sourceRootRelativeProjectDir%

REM Pause so we have time to read output.
pause > NUL