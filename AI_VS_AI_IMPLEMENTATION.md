# AI vs AI Implementation Guide

## Overview
This guide shows how to add an "AI vs AI" mode to your tic-tac-toe game where two AIs play against each other.

---

## Current Game Modes

Based on your documentation:
- ✅ **Player vs AI** (implemented)
- ❌ **AI vs AI** (not mentioned)

---

## Implementation Options

### Option 1: Simple Loop (Batch Testing)

Create a standalone program that simulates AI vs AI games:

```c
// File: src/ai_vs_ai_test.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 9

// Include or link your AI implementations
// - naive_bayes_predict()
// - linear_regression_predict()
// - minimax_move()

typedef struct {
    char board[BOARD_SIZE];
    int game_over;
    char winner; // 'X', 'O', or 'D' for draw
} GameState;

void init_board(GameState *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        game->board[i] = 'b'; // blank
    }
    game->game_over = 0;
    game->winner = ' ';
}

void print_board(char *board) {
    printf("\n");
    for (int i = 0; i < 9; i += 3) {
        printf(" %c | %c | %c\n", 
               board[i] == 'b' ? ' ' : board[i],
               board[i+1] == 'b' ? ' ' : board[i+1],
               board[i+2] == 'b' ? ' ' : board[i+2]);
        if (i < 6) printf("---+---+---\n");
    }
    printf("\n");
}

char check_winner(char *board) {
    // Check rows
    for (int i = 0; i < 9; i += 3) {
        if (board[i] != 'b' && board[i] == board[i+1] && board[i] == board[i+2])
            return board[i];
    }
    
    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] != 'b' && board[i] == board[i+3] && board[i] == board[i+6])
            return board[i];
    }
    
    // Check diagonals
    if (board[0] != 'b' && board[0] == board[4] && board[0] == board[8])
        return board[0];
    if (board[2] != 'b' && board[2] == board[4] && board[2] == board[6])
        return board[2];
    
    // Check for draw
    int filled = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'b') filled++;
    }
    if (filled == 9) return 'D'; // Draw
    
    return ' '; // Game continues
}

// Placeholder AI functions - replace with your actual implementations
int naive_bayes_move(char *board, char player) {
    // Your Naive Bayes AI logic here
    // For now, returns random valid move
    int moves[9], count = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 'b') moves[count++] = i;
    }
    return (count > 0) ? moves[rand() % count] : -1;
}

int linear_regression_move(char *board, char player) {
    // Your Linear Regression AI logic here
    // For now, returns random valid move
    int moves[9], count = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 'b') moves[count++] = i;
    }
    return (count > 0) ? moves[rand() % count] : -1;
}

int minimax_move(char *board, char player) {
    // Your Minimax AI logic here
    // For now, returns random valid move
    int moves[9], count = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 'b') moves[count++] = i;
    }
    return (count > 0) ? moves[rand() % count] : -1;
}

// AI function pointer type
typedef int (*AIFunction)(char*, char);

typedef struct {
    const char *name;
    AIFunction function;
    int has_randomness;
} AIPlayer;

void play_game(AIPlayer ai1, AIPlayer ai2, int visualize, int *stats) {
    GameState game;
    init_board(&game);
    
    char current_player = 'x'; // X always starts
    int move_count = 0;
    
    if (visualize) {
        printf("\n========================================\n");
        printf("Game: %s (X) vs %s (O)\n", ai1.name, ai2.name);
        printf("========================================\n");
    }
    
    while (!game.game_over && move_count < 100) { // Safety limit
        AIPlayer current_ai = (current_player == 'x') ? ai1 : ai2;
        
        // Get AI move
        int move = current_ai.function(game.board, current_player);
        
        if (move < 0 || move >= 9 || game.board[move] != 'b') {
            printf("ERROR: Invalid move by %s\n", current_ai.name);
            break;
        }
        
        // Make move
        game.board[move] = current_player;
        move_count++;
        
        if (visualize) {
            printf("\nMove %d: %s (%c) plays position %d\n", 
                   move_count, current_ai.name, current_player, move);
            print_board(game.board);
        }
        
        // Check for winner
        char result = check_winner(game.board);
        if (result != ' ') {
            game.game_over = 1;
            game.winner = result;
            
            if (visualize) {
                if (result == 'D') {
                    printf("Game ended in a DRAW!\n");
                } else {
                    AIPlayer winner = (result == 'x') ? ai1 : ai2;
                    printf("%s (%c) WINS!\n", winner.name, result);
                }
            }
            break;
        }
        
        // Switch player
        current_player = (current_player == 'x') ? 'o' : 'x';
    }
    
    // Update statistics
    if (game.winner == 'x') stats[0]++; // AI1 wins
    else if (game.winner == 'o') stats[1]++; // AI2 wins
    else if (game.winner == 'D') stats[2]++; // Draws
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    // Define available AIs
    AIPlayer ais[] = {
        {"Naive Bayes", naive_bayes_move, 1},
        {"Linear Regression", linear_regression_move, 1},
        {"Minimax", minimax_move, 0},
        {"Random", naive_bayes_move, 1} // Using NB placeholder for now
    };
    
    printf("========================================\n");
    printf("AI vs AI Testing Suite\n");
    printf("========================================\n\n");
    
    printf("Available AIs:\n");
    printf("[0] Naive Bayes (with randomness)\n");
    printf("[1] Linear Regression (with randomness)\n");
    printf("[2] Minimax (deterministic)\n");
    printf("[3] Random\n\n");
    
    int ai1_idx, ai2_idx, num_games, visualize;
    
    printf("Select AI #1 (0-3): ");
    scanf("%d", &ai1_idx);
    printf("Select AI #2 (0-3): ");
    scanf("%d", &ai2_idx);
    printf("Number of games to play: ");
    scanf("%d", &num_games);
    printf("Visualize games? (1=yes, 0=no): ");
    scanf("%d", &visualize);
    
    if (ai1_idx < 0 || ai1_idx > 3 || ai2_idx < 0 || ai2_idx > 3) {
        printf("Invalid AI selection!\n");
        return 1;
    }
    
    AIPlayer ai1 = ais[ai1_idx];
    AIPlayer ai2 = ais[ai2_idx];
    
    printf("\n========================================\n");
    printf("Running %d games: %s vs %s\n", num_games, ai1.name, ai2.name);
    printf("========================================\n");
    
    if (!ai1.has_randomness && !ai2.has_randomness) {
        printf("\n⚠️  WARNING: Both AIs are deterministic!\n");
        printf("All games will have the same outcome.\n\n");
    }
    
    int stats[3] = {0, 0, 0}; // wins for AI1, AI2, draws
    
    for (int i = 0; i < num_games; i++) {
        if (!visualize && (i + 1) % 100 == 0) {
            printf("Progress: %d/%d games completed...\n", i + 1, num_games);
        }
        
        play_game(ai1, ai2, visualize, stats);
    }
    
    // Print results
    printf("\n========================================\n");
    printf("RESULTS (%d games)\n", num_games);
    printf("========================================\n\n");
    
    printf("%s (X) wins:  %d (%.1f%%)\n", 
           ai1.name, stats[0], (double)stats[0]/num_games*100);
    printf("%s (O) wins:  %d (%.1f%%)\n", 
           ai2.name, stats[1], (double)stats[1]/num_games*100);
    printf("Draws:                 %d (%.1f%%)\n", 
           stats[2], (double)stats[2]/num_games*100);
    
    printf("\n========================================\n");
    printf("ANALYSIS\n");
    printf("========================================\n\n");
    
    if (stats[0] > stats[1] * 1.2) {
        printf("✅ %s is significantly stronger!\n", ai1.name);
    } else if (stats[1] > stats[0] * 1.2) {
        printf("✅ %s is significantly stronger!\n", ai2.name);
    } else {
        printf("⚖️  AIs are evenly matched.\n");
    }
    
    if (stats[2] > num_games * 0.3) {
        printf("🔄 High draw rate (>30%%) - AIs play defensively.\n");
    }
    
    if (!ai1.has_randomness && !ai2.has_randomness && num_games > 1) {
        printf("ℹ️  Deterministic AIs: All games had identical outcomes.\n");
    }
    
    return 0;
}
```

**Compile:**
```bash
cd src
gcc ai_vs_ai_test.c -o ai_vs_ai.exe -Wall
ai_vs_ai.exe
```

---

### Option 2: GUI Integration

Add "AI vs AI" button to your existing GUI:

**Pseudo-code changes for `gui_ai.c`:**

```c
// Add new game mode enum
typedef enum {
    MODE_PLAYER_VS_PLAYER,
    MODE_PLAYER_VS_AI,
    MODE_AI_VS_AI,  // NEW
    MODE_MENU
} GameMode;

// In main game loop
if (current_mode == MODE_AI_VS_AI) {
    // Automatically make moves for both players
    
    if (current_player == PLAYER_X) {
        // AI #1 move
        int move = get_ai_move(board, difficulty_x, PLAYER_X);
        make_move(board, move, PLAYER_X);
        
        // Optional: Add delay for visualization
        wait_milliseconds(500);
        
        current_player = PLAYER_O;
    } else {
        // AI #2 move
        int move = get_ai_move(board, difficulty_o, PLAYER_O);
        make_move(board, move, PLAYER_O);
        
        wait_milliseconds(500);
        
        current_player = PLAYER_X;
    }
    
    // Check for game over
    if (check_winner(board) || is_board_full(board)) {
        // Show result and reset
    }
}
```

---

## Randomness Implementation

### With Randomness (Recommended)

**Linear Regression** (`linear_regression_ai.c`):
```c
int linear_regression_get_move(char board[9]) {
    // 10% chance of random move
    if (rand() % 100 < 10) {
        // Random move from valid positions
        int valid_moves[9], count = 0;
        for (int i = 0; i < 9; i++) {
            if (board[i] == EMPTY) {
                valid_moves[count++] = i;
            }
        }
        return valid_moves[rand() % count];
    }
    
    // Otherwise use model prediction
    return best_move_from_model(board);
}
```

**Benefits:**
- ✅ Each game is different
- ✅ More realistic
- ✅ Better for testing
- ✅ Won't get stuck in loops

### Without Randomness (For Benchmarking)

**Deterministic version:**
```c
int linear_regression_get_move_deterministic(char board[9]) {
    // Always use model prediction, no randomness
    return best_move_from_model(board);
}
```

**Use cases:**
- Benchmarking consistent performance
- Debugging specific game states
- Comparing AI strategies

---

## Recommended AI Matchups

### Interesting Matchups WITH Randomness:

| Matchup | Expected Outcome | Purpose |
|---------|------------------|---------|
| **Naive Bayes vs Linear Reg** | LR wins 60-70% | Compare ML models |
| **Linear Reg vs Minimax-3** | Minimax wins 70-80% | ML vs Traditional |
| **Naive Bayes vs Random** | NB wins 80%+ | Validate NB works |
| **Linear Reg vs Linear Reg** | ~50/50 + draws | Test consistency |
| **Minimax-3 vs Minimax-Full** | Full wins 60%+ | Depth comparison |

### With Deterministic Play:

| Matchup | Expected Outcome |
|---------|------------------|
| **Minimax vs Minimax** | Always draw (same algorithm) |
| **LR (no random) vs LR** | Identical game every time |
| **Minimax vs LR** | Same result every time |

---

## Example Usage

### Quick Test (100 games):
```bash
cd src
gcc ai_vs_ai_test.c -o ai_vs_ai.exe -Wall
ai_vs_ai.exe

# Prompts:
Select AI #1: 1  (Linear Regression)
Select AI #2: 0  (Naive Bayes)
Number of games: 100
Visualize: 0  (no)
```

**Expected output:**
```
========================================
RESULTS (100 games)
========================================

Linear Regression (X) wins:  62 (62.0%)
Naive Bayes (O) wins:        28 (28.0%)
Draws:                       10 (10.0%)

========================================
ANALYSIS
========================================

✅ Linear Regression is significantly stronger!
```

### Detailed View (1 game):
```bash
ai_vs_ai.exe

Select AI #1: 1
Select AI #2: 2  (Minimax)
Number of games: 1
Visualize: 1  (yes)
```

**Shows move-by-move board states**

---

## Implementation Recommendations

### For Testing/Research:
✅ **Use randomness** - More interesting results  
✅ **Run 100+ games** - Statistical significance  
✅ **Track win rates** - Measure AI strength  
✅ **Try all matchups** - Find best AI  

### For Benchmarking:
✅ **Disable randomness** - Consistent results  
✅ **Test specific positions** - Edge cases  
✅ **Measure computation time** - Performance  

### For Visualization:
✅ **Add delays** - Watch game unfold  
✅ **Show move reasoning** - Debug AI logic  
✅ **Highlight best moves** - Learn strategies  

---

## Next Steps

1. **Extract AI logic** from game files into reusable functions
2. **Implement the test program** above
3. **Run benchmarks** for each AI matchup
4. **Document results** in a new file
5. **(Optional)** Add AI vs AI button to GUI

---

## Statistics to Track

When running AI vs AI:

```c
typedef struct {
    int total_games;
    int ai1_wins;
    int ai2_wins;
    int draws;
    double avg_moves_per_game;
    int fastest_game;
    int longest_game;
    double ai1_win_rate;
    double ai2_win_rate;
    double draw_rate;
} MatchupStats;
```

---

## Useful Commands

```bash
# Run 1000 games (fast benchmark)
echo 1 0 1000 0 | ai_vs_ai.exe

# Watch 5 games in detail
echo 1 2 5 1 | ai_vs_ai.exe

# Tournament mode (all vs all)
for i in {0..3}; do
    for j in {0..3}; do
        echo $i $j 100 0 | ai_vs_ai.exe >> tournament_results.txt
    done
done
```

---

## Expected Results

### Linear Regression (96% test acc) vs Naive Bayes (88% test acc):
- LR should win **60-70%** of games
- Draws: **10-20%**
- Games per second: **100+**

### Any AI vs Minimax (perfect):
- Minimax should **win or draw** every game
- If AI beats Minimax, there's a bug!

### Same AI vs itself (with randomness):
- Should be close to **50/50**
- If skewed, indicates first-player advantage or bias

---

*This implementation allows you to test AI strength and compare different algorithms objectively!*
