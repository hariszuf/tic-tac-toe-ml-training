# Quick Migration Reference Card

## 🚀 Fastest Way to Migrate (Recommended)

### Option 1: Use the Automated Script
```powershell
# Run from current repo
.\migrate-to-new-repo.ps1

# With datasets and outputs
.\migrate-to-new-repo.ps1 -IncludeDatasets -IncludeOutput

# Custom location
.\migrate-to-new-repo.ps1 -NewRepoPath "c:\path\to\new\repo"
```

### Option 2: Manual Copy (3 commands)
```powershell
# Create and copy
mkdir c:\Users\muhdh\Documents\GitHub\new-repo
cd c:\Users\muhdh\Documents\GitHub\new-repo

# Copy minimax system only
robocopy ..\tic-tac-toe-ml-training\src-haris .\src *.c *.bat /E
robocopy ..\tic-tac-toe-ml-training\models .\models *_non_terminal *_combined /E
```

---

## 📋 What to Migrate (Checklist)

### ✅ Must Include
- [ ] `dataset-gen.c` (dataset generator)
- [ ] `dataset_processor.c` (train/test splitter)
- [ ] `naive_bayes.c` (from src/)
- [ ] `linear_regression.c` (from src/)
- [ ] `q_learning.c` (from src-haris/)
- [ ] `train_models_compare.bat` (main training script)
- [ ] Trained model folders (*_non_terminal, *_combined)

### 🔄 Optional (Can Regenerate)
- [ ] `*.data` files (datasets)
- [ ] `output/` folder
- [ ] Compiled `.exe` files

### 📚 Documentation
- [ ] `Q_LEARNING_GUIDE.md`
- [ ] `TRAINING_GUIDE.md`
- [ ] `MIGRATION_GUIDE.md`

---

## 🎯 After Migration Checklist

```powershell
cd new-repo

# 1. Test compilation
cd src
gcc dataset-gen.c -o dataset-gen.exe
gcc dataset_processor.c -o dataset_processor.exe
gcc q_learning.c -o q_learning.exe -lm

# 2. Generate datasets (if not included)
.\dataset-gen.exe   # Choose option 3 for both

# 3. Test training
.\train_models_compare.bat

# 4. Verify outputs
cd ..
tree output /F

# 5. Initialize git
git init
git add .
git commit -m "Initial commit"
git remote add origin https://github.com/username/repo.git
git push -u origin main
```

---

## 📁 Recommended New Structure

```
new-repo/
├── src/                          # All source code here
│   ├── dataset-gen.c
│   ├── dataset_processor.c
│   ├── naive_bayes.c
│   ├── linear_regression.c
│   ├── q_learning.c
│   └── train_models_compare.bat
│
├── models/                       # Trained models
│   ├── naive_bayes_non_terminal/
│   ├── naive_bayes_combined/
│   ├── linear_regression_non_terminal/
│   └── linear_regression_combined/
│
├── dataset/                      # Generated datasets
│   ├── tic-tac-toe-minimax-non-terminal.data
│   └── tic-tac-toe-minimax-complete.data
│
├── output/                       # Auto-generated outputs
│   ├── non_terminal/
│   ├── combined/
│   ├── datasets/
│   └── reports/
│
├── docs/                         # Documentation
│   ├── Q_LEARNING_GUIDE.md
│   ├── TRAINING_GUIDE.md
│   └── MIGRATION_GUIDE.md
│
├── README.md
└── .gitignore
```

---

## ⚡ One-Line Migrations

### Just the Code
```powershell
robocopy tic-tac-toe-ml-training\src-haris new-repo\src *.c *.bat
```

### Code + Models
```powershell
robocopy tic-tac-toe-ml-training\src-haris new-repo\src *.c *.bat & robocopy tic-tac-toe-ml-training\models new-repo\models /E
```

### Everything
```powershell
.\migrate-to-new-repo.ps1 -IncludeDatasets -IncludeOutput
```

---

## 🔧 Path Updates Needed

### In batch files, change:
```batch
# OLD (src-haris structure)
gcc ..\src\naive_bayes.c -o naive_bayes.exe

# NEW (if src/ contains everything)
gcc naive_bayes.c -o naive_bayes.exe
```

### In C files, check:
- `linear_regression.c` → `#define DATASET_PATH`
- `naive_bayes.c` → `#define MODEL_PATH`

Update to match your new structure.

---

## 🎮 Suggested Repository Names

- `tic-tac-toe-ml-minimax` (descriptive)
- `ttt-ml-training` (concise)
- `minimax-ml-trainer` (focus on method)
- `tictactoe-ai-training` (general)

---

## ⚠️ Common Issues

### Issue: Batch file can't find files
**Solution**: Update paths in `.bat` files

### Issue: Can't compile
**Solution**: Check gcc in PATH: `gcc --version`

### Issue: Models don't load
**Solution**: Check model file paths match program expectations

### Issue: Datasets empty
**Solution**: Regenerate with `dataset-gen.exe`

---

## 💡 Pro Tips

1. **Don't migrate .git folder** - start fresh
2. **Regenerate datasets** - ensures reproducibility  
3. **Test before pushing** - run full training pipeline
4. **Update README** - document new structure
5. **Add .gitignore** - exclude .exe and temp files

---

## 📞 Quick Reference

| Task | Command |
|------|---------|
| Auto migrate | `.\migrate-to-new-repo.ps1` |
| Manual copy | `robocopy old\src new\src *.c *.bat` |
| Test compile | `gcc dataset-gen.c -o dataset-gen.exe` |
| Generate data | `.\dataset-gen.exe` |
| Train models | `.\train_models_compare.bat` |
| Check output | `tree output /F` |
| Init git | `git init && git add . && git commit -m "Initial"` |

---

## 🎯 TL;DR

**Easiest method:**
```powershell
cd c:\Users\muhdh\Documents\GitHub\tic-tac-toe-ml-training
.\migrate-to-new-repo.ps1
cd ..\tic-tac-toe-ml-minimax
git init
# Test training, then push to GitHub
```

Done! ✅
