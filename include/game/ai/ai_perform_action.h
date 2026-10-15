#ifndef GAME_AI_AI_PERFORM_ACTION_H
#define GAME_AI_AI_PERFORM_ACTION_H

#include "types.h"

void AiPlayer_PerformAction(int action);
void AiUnit_PerformAction(s16 x, s16 y, u8 action, u8 target, u8 arg);
void func_0208fda8(void);
void AiTurn_RunQueue(void);
BOOL func_02090154(void);

#endif
