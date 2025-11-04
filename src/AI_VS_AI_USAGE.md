# AI vs AI Testing - Usage Guide

## 🚀 Quick Start

### Option 1: Use the batch file (Easy)
```bash
cd src
run_ai_vs_ai.bat
```

### Option 2: Manual compilation
```bash
cd src
gcc ai_vs_ai_test.c -o ai_vs_ai.exe -lm -Wall
ai_vs_ai.exe
```

---

## 🎮 How to Use

When you run the program, you'll be prompted:

```
Available AIs:
[0] Random (with randomness)
[1] Smart Random (with randomness)
[2] Linear Regression (with randomness)
[3] Minimax (deterministic)

Select AI #1 (0-3): 2
Select AI #2 (0-3): 1
Number of games to play: 100
Visualize games? (1=yes, 0=no): 0
```

### AI Descriptions

| AI | Description | Strength | Randomness |
|----|-------------|----------|------------|
| **Random** | Picks random valid moves | Weakest | Yes |
| **Smart Random** | Blocks/wins when obvious, otherwise random | Weak-Medium | Yes |
| **Linear Regression** | Uses trained ML model (96% accuracy) | Strong | Yes (10%) |
| **Minimax** | Game tree search algorithm | Very Strong | No |

---

## 📊 Example Outputs

### Quick Test (100 games, no visualization)
```
Select AI #1: 2  (Linear Regression)
Select AI #2: 1  (Smart Random)
Number of games: 100
Visualize: 0

========================================
📊 RESULTS (100 games)
========================================

Linear Regression (X):    72 wins (72.0%)
Smart Random      (O):    18 wins (18.0%)
Draws                     10      (10.0%)

Average moves per game: 6.8
Fastest game:           5 moves
Longest game:           9 moves
Total time:             0.15 seconds
Games per second:       666.7

========================================
📈 ANALYSIS
========================================

✅ Linear Regression is significantly stronger!
⚡ X (first player) has an advantage in this matchup.
```

### Visualized Game (1 game)
```
Select AI #1: 2
Select AI #2: 3
Number of games: 1
Visualize: 1

========================================
Game: Linear Regression (X) vs Minimax (O)
========================================

   0 | 1 | 2
  ---+---+---
   3 | 4 | 5
  ---+---+---
   6 | 7 | 8

Move 1: Linear Regression (X) plays position 4

   0 | 1 | 2
  ---+---+---
   3 | X | 5
  ---+---+---
   6 | 7 | 8

Move 2: Minimax (O) plays position 0

   O | 1 | 2
  ---+---+---
   3 | X | 5
  ---+---+---
   6 | 7 | 8

... (game continues) ...

🤝 Game ended in a DRAW!
```

---

## 🔬 Recommended Tests

### 1. Compare ML Models (if you have Naive Bayes)
```
AI #1: 2 (Linear Regression)
AI #2: 0 (Random - as proxy for Naive Bayes)
Games: 100
Visualize: 0

Expected: LR should win 70-80% against Random
```

### 2. ML vs Traditional AI
```
AI #1: 2 (Linear Regression)
AI #2: 3 (Minimax)
Games: 100
Visualize: 0

Expected: Minimax should win 60-70%
```

### 3. Consistency Test
```
AI #1: 2 (Linear Regression)
AI #2: 2 (Linear Regression)
Games: 100
Visualize: 0

Expected: ~50/50 split (with variance due to randomness)
```

### 4. Watch a Single Game
```
AI #1: 2 (Linear Regression)
AI #2: 3 (Minimax)
Games: 1
Visualize: 1

Purpose: See move-by-move how AIs play
```

### 5. Large-Scale Benchmark
```
AI #1: 2 (Linear Regression)
AI #2: 1 (Smart Random)
Games: 1000
Visualize: 0

Purpose: Get statistically significant results
```

---

## 💡 Tips

### For Testing AI Strength:
- ✅ Run **100+ games** for reliable statistics
- ✅ Test against **multiple opponents**
- ✅ Compare **win rates** between matchups
- ✅ **Disable visualization** for faster testing

### For Debugging:
- ✅ **Visualize 1-5 games** to watch AI decisions
- ✅ Test against **Random AI** to verify basic competence
- ✅ Test against **Minimax** to check for bugs (should never beat perfect play)

### For Learning:
- ✅ **Visualize games** to understand AI strategies
- ✅ Watch **Linear Regression vs Minimax** to see ML vs traditional
- ✅ Compare **move counts** between matchups

---

## 🎯 Expected Results

### Linear Regression Performance:

| Opponent | Expected Win Rate | Draw Rate |
|----------|------------------|-----------|
| Random | 85-95% | 5-10% |
| Smart Random | 70-80% | 10-15% |
| Linear Regression | 45-55% | 10-20% |
| Minimax | 10-30% | 30-50% |

**Note:** If Linear Regression consistently beats Minimax (>50%), there may be a bug!

### First Player Advantage:

In tic-tac-toe, X (first player) has a theoretical advantage:
- Against perfect play: X should always draw or win
- Against imperfect play: X wins more often

**Expected X advantage: 5-15% higher win rate**

---

## 📈 Performance

The program is fast:
- **500-1000 games/second** for Random/Smart Random
- **100-500 games/second** for Linear Regression
- **10-50 games/second** for Minimax (depends on depth)

### Benchmarks on typical PC:
```
100 games:    < 1 second
1,000 games:  1-3 seconds
10,000 games: 10-30 seconds
```

---

## 🔧 Customization

### Adjust Randomness Level

Edit `ai_vs_ai_test.c`, line ~165:
```c
// Change from 10% to your desired percentage
if (g_model_loaded && rand() % 100 < 10) {  // <- Change this number
```

**Example:**
- `< 0`: No randomness (deterministic)
- `< 5`: 5% randomness (less variety)
- `< 20`: 20% randomness (more variety)
- `< 50`: 50% randomness (very chaotic)

### Add More AI Types

Add to the `ais[]` array in `main()`:
```c
AIPlayer ais[] = {
    {"Random", random_move, 1},
    {"Smart Random", smart_random_move, 1},
    {"Linear Regression", linear_regression_move, 1},
    {"Minimax", minimax_move, 0},
    {"Your New AI", your_ai_function, 1},  // Add here
};
int num_ais = 5;  // Update count
```

---

## 🐛 Troubleshooting

### Model Not Found
```
Warning: Could not load Linear Regression model from ../models/linear regression/model.bin
Linear Regression AI will use random moves.
```

**Solution:**
1. Make sure you've trained the model first:
   ```bash
   cd src
   gcc linear_regression.c -o linear_regression.exe -lm -Wall
   linear_regression.exe
   ```
2. Or adjust the path in the code (line ~249)

### Compilation Errors
```
gcc: command not found
```

**Solution:** Make sure GCC is installed and in your PATH.

### Invalid Input
```
Invalid AI selection!
```

**Solution:** Enter a number between 0-3 when prompted.

---

## 📊 Tournament Mode (Advanced)

### Run All Matchups

Create `tournament.bat`:
```batch
@echo off
echo Running tournament...

REM All vs All
for %%i in (0 1 2 3) do (
    for %%j in (0 1 2 3) do (
        echo %%i %%j 100 0 | ai_vs_ai.exe >> tournament_results.txt
    )
)

echo Tournament complete! See tournament_results.txt
```

Then run:
```bash
cd src
tournament.bat
```

This tests every AI against every other AI (16 matchups x 100 games = 1,600 games).

---

## 🎓 Educational Uses

### Learn AI Strategies:
```bash
# Watch how Linear Regression plays
AI #1: 2, AI #2: 1, Games: 5, Visualize: 1
```

### Compare Algorithms:
```bash
# ML vs Rule-based
AI #1: 2 (ML), AI #2: 3 (Rule-based), Games: 100
```

### Test Improvements:
```bash
# Before and after retraining
1. Run test, note win rate
2. Retrain model with new parameters
3. Run test again, compare results
```

---

## 📝 Next Steps

1. ✅ **Test your Linear Regression model**
2. ✅ **Compare against different AIs**
3. ✅ **Document win rates** for future reference
4. ✅ **Integrate Naive Bayes** if you have it
5. ✅ **Add tournament mode** for comprehensive testing

---

*Have fun testing your AIs! 🎮*
