@echo off
setlocal EnableDelayedExpansion

set "EXE_NAME=tictactoe"
set "EXE_EXTENSION=.exe"
set "EXE_PATH=%EXE_NAME%%EXE_EXTENSION%"

set "COMPILER="
set "RELEASE_MODE=false"

rem Color variables (Windows CMD doesn't support ANSI by default; for colors, use PowerShell if needed)
set "YELLOW=[*]"
set "GREEN=[*]"
set "RED=[!]"

rem Parse command-line arguments
:parse_args
for %%A in (%*) do (
    set "arg=%%A"
    if /I "!arg!"=="-CMSVC" (
        set "COMPILER=msvc"
    ) else if /I "!arg!"=="-CGCC" (
        set "COMPILER=gcc"
    ) else if /I "!arg!"=="-CCLANG" (
        set "COMPILER=clang"
    ) else if /I "!arg!"=="-DRELEASE" (
        set "RELEASE_MODE=true"
    ) else if /I "!arg!"=="--help" (
        goto :help
    ) else (
        echo %RED% Unknown argument: !arg!
    )
)
goto :detect_os

:help
echo Usage: ./build.sh [options]
echo(
echo Options:
echo   -C           Compiler flag(no compiler specified = CMake detects which one)
echo   -D           Build flag
echo   --help       Show this help message and exit

echo Compilers:
echo   MSVC         Use the MSVC compiler
echo   GCC          Use the GCC compiler
echo   CLANG        Use the Clang compiler
echo( 
echo Examples:
echo   ./build.sh -CGCC -DRELEASE
echo   ./build.sh -DRELEASE -CMSVC
exit /b 0

:detect_os
rem Only Windows is supported in this batch script
echo %YELLOW% Running on Windows

set "BUILD_DIR=build"
set "ROOT_DIR=%cd%"

if not exist "%BUILD_DIR%" (
    echo %YELLOW% Build directory does not exist. Creating it now...
    mkdir "%BUILD_DIR%"
) else (
    echo %GREEN% Build directory exists
)

cd "%BUILD_DIR%" || (
    echo %RED% Failed to navigate to build directory
    exit /b 1
)

if exist "CMakeCache.txt" (
    echo %YELLOW% Project is already configured
) else (
    echo %GREEN% Configuring the project with CMake...
    set "CMAKE_ARGS="

    if "%RELEASE_MODE%"=="true" (
        set "CMAKE_ARGS=-DRELEASE=ON"
        echo %YELLOW% Building in RELEASE
    ) else (
        echo %YELLOW% Building in DEBUG
    )

    if "%COMPILER%"=="" (
        cmake .. %CMAKE_ARGS% || goto :cmake_fail
    ) else if "%COMPILER%"=="msvc" (
        echo %YELLOW% Using MSVC Compiler
        cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release %CMAKE_ARGS% .. || goto :cmake_fail
    ) else if "%COMPILER%"=="clang" (
        echo %YELLOW% Using Clang Compiler
        cmake -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release %CMAKE_ARGS% .. || goto :cmake_fail
    ) else if "%COMPILER%"=="gcc" (
        echo %YELLOW% Using GCC Compiler
        cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release %CMAKE_ARGS% .. || goto :cmake_fail
    ) else (
        echo %RED% Unsupported compiler: %COMPILER%
        exit /b 1
    )
)

:build
echo %YELLOW% Building the project...
cmake --build . --config Release || goto :build_fail
echo %GREEN% Build completed successfully

rem Search for the executable
for /R %%F in ("%EXE_NAME%%EXE_EXTENSION%") do (
    set "FOUND=%%F"
    goto :found
)

echo %RED% Executable %EXE_PATH% not found
exit /b 1

:found
echo %GREEN% Executable found at: !FOUND!
if not "!FOUND!"=="%ROOT_DIR%\%EXE_PATH%" (
    echo %YELLOW% Moving %EXE_PATH% to the root directory...
    move /Y "!FOUND!" "%ROOT_DIR%" >nul
)

cd "%ROOT_DIR%"
echo %YELLOW% Running %EXE_PATH%
call "%EXE_PATH%" || (
    echo %RED% Failed to run %EXE_PATH%
    exit /b 1
)
exit /b 0

:cmake_fail
echo %RED% CMake configuration failed
exit /b 1

:build_fail
echo %RED% Build failed
exit /b 1
