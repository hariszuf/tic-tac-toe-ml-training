# Repository Migration Guide

This guide explains how to migrate the tic-tac-toe ML training system to a new repository while maintaining functionality.

## 📦 What You Have

Your current repository has two main workflows:

### **Option 1: Original CSV-based System** (`src/` folder)
- Uses UCI dataset format
- Training programs: `linear_regression_csv.c`, `naive_bayes_csv.c`
- Batch files: `train_with_csv.bat`, `evaluate.bat`

### **Option 2: Minimax-based System** (`src-haris/` folder) 
- Uses minimax-generated datasets
- Training programs: `linear_regression.c`, `naive_bayes.c`, `q_learning.c`
- Dataset generator: `dataset-gen.c`, `dataset_processor.c`
- Batch files: `train_models_compare.bat`, `train_models.bat`

---

## 🎯 Migration Strategy

You have **three options** depending on what you want to migrate:

### **Option A: Migrate Everything** (Full Repository)
Best if you want to keep all history and workflows.

### **Option B: Migrate Only Minimax System** (Recommended)
Best if you only want the newer, self-contained minimax-based system.

### **Option C: Migrate Specific Components**
Best if you want to cherry-pick specific parts.

---

## 📋 Option A: Migrate Full Repository

### Step 1: Clone to New Repository
```powershell
# Navigate to where you want the new repo
cd c:\Users\muhdh\Documents\GitHub

# Create new repository on GitHub first, then:
git clone https://github.com/yourusername/new-repo-name.git
cd new-repo-name

# Copy everything from old repo
Copy-Item -Path "..\tic-tac-toe-ml-training\*" -Destination "." -Recurse -Exclude ".git"

# Initialize git if needed
git add .
git commit -m "Initial migration from tic-tac-toe-ml-training"
git push origin main
```

### Pros:
- ✅ Complete history and all files
- ✅ Both workflows available
- ✅ All documentation preserved

### Cons:
- ❌ Includes duplicate/redundant systems
- ❌ Larger repository size
- ❌ May be confusing which system to use

---

## 📋 Option B: Migrate Minimax System Only (Recommended)

This creates a clean, self-contained repository with just the minimax-based training system.

### File Structure to Migrate:

```
new-repo/
├── dataset/                          # Generated datasets
│   └── tic-tac-toe-minimax-*.data
│
├── models/                           # Trained models
│   ├── naive_bayes_non_terminal/
│   ├── naive_bayes_combined/
│   ├── linear_regression_non_terminal/
│   └── linear_regression_combined/
│
├── src/                              # Core training programs
│   ├── dataset-gen.c                 # Dataset generator
│   ├── dataset_processor.c           # Train/test splitter
│   ├── naive_bayes.c                 # Naive Bayes trainer
│   ├── linear_regression.c           # Linear Regression trainer
│   ├── q_learning.c                  # Q-Learning trainer
│   ├── train_models_compare.bat      # Main training script
│   ├── train_models.bat              # Simple training
│   └── train_qlearning.bat           # Q-Learning only
│
├── output/                           # Training outputs
│   ├── non_terminal/
│   ├── combined/
│   ├── datasets/
│   └── reports/
│
├── docs/                             # Documentation
│   ├── Q_LEARNING_GUIDE.md
│   ├── TRAINING_GUIDE.md
│   └── MIGRATION_GUIDE.md
│
├── README.md                         # Main documentation
└── .gitignore                        # Git ignore rules
```

### Migration Commands:

```powershell
# Create new repository directory
cd c:\Users\muhdh\Documents\GitHub
mkdir tic-tac-toe-ml-minimax
cd tic-tac-toe-ml-minimax

# Create folder structure
mkdir dataset, models, src, output, docs

# Copy dataset generator and training programs
Copy-Item "..\tic-tac-toe-ml-training\src-haris\dataset-gen.c" "src\"
Copy-Item "..\tic-tac-toe-ml-training\src-haris\dataset_processor.c" "src\"
Copy-Item "..\tic-tac-toe-ml-training\src-haris\q_learning.c" "src\"
Copy-Item "..\tic-tac-toe-ml-training\src\naive_bayes.c" "src\"
Copy-Item "..\tic-tac-toe-ml-training\src\linear_regression.c" "src\"

# Copy batch files
Copy-Item "..\tic-tac-toe-ml-training\src-haris\train_models_compare.bat" "src\"
Copy-Item "..\tic-tac-toe-ml-training\src-haris\train_models.bat" "src\"
Copy-Item "..\tic-tac-toe-ml-training\src-haris\train_qlearning.bat" "src\"

# Copy generated datasets (if you want them)
Copy-Item "..\tic-tac-toe-ml-training\src-haris\tic-tac-toe-minimax-*.data" "dataset\"

# Copy trained models
Copy-Item "..\tic-tac-toe-ml-training\models\*_non_terminal" "models\" -Recurse
Copy-Item "..\tic-tac-toe-ml-training\models\*_combined" "models\" -Recurse

# Copy output folder
Copy-Item "..\tic-tac-toe-ml-training\src-haris\output" "." -Recurse

# Copy documentation
Copy-Item "..\tic-tac-toe-ml-training\src-haris\Q_LEARNING_GUIDE.md" "docs\"
Copy-Item "..\tic-tac-toe-ml-training\src-haris\TRAINING_GUIDE.md" "docs\"
Copy-Item "..\tic-tac-toe-ml-training\MIGRATION_GUIDE.md" "docs\"

# Create README (see template below)
# Create .gitignore (see template below)

# Initialize git
git init
git add .
git commit -m "Initial commit: Minimax-based ML training system"

# Connect to GitHub
git remote add origin https://github.com/yourusername/new-repo-name.git
git branch -M main
git push -u origin main
```

### Pros:
- ✅ Clean, focused repository
- ✅ Self-contained system
- ✅ Easy to understand structure
- ✅ No redundant code

### Cons:
- ❌ Loses CSV-based workflow
- ❌ Loses git history

---

## 📋 Option C: Migrate Specific Components

### Minimal Migration (Just Training Programs):
```powershell
mkdir ml-training
cd ml-training

# Copy only the core C files
Copy-Item "..\tic-tac-toe-ml-training\src\naive_bayes.c" "."
Copy-Item "..\tic-tac-toe-ml-training\src\linear_regression.c" "."
Copy-Item "..\tic-tac-toe-ml-training\src-haris\q_learning.c" "."
Copy-Item "..\tic-tac-toe-ml-training\src-haris\dataset_processor.c" "."
```

### Dataset Generator Only:
```powershell
mkdir ttt-dataset-generator
cd ttt-dataset-generator

Copy-Item "..\tic-tac-toe-ml-training\src-haris\dataset-gen.c" "."
```

---

## 📝 Required File Updates After Migration

### 1. Update Batch File Paths

If you reorganize folders, update paths in `train_models_compare.bat`:

**OLD** (src-haris structure):
```batch
gcc ..\src\naive_bayes.c -o naive_bayes.exe -lm
if not exist "..\models\naive_bayes_non_terminal" mkdir "..\models\naive_bayes_non_terminal"
```

**NEW** (if everything is in src/):
```batch
gcc naive_bayes.c -o naive_bayes.exe -lm
if not exist "..\models\naive_bayes_non_terminal" mkdir "..\models\naive_bayes_non_terminal"
```

### 2. Update Program Paths

In the C files, check for hardcoded paths:
- `linear_regression.c` line ~10: `#define DATASET_PATH "../dataset/processed/"`
- `naive_bayes.c` line ~10: `#define MODEL_PATH "../models/naive bayes/"`

Update these to match your new folder structure.

### 3. Create New README.md

```markdown
# Tic-Tac-Toe ML Training System

Minimax-based machine learning training system for tic-tac-toe AI.

## Features
- Minimax dataset generation (optimal play evaluation)
- Three ML algorithms: Naive Bayes, Linear Regression, Q-Learning
- Automated training pipeline with comparison
- Organized output structure

## Quick Start
1. Generate datasets: `cd src && gcc dataset-gen.c -o dataset-gen.exe && dataset-gen.exe`
2. Train models: `train_models_compare.bat`
3. Check results: See `output/` folder

## Documentation
- [Training Guide](docs/TRAINING_GUIDE.md)
- [Q-Learning Guide](docs/Q_LEARNING_GUIDE.md)
```

### 4. Create .gitignore

```gitignore
# Executables
*.exe
*.out
*.bin

# Compiled files
*.o
*.obj

# Generated datasets (optional - you may want to commit these)
# dataset/*.data

# Temporary files
*.tmp
temp/
output/datasets/*.data
output/reports/*.txt

# IDE files
.vscode/
.vs/
*.suo
*.user

# OS files
.DS_Store
Thumbs.db
desktop.ini
```

---

## 🔧 Post-Migration Testing

After migration, test everything:

### 1. Compile All Programs
```powershell
cd src

# Test dataset generator
gcc dataset-gen.c -o dataset-gen.exe
.\dataset-gen.exe

# Test processors
gcc dataset_processor.c -o dataset_processor.exe
gcc naive_bayes.c -o naive_bayes.exe -lm
gcc linear_regression.c -o linear_regression.exe -lm
gcc q_learning.c -o q_learning.exe -lm
```

### 2. Run Training Pipeline
```powershell
.\train_models_compare.bat
```

### 3. Verify Outputs
```powershell
# Check if all folders created
tree output /F

# Check if models exist
dir ..\models\*_non_terminal
dir ..\models\*_combined
```

---

## 🎯 Recommended Approach

**I recommend Option B** (Migrate Minimax System Only) because:

1. ✅ **Clean separation**: Minimax system is self-contained
2. ✅ **Better organization**: src-haris has the improved structure
3. ✅ **Modern approach**: Generates its own datasets with optimal play
4. ✅ **Well documented**: Has complete guides and automated workflows
5. ✅ **Future-proof**: Easy to extend with more algorithms

### Suggested New Repository Name:
- `tic-tac-toe-ml-minimax`
- `ttt-ml-training`
- `minimax-ml-trainer`

---

## ⚠️ Important Notes

### Don't Migrate:
- ❌ `.git` folder (creates conflicts)
- ❌ Executable files (*.exe) - recompile in new location
- ❌ Binary model files initially - regenerate after migration
- ❌ Temporary output files

### Do Migrate:
- ✅ All `.c` source files
- ✅ All `.bat` batch files
- ✅ Documentation (*.md files)
- ✅ Trained model `.txt` files (reference)
- ✅ Dataset `.data` files (optional - can regenerate)

### After Migration:
1. Update all relative paths in batch files
2. Update hardcoded paths in C files
3. Recompile all executables
4. Test complete workflow
5. Update documentation with new structure

---

## 📞 Need Help?

If you run into issues:
1. Check that all paths are updated correctly
2. Verify gcc is in PATH
3. Ensure folder structure matches batch file expectations
4. Test each component individually before running full pipeline

---

## 🚀 Quick Migration Script

Here's a complete PowerShell script for **Option B**:

```powershell
# Save this as migrate.ps1 and run it

$oldRepo = "c:\Users\muhdh\Documents\GitHub\tic-tac-toe-ml-training"
$newRepo = "c:\Users\muhdh\Documents\GitHub\tic-tac-toe-ml-minimax"

# Create new repo structure
New-Item -ItemType Directory -Path "$newRepo\src" -Force
New-Item -ItemType Directory -Path "$newRepo\dataset" -Force
New-Item -ItemType Directory -Path "$newRepo\models" -Force
New-Item -ItemType Directory -Path "$newRepo\docs" -Force

# Copy source files
Copy-Item "$oldRepo\src-haris\*.c" "$newRepo\src\"
Copy-Item "$oldRepo\src-haris\*.bat" "$newRepo\src\"

# Copy models
Copy-Item "$oldRepo\models\*_non_terminal" "$newRepo\models\" -Recurse
Copy-Item "$oldRepo\models\*_combined" "$newRepo\models\" -Recurse

# Copy docs
Copy-Item "$oldRepo\src-haris\*.md" "$newRepo\docs\"

# Copy output if exists
if (Test-Path "$oldRepo\src-haris\output") {
    Copy-Item "$oldRepo\src-haris\output" "$newRepo\" -Recurse
}

Write-Host "Migration complete! Check $newRepo" -ForegroundColor Green
```

Run it with: `.\migrate.ps1`
