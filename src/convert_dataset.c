#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

// ============================================
// CSV to .data format converter
// ============================================

// CSV: 0=blank, 1=X, 2=O, label=numerical
// .data: b=blank, x=X, o=O, label=win/lose/draw

char number_to_char(int num) {
    switch(num) {
        case 0: return 'b';  // blank
        case 1: return 'x';  // X
        case 2: return 'o';  // O
        default: return 'b';
    }
}

const char* number_to_label(double label) {
    if (label > 0.5) return "win";
    if (label < -0.5) return "lose";
    return "draw";
}

int char_to_number(char c) {
    switch(c) {
        case 'b': return 0;
        case 'x': return 1;
        case 'o': return 2;
        default: return 0;
    }
}

double label_to_number(const char *label) {
    if (strcmp(label, "win") == 0) return 1.0;
    if (strcmp(label, "lose") == 0) return -1.0;
    return 0.0;
}

void csv_to_data(const char *csv_file, const char *data_file) {
    FILE *fin = fopen(csv_file, "r");
    if (!fin) {
        printf("Error: Cannot open input file %s\n", csv_file);
        return;
    }
    
    FILE *fout = fopen(data_file, "w");
    if (!fout) {
        printf("Error: Cannot create output file %s\n", data_file);
        fclose(fin);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_num = 0;
    int converted = 0;
    
    while (fgets(line, sizeof(line), fin)) {
        line_num++;
        line[strcspn(line, "\n\r")] = 0;
        
        // Skip header
        if (line_num == 1 && (strstr(line, "x1") || strstr(line, "X1"))) {
            continue;
        }
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Parse CSV
        int positions[9];
        double label;
        
        int parsed = sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf",
                           &positions[0], &positions[1], &positions[2],
                           &positions[3], &positions[4], &positions[5],
                           &positions[6], &positions[7], &positions[8],
                           &label);
        
        if (parsed == 10) {
            // Convert to .data format
            for (int i = 0; i < 9; i++) {
                fprintf(fout, "%c", number_to_char(positions[i]));
                if (i < 8) fprintf(fout, ",");
            }
            fprintf(fout, ",%s\n", number_to_label(label));
            converted++;
        }
    }
    
    fclose(fin);
    fclose(fout);
    
    printf("✓ Converted %d instances from %s to %s\n", converted, csv_file, data_file);
}

void data_to_csv(const char *data_file, const char *csv_file) {
    FILE *fin = fopen(data_file, "r");
    if (!fin) {
        printf("Error: Cannot open input file %s\n", data_file);
        return;
    }
    
    FILE *fout = fopen(csv_file, "w");
    if (!fout) {
        printf("Error: Cannot create output file %s\n", csv_file);
        fclose(fin);
        return;
    }
    
    // Write CSV header
    fprintf(fout, "x1,x2,x3,x4,x5,x6,x7,x8,x9,y\n");
    
    char line[MAX_LINE_LENGTH];
    int converted = 0;
    
    while (fgets(line, sizeof(line), fin)) {
        line[strcspn(line, "\n\r")] = 0;
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Find last comma
        char *last_comma = strrchr(line, ',');
        if (!last_comma) continue;
        
        *last_comma = '\0';
        char *label_str = last_comma + 1;
        
        // Convert board state
        char *token = strtok(line, ",");
        int pos = 0;
        
        while (token != NULL && pos < 9) {
            if (pos > 0) fprintf(fout, ",");
            fprintf(fout, "%d", char_to_number(token[0]));
            token = strtok(NULL, ",");
            pos++;
        }
        
        // Convert label
        fprintf(fout, ",%.1f\n", label_to_number(label_str));
        converted++;
    }
    
    fclose(fin);
    fclose(fout);
    
    printf("✓ Converted %d instances from %s to %s\n", converted, data_file, csv_file);
}

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("Dataset Format Converter\n");
    printf("========================================\n\n");
    
    if (argc < 4) {
        printf("Usage:\n");
        printf("  CSV to .data:  %s csv2data <input.csv> <output.data>\n", argv[0]);
        printf("  .data to CSV:  %s data2csv <input.data> <output.csv>\n\n", argv[0]);
        
        printf("Examples:\n");
        printf("  %s csv2data train_dataset.csv train.data\n", argv[0]);
        printf("  %s data2csv train.data train_dataset.csv\n\n", argv[0]);
        
        // Interactive mode
        printf("Interactive Mode:\n");
        printf("[1] Convert CSV to .data\n");
        printf("[2] Convert .data to CSV\n");
        printf("[0] Exit\n\n");
        
        int choice;
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            return 1;
        }
        
        char input[256], output[256];
        
        if (choice == 1) {
            printf("Input CSV file: ");
            scanf("%s", input);
            printf("Output .data file: ");
            scanf("%s", output);
            csv_to_data(input, output);
        } else if (choice == 2) {
            printf("Input .data file: ");
            scanf("%s", input);
            printf("Output CSV file: ");
            scanf("%s", output);
            data_to_csv(input, output);
        } else {
            printf("Goodbye!\n");
        }
        
        return 0;
    }
    
    const char *mode = argv[1];
    const char *input = argv[2];
    const char *output = argv[3];
    
    if (strcmp(mode, "csv2data") == 0) {
        csv_to_data(input, output);
    } else if (strcmp(mode, "data2csv") == 0) {
        data_to_csv(input, output);
    } else {
        printf("Error: Unknown mode '%s'\n", mode);
        printf("Use 'csv2data' or 'data2csv'\n");
        return 1;
    }
    
    printf("\n✅ Conversion complete!\n");
    return 0;
}
