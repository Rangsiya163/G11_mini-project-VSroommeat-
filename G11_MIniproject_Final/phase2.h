#ifndef PHASE2_H
#define PHASE2_H

#include "game_state.h"
#include "shop.h"

void renderDebateUI(const GameState *game, int round, const DebatePair *debate, const char *input, int remaining);
int playOneDebate(GameState *game, int round);
int runDebatePhase(GameState *game);

#endif