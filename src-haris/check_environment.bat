@echo off
REM ========================================
REM Diagnostic Script for Training Issues
REM ========================================

echo ========================================
echo TRAINING ENVIRONMENT DIAGNOSTIC
echo ========================================
echo.

echo [1] Current Directory:
echo %CD%
echo.

echo [2] Checking for dataset_processor.c:
if exist "dataset_processor.c" (
    echo    ✓ Found: dataset_processor.c
) else (
    echo    ✗ NOT FOUND: dataset_processor.c
    echo    ERROR: You must run this from the src-haris directory!
)
echo.

echo [3] Checking for source files:
if exist "..\src\naive_bayes.c" (
    echo    ✓ Found: ..\src\naive_bayes.c
) else (
    echo    ✗ NOT FOUND: ..\src\naive_bayes.c
)

if exist "..\src\linear_regression.c" (
    echo    ✓ Found: ..\src\linear_regression.c
) else (
    echo    ✗ NOT FOUND: ..\src\linear_regression.c
)
echo.

echo [4] Checking for dataset files:
if exist "tic-tac-toe-minimax-non-terminal.data" (
    echo    ✓ Found: tic-tac-toe-minimax-non-terminal.data
) else (
    echo    ✗ NOT FOUND: tic-tac-toe-minimax-non-terminal.data
    echo      Generate it with: dataset-gen.exe
)

if exist "tic-tac-toe-minimax-complete.data" (
    echo    ✓ Found: tic-tac-toe-minimax-complete.data
) else (
    echo    ✗ NOT FOUND: tic-tac-toe-minimax-complete.data
    echo      Generate it with: dataset-gen.exe
)
echo.

echo [5] Checking for GCC compiler:
gcc --version >nul 2>&1
if errorlevel 1 (
    echo    ✗ NOT FOUND: gcc is not in your PATH
    echo      Install gcc or add it to PATH
    echo      Common location: C:\msys64\ucrt64\bin\gcc.exe
) else (
    echo    ✓ Found: gcc compiler
    gcc --version | findstr /C:"gcc"
)
echo.

echo [6] Checking compiled executables:
if exist "dataset_processor.exe" (
    echo    ✓ Found: dataset_processor.exe
) else (
    echo    ✗ NOT FOUND: dataset_processor.exe
    echo      Will be compiled on first run
)

if exist "naive_bayes.exe" (
    echo    ✓ Found: naive_bayes.exe
) else (
    echo    ✗ NOT FOUND: naive_bayes.exe
    echo      Will be compiled on first run
)

if exist "linear_regression.exe" (
    echo    ✓ Found: linear_regression.exe
) else (
    echo    ✗ NOT FOUND: linear_regression.exe
    echo      Will be compiled on first run
)
echo.

echo [7] Checking directory structure:
if exist "..\src" (
    echo    ✓ Found: ..\src directory
) else (
    echo    ✗ NOT FOUND: ..\src directory
    echo      ERROR: Incorrect directory structure!
)

if exist "..\models" (
    echo    ✓ Found: ..\models directory
) else (
    echo    ⚠ WARNING: ..\models directory not found
    echo      Will be created automatically
)
echo.

echo ========================================
echo DIAGNOSTIC COMPLETE
echo ========================================
echo.
echo If you see any ✗ errors above, fix them before running training scripts.
echo.
echo Recommended actions:
echo   1. Make sure you're in the src-haris directory
echo   2. Generate datasets with dataset-gen.exe if missing
echo   3. Ensure gcc is installed and in your PATH
echo.
echo Ready to train? Run one of these:
echo   .\train_models.bat          (simple training)
echo   .\train_models_compare.bat  (comparison training)
echo.

pause
