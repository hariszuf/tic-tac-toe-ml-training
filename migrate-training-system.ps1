# Train Models Compare - Complete Migration Script
# Migrates ONLY the train_models_compare.bat system and its dependencies

param(
    [string]$NewRepoPath = "c:\Users\muhdh\Documents\GitHub\ttt-ml-training",
    [switch]$IncludeDatasets,
    [switch]$IncludeModels
)

$oldRepo = "c:\Users\muhdh\Documents\GitHub\tic-tac-toe-ml-training"

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "TRAIN_MODELS_COMPARE MIGRATION" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

Write-Host "This will migrate the complete training system:" -ForegroundColor Yellow
Write-Host "  ✓ train_models_compare.bat (main script)" -ForegroundColor White
Write-Host "  ✓ dataset-gen.c (dataset generator)" -ForegroundColor White
Write-Host "  ✓ dataset_processor.c (train/test splitter)" -ForegroundColor White
Write-Host "  ✓ naive_bayes.c (trainer)" -ForegroundColor White
Write-Host "  ✓ linear_regression.c (trainer)" -ForegroundColor White
Write-Host "  ✓ q_learning.c (trainer)" -ForegroundColor White
Write-Host "  ✓ Documentation files" -ForegroundColor White

if ($IncludeDatasets) {
    Write-Host "  ✓ Dataset files (.data)" -ForegroundColor Green
}
if ($IncludeModels) {
    Write-Host "  ✓ Trained models" -ForegroundColor Green
}

Write-Host "`nSource: $oldRepo" -ForegroundColor Yellow
Write-Host "Target: $NewRepoPath`n" -ForegroundColor Green

$confirm = Read-Host "Continue? (y/n)"
if ($confirm -ne "y") {
    Write-Host "Migration cancelled." -ForegroundColor Red
    exit
}

# Create directory structure
Write-Host "`n[1/8] Creating directory structure..." -ForegroundColor Cyan
New-Item -ItemType Directory -Path "$NewRepoPath" -Force | Out-Null
New-Item -ItemType Directory -Path "$NewRepoPath\models" -Force | Out-Null

$folders = @("naive_bayes_non_terminal", "naive_bayes_combined", 
             "linear_regression_non_terminal", "linear_regression_combined")
foreach ($folder in $folders) {
    New-Item -ItemType Directory -Path "$NewRepoPath\models\$folder" -Force | Out-Null
}

Write-Host "  ✓ Directories created" -ForegroundColor Green

# Copy source files
Write-Host "`n[2/8] Copying source files..." -ForegroundColor Cyan

# From src-haris
$srcHarisFiles = @("dataset-gen.c", "dataset_processor.c", "q_learning.c")
foreach ($file in $srcHarisFiles) {
    $srcPath = "$oldRepo\src-haris\$file"
    if (Test-Path $srcPath) {
        Copy-Item $srcPath "$NewRepoPath\"
        Write-Host "  ✓ $file" -ForegroundColor Green
    }
}

# From src
$srcFiles = @("naive_bayes.c", "linear_regression.c")
foreach ($file in $srcFiles) {
    $srcPath = "$oldRepo\src\$file"
    if (Test-Path $srcPath) {
        Copy-Item $srcPath "$NewRepoPath\"
        Write-Host "  ✓ $file" -ForegroundColor Green
    }
}

# Copy and update batch file
Write-Host "`n[3/8] Copying batch file..." -ForegroundColor Cyan
$batchPath = "$oldRepo\src-haris\train_models_compare.bat"
if (Test-Path $batchPath) {
    $batchContent = Get-Content $batchPath -Raw
    $batchContent = $batchContent -replace '\.\.\\src\\naive_bayes\.c', 'naive_bayes.c'
    $batchContent = $batchContent -replace '\.\.\\src\\linear_regression\.c', 'linear_regression.c'
    $batchContent = $batchContent -replace 'gcc \.\.\\src\\naive_bayes\.c', 'gcc naive_bayes.c'
    $batchContent = $batchContent -replace 'gcc \.\.\\src\\linear_regression\.c', 'gcc linear_regression.c'
    $batchContent = $batchContent -replace '\.\.\\models\\', 'models\'
    $batchContent = $batchContent -replace '\.\.\\dataset\\processed', 'models\temp'
    Set-Content -Path "$NewRepoPath\train_models_compare.bat" -Value $batchContent
    Write-Host "  ✓ train_models_compare.bat (paths updated)" -ForegroundColor Green
}

# Copy documentation
Write-Host "`n[4/8] Copying documentation..." -ForegroundColor Cyan
$docFiles = @("Q_LEARNING_GUIDE.md", "TRAINING_GUIDE.md")
foreach ($file in $docFiles) {
    $srcPath = "$oldRepo\src-haris\$file"
    if (Test-Path $srcPath) {
        Copy-Item $srcPath "$NewRepoPath\"
        Write-Host "  ✓ $file" -ForegroundColor Green
    }
}

# Copy datasets
if ($IncludeDatasets) {
    Write-Host "`n[5/8] Copying datasets..." -ForegroundColor Cyan
    $datasetFiles = @("tic-tac-toe-minimax-non-terminal.data", "tic-tac-toe-minimax-complete.data")
    foreach ($file in $datasetFiles) {
        $srcPath = "$oldRepo\src-haris\$file"
        if (Test-Path $srcPath) {
            Copy-Item $srcPath "$NewRepoPath\"
            Write-Host "  ✓ $file" -ForegroundColor Green
        }
    }
} else {
    Write-Host "`n[5/8] Skipping datasets" -ForegroundColor Yellow
}

# Copy models
if ($IncludeModels) {
    Write-Host "`n[6/8] Copying trained models..." -ForegroundColor Cyan
    foreach ($folder in $folders) {
        $srcPath = "$oldRepo\models\$folder"
        if (Test-Path $srcPath) {
            Copy-Item "$srcPath\*" "$NewRepoPath\models\$folder\" -Force
            Write-Host "  ✓ $folder" -ForegroundColor Green
        }
    }
} else {
    Write-Host "`n[6/8] Skipping trained models" -ForegroundColor Yellow
}

# Create README
Write-Host "`n[7/8] Creating README.md..." -ForegroundColor Cyan
$readme = @"
# Tic-Tac-Toe ML Training System

Automated training system for tic-tac-toe AI using minimax datasets.

## Quick Start

``````powershell
# 1. Generate datasets
gcc dataset-gen.c -o dataset-gen.exe
.\dataset-gen.exe  # Choose option 3

# 2. Train all models
.\train_models_compare.bat

# 3. Check results
tree output /F
``````

See TRAINING_GUIDE.md for complete instructions.
"@
Set-Content -Path "$NewRepoPath\README.md" -Value $readme
Write-Host "  ✓ README.md" -ForegroundColor Green

# Create .gitignore
$gitignore = @"
*.exe
*.o
*.bin
*.tmp
output/datasets/*.data
output/reports/*.txt
.vscode/
"@
Set-Content -Path "$NewRepoPath\.gitignore" -Value $gitignore
Write-Host "  ✓ .gitignore" -ForegroundColor Green

Write-Host "`n[8/8] Complete!" -ForegroundColor Green
Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "✅ MIGRATION COMPLETE" -ForegroundColor Green
Write-Host "========================================`n" -ForegroundColor Cyan
Write-Host "Location: $NewRepoPath`n" -ForegroundColor Cyan
Write-Host "Next: cd $NewRepoPath && .\train_models_compare.bat`n" -ForegroundColor Yellow
