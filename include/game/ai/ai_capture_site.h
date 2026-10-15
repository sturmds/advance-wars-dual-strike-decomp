#ifndef GAME_AI_AI_CAPTURE_SITE_H
#define GAME_AI_AI_CAPTURE_SITE_H

#include "types.h"
#include "game/map_unit.h"

void Ai_ClearActingUnitTarget(void);
BOOL AiUnit_CanReachCaptureSite(MapUnit* unit);
BOOL Ai_FindCaptureSiteNear(int x, int y, int distance, s16* property);
BOOL Ai_IsCaptureSiteTaken(int x, int y);
int Tile_GetDirection(int fromX, int fromY, int toX, int toY);
int func_020a507c(int x, int y, int range);
int func_020a50d4(int x, int y, int unitType, int movement);
BOOL Ai_HasEnemyWithinMoves(int x, int y, int unitType, int maxCost);

#endif
