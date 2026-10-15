#ifndef GAME_AI_AI_SEA_UNITS_H
#define GAME_AI_AI_SEA_UNITS_H

#include "types.h"
#include "game/map_unit.h"

void AiSeaTransport_Move(void);
void AiBlackBoat_GoToUnitInNeed(void);
void AiBlackBoat_RepairDamagedUnit(void);
BOOL AiBlackBoat_IsBetterRepairTarget(MapUnit* best, MapUnit* unit);
int AiBlackBoat_GetRepairPriority(MapUnit* unit);
void AiSeaTransport_WaitNearPort(void);
void AiSeaTransport_GoToFootUnit(void);
int AiSeaTransport_PickBoardingTile(MapUnit* footUnit, s16* dest);
BOOL AiSeaTransport_FindPickupTile(int x, int y, s16* dest);
void AiSeaTransport_MoveLoaded(void);
BOOL AiTransport_SecondCargoHasTaskAt(int x, int y);
BOOL AiTransport_SecondCargoReachesTargetAt(int x, int y);
void func_020b4ca4(void);

#endif
