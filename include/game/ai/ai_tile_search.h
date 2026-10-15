#ifndef GAME_AI_AI_TILE_SEARCH_H
#define GAME_AI_AI_TILE_SEARCH_H

#include "types.h"
#include "game/map_unit.h"

int AiUnit_TeammateOutdamages(MapUnit* target);
BOOL AiUnit_FindFiringTile(s16* pos);
void AiActingUnit_MarkAttackRange(int movement);
int AiUnit_GetUnloadMovement(MapUnit* unit);
int AiActingUnit_GetSearchMovement(void);
int AiUnit_GetSearchMovement(MapUnit* unit);
BOOL Ai_CanEndMoveAt(s16 x, s16 y);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
BOOL Ai_FindNearestEnemyHqTile(s16* out);
BOOL AiUnit_FindStandTileNextTo(int targetX, int targetY, s16* pos);
BOOL FindAttackTile(int targetX, int targetY, s16* pos);
int AiTile_ScoreAttackFrom(int x, int y);

#endif
