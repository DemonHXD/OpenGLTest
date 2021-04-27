@echo off

where "C:\Program Files\CMake\bin\;C:\Program Files (x86)\CMake\bin\;%PATH%:cmake.exe" > tmp
set /P cmake=<tmp
del tmp

set SOURCE_DIR=%cd%
for /f "delims=" %%i in ("%cd%") do set NAME=%%~ni
set BUILD_DIR=%SOURCE_DIR%\\build
@rem set BUILD_DIR=build

@rem if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%" || GOTO :skip
:build
if not exist "%BUILD_DIR%" md "%BUILD_DIR%"
cd /d "%BUILD_DIR%"
"%cmake%" -G "Visual Studio 15 2017" -A Win32 "%SOURCE_DIR%"
cd /d "%SOURCE_DIR%"

@rem vs2017: 15, vs2015: 14, vs2013: 12, vs2012: 11, vs2010: 10
set vs_version=15
for /f "tokens=1,2*" %%a in ('reg query "HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7" /v "%vs_version%.0" 2^>nul') do set "devenv_cmd=%%c"
@rem start "" "%devenv_cmd%\\Common7\\IDE\\devenv.exe" "%BUILD_DIR%\\%NAME%.sln"

:skip

