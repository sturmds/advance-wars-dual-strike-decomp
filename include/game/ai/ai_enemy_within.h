#ifndef GAME_AI_AI_ENEMY_WITHIN_H
#define GAME_AI_AI_ENEMY_WITHIN_H

#include "types.h"

BOOL Ai_HasOwnBaseWithin(int x, int y, int radius);
BOOL Ai_HasVisibleEnemyWithin(s16 x, s16 y, int radius);
BOOL Ai_HasEnemyWithin(s16 x, s16 y, int radius);

#endif
