#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INSTANCES 1000
#define NUM_FEATURES 10
#define MAX_LINE_LENGTH 256

// Instance structure
typedef struct {
    double features[NUM_FEATURES];
    double label;
} Instance;

// Model structure
typedef struct {
    double weights[NUM_FEATURES];
} LinearModel;

// Convert board state to features (same as training)
void encode_features(const char *board_str, double *features) {
    features[0] = 1.0;  // Bias term
    
    for (int i = 0; i < 9; i++) {
        char cell = board_str[i * 2];
        if (cell == 'x') {
            features[i + 1] = 1.0;
        } else if (cell == 'o') {
            features[i + 1] = -1.0;
        } else {
            features[i + 1] = 0.0;
        }
    }
}

// Convert label to numerical value
double encode_label(const char *label_str) {
    if (strcmp(label_str, "win") == 0) return 1.0;
    else if (strcmp(label_str, "lose") == 0) return -1.0;
    else return 0.0;
}

// Load data
int load_data(const char *filename, Instance *data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < MAX_INSTANCES) {
        line[strcspn(line, "\n")] = 0;
        char *last_comma = strrchr(line, ',');
        if (last_comma) {
            *last_comma = '\0';
            char *label_str = last_comma + 1;
            encode_features(line, data[count].features);
            data[count].label = encode_label(label_str);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

// Load model
int load_model(const char *filename, LinearModel *model) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Could not open model file %s\n", filename);
        return 0;
    }
    fread(model, sizeof(LinearModel), 1, fp);
    fclose(fp);
    return 1;
}

// Predict
double predict(const LinearModel *model, const double *features) {
    double result = 0.0;
    for (int i = 0; i < NUM_FEATURES; i++) {
        result += model->weights[i] * features[i];
    }
    return result;
}

// Comprehensive evaluation
void evaluate_comprehensive(const LinearModel *model, Instance *data, int data_size, const char *dataset_name) {
    int correct = 0;
    int win_correct = 0, win_total = 0, win_tp = 0, win_fp = 0, win_fn = 0;
    int lose_correct = 0, lose_total = 0, lose_tp = 0, lose_fp = 0, lose_fn = 0;
    int draw_correct = 0, draw_total = 0, draw_tp = 0, draw_fp = 0, draw_fn = 0;
    
    double total_mse = 0.0;
    double total_mae = 0.0;
    
    printf("\n========================================\n");
    printf("EVALUATION: %s\n", dataset_name);
    printf("========================================\n");
    
    for (int i = 0; i < data_size; i++) {
        double prediction = predict(model, data[i].features);
        
        // Calculate regression metrics
        double error = data[i].label - prediction;
        total_mse += error * error;
        total_mae += fabs(error);
        
        // Convert to class predictions
        char predicted_class, actual_class;
        
        if (prediction > 0.5) predicted_class = 'w';
        else if (prediction < -0.5) predicted_class = 'l';
        else predicted_class = 'd';
        
        if (data[i].label > 0.5) {
            actual_class = 'w';
            win_total++;
        } else if (data[i].label < -0.5) {
            actual_class = 'l';
            lose_total++;
        } else {
            actual_class = 'd';
            draw_total++;
        }
        
        // Count correct predictions
        if (predicted_class == actual_class) {
            correct++;
            if (actual_class == 'w') win_correct++;
            else if (actual_class == 'l') lose_correct++;
            else draw_correct++;
        }
        
        // Calculate precision/recall metrics
        if (actual_class == 'w') {
            if (predicted_class == 'w') win_tp++;
            else win_fn++;
        } else {
            if (predicted_class == 'w') win_fp++;
        }
        
        if (actual_class == 'l') {
            if (predicted_class == 'l') lose_tp++;
            else lose_fn++;
        } else {
            if (predicted_class == 'l') lose_fp++;
        }
        
        if (actual_class == 'd') {
            if (predicted_class == 'd') draw_tp++;
            else draw_fn++;
        } else {
            if (predicted_class == 'd') draw_fp++;
        }
    }
    
    double accuracy = (double)correct / data_size * 100.0;
    double mse = total_mse / data_size;
    double mae = total_mae / data_size;
    double rmse = sqrt(mse);
    
    printf("\n--- Overall Metrics ---\n");
    printf("Total instances:    %d\n", data_size);
    printf("Accuracy:           %.2f%% (%d/%d)\n", accuracy, correct, data_size);
    printf("Mean Squared Error: %.6f\n", mse);
    printf("Root MSE:           %.6f\n", rmse);
    printf("Mean Absolute Err:  %.6f\n", mae);
    
    printf("\n--- Per-Class Performance ---\n");
    printf("\nWIN Class:\n");
    printf("  Instances:  %d (%.1f%%)\n", win_total, (double)win_total/data_size*100);
    printf("  Accuracy:   %.2f%% (%d/%d)\n", 
           win_total > 0 ? (double)win_correct/win_total*100 : 0, win_correct, win_total);
    if (win_tp + win_fp > 0) {
        double precision = (double)win_tp / (win_tp + win_fp);
        printf("  Precision:  %.2f%%\n", precision * 100);
    }
    if (win_tp + win_fn > 0) {
        double recall = (double)win_tp / (win_tp + win_fn);
        printf("  Recall:     %.2f%%\n", recall * 100);
    }
    
    printf("\nLOSE Class:\n");
    printf("  Instances:  %d (%.1f%%)\n", lose_total, (double)lose_total/data_size*100);
    printf("  Accuracy:   %.2f%% (%d/%d)\n", 
           lose_total > 0 ? (double)lose_correct/lose_total*100 : 0, lose_correct, lose_total);
    if (lose_tp + lose_fp > 0) {
        double precision = (double)lose_tp / (lose_tp + lose_fp);
        printf("  Precision:  %.2f%%\n", precision * 100);
    }
    if (lose_tp + lose_fn > 0) {
        double recall = (double)lose_tp / (lose_tp + lose_fn);
        printf("  Recall:     %.2f%%\n", recall * 100);
    }
    
    printf("\nDRAW Class:\n");
    printf("  Instances:  %d (%.1f%%)\n", draw_total, (double)draw_total/data_size*100);
    printf("  Accuracy:   %.2f%% (%d/%d)\n", 
           draw_total > 0 ? (double)draw_correct/draw_total*100 : 0, draw_correct, draw_total);
    if (draw_tp + draw_fp > 0) {
        double precision = (double)draw_tp / (draw_tp + draw_fp);
        printf("  Precision:  %.2f%%\n", precision * 100);
    }
    if (draw_tp + draw_fn > 0) {
        double recall = (double)draw_tp / (draw_tp + draw_fn);
        printf("  Recall:     %.2f%%\n", recall * 100);
    }
    
    // Recommendations
    printf("\n--- Recommendations ---\n");
    if (accuracy < 90.0) {
        printf("⚠️  Low accuracy! Consider:\n");
        printf("   - Increasing training epochs\n");
        printf("   - Trying different learning rate\n");
        printf("   - Adding more training data\n");
    } else if (accuracy >= 95.0) {
        printf("✅ Excellent accuracy!\n");
    } else {
        printf("✓  Good accuracy, but room for improvement.\n");
    }
    
    if (draw_total > 0 && draw_correct == 0) {
        printf("⚠️  Poor draw prediction! Suggestions:\n");
        printf("   - Dataset has only %.1f%% draws (imbalanced)\n", (double)draw_total/data_size*100);
        printf("   - Consider collecting more draw examples\n");
        printf("   - Or use class weighting in training\n");
    }
    
    if (mse > 0.1) {
        printf("⚠️  High MSE! Model may need more training.\n");
    }
}

int main() {
    Instance train_data[MAX_INSTANCES];
    Instance test_data[MAX_INSTANCES];
    LinearModel model;
    
    // Load model
    printf("Loading model from ../models/linear regression/model.bin...\n");
    if (!load_model("../models/linear regression/model.bin", &model)) {
        printf("Failed to load model. Make sure you've trained it first.\n");
        return 1;
    }
    printf("Model loaded successfully!\n");
    
    // Display model weights
    printf("\n========================================\n");
    printf("MODEL WEIGHTS\n");
    printf("========================================\n");
    printf("Bias:            %.6f\n", model.weights[0]);
    const char *positions[] = {
        "Top-Left", "Top-Middle", "Top-Right",
        "Middle-Left", "Center", "Middle-Right",
        "Bottom-Left", "Bottom-Middle", "Bottom-Right"
    };
    for (int i = 0; i < 9; i++) {
        printf("%-13s    %.6f\n", positions[i], model.weights[i + 1]);
    }
    
    // Load and evaluate training data
    printf("\nLoading training data...\n");
    int train_size = load_data("../dataset/processed/train.data", train_data);
    if (train_size > 0) {
        printf("Loaded %d training instances\n", train_size);
        evaluate_comprehensive(&model, train_data, train_size, "TRAINING SET");
    }
    
    // Load and evaluate test data
    printf("\nLoading test data...\n");
    int test_size = load_data("../dataset/processed/test.data", test_data);
    if (test_size > 0) {
        printf("Loaded %d test instances\n", test_size);
        evaluate_comprehensive(&model, test_data, test_size, "TEST SET");
    }
    
    // Check for overfitting
    if (train_size > 0 && test_size > 0) {
        // Simple accuracy comparison
        int train_correct = 0, test_correct = 0;
        
        for (int i = 0; i < train_size; i++) {
            double pred = predict(&model, train_data[i].features);
            char pred_class = (pred > 0.5) ? 'w' : (pred < -0.5) ? 'l' : 'd';
            char actual_class = (train_data[i].label > 0.5) ? 'w' : 
                               (train_data[i].label < -0.5) ? 'l' : 'd';
            if (pred_class == actual_class) train_correct++;
        }
        
        for (int i = 0; i < test_size; i++) {
            double pred = predict(&model, test_data[i].features);
            char pred_class = (pred > 0.5) ? 'w' : (pred < -0.5) ? 'l' : 'd';
            char actual_class = (test_data[i].label > 0.5) ? 'w' : 
                               (test_data[i].label < -0.5) ? 'l' : 'd';
            if (pred_class == actual_class) test_correct++;
        }
        
        double train_acc = (double)train_correct / train_size * 100.0;
        double test_acc = (double)test_correct / test_size * 100.0;
        double gap = train_acc - test_acc;
        
        printf("\n========================================\n");
        printf("OVERFITTING CHECK\n");
        printf("========================================\n");
        printf("Training accuracy: %.2f%%\n", train_acc);
        printf("Test accuracy:     %.2f%%\n", test_acc);
        printf("Accuracy gap:      %.2f%%\n", gap);
        
        if (gap > 10.0) {
            printf("\n⚠️  WARNING: Possible overfitting detected!\n");
            printf("The model performs much better on training than test data.\n");
            printf("Consider: Reducing epochs, adding regularization, or more test data.\n");
        } else if (gap < 0) {
            printf("\n⚠️  Test accuracy > Training accuracy (unusual)\n");
            printf("This might indicate lucky test set or training issues.\n");
        } else {
            printf("\n✅ Good generalization! Model is not overfitting.\n");
        }
    }
    
    printf("\n========================================\n");
    printf("EVALUATION COMPLETE\n");
    printf("========================================\n");
    
    return 0;
}
