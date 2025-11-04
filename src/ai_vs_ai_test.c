#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define BOARD_SIZE 9
#define NUM_FEATURES 10
#define EMPTY 'b'
#define PLAYER_X 'x'
#define PLAYER_O 'o'

// ============================================
// Data Structures
// ============================================

typedef struct {
    char board[BOARD_SIZE];
    int game_over;
    char winner; // 'x', 'o', or 'd' for draw
} GameState;

typedef struct {
    double weights[NUM_FEATURES];
} LinearModel;

typedef int (*AIFunction)(char*, char);

typedef struct {
    const char *name;
    AIFunction function;
    int has_randomness;
} AIPlayer;

typedef struct {
    int total_games;
    int ai1_wins;
    int ai2_wins;
    int draws;
    int total_moves;
    int fastest_game;
    int longest_game;
} MatchupStats;

// ============================================
// Global Model (loaded once)
// ============================================

LinearModel g_linear_model;
int g_model_loaded = 0;

// ============================================
// Board Functions
// ============================================

void init_board(GameState *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        game->board[i] = EMPTY;
    }
    game->game_over = 0;
    game->winner = ' ';
}

void print_board(char *board) {
    printf("\n");
    for (int i = 0; i < 9; i += 3) {
        printf("  ");
        for (int j = 0; j < 3; j++) {
            char cell = board[i + j];
            if (cell == EMPTY) {
                printf(" %d ", i + j);
            } else {
                printf(" %c ", cell == PLAYER_X ? 'X' : 'O');
            }
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 6) printf("  ---+---+---\n");
    }
    printf("\n");
}

char check_winner(char *board) {
    // Winning combinations
    int wins[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}              // Diagonals
    };
    
    for (int i = 0; i < 8; i++) {
        char a = board[wins[i][0]];
        char b = board[wins[i][1]];
        char c = board[wins[i][2]];
        
        if (a != EMPTY && a == b && b == c) {
            return a;
        }
    }
    
    // Check for draw
    int filled = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] != EMPTY) filled++;
    }
    if (filled == BOARD_SIZE) return 'd';
    
    return ' '; // Game continues
}

int is_valid_move(char *board, int pos) {
    return (pos >= 0 && pos < BOARD_SIZE && board[pos] == EMPTY);
}

// ============================================
// Linear Regression AI
// ============================================

int load_linear_model(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Warning: Could not load Linear Regression model from %s\n", filename);
        printf("Linear Regression AI will use random moves.\n");
        return 0;
    }
    
    fread(&g_linear_model, sizeof(LinearModel), 1, fp);
    fclose(fp);
    g_model_loaded = 1;
    return 1;
}

double predict_linear(char *board, char player) {
    if (!g_model_loaded) return 0.0;
    
    double features[NUM_FEATURES];
    features[0] = 1.0; // Bias
    
    for (int i = 0; i < 9; i++) {
        if (board[i] == PLAYER_X) {
            features[i + 1] = 1.0;
        } else if (board[i] == PLAYER_O) {
            features[i + 1] = -1.0;
        } else {
            features[i + 1] = 0.0;
        }
    }
    
    double result = 0.0;
    for (int i = 0; i < NUM_FEATURES; i++) {
        result += g_linear_model.weights[i] * features[i];
    }
    
    return result;
}

int linear_regression_move(char *board, char player) {
    // 10% chance of random move for variety
    if (g_model_loaded && rand() % 100 < 10) {
        int valid_moves[BOARD_SIZE];
        int count = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i] == EMPTY) {
                valid_moves[count++] = i;
            }
        }
        if (count > 0) return valid_moves[rand() % count];
    }
    
    if (!g_model_loaded) {
        // Fallback to random if model not loaded
        int valid_moves[BOARD_SIZE];
        int count = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i] == EMPTY) {
                valid_moves[count++] = i;
            }
        }
        return (count > 0) ? valid_moves[rand() % count] : -1;
    }
    
    // Try each empty cell and find best move
    double best_score = (player == PLAYER_O) ? 999999.0 : -999999.0;
    int best_move = -1;
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == EMPTY) {
            // Simulate move
            char temp_board[BOARD_SIZE];
            memcpy(temp_board, board, BOARD_SIZE);
            temp_board[i] = player;
            
            double score = predict_linear(temp_board, player);
            
            // O wants to minimize score (X losing), X wants to maximize
            if (player == PLAYER_O) {
                if (score < best_score) {
                    best_score = score;
                    best_move = i;
                }
            } else {
                if (score > best_score) {
                    best_score = score;
                    best_move = i;
                }
            }
        }
    }
    
    return best_move;
}

// ============================================
// Simple Minimax AI (for comparison)
// ============================================

int minimax(char *board, char player, int depth, int is_maximizing) {
    char result = check_winner(board);
    
    if (result == PLAYER_X) return 10 - depth;
    if (result == PLAYER_O) return depth - 10;
    if (result == 'd') return 0;
    
    if (depth >= 6) return 0; // Limit depth for medium difficulty
    
    if (is_maximizing) {
        int best = -999999;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i] == EMPTY) {
                board[i] = PLAYER_X;
                int score = minimax(board, PLAYER_X, depth + 1, 0);
                board[i] = EMPTY;
                if (score > best) best = score;
            }
        }
        return best;
    } else {
        int best = 999999;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i] == EMPTY) {
                board[i] = PLAYER_O;
                int score = minimax(board, PLAYER_O, depth + 1, 1);
                board[i] = EMPTY;
                if (score < best) best = score;
            }
        }
        return best;
    }
}

int minimax_move(char *board, char player) {
    int best_move = -1;
    int best_score = (player == PLAYER_X) ? -999999 : 999999;
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == EMPTY) {
            char temp_board[BOARD_SIZE];
            memcpy(temp_board, board, BOARD_SIZE);
            temp_board[i] = player;
            
            int score = minimax(temp_board, player, 0, player == PLAYER_O);
            
            if (player == PLAYER_X) {
                if (score > best_score) {
                    best_score = score;
                    best_move = i;
                }
            } else {
                if (score < best_score) {
                    best_score = score;
                    best_move = i;
                }
            }
        }
    }
    
    return best_move;
}

// ============================================
// Random AI
// ============================================

int random_move(char *board, char player) {
    int valid_moves[BOARD_SIZE];
    int count = 0;
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == EMPTY) {
            valid_moves[count++] = i;
        }
    }
    
    return (count > 0) ? valid_moves[rand() % count] : -1;
}

// ============================================
// Smart Random AI (blocks/wins when possible)
// ============================================

int smart_random_move(char *board, char player) {
    char opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    
    // First, check if we can win
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == EMPTY) {
            char temp_board[BOARD_SIZE];
            memcpy(temp_board, board, BOARD_SIZE);
            temp_board[i] = player;
            if (check_winner(temp_board) == player) {
                return i; // Winning move!
            }
        }
    }
    
    // Second, check if we need to block opponent
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == EMPTY) {
            char temp_board[BOARD_SIZE];
            memcpy(temp_board, board, BOARD_SIZE);
            temp_board[i] = opponent;
            if (check_winner(temp_board) == opponent) {
                return i; // Block opponent!
            }
        }
    }
    
    // Otherwise, random move
    return random_move(board, player);
}

// ============================================
// Game Simulation
// ============================================

void play_game(AIPlayer ai1, AIPlayer ai2, int visualize, MatchupStats *stats) {
    GameState game;
    init_board(&game);
    
    char current_player = PLAYER_X; // X always starts
    int move_count = 0;
    
    if (visualize) {
        printf("\n========================================\n");
        printf("Game: %s (X) vs %s (O)\n", ai1.name, ai2.name);
        printf("========================================\n");
        print_board(game.board);
    }
    
    while (!game.game_over && move_count < 100) { // Safety limit
        AIPlayer current_ai = (current_player == PLAYER_X) ? ai1 : ai2;
        
        // Get AI move
        int move = current_ai.function(game.board, current_player);
        
        if (move < 0 || move >= BOARD_SIZE || game.board[move] != EMPTY) {
            printf("ERROR: Invalid move %d by %s\n", move, current_ai.name);
            break;
        }
        
        // Make move
        game.board[move] = current_player;
        move_count++;
        
        if (visualize) {
            printf("Move %d: %s (%c) plays position %d\n", 
                   move_count, current_ai.name, 
                   current_player == PLAYER_X ? 'X' : 'O', move);
            print_board(game.board);
        }
        
        // Check for winner
        char result = check_winner(game.board);
        if (result != ' ') {
            game.game_over = 1;
            game.winner = result;
            
            if (visualize) {
                if (result == 'd') {
                    printf("🤝 Game ended in a DRAW!\n");
                } else {
                    AIPlayer winner = (result == PLAYER_X) ? ai1 : ai2;
                    printf("🎉 %s (%c) WINS!\n", winner.name, result == PLAYER_X ? 'X' : 'O');
                }
            }
            break;
        }
        
        // Switch player
        current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }
    
    // Update statistics
    stats->total_moves += move_count;
    if (stats->fastest_game == 0 || move_count < stats->fastest_game) {
        stats->fastest_game = move_count;
    }
    if (move_count > stats->longest_game) {
        stats->longest_game = move_count;
    }
    
    if (game.winner == PLAYER_X) stats->ai1_wins++;
    else if (game.winner == PLAYER_O) stats->ai2_wins++;
    else if (game.winner == 'd') stats->draws++;
}

// ============================================
// Main Program
// ============================================

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    // Try to load Linear Regression model
    printf("Loading Linear Regression model...\n");
    load_linear_model("../models/linear regression/model.bin");
    printf("\n");
    
    // Define available AIs
    AIPlayer ais[] = {
        {"Random", random_move, 1},
        {"Smart Random", smart_random_move, 1},
        {"Linear Regression", linear_regression_move, 1},
        {"Minimax", minimax_move, 0}
    };
    int num_ais = 4;
    
    printf("========================================\n");
    printf("🎮 AI vs AI Testing Suite\n");
    printf("========================================\n\n");
    
    printf("Available AIs:\n");
    for (int i = 0; i < num_ais; i++) {
        printf("[%d] %s %s\n", i, ais[i].name, 
               ais[i].has_randomness ? "(with randomness)" : "(deterministic)");
    }
    printf("\n");
    
    int ai1_idx, ai2_idx, num_games, visualize;
    
    // Get user input
    printf("Select AI #1 (0-%d): ", num_ais - 1);
    if (scanf("%d", &ai1_idx) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    
    printf("Select AI #2 (0-%d): ", num_ais - 1);
    if (scanf("%d", &ai2_idx) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    
    printf("Number of games to play: ");
    if (scanf("%d", &num_games) != 1 || num_games < 1) {
        printf("Invalid number of games!\n");
        return 1;
    }
    
    printf("Visualize games? (1=yes, 0=no): ");
    if (scanf("%d", &visualize) != 1) {
        visualize = 0;
    }
    
    if (ai1_idx < 0 || ai1_idx >= num_ais || ai2_idx < 0 || ai2_idx >= num_ais) {
        printf("Invalid AI selection!\n");
        return 1;
    }
    
    AIPlayer ai1 = ais[ai1_idx];
    AIPlayer ai2 = ais[ai2_idx];
    
    printf("\n========================================\n");
    printf("🏁 Running %d games: %s vs %s\n", num_games, ai1.name, ai2.name);
    printf("========================================\n");
    
    if (!ai1.has_randomness && !ai2.has_randomness) {
        printf("\n⚠️  WARNING: Both AIs are deterministic!\n");
        printf("All games will have the same outcome.\n\n");
    }
    
    // Initialize statistics
    MatchupStats stats = {0};
    stats.total_games = num_games;
    
    // Play games
    clock_t start_time = clock();
    
    for (int i = 0; i < num_games; i++) {
        if (!visualize && num_games >= 100 && (i + 1) % 100 == 0) {
            printf("Progress: %d/%d games completed...\n", i + 1, num_games);
        }
        
        play_game(ai1, ai2, visualize, &stats);
    }
    
    clock_t end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    // Print results
    printf("\n========================================\n");
    printf("📊 RESULTS (%d games)\n", num_games);
    printf("========================================\n\n");
    
    printf("%-20s (X):  %4d wins (%.1f%%)\n", 
           ai1.name, stats.ai1_wins, (double)stats.ai1_wins/num_games*100);
    printf("%-20s (O):  %4d wins (%.1f%%)\n", 
           ai2.name, stats.ai2_wins, (double)stats.ai2_wins/num_games*100);
    printf("%-20s     %4d      (%.1f%%)\n", 
           "Draws", stats.draws, (double)stats.draws/num_games*100);
    
    printf("\n");
    printf("Average moves per game: %.1f\n", (double)stats.total_moves / num_games);
    printf("Fastest game:           %d moves\n", stats.fastest_game);
    printf("Longest game:           %d moves\n", stats.longest_game);
    printf("Total time:             %.2f seconds\n", elapsed);
    printf("Games per second:       %.1f\n", num_games / elapsed);
    
    printf("\n========================================\n");
    printf("📈 ANALYSIS\n");
    printf("========================================\n\n");
    
    if (stats.ai1_wins > stats.ai2_wins * 1.2) {
        printf("✅ %s is significantly stronger!\n", ai1.name);
    } else if (stats.ai2_wins > stats.ai1_wins * 1.2) {
        printf("✅ %s is significantly stronger!\n", ai2.name);
    } else {
        printf("⚖️  AIs are evenly matched.\n");
    }
    
    if (stats.draws > num_games * 0.3) {
        printf("🔄 High draw rate (%.1f%%) - AIs play defensively.\n", 
               (double)stats.draws/num_games*100);
    }
    
    if (!ai1.has_randomness && !ai2.has_randomness && num_games > 1) {
        printf("ℹ️  Deterministic AIs: All games had identical outcomes.\n");
    }
    
    // First-player advantage analysis
    double expected_x_wins = num_games * 0.5;
    if (stats.ai1_wins > expected_x_wins * 1.1) {
        printf("⚡ X (first player) has an advantage in this matchup.\n");
    } else if (stats.ai2_wins > expected_x_wins * 1.1) {
        printf("⚡ O (second player) has an advantage in this matchup.\n");
    }
    
    printf("\n========================================\n");
    printf("✅ Testing complete!\n");
    printf("========================================\n");
    
    return 0;
}
