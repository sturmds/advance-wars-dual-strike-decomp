#ifndef GAME_AI_AI_TURN_MARKS_H
#define GAME_AI_AI_TURN_MARKS_H

#include "types.h"
#include "game/map_unit.h"

void AiUnit_ClearSupplyWanted(MapUnit* unit);
void AiUnit_ChooseServiceWanted(MapUnit* unit);
void Ai_ClearTransportsSentToUnit(void);
void AiTurn_ClearUnitMarks(void);
void AiTurn_CountCaptureTargets(void);
void AiTurn_MarkReachableTiles(void);

#endif
