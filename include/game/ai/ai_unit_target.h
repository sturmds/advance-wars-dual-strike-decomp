#ifndef GAME_AI_AI_UNIT_TARGET_H
#define GAME_AI_AI_UNIT_TARGET_H

#include "types.h"
#include "game/map_unit.h"

void AiUnit_HandleUnitAtTarget(int x, int y);
void AiUnit_AttackFromPlaceThenMove(MapUnit* mover);
void AiUnit_PickDirectAttackInReachOf(MapUnit* other);
void Ai_SwitchActingUnit(int fromSlot, int toSlot);
void AiUnit_WaitToAttack(int x, int y, u8* action, u8* target);
int AiUnit_PickIndirectTarget(int x, int y);
int AiUnit_FindBestAdjacentTarget(int x, int y);
void AiUnit_UpgradeWaitAction(s16 x, s16 y, u8* action, u8* target, u8* arg);
void AiUnit_AdjustAction(s16 x, s16 y, u8* action, u8* target, u8* arg);

#endif
