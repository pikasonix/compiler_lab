@echo off
setlocal

:: 1. Build
gcc -o compare.exe compare.c || (echo [!] Build Error & exit /b 1)

:: 2. Run
compare.exe

echo.
