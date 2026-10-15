#ifndef GAME_AI_AI_BUILD_TRANSPORT_H
#define GAME_AI_AI_BUILD_TRANSPORT_H

#include "types.h"

BOOL AiTurn_TryBuildLander(void);
BOOL AiTurn_TryBuildLanderNearSite(void);
BOOL AiTurn_TryBuildBlackBoat(void);
BOOL AiTurn_TryBuildBlackBoatTransport(void);
BOOL AiTurn_TryBuildBlackBoatNearSite(void);
int Ai_CountArmedShips(void);
int Ai_CountVehiclesWantingTransport(void);
int Ai_CountFootWantingTransport(void);
int Ai_CountEmptyTransports(int type);
void RangeMap_SpreadOverSea(u8 mark);
BOOL Lander_CanEnterTile(int tile);

#endif
