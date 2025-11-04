@echo off
echo ========================================
echo Model Evaluation Tool
echo ========================================
echo.

REM Check if evaluation tool exists
if not exist "evaluate_models.exe" (
    echo Compiling evaluation tool...
    gcc evaluate_models.c -o evaluate_models.exe -lm -Wall
    if errorlevel 1 (
        echo ERROR: Compilation failed!
        pause
        exit /b 1
    )
    echo Compilation successful!
    echo.
)

REM Run evaluation
echo Running comprehensive evaluation...
echo.
evaluate_models.exe

echo.
echo ========================================
echo Evaluation complete!
echo ========================================
echo.
echo To retrain models:
echo   1. Linear Regression: linear_regression.exe
echo   2. Naive Bayes:       naive_bayes.exe
echo.
pause
