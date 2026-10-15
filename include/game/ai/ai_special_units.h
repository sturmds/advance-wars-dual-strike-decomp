#ifndef GAME_AI_AI_SPECIAL_UNITS_H
#define GAME_AI_AI_SPECIAL_UNITS_H

#include "types.h"
#include "game/map_unit.h"

void AiOozium_MoveToTarget(void);
void AiBlackBomb_MoveTowardEnemy(void);
void AiBlackBomb_ChooseExplosionTile(void);
int AiBlackBomb_ScoreExplosionAt(int x, int y);
u32 AiBlackBomb_GetStartScore(void);
int AiBlackBomb_ScoreUnit(MapUnit* unit);

#endif
