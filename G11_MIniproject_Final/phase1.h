#ifndef PHASE1_H
#define PHASE1_H

#include "game_state.h"
#include "shop.h"

void spawnWord(FallingWord words[]);
int findTargetWord(FallingWord words[]);
void drawPhase1(const GameState *game, FallingWord words[], int timeLeft);
int runTypingPhase(GameState *game);

#endif