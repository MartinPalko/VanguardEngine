@echo off

SET projectFileDirectory="%~dp0\Intermediate\VisualStudio\"
SET cmakeExecutableRelativeProjectDir=..\..\ThirdParty\CMake\Windows\bin\cmake.exe
SET sourceRootRelativeProjectDir="..\.."


REM Change directory to where we want the project files put.
if not exist %projectFileDirectory% mkdir %projectFileDirectory%
cd %projectFileDirectory%

REM /b has it start without creating a new window, so we can see the output.
REM /w waits until cmake is finished before calling pause.
start /w /b %cmakeExecutableRelativeProjectDir% -G "Visual Studio 14 2015 Win64" %sourceRootRelativeProjectDir%
REM start /w /b %cmakeExecutableRelativeProjectDir% -G "Visual Studio 14 2015" %sourceRootRelativeProjectDir%

REM Pause so we have time to read output.
powershell "$s=(New-Object -COM WScript.Shell).CreateShortcut('%~dp0\Vanguard.sln.lnk');$s.TargetPath='%cd%\Vanguard.sln';$s.Save()"
pause > NUL
