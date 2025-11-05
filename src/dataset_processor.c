#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FEATURES 9
#define INITIAL_CAPACITY 512  // start size, doubles as needed

typedef struct {
    int **X;      // feature matrix [rows][9]
    int *y;       // labels (+1/-1)
    int rows;
    int capacity;
} Dataset;

int encode_symbol(char c) {
    if (c == 'X' || c == 'x') return 1;  // x-player
    if (c == 'O' || c == 'o') return 2;  // o-player
    return 0;                            // blank
}

void init_dataset(Dataset *ds) {
    ds->rows = 0;
    ds->capacity = INITIAL_CAPACITY;
    ds->X = malloc(ds->capacity * sizeof(int *));
    ds->y = malloc(ds->capacity * sizeof(int));
    if (!ds->X || !ds->y) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void expand_dataset(Dataset *ds) {
    ds->capacity *= 2;
    ds->X = realloc(ds->X, ds->capacity * sizeof(int *));
    ds->y = realloc(ds->y, ds->capacity * sizeof(int));
    if (!ds->X || !ds->y) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }
}

void free_dataset(Dataset *ds) {
    for (int i = 0; i < ds->rows; i++)
        free(ds->X[i]);
    free(ds->X);
    free(ds->y);
}

void load_dataset(const char *filename, Dataset *ds) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { fprintf(stderr, "Error: cannot open %s\n", filename); exit(1); }

    char board[16], player;
    int outcome, move;
    while (fscanf(fp, "%9[^,],%c,%d,%d\n", board, &player, &outcome, &move) == 4) {
        if (outcome == 0) continue; // skip draws

        if (ds->rows >= ds->capacity)
            expand_dataset(ds);

        ds->X[ds->rows] = malloc(FEATURES * sizeof(int));
        for (int i = 0; i < FEATURES; i++)
            ds->X[ds->rows][i] = encode_symbol(board[i]);

        ds->y[ds->rows] = (outcome > 0 ? 1 : -1);
        ds->rows++;
    }
    fclose(fp);
}

void split_dataset(Dataset *full, Dataset *train, Dataset *test) {
    int split_index = (int)(0.8 * full->rows);
    init_dataset(train);
    init_dataset(test);

    for (int i = 0; i < full->rows; i++) {
        Dataset *target = (i < split_index) ? train : test;
        if (target->rows >= target->capacity)
            expand_dataset(target);

        target->X[target->rows] = malloc(FEATURES * sizeof(int));
        memcpy(target->X[target->rows], full->X[i], FEATURES * sizeof(int));
        target->y[target->rows] = full->y[i];
        target->rows++;
    }
}

// ====== New functions below ======

// Count positives / negatives and print summary
void summarize_dataset(const char *name, Dataset *ds) {
    int pos = 0, neg = 0;
    for (int i = 0; i < ds->rows; i++) {
        if (ds->y[i] == 1) pos++;
        else if (ds->y[i] == -1) neg++;
    }
    printf("%s set: %d rows | +1 (win): %d | -1 (lose): %d\n",
           name, ds->rows, pos, neg);
}

// Export dataset to CSV file for ML training
void export_dataset_csv(const char *filename, Dataset *ds) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { fprintf(stderr, "Cannot write to %s\n", filename); return; }

    // Header (optional)
    for (int j = 1; j <= FEATURES; j++) fprintf(fp, "x%d,", j);
    fprintf(fp, "y\n");

    for (int i = 0; i < ds->rows; i++) {
        for (int j = 0; j < FEATURES; j++)
            fprintf(fp, "%d,", ds->X[i][j]);
        fprintf(fp, "%d\n", ds->y[i]);
    }
    fclose(fp);
    printf("Exported %s (%d rows)\n", filename, ds->rows);
}

int main(void) {
    Dataset full;
    init_dataset(&full);

    load_dataset("ttt_dataset.data", &full);
    printf("Loaded %d rows from file.\n", full.rows);

    Dataset train, test;
    split_dataset(&full, &train, &test);

    summarize_dataset("Training", &train);
    summarize_dataset("Testing", &test);

    export_dataset_csv("train_dataset.csv", &train);
    export_dataset_csv("test_dataset.csv", &test);

    printf("\nExample row (train[0]): ");
    for (int j = 0; j < FEATURES; j++) printf("%d ", train.X[0][j]);
    printf("| label = %d\n", train.y[0]);

    free_dataset(&full);
    free_dataset(&train);
    free_dataset(&test);
    return 0;
}
