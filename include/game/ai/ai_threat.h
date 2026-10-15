#ifndef GAME_AI_AI_THREAT_H
#define GAME_AI_AI_THREAT_H

#include "types.h"
#include "game/map_unit.h"

void Ai_AddIndirectEnemyThreat(u8 player);
void Ai_BuildThreatMaps(void);
void Ai_ClearThreatMaps(void);
void Ai_AddEnemyThreat(u8 player);
void Ai_AddObjectThreat(void);
void Ai_MarkObjectTileThreat(void);
void Ai_AddOoziumThreat(MapUnit* unit);
BOOL Ai_EnemyWithinReach(MapUnit* acting, MapUnit* enemy, int slack);
int Ai_RatedBaseDamage(int unitType, int targetType);
BOOL Ai_UnitUsesThreatMaps(int unitType);
int Ai_CountReachableAdjacentTiles(int x, int y);

#endif
