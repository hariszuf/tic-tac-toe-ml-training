@echo off
echo ========================================
echo Tic-Tac-Toe ML Training - Quick Start
echo ========================================
echo.
echo What would you like to do?
echo.
echo [1] Evaluate current models
echo [2] Retrain Linear Regression
echo [3] Retrain Naive Bayes
echo [4] Test in game
echo [5] AI vs AI testing
echo [6] View documentation
echo [0] Exit
echo.
set /p choice="Enter choice (0-6): "

if "%choice%"=="1" goto evaluate
if "%choice%"=="2" goto train_lr
if "%choice%"=="3" goto train_nb
if "%choice%"=="4" goto test_game
if "%choice%"=="5" goto ai_vs_ai
if "%choice%"=="6" goto docs
if "%choice%"=="0" goto end
goto invalid

:evaluate
echo.
echo Running model evaluation...
cd src
call evaluate.bat
cd ..
goto menu

:train_lr
echo.
echo Training Linear Regression model...
cd src
if not exist "linear_regression.exe" (
    echo Compiling...
    gcc linear_regression.c -o linear_regression.exe -lm -Wall
)
echo.
echo Training (this takes 2-3 minutes)...
linear_regression.exe
echo.
echo Training complete! Model saved to ../models/linear regression/
echo.
pause
cd ..
goto menu

:train_nb
echo.
echo Training Naive Bayes model...
cd src
if not exist "naive_bayes.exe" (
    echo Compiling...
    gcc naive_bayes.c -o naive_bayes.exe -Wall
)
echo.
echo Training (this takes 1-2 minutes)...
naive_bayes.exe
echo.
echo Training complete! Model saved to current directory.
echo.
pause
cd ..
goto menu

:test_game
echo.
echo Compiling and launching game...
cd TTTGUI(wtf)
call compile.bat
if errorlevel 1 (
    echo.
    echo ERROR: Game compilation failed!
    echo Check if raylib is installed correctly.
    pause
    cd ..
    goto menu
)
echo.
echo Launching game...
start ttt_gui.exe
cd ..
echo.
echo Game launched in new window!
echo Press E for Easy (Naive Bayes)
echo Press M for Medium (Linear Regression)
echo Press H for Hard (Minimax)
echo Press X for Expert (Full Minimax)
echo.
pause
goto menu

:ai_vs_ai
echo.
echo Running AI vs AI Testing...
cd src
if not exist "ai_vs_ai.exe" (
    echo Compiling...
    gcc ai_vs_ai_test.c -o ai_vs_ai.exe -lm -Wall
    if errorlevel 1 (
        echo.
        echo ERROR: Compilation failed!
        pause
        cd ..
        goto menu
    )
)
echo.
ai_vs_ai.exe
cd ..
pause
goto menu

:docs
echo.
echo Available documentation:
echo.
echo [1] QUICK_REFERENCE.md - Fast lookup guide
echo [2] MODEL_EVALUATION_GUIDE.md - Comprehensive evaluation guide
echo [3] LINEAR_REGRESSION_SUMMARY.md - Linear Regression details
echo [4] AI_VS_AI_IMPLEMENTATION.md - AI testing guide
echo [5] src/AI_VS_AI_USAGE.md - Usage instructions
echo.
echo Opening QUICK_REFERENCE.md in default text editor...
start QUICK_REFERENCE.md
pause
goto menu

:invalid
echo.
echo Invalid choice! Please enter 0-6.
echo.
pause
goto menu

:menu
echo.
echo ========================================
echo.
set /p again="Return to menu? (Y/N): "
if /i "%again%"=="Y" goto start
goto end

:start
cls
echo ========================================
echo Tic-Tac-Toe ML Training - Quick Start
echo ========================================
echo.
echo What would you like to do?
echo.
echo [1] Evaluate current models
echo [2] Retrain Linear Regression
echo [3] Retrain Naive Bayes
echo [4] Test in game
echo [5] AI vs AI testing
echo [6] View documentation
echo [0] Exit
echo.
set /p choice="Enter choice (0-6): "

if "%choice%"=="1" goto evaluate
if "%choice%"=="2" goto train_lr
if "%choice%"=="3" goto train_nb
if "%choice%"=="4" goto test_game
if "%choice%"=="5" goto ai_vs_ai
if "%choice%"=="6" goto docs
if "%choice%"=="0" goto end
goto invalid

:end
echo.
echo Thanks for using Tic-Tac-Toe ML Training!
echo.
pause
