#ifndef GAME_AI_AI_UNITS_IN_RANGE_H
#define GAME_AI_AI_UNITS_IN_RANGE_H

#include "types.h"

BOOL AiTile_IsFreeForUnitType(int x, int y, int unitType);
BOOL Ai_TeamFootOrEnemyVehicleInRange(void);
int Ai_CountEnemyGroundUnitsInRange(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);
int Ai_CountTeamFootUnitsInRange(void);
int Ai_ScoreReachableUnclaimedEnemyProperties(void);
int Ai_ScoreReachableEnemyProperties(void);

#endif
