# Tic-Tac-Toe ML Migration Script
# Migrates the minimax-based training system to a new repository

param(
    [string]$NewRepoPath = "c:\Users\muhdh\Documents\GitHub\tic-tac-toe-ml-minimax",
    [switch]$IncludeDatasets,
    [switch]$IncludeOutput
)

$oldRepo = "c:\Users\muhdh\Documents\GitHub\tic-tac-toe-ml-training"

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "TIC-TAC-TOE ML MIGRATION SCRIPT" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

Write-Host "Source: $oldRepo" -ForegroundColor Yellow
Write-Host "Target: $NewRepoPath`n" -ForegroundColor Green

# Confirm before proceeding
$confirm = Read-Host "Continue? (y/n)"
if ($confirm -ne "y") {
    Write-Host "Migration cancelled." -ForegroundColor Red
    exit
}

# Create new repository structure
Write-Host "`n[1/7] Creating folder structure..." -ForegroundColor Cyan
New-Item -ItemType Directory -Path "$NewRepoPath\src" -Force | Out-Null
New-Item -ItemType Directory -Path "$NewRepoPath\dataset" -Force | Out-Null
New-Item -ItemType Directory -Path "$NewRepoPath\models" -Force | Out-Null
New-Item -ItemType Directory -Path "$NewRepoPath\docs" -Force | Out-Null
Write-Host "  ✓ Folders created" -ForegroundColor Green

# Copy source files
Write-Host "`n[2/7] Copying source files (.c)..." -ForegroundColor Cyan
$sourceFiles = @(
    "dataset-gen.c",
    "dataset_processor.c", 
    "q_learning.c"
)

foreach ($file in $sourceFiles) {
    if (Test-Path "$oldRepo\src-haris\$file") {
        Copy-Item "$oldRepo\src-haris\$file" "$NewRepoPath\src\"
        Write-Host "  ✓ $file" -ForegroundColor Green
    }
}

# Copy training programs from main src
$trainingFiles = @("naive_bayes.c", "linear_regression.c")
foreach ($file in $trainingFiles) {
    if (Test-Path "$oldRepo\src\$file") {
        Copy-Item "$oldRepo\src\$file" "$NewRepoPath\src\"
        Write-Host "  ✓ $file (from src)" -ForegroundColor Green
    }
}

# Copy batch files
Write-Host "`n[3/7] Copying batch files..." -ForegroundColor Cyan
$batchFiles = @(
    "train_models_compare.bat",
    "train_models.bat",
    "train_qlearning.bat"
)

foreach ($file in $batchFiles) {
    if (Test-Path "$oldRepo\src-haris\$file") {
        Copy-Item "$oldRepo\src-haris\$file" "$NewRepoPath\src\"
        Write-Host "  ✓ $file" -ForegroundColor Green
    }
}

# Copy models
Write-Host "`n[4/7] Copying trained models..." -ForegroundColor Cyan
$modelFolders = @(
    "naive_bayes_non_terminal",
    "naive_bayes_combined",
    "linear_regression_non_terminal",
    "linear_regression_combined"
)

foreach ($folder in $modelFolders) {
    if (Test-Path "$oldRepo\models\$folder") {
        Copy-Item "$oldRepo\models\$folder" "$NewRepoPath\models\" -Recurse -Force
        Write-Host "  ✓ $folder" -ForegroundColor Green
    }
}

# Copy documentation
Write-Host "`n[5/7] Copying documentation..." -ForegroundColor Cyan
$docFiles = @(
    "Q_LEARNING_GUIDE.md",
    "TRAINING_GUIDE.md"
)

foreach ($file in $docFiles) {
    if (Test-Path "$oldRepo\src-haris\$file") {
        Copy-Item "$oldRepo\src-haris\$file" "$NewRepoPath\docs\"
        Write-Host "  ✓ $file" -ForegroundColor Green
    }
}

# Copy main MIGRATION_GUIDE
if (Test-Path "$oldRepo\MIGRATION_GUIDE.md") {
    Copy-Item "$oldRepo\MIGRATION_GUIDE.md" "$NewRepoPath\docs\"
    Write-Host "  ✓ MIGRATION_GUIDE.md" -ForegroundColor Green
}

# Optional: Copy datasets
if ($IncludeDatasets) {
    Write-Host "`n[6/7] Copying datasets..." -ForegroundColor Cyan
    $datasetFiles = Get-ChildItem "$oldRepo\src-haris\*.data" -ErrorAction SilentlyContinue
    foreach ($file in $datasetFiles) {
        Copy-Item $file.FullName "$NewRepoPath\dataset\"
        Write-Host "  ✓ $($file.Name)" -ForegroundColor Green
    }
} else {
    Write-Host "`n[6/7] Skipping datasets (use -IncludeDatasets to copy)" -ForegroundColor Yellow
}

# Optional: Copy output folder
if ($IncludeOutput -and (Test-Path "$oldRepo\src-haris\output")) {
    Write-Host "`n[7/7] Copying output folder..." -ForegroundColor Cyan
    Copy-Item "$oldRepo\src-haris\output" "$NewRepoPath\" -Recurse -Force
    Write-Host "  ✓ Output folder copied" -ForegroundColor Green
} else {
    Write-Host "`n[7/7] Skipping output folder (use -IncludeOutput to copy)" -ForegroundColor Yellow
}

# Create README.md
Write-Host "`nCreating README.md..." -ForegroundColor Cyan
$readmeContent = @"
# Tic-Tac-Toe ML Training System

Minimax-based machine learning training system for tic-tac-toe AI.

## 🎯 Features

- **Minimax Dataset Generation**: Generates datasets with optimal play evaluation
- **Three ML Algorithms**: 
  - Naive Bayes (probabilistic)
  - Linear Regression (numerical prediction)
  - Q-Learning (reinforcement learning)
- **Automated Training Pipeline**: Train all models with comparison
- **Organized Output Structure**: Clear separation of models and datasets

## 📁 Repository Structure

``````
src/                    # Source code
├── dataset-gen.c       # Minimax dataset generator
├── dataset_processor.c # Train/test splitter
├── naive_bayes.c       # Naive Bayes trainer
├── linear_regression.c # Linear Regression trainer
├── q_learning.c        # Q-Learning trainer
└── *.bat              # Training automation scripts

models/                 # Trained models
├── *_non_terminal/    # Models trained on strategic positions
└── *_combined/        # Models trained on all positions

dataset/               # Generated datasets
docs/                  # Documentation
``````

## 🚀 Quick Start

### 1. Generate Datasets
``````powershell
cd src
gcc dataset-gen.c -o dataset-gen.exe
.\dataset-gen.exe
``````

Choose option:
- **1**: Non-terminal only (~4,520 strategic positions)
- **2**: Complete dataset (~5,500 all positions)
- **3**: Both (recommended)

### 2. Train Models
``````powershell
.\train_models_compare.bat
``````

This will:
- Compile all training programs
- Process both datasets
- Train all three algorithms
- Organize outputs automatically

### 3. Check Results
``````powershell
cd output
tree /F
``````

Review `output/reports/` for statistical analysis.

## 📚 Documentation

- [Training Guide](docs/TRAINING_GUIDE.md) - Complete training workflow
- [Q-Learning Guide](docs/Q_LEARNING_GUIDE.md) - Q-Learning specifics
- [Migration Guide](docs/MIGRATION_GUIDE.md) - Repository setup

## 🔧 Requirements

- **Compiler**: GCC (MinGW for Windows)
- **OS**: Windows (batch files) or adapt for Linux/Mac
- **Memory**: ~50MB for datasets

## 📊 Dataset Types

### Non-Terminal Dataset
- ~4,520 samples
- Games in progress only
- Focuses on strategic decision-making
- Better for gameplay AI

### Combined Dataset  
- ~5,500 samples
- Includes terminal states (game endings)
- Complete state coverage
- Better for general evaluation

## 🎮 Model Comparison

After training, compare:
1. **Test Accuracy**: Check training output
2. **Reports**: See `output/reports/*.txt`
3. **AI vs AI**: Test in actual gameplay

## 🤝 Contributing

This is a clean migration of the minimax-based training system.
Original repository: tic-tac-toe-ml-training

## 📝 License

[Your License Here]

## 🙏 Acknowledgments

Migrated from comprehensive tic-tac-toe ML research project.
"@

Set-Content -Path "$NewRepoPath\README.md" -Value $readmeContent
Write-Host "  ✓ README.md created" -ForegroundColor Green

# Create .gitignore
Write-Host "Creating .gitignore..." -ForegroundColor Cyan
$gitignoreContent = @"
# Executables
*.exe
*.out
*.o

# Binary models (regenerate after training)
*.bin

# Temporary files
*.tmp
temp/

# Generated datasets (optional - uncomment to ignore)
# dataset/*.data

# Training outputs (optional - uncomment to ignore)
# output/datasets/*.data
# output/reports/*.txt

# IDE files
.vscode/
.vs/
*.suo
*.user

# OS files
.DS_Store
Thumbs.db
desktop.ini
"@

Set-Content -Path "$NewRepoPath\.gitignore" -Value $gitignoreContent
Write-Host "  ✓ .gitignore created" -ForegroundColor Green

# Summary
Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "MIGRATION COMPLETE!" -ForegroundColor Green
Write-Host "========================================`n" -ForegroundColor Cyan

Write-Host "New repository location:" -ForegroundColor Yellow
Write-Host "  $NewRepoPath`n" -ForegroundColor White

Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. cd $NewRepoPath" -ForegroundColor White
Write-Host "  2. git init" -ForegroundColor White
Write-Host "  3. git add ." -ForegroundColor White
Write-Host "  4. git commit -m 'Initial commit: Minimax ML training system'" -ForegroundColor White
Write-Host "  5. Connect to GitHub and push" -ForegroundColor White
Write-Host "`n  OR use GitHub Desktop to publish`n" -ForegroundColor White

Write-Host "Test the migration:" -ForegroundColor Yellow
Write-Host "  cd $NewRepoPath\src" -ForegroundColor White
Write-Host "  .\train_models_compare.bat`n" -ForegroundColor White

Write-Host "Files migrated:" -ForegroundColor Cyan
$fileCount = (Get-ChildItem -Path $NewRepoPath -Recurse -File).Count
Write-Host "  Total: $fileCount files`n" -ForegroundColor White

# Create summary file
$summary = @"
MIGRATION SUMMARY
Generated: $(Get-Date)

Source: $oldRepo
Target: $NewRepoPath

Migrated Components:
✓ 5 Core C source files
✓ 3 Batch automation files
✓ 4 Model directories
✓ 3 Documentation files
✓ README.md and .gitignore created

$(if ($IncludeDatasets) { "✓ Dataset files included" } else { "○ Dataset files not included (can be regenerated)" })
$(if ($IncludeOutput) { "✓ Output folder included" } else { "○ Output folder not included (will be regenerated)" })

Next Actions Required:
1. Initialize git repository
2. Test compilation: gcc dataset-gen.c -o dataset-gen.exe
3. Run training: train_models_compare.bat
4. Verify all outputs in output/ folder
5. Push to GitHub

Notes:
- All paths in batch files should work as-is
- Recompile all .exe files in new location
- Generate new datasets if not included
- Review and update README.md as needed
"@

Set-Content -Path "$NewRepoPath\MIGRATION_SUMMARY.txt" -Value $summary
Write-Host "Migration summary saved to: MIGRATION_SUMMARY.txt`n" -ForegroundColor Cyan
