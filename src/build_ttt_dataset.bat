@echo off
REM ==============================================
REM Tic-Tac-Toe Minimax Dataset Generator
REM Compiles and runs ttt_minimax_dataset.c
REM Output: ttt_dataset.data
REM ==============================================

echo [1/3] Checking for GCC...
where gcc >nul 2>nul
if errorlevel 1 (
    echo Error: GCC compiler not found.
    echo Please install MinGW or MSYS2 and ensure gcc.exe is in your PATH.
    pause
    exit /b
)

echo [2/3] Compiling program...
gcc -O2 -std=c17 ttt_minimax_dataset.c -o ttt_stategen
if errorlevel 1 (
    echo Compilation failed. Please check your C code for errors.
    pause
    exit /b
)

echo [3/3] Running program to generate dataset...
ttt_stategen

if exist ttt_dataset.data (
    echo ✅ Dataset generated successfully: ttt_dataset.data
) else (
    echo ⚠️ Warning: No .data file found. Program may not have run correctly.
)

echo Done.
pause
