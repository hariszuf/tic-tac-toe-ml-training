@echo off
echo ========================================
echo Linear Regression - CSV Dataset Training
echo ========================================
echo.

REM Compile the CSV-compatible version
echo Compiling CSV-compatible linear regression...
gcc linear_regression_csv.c -o linear_regression_csv.exe -lm -Wall

if errorlevel 1 (
    echo.
    echo ❌ Compilation failed!
    pause
    exit /b 1
)

echo ✅ Compilation successful!
echo.

REM Check if user wants to use CSV or text format
echo Which dataset format do you want to use?
echo.
echo [1] CSV format (train_dataset.csv / test_dataset.csv)
echo [2] Text format (train.data / test.data)
echo.
set /p choice="Enter choice (1-2): "

if "%choice%"=="1" goto csv
if "%choice%"=="2" goto text
echo Invalid choice, using CSV format...

:csv
echo.
echo Training with CSV datasets...
echo.
linear_regression_csv.exe ../dataset/processed/train_dataset.csv ../dataset/processed/test_dataset.csv
goto end

:text
echo.
echo Training with text datasets...
echo.
linear_regression_csv.exe ../dataset/processed/train.data ../dataset/processed/test.data
goto end

:end
echo.
echo ========================================
echo Training Complete!
echo ========================================
pause
