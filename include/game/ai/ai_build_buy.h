#ifndef GAME_AI_AI_BUILD_BUY_H
#define GAME_AI_AI_BUILD_BUY_H

#include "types.h"
#include "game/map_unit.h"

int AiUnit_GetRepairValue(int player, int coSlot, MapUnit* unit);
int Unit_GetValuePerHpNoPower(int player, int coSlot, int unitType);
void AiEndActions_Start(void);
void AiTurn_QueueBuildAtSite(int index);
u8 AiBuild_RollOrder(int unitType, u8 excluded);
int Ai_CountFootUnitsByUnk0c4(int skipSlot, int flag);
BOOL AiBuild_BuyAtSite(void);

#endif
