@echo off
REM ========================================
REM Q-Learning Training Script
REM Trains Q-learning agent with optional minimax dataset
REM ========================================

echo ========================================
echo Q-LEARNING TRAINING FOR TIC-TAC-TOE
echo ========================================
echo.
echo This will train a Q-learning agent through self-play.
echo Optionally, you can bootstrap with a minimax dataset.
echo.
pause

REM Check if Q-learning program is compiled
if not exist "q_learning.exe" (
    echo Compiling Q-learning trainer...
    gcc q_learning.c -o q_learning.exe -lm
    if errorlevel 1 (
        echo ERROR: Failed to compile q_learning.c
        pause
        exit /b 1
    )
    echo Done!
    echo.
)

REM ========================================
REM Ask user about dataset bootstrapping
REM ========================================
echo.
echo Do you want to bootstrap Q-values with a minimax dataset?
echo   1. Yes - Use non-terminal dataset (recommended for strategy)
echo   2. Yes - Use combined dataset (terminal + non-terminal)
echo   3. No - Start from scratch (pure self-play)
echo.
set /p choice="Enter choice (1/2/3): "

if "%choice%"=="1" (
    if exist "tic-tac-toe-minimax-non-terminal.data" (
        echo.
        echo Training with NON-TERMINAL dataset bootstrapping...
        q_learning.exe tic-tac-toe-minimax-non-terminal.data
        
        REM Save with specific name
        if exist "q_learning_model.txt" (
            copy /Y q_learning_model.txt q_learning_non_terminal.txt
            echo.
            echo Model saved as: q_learning_non_terminal.txt
        )
    ) else (
        echo ERROR: tic-tac-toe-minimax-non-terminal.data not found!
        echo Please generate it first using dataset-gen.exe
        pause
        exit /b 1
    )
) else if "%choice%"=="2" (
    if exist "tic-tac-toe-minimax-complete.data" (
        echo.
        echo Training with COMBINED dataset bootstrapping...
        q_learning.exe tic-tac-toe-minimax-complete.data
        
        REM Save with specific name
        if exist "q_learning_model.txt" (
            copy /Y q_learning_model.txt q_learning_combined.txt
            echo.
            echo Model saved as: q_learning_combined.txt
        )
    ) else (
        echo ERROR: tic-tac-toe-minimax-complete.data not found!
        echo Please generate it first using dataset-gen.exe
        pause
        exit /b 1
    )
) else (
    echo.
    echo Training from scratch (no bootstrapping)...
    echo Note: This will take longer to converge.
    echo.
    echo Starting training...
    q_learning.exe
    
    REM Save with specific name
    if exist "q_learning_model.txt" (
        copy /Y q_learning_model.txt q_learning_scratch.txt
        echo.
        echo Model saved as: q_learning_scratch.txt
    )
)

echo.
echo ========================================
echo Q-LEARNING TRAINING COMPLETE
echo ========================================
echo.
echo The Q-learning agent has been trained through 50,000 episodes of self-play.
echo.
echo Key differences from Naive Bayes/Linear Regression:
echo   - Learns through trial and error (reinforcement learning)
echo   - Builds a Q-table mapping (state,action) to expected reward
echo   - Continues improving through self-play
echo.
echo Model files created:
if exist "q_learning_non_terminal.txt" (
    echo   - q_learning_non_terminal.txt
)
if exist "q_learning_combined.txt" (
    echo   - q_learning_combined.txt
)
if exist "q_learning_scratch.txt" (
    echo   - q_learning_scratch.txt
)
echo.
echo Next steps:
echo   - Compare Q-learning with supervised learning models
echo   - Test in AI vs AI gameplay
echo   - Train longer for better performance (modify MAX_EPISODES in code)
echo.
echo ========================================

pause
