:: Copyright 2024 GlitchyByte
:: SPDX-License-Identifier: MIT-0

:: Builds project.

@REM cd code
@REM cmake -DCMAKE_BUILD_TYPE=MinSizeRel -B build/build.cmake -S .
@REM cmake --build build/build.cmake --config MinSizeRel --parallel
@REM cd ..

:: [Setup]
@echo off
setlocal enabledelayedexpansion
set "calling_dir=%CD%"
set "script_dir=%~dp0"
cd /d "%script_dir%"

:: [Main]
goto afterFuncs

:: Usage.
:printUsage
echo Usage: build [Debug^|Release^|MinSizeRel^|RelWithDebInfo] [clean]
cd /d "%calling_dir%"
exit /b 1

:afterFuncs

:: Accept 1 or more parameters.
if "%~1"=="" goto printUsage

:: If there is more than 1 parameter, the 2nd must be "clean".
if not "%~2"=="" (
  if /i "%~2"=="clean" (
    set "clean=yes"
  ) else (
    goto printUsage
  )
) else (
  set "clean=no"
)

:: Capture valid flavor.
set "flavor=%~1"
if /i not "%flavor%"=="Debug" if /i not "%flavor%"=="Release" if /i not "%flavor%"=="MinSizeRel" if /i not "%flavor%"=="RelWithDebInfo" (
  goto printUsage
)

:: Dir constants.
cd /d "%script_dir%\code"
set "buildConfigDir=build\build.cmake"
set "binDir=build\bin"

:: Make sure build dir exists.
if not exist "build" mkdir "build"

if "%clean%"=="yes" (
  echo Refreshing configuration...
  :: Remove build dir.
  if exist "%buildConfigDir%" rmdir /s /q "%buildConfigDir%"
  :: Clean bin dir.
  if exist "%binDir%" (
    rmdir /s /q "%binDir%"
    mkdir "%binDir%"
  )
)

:: Configure.
echo Configuring: !flavor!
cmake -DCMAKE_BUILD_TYPE=!flavor! -B "%buildConfigDir%" -S .

:: Build.
echo Building: !flavor!
cmake --build "%buildConfigDir%" --config %flavor% --parallel


:: [Teardown]
cd /d "%calling_dir%"

:: Done!
echo Build done!
