# Model Evaluation & Retraining - Quick Reference Card

## 🎯 Current Status

```
Linear Regression:    96.35% ✅
Naive Bayes:          ~88%    ✅
Dataset Size:         958 games (766 train / 192 test)
Class Balance:        65% Win / 33% Lose / 2% Draw
```

---

## 🚦 When to Retrain: Quick Decision Matrix

| Scenario | Action | Priority |
|----------|--------|----------|
| Test accuracy < 85% | 🔴 Retrain NOW | HIGH |
| Train-Test gap > 15% | 🟡 Check for overfitting | MEDIUM |
| New data added | 🟢 Retrain when convenient | LOW |
| Model works well | ⚪ No action needed | NONE |

---

## ⚡ Quick Evaluation

```bash
# Navigate to src folder
cd src

# Run this ONE command
evaluate.bat
```

**What you'll see:**
- Overall accuracy %
- Win/Lose/Draw performance
- Overfitting detection
- Specific recommendations

---

## 🔧 Hyperparameter Quick Reference

### Current Settings (Working Well!)
```c
epochs = 1000
learning_rate = 0.01
```

### Common Adjustments

| Problem | Change | New Value |
|---------|--------|-----------|
| 🐌 Slow convergence | ↑ Learning rate | 0.02 - 0.05 |
| 💥 Loss oscillates | ↓ Learning rate | 0.005 - 0.001 |
| 📈 Not converged | ↑ Epochs | 2000 - 5000 |
| 🎯 Overfitting | ↓ Epochs | 500 - 800 |

---

## 📋 Retraining Checklist

### Before Retraining
- [ ] Run `evaluate.bat` to see current performance
- [ ] Identify specific problem (if any)
- [ ] Note current test accuracy: ______%
- [ ] Decide on parameter changes

### During Retraining
- [ ] Watch MSE decrease each 100 epochs
- [ ] Confirm loss is converging (not oscillating)
- [ ] Wait for "Training complete!" message

### After Retraining
- [ ] Run `evaluate.bat` again
- [ ] Compare new vs old accuracy
- [ ] Test in game (press 'M' for Medium difficulty)
- [ ] Save results in training log

---

## 🎮 In-Game Testing Guide

```bash
cd TTTGUI(wtf)
compile.bat
ttt_gui.exe
# Press 'M' for Medium (Linear Regression AI)
```

### What to Test
1. ✅ AI blocks obvious winning moves
2. ✅ AI takes winning opportunities  
3. ✅ AI makes reasonable moves
4. ✅ AI wins 60-70% of games
5. ⚠️ AI may make 1-2 suboptimal moves (acceptable!)

---

## 📊 Key Metrics Explained

| Metric | Good Value | What It Means |
|--------|------------|---------------|
| **Accuracy** | >90% | Overall correct predictions |
| **MSE** | <0.02 | Prediction error (lower = better) |
| **Train-Test Gap** | <10% | Difference between train/test accuracy |
| **Win Accuracy** | >95% | Correctly predicts wins |
| **Lose Accuracy** | >95% | Correctly predicts losses |
| **Draw Accuracy** | <50% | Expected (only 2% of data!) |

---

## 🔄 Full Retraining Procedure

### Linear Regression (3 minutes)

```bash
cd src

# 1. (Optional) Edit linear_regression.c to change parameters

# 2. Compile
gcc linear_regression.c -o linear_regression.exe -lm -Wall

# 3. Train
linear_regression.exe

# 4. Evaluate
evaluate.bat

# 5. Test in game
cd ..\TTTGUI(wtf)
compile.bat
ttt_gui.exe
```

### Naive Bayes (2 minutes)

```bash
cd src

# 1. Compile
gcc naive_bayes.c -o naive_bayes.exe -Wall

# 2. Train (includes automatic evaluation)
naive_bayes.exe

# 3. Test in game
cd ..\TTTGUI(wtf)
compile.bat
ttt_gui.exe
# Press 'E' for Easy (Naive Bayes AI)
```

---

## 🆘 Common Issues & Fixes

### Issue: "Test accuracy dropped after retraining"
```
✅ Solution: Revert to previous model file
   Copy model_backup.bin to model.bin
   OR: Retrain with lower learning rate
```

### Issue: "Loss is not decreasing"
```
✅ Solution: Check learning rate
   - Too high (0.1+): Loss oscillates → Lower it
   - Too low (0.0001): Loss flat → Raise it
   - Just right (0.01): Loss decreases smoothly
```

### Issue: "Model predicts everything as 'win'"
```
✅ Solution: Dataset imbalance
   - Check class distribution (should be 65/33/2)
   - Consider class weighting
   - Collect more diverse examples
```

### Issue: "Game crashes when loading model"
```
✅ Solution: File path or corruption
   - Verify: ls ../models/linear\ regression/model.bin
   - Check file size (should be ~80 bytes)
   - Retrain to regenerate file
```

---

## 💡 Performance Targets

### Minimum Acceptable
- Linear Regression: **90% accuracy**
- Naive Bayes: **85% accuracy**
- MSE: **< 0.05**

### Current Performance (GOOD!)
- Linear Regression: **96.35% accuracy** ✅
- Naive Bayes: **~88% accuracy** ✅
- MSE: **0.012** ✅

### Theoretical Maximum
- Perfect play: **100% accuracy**
- But 96% is excellent for a simple linear model!

---

## 📈 Improvement Roadmap

### Level 1: Parameter Tuning (Easy)
- Adjust epochs and learning rate
- Target: 97-98% accuracy

### Level 2: Data Enhancement (Medium)
- Collect more draw examples
- Target: Better draw predictions

### Level 3: Feature Engineering (Advanced)
- Add "two-in-a-row" features
- Add "center control" feature
- Target: 98-99% accuracy

### Level 4: New Algorithms (Expert)
- Neural networks
- Ensemble methods
- Target: 99%+ accuracy

---

## 🎯 Decision Tree: Should I Retrain?

```
                    START
                      ↓
            Run evaluate.bat
                      ↓
        ┌─────────────┴─────────────┐
        ↓                           ↓
   Accuracy < 90%?              Accuracy ≥ 90%?
        ↓                           ↓
      YES                          NO
        ↓                           ↓
  🔴 RETRAIN NOW!           Train-Test gap > 15%?
                                    ↓
                          ┌─────────┴─────────┐
                          ↓                   ↓
                        YES                  NO
                          ↓                   ↓
                   🟡 Overfitting!      ✅ Model is fine!
                   Reduce epochs        No retraining needed
```

---

## 📝 Training Log Template

Copy this into `src/training_log.txt`:

```
========================================
TRAINING LOG
========================================

[Experiment #1 - Baseline]
Date: 2025-11-04
Parameters: epochs=1000, lr=0.01
Results:
  - Train Accuracy: 96.5%
  - Test Accuracy: 96.35%
  - MSE: 0.012
  - Train-Test Gap: 0.15%
Notes: Baseline model, working well
Action: Keep as primary model

[Experiment #2 - Higher Learning Rate]
Date: 2025-11-05
Parameters: epochs=1000, lr=0.05
Results:
  - Train Accuracy: 95.2%
  - Test Accuracy: 94.8%
  - MSE: 0.018
  - Train-Test Gap: 0.4%
Notes: Loss oscillated, lower accuracy
Action: Reverted to Experiment #1

[Experiment #3 - More Epochs]
Date: 2025-11-06
Parameters: epochs=2000, lr=0.01
Results:
  - Train Accuracy: 97.8%
  - Test Accuracy: 96.1%
  - MSE: 0.010
  - Train-Test Gap: 1.7%
Notes: Slight overfitting, no real improvement
Action: Kept Experiment #1

... continue logging ...
```

---

## ⏱️ Time Estimates

| Task | Duration | Frequency |
|------|----------|-----------|
| Quick evaluation | 30 seconds | Before each change |
| Full retraining (LR) | 2-3 minutes | When needed |
| Full retraining (NB) | 1-2 minutes | When needed |
| Hyperparameter testing | 15-30 minutes | Monthly |
| In-game testing | 10 minutes | After retraining |

---

## 🏆 Success Indicators

Your model is performing well if:

✅ Test accuracy > 90%  
✅ Players report AI is challenging but beatable  
✅ No obvious mistakes in game  
✅ MSE < 0.02  
✅ Win/Lose predictions > 95%  
✅ Training converges smoothly  

---

## 📞 Quick Help

| Question | Answer |
|----------|--------|
| How often should I retrain? | Only when performance drops or new data added |
| What's the best learning rate? | Start with 0.01, adjust if needed |
| Can I get 100% accuracy? | No - draws are rare and hard to predict |
| Is 96% accuracy good? | Yes! That's excellent for linear regression |
| Should I use more features? | Only if current performance isn't sufficient |

---

**Remember**: Your model is already performing excellently at 96.35%!  
Only retrain if you have a specific reason to improve it.

---

*Generated: November 4, 2025*  
*Model Version: Linear Regression v1.0 / Naive Bayes v1.0*
