@echo off
echo ========================================
echo Compiling AI vs AI Test Program
echo ========================================
echo.

gcc ai_vs_ai_test.c -o ai_vs_ai.exe -lm -Wall

if errorlevel 1 (
    echo.
    echo ❌ Compilation failed!
    pause
    exit /b 1
)

echo ✅ Compilation successful!
echo.
echo ========================================
echo Running AI vs AI Test
echo ========================================
echo.

ai_vs_ai.exe

pause
