#ifndef GAME_AI_AI_UNIT_CLAIMS_H
#define GAME_AI_AI_UNIT_CLAIMS_H

#include "types.h"
#include "game/map_unit.h"

int AiUnit_CruiserCanReachSub(void);
BOOL AiUnit_ClaimLandmass(void);
void AiTurn_ReleaseCaptureClaims(void);
void AiTurn_ClaimCaptureTargets(void);
void AiUnit_MarkFinishingCapture(int action);
BOOL AiTile_HasStrongAllyInReach(int x, int y, MapUnit* target);
int func_020a3dfc(int unitType, int tile);

#endif
