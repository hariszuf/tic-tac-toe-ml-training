@echo off
REM ========================================
REM Advanced Training Script
REM Trains models with BOTH non-terminal and combined datasets
REM Compares performance
REM ========================================

REM Ensure we're in the src-haris directory
if not exist "dataset_processor.c" (
    echo ERROR: This script must be run from the src-haris directory!
    echo Current directory: %CD%
    echo.
    echo Please navigate to src-haris and run again:
    echo   cd c:\Users\muhdh\Documents\GitHub\tic-tac-toe-ml-training\src-haris
    echo   .\train_models_compare.bat
    pause
    exit /b 1
)

echo ========================================
echo TIC-TAC-TOE ML TRAINING - COMPARISON MODE
echo ========================================
echo.
echo This script will train both models with:
echo   1. Non-terminal dataset (strategic positions)
echo   2. Combined dataset (all positions)
echo.
echo You can compare which performs better!
echo.
pause

REM ========================================
REM Check Prerequisites
REM ========================================
echo Checking prerequisites...

if not exist "dataset_processor.exe" (
    echo Compiling dataset processor...
    gcc dataset_processor.c -o dataset_processor.exe
    if errorlevel 1 (
        echo ERROR: Failed to compile dataset_processor.c
        pause
        exit /b 1
    )
)

if not exist "tic-tac-toe-minimax-non-terminal.data" (
    echo WARNING: tic-tac-toe-minimax-non-terminal.data not found!
    echo Please generate it first using dataset-gen.exe
)

if not exist "tic-tac-toe-minimax-complete.data" (
    echo WARNING: tic-tac-toe-minimax-complete.data not found!
    echo Please generate it first using dataset-gen.exe
)

echo.

REM ========================================
REM Compile Training Programs
REM ========================================
echo ========================================
echo COMPILING TRAINING PROGRAMS
echo ========================================
echo.

echo Compiling Naive Bayes...
if exist "..\src\naive_bayes.c" (
    gcc ..\src\naive_bayes.c -o naive_bayes.exe -lm
    if errorlevel 1 (
        echo ERROR: Failed to compile naive_bayes.c
        echo Make sure gcc is in your PATH
        pause
        exit /b 1
    )
    echo Done!
) else (
    echo ERROR: ..\src\naive_bayes.c not found!
    echo Current directory: %CD%
    pause
    exit /b 1
)

echo Compiling Linear Regression...
if exist "..\src\linear_regression.c" (
    gcc ..\src\linear_regression.c -o linear_regression.exe -lm
    if errorlevel 1 (
        echo ERROR: Failed to compile linear_regression.c
        echo Make sure gcc is in your PATH
        pause
        exit /b 1
    )
    echo Done!
) else (
    echo ERROR: ..\src\linear_regression.c not found!
    echo Current directory: %CD%
    pause
    exit /b 1
)

echo.
echo All programs compiled successfully!
echo.

REM Create output folders for organization
echo Creating output folders...
if not exist "output" mkdir "output"
if not exist "output\non_terminal" mkdir "output\non_terminal"
if not exist "output\combined" mkdir "output\combined"
if not exist "output\datasets" mkdir "output\datasets"
if not exist "output\reports" mkdir "output\reports"
echo Output folders ready!
echo.

echo Compiling Q-Learning (optional)...
if exist "q_learning.c" (
    gcc q_learning.c -o q_learning.exe -lm
    if errorlevel 1 (
        echo WARNING: Failed to compile q_learning.c
        echo Q-Learning training will be skipped
    ) else (
        echo Q-Learning compiled successfully!
    )
) else (
    echo q_learning.c not found, Q-Learning training will be skipped
)
echo.
pause

REM ========================================
REM TRAINING SET 1: NON-TERMINAL DATASET
REM ========================================
echo.
echo ========================================
echo TRAINING SET 1: NON-TERMINAL DATASET
echo ========================================
echo.

if exist "tic-tac-toe-minimax-non-terminal.data" (
    echo Step 1/4: Processing non-terminal dataset...
    dataset_processor.exe tic-tac-toe-minimax-non-terminal.data 0.8
    
    if not exist "train_non_terminal.data" (
        echo ERROR: Failed to create train_non_terminal.data
        pause
        exit /b 1
    )
    
    echo.
    echo Step 2/4: Training Naive Bayes on Non-Terminal dataset...
    copy /Y train_non_terminal.data train.data
    copy /Y test_non_terminal.data test.data
    naive_bayes.exe
    
    REM Save model with specific name
    if not exist "..\models\naive_bayes_non_terminal" mkdir "..\models\naive_bayes_non_terminal"
    copy /Y "..\models\naive bayes\model.txt" "..\models\naive_bayes_non_terminal\model_non_terminal.txt"
    echo Saved to: ..\models\naive_bayes_non_terminal\model_non_terminal.txt
    
    echo.
    echo Step 3/4: Training Linear Regression on Non-Terminal dataset...
    if not exist "..\dataset\processed" mkdir "..\dataset\processed"
    copy /Y train_non_terminal.data ..\dataset\processed\train.data
    copy /Y test_non_terminal.data ..\dataset\processed\test.data
    
    linear_regression.exe
    
    REM Save model with specific name
    if not exist "..\models\linear_regression_non_terminal" mkdir "..\models\linear_regression_non_terminal"
    copy /Y "..\models\linear regression\model.txt" "..\models\linear_regression_non_terminal\model_non_terminal.txt"
    copy /Y "..\models\linear regression\model.bin" "..\models\linear_regression_non_terminal\model_non_terminal.bin"
    echo Saved to: ..\models\linear_regression_non_terminal\
    
    echo.
    echo Step 4/4: Training Q-Learning on Non-Terminal dataset...
    if exist "q_learning.exe" (
        q_learning.exe tic-tac-toe-minimax-non-terminal.data
        if exist "q_learning_model.txt" (
            copy /Y q_learning_model.txt output\non_terminal\q_learning_non_terminal.txt
            echo Saved to: output\non_terminal\q_learning_non_terminal.txt
        )
    ) else (
        echo Q-learning not compiled. Skipping...
    )
    
    REM Move dataset and report files to output folders
    if exist "train_non_terminal.data" move /Y train_non_terminal.data output\datasets\ >nul
    if exist "test_non_terminal.data" move /Y test_non_terminal.data output\datasets\ >nul
    if exist "report_non_terminal.txt" move /Y report_non_terminal.txt output\reports\ >nul
    
    echo.
    echo Non-terminal training complete!
    pause
) else (
    echo Skipping non-terminal dataset (file not found)
)

REM ========================================
REM TRAINING SET 2: COMBINED DATASET
REM ========================================
echo.
echo ========================================
echo TRAINING SET 2: COMBINED DATASET
echo ========================================
echo.

if exist "tic-tac-toe-minimax-complete.data" (
    echo Step 1/4: Processing combined dataset...
    dataset_processor.exe tic-tac-toe-minimax-complete.data 0.8
    
    if not exist "train_combined.data" (
        echo ERROR: Failed to create train_combined.data
        pause
        exit /b 1
    )
    
    echo.
    echo Step 2/4: Training Naive Bayes on Combined dataset...
    copy /Y train_combined.data train.data
    copy /Y test_combined.data test.data
    naive_bayes.exe
    
    REM Save model with specific name
    if not exist "..\models\naive_bayes_combined" mkdir "..\models\naive_bayes_combined"
    copy /Y "..\models\naive bayes\model.txt" "..\models\naive_bayes_combined\model_combined.txt"
    echo Saved to: ..\models\naive_bayes_combined\model_combined.txt
    
    echo.
    echo Step 3/4: Training Linear Regression on Combined dataset...
    if not exist "..\dataset\processed" mkdir "..\dataset\processed"
    copy /Y train_combined.data ..\dataset\processed\train.data
    copy /Y test_combined.data ..\dataset\processed\test.data
    
    linear_regression.exe
    
    REM Save model with specific name
    if not exist "..\models\linear_regression_combined" mkdir "..\models\linear_regression_combined"
    copy /Y "..\models\linear regression\model.txt" "..\models\linear_regression_combined\model_combined.txt"
    copy /Y "..\models\linear regression\model.bin" "..\models\linear_regression_combined\model_combined.bin"
    echo Saved to: ..\models\linear_regression_combined\
    
    echo.
    echo Step 4/4: Training Q-Learning on Combined dataset...
    if exist "q_learning.exe" (
        q_learning.exe tic-tac-toe-minimax-complete.data
        if exist "q_learning_model.txt" (
            copy /Y q_learning_model.txt output\combined\q_learning_combined.txt
            echo Saved to: output\combined\q_learning_combined.txt
        )
    ) else (
        echo Q-learning not compiled. Skipping...
    )
    
    REM Move dataset and report files to output folders
    if exist "train_combined.data" move /Y train_combined.data output\datasets\ >nul
    if exist "test_combined.data" move /Y test_combined.data output\datasets\ >nul
    if exist "report_combined.txt" move /Y report_combined.txt output\reports\ >nul
    if exist "train.data" move /Y train.data output\datasets\ >nul
    if exist "test.data" move /Y test.data output\datasets\ >nul
    if exist "dataset_report.txt" move /Y dataset_report.txt output\reports\ >nul
    
    echo.
    echo Combined training complete!
    pause
) else (
    echo Skipping combined dataset (file not found)
)

REM ========================================
REM Summary
REM ========================================
echo.
echo ========================================
echo TRAINING COMPLETE - COMPARISON SUMMARY
echo ========================================
echo.
echo All outputs organized in the output\ folder:
echo.

if exist "output\non_terminal\q_learning_non_terminal.txt" (
    echo [NON-TERMINAL MODELS]
    echo   Naive Bayes:        ..\models\naive_bayes_non_terminal\model_non_terminal.txt
    echo   Linear Regression:  ..\models\linear_regression_non_terminal\model_non_terminal.txt
    echo   Linear Regression:  ..\models\linear_regression_non_terminal\model_non_terminal.bin
    echo   Q-Learning:         output\non_terminal\q_learning_non_terminal.txt
    echo.
)

if exist "output\combined\q_learning_combined.txt" (
    echo [COMBINED MODELS]
    echo   Naive Bayes:        ..\models\naive_bayes_combined\model_combined.txt
    echo   Linear Regression:  ..\models\linear_regression_combined\model_combined.txt
    echo   Linear Regression:  ..\models\linear_regression_combined\model_combined.bin
    echo   Q-Learning:         output\combined\q_learning_combined.txt
    echo.
)

echo [DATASETS]
echo   Location:           output\datasets\
if exist "output\datasets\train_non_terminal.data" (
    echo   Non-Terminal:       train_non_terminal.data, test_non_terminal.data
)
if exist "output\datasets\train_combined.data" (
    echo   Combined:           train_combined.data, test_combined.data
)
echo.

echo [REPORTS]
echo   Location:           output\reports\
if exist "output\reports\report_non_terminal.txt" (
    echo   Non-Terminal:       report_non_terminal.txt
)
if exist "output\reports\report_combined.txt" (
    echo   Combined:           report_combined.txt
)
echo.

echo See output\README.md for detailed organization and comparison guide.
echo.

echo Next Steps:
echo   1. Compare accuracy metrics from the training output
echo   2. Review the dataset reports (report_*.txt)
echo   3. Test models with evaluate_models.exe
echo   4. Compare win rates in AI vs AI gameplay
echo.
echo Model Files:
echo   - *_non_terminal.*  = Trained on strategic positions only
echo   - *_combined.*      = Trained on all positions (terminal + non-terminal)
echo.
echo To use a specific model in gameplay:
echo   For Linear Regression:
echo     copy ..\models\linear_regression_non_terminal\model_non_terminal.bin ..\models\linear regression\model.bin
echo     OR
echo     copy ..\models\linear_regression_combined\model_combined.bin ..\models\linear regression\model.bin
echo.
echo   For Naive Bayes:
echo     copy ..\models\naive_bayes_non_terminal\model_non_terminal.txt ..\models\naive bayes\model.txt
echo     OR
echo     copy ..\models\naive_bayes_combined\model_combined.txt ..\models\naive bayes\model.txt
echo.
echo ========================================

pause
