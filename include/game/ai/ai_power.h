#ifndef GAME_AI_AI_POWER_H
#define GAME_AI_AI_POWER_H

#include "types.h"

BOOL func_02099528(void);
int AiTurn_IsPhase3(void);
void AiPlayer_TryUsePower(void);
BOOL AiPlayer_FrontCoNearerToSuper(void);
BOOL AiCo_FrontCloserToSuper(void);
int AiCo_TagRank(int coId);
void AiTurn_RunPowerStep(void);

#endif
