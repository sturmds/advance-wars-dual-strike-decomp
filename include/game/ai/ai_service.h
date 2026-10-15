#ifndef GAME_AI_AI_SERVICE_H
#define GAME_AI_AI_SERVICE_H

#include "types.h"

void AiBlackBoat_RepairUnit(void);
void AiClass3_SupplyUnit(void);
void AiUnit_SeekSupply(void);
void func_020af5f8(void);
void AiUnit_GoToSupply(void);
void AiUnit_HeadForRepairTile(void);
void AiUnit_JoinDamagedUnit(void);
void AiUnit_TryAttackBeforeRepair(void);
void AiUnit_LaunchSilo(void);
void AiUnit_RetreatToRepair(void);
void AiUnit_RetreatTowardSite(void);
BOOL AiUnit_NeedsService(void);

#endif
