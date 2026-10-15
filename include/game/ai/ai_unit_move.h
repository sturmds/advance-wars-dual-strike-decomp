#ifndef GAME_AI_AI_UNIT_MOVE_H
#define GAME_AI_AI_UNIT_MOVE_H

#include "types.h"

void AiUnit_MoveToSafestTile(void);
void AiUnit_AttackAdjacent(void);
void AiUnit_PlanAttack(void);
void Ai_PlanIndirectAttack(void);
void AiUnit_PickDirectAttack(void);
void AiUnit_WaitInPlace(void);
void AiUnit_HeadForObjectTarget(void);
void AiUnit_MoveTowardTile(int targetX, int targetY);
BOOL AiTile_IsSafeStop(s16 x, s16 y);

#endif
