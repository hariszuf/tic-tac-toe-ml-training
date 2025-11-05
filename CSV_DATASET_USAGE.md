# Using CSV Datasets with Your ML Models ✅ IMPLEMENTED!

## 📊 Dataset Formats

You have **two data formats** available:

### Format 1: Text `.data` files
```
b,o,x,o,x,b,x,o,x,win
x,b,o,o,x,o,b,x,x,win
```
- **Encoding**: `x`, `o`, `b` (blank)
- **Labels**: `win`, `lose`, `draw`
- **Size**: 766 training, 192 test
- **Location**: `dataset/processed/train.data`, `test.data`

### Format 2: CSV files (numerical)
```csv
x1,x2,x3,x4,x5,x6,x7,x8,x9,y
0,0,0,0,0,0,0,0,0,1
1,0,0,0,0,0,0,0,0,1
2,1,0,0,0,0,0,0,0,1
```
- **Encoding**: `0` = blank, `1` = X, `2` = O
- **Labels**: `1.0` = win, `-1.0` = lose, `0.0` = draw
- **Size**: 3,553 training, 888 test (much larger!)
- **Location**: `dataset/processed/train_dataset.csv`, `test_dataset.csv`

---

## 🎉 What's Been Implemented

### ✅ Option 1: CSV-Compatible Linear Regression
**File**: `src/linear_regression_csv.c`

Features:
- Reads both CSV and .data formats
- Auto-detects format by file extension
- Supports numerical encoding (0/1/2)
- Backwards compatible

### ✅ Option 2: Format Converter
**File**: `src/convert_dataset.c`

Features:
- Converts CSV ↔ .data format
- Preserves all data
- Interactive or command-line mode

### ✅ Option 3: Easy Training Script
**File**: `src/train_with_csv.bat`

Features:
- One-click training with CSV
- Choose format interactively
- Automatic compilation

---

## 🚀 How to Use

### Method 1: Train with CSV (Recommended - 3,553 samples!)

```bash
cd src
train_with_csv.bat
# Choose [1] for CSV format
```

**Manual:**
```bash
cd src
gcc linear_regression_csv.c -o linear_regression_csv.exe -lm -Wall
linear_regression_csv.exe ../dataset/processed/train_dataset.csv ../dataset/processed/test_dataset.csv
```

### Method 2: Convert CSV to .data format

```bash
cd src
gcc convert_dataset.c -o convert.exe -Wall
convert.exe csv2data ../dataset/processed/train_dataset.csv train_from_csv.data
convert.exe csv2data ../dataset/processed/test_dataset.csv test_from_csv.data
```

Then use with original training programs:
```bash
gcc linear_regression.c -o lr.exe -lm -Wall
lr.exe
```

### Method 3: Convert .data to CSV

```bash
cd src
convert.exe data2csv ../dataset/processed/train.data train_converted.csv
```

---

## 📈 Benefits of Using CSV

### Larger Dataset:
- **CSV**: 3,553 training + 888 test = **4,441 total**
- **Text**: 766 training + 192 test = **958 total**
- **4.6x more data!** → Better model performance

### Standard Format:
- Easy to edit in Excel/Google Sheets
- Compatible with pandas, NumPy
- Easy to generate more data

### Future-Proof:
- Add new samples easily
- Merge datasets
- Export from simulations

---

## 🔍 Data Encoding Details

### Board Positions (x1-x9):

| Value | Meaning | Linear Regression Encoding |
|-------|---------|----------------------------|
| `0` | Blank/Empty | `0.0` |
| `1` | X player | `+1.0` |
| `2` | O player | `-1.0` |

### Labels (y column):

| Value | Meaning | Description |
|-------|---------|-------------|
| `1.0` | Win | X (player) wins |
| `-1.0` | Lose | O (AI) wins |
| `0.0` | Draw | Tie game |

**Note**: Your CSV files contain mostly `1` labels (wins), which is expected for training data focused on winning positions.

---

## 📊 Expected Performance

### With Larger CSV Dataset:

Training with 3,553 samples (vs 766):
- ✅ Better generalization
- ✅ More diverse board states
- ✅ Improved accuracy (potentially 97-98%)
- ✅ Better handling of edge cases

### Comparison Test:

```bash
# Test 1: Original .data files (766 samples)
linear_regression.exe

# Test 2: CSV files (3,553 samples)  
linear_regression_csv.exe ../dataset/processed/train_dataset.csv ../dataset/processed/test_dataset.csv

# Compare the results!
```

---

## 🛠️ Tools Created

### 1. `linear_regression_csv.c`
Auto-detects CSV or text format:
```c
// Usage with CSV
linear_regression_csv.exe train_dataset.csv test_dataset.csv

// Usage with .data
linear_regression_csv.exe train.data test.data

// Uses default .data files if no arguments
linear_regression_csv.exe
```

### 2. `convert_dataset.c`
Bidirectional converter:
```bash
# CSV to .data
convert.exe csv2data input.csv output.data

# .data to CSV
convert.exe data2csv input.data output.csv

# Interactive mode (no arguments)
convert.exe
```

### 3. `train_with_csv.bat`
Interactive training:
```
Which dataset format do you want to use?
[1] CSV format (3,553 samples)
[2] Text format (766 samples)
```

---

## 🎯 Recommendations

### For Best Results:
1. ✅ **Use CSV files** - 4.6x more data
2. ✅ **Train with `linear_regression_csv.c`**
3. ✅ **Compare old vs new accuracy**
4. ✅ **Use larger dataset going forward**

### For Naive Bayes:
The converter can also prepare CSV data for Naive Bayes:
```bash
# Convert CSV to text format for Naive Bayes
convert.exe csv2data train_dataset.csv train_nb.data
convert.exe csv2data test_dataset.csv test_nb.data

# Then train Naive Bayes
naive_bayes.exe
```

---

## � Quick Start Commands

### Train with CSV (one command):
```bash
cd src && gcc linear_regression_csv.c -o lr_csv.exe -lm -Wall && lr_csv.exe ../dataset/processed/train_dataset.csv ../dataset/processed/test_dataset.csv
```

### Convert all datasets:
```bash
cd src
gcc convert_dataset.c -o convert.exe -Wall
convert.exe csv2data ../dataset/processed/train_dataset.csv ../dataset/processed/train_from_csv.data
convert.exe csv2data ../dataset/processed/test_dataset.csv ../dataset/processed/test_from_csv.data
```

---

## � File Locations

```
dataset/processed/
├── train.data              (766 samples, text format)
├── test.data               (192 samples, text format)
├── train_dataset.csv       (3,553 samples, CSV) ⭐
└── test_dataset.csv        (888 samples, CSV) ⭐

src/
├── linear_regression_csv.c (NEW - CSV compatible)
├── convert_dataset.c       (NEW - format converter)
├── train_with_csv.bat      (NEW - easy launcher)
├── linear_regression.c     (Original - text only)
└── naive_bayes.c           (Original - text only)
```

---

## 🎓 Next Steps

1. **Try CSV training** to see improved results
2. **Compare accuracies** between datasets
3. **Generate more CSV data** if needed
4. **Update Naive Bayes** to support CSV (optional)

---

*All tools are ready to use! Start with `train_with_csv.bat` for the easiest experience.*
