#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_INSTANCES 5000
#define NUM_FEATURES 10  // 9 board positions + 1 bias term
#define MAX_LINE_LENGTH 256

// Instance structure
typedef struct {
    double features[NUM_FEATURES];  // features[0] = bias (1.0), features[1-9] = board state
    double label;  // 1.0 for win, -1.0 for lose, 0.0 for draw
} Instance;

// Model structure
typedef struct {
    double weights[NUM_FEATURES];
} LinearModel;

// ============================================
// DATA LOADING FUNCTIONS - MULTIPLE FORMATS
// ============================================

// Convert board state string to numerical features (.data format)
void encode_features_from_text(const char *board_str, double *features) {
    features[0] = 1.0;  // Bias term
    
    for (int i = 0; i < 9; i++) {
        char cell = board_str[i * 2];  // Skip commas
        
        if (cell == 'x') {
            features[i + 1] = 1.0;
        } else if (cell == 'o') {
            features[i + 1] = -1.0;
        } else {  // 'b' for blank
            features[i + 1] = 0.0;
        }
    }
}

// Convert label string to numerical value (.data format)
double encode_label_from_text(const char *label_str) {
    if (strcmp(label_str, "win") == 0) {
        return 1.0;
    } else if (strcmp(label_str, "lose") == 0) {
        return -1.0;
    } else {  // "draw"
        return 0.0;
    }
}

// NEW: Convert numerical encoding to features (CSV format)
// CSV encoding: 0 = blank, 1 = X, 2 = O
void encode_features_from_numbers(int positions[9], double *features) {
    features[0] = 1.0;  // Bias term
    
    for (int i = 0; i < 9; i++) {
        if (positions[i] == 1) {
            features[i + 1] = 1.0;  // X
        } else if (positions[i] == 2) {
            features[i + 1] = -1.0;  // O
        } else {  // 0 = blank
            features[i + 1] = 0.0;
        }
    }
}

// NEW: Load data from CSV file
int load_data_csv(const char *filename, Instance *data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening CSV file: %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    int line_num = 0;
    
    while (fgets(line, sizeof(line), file) && count < MAX_INSTANCES) {
        line_num++;
        line[strcspn(line, "\n\r")] = 0;  // Remove newline
        
        // Skip header line
        if (line_num == 1 && (strstr(line, "x1") || strstr(line, "X1"))) {
            continue;
        }
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Parse CSV: x1,x2,x3,x4,x5,x6,x7,x8,x9,y
        int positions[9];
        double label;
        
        int parsed = sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf",
                           &positions[0], &positions[1], &positions[2],
                           &positions[3], &positions[4], &positions[5],
                           &positions[6], &positions[7], &positions[8],
                           &label);
        
        if (parsed == 10) {
            encode_features_from_numbers(positions, data[count].features);
            data[count].label = label;
            count++;
        }
    }
    
    fclose(file);
    return count;
}

// Load data from .data file (original format)
int load_data_text(const char *filename, Instance *data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening text file: %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < MAX_INSTANCES) {
        line[strcspn(line, "\n")] = 0;  // Remove newline
        
        // Split into board state and label
        char *last_comma = strrchr(line, ',');
        if (last_comma) {
            *last_comma = '\0';
            char *label_str = last_comma + 1;
            
            // Encode features and label
            encode_features_from_text(line, data[count].features);
            data[count].label = encode_label_from_text(label_str);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

// NEW: Auto-detect format and load data
int load_data(const char *filename, Instance *data) {
    // Check file extension
    const char *ext = strrchr(filename, '.');
    
    if (ext && strcmp(ext, ".csv") == 0) {
        printf("Detected CSV format, loading...\n");
        return load_data_csv(filename, data);
    } else {
        printf("Detected text format, loading...\n");
        return load_data_text(filename, data);
    }
}

// ============================================
// TRAINING FUNCTIONS (unchanged)
// ============================================

double predict(const LinearModel *model, const double *features) {
    double result = 0.0;
    for (int i = 0; i < NUM_FEATURES; i++) {
        result += model->weights[i] * features[i];
    }
    return result;
}

void train_model(LinearModel *model, Instance *train_data, int train_size, 
                 int epochs, double learning_rate) {
    // Initialize weights to small random values
    srand(time(NULL));
    for (int i = 0; i < NUM_FEATURES; i++) {
        model->weights[i] = ((double)rand() / RAND_MAX - 0.5) * 0.1;
    }
    
    printf("Training linear regression model...\n");
    printf("Epochs: %d, Learning rate: %.4f\n\n", epochs, learning_rate);
    
    for (int epoch = 0; epoch < epochs; epoch++) {
        double total_loss = 0.0;
        
        // Stochastic gradient descent
        for (int i = 0; i < train_size; i++) {
            // Forward pass
            double prediction = predict(model, train_data[i].features);
            double error = train_data[i].label - prediction;
            
            // Backward pass (update weights)
            for (int j = 0; j < NUM_FEATURES; j++) {
                model->weights[j] += learning_rate * error * train_data[i].features[j];
            }
            
            // Accumulate loss (MSE)
            total_loss += error * error;
        }
        
        double mse = total_loss / train_size;
        
        // Print progress every 100 epochs
        if ((epoch + 1) % 100 == 0 || epoch == 0) {
            printf("Epoch %d/%d - MSE: %.6f\n", epoch + 1, epochs, mse);
        }
    }
    
    printf("\nTraining complete!\n\n");
}

void evaluate_model(const LinearModel *model, Instance *test_data, int test_size) {
    int correct = 0;
    int win_correct = 0, win_total = 0;
    int lose_correct = 0, lose_total = 0;
    int draw_correct = 0, draw_total = 0;
    
    for (int i = 0; i < test_size; i++) {
        double prediction = predict(model, test_data[i].features);
        
        // Convert continuous prediction to class
        char predicted_class;
        if (prediction > 0.5) {
            predicted_class = 'w';  // win
        } else if (prediction < -0.5) {
            predicted_class = 'l';  // lose
        } else {
            predicted_class = 'd';  // draw
        }
        
        // Convert actual label to class
        char actual_class;
        if (test_data[i].label > 0.5) {
            actual_class = 'w';
            win_total++;
        } else if (test_data[i].label < -0.5) {
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
    }
    
    double accuracy = (double)correct / test_size * 100.0;
    
    printf("Test Set Evaluation:\n");
    printf("  Total accuracy: %.2f%% (%d/%d)\n", accuracy, correct, test_size);
    printf("  Win predictions: %.2f%% (%d/%d)\n", 
           win_total > 0 ? (double)win_correct / win_total * 100.0 : 0.0, 
           win_correct, win_total);
    printf("  Lose predictions: %.2f%% (%d/%d)\n", 
           lose_total > 0 ? (double)lose_correct / lose_total * 100.0 : 0.0, 
           lose_correct, lose_total);
    printf("  Draw predictions: %.2f%% (%d/%d)\n", 
           draw_total > 0 ? (double)draw_correct / draw_total * 100.0 : 0.0, 
           draw_correct, draw_total);
}

void save_model_text(const char *filename, const LinearModel *model) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not create file %s\n", filename);
        return;
    }
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "LINEAR REGRESSION MODEL\n");
    fprintf(fp, "========================================\n\n");
    
    fprintf(fp, "WEIGHTS (for Tic-Tac-Toe)\n");
    fprintf(fp, "----------------------------------------\n");
    fprintf(fp, "Feature encoding: x=1.0, o=-1.0, blank=0.0\n");
    fprintf(fp, "Output: >0.5=win, <-0.5=lose, else=draw\n\n");
    
    fprintf(fp, "Weight[0] (bias): %.10f\n", model->weights[0]);
    
    const char *positions[] = {
        "top-left", "top-middle", "top-right",
        "middle-left", "center", "middle-right",
        "bottom-left", "bottom-middle", "bottom-right"
    };
    
    for (int i = 0; i < 9; i++) {
        fprintf(fp, "Weight[%d] (%s): %.10f\n", 
                i + 1, positions[i], model->weights[i + 1]);
    }
    
    fprintf(fp, "\n========================================\n");
    fprintf(fp, "END OF MODEL\n");
    fprintf(fp, "========================================\n");
    
    fclose(fp);
    printf("Model saved to %s\n", filename);
}

void save_model_binary(const char *filename, const LinearModel *model) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Could not create file %s\n", filename);
        return;
    }
    
    fwrite(model, sizeof(LinearModel), 1, fp);
    fclose(fp);
    
    printf("Model saved to %s (binary format)\n", filename);
}

// ============================================
// MAIN PROGRAM
// ============================================

int main(int argc, char *argv[]) {
    Instance train_data[MAX_INSTANCES];
    Instance test_data[MAX_INSTANCES];
    
    // Allow custom file paths from command line
    const char *train_file = (argc > 1) ? argv[1] : "../dataset/processed/train_dataset.csv";
    const char *test_file = (argc > 2) ? argv[2] : "../dataset/processed/test_dataset.csv";
    
    // Load datasets (auto-detects CSV or text format)
    printf("========================================\n");
    printf("LINEAR REGRESSION - CSV/TEXT COMPATIBLE\n");
    printf("========================================\n\n");
    
    printf("Loading training data from: %s\n", train_file);
    int train_size = load_data(train_file, train_data);
    
    if (train_size == 0) {
        printf("No training data loaded.\n");
        return 1;
    }
    printf("✓ Loaded %d training instances\n\n", train_size);
    
    printf("Loading test data from: %s\n", test_file);
    int test_size = load_data(test_file, test_data);
    
    if (test_size == 0) {
        printf("No test data loaded.\n");
        return 1;
    }
    printf("✓ Loaded %d test instances\n\n", test_size);
    
    // Train model
    LinearModel model;
    int epochs = 1000;
    double learning_rate = 0.01;
    
    train_model(&model, train_data, train_size, epochs, learning_rate);
    
    // Print learned weights
    printf("Learned weights:\n");
    printf("  Bias: %.4f\n", model.weights[0]);
    printf("  Board weights: ");
    for (int i = 1; i < NUM_FEATURES; i++) {
        printf("%.4f ", model.weights[i]);
    }
    printf("\n\n");
    
    // Evaluate on test set
    evaluate_model(&model, test_data, test_size);
    printf("\n");
    
    // Save model
    save_model_text("../models/linear regression/model.txt", &model);
    save_model_binary("../models/linear regression/model.bin", &model);
    
    printf("\nModel training complete!\n");
    printf("You can now integrate this model into the game.\n");
    
    return 0;
}
