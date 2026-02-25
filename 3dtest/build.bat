@echo off
set SRC=main.c++
set OUT=app.exe

if not exist "%OUT%" goto build

for %%S in ("%SRC%") do set SRCTIME=%%~tS
for %%O in ("%OUT%") do set OUTTIME=%%~tO

if "%SRCTIME%" GTR "%OUTTIME%" goto build

echo Nothing changed. Skipping build.
goto runcheck

:build
g++ "%SRC%" -lgdi32 -luser32 -o "%OUT%"
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)
echo Build succeeded.

:runcheck
if "%1"=="run" (
    "%OUT%"
)
