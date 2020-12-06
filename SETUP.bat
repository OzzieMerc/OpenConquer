::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: File: SETUP.bat
:: Author: Ozzie Mercado
:: Created: December 5, 2020
:: Description: Creates two folders and runs CMake to generate the
::              Visual Studio project inside it.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo off

:: Make build folder if needed.
if not exist "Build" mkdir Build
cd Build

:: Make VisualStudio folder if needed.
if not exist "VisualStudio" mkdir VisualStudio
cd VisualStudio

:: Run CMake.
cmake -A x64 "../../"

:: Pause so any important information (such as errors) can be read.
pause
