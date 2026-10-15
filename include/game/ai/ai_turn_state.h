#ifndef GAME_AI_AI_TURN_STATE_H
#define GAME_AI_AI_TURN_STATE_H

#include "types.h"
#include "game/map_unit.h"

void AiTurn_SetState(int state);
int AiTurn_GetState(void);
void AiTurn_Restart(void);
void AiTurn_Update(void);
void Player_ResetUnitsAiState(int player);
void MapUnit_ResetAiState(MapUnit* unit);

#endif
