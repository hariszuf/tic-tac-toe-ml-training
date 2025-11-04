# Model Evaluation & Retraining Guide

## 📋 Quick Reference

### Current Model Performance
| Model | Test Accuracy | Status |
|-------|---------------|--------|
| **Linear Regression** | 96.35% | ✅ Excellent |
| **Naive Bayes** | ~88% | ✅ Good |

---

## 🔍 How to Evaluate Models

### Method 1: Run Comprehensive Evaluation Tool

```bash
cd src
gcc evaluate_models.c -o evaluate_models.exe -lm -Wall
./evaluate_models.exe
```

**What it shows:**
- Overall accuracy, MSE, RMSE, MAE
- Per-class performance (Win/Lose/Draw)
- Precision and Recall metrics
- Overfitting detection
- Specific recommendations

### Method 2: Re-run Training with Verbose Output

**Linear Regression:**
```bash
cd src
gcc linear_regression.c -o linear_regression.exe -lm -Wall
./linear_regression.exe
```

**Naive Bayes:**
```bash
cd src
gcc naive_bayes.c -o naive_bayes.exe -Wall
./naive_bayes.exe
```

---

## 🎯 Key Metrics to Monitor

### 1. **Accuracy Metrics**
- **Overall Accuracy**: Should be >90% for LR, >85% for NB
- **Per-class Accuracy**: Check Win/Lose/Draw separately
- **Training vs Test Gap**: Should be <10% (overfitting check)

### 2. **Regression Metrics (Linear Regression)**
- **MSE (Mean Squared Error)**: Lower is better, target <0.02
- **RMSE (Root MSE)**: Interpretable in same units as labels
- **MAE (Mean Absolute Error)**: Average prediction error

### 3. **Classification Metrics**
- **Precision**: Of predicted wins, how many are correct?
- **Recall**: Of actual wins, how many did we predict?
- **F1-Score**: Harmonic mean of precision and recall

### 4. **Cross-Validation (Naive Bayes)**
- Average across 6 folds
- Look for consistency (low variance between folds)

---

## 🔄 When to Retrain: Decision Tree

```
START
  │
  ├─► Is test accuracy < 85%? ────YES──► Retrain with more epochs/data
  │                            NO
  │                            │
  ├─► Is train accuracy - test accuracy > 10%? ───YES──► Overfitting! Reduce epochs
  │                                               NO
  │                                               │
  ├─► Did you add new data to dataset? ──────YES──► Retrain from scratch
  │                                         NO
  │                                         │
  ├─► Has game behavior changed? ───────YES──► Retrain with new config
  │                                    NO
  │                                    │
  └─► Model is good! No retraining needed ✓
```

---

## ⚙️ Hyperparameter Tuning

### Linear Regression Parameters

Edit `src/linear_regression.c` (lines 236-239):

```c
int epochs = 1000;           // Number of training iterations
double learning_rate = 0.01; // Step size for weight updates
```

#### **Epochs** (Number of Iterations)

| Symptom | Current | Recommended | Why |
|---------|---------|-------------|-----|
| Loss still decreasing at end | 1000 | 2000-5000 | Not converged yet |
| Loss plateaus early (<200 epochs) | 1000 | 500-800 | Already converged, save time |
| Overfitting (train >> test acc) | 1000 | 500-700 | Too much training |
| Good balance | 1000 | Keep it! | ✓ |

#### **Learning Rate** (Step Size)

| Symptom | Current | Recommended | Why |
|---------|---------|-------------|-----|
| Loss oscillates/explodes | 0.01 | 0.001-0.005 | Steps too big |
| Loss decreases very slowly | 0.01 | 0.05-0.1 | Steps too small |
| Good convergence | 0.01 | Keep it! | ✓ |

**Safe starting points to try:**
- Conservative: `epochs=2000, learning_rate=0.005`
- Aggressive: `epochs=1000, learning_rate=0.05`
- Very safe: `epochs=5000, learning_rate=0.001`

---

## 🚨 When to Retrain: Specific Scenarios

### Scenario 1: Poor Performance on Draws
**Current Issue**: 0% accuracy on draws (only 7 examples in test set)

**Solutions:**
1. **Collect more draw data**: Add 50-100 draw game states
2. **Use class weighting**: Modify training to weight draws 10x
3. **Accept limitation**: Draws are rare in tic-tac-toe anyway

**When to retrain**: After collecting ≥30 more draw examples

### Scenario 2: Overfitting Detected
**Signs:**
- Training accuracy >98%, Test accuracy <90%
- MSE on training <0.01, MSE on test >0.05

**Solutions:**
```c
int epochs = 500;              // Reduce training time
double learning_rate = 0.005;  // Smaller steps
```

**When to retrain**: Immediately with new parameters

### Scenario 3: Underfitting
**Signs:**
- Both training and test accuracy <85%
- Loss hasn't plateaued after 1000 epochs

**Solutions:**
```c
int epochs = 3000;            // More training
double learning_rate = 0.02;  // Larger steps (carefully!)
```

**When to retrain**: Immediately with new parameters

### Scenario 4: New Data Available
**When**: Dataset expanded from 958 to 1500+ samples

**Action**: 
1. Re-split data (80/20 train/test)
2. Retrain from scratch
3. Compare new vs old model performance

### Scenario 5: Game Integration Issues
**Signs:**
- Model file loads correctly but plays poorly
- In-game AI makes obvious mistakes

**Check before retraining:**
1. ✅ Model file path correct?
2. ✅ Feature encoding matches training?
3. ✅ Prediction threshold correct (0.5/-0.5)?
4. ✅ Random move probability not too high?

**Only retrain if**: Model evaluation shows poor test accuracy

---

## 📊 Evaluation Checklist

Use this before and after retraining:

```
Before Retraining:
[ ] Run evaluate_models.exe and save output
[ ] Note current test accuracy: ______%
[ ] Note current MSE: ______
[ ] Identify specific problem (draws? overfitting? underfitting?)
[ ] Decide on parameter changes

During Training:
[ ] Watch MSE decrease over epochs
[ ] Look for convergence (loss plateaus)
[ ] Check for oscillation (loss going up/down)

After Retraining:
[ ] Run evaluate_models.exe again
[ ] Test accuracy improved? ______%
[ ] MSE improved? ______
[ ] Overfitting check: Train-Test gap < 10%?
[ ] Save model weights
[ ] Update model.txt and model.bin
[ ] Test in actual game
[ ] Document changes in LINEAR_REGRESSION_SUMMARY.md
```

---

## 🔬 Advanced: Experiment Tracking

### Create an Experiments Log

Create `src/training_log.txt`:

```
Date: 2025-11-04
Experiment #1: Baseline
Config: epochs=1000, lr=0.01
Train Acc: 96.5%
Test Acc: 96.35%
MSE: 0.012
Notes: Good baseline, poor on draws

Date: 2025-11-05
Experiment #2: More epochs
Config: epochs=2000, lr=0.01
Train Acc: 97.2%
Test Acc: 95.8%
MSE: 0.011
Notes: Slight overfitting, reverted to #1

... continue logging ...
```

### Automated Testing Script

Create `src/test_parameters.bat`:

```batch
@echo off
echo ========================================
echo Hyperparameter Testing Suite
echo ========================================

REM Test 1: Baseline
echo.
echo [Test 1] Baseline: epochs=1000, lr=0.01
gcc linear_regression.c -o lr_test.exe -lm -Wall
lr_test.exe > results_baseline.txt
echo Saved to results_baseline.txt

REM Test 2: More epochs
echo.
echo [Test 2] High epochs: epochs=2000, lr=0.01
REM Edit epochs in code, recompile, run
REM (requires manual code modification for now)

REM Test 3: Lower learning rate
echo.
echo [Test 3] Lower LR: epochs=1000, lr=0.005
REM Edit learning_rate in code, recompile, run

echo.
echo ========================================
echo Testing complete! Check results_*.txt
echo ========================================
pause
```

---

## 🎮 Integration Testing

After retraining, test in-game:

### Test Cases

1. **Easy Positions**: AI should win against random play
2. **Blocking**: AI should block obvious player wins
3. **Winning**: AI should take obvious winning moves
4. **Corner Strategy**: AI should prefer corners/center
5. **Draws**: Against perfect play, should draw

### Quick In-Game Test

```bash
cd TTTGUI(wtf)
./compile.bat
./ttt_gui.exe
# Press 'M' for Medium (Linear Regression)
# Play 10 games, count wins/losses/draws
```

**Expected results:**
- AI should win 60-70% against average player
- Should never lose obviously winning positions
- May make 1-2 suboptimal moves per game (that's okay!)

---

## 📝 Retraining Procedure

### Step-by-Step Guide

#### **For Linear Regression:**

```bash
# 1. Navigate to src directory
cd src

# 2. (Optional) Edit hyperparameters in linear_regression.c
# Open in editor, modify epochs/learning_rate around line 238

# 3. Compile
gcc linear_regression.c -o linear_regression.exe -lm -Wall

# 4. Train
./linear_regression.exe

# 5. Check output
# Look for: "Test accuracy: XX.XX%"
# Look for: MSE values during training

# 6. Evaluate comprehensively
gcc evaluate_models.c -o evaluate_models.exe -lm -Wall
./evaluate_models.exe

# 7. If satisfied, model is already saved!
# Files updated: 
#   - ../models/linear regression/model.txt
#   - ../models/linear regression/model.bin

# 8. Test in game
cd ../TTTGUI(wtf)
./compile.bat
./ttt_gui.exe
```

#### **For Naive Bayes:**

```bash
# 1. Navigate to src directory
cd src

# 2. Ensure train.data and test.data are present
# (They should be in current directory or adjust paths in code)

# 3. Compile
gcc naive_bayes.c -o naive_bayes.exe -Wall

# 4. Train
./naive_bayes.exe

# 5. Check output
# Look for: "Test Accuracy: X.XXXX"
# Look for: 6-fold cross-validation results

# 6. Model files saved:
#   - model.txt
#   - model.bin

# 7. Move to correct location if needed
# (Check where game loads from)
```

---

## 🔧 Troubleshooting

### "Test accuracy worse after retraining"
- ✅ Check if you used same train/test split
- ✅ Try reverting to previous hyperparameters
- ✅ Ensure data wasn't corrupted

### "Loss not decreasing"
- ✅ Check learning rate (too high = oscillation, too low = no movement)
- ✅ Verify data is loading correctly
- ✅ Try different random seed initialization

### "Model predicts everything as one class"
- ✅ Check class balance in dataset
- ✅ Try adding class weights
- ✅ Increase learning rate slightly

### "Game crashes when loading model"
- ✅ Verify model.bin file exists and is not corrupted
- ✅ Check file path in game code
- ✅ Ensure model structure matches (10 weights for LR)

---

## 📈 Success Criteria

### Your model is ready when:

✅ **Test accuracy ≥ 90%** (for Linear Regression)  
✅ **Test accuracy ≥ 85%** (for Naive Bayes)  
✅ **Training-Test gap < 10%** (no overfitting)  
✅ **Win accuracy ≥ 95%** (must predict wins well)  
✅ **Lose accuracy ≥ 95%** (must predict losses well)  
✅ **MSE < 0.02** (low prediction error)  
✅ **In-game performance feels right** (subjective but important!)  

---

## 🎯 Next Steps

After mastering evaluation:

1. **Feature Engineering**: Add "two-in-a-row" features
2. **Ensemble Methods**: Combine LR + NB predictions
3. **Deep Learning**: Try a simple neural network
4. **Reinforcement Learning**: Let AI learn by playing itself
5. **Minimax Comparison**: Benchmark against perfect play

---

## 📚 Quick Command Reference

```bash
# Train Linear Regression
cd src && gcc linear_regression.c -o lr.exe -lm && lr.exe

# Train Naive Bayes  
cd src && gcc naive_bayes.c -o nb.exe && nb.exe

# Evaluate Models
cd src && gcc evaluate_models.c -o eval.exe -lm && eval.exe

# Compile Game
cd TTTGUI(wtf) && compile.bat

# Run Game
cd TTTGUI(wtf) && ttt_gui.exe
```

---

## 💡 Pro Tips

1. **Always evaluate before and after retraining** to track progress
2. **Keep a log** of experiments and results
3. **Don't overtrain** - more epochs ≠ better model
4. **Test in-game frequently** - metrics don't tell the whole story
5. **Start with small changes** to hyperparameters (±20% at a time)
6. **Save old model files** before overwriting (e.g., `model_v1.bin`)
7. **Document what works** for future reference

---

## 🆘 Need Help?

If you're unsure about retraining:

1. Run `evaluate_models.exe` first
2. Check the recommendations it prints
3. Follow the decision tree in this guide
4. Start with conservative parameter changes

**Remember**: Your current model is already performing well (96.35%)!  
Only retrain if you have a specific reason to.

---

*Last Updated: November 4, 2025*
