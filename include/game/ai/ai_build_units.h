#ifndef GAME_AI_AI_BUILD_UNITS_H
#define GAME_AI_AI_BUILD_UNITS_H

#include "types.h"

BOOL AiTurn_TryBuildApc(void);
BOOL AiTurn_TryBuildTCopter(void);
BOOL AiTurn_TryBuildTCopterForFoot(void);
BOOL func_02091904(void);
BOOL AiTurn_TryBuildFootUnit(void);
u32 AiTurn_GetMechBuildPercent(void);
BOOL AiTurn_TryBuildCruiser(void);
BOOL func_02091c54(void);
BOOL AiTurn_TryBuildFighter(void);

#endif
