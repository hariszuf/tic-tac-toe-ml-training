#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define NSTATE 19683  // 3^9

static int8_t outcome_cache[NSTATE];   // -1,0,+1 outcomes; 2 = unknown
static int8_t bestmove_cache[NSTATE];  // 0..8 or -1
static int pow3[10];

static inline int cell(int s, int i){ return (s / pow3[i]) % 3; }               // 0 empty,1 X,2 O
static inline int setcell(int s, int i, int v){ return s + (v - cell(s,i)) * pow3[i]; }

static const int wins[8][3] = {
  {0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}
};

static int winner(int s){ // 1 if X wins, 2 if O wins, 0 otherwise
  for(int k=0;k<8;k++){
    int a=cell(s,wins[k][0]), b=cell(s,wins[k][1]), c=cell(s,wins[k][2]);
    if(a!=0 && a==b && b==c) return a;
  }
  return 0;
}

static bool full(int s){
  for(int i=0;i<9;i++) if(cell(s,i)==0) return false;
  return true;
}

static bool legal_and_turn(int s, int *turn){ // turn: 1=X, 2=O, 0=terminal
  int cx=0, co=0;
  for(int i=0;i<9;i++){
    int v=cell(s,i);
    if(v==1) cx++; else if(v==2) co++;
  }
  if(cx<co || cx>co+1) return false; // illegal counts
  int w = winner(s);
  if(w!=0 || full(s)){ *turn = 0; return true; } // terminal but legal
  *turn = (cx==co) ? 1 : 2;
  return true;
}

// Minimax from perspective of current player; memoized
static int minimax(int s){
  if(outcome_cache[s] != 2) return outcome_cache[s];

  int turn;
  if(!legal_and_turn(s, &turn)){
    outcome_cache[s]=0; bestmove_cache[s]=-1; return 0; // shouldn't happen if filtered
  }

  int w = winner(s);
  if(w==1){ outcome_cache[s]=+1; bestmove_cache[s]=-1; return +1; }
  if(w==2){ outcome_cache[s]=-1; bestmove_cache[s]=-1; return -1; }
  if(full(s)){ outcome_cache[s]=0; bestmove_cache[s]=-1; return 0; }

  int bestScore = (turn==1 ? -2 : +2);
  int bestMove  = -1;

  for(int i=0;i<9;i++){
    if(cell(s,i)!=0) continue;
    int child = setcell(s,i,turn);
    int sc = minimax(child);            // child's score from child's current player
    if(turn==2) sc = -sc;               // flip so it's from *this* current player's perspective

    if(sc > bestScore){
      bestScore = sc;
      bestMove  = i;
      if(bestScore==+1) break;          // can't beat a sure win
    }
  }

  outcome_cache[s] = bestScore;
  bestmove_cache[s] = bestMove;
  return bestScore;
}

static void board_string(int s, char* out){ // 10 bytes including '\0'
  for(int i=0;i<9;i++){
    int v=cell(s,i);
    out[i] = (v==0?'_': (v==1?'X':'O'));
  }
  out[9]='\0';
}

int main(int argc, char** argv){
  const char* outname = (argc >= 2 ? argv[1] : "ttt_dataset.data");
  FILE* fp = fopen(outname, "w");
  if(!fp){
    fprintf(stderr, "Error: cannot open output file '%s'\n", outname);
    return 1;
  }

  // init pow3 and caches
  pow3[0]=1; for(int i=1;i<10;i++) pow3[i]=pow3[i-1]*3;
  for(int i=0;i<NSTATE;i++){ outcome_cache[i]=2; bestmove_cache[i]=-1; }

  // Solve all legal states (memoization makes this fast)
  for(int s=0;s<NSTATE;s++){
    int turn;
    if(!legal_and_turn(s,&turn)) continue;
    if(turn != 0) minimax(s); // only compute for non-terminal states
  }

  // Emit .data (no header): board,player_to_move   ,outcome,best_move
  for(int s=0;s<NSTATE;s++){
    int turn;
    if(!legal_and_turn(s,&turn)) continue;
    if(turn == 0) continue; // skip terminal states

    char board[10]; board_string(s, board);
    int outc = outcome_cache[s];        // -1/0/+1 from current player's perspective
    int mv   = bestmove_cache[s];       // 0..8 (at least one exists for non-terminals)

    // One line per state
    fprintf(fp, "%s,%c,%d,%d\n", board, (turn==1?'X':'O'), outc, mv);
  }

  fclose(fp);
  return 0;
}
